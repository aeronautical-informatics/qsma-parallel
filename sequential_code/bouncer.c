///* -------------------------- FILE PROLOGUE -------------------------------- */
//
///**
// *****************************************************************************
// * @copyright
// * 	(C) Copyright SYSGO GmbH.
// * 	Mainz, Germany
// * 	All rights reserved.
// *****************************************************************************
// */
//
///* ------------------------- FILE INCLUSION -------------------------------- */
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <string.h>
////#include <fcntl.h>
//#include <sys/fcntl.h>
//#include <errno.h>
//#include <sched.h>
//#include <unistd.h>
//#include <pthread.h>
//#include <mqueue2.h>
//#include <sys/stat.h>
//#include <time.h>
//
//#include "errors.h"
//
///* ------------------- MACRO / CONSTANT DEFINITIONS ------------------------ */
//
//#define RTT_TIME	10			/* Round-trip time for one timing loop. */
//#define MAXSZ		128			/* Maximum message size. */
//
///* ----------------------- OBJECT DECLARATIONS ----------------------------- */
//
//int fd_in;						/* /qport/FIFOrx (input port) */
//int fd_out;						/* /qport/FIFOtx (output port) */
//mqd_t mq_in;					/* Message Q for input data from fd_in */
//mqd_t mq_out;					/* Message Q for output data to fd_out */
//
///* -------------------------- LOCAL FUNCTIONS ------------------------------ */
//
///* Writer thread: Read data from mq_out and write it to fd_out. */
//static void *thread_w(void *unused __unused)
//{
//	char buf[MAXSZ];
//	ssize_t size;
//	ssize_t r;
//
//	for (;;) {
//		if ((size = mq_receive(mq_out, buf, sizeof(buf), 0)) == -1)
//			perror("thread_w: mq_receive");
//		else {
//			if ((r = write(fd_out, buf, size)) < 0)
//				perror("thread_w: write");
//			else if (r != size)
//				fprintf(stderr, "thread_w: write short %zd/%zd\n", r, size);
//		}
//	}
//	return 0;
//}
//
///* Reader thread: Read data from fd_in and write it to mq_in. */
//static void *thread_r(void *unused __unused)
//{
//	char buf[MAXSZ];
//	int r;
//
//	for (;;) {
//		if ((r = read(fd_in, buf, sizeof(buf))) <= 0) {
//			if (r == 0 || errno == EAGAIN)
//				fprintf(stderr, "rA ");
//			else
//				perror("read");
//		} else if (mq_send(mq_in, buf, r, 0) == -1)
//			perror("thread_r: mq_send");
//	}
//	return 0;
//}
//
///* ------------------ GLOBAL FUNCTION DEFINITIONS -------------------------- */
//int main(void)
//{
//	sleep(5);
//	printf("Here3");
//
//	pthread_attr_t attr;
//	pthread_t tid_w;
//	pthread_t tid_r;
//	struct sched_param sc;
//	struct mq_attr mattr;
//	struct stat sb;
//	struct timespec ts_start;
//	struct timespec ts_end;
//	struct timespec d;
//	unsigned long n;
//	unsigned long dump;
//	ssize_t size;
//	char buf[MAXSZ];
//	int policy;
//	int r;
//
//	/* Try to open ports. */
//	if ((fd_out = open("/qport/FIFOtx", O_WRONLY)) < 0)
//		errno_abort("open FIFOtx");
//	if ((fd_in = open("/qport/FIFOrx", O_RDONLY)) < 0)
//		errno_abort("open FIFOrx");
//
//	/* Retrieve input port configuration (block size, number of blocks). */
//	if (fstat(fd_in, &sb) < 0)
//		errno_abort(" stat fd_in");
//	printf("Bouncer: fd_in blocks %d blksize %d\n",
//		   sb.st_blocks, sb.st_blksize);
//
//	/* Limit block size to MAXSZ. */
//	if (sb.st_blksize > MAXSZ)
//		sb.st_blksize = MAXSZ;
//
//	/* Set up input message queue. */
//	mattr.mq_flags = 0;
//	mattr.mq_maxmsg = sb.st_blocks;
//	mattr.mq_msgsize = sb.st_blksize;
//
//	if ((mq_in =
//		 mq_open("/mq_in", O_CREAT | O_RDWR, 0666, &mattr)) == (mqd_t)-1)
//		errno_abort("mq_open mq_in");
//
//	/* Retrieve output port configuration (block size, number of blocks). */
//	if (fstat(fd_out, &sb) < 0)
//		errno_abort(" stat fd_out");
//	printf("Bouncer: fd_out blocks %d blksize %d\n",
//		   sb.st_blocks, sb.st_blksize);
//
//	if (sb.st_blksize > MAXSZ)
//		sb.st_blksize = MAXSZ;
//
//	if (mattr.mq_msgsize != (long)sb.st_blksize) {
//		fprintf(stderr, "Bouncer: Port block sizes don't match.\n");
//		exit(EXIT_FAILURE);
//	}
//
//	/* Set up output message queue. */
//	mattr.mq_flags = 0;
//	mattr.mq_maxmsg = sb.st_blocks;
//	mattr.mq_msgsize = sb.st_blksize;
//	if ((mq_out =
//		 mq_open("/mq_out", O_CREAT | O_RDWR, 0666, &mattr)) == (mqd_t)-1)
//		errno_abort("mq_open mq_out");
//
//	/* Create reader and writer thread. */
//	pthread_attr_init(&attr);
//	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
//	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
//#if 1
//	/* Run threads at higher priority than main thread. */
//	pthread_getschedparam(pthread_self(), &policy, &sc);
//	++sc.sched_priority;
//	pthread_attr_setschedparam(&attr, &sc);
//#endif
//	if ((r = pthread_create(&tid_r, &attr, thread_r, 0)))
//		err_abort(r, "pthread_create thread_r");
//	if ((r = pthread_create(&tid_w, &attr, thread_w, 0)))
//		err_abort(r, "pthread_create thread_w");
//
//	puts("Bouncer: Threads created");
//
//
//	dump = 0;
//	clock_gettime(CLOCK_REALTIME, &ts_start);
//	for (n = 0;; n++) {
//		// Read messages from mq_in and sedn them back on mq_out.
//		if ((size = mq_receive(mq_in, buf, sizeof(buf), 0)) != -1) {
//			if (mq_send(mq_out, buf, size, 0) == -1)
//				perror("mq_send");
//		} else
//			perror("mq_receive");
//
//		if (dump == 0) {
//			/* Calibration: While dump == 0, calculate how many messages can be sent out in about 1seconds.
//			 * If RTT_TIME seconds passed since start, set dump to the approx. current message count (rounded down).
//             */
//			clock_gettime(CLOCK_REALTIME, &ts_end);
//			if (ts_end.tv_sec - ts_start.tv_sec > RTT_TIME)
//				dump = (n / 100) * 100;
//		} else if (n >= dump) {
//			// Timing loop: after dump messages, print elapsed time.
//			clock_gettime(CLOCK_REALTIME, &ts_end);
//			d.tv_sec = 1;
//			//d.tv_sec = ts_end.tv_sec - ts_start.tv_sec;
//			if ((d.tv_nsec = ts_end.tv_nsec - ts_start.tv_nsec) < 0) {
//				++d.tv_sec;
//				d.tv_nsec += 1000 * 1000 * 1000;
//			}
//
//			printf("\n Received data\n %s ", buf);
//
//			ts_start = ts_end;
//			n = 0;
//		}
//	}
//	exit(EXIT_SUCCESS);
//}
