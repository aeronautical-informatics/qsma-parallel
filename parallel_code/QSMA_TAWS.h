/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: QSMA_TAWS.h
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

#ifndef RTW_HEADER_QSMA_TAWS_h_
#define RTW_HEADER_QSMA_TAWS_h_
#include <math.h>
#ifndef QSMA_TAWS_COMMON_INCLUDES_
# define QSMA_TAWS_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* QSMA_TAWS_COMMON_INCLUDES_ */

#include "QSMA_TAWS_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  real_T In1;                          /* '<S12>/In1' */
} B_QSMA_TAWS_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Delay_DSTATE;                 /* '<S6>/Delay' */
  int32_T Delay_DSTATE_l;              /* '<S2>/Delay' */
} DW_QSMA_TAWS_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  int32_T height_ground;               /* '<Root>/height_ground' */
  int32_T climb_rate;                  /* '<Root>/climb_rate' */
  real_T pitch;                        /* '<Root>/pitch' */
  int32_T timestamp;                   /* '<Root>/timestamp' */
  int32_T height_sea;                  /* '<Root>/height_sea' */
  real_T position_lat;                 /* '<Root>/position_lat' */
  real_T position_lon;                 /* '<Root>/position_lon' */
  real_T ground_speed;                 /* '<Root>/ground_speed' */
  real_T air_speed;                    /* '<Root>/air_speed' */
  real_T heading;                      /* '<Root>/heading' */
  real_T roll;                         /* '<Root>/roll' */
  real_T Inhibit_FLTA_PDA1;            /* '<Root>/Inhibit_FLTA_PDA1' */
  real_T runway_pos_lat;               /* '<Root>/runway_pos_lat' */
  real_T runway_pos_long;              /* '<Root>/runway_pos_long' */
  real_T runway_height_sea;            /* '<Root>/runway_height_sea' */
} ExtU_QSMA_TAWS_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  int8_T currentalert;                 /* '<Root>/current alert' */
} ExtY_QSMA_TAWS_T;

/* Real-time Model Data Structure */
struct tag_RTM_QSMA_TAWS_T {
  const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_QSMA_TAWS_T QSMA_TAWS_B;

/* Block states (default storage) */
extern DW_QSMA_TAWS_T QSMA_TAWS_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_QSMA_TAWS_T QSMA_TAWS_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_QSMA_TAWS_T QSMA_TAWS_Y;

/* Model entry point functions */
extern void QSMA_TAWS_initialize(void);
extern void QSMA_TAWS_step(void);
extern void QSMA_TAWS_terminate(void);
extern void ExternalComm(void);

/* Real-time Model object */
extern RT_MODEL_QSMA_TAWS_T *const QSMA_TAWS_M;


/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'QSMA_TAWS'
 * '<S1>'   : 'QSMA_TAWS/TAWS'
 * '<S2>'   : 'QSMA_TAWS/TAWS/500 ft. Callout'
 * '<S3>'   : 'QSMA_TAWS/TAWS/Alert Prioritisation Scheme'
 * '<S4>'   : 'QSMA_TAWS/TAWS/Flight Environment '
 * '<S5>'   : 'QSMA_TAWS/TAWS/Mode 1'
 * '<S6>'   : 'QSMA_TAWS/TAWS/Mode 3'
 * '<S7>'   : 'QSMA_TAWS/TAWS/Premature Descent Alert'
 * '<S8>'   : 'QSMA_TAWS/TAWS/500 ft. Callout/If Action Subsystem'
 * '<S9>'   : 'QSMA_TAWS/TAWS/500 ft. Callout/If Action Subsystem1'
 * '<S10>'  : 'QSMA_TAWS/TAWS/Flight Environment /MATLAB Function'
 * '<S11>'  : 'QSMA_TAWS/TAWS/Mode 1/MATLAB Function'
 * '<S12>'  : 'QSMA_TAWS/TAWS/Mode 3/If Action Subsystem'
 * '<S13>'  : 'QSMA_TAWS/TAWS/Mode 3/MATLAB Function'
 * '<S14>'  : 'QSMA_TAWS/TAWS/Premature Descent Alert/MATLAB Function'
 */
#endif                                 /* RTW_HEADER_QSMA_TAWS_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
