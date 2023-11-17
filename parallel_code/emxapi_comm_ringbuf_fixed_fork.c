#define _GNU_SOURCE

#include "emxapi_comm_ringbuf_fixed_fork.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <sched.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>

void emx_ringbuf_wait() {};

#define __USE_GNU 1

void run_main(int core);

static int pids[EMX_NUM_CORES] = {0};
int emx_core;

#ifdef __linux
#include <sys/sysinfo.h>
static void setaffinity(int rank) {
	cpu_set_t mask;
	int status;

	if (rank >= get_nprocs_conf()) {
		return;
	}

	CPU_ZERO(&mask);
	CPU_SET(rank, &mask);
	status = sched_setaffinity(0, sizeof(mask), &mask);
	if (status != 0) {
		perror("sched_setaffinity");
	}
}

#elif defined(__CYGWIN__ )

#include <windows.h>

static void setaffinity(int rank) {
	int cpu = rank * 2;

	DWORD res = SetThreadIdealProcessor(GetCurrentThread(), cpu);
	if (res == -1) {
		fprintf(stderr, "Affinity: %d\n", cpu);
		fprintf(stderr, "Warning: SetThreadIdealProcessor failed (code 0x%X)\n", GetLastError());
	}
}

#else

static void setaffinity(int rank) {
}

#endif

static void sigterm_handler(int signal, siginfo_t *info, void *_unused) {
	fprintf(stderr, "Received SIGTERM from process with pid = %u\n", info->si_pid);
	EMX_Finalize(0);

#ifdef EMX_PROF
	EMX_ProfFinalize();
#endif

	exit(0);
}

int EMX_Init(int argc, char *argv[]) {
	int i;

	emx_rb_buf_0_1 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_0_1 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_0_1 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_0_1 = 0;
	*emx_wr_pos_0_1 = 0;
	emx_rb_buf_0_2 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_0_2 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_0_2 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_0_2 = 0;
	*emx_wr_pos_0_2 = 0;
	emx_rb_buf_0_3 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_0_3 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_0_3 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_0_3 = 0;
	*emx_wr_pos_0_3 = 0;
	emx_rb_buf_1_0 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_1_0 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_1_0 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_1_0 = 0;
	*emx_wr_pos_1_0 = 0;
	emx_rb_buf_1_2 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_1_2 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_1_2 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_1_2 = 0;
	*emx_wr_pos_1_2 = 0;
	emx_rb_buf_1_3 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_1_3 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_1_3 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_1_3 = 0;
	*emx_wr_pos_1_3 = 0;
	emx_rb_buf_2_0 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_2_0 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_2_0 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_2_0 = 0;
	*emx_wr_pos_2_0 = 0;
	emx_rb_buf_2_1 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_2_1 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_2_1 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_2_1 = 0;
	*emx_wr_pos_2_1 = 0;
	emx_rb_buf_2_3 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_2_3 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_2_3 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_2_3 = 0;
	*emx_wr_pos_2_3 = 0;
	emx_rb_buf_3_0 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_3_0 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_3_0 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_3_0 = 0;
	*emx_wr_pos_3_0 = 0;
	emx_rb_buf_3_1 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_3_1 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_3_1 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_3_1 = 0;
	*emx_wr_pos_3_1 = 0;
	emx_rb_buf_3_2 = mmap(0, EMX_RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_rd_pos_3_2 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	emx_wr_pos_3_2 = mmap(0, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*emx_rd_pos_3_2 = 0;
	*emx_wr_pos_3_2 = 0;

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	fflush(stdout);

	pids[0] = getpid();
	emx_core = 0;

	for (i = 1; i < EMX_NUM_CORES; ++i) {
		pid_t pid = fork();

		if (pid != 0) {
			pids[i] = pid;
		}

		if (pid == 0) {
			emx_core = i;
			break;
		}
	}

#ifdef __linux
	struct sigaction action = {0};
	action.sa_sigaction = sigterm_handler;
	action.sa_flags = SA_SIGINFO;

	sigaction(SIGTERM, &action, NULL);
#endif

	setaffinity(emx_core);

	return emx_core;
}

void EMX_Finalize() {
}

void EMX_Run() {
	run_main(emx_core);
}

void EMX_Join() {
	int i;

	if (emx_core == 0) {
		for (i = 1; i < EMX_NUM_CORES; ++i) {
			waitpid(pids[i], NULL, 0);
		}
	} else {
		exit(0);
	}
}

