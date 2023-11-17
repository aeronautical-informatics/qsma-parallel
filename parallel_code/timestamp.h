/*
 * File:   timestamp.h
 * Author: Trevor Welsby
 *
 * Created on 3. September 2020, 14:11
 */

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <stdint.h>
#include <time.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif




static inline uint32_t EMX_TimeRef() {
#if _POSIX_TIMERS>0
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return tp.tv_sec * 1000000000 + tp.tv_nsec;
#else
	return clock();
#endif
}

static inline uint32_t EMX_IntervalTime(uint32_t interval) {
#if _POSIX_TIMERS>0
	return interval;
#else
	return interval * (1000000000/CLOCKS_PER_SEC);
#endif
}

static inline uint32_t EMX_Timestamp() {
	return EMX_IntervalTime(EMX_TimeRef());
}

static inline uint32_t EMX_ElapsedTime(uint32_t ref) {
	return EMX_IntervalTime(EMX_TimeRef() - ref);
}



#ifdef __cplusplus
}
#endif

#endif //TIMESTAMP_H
