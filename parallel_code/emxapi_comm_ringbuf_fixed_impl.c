#include "_emx_main.h"
#include <string.h>

#define EMX_ALIGNED(X) (X + (EMX_RINGBUF_ALIGN-1)) & (-EMX_RINGBUF_ALIGN)

volatile uint32_t* emx_wr_pos_0_1;
volatile uint32_t* emx_rd_pos_0_1;
volatile uint8_t* emx_rb_buf_0_1;

void emx_send_array_0_1(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_0_1;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_0_1 + *emx_wr_pos_0_1), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_0_1, *emx_wr_pos_0_1 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_0_1 + *emx_wr_pos_0_1), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_0_1, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_0_1, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_0_1(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_0_1;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_0_1 + *emx_rd_pos_0_1), size);
		EMX_ATOMICWRITE(*emx_rd_pos_0_1, *emx_rd_pos_0_1 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_0_1 + *emx_rd_pos_0_1), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_0_1, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_0_1, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_0_1(uint8_t val) {
	while (emx_ringbuf_free_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_0_1 + *emx_wr_pos_0_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_1, *emx_wr_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_1, 0);
	}
}

uint8_t EMX_Recv8_0_1() {
	uint8_t val;

	while (emx_ringbuf_size_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_0_1 + *emx_rd_pos_0_1);

	EMX_ATOMICWRITE(*emx_rd_pos_0_1, *emx_rd_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_1, 0);
	}

	return val;
}

void EMX_Send16_0_1(uint16_t val) {
	while (emx_ringbuf_free_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_0_1 + *emx_wr_pos_0_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_1, *emx_wr_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_1, 0);
	}
}

uint16_t EMX_Recv16_0_1() {
	uint16_t val;

	while (emx_ringbuf_size_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_0_1 + *emx_rd_pos_0_1);

	EMX_ATOMICWRITE(*emx_rd_pos_0_1, *emx_rd_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_1, 0);
	}

	return val;
}

void EMX_Send32_0_1(uint32_t val) {
	while (emx_ringbuf_free_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_0_1 + *emx_wr_pos_0_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_1, *emx_wr_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_1, 0);
	}
}

uint32_t EMX_Recv32_0_1() {
	uint32_t val;

	while (emx_ringbuf_size_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_0_1 + *emx_rd_pos_0_1);

	EMX_ATOMICWRITE(*emx_rd_pos_0_1, *emx_rd_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_1, 0);
	}

	return val;
}

void EMX_Send64_0_1(uint64_t val) {
	while (emx_ringbuf_free_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_0_1 + *emx_wr_pos_0_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_1, *emx_wr_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_1, 0);
	}
}

uint64_t EMX_Recv64_0_1() {
	uint64_t val;

	while (emx_ringbuf_size_0_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_0_1 + *emx_rd_pos_0_1);

	EMX_ATOMICWRITE(*emx_rd_pos_0_1, *emx_rd_pos_0_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_1, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_0_2;
volatile uint32_t* emx_rd_pos_0_2;
volatile uint8_t* emx_rb_buf_0_2;

void emx_send_array_0_2(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_0_2;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_0_2 + *emx_wr_pos_0_2), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_0_2, *emx_wr_pos_0_2 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_0_2 + *emx_wr_pos_0_2), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_0_2, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_0_2, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_0_2(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_0_2;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_0_2 + *emx_rd_pos_0_2), size);
		EMX_ATOMICWRITE(*emx_rd_pos_0_2, *emx_rd_pos_0_2 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_0_2 + *emx_rd_pos_0_2), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_0_2, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_0_2, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_0_2(uint8_t val) {
	while (emx_ringbuf_free_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_0_2 + *emx_wr_pos_0_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_2, *emx_wr_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_2, 0);
	}
}

uint8_t EMX_Recv8_0_2() {
	uint8_t val;

	while (emx_ringbuf_size_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_0_2 + *emx_rd_pos_0_2);

	EMX_ATOMICWRITE(*emx_rd_pos_0_2, *emx_rd_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_2, 0);
	}

	return val;
}

void EMX_Send16_0_2(uint16_t val) {
	while (emx_ringbuf_free_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_0_2 + *emx_wr_pos_0_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_2, *emx_wr_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_2, 0);
	}
}

uint16_t EMX_Recv16_0_2() {
	uint16_t val;

	while (emx_ringbuf_size_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_0_2 + *emx_rd_pos_0_2);

	EMX_ATOMICWRITE(*emx_rd_pos_0_2, *emx_rd_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_2, 0);
	}

	return val;
}

void EMX_Send32_0_2(uint32_t val) {
	while (emx_ringbuf_free_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_0_2 + *emx_wr_pos_0_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_2, *emx_wr_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_2, 0);
	}
}

uint32_t EMX_Recv32_0_2() {
	uint32_t val;

	while (emx_ringbuf_size_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_0_2 + *emx_rd_pos_0_2);

	EMX_ATOMICWRITE(*emx_rd_pos_0_2, *emx_rd_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_2, 0);
	}

	return val;
}

void EMX_Send64_0_2(uint64_t val) {
	while (emx_ringbuf_free_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_0_2 + *emx_wr_pos_0_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_2, *emx_wr_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_2, 0);
	}
}

uint64_t EMX_Recv64_0_2() {
	uint64_t val;

	while (emx_ringbuf_size_0_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_0_2 + *emx_rd_pos_0_2);

	EMX_ATOMICWRITE(*emx_rd_pos_0_2, *emx_rd_pos_0_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_2, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_0_3;
volatile uint32_t* emx_rd_pos_0_3;
volatile uint8_t* emx_rb_buf_0_3;

void emx_send_array_0_3(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_0_3;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_0_3 + *emx_wr_pos_0_3), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_0_3, *emx_wr_pos_0_3 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_0_3 + *emx_wr_pos_0_3), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_0_3, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_0_3, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_0_3(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_0_3;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_0_3 + *emx_rd_pos_0_3), size);
		EMX_ATOMICWRITE(*emx_rd_pos_0_3, *emx_rd_pos_0_3 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_0_3 + *emx_rd_pos_0_3), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_0_3, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_0_3, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_0_3(uint8_t val) {
	while (emx_ringbuf_free_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_0_3 + *emx_wr_pos_0_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_3, *emx_wr_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_3, 0);
	}
}

uint8_t EMX_Recv8_0_3() {
	uint8_t val;

	while (emx_ringbuf_size_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_0_3 + *emx_rd_pos_0_3);

	EMX_ATOMICWRITE(*emx_rd_pos_0_3, *emx_rd_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_3, 0);
	}

	return val;
}

void EMX_Send16_0_3(uint16_t val) {
	while (emx_ringbuf_free_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_0_3 + *emx_wr_pos_0_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_3, *emx_wr_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_3, 0);
	}
}

uint16_t EMX_Recv16_0_3() {
	uint16_t val;

	while (emx_ringbuf_size_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_0_3 + *emx_rd_pos_0_3);

	EMX_ATOMICWRITE(*emx_rd_pos_0_3, *emx_rd_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_3, 0);
	}

	return val;
}

void EMX_Send32_0_3(uint32_t val) {
	while (emx_ringbuf_free_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_0_3 + *emx_wr_pos_0_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_3, *emx_wr_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_3, 0);
	}
}

uint32_t EMX_Recv32_0_3() {
	uint32_t val;

	while (emx_ringbuf_size_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_0_3 + *emx_rd_pos_0_3);

	EMX_ATOMICWRITE(*emx_rd_pos_0_3, *emx_rd_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_3, 0);
	}

	return val;
}

void EMX_Send64_0_3(uint64_t val) {
	while (emx_ringbuf_free_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_0_3 + *emx_wr_pos_0_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_0_3, *emx_wr_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_0_3, 0);
	}
}

uint64_t EMX_Recv64_0_3() {
	uint64_t val;

	while (emx_ringbuf_size_0_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_0_3 + *emx_rd_pos_0_3);

	EMX_ATOMICWRITE(*emx_rd_pos_0_3, *emx_rd_pos_0_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_0_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_0_3, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_1_0;
volatile uint32_t* emx_rd_pos_1_0;
volatile uint8_t* emx_rb_buf_1_0;

void emx_send_array_1_0(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_1_0;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_1_0 + *emx_wr_pos_1_0), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_1_0, *emx_wr_pos_1_0 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_1_0 + *emx_wr_pos_1_0), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_1_0, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_1_0, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_1_0(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_1_0;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_1_0 + *emx_rd_pos_1_0), size);
		EMX_ATOMICWRITE(*emx_rd_pos_1_0, *emx_rd_pos_1_0 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_1_0 + *emx_rd_pos_1_0), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_1_0, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_1_0, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_1_0(uint8_t val) {
	while (emx_ringbuf_free_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_1_0 + *emx_wr_pos_1_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_0, *emx_wr_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_0, 0);
	}
}

uint8_t EMX_Recv8_1_0() {
	uint8_t val;

	while (emx_ringbuf_size_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_1_0 + *emx_rd_pos_1_0);

	EMX_ATOMICWRITE(*emx_rd_pos_1_0, *emx_rd_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_0, 0);
	}

	return val;
}

void EMX_Send16_1_0(uint16_t val) {
	while (emx_ringbuf_free_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_1_0 + *emx_wr_pos_1_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_0, *emx_wr_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_0, 0);
	}
}

uint16_t EMX_Recv16_1_0() {
	uint16_t val;

	while (emx_ringbuf_size_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_1_0 + *emx_rd_pos_1_0);

	EMX_ATOMICWRITE(*emx_rd_pos_1_0, *emx_rd_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_0, 0);
	}

	return val;
}

void EMX_Send32_1_0(uint32_t val) {
	while (emx_ringbuf_free_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_1_0 + *emx_wr_pos_1_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_0, *emx_wr_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_0, 0);
	}
}

uint32_t EMX_Recv32_1_0() {
	uint32_t val;

	while (emx_ringbuf_size_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_1_0 + *emx_rd_pos_1_0);

	EMX_ATOMICWRITE(*emx_rd_pos_1_0, *emx_rd_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_0, 0);
	}

	return val;
}

void EMX_Send64_1_0(uint64_t val) {
	while (emx_ringbuf_free_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_1_0 + *emx_wr_pos_1_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_0, *emx_wr_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_0, 0);
	}
}

uint64_t EMX_Recv64_1_0() {
	uint64_t val;

	while (emx_ringbuf_size_1_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_1_0 + *emx_rd_pos_1_0);

	EMX_ATOMICWRITE(*emx_rd_pos_1_0, *emx_rd_pos_1_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_0, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_1_2;
volatile uint32_t* emx_rd_pos_1_2;
volatile uint8_t* emx_rb_buf_1_2;

void emx_send_array_1_2(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_1_2;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_1_2 + *emx_wr_pos_1_2), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_1_2, *emx_wr_pos_1_2 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_1_2 + *emx_wr_pos_1_2), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_1_2, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_1_2, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_1_2(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_1_2;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_1_2 + *emx_rd_pos_1_2), size);
		EMX_ATOMICWRITE(*emx_rd_pos_1_2, *emx_rd_pos_1_2 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_1_2 + *emx_rd_pos_1_2), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_1_2, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_1_2, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_1_2(uint8_t val) {
	while (emx_ringbuf_free_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_1_2 + *emx_wr_pos_1_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_2, *emx_wr_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_2, 0);
	}
}

uint8_t EMX_Recv8_1_2() {
	uint8_t val;

	while (emx_ringbuf_size_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_1_2 + *emx_rd_pos_1_2);

	EMX_ATOMICWRITE(*emx_rd_pos_1_2, *emx_rd_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_2, 0);
	}

	return val;
}

void EMX_Send16_1_2(uint16_t val) {
	while (emx_ringbuf_free_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_1_2 + *emx_wr_pos_1_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_2, *emx_wr_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_2, 0);
	}
}

uint16_t EMX_Recv16_1_2() {
	uint16_t val;

	while (emx_ringbuf_size_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_1_2 + *emx_rd_pos_1_2);

	EMX_ATOMICWRITE(*emx_rd_pos_1_2, *emx_rd_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_2, 0);
	}

	return val;
}

void EMX_Send32_1_2(uint32_t val) {
	while (emx_ringbuf_free_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_1_2 + *emx_wr_pos_1_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_2, *emx_wr_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_2, 0);
	}
}

uint32_t EMX_Recv32_1_2() {
	uint32_t val;

	while (emx_ringbuf_size_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_1_2 + *emx_rd_pos_1_2);

	EMX_ATOMICWRITE(*emx_rd_pos_1_2, *emx_rd_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_2, 0);
	}

	return val;
}

void EMX_Send64_1_2(uint64_t val) {
	while (emx_ringbuf_free_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_1_2 + *emx_wr_pos_1_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_2, *emx_wr_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_2, 0);
	}
}

uint64_t EMX_Recv64_1_2() {
	uint64_t val;

	while (emx_ringbuf_size_1_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_1_2 + *emx_rd_pos_1_2);

	EMX_ATOMICWRITE(*emx_rd_pos_1_2, *emx_rd_pos_1_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_2, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_1_3;
volatile uint32_t* emx_rd_pos_1_3;
volatile uint8_t* emx_rb_buf_1_3;

void emx_send_array_1_3(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_1_3;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_1_3 + *emx_wr_pos_1_3), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_1_3, *emx_wr_pos_1_3 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_1_3 + *emx_wr_pos_1_3), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_1_3, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_1_3, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_1_3(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_1_3;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_1_3 + *emx_rd_pos_1_3), size);
		EMX_ATOMICWRITE(*emx_rd_pos_1_3, *emx_rd_pos_1_3 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_1_3 + *emx_rd_pos_1_3), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_1_3, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_1_3, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_1_3(uint8_t val) {
	while (emx_ringbuf_free_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_1_3 + *emx_wr_pos_1_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_3, *emx_wr_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_3, 0);
	}
}

uint8_t EMX_Recv8_1_3() {
	uint8_t val;

	while (emx_ringbuf_size_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_1_3 + *emx_rd_pos_1_3);

	EMX_ATOMICWRITE(*emx_rd_pos_1_3, *emx_rd_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_3, 0);
	}

	return val;
}

void EMX_Send16_1_3(uint16_t val) {
	while (emx_ringbuf_free_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_1_3 + *emx_wr_pos_1_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_3, *emx_wr_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_3, 0);
	}
}

uint16_t EMX_Recv16_1_3() {
	uint16_t val;

	while (emx_ringbuf_size_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_1_3 + *emx_rd_pos_1_3);

	EMX_ATOMICWRITE(*emx_rd_pos_1_3, *emx_rd_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_3, 0);
	}

	return val;
}

void EMX_Send32_1_3(uint32_t val) {
	while (emx_ringbuf_free_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_1_3 + *emx_wr_pos_1_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_3, *emx_wr_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_3, 0);
	}
}

uint32_t EMX_Recv32_1_3() {
	uint32_t val;

	while (emx_ringbuf_size_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_1_3 + *emx_rd_pos_1_3);

	EMX_ATOMICWRITE(*emx_rd_pos_1_3, *emx_rd_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_3, 0);
	}

	return val;
}

void EMX_Send64_1_3(uint64_t val) {
	while (emx_ringbuf_free_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_1_3 + *emx_wr_pos_1_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_1_3, *emx_wr_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_1_3, 0);
	}
}

uint64_t EMX_Recv64_1_3() {
	uint64_t val;

	while (emx_ringbuf_size_1_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_1_3 + *emx_rd_pos_1_3);

	EMX_ATOMICWRITE(*emx_rd_pos_1_3, *emx_rd_pos_1_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_1_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_1_3, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_2_0;
volatile uint32_t* emx_rd_pos_2_0;
volatile uint8_t* emx_rb_buf_2_0;

void emx_send_array_2_0(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_2_0;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_2_0 + *emx_wr_pos_2_0), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_2_0, *emx_wr_pos_2_0 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_2_0 + *emx_wr_pos_2_0), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_2_0, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_2_0, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_2_0(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_2_0;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_2_0 + *emx_rd_pos_2_0), size);
		EMX_ATOMICWRITE(*emx_rd_pos_2_0, *emx_rd_pos_2_0 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_2_0 + *emx_rd_pos_2_0), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_2_0, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_2_0, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_2_0(uint8_t val) {
	while (emx_ringbuf_free_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_2_0 + *emx_wr_pos_2_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_0, *emx_wr_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_0, 0);
	}
}

uint8_t EMX_Recv8_2_0() {
	uint8_t val;

	while (emx_ringbuf_size_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_2_0 + *emx_rd_pos_2_0);

	EMX_ATOMICWRITE(*emx_rd_pos_2_0, *emx_rd_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_0, 0);
	}

	return val;
}

void EMX_Send16_2_0(uint16_t val) {
	while (emx_ringbuf_free_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_2_0 + *emx_wr_pos_2_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_0, *emx_wr_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_0, 0);
	}
}

uint16_t EMX_Recv16_2_0() {
	uint16_t val;

	while (emx_ringbuf_size_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_2_0 + *emx_rd_pos_2_0);

	EMX_ATOMICWRITE(*emx_rd_pos_2_0, *emx_rd_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_0, 0);
	}

	return val;
}

void EMX_Send32_2_0(uint32_t val) {
	while (emx_ringbuf_free_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_2_0 + *emx_wr_pos_2_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_0, *emx_wr_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_0, 0);
	}
}

uint32_t EMX_Recv32_2_0() {
	uint32_t val;

	while (emx_ringbuf_size_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_2_0 + *emx_rd_pos_2_0);

	EMX_ATOMICWRITE(*emx_rd_pos_2_0, *emx_rd_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_0, 0);
	}

	return val;
}

void EMX_Send64_2_0(uint64_t val) {
	while (emx_ringbuf_free_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_2_0 + *emx_wr_pos_2_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_0, *emx_wr_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_0, 0);
	}
}

uint64_t EMX_Recv64_2_0() {
	uint64_t val;

	while (emx_ringbuf_size_2_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_2_0 + *emx_rd_pos_2_0);

	EMX_ATOMICWRITE(*emx_rd_pos_2_0, *emx_rd_pos_2_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_0, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_2_1;
volatile uint32_t* emx_rd_pos_2_1;
volatile uint8_t* emx_rb_buf_2_1;

void emx_send_array_2_1(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_2_1;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_2_1 + *emx_wr_pos_2_1), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_2_1, *emx_wr_pos_2_1 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_2_1 + *emx_wr_pos_2_1), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_2_1, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_2_1, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_2_1(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_2_1;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_2_1 + *emx_rd_pos_2_1), size);
		EMX_ATOMICWRITE(*emx_rd_pos_2_1, *emx_rd_pos_2_1 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_2_1 + *emx_rd_pos_2_1), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_2_1, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_2_1, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_2_1(uint8_t val) {
	while (emx_ringbuf_free_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_2_1 + *emx_wr_pos_2_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_1, *emx_wr_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_1, 0);
	}
}

uint8_t EMX_Recv8_2_1() {
	uint8_t val;

	while (emx_ringbuf_size_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_2_1 + *emx_rd_pos_2_1);

	EMX_ATOMICWRITE(*emx_rd_pos_2_1, *emx_rd_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_1, 0);
	}

	return val;
}

void EMX_Send16_2_1(uint16_t val) {
	while (emx_ringbuf_free_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_2_1 + *emx_wr_pos_2_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_1, *emx_wr_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_1, 0);
	}
}

uint16_t EMX_Recv16_2_1() {
	uint16_t val;

	while (emx_ringbuf_size_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_2_1 + *emx_rd_pos_2_1);

	EMX_ATOMICWRITE(*emx_rd_pos_2_1, *emx_rd_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_1, 0);
	}

	return val;
}

void EMX_Send32_2_1(uint32_t val) {
	while (emx_ringbuf_free_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_2_1 + *emx_wr_pos_2_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_1, *emx_wr_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_1, 0);
	}
}

uint32_t EMX_Recv32_2_1() {
	uint32_t val;

	while (emx_ringbuf_size_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_2_1 + *emx_rd_pos_2_1);

	EMX_ATOMICWRITE(*emx_rd_pos_2_1, *emx_rd_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_1, 0);
	}

	return val;
}

void EMX_Send64_2_1(uint64_t val) {
	while (emx_ringbuf_free_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_2_1 + *emx_wr_pos_2_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_1, *emx_wr_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_1, 0);
	}
}

uint64_t EMX_Recv64_2_1() {
	uint64_t val;

	while (emx_ringbuf_size_2_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_2_1 + *emx_rd_pos_2_1);

	EMX_ATOMICWRITE(*emx_rd_pos_2_1, *emx_rd_pos_2_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_1, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_2_3;
volatile uint32_t* emx_rd_pos_2_3;
volatile uint8_t* emx_rb_buf_2_3;

void emx_send_array_2_3(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_2_3;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_2_3 + *emx_wr_pos_2_3), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_2_3, *emx_wr_pos_2_3 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_2_3 + *emx_wr_pos_2_3), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_2_3, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_2_3, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_2_3(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_2_3;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_2_3 + *emx_rd_pos_2_3), size);
		EMX_ATOMICWRITE(*emx_rd_pos_2_3, *emx_rd_pos_2_3 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_2_3 + *emx_rd_pos_2_3), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_2_3, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_2_3, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_2_3(uint8_t val) {
	while (emx_ringbuf_free_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_2_3 + *emx_wr_pos_2_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_3, *emx_wr_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_3, 0);
	}
}

uint8_t EMX_Recv8_2_3() {
	uint8_t val;

	while (emx_ringbuf_size_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_2_3 + *emx_rd_pos_2_3);

	EMX_ATOMICWRITE(*emx_rd_pos_2_3, *emx_rd_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_3, 0);
	}

	return val;
}

void EMX_Send16_2_3(uint16_t val) {
	while (emx_ringbuf_free_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_2_3 + *emx_wr_pos_2_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_3, *emx_wr_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_3, 0);
	}
}

uint16_t EMX_Recv16_2_3() {
	uint16_t val;

	while (emx_ringbuf_size_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_2_3 + *emx_rd_pos_2_3);

	EMX_ATOMICWRITE(*emx_rd_pos_2_3, *emx_rd_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_3, 0);
	}

	return val;
}

void EMX_Send32_2_3(uint32_t val) {
	while (emx_ringbuf_free_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_2_3 + *emx_wr_pos_2_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_3, *emx_wr_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_3, 0);
	}
}

uint32_t EMX_Recv32_2_3() {
	uint32_t val;

	while (emx_ringbuf_size_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_2_3 + *emx_rd_pos_2_3);

	EMX_ATOMICWRITE(*emx_rd_pos_2_3, *emx_rd_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_3, 0);
	}

	return val;
}

void EMX_Send64_2_3(uint64_t val) {
	while (emx_ringbuf_free_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_2_3 + *emx_wr_pos_2_3) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_2_3, *emx_wr_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_2_3, 0);
	}
}

uint64_t EMX_Recv64_2_3() {
	uint64_t val;

	while (emx_ringbuf_size_2_3() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_2_3 + *emx_rd_pos_2_3);

	EMX_ATOMICWRITE(*emx_rd_pos_2_3, *emx_rd_pos_2_3 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_2_3 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_2_3, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_3_0;
volatile uint32_t* emx_rd_pos_3_0;
volatile uint8_t* emx_rb_buf_3_0;

void emx_send_array_3_0(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_3_0;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_3_0 + *emx_wr_pos_3_0), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_3_0, *emx_wr_pos_3_0 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_3_0 + *emx_wr_pos_3_0), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_3_0, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_3_0, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_3_0(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_3_0;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_3_0 + *emx_rd_pos_3_0), size);
		EMX_ATOMICWRITE(*emx_rd_pos_3_0, *emx_rd_pos_3_0 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_3_0 + *emx_rd_pos_3_0), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_3_0, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_3_0, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_3_0(uint8_t val) {
	while (emx_ringbuf_free_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_3_0 + *emx_wr_pos_3_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_0, *emx_wr_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_0, 0);
	}
}

uint8_t EMX_Recv8_3_0() {
	uint8_t val;

	while (emx_ringbuf_size_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_3_0 + *emx_rd_pos_3_0);

	EMX_ATOMICWRITE(*emx_rd_pos_3_0, *emx_rd_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_0, 0);
	}

	return val;
}

void EMX_Send16_3_0(uint16_t val) {
	while (emx_ringbuf_free_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_3_0 + *emx_wr_pos_3_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_0, *emx_wr_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_0, 0);
	}
}

uint16_t EMX_Recv16_3_0() {
	uint16_t val;

	while (emx_ringbuf_size_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_3_0 + *emx_rd_pos_3_0);

	EMX_ATOMICWRITE(*emx_rd_pos_3_0, *emx_rd_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_0, 0);
	}

	return val;
}

void EMX_Send32_3_0(uint32_t val) {
	while (emx_ringbuf_free_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_3_0 + *emx_wr_pos_3_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_0, *emx_wr_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_0, 0);
	}
}

uint32_t EMX_Recv32_3_0() {
	uint32_t val;

	while (emx_ringbuf_size_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_3_0 + *emx_rd_pos_3_0);

	EMX_ATOMICWRITE(*emx_rd_pos_3_0, *emx_rd_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_0, 0);
	}

	return val;
}

void EMX_Send64_3_0(uint64_t val) {
	while (emx_ringbuf_free_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_3_0 + *emx_wr_pos_3_0) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_0, *emx_wr_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_0, 0);
	}
}

uint64_t EMX_Recv64_3_0() {
	uint64_t val;

	while (emx_ringbuf_size_3_0() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_3_0 + *emx_rd_pos_3_0);

	EMX_ATOMICWRITE(*emx_rd_pos_3_0, *emx_rd_pos_3_0 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_0 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_0, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_3_1;
volatile uint32_t* emx_rd_pos_3_1;
volatile uint8_t* emx_rb_buf_3_1;

void emx_send_array_3_1(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_3_1;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_3_1 + *emx_wr_pos_3_1), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_3_1, *emx_wr_pos_3_1 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_3_1 + *emx_wr_pos_3_1), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_3_1, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_3_1, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_3_1(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_3_1;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_3_1 + *emx_rd_pos_3_1), size);
		EMX_ATOMICWRITE(*emx_rd_pos_3_1, *emx_rd_pos_3_1 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_3_1 + *emx_rd_pos_3_1), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_3_1, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_3_1, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_3_1(uint8_t val) {
	while (emx_ringbuf_free_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_3_1 + *emx_wr_pos_3_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_1, *emx_wr_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_1, 0);
	}
}

uint8_t EMX_Recv8_3_1() {
	uint8_t val;

	while (emx_ringbuf_size_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_3_1 + *emx_rd_pos_3_1);

	EMX_ATOMICWRITE(*emx_rd_pos_3_1, *emx_rd_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_1, 0);
	}

	return val;
}

void EMX_Send16_3_1(uint16_t val) {
	while (emx_ringbuf_free_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_3_1 + *emx_wr_pos_3_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_1, *emx_wr_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_1, 0);
	}
}

uint16_t EMX_Recv16_3_1() {
	uint16_t val;

	while (emx_ringbuf_size_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_3_1 + *emx_rd_pos_3_1);

	EMX_ATOMICWRITE(*emx_rd_pos_3_1, *emx_rd_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_1, 0);
	}

	return val;
}

void EMX_Send32_3_1(uint32_t val) {
	while (emx_ringbuf_free_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_3_1 + *emx_wr_pos_3_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_1, *emx_wr_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_1, 0);
	}
}

uint32_t EMX_Recv32_3_1() {
	uint32_t val;

	while (emx_ringbuf_size_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_3_1 + *emx_rd_pos_3_1);

	EMX_ATOMICWRITE(*emx_rd_pos_3_1, *emx_rd_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_1, 0);
	}

	return val;
}

void EMX_Send64_3_1(uint64_t val) {
	while (emx_ringbuf_free_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_3_1 + *emx_wr_pos_3_1) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_1, *emx_wr_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_1, 0);
	}
}

uint64_t EMX_Recv64_3_1() {
	uint64_t val;

	while (emx_ringbuf_size_3_1() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_3_1 + *emx_rd_pos_3_1);

	EMX_ATOMICWRITE(*emx_rd_pos_3_1, *emx_rd_pos_3_1 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_1 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_1, 0);
	}

	return val;
}

volatile uint32_t* emx_wr_pos_3_2;
volatile uint32_t* emx_rd_pos_3_2;
volatile uint8_t* emx_rb_buf_3_2;

void emx_send_array_3_2(const void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_wr_pos_3_2;
	if (remain >= size) {
		memcpy((void*)(emx_rb_buf_3_2 + *emx_wr_pos_3_2), (uint8_t*)buf, size);
		EMX_ATOMICWRITE(*emx_wr_pos_3_2, *emx_wr_pos_3_2 + EMX_ALIGNED(size));
	} else {
		memcpy((void*)(emx_rb_buf_3_2 + *emx_wr_pos_3_2), (uint8_t*)buf, remain);
		memcpy((void*)emx_rb_buf_3_2, (uint8_t*)buf + remain, size - remain);
		EMX_ATOMICWRITE(*emx_wr_pos_3_2, EMX_ALIGNED(size - remain));
	}
}

void emx_recv_array_3_2(void *buf, uint32_t size) {
	uint32_t remain = EMX_RINGBUF_SIZE - *emx_rd_pos_3_2;
	if (remain >= size) {
		memcpy(buf, (void*)(emx_rb_buf_3_2 + *emx_rd_pos_3_2), size);
		EMX_ATOMICWRITE(*emx_rd_pos_3_2, *emx_rd_pos_3_2 + EMX_ALIGNED(size));
	} else {
		memcpy(buf, (void*)(emx_rb_buf_3_2 + *emx_rd_pos_3_2), remain);
		memcpy(buf + remain, (void*)emx_rb_buf_3_2, size - remain);
		EMX_ATOMICWRITE(*emx_rd_pos_3_2, EMX_ALIGNED(size - remain));
	}
}

void EMX_Send8_3_2(uint8_t val) {
	while (emx_ringbuf_free_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint8_t*)(emx_rb_buf_3_2 + *emx_wr_pos_3_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_2, *emx_wr_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_2, 0);
	}
}

uint8_t EMX_Recv8_3_2() {
	uint8_t val;

	while (emx_ringbuf_size_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint8_t*)(emx_rb_buf_3_2 + *emx_rd_pos_3_2);

	EMX_ATOMICWRITE(*emx_rd_pos_3_2, *emx_rd_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_2, 0);
	}

	return val;
}

void EMX_Send16_3_2(uint16_t val) {
	while (emx_ringbuf_free_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint16_t*)(emx_rb_buf_3_2 + *emx_wr_pos_3_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_2, *emx_wr_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_2, 0);
	}
}

uint16_t EMX_Recv16_3_2() {
	uint16_t val;

	while (emx_ringbuf_size_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint16_t*)(emx_rb_buf_3_2 + *emx_rd_pos_3_2);

	EMX_ATOMICWRITE(*emx_rd_pos_3_2, *emx_rd_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_2, 0);
	}

	return val;
}

void EMX_Send32_3_2(uint32_t val) {
	while (emx_ringbuf_free_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint32_t*)(emx_rb_buf_3_2 + *emx_wr_pos_3_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_2, *emx_wr_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_2, 0);
	}
}

uint32_t EMX_Recv32_3_2() {
	uint32_t val;

	while (emx_ringbuf_size_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint32_t*)(emx_rb_buf_3_2 + *emx_rd_pos_3_2);

	EMX_ATOMICWRITE(*emx_rd_pos_3_2, *emx_rd_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_2, 0);
	}

	return val;
}

void EMX_Send64_3_2(uint64_t val) {
	while (emx_ringbuf_free_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	*(uint64_t*)(emx_rb_buf_3_2 + *emx_wr_pos_3_2) = val;

	EMX_ATOMICWRITE(*emx_wr_pos_3_2, *emx_wr_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_wr_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_wr_pos_3_2, 0);
	}
}

uint64_t EMX_Recv64_3_2() {
	uint64_t val;

	while (emx_ringbuf_size_3_2() < EMX_RINGBUF_ALIGN) {
		emx_ringbuf_wait();
	}

	val = *(uint64_t*)(emx_rb_buf_3_2 + *emx_rd_pos_3_2);

	EMX_ATOMICWRITE(*emx_rd_pos_3_2, *emx_rd_pos_3_2 + EMX_RINGBUF_ALIGN);

	if (*emx_rd_pos_3_2 >= EMX_RINGBUF_SIZE) {
		EMX_ATOMICWRITE(*emx_rd_pos_3_2, 0);
	}

	return val;
}

