#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "emxapi_comm_ringbuf_thread.h"
#include "emxapi_defines.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <signal.h>
#include <sched.h>
#include <sys/sysinfo.h>

#include "emx_ringbuf.inl"

void run_main(void *core);

int emx_core;
pthread_t thread[EMX_NUM_CORES];

#define RB_COUNT ((EMX_NUM_CORES)*(EMX_NUM_CORES-1))

emx_ringbuf_t *emx_rb;

void sigterm_handler(int signal, siginfo_t *info, void *_unused) {
	fprintf(stderr, "Received SIGTERM from process with pid = %u\n", info->si_pid);
	EMX_Finalize();
#ifdef EMX_PROF
	EMX_ProfFinalize();
#endif
#ifdef KINECT
	KinectInterfaceQuit();
#endif
#ifdef SDL
	SDLInterfaceQuit();
#endif
	exit(0);
}

#ifdef __linux

static void emx_setaffinity(int core) {
	cpu_set_t mask;
	int status;

	if (core >= sysconf(_SC_NPROCESSORS_ONLN)) {
		return;
	}

	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
	status = pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
	if (status != 0) {
		perror("sched_setaffinity");
	}
}

#elif defined(__CYGWIN__ )

#include <windows.h>

static void emx_setaffinity(int core) {
	int cpu = core * 2;

	DWORD res = SetThreadIdealProcessor(GetCurrentThread(), cpu);
	if (res == -1) {
		fprintf(stderr, "Affinity: %d\n", cpu);
		fprintf(stderr, "Warning: SetThreadIdealProcessor failed (code 0x%X)\n", GetLastError());
	}

	res = SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	if (res == 0) {
		fprintf(stderr, "Warning: SetPriorityClass failed (code 0x%X)\n", GetLastError());
	}
}

#else

static void emx_setaffinity(int core) {
}

#endif

int EMX_Init(int argc, char *argv[]) {
	int counter = 0;
	int i, j, result;

	emx_rb = (emx_ringbuf_t*) mmap(0, sizeof(emx_ringbuf_t) * RB_COUNT,
			PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	for (i = 0; i < EMX_NUM_CORES; ++i) {
		for (j = 0; j < EMX_NUM_CORES; ++j) {
			if (i != j) {
				void *buf = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE,
						MAP_SHARED | MAP_ANONYMOUS, -1, 0);
				emx_ringbuf_init(&emx_rb[EMX_SndRcv2Buf(i, j)],
				EMX_RINGBUF_SIZE, buf);
			}
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	fflush(stdout);

#ifdef __linux
	struct sigaction action = {0};
	action.sa_sigaction = sigterm_handler;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &action, NULL);
#endif

	return 0;
}

void* start_thread(void *core) {
	emx_setaffinity((intptr_t)core);
	run_main(core);
	return 0;
}

void EMX_Finalize() {
}

void EMX_Run(int core) {
	intptr_t i;
	thread[0] = pthread_self();
	for (i = 1; i < EMX_NUM_CORES; ++i) {
		if (pthread_create(&thread[i], NULL, start_thread, (void*) i)) {
			fprintf(stderr, "Error creating thread\n");
		}
	}
	run_main(0);
}

void EMX_Join(int core) {
	int i;
	pthread_t self = pthread_self();

	if (self == thread[0]) {
		for (i = 1; i < EMX_NUM_CORES; i++) {
			int err = pthread_join(thread[i], NULL);

			if (err) {
				fprintf(stderr, "Error joining thread %d: %d\n", i, err);
			}
		}
	}
}

void EMX_SyncCores(int core) {
	int src, dst;

	src = core == EMX_NUM_CORES - 1 ? 0 : core + 1;
	dst = core == 0 ? EMX_NUM_CORES - 1 : core - 1;

	if (core == EMX_NUM_CORES - 1) {
		EMX_Signal(core, dst, -1, -1);
	}

	EMX_Wait(src, core, -1, -1);

	if (core != EMX_NUM_CORES - 1) {
		EMX_Signal(core, dst, -1, -1);
	}
}

