#include "_emx_main.h"

EMX_COMM_CODE_SECTION(0, EMX_Send_0_1) void EMX_Send_0_1(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_0_1(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_0_1(buf, size);
}

EMX_COMM_CODE_SECTION(1, EMX_Recv_0_1) void EMX_Recv_0_1(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_0_1(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_0_1(buf, size);
}

EMX_COMM_CODE_SECTION(0, EMX_Send_0_2) void EMX_Send_0_2(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_0_2(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_0_2(buf, size);
}

EMX_COMM_CODE_SECTION(2, EMX_Recv_0_2) void EMX_Recv_0_2(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_0_2(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_0_2(buf, size);
}

EMX_COMM_CODE_SECTION(0, EMX_Send_0_3) void EMX_Send_0_3(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_0_3(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_0_3(buf, size);
}

EMX_COMM_CODE_SECTION(3, EMX_Recv_0_3) void EMX_Recv_0_3(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_0_3(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_0_3(buf, size);
}

EMX_COMM_CODE_SECTION(1, EMX_Send_1_0) void EMX_Send_1_0(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_1_0(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_1_0(buf, size);
}

EMX_COMM_CODE_SECTION(0, EMX_Recv_1_0) void EMX_Recv_1_0(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_1_0(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_1_0(buf, size);
}

EMX_COMM_CODE_SECTION(1, EMX_Send_1_2) void EMX_Send_1_2(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_1_2(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_1_2(buf, size);
}

EMX_COMM_CODE_SECTION(2, EMX_Recv_1_2) void EMX_Recv_1_2(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_1_2(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_1_2(buf, size);
}

EMX_COMM_CODE_SECTION(1, EMX_Send_1_3) void EMX_Send_1_3(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_1_3(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_1_3(buf, size);
}

EMX_COMM_CODE_SECTION(3, EMX_Recv_1_3) void EMX_Recv_1_3(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_1_3(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_1_3(buf, size);
}

EMX_COMM_CODE_SECTION(2, EMX_Send_2_0) void EMX_Send_2_0(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_2_0(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_2_0(buf, size);
}

EMX_COMM_CODE_SECTION(0, EMX_Recv_2_0) void EMX_Recv_2_0(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_2_0(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_2_0(buf, size);
}

EMX_COMM_CODE_SECTION(2, EMX_Send_2_1) void EMX_Send_2_1(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_2_1(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_2_1(buf, size);
}

EMX_COMM_CODE_SECTION(1, EMX_Recv_2_1) void EMX_Recv_2_1(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_2_1(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_2_1(buf, size);
}

EMX_COMM_CODE_SECTION(2, EMX_Send_2_3) void EMX_Send_2_3(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_2_3(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_2_3(buf, size);
}

EMX_COMM_CODE_SECTION(3, EMX_Recv_2_3) void EMX_Recv_2_3(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_2_3(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_2_3(buf, size);
}

EMX_COMM_CODE_SECTION(3, EMX_Send_3_0) void EMX_Send_3_0(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_3_0(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_3_0(buf, size);
}

EMX_COMM_CODE_SECTION(0, EMX_Recv_3_0) void EMX_Recv_3_0(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_3_0(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_3_0(buf, size);
}

EMX_COMM_CODE_SECTION(3, EMX_Send_3_1) void EMX_Send_3_1(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_3_1(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_3_1(buf, size);
}

EMX_COMM_CODE_SECTION(1, EMX_Recv_3_1) void EMX_Recv_3_1(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_3_1(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_3_1(buf, size);
}

EMX_COMM_CODE_SECTION(3, EMX_Send_3_2) void EMX_Send_3_2(const void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_send_3_2(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_send_3_2(buf, size);
}

EMX_COMM_CODE_SECTION(2, EMX_Recv_3_2) void EMX_Recv_3_2(void *buf, uint32_t size) {
	for (; size >= EMX_RINGBUF_CHUNK_SIZE; size -= EMX_RINGBUF_CHUNK_SIZE) {
		emx_recv_3_2(buf, EMX_RINGBUF_CHUNK_SIZE);
		buf += EMX_RINGBUF_CHUNK_SIZE;
	}

	emx_recv_3_2(buf, size);
}


static void (*emx_sync_functions[EMX_NUM_CORES][2])() = {
	{ EMX_Signal_0_3, EMX_Wait_1_0 },
	{ EMX_Signal_1_0, EMX_Wait_2_1 },
	{ EMX_Signal_2_1, EMX_Wait_3_2 },
	{ EMX_Signal_3_2, EMX_Wait_0_3 }
};

void EMX_SyncCores(int core) {
	if (core == EMX_NUM_CORES - 1) {
		(*emx_sync_functions[core][0])();
	}

	(*emx_sync_functions[core][1])();

	if (core != EMX_NUM_CORES - 1) {
		(*emx_sync_functions[core][0])();
	}
}
