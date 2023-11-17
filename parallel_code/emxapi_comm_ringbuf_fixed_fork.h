#ifndef EMXAPI_COMM_RINGBUF_FIXED_FORK_H
#define EMXAPI_COMM_RINGBUF_FIXED_FORK_H

#include <stdint.h>
#include "emxapi_comm_ringbuf_config.h"

#define EMX_COMM_CODE_SECTION(LOC, NAME)

#define emx_ringbuf_wait_0 emx_ringbuf_wait
extern volatile uint8_t* emx_rb_buf_0_1;
extern volatile uint32_t* emx_wr_pos_0_1;
extern volatile uint32_t* emx_rd_pos_0_1;
#define emx_loc_wr_pos_0_1 emx_wr_pos_0_1
#define emx_loc_rd_pos_0_1 emx_rd_pos_0_1
extern volatile uint8_t* emx_rb_buf_0_2;
extern volatile uint32_t* emx_wr_pos_0_2;
extern volatile uint32_t* emx_rd_pos_0_2;
#define emx_loc_wr_pos_0_2 emx_wr_pos_0_2
#define emx_loc_rd_pos_0_2 emx_rd_pos_0_2
extern volatile uint8_t* emx_rb_buf_0_3;
extern volatile uint32_t* emx_wr_pos_0_3;
extern volatile uint32_t* emx_rd_pos_0_3;
#define emx_loc_wr_pos_0_3 emx_wr_pos_0_3
#define emx_loc_rd_pos_0_3 emx_rd_pos_0_3
#define emx_ringbuf_wait_1 emx_ringbuf_wait
extern volatile uint8_t* emx_rb_buf_1_0;
extern volatile uint32_t* emx_wr_pos_1_0;
extern volatile uint32_t* emx_rd_pos_1_0;
#define emx_loc_wr_pos_1_0 emx_wr_pos_1_0
#define emx_loc_rd_pos_1_0 emx_rd_pos_1_0
extern volatile uint8_t* emx_rb_buf_1_2;
extern volatile uint32_t* emx_wr_pos_1_2;
extern volatile uint32_t* emx_rd_pos_1_2;
#define emx_loc_wr_pos_1_2 emx_wr_pos_1_2
#define emx_loc_rd_pos_1_2 emx_rd_pos_1_2
extern volatile uint8_t* emx_rb_buf_1_3;
extern volatile uint32_t* emx_wr_pos_1_3;
extern volatile uint32_t* emx_rd_pos_1_3;
#define emx_loc_wr_pos_1_3 emx_wr_pos_1_3
#define emx_loc_rd_pos_1_3 emx_rd_pos_1_3
#define emx_ringbuf_wait_2 emx_ringbuf_wait
extern volatile uint8_t* emx_rb_buf_2_0;
extern volatile uint32_t* emx_wr_pos_2_0;
extern volatile uint32_t* emx_rd_pos_2_0;
#define emx_loc_wr_pos_2_0 emx_wr_pos_2_0
#define emx_loc_rd_pos_2_0 emx_rd_pos_2_0
extern volatile uint8_t* emx_rb_buf_2_1;
extern volatile uint32_t* emx_wr_pos_2_1;
extern volatile uint32_t* emx_rd_pos_2_1;
#define emx_loc_wr_pos_2_1 emx_wr_pos_2_1
#define emx_loc_rd_pos_2_1 emx_rd_pos_2_1
extern volatile uint8_t* emx_rb_buf_2_3;
extern volatile uint32_t* emx_wr_pos_2_3;
extern volatile uint32_t* emx_rd_pos_2_3;
#define emx_loc_wr_pos_2_3 emx_wr_pos_2_3
#define emx_loc_rd_pos_2_3 emx_rd_pos_2_3
#define emx_ringbuf_wait_3 emx_ringbuf_wait
extern volatile uint8_t* emx_rb_buf_3_0;
extern volatile uint32_t* emx_wr_pos_3_0;
extern volatile uint32_t* emx_rd_pos_3_0;
#define emx_loc_wr_pos_3_0 emx_wr_pos_3_0
#define emx_loc_rd_pos_3_0 emx_rd_pos_3_0
extern volatile uint8_t* emx_rb_buf_3_1;
extern volatile uint32_t* emx_wr_pos_3_1;
extern volatile uint32_t* emx_rd_pos_3_1;
#define emx_loc_wr_pos_3_1 emx_wr_pos_3_1
#define emx_loc_rd_pos_3_1 emx_rd_pos_3_1
extern volatile uint8_t* emx_rb_buf_3_2;
extern volatile uint32_t* emx_wr_pos_3_2;
extern volatile uint32_t* emx_rd_pos_3_2;
#define emx_loc_wr_pos_3_2 emx_wr_pos_3_2
#define emx_loc_rd_pos_3_2 emx_rd_pos_3_2

#include "emxapi_comm_ringbuf_fixed.h"

void EMX_Run();
void EMX_Join();

extern int emx_core;

static inline int EMX_GetCore() {
	return emx_core;
}

#endif
