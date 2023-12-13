#ifndef EMX_RINGBUF_H
#define EMX_RINGBUF_H

#include <stdint.h>
#include <string.h>

#define	EMX_RINGBUF_IMPL_C11			0
#define	EMX_RINGBUF_IMPL_CPP11		1
#define	EMX_RINGBUF_IMPL_VOLATILE	2
#define	EMX_RINGBUF_IMPL_ARM_ATOMIC	3

#define	EMX_RINGBUF_IMPL		EMX_RINGBUF_IMPL_ARM_ATOMIC


#if EMX_RINGBUF_IMPL == EMX_RINGBUF_IMPL_C11

	#include <stdatomic.h>

	#define		EMX_ATOMICREAD(x)			atomic_load_explicit(&x, memory_order_acquire)
	#define		EMX_ATOMICWRITE(x,y)		atomic_store_explicit(&x, y, memory_order_release)
	#define		EMX_ATOMICDECLARE(x)		x

#elif EMX_RINGBUF_IMPL == EMX_RINGBUF_IMPL_CPP11

	#include <atomic>

	#define		EMX_ATOMICREAD(x)			std::atomic_load_explicit(&x, std::memory_order_acquire)
	#define		EMX_ATOMICWRITE(x,y)		std::atomic_store_explicit(&x, y, std::memory_order_release)
	#define		EMX_ATOMICDECLARE(x)		x

#elif EMX_RINGBUF_IMPL == EMX_RINGBUF_IMPL_VOLATILE

	#define		EMX_ATOMICREAD(x)			x
	#define		EMX_ATOMICWRITE(x,y)		x = y
	#define		EMX_ATOMICDECLARE(x)		volatile x

#elif EMX_RINGBUF_IMPL == EMX_RINGBUF_IMPL_ARM_ATOMIC

	static inline uint32_t arm_atomic_load(uint32_t *ptr);
	static inline void arm_atomic_store(uint32_t *ptr, uint32_t value);

	#define		EMX_ATOMICREAD(x)			arm_atomic_load(&x)
	#define		EMX_ATOMICWRITE(x,y)		arm_atomic_store(&x, y)
	#define		EMX_ATOMICDECLARE(x)		x

#else
	#error		Invalid EMX_RINGBUF_IMPL
#endif

#ifndef EMX_RINGBUF_LINESIZE
	#define EMX_RINGBUF_LINESIZE 64
#endif

#ifndef EMX_RINGBUF_ALIGN
	#define EMX_RINGBUF_ALIGN 8
#endif

#ifndef EMX_RINGBUF_SIZE
	#define	EMX_RINGBUF_SIZE rb->size
	#define EMX_RINGBUF_ELEMENTS rb->elements
#else
	#define EMX_RINGBUF_ELEMENTS (EMX_RINGBUF_SIZE/EMX_RINGBUF_ALIGN)
#endif

#define EMX_RINGBUF_CHUNK	EMX_RINGBUF_SIZE/4

typedef uint32_t emx_ringbuf_size_t;
typedef uint8_t emx_ringbuf_element_t[EMX_RINGBUF_ALIGN];

typedef struct {
	emx_ringbuf_size_t size;
	emx_ringbuf_size_t elements;
	EMX_ATOMICDECLARE(emx_ringbuf_element_t*) buf;

	EMX_ATOMICDECLARE(emx_ringbuf_size_t) rd_pos __attribute__ ((aligned (EMX_RINGBUF_LINESIZE)));
	//emx_ringbuf_size_t dummy1[EMX_RINGBUF_LINESIZE/sizeof(emx_ringbuf_size_t)-1];

	EMX_ATOMICDECLARE(emx_ringbuf_size_t) wr_pos __attribute__ ((aligned (EMX_RINGBUF_LINESIZE)));
	//emx_ringbuf_size_t dummy2[EMX_RINGBUF_LINESIZE/sizeof(emx_ringbuf_size_t)-1];
} emx_ringbuf_t __attribute__ ((aligned (EMX_RINGBUF_LINESIZE)));

void emx_ringbuf_init(emx_ringbuf_t* rb, emx_ringbuf_size_t size, void *buf);

void emx_ringbuf_wait();

static inline emx_ringbuf_size_t emx_ringbuf_size2elements(emx_ringbuf_size_t size) {
	return (size + EMX_RINGBUF_ALIGN-1) / EMX_RINGBUF_ALIGN;
}

static inline emx_ringbuf_size_t emx_ringbuf_size2(emx_ringbuf_size_t rd_pos, emx_ringbuf_size_t wr_pos) {
	return wr_pos + ((wr_pos < rd_pos) ? EMX_RINGBUF_ELEMENTS : 0) - rd_pos;
}

static inline emx_ringbuf_size_t emx_ringbuf_size(emx_ringbuf_t* rb) {
	return emx_ringbuf_size2(rb->rd_pos, rb->wr_pos);
}

static inline int emx_ringbuf_empty2(emx_ringbuf_size_t rd_pos, emx_ringbuf_size_t wr_pos) {
	return rd_pos == wr_pos;
}

static inline int emx_ringbuf_empty(emx_ringbuf_t* rb) {
	return emx_ringbuf_empty2(rb->rd_pos, rb->wr_pos);
}

static inline emx_ringbuf_size_t emx_ringbuf_free2(emx_ringbuf_size_t rd_pos, emx_ringbuf_size_t wr_pos) {
	return rd_pos + ((wr_pos >= rd_pos) ? EMX_RINGBUF_ELEMENTS : 0) - wr_pos - 1;
}

static inline emx_ringbuf_size_t emx_ringbuf_free(emx_ringbuf_t* rb) {
	return emx_ringbuf_free2(rb->rd_pos, rb->wr_pos);
}

static inline int emx_ringbuf_full2(emx_ringbuf_size_t rd_pos, emx_ringbuf_size_t wr_pos) {
	return (rd_pos == wr_pos+1) || (rd_pos == wr_pos-EMX_RINGBUF_ELEMENTS+1);
}

static inline int emx_ringbuf_full(emx_ringbuf_t* rb) {
	return emx_ringbuf_full2(rb->rd_pos, rb->wr_pos);
}

static inline emx_ringbuf_size_t emx_ringbuf_inc(emx_ringbuf_size_t pos, int amount) {
	pos += amount;
	if (pos >= EMX_RINGBUF_ELEMENTS)
		pos -= EMX_RINGBUF_ELEMENTS;
	return pos;
}

static void emx_ringbuf_readsmall(emx_ringbuf_t* rb, void *buf, emx_ringbuf_size_t size) {
	uint8_t* curbuf = (uint8_t*) buf;
	emx_ringbuf_size_t rd_pos = rb->rd_pos;
	emx_ringbuf_size_t elcount = emx_ringbuf_size2elements(size);

	while (emx_ringbuf_size2(rd_pos, EMX_ATOMICREAD(rb->wr_pos)) < elcount)
		emx_ringbuf_wait();

	emx_ringbuf_size_t firstChunkSize = EMX_RINGBUF_SIZE - rd_pos * EMX_RINGBUF_ALIGN;
	if (firstChunkSize >= size) {
		memcpy(curbuf, (uint8_t*)rb->buf + rd_pos * EMX_RINGBUF_ALIGN, size);
	} else {
		memcpy(curbuf, (uint8_t*)rb->buf + rd_pos * EMX_RINGBUF_ALIGN, firstChunkSize);
		memcpy(curbuf + firstChunkSize, (uint8_t*)rb->buf, size - firstChunkSize);
	}

	EMX_ATOMICWRITE(rb->rd_pos, emx_ringbuf_inc(rd_pos, elcount));
}

static void emx_ringbuf_writesmall(emx_ringbuf_t* rb, const void *buf, emx_ringbuf_size_t size) {
	uint8_t* curbuf = (uint8_t*) buf;
	emx_ringbuf_size_t wr_pos = rb->wr_pos;
	emx_ringbuf_size_t elcount = emx_ringbuf_size2elements(size);

	while (emx_ringbuf_free2(EMX_ATOMICREAD(rb->rd_pos), wr_pos) < elcount)
		emx_ringbuf_wait();

	emx_ringbuf_size_t firstChunkSize = EMX_RINGBUF_SIZE - wr_pos * EMX_RINGBUF_ALIGN;
	if (firstChunkSize >= size) {
		memcpy((uint8_t*)rb->buf + wr_pos * EMX_RINGBUF_ALIGN, curbuf, size);
	} else {
		memcpy((uint8_t*)rb->buf + wr_pos * EMX_RINGBUF_ALIGN, curbuf, firstChunkSize);
		memcpy((uint8_t*)rb->buf, curbuf + firstChunkSize, size - firstChunkSize);
	}

	EMX_ATOMICWRITE(rb->wr_pos, emx_ringbuf_inc(wr_pos, elcount));
}

void emx_ringbuf_readlarge(emx_ringbuf_t* rb, void *buf, emx_ringbuf_size_t size);
void emx_ringbuf_writelarge(emx_ringbuf_t* rb, const void *buf, emx_ringbuf_size_t size);

#define EMX_RINGBUF_READIMPL(SIZE, TYPE)                           \
	TYPE val;                                                      \
                                                                   \
	emx_ringbuf_size_t rd_pos = rb->rd_pos;                        \
                                                                   \
	while (emx_ringbuf_empty2(rd_pos, EMX_ATOMICREAD(rb->wr_pos))) \
		emx_ringbuf_wait();                                        \
                                                                   \
	val = *(TYPE*)(rb->buf + rd_pos);                              \
                                                                   \
	EMX_ATOMICWRITE(rb->rd_pos, emx_ringbuf_inc(rd_pos, 1));       \
                                                                   \
	return val;                                                    \

#define EMX_RINGBUF_WRITEIMPL(SIZE, TYPE)                          \
	emx_ringbuf_size_t wr_pos = rb->wr_pos;                        \
	while (emx_ringbuf_full2(EMX_ATOMICREAD(rb->rd_pos), wr_pos))  \
		emx_ringbuf_wait();                                        \
	*(TYPE*)(rb->buf + wr_pos) = val;                              \
                                                                   \
																   \
	EMX_ATOMICWRITE(rb->wr_pos, emx_ringbuf_inc(wr_pos, 1));       

static inline uint8_t emx_ringbuf_read8(emx_ringbuf_t* rb) {
	EMX_RINGBUF_READIMPL(1, uint8_t)
}

static inline void emx_ringbuf_write8(emx_ringbuf_t* rb, uint8_t val) {
	EMX_RINGBUF_WRITEIMPL(1, uint8_t)
}

static inline uint16_t emx_ringbuf_read16(emx_ringbuf_t* rb) {
#if EMX_RINGBUF_ALIGN >= 2
	EMX_RINGBUF_READIMPL(2, uint16_t)
#else
	uint16_t val
	emx_ringbuf_readsmall(rb, &val, 2);
	return val;
#endif
}

static inline void emx_ringbuf_write16(emx_ringbuf_t* rb, uint16_t val) {
#if EMX_RINGBUF_ALIGN >= 2
	EMX_RINGBUF_WRITEIMPL(2, uint16_t)
#else
	emx_ringbuf_writesmall(rb, &val, 2);
#endif
}

static inline uint32_t emx_ringbuf_read32(emx_ringbuf_t* rb) {
#if EMX_RINGBUF_ALIGN >= 4
	EMX_RINGBUF_READIMPL(4, uint32_t)
#else
	uint32_t val
	emx_ringbuf_readsmall(rb, &val, 4);
	return val;
#endif
}

static inline void emx_ringbuf_write32(emx_ringbuf_t* rb, uint32_t val) {
#if EMX_RINGBUF_ALIGN >= 4
	EMX_RINGBUF_WRITEIMPL(4, uint32_t)
#else
	emx_ringbuf_writesmall(rb, &val, 4);
#endif
}

static inline uint64_t emx_ringbuf_read64(emx_ringbuf_t* rb) {
#if EMX_RINGBUF_ALIGN >= 8
	EMX_RINGBUF_READIMPL(8, uint64_t)
#else
	uint64_t val
	emx_ringbuf_readsmall(rb, &val, 8);
	return val;
#endif
}

static inline void emx_ringbuf_write64(emx_ringbuf_t* rb, uint64_t val) {
#if EMX_RINGBUF_ALIGN >= 8
	EMX_RINGBUF_WRITEIMPL(8, uint64_t)
#else
	emx_ringbuf_writesmall(rb, &val, 8);
#endif
}

static inline void emx_ringbuf_read(emx_ringbuf_t* rb, emx_ringbuf_t* rb_back, void *buf, emx_ringbuf_size_t size) {
#if 0

	if (size <= EMX_RINGBUF_CHUNK) {
		emx_ringbuf_readsmall(rb, buf, size);
	} else {
		emx_ringbuf_readlarge(rb, buf, size);
	}
#else
	void* write_buffer;
	emx_ringbuf_readsmall(rb, &write_buffer, sizeof(write_buffer));

	memcpy(buf, write_buffer, size);

	emx_ringbuf_write8(rb_back, 1);
#endif
}

static inline void emx_ringbuf_write(emx_ringbuf_t* rb, emx_ringbuf_t* rb_back, const void *buf, emx_ringbuf_size_t size) {
#if 0
	if (size <= EMX_RINGBUF_CHUNK) {
		emx_ringbuf_writesmall(rb, buf, size);
	} else {
		emx_ringbuf_writelarge(rb, buf, size);
	}
#else
	emx_ringbuf_writesmall(rb, &buf, sizeof(buf));

	emx_ringbuf_read8(rb_back);
#endif
}

#ifdef EMX_RINGBUF_IMPL_ARM_ATOMIC

static inline uint32_t arm_atomic_load(uint32_t* ptr) {
    uint32_t value;
    __asm__ volatile (
        "ldxr %w0, [%1]"
        : "=r" (value)
        : "r" (ptr)
        : "memory"
    );
    return value;
}

// Atomic Store
static inline void arm_atomic_store(uint32_t* ptr, uint32_t value) {
    __asm__ volatile (
        "stlr %w1, [%0]"
        :
        : "r" (ptr), "r" (value)
        : "memory"
    );
}

#endif

#endif
