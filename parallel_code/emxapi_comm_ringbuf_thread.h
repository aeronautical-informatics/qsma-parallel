#ifndef EMXAPI_COMM_RINGBUF_THREAD_H
#define EMXAPI_COMM_RINGBUF_THREAD_H

#include <stdint.h>
#include "emx_ringbuf_defines.h"
#include "emx_ringbuf.h"
#include <string.h>
#include <pthread.h>

extern emx_ringbuf_t* emx_rb;

static inline int EMX_SndRcv2Buf(int src, int dst) {
	return src * (EMX_NUM_CORES-1) + dst - (dst > src);
}

int EMX_Init(int argc, char *argv[]);

void EMX_Finalize();

static inline void EMX_Send(int src, int dst, int id, int index, const void *buf, int size) {
	emx_ringbuf_write(&emx_rb[EMX_SndRcv2Buf(src, dst)], &emx_rb[EMX_SndRcv2Buf(dst, src)], buf, size);
}

static inline void EMX_Recv(int src, int dst, int id, int index, void *buf, int size) {
	emx_ringbuf_read(&emx_rb[EMX_SndRcv2Buf(src, dst)], &emx_rb[EMX_SndRcv2Buf(dst, src)], buf, size);
}

static inline void EMX_Send8(int src, int dst, int id, int index, uint8_t val) {
	emx_ringbuf_write8(&emx_rb[EMX_SndRcv2Buf(src, dst)], val);
}

static inline uint8_t EMX_Recv8(int src, int dst, int id, int index) {
	return emx_ringbuf_read8(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
}

static inline void EMX_Send16(int src, int dst, int id, int index, uint16_t val) {
	emx_ringbuf_write16(&emx_rb[EMX_SndRcv2Buf(src, dst)], val);
}

static inline uint16_t EMX_Recv16(int src, int dst, int id, int index) {
	return emx_ringbuf_read16(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
}

static inline void EMX_Send32(int src, int dst, int id, int index, uint32_t val) {
	emx_ringbuf_write32(&emx_rb[EMX_SndRcv2Buf(src, dst)], val);
}

static inline uint32_t EMX_Recv32(int src, int dst, int id, int index) {
	return emx_ringbuf_read32(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
}

static inline void EMX_Send64(int src, int dst, int id, int index, uint64_t val) {
	emx_ringbuf_write64(&emx_rb[EMX_SndRcv2Buf(src, dst)], val);
}

static inline uint64_t EMX_Recv64(int src, int dst, int id, int index) {
	return emx_ringbuf_read64(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
}

static inline void EMX_Send32F(int src, int dst, int id, int index, float val) {
	emx_ringbuf_write32(&emx_rb[EMX_SndRcv2Buf(src, dst)], *(uint32_t*)&val);
}

static inline float EMX_Recv32F(int src, int dst, int id, int index) {
	uint32_t r = emx_ringbuf_read32(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
	return *(float*)&r;
}

static inline void EMX_Send64F(int src, int dst, int id, int index, double val) {
	emx_ringbuf_write64(&emx_rb[EMX_SndRcv2Buf(src, dst)], *(uint64_t*)&val);
}

static inline double EMX_Recv64F(int src, int dst, int id, int index) {
	uint64_t r = emx_ringbuf_read64(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
	return *(double*)&r;
}

static inline int EMX_RecvSync(int src, int dst, int id, int index) {
	return EMX_Recv8(src, dst, id, index);
}

static inline void EMX_SendSync(int src, int dst, int id, int index, int8_t sync) {
	EMX_Send8(src, dst, id, index, sync);
}

static inline void EMX_Wait(int src, int dst, int id, int index) {
	EMX_Recv8(src, dst, id, index);
}

static inline void EMX_Signal(int src, int dst, int id, int index) {
	EMX_Send8(src, dst, id, index, 0);
}

static inline size_t EMX_RingBuffer_Free(int src, int dst) {
	return emx_ringbuf_free(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
}

static inline size_t EMX_RingBuffer_Size(int src, int dst) {
	return emx_ringbuf_size(&emx_rb[EMX_SndRcv2Buf(src, dst)]);
}

void EMX_Run(int core);

void EMX_Join(int core);

void EMX_SyncCores(int core);

extern pthread_t thread[EMX_NUM_CORES];

static inline int EMX_GetCore() {
	int i;
	pthread_t self = pthread_self();
	for (i = 0; i < EMX_NUM_CORES; i++) {
		if (thread[i] == self)
			return i;
	}
}

#endif
