#include "emx_ringbuf.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

static int wait_var = 0;

void emx_ringbuf_wait() {
	int i;
	for (i=0; i<100; ++i) {
		wait_var = wait_var * 3 + 1;
	}
}

void emx_ringbuf_init(emx_ringbuf_t* rb, emx_ringbuf_size_t size, void *buf) {
	rb->size = size;
	rb->elements = size / EMX_RINGBUF_ALIGN;
	rb->buf = (emx_ringbuf_element_t*)buf;
	rb->rd_pos = 0;
	rb->wr_pos = 0;

	assert(EMX_RINGBUF_SIZE == size);
}

void emx_ringbuf_readlarge(emx_ringbuf_t* rb, void* buffer, emx_ringbuf_size_t size) {
	uint8_t* curbuf = (uint8_t*) buffer;
	emx_ringbuf_size_t chunkSize = EMX_RINGBUF_CHUNK;

	while (size > chunkSize) {  // read in chunks
		emx_ringbuf_readsmall(rb, curbuf, chunkSize);
		curbuf += chunkSize;
		size -= chunkSize;
	}

	emx_ringbuf_readsmall(rb, curbuf, size);
}

void emx_ringbuf_writelarge(emx_ringbuf_t* rb, const void* _buffer, emx_ringbuf_size_t size) {
	uint8_t* curbuf = (uint8_t*)_buffer;
	emx_ringbuf_size_t chunkSize = EMX_RINGBUF_CHUNK;

	while (size > chunkSize) {  // write in chunks
		emx_ringbuf_writesmall(rb, curbuf, chunkSize);
		curbuf += chunkSize;
		size -= chunkSize;
	}

	emx_ringbuf_writesmall(rb, curbuf, size);
}
