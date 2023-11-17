#ifndef EMXAPI_COMM_RINGBUF_FIXED_H
#define EMXAPI_COMM_RINGBUF_FIXED_H

#include "emxapi_defines.h"
#include "emxapi_comm_ringbuf_config.h"
#include <stdint.h>

#define	EMX_RINGBUF_IMPL EMX_RINGBUF_IMPL_C11

#if EMX_RINGBUF_IMPL == EMX_RINGBUF_IMPL_C11
#include <stdatomic.h>

#define EMX_ATOMICREAD(x) atomic_load_explicit(&x, memory_order_acquire)
#define EMX_ATOMICWRITE(x,y) atomic_store_explicit(&x, y, memory_order_release)
#define EMX_ATOMICDECLARE(x) x
#elif EMX_RINGBUF_IMPL == EMX_RINGBUF_IMPL_CPP11
#include <atomic>

#define EMX_ATOMICREAD(x) std::atomic_load_explicit(&x, std::memory_order_acquire)
#define EMX_ATOMICWRITE(x,y) std::atomic_store_explicit(&x, y, std::memory_order_release)
#define EMX_ATOMICDECLARE(x) x
#elif EMX_RINGBUF_IMPL == EMX_RINGBUF_IMPL_VOLATILE
#define EMX_ATOMICREAD(x) x
#define EMX_ATOMICWRITE(x,y) x = y
#define EMX_ATOMICDECLARE(x) volatile x
#else
#error Invalid EMX_RINGBUF_IMPL
#endif

#if ((EMX_RINGBUF_ALIGN == 0) || ((EMX_RINGBUF_ALIGN &(EMX_RINGBUF_ALIGN - 1)) != 0))
#error The ringbuffer alignment (EMX_RINGBUF_ALIGN) must be a power of 2
#endif

#ifndef EMX_RINGBUF_CHUNK_SIZE
#define EMX_RINGBUF_CHUNK_SIZE (EMX_RINGBUF_SIZE/4)
#endif

#if defined(__GNUC__) || defined(__clang__) || defined(__MINGW32__) || defined(__MINGW64__)
#define EMX_ALWAYS_INLINE __attribute__ ((always_inline))
#else
#define EMX_ALWAYS_INLINE
#endif

int EMX_Init(int argc, char *argv[]);
void EMX_Finalize();
int EMX_RecvSync(int src, int dst, int id, int index);
void EMX_SendSync(int sync, int src, int dst, int id, int index);
void EMX_Wait(int src, int dst, int id, int index);
void EMX_Signal(int src, int dst, int id, int index);
void EMX_SyncCores(int core);

#define EMX_Send8(SRC, DST, ID, INDEX, VAL) EMX_Send8_##SRC##_##DST(VAL)
#define EMX_Recv8(SRC, DST, ID, INDEX) EMX_Recv8_##SRC##_##DST()
#define EMX_Send16(SRC, DST, ID, INDEX, VAL) EMX_Send16_##SRC##_##DST(VAL)
#define EMX_Recv16(SRC, DST, ID, INDEX) EMX_Recv16_##SRC##_##DST()
#define EMX_Send32(SRC, DST, ID, INDEX, VAL) EMX_Send32_##SRC##_##DST(VAL)
#define EMX_Recv32(SRC, DST, ID, INDEX) EMX_Recv32_##SRC##_##DST()
#define EMX_Send64(SRC, DST, ID, INDEX, VAL) EMX_Send64_##SRC##_##DST(VAL)
#define EMX_Recv64(SRC, DST, ID, INDEX) EMX_Recv64_##SRC##_##DST()
#define EMX_Send32F(SRC, DST, ID, INDEX, VAL) EMX_Send32F_##SRC##_##DST(VAL)
#define EMX_Recv32F(SRC, DST, ID, INDEX) EMX_Recv32F_##SRC##_##DST()
#define EMX_Send64F(SRC, DST, ID, INDEX, VAL) EMX_Send64F_##SRC##_##DST(VAL)
#define EMX_Recv64F(SRC, DST, ID, INDEX) EMX_Recv64F_##SRC##_##DST()
#define EMX_Send(SRC, DST, ID, INDEX, BUF, SIZE) EMX_Send_##SRC##_##DST(BUF, SIZE)
#define EMX_Recv(SRC, DST, ID, INDEX, BUF, SIZE) EMX_Recv_##SRC##_##DST(BUF, SIZE)
#define EMX_SendSync(SRC, DST, ID, INDEX, VAL) EMX_SendSync_##SRC##_##DST(VAL)
#define EMX_RecvSync(SRC, DST, ID, INDEX) EMX_RecvSync_##SRC##_##DST()
#define EMX_Signal(SRC, DST, ID, INDEX) EMX_Signal_##SRC##_##DST()
#define EMX_Wait(SRC, DST, ID, INDEX) EMX_Wait_##SRC##_##DST()

void emx_ringbuf_wait_0();

void EMX_Send8_0_1(register uint8_t);
uint8_t EMX_Recv8_0_1();
void EMX_Send16_0_1(register uint16_t);
uint16_t EMX_Recv16_0_1();
void EMX_Send32_0_1(register uint32_t);
uint32_t EMX_Recv32_0_1();
void EMX_Send64_0_1(register uint64_t);
uint64_t EMX_Recv64_0_1();
void emx_send_array_0_1(register const void *buf, register uint32_t size);
void emx_recv_array_0_1(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_0_1();
static inline uint32_t emx_ringbuf_size_0_1() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_0_1);
	return wr + ((wr < *emx_loc_rd_pos_0_1) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_0_1;
}

static inline uint32_t emx_ringbuf_free_0_1();
static inline uint32_t emx_ringbuf_free_0_1() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_0_1);
	return rd + ((*emx_loc_wr_pos_0_1 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_0_1 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_0_1(const register void *buf, register uint32_t size);

static inline void emx_send_0_1(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_0_1(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_0_1() < size) emx_ringbuf_wait_0();

	emx_send_array_0_1(buf, size);
}

void EMX_Send_0_1(const void *buf, uint32_t size);

extern void emx_recv_array_0_1(register void *buf, register uint32_t size);

static inline void emx_recv_0_1(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_0_1(void *buf, uint32_t size) {
	while (emx_ringbuf_size_0_1() < size) emx_ringbuf_wait_1();

	emx_recv_array_0_1(buf, size);
}

void EMX_Recv_0_1(void *buf, uint32_t size);

static inline void EMX_Send32F_0_1(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_0_1(float val) {
	EMX_Send32_0_1(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_0_1() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_0_1() {
	uint32_t r = EMX_Recv32_0_1();
	return *(float*)&r;
}

static inline void EMX_Send64F_0_1(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_0_1(double val) {
	EMX_Send64_0_1(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_0_1() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_0_1() {
	uint64_t r = EMX_Recv64_0_1();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_0_1() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_0_1() {
	return EMX_Recv8_0_1();
}

static inline void EMX_SendSync_0_1(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_0_1(uint8_t sync) {
	EMX_Send8_0_1(sync);
}

static inline void EMX_Wait_0_1() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_0_1() {
	EMX_Recv8_0_1();
}

static inline void EMX_Signal_0_1() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_0_1() {
	EMX_Send8_0_1(0);
}

void EMX_Send8_0_2(register uint8_t);
uint8_t EMX_Recv8_0_2();
void EMX_Send16_0_2(register uint16_t);
uint16_t EMX_Recv16_0_2();
void EMX_Send32_0_2(register uint32_t);
uint32_t EMX_Recv32_0_2();
void EMX_Send64_0_2(register uint64_t);
uint64_t EMX_Recv64_0_2();
void emx_send_array_0_2(register const void *buf, register uint32_t size);
void emx_recv_array_0_2(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_0_2();
static inline uint32_t emx_ringbuf_size_0_2() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_0_2);
	return wr + ((wr < *emx_loc_rd_pos_0_2) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_0_2;
}

static inline uint32_t emx_ringbuf_free_0_2();
static inline uint32_t emx_ringbuf_free_0_2() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_0_2);
	return rd + ((*emx_loc_wr_pos_0_2 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_0_2 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_0_2(const register void *buf, register uint32_t size);

static inline void emx_send_0_2(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_0_2(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_0_2() < size) emx_ringbuf_wait_0();

	emx_send_array_0_2(buf, size);
}

void EMX_Send_0_2(const void *buf, uint32_t size);

extern void emx_recv_array_0_2(register void *buf, register uint32_t size);

static inline void emx_recv_0_2(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_0_2(void *buf, uint32_t size) {
	while (emx_ringbuf_size_0_2() < size) emx_ringbuf_wait_2();

	emx_recv_array_0_2(buf, size);
}

void EMX_Recv_0_2(void *buf, uint32_t size);

static inline void EMX_Send32F_0_2(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_0_2(float val) {
	EMX_Send32_0_2(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_0_2() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_0_2() {
	uint32_t r = EMX_Recv32_0_2();
	return *(float*)&r;
}

static inline void EMX_Send64F_0_2(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_0_2(double val) {
	EMX_Send64_0_2(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_0_2() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_0_2() {
	uint64_t r = EMX_Recv64_0_2();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_0_2() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_0_2() {
	return EMX_Recv8_0_2();
}

static inline void EMX_SendSync_0_2(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_0_2(uint8_t sync) {
	EMX_Send8_0_2(sync);
}

static inline void EMX_Wait_0_2() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_0_2() {
	EMX_Recv8_0_2();
}

static inline void EMX_Signal_0_2() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_0_2() {
	EMX_Send8_0_2(0);
}

void EMX_Send8_0_3(register uint8_t);
uint8_t EMX_Recv8_0_3();
void EMX_Send16_0_3(register uint16_t);
uint16_t EMX_Recv16_0_3();
void EMX_Send32_0_3(register uint32_t);
uint32_t EMX_Recv32_0_3();
void EMX_Send64_0_3(register uint64_t);
uint64_t EMX_Recv64_0_3();
void emx_send_array_0_3(register const void *buf, register uint32_t size);
void emx_recv_array_0_3(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_0_3();
static inline uint32_t emx_ringbuf_size_0_3() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_0_3);
	return wr + ((wr < *emx_loc_rd_pos_0_3) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_0_3;
}

static inline uint32_t emx_ringbuf_free_0_3();
static inline uint32_t emx_ringbuf_free_0_3() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_0_3);
	return rd + ((*emx_loc_wr_pos_0_3 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_0_3 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_0_3(const register void *buf, register uint32_t size);

static inline void emx_send_0_3(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_0_3(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_0_3() < size) emx_ringbuf_wait_0();

	emx_send_array_0_3(buf, size);
}

void EMX_Send_0_3(const void *buf, uint32_t size);

extern void emx_recv_array_0_3(register void *buf, register uint32_t size);

static inline void emx_recv_0_3(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_0_3(void *buf, uint32_t size) {
	while (emx_ringbuf_size_0_3() < size) emx_ringbuf_wait_3();

	emx_recv_array_0_3(buf, size);
}

void EMX_Recv_0_3(void *buf, uint32_t size);

static inline void EMX_Send32F_0_3(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_0_3(float val) {
	EMX_Send32_0_3(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_0_3() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_0_3() {
	uint32_t r = EMX_Recv32_0_3();
	return *(float*)&r;
}

static inline void EMX_Send64F_0_3(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_0_3(double val) {
	EMX_Send64_0_3(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_0_3() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_0_3() {
	uint64_t r = EMX_Recv64_0_3();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_0_3() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_0_3() {
	return EMX_Recv8_0_3();
}

static inline void EMX_SendSync_0_3(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_0_3(uint8_t sync) {
	EMX_Send8_0_3(sync);
}

static inline void EMX_Wait_0_3() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_0_3() {
	EMX_Recv8_0_3();
}

static inline void EMX_Signal_0_3() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_0_3() {
	EMX_Send8_0_3(0);
}

void emx_ringbuf_wait_1();

void EMX_Send8_1_0(register uint8_t);
uint8_t EMX_Recv8_1_0();
void EMX_Send16_1_0(register uint16_t);
uint16_t EMX_Recv16_1_0();
void EMX_Send32_1_0(register uint32_t);
uint32_t EMX_Recv32_1_0();
void EMX_Send64_1_0(register uint64_t);
uint64_t EMX_Recv64_1_0();
void emx_send_array_1_0(register const void *buf, register uint32_t size);
void emx_recv_array_1_0(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_1_0();
static inline uint32_t emx_ringbuf_size_1_0() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_1_0);
	return wr + ((wr < *emx_loc_rd_pos_1_0) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_1_0;
}

static inline uint32_t emx_ringbuf_free_1_0();
static inline uint32_t emx_ringbuf_free_1_0() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_1_0);
	return rd + ((*emx_loc_wr_pos_1_0 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_1_0 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_1_0(const register void *buf, register uint32_t size);

static inline void emx_send_1_0(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_1_0(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_1_0() < size) emx_ringbuf_wait_1();

	emx_send_array_1_0(buf, size);
}

void EMX_Send_1_0(const void *buf, uint32_t size);

extern void emx_recv_array_1_0(register void *buf, register uint32_t size);

static inline void emx_recv_1_0(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_1_0(void *buf, uint32_t size) {
	while (emx_ringbuf_size_1_0() < size) emx_ringbuf_wait_0();

	emx_recv_array_1_0(buf, size);
}

void EMX_Recv_1_0(void *buf, uint32_t size);

static inline void EMX_Send32F_1_0(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_1_0(float val) {
	EMX_Send32_1_0(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_1_0() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_1_0() {
	uint32_t r = EMX_Recv32_1_0();
	return *(float*)&r;
}

static inline void EMX_Send64F_1_0(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_1_0(double val) {
	EMX_Send64_1_0(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_1_0() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_1_0() {
	uint64_t r = EMX_Recv64_1_0();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_1_0() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_1_0() {
	return EMX_Recv8_1_0();
}

static inline void EMX_SendSync_1_0(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_1_0(uint8_t sync) {
	EMX_Send8_1_0(sync);
}

static inline void EMX_Wait_1_0() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_1_0() {
	EMX_Recv8_1_0();
}

static inline void EMX_Signal_1_0() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_1_0() {
	EMX_Send8_1_0(0);
}

void EMX_Send8_1_2(register uint8_t);
uint8_t EMX_Recv8_1_2();
void EMX_Send16_1_2(register uint16_t);
uint16_t EMX_Recv16_1_2();
void EMX_Send32_1_2(register uint32_t);
uint32_t EMX_Recv32_1_2();
void EMX_Send64_1_2(register uint64_t);
uint64_t EMX_Recv64_1_2();
void emx_send_array_1_2(register const void *buf, register uint32_t size);
void emx_recv_array_1_2(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_1_2();
static inline uint32_t emx_ringbuf_size_1_2() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_1_2);
	return wr + ((wr < *emx_loc_rd_pos_1_2) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_1_2;
}

static inline uint32_t emx_ringbuf_free_1_2();
static inline uint32_t emx_ringbuf_free_1_2() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_1_2);
	return rd + ((*emx_loc_wr_pos_1_2 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_1_2 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_1_2(const register void *buf, register uint32_t size);

static inline void emx_send_1_2(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_1_2(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_1_2() < size) emx_ringbuf_wait_1();

	emx_send_array_1_2(buf, size);
}

void EMX_Send_1_2(const void *buf, uint32_t size);

extern void emx_recv_array_1_2(register void *buf, register uint32_t size);

static inline void emx_recv_1_2(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_1_2(void *buf, uint32_t size) {
	while (emx_ringbuf_size_1_2() < size) emx_ringbuf_wait_2();

	emx_recv_array_1_2(buf, size);
}

void EMX_Recv_1_2(void *buf, uint32_t size);

static inline void EMX_Send32F_1_2(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_1_2(float val) {
	EMX_Send32_1_2(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_1_2() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_1_2() {
	uint32_t r = EMX_Recv32_1_2();
	return *(float*)&r;
}

static inline void EMX_Send64F_1_2(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_1_2(double val) {
	EMX_Send64_1_2(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_1_2() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_1_2() {
	uint64_t r = EMX_Recv64_1_2();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_1_2() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_1_2() {
	return EMX_Recv8_1_2();
}

static inline void EMX_SendSync_1_2(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_1_2(uint8_t sync) {
	EMX_Send8_1_2(sync);
}

static inline void EMX_Wait_1_2() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_1_2() {
	EMX_Recv8_1_2();
}

static inline void EMX_Signal_1_2() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_1_2() {
	EMX_Send8_1_2(0);
}

void EMX_Send8_1_3(register uint8_t);
uint8_t EMX_Recv8_1_3();
void EMX_Send16_1_3(register uint16_t);
uint16_t EMX_Recv16_1_3();
void EMX_Send32_1_3(register uint32_t);
uint32_t EMX_Recv32_1_3();
void EMX_Send64_1_3(register uint64_t);
uint64_t EMX_Recv64_1_3();
void emx_send_array_1_3(register const void *buf, register uint32_t size);
void emx_recv_array_1_3(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_1_3();
static inline uint32_t emx_ringbuf_size_1_3() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_1_3);
	return wr + ((wr < *emx_loc_rd_pos_1_3) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_1_3;
}

static inline uint32_t emx_ringbuf_free_1_3();
static inline uint32_t emx_ringbuf_free_1_3() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_1_3);
	return rd + ((*emx_loc_wr_pos_1_3 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_1_3 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_1_3(const register void *buf, register uint32_t size);

static inline void emx_send_1_3(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_1_3(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_1_3() < size) emx_ringbuf_wait_1();

	emx_send_array_1_3(buf, size);
}

void EMX_Send_1_3(const void *buf, uint32_t size);

extern void emx_recv_array_1_3(register void *buf, register uint32_t size);

static inline void emx_recv_1_3(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_1_3(void *buf, uint32_t size) {
	while (emx_ringbuf_size_1_3() < size) emx_ringbuf_wait_3();

	emx_recv_array_1_3(buf, size);
}

void EMX_Recv_1_3(void *buf, uint32_t size);

static inline void EMX_Send32F_1_3(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_1_3(float val) {
	EMX_Send32_1_3(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_1_3() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_1_3() {
	uint32_t r = EMX_Recv32_1_3();
	return *(float*)&r;
}

static inline void EMX_Send64F_1_3(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_1_3(double val) {
	EMX_Send64_1_3(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_1_3() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_1_3() {
	uint64_t r = EMX_Recv64_1_3();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_1_3() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_1_3() {
	return EMX_Recv8_1_3();
}

static inline void EMX_SendSync_1_3(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_1_3(uint8_t sync) {
	EMX_Send8_1_3(sync);
}

static inline void EMX_Wait_1_3() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_1_3() {
	EMX_Recv8_1_3();
}

static inline void EMX_Signal_1_3() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_1_3() {
	EMX_Send8_1_3(0);
}

void emx_ringbuf_wait_2();

void EMX_Send8_2_0(register uint8_t);
uint8_t EMX_Recv8_2_0();
void EMX_Send16_2_0(register uint16_t);
uint16_t EMX_Recv16_2_0();
void EMX_Send32_2_0(register uint32_t);
uint32_t EMX_Recv32_2_0();
void EMX_Send64_2_0(register uint64_t);
uint64_t EMX_Recv64_2_0();
void emx_send_array_2_0(register const void *buf, register uint32_t size);
void emx_recv_array_2_0(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_2_0();
static inline uint32_t emx_ringbuf_size_2_0() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_2_0);
	return wr + ((wr < *emx_loc_rd_pos_2_0) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_2_0;
}

static inline uint32_t emx_ringbuf_free_2_0();
static inline uint32_t emx_ringbuf_free_2_0() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_2_0);
	return rd + ((*emx_loc_wr_pos_2_0 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_2_0 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_2_0(const register void *buf, register uint32_t size);

static inline void emx_send_2_0(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_2_0(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_2_0() < size) emx_ringbuf_wait_2();

	emx_send_array_2_0(buf, size);
}

void EMX_Send_2_0(const void *buf, uint32_t size);

extern void emx_recv_array_2_0(register void *buf, register uint32_t size);

static inline void emx_recv_2_0(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_2_0(void *buf, uint32_t size) {
	while (emx_ringbuf_size_2_0() < size) emx_ringbuf_wait_0();

	emx_recv_array_2_0(buf, size);
}

void EMX_Recv_2_0(void *buf, uint32_t size);

static inline void EMX_Send32F_2_0(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_2_0(float val) {
	EMX_Send32_2_0(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_2_0() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_2_0() {
	uint32_t r = EMX_Recv32_2_0();
	return *(float*)&r;
}

static inline void EMX_Send64F_2_0(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_2_0(double val) {
	EMX_Send64_2_0(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_2_0() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_2_0() {
	uint64_t r = EMX_Recv64_2_0();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_2_0() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_2_0() {
	return EMX_Recv8_2_0();
}

static inline void EMX_SendSync_2_0(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_2_0(uint8_t sync) {
	EMX_Send8_2_0(sync);
}

static inline void EMX_Wait_2_0() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_2_0() {
	EMX_Recv8_2_0();
}

static inline void EMX_Signal_2_0() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_2_0() {
	EMX_Send8_2_0(0);
}

void EMX_Send8_2_1(register uint8_t);
uint8_t EMX_Recv8_2_1();
void EMX_Send16_2_1(register uint16_t);
uint16_t EMX_Recv16_2_1();
void EMX_Send32_2_1(register uint32_t);
uint32_t EMX_Recv32_2_1();
void EMX_Send64_2_1(register uint64_t);
uint64_t EMX_Recv64_2_1();
void emx_send_array_2_1(register const void *buf, register uint32_t size);
void emx_recv_array_2_1(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_2_1();
static inline uint32_t emx_ringbuf_size_2_1() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_2_1);
	return wr + ((wr < *emx_loc_rd_pos_2_1) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_2_1;
}

static inline uint32_t emx_ringbuf_free_2_1();
static inline uint32_t emx_ringbuf_free_2_1() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_2_1);
	return rd + ((*emx_loc_wr_pos_2_1 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_2_1 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_2_1(const register void *buf, register uint32_t size);

static inline void emx_send_2_1(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_2_1(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_2_1() < size) emx_ringbuf_wait_2();

	emx_send_array_2_1(buf, size);
}

void EMX_Send_2_1(const void *buf, uint32_t size);

extern void emx_recv_array_2_1(register void *buf, register uint32_t size);

static inline void emx_recv_2_1(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_2_1(void *buf, uint32_t size) {
	while (emx_ringbuf_size_2_1() < size) emx_ringbuf_wait_1();

	emx_recv_array_2_1(buf, size);
}

void EMX_Recv_2_1(void *buf, uint32_t size);

static inline void EMX_Send32F_2_1(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_2_1(float val) {
	EMX_Send32_2_1(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_2_1() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_2_1() {
	uint32_t r = EMX_Recv32_2_1();
	return *(float*)&r;
}

static inline void EMX_Send64F_2_1(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_2_1(double val) {
	EMX_Send64_2_1(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_2_1() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_2_1() {
	uint64_t r = EMX_Recv64_2_1();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_2_1() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_2_1() {
	return EMX_Recv8_2_1();
}

static inline void EMX_SendSync_2_1(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_2_1(uint8_t sync) {
	EMX_Send8_2_1(sync);
}

static inline void EMX_Wait_2_1() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_2_1() {
	EMX_Recv8_2_1();
}

static inline void EMX_Signal_2_1() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_2_1() {
	EMX_Send8_2_1(0);
}

void EMX_Send8_2_3(register uint8_t);
uint8_t EMX_Recv8_2_3();
void EMX_Send16_2_3(register uint16_t);
uint16_t EMX_Recv16_2_3();
void EMX_Send32_2_3(register uint32_t);
uint32_t EMX_Recv32_2_3();
void EMX_Send64_2_3(register uint64_t);
uint64_t EMX_Recv64_2_3();
void emx_send_array_2_3(register const void *buf, register uint32_t size);
void emx_recv_array_2_3(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_2_3();
static inline uint32_t emx_ringbuf_size_2_3() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_2_3);
	return wr + ((wr < *emx_loc_rd_pos_2_3) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_2_3;
}

static inline uint32_t emx_ringbuf_free_2_3();
static inline uint32_t emx_ringbuf_free_2_3() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_2_3);
	return rd + ((*emx_loc_wr_pos_2_3 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_2_3 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_2_3(const register void *buf, register uint32_t size);

static inline void emx_send_2_3(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_2_3(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_2_3() < size) emx_ringbuf_wait_2();

	emx_send_array_2_3(buf, size);
}

void EMX_Send_2_3(const void *buf, uint32_t size);

extern void emx_recv_array_2_3(register void *buf, register uint32_t size);

static inline void emx_recv_2_3(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_2_3(void *buf, uint32_t size) {
	while (emx_ringbuf_size_2_3() < size) emx_ringbuf_wait_3();

	emx_recv_array_2_3(buf, size);
}

void EMX_Recv_2_3(void *buf, uint32_t size);

static inline void EMX_Send32F_2_3(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_2_3(float val) {
	EMX_Send32_2_3(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_2_3() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_2_3() {
	uint32_t r = EMX_Recv32_2_3();
	return *(float*)&r;
}

static inline void EMX_Send64F_2_3(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_2_3(double val) {
	EMX_Send64_2_3(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_2_3() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_2_3() {
	uint64_t r = EMX_Recv64_2_3();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_2_3() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_2_3() {
	return EMX_Recv8_2_3();
}

static inline void EMX_SendSync_2_3(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_2_3(uint8_t sync) {
	EMX_Send8_2_3(sync);
}

static inline void EMX_Wait_2_3() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_2_3() {
	EMX_Recv8_2_3();
}

static inline void EMX_Signal_2_3() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_2_3() {
	EMX_Send8_2_3(0);
}

void emx_ringbuf_wait_3();

void EMX_Send8_3_0(register uint8_t);
uint8_t EMX_Recv8_3_0();
void EMX_Send16_3_0(register uint16_t);
uint16_t EMX_Recv16_3_0();
void EMX_Send32_3_0(register uint32_t);
uint32_t EMX_Recv32_3_0();
void EMX_Send64_3_0(register uint64_t);
uint64_t EMX_Recv64_3_0();
void emx_send_array_3_0(register const void *buf, register uint32_t size);
void emx_recv_array_3_0(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_3_0();
static inline uint32_t emx_ringbuf_size_3_0() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_3_0);
	return wr + ((wr < *emx_loc_rd_pos_3_0) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_3_0;
}

static inline uint32_t emx_ringbuf_free_3_0();
static inline uint32_t emx_ringbuf_free_3_0() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_3_0);
	return rd + ((*emx_loc_wr_pos_3_0 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_3_0 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_3_0(const register void *buf, register uint32_t size);

static inline void emx_send_3_0(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_3_0(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_3_0() < size) emx_ringbuf_wait_3();

	emx_send_array_3_0(buf, size);
}

void EMX_Send_3_0(const void *buf, uint32_t size);

extern void emx_recv_array_3_0(register void *buf, register uint32_t size);

static inline void emx_recv_3_0(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_3_0(void *buf, uint32_t size) {
	while (emx_ringbuf_size_3_0() < size) emx_ringbuf_wait_0();

	emx_recv_array_3_0(buf, size);
}

void EMX_Recv_3_0(void *buf, uint32_t size);

static inline void EMX_Send32F_3_0(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_3_0(float val) {
	EMX_Send32_3_0(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_3_0() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_3_0() {
	uint32_t r = EMX_Recv32_3_0();
	return *(float*)&r;
}

static inline void EMX_Send64F_3_0(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_3_0(double val) {
	EMX_Send64_3_0(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_3_0() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_3_0() {
	uint64_t r = EMX_Recv64_3_0();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_3_0() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_3_0() {
	return EMX_Recv8_3_0();
}

static inline void EMX_SendSync_3_0(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_3_0(uint8_t sync) {
	EMX_Send8_3_0(sync);
}

static inline void EMX_Wait_3_0() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_3_0() {
	EMX_Recv8_3_0();
}

static inline void EMX_Signal_3_0() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_3_0() {
	EMX_Send8_3_0(0);
}

void EMX_Send8_3_1(register uint8_t);
uint8_t EMX_Recv8_3_1();
void EMX_Send16_3_1(register uint16_t);
uint16_t EMX_Recv16_3_1();
void EMX_Send32_3_1(register uint32_t);
uint32_t EMX_Recv32_3_1();
void EMX_Send64_3_1(register uint64_t);
uint64_t EMX_Recv64_3_1();
void emx_send_array_3_1(register const void *buf, register uint32_t size);
void emx_recv_array_3_1(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_3_1();
static inline uint32_t emx_ringbuf_size_3_1() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_3_1);
	return wr + ((wr < *emx_loc_rd_pos_3_1) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_3_1;
}

static inline uint32_t emx_ringbuf_free_3_1();
static inline uint32_t emx_ringbuf_free_3_1() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_3_1);
	return rd + ((*emx_loc_wr_pos_3_1 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_3_1 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_3_1(const register void *buf, register uint32_t size);

static inline void emx_send_3_1(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_3_1(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_3_1() < size) emx_ringbuf_wait_3();

	emx_send_array_3_1(buf, size);
}

void EMX_Send_3_1(const void *buf, uint32_t size);

extern void emx_recv_array_3_1(register void *buf, register uint32_t size);

static inline void emx_recv_3_1(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_3_1(void *buf, uint32_t size) {
	while (emx_ringbuf_size_3_1() < size) emx_ringbuf_wait_1();

	emx_recv_array_3_1(buf, size);
}

void EMX_Recv_3_1(void *buf, uint32_t size);

static inline void EMX_Send32F_3_1(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_3_1(float val) {
	EMX_Send32_3_1(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_3_1() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_3_1() {
	uint32_t r = EMX_Recv32_3_1();
	return *(float*)&r;
}

static inline void EMX_Send64F_3_1(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_3_1(double val) {
	EMX_Send64_3_1(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_3_1() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_3_1() {
	uint64_t r = EMX_Recv64_3_1();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_3_1() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_3_1() {
	return EMX_Recv8_3_1();
}

static inline void EMX_SendSync_3_1(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_3_1(uint8_t sync) {
	EMX_Send8_3_1(sync);
}

static inline void EMX_Wait_3_1() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_3_1() {
	EMX_Recv8_3_1();
}

static inline void EMX_Signal_3_1() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_3_1() {
	EMX_Send8_3_1(0);
}

void EMX_Send8_3_2(register uint8_t);
uint8_t EMX_Recv8_3_2();
void EMX_Send16_3_2(register uint16_t);
uint16_t EMX_Recv16_3_2();
void EMX_Send32_3_2(register uint32_t);
uint32_t EMX_Recv32_3_2();
void EMX_Send64_3_2(register uint64_t);
uint64_t EMX_Recv64_3_2();
void emx_send_array_3_2(register const void *buf, register uint32_t size);
void emx_recv_array_3_2(register void *buf, register uint32_t size);

static inline uint32_t emx_ringbuf_size_3_2();
static inline uint32_t emx_ringbuf_size_3_2() {
	uint32_t wr = EMX_ATOMICREAD(*emx_wr_pos_3_2);
	return wr + ((wr < *emx_loc_rd_pos_3_2) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_rd_pos_3_2;
}

static inline uint32_t emx_ringbuf_free_3_2();
static inline uint32_t emx_ringbuf_free_3_2() {
	uint32_t rd = EMX_ATOMICREAD(*emx_rd_pos_3_2);
	return rd + ((*emx_loc_wr_pos_3_2 >= rd) ? EMX_RINGBUF_SIZE : 0)
		- *emx_loc_wr_pos_3_2 - EMX_RINGBUF_ALIGN;
}

void emx_send_array_3_2(const register void *buf, register uint32_t size);

static inline void emx_send_3_2(const void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_send_3_2(const void *buf, uint32_t size) {
	while (emx_ringbuf_free_3_2() < size) emx_ringbuf_wait_3();

	emx_send_array_3_2(buf, size);
}

void EMX_Send_3_2(const void *buf, uint32_t size);

extern void emx_recv_array_3_2(register void *buf, register uint32_t size);

static inline void emx_recv_3_2(void *buf, uint32_t size) EMX_ALWAYS_INLINE;
static inline void emx_recv_3_2(void *buf, uint32_t size) {
	while (emx_ringbuf_size_3_2() < size) emx_ringbuf_wait_2();

	emx_recv_array_3_2(buf, size);
}

void EMX_Recv_3_2(void *buf, uint32_t size);

static inline void EMX_Send32F_3_2(float val) EMX_ALWAYS_INLINE;
static inline void EMX_Send32F_3_2(float val) {
	EMX_Send32_3_2(*(uint32_t*)&val);
}

static inline float EMX_Recv32F_3_2() EMX_ALWAYS_INLINE;
static inline float EMX_Recv32F_3_2() {
	uint32_t r = EMX_Recv32_3_2();
	return *(float*)&r;
}

static inline void EMX_Send64F_3_2(double val) EMX_ALWAYS_INLINE;
static inline void EMX_Send64F_3_2(double val) {
	EMX_Send64_3_2(*(uint64_t*)&val);
}

static inline double EMX_Recv64F_3_2() EMX_ALWAYS_INLINE;
static inline double EMX_Recv64F_3_2() {
	uint64_t r = EMX_Recv64_3_2();
	return *(double*)&r;
}

static inline uint8_t EMX_RecvSync_3_2() EMX_ALWAYS_INLINE;
static inline uint8_t EMX_RecvSync_3_2() {
	return EMX_Recv8_3_2();
}

static inline void EMX_SendSync_3_2(uint8_t sync) EMX_ALWAYS_INLINE;
static inline void EMX_SendSync_3_2(uint8_t sync) {
	EMX_Send8_3_2(sync);
}

static inline void EMX_Wait_3_2() EMX_ALWAYS_INLINE;
static inline void EMX_Wait_3_2() {
	EMX_Recv8_3_2();
}

static inline void EMX_Signal_3_2() EMX_ALWAYS_INLINE;
static inline void EMX_Signal_3_2() {
	EMX_Send8_3_2(0);
}

#endif
