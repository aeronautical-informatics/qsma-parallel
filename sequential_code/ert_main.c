/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'QSMA_TAWS'.
 *
 * Model version                  : 1.49
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Wed Oct 20 15:31:25 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
/* ------------------------- FILE INCLUSION -------------------------------- */
#include "FLTA_STEP.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>

#include "errors.h"

#include <stddef.h>
#include <stdio.h>              /* This ert_main.c example uses printf/fflush */
#include "QSMA_TAWS.h"          /* Model's header file */
#include "rtwtypes.h"

/* ------------------- MACRO / CONSTANT DEFINITIONS (bouncer)------------------------ */

#define RTT_TIME	10			/* Round-trip time for one timing loop. */
#define MAXSZ		128			/* Maximum message size. */
#define MAXLINELEN	64         /* Maximum amount of data we're going to write. */

#define PITCH			0
#define HEIGHT			1
#define CLIMB_RATE		2

/* ----------------------- OBJECT DECLARATIONS ----------------------------- */

int fd_in;						/* /qport/FIFOrx (input port) */
int fd_out;						/* /qport/FIFOtx (output port) */
mqd_t mq_in;					/* Message Q for input data from fd_in */
mqd_t mq_out;					/* Message Q for output data to fd_out */


/* -------------------------- LOCAL FUNCTIONS ------------------------------ */

/* Writer thread: Read data from mq_out and write it to fd_out. */
static void *thread_w(void *unused)
{
	char buf[MAXSZ];
	ssize_t size;
	ssize_t r;

	for (;;) {
		if ((size = mq_receive(mq_out, buf, sizeof(buf), 0)) == -1)
			perror("thread_w: mq_receive");
		else {
			if ((r = write(fd_out, buf, size)) < 0)
				perror("thread_w: write");
			else if (r != size)
				fprintf(stderr, "thread_w: write short %zd/%zd\n", r, size);
		}
	}
	return 0;
}

/* Reader thread: Read data from fd_in and write it to mq_in. */
static void *thread_r(void *unused)
{
	char buf[MAXSZ];
	int r;

	for (;;) {
		if ((r = read(fd_in, buf, sizeof(buf))) <= 0) {
			if (r == 0 || errno == EAGAIN)
				fprintf(stderr, "rA ");
			else
				perror("read");
		} else if (mq_send(mq_in, buf, r, 0) == -1)
			perror("thread_r: mq_send");
	}
	return 0;
}

void ExternalComm(void)
{
		pthread_attr_t attr;
		pthread_t tid_w;
		pthread_t tid_r;
		struct sched_param sc;
		struct mq_attr mattr;
		struct stat sb;
		struct timespec ts_start;
		struct timespec ts_end;
		struct timespec d;
		unsigned long n;
		unsigned long dump;
		ssize_t size;
		char buf[MAXSZ];
		int policy;
		int r;

		/* Try to open ports. */
		if ((fd_out = open("/qport/FIFOtx", O_WRONLY)) < 0)
			errno_abort("open FIFOtx");
		if ((fd_in = open("/qport/FIFOrx", O_RDONLY)) < 0)
			errno_abort("open FIFOrx");

		/* Retrieve input port configuration (block size, number of blocks). */
		if (fstat(fd_in, &sb) < 0)
			errno_abort(" stat fd_in");
		printf("Bouncer: fd_in blocks %ld blksize %ld\n",
			   sb.st_blocks, sb.st_blksize);

		/* Limit block size to MAXSZ. */
		if (sb.st_blksize > MAXSZ)
			sb.st_blksize = MAXSZ;

		/* Set up input message queue. */
		mattr.mq_flags = 0;
		mattr.mq_maxmsg = sb.st_blocks;
		mattr.mq_msgsize = sb.st_blksize;

		if ((mq_in =
			 mq_open("/mq_in", O_CREAT | O_RDWR, 0666, &mattr)) == (mqd_t)-1)
			errno_abort("mq_open mq_in");

		/* Retrieve output port configuration (block size, number of blocks). */
		if (fstat(fd_out, &sb) < 0)
			errno_abort(" stat fd_out");
		printf("Bouncer: fd_out blocks %ld blksize %ld\n",
			   sb.st_blocks, sb.st_blksize);

		if (sb.st_blksize > MAXSZ)
			sb.st_blksize = MAXSZ;

		if (mattr.mq_msgsize != (long)sb.st_blksize) {
			fprintf(stderr, "Bouncer: Port block sizes don't match.\n");
			exit(EXIT_FAILURE);
		}

		/* Set up output message queue. */
		mattr.mq_flags = 0;
		mattr.mq_maxmsg = sb.st_blocks;
		mattr.mq_msgsize = sb.st_blksize;
		if ((mq_out =
			 mq_open("/mq_out", O_CREAT | O_RDWR, 0666, &mattr)) == (mqd_t)-1)
			errno_abort("mq_open mq_out");

		/* Create reader and writer thread. */
		pthread_attr_init(&attr);
		pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
		pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	#if 1
		/* Run threads at higher priority than main thread. */
		pthread_getschedparam(pthread_self(), &policy, &sc);
		++sc.sched_priority;
		pthread_attr_setschedparam(&attr, &sc);
	#endif
		if ((r = pthread_create(&tid_r, &attr, thread_r, 0)))
			err_abort(r, "pthread_create thread_r");
		if ((r = pthread_create(&tid_w, &attr, thread_w, 0)))
			err_abort(r, "pthread_create thread_w");

		puts("Bouncer: Threads created");


		dump = 0;
		clock_gettime(CLOCK_REALTIME, &ts_start);
		for (n = 0;; n++) {
			// Read messages from mq_in and sedn them back on mq_out.
			if ((size = mq_receive(mq_in, buf, sizeof(buf), 0)) != -1) {
				if (mq_send(mq_out, buf, size, 0) == -1)
					perror("mq_send");
			} else
				perror("mq_receive");

			if (dump == 0) {
				/* Calibration: While dump == 0, calculate how many messages can be sent out in about 1seconds.
				 * If RTT_TIME seconds passed since start, set dump to the approx. current message count (rounded down).
	             */
				clock_gettime(CLOCK_REALTIME, &ts_end);
				if (ts_end.tv_sec - ts_start.tv_sec > RTT_TIME)
					dump = (n / 100) * 100;
			} else if (n >= dump) {
				// Timing loop: after dump messages, print elapsed time.
				clock_gettime(CLOCK_REALTIME, &ts_end);
				d.tv_sec = 1;
				//d.tv_sec = ts_end.tv_sec - ts_start.tv_sec;
				if ((d.tv_nsec = ts_end.tv_nsec - ts_start.tv_nsec) < 0) {
					++d.tv_sec;
					d.tv_nsec += 1000 * 1000 * 1000;
				}

				printf("\n Received data\n %s ", buf);

				ts_start = ts_end;
				n = 0;
			}
		}
		exit(EXIT_SUCCESS);
}



int_T main(int_T argc, const char *argv[])
{
	sleep(5);
	//ExternalComm();
	printf("ERT_MAIN STARTS\n");

	//Initialize the FLTA DATA
	FLTA_DATA.fileName="/rfs/data";
	init();

	while (1)
	{
		//Initialize the FLTA DATA
		FLTA_DATA.position.alt=400;
		FLTA_DATA.position.lon=-20.715083; //X;
		FLTA_DATA.position.lat=64.719444; //Y;
		FLTA_DATA.Vground=300;
		FLTA_DATA.VerticalSpeed=0;
		FLTA_DATA.RTC=500 * 0.3048; //clearance required in meters
		FLTA_DATA.TrueTrack=135; //degrees
		FLTA_DATA.YawRate=0.05; //degrees

		fltastep();
	}
}

//////////////////TAWS manual and auto-Generated Code///////////////////////////////////////////////////////
/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  /* Test Cases */
//  real_T a_p[]  = {-1, -0.8, -0.5, -0.3, -0.2, 0.2, -0.1};
//  int32_T c_c[] = {8000, 8000, 8000, 8000, 8000, 8000, 8000};
//  int32_T b_h[] = {5000, 4000, 3000, 2000, 1000, 800, 500};

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(QSMA_TAWS_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */
//  for(int j=0; j<6; j++)
  for(;;)
  {
//	  QSMA_TAWS_U.pitch = a_p[j];
//	  QSMA_TAWS_U.height_ground = b_h[j];
//	  QSMA_TAWS_U.climb_rate = c_c[j];


	  /* Step the model */
	  QSMA_TAWS_step();

	  /* Get model outputs here */
	   //printf("Test Case:%d ---> Output:%d \n",QSMA_TAWS_Y.currentalert);
//	  printf("Test Case ---> Output:%d \n", QSMA_TAWS_Y.currentalert);

  }




  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/*
 * The example "main" function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific.  This example
 * illustrates how you do this relative to initializing the model.
 */

//int_T main(int_T argc, const char *argv[])
//{
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
//	static boolean_T OverrunFlag = false;
//	real_T a_p[6] ;
//    int32_T c_c[6] ;
//	int32_T b_h[6] ;
//
//	float fdmData[100];
//	int CB[100];
//
//
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
//	printf("Receiver: fd_in blocks %d blksize %d\n", sb.st_blocks, sb.st_blksize);
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
//	if ((mq_in = mq_open("/mq_in", O_CREAT | O_RDWR, 0666, &mattr)) == (mqd_t)-1)
//		errno_abort("mq_open mq_in");
//
//	/* Retrieve output port configuration (block size, number of blocks). */
//	if (fstat(fd_out, &sb) < 0)
//		errno_abort(" stat fd_out");
//	printf("Bouncer: fd_out blocks %d blksize %d\n",sb.st_blocks, sb.st_blksize);
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
//	if ((mq_out = mq_open("/mq_out", O_CREAT | O_RDWR, 0666, &mattr)) == (mqd_t)-1)
//		errno_abort("mq_open mq_out");
//
//	/* Create reader and writer thread. */
//	pthread_attr_init(&attr);
//	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
//	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
//
//	#if 1
//		/* Run threads at higher priority than main thread. */
//		pthread_getschedparam(pthread_self(), &policy, &sc);
//		++sc.sched_priority;
//		pthread_attr_setschedparam(&attr, &sc);
//	#endif
//
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
//	for (n = 0;; n++)
//	{
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
//			 */
//			clock_gettime(CLOCK_REALTIME, &ts_end);
//			if (ts_end.tv_sec - ts_start.tv_sec > RTT_TIME)
//				dump = (n / 100) * 100;
//		} else if (n >= dump) {
//			// Timing loop: after dump messages, print elapsed time.
//			clock_gettime(CLOCK_REALTIME, &ts_end);
//			d.tv_sec = 1;
//			if ((d.tv_nsec = ts_end.tv_nsec - ts_start.tv_nsec) < 0) {
//				++d.tv_sec;
//				d.tv_nsec += 1000 * 1000 * 1000;
//			}
//
//			fflush(stdout);
//			sscanf(buf, "%f %f %d \n",&fdmData[PITCH], &fdmData[HEIGHT], &CB[CLIMB_RATE]);
//
//			/* Print data to screen for monitoring purposes */
//			printf("\n Flight parameters received \n PITCH: %.3f\t HEIGHT: %.3f\t CLIMB_RATE: %d\n",fdmData[PITCH], fdmData[HEIGHT],CB[CLIMB_RATE]);
//
//			/*begin rt_OneStep*/
//			for(int i=0; i<6; i++)
//			{
//				 a_p[i+1]  = fdmData[PITCH];
//				 c_c[i+1]  = fdmData[HEIGHT];
//				 b_h[i+1]  = CB[CLIMB_RATE];
//			}
//
//			  /* Check for overrun */
//			  if (OverrunFlag) {
//				rtmSetErrorStatus(QSMA_TAWS_M, "Overrun");
//				return 0;
//			  }
//
//			  OverrunFlag = true;
//
//			  for(int j=0; j<6; j++)
//			  {
//				  QSMA_TAWS_U.pitch = a_p[j];
//				  QSMA_TAWS_U.height_ground = b_h[j];
//				  QSMA_TAWS_U.climb_rate = c_c[j];
//
//				  /* Step the model */
//				  QSMA_TAWS_step();
//
//				  /* Get model outputs here */
//				  printf("Test Case:%d ---> Output:%d \n",j+1, QSMA_TAWS_Y.currentalert);
//
//			  }
//
//			  /* Indicate task complete */
//			  OverrunFlag = false;
//
//			/*end rt_OneStep()*/
//
//
//			ts_start = ts_end;
//			n = 0;
//		}
////			rt_OneStep();
//	}
//
//
//  /* Unused arguments */
//  (void)(argc);
//  (void)(argv);
//
//  /* Initialize model */
//  QSMA_TAWS_initialize();
//
//  /* Attach rt_OneStep to a timer or interrupt service routine with
//   * period 0.5 seconds (the model's base sample time) here.  The
//   * call syntax for rt_OneStep is
//   *
//   * rt_OneStep();
//   */
//
//  printf("Warning: The simulation will run forever. "
//         "Generated ERT main won't simulate model step behavior. "
//         "To change this behavior select the 'MAT-file logging' option.\n");
//  fflush((NULL));
//  while (rtmGetErrorStatus(QSMA_TAWS_M) == (NULL)) {
//    /*  Perform other application tasks here */
//  }
//  exit(EXIT_SUCCESS);
//  /* Disable rt_OneStep() here */
//
//  /* Terminate model */
//  QSMA_TAWS_terminate();
//  return 0;
//}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
