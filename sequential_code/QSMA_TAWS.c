/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: QSMA_TAWS.c
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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include <fcntl.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>

#include "errors.h"


#include "QSMA_TAWS.h"
#include "QSMA_TAWS_private.h"

/* Block signals (default storage) */
B_QSMA_TAWS_T QSMA_TAWS_B;

/* Block states (default storage) */
DW_QSMA_TAWS_T QSMA_TAWS_DW;

/* External inputs (root inport signals with default storage) */
ExtU_QSMA_TAWS_T QSMA_TAWS_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_QSMA_TAWS_T QSMA_TAWS_Y;

/* Real-time model */
RT_MODEL_QSMA_TAWS_T QSMA_TAWS_M_;
RT_MODEL_QSMA_TAWS_T *const QSMA_TAWS_M = &QSMA_TAWS_M_;

/* Forward declaration for local functions */
static void QSMA_TAWS_contrib(real_T x1, real_T b_y1, real_T x2, real_T y2,
  int8_T quad1, int8_T quad2, real_T scale, int8_T *diffQuad, boolean_T *onj);
static boolean_T QSMA_TAWS_scalarInpolygon(int32_T xj, int32_T yj, const real_T
  xv[5], const real_T yv[5], int32_T nloops, const int32_T first[5], const
  int32_T last[5], real_T minxv, real_T maxxv, real_T minyv, real_T maxyv, const
  real_T scale[5]);

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
static void QSMA_TAWS_contrib(real_T x1, real_T b_y1, real_T x2, real_T y2,
  int8_T quad1, int8_T quad2, real_T scale, int8_T *diffQuad, boolean_T *onj)
{
  real_T cp;
  *onj = false;
  *diffQuad = (int8_T)(quad2 - quad1);
  cp = x1 * y2 - x2 * b_y1;
  if (fabs(cp) < scale) {
    *onj = (x1 * x2 + b_y1 * y2 <= 0.0);
    if ((*diffQuad == 2) || (*diffQuad == -2)) {
      *diffQuad = 0;
    } else {
      switch (*diffQuad) {
       case -3:
        *diffQuad = 1;
        break;

       case 3:
        *diffQuad = -1;
        break;
      }
    }
  } else if (cp < 0.0) {
    switch (*diffQuad) {
     case 2:
      *diffQuad = -2;
      break;

     case -3:
      *diffQuad = 1;
      break;

     case 3:
      *diffQuad = -1;
      break;
    }
  } else {
    switch (*diffQuad) {
     case -2:
      *diffQuad = 2;
      break;

     case -3:
      *diffQuad = 1;
      break;

     case 3:
      *diffQuad = -1;
      break;
    }
  }
}

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
static boolean_T QSMA_TAWS_scalarInpolygon(int32_T xj, int32_T yj, const real_T
  xv[5], const real_T yv[5], int32_T nloops, const int32_T first[5], const
  int32_T last[5], real_T minxv, real_T maxxv, real_T minyv, real_T maxyv, const
  real_T scale[5])
{
  boolean_T inj;
  int8_T sdq;
  real_T xvFirst;
  real_T yvFirst;
  int8_T quadFirst;
  real_T xv2;
  real_T yv2;
  int8_T quad2;
  real_T xv1;
  real_T yv1;
  int8_T quad1;
  boolean_T onj;
  int8_T b_dquad;
  int32_T b_k;
  int32_T i;
  int32_T exitg1;
  int32_T exitg2;
  inj = false;
  if ((xj >= minxv) && (xj <= maxxv) && (yj >= minyv) && (yj <= maxyv)) {
    sdq = 0;
    b_k = 0;
    do {
      exitg2 = 0;
      if (b_k <= nloops - 1) {
        xvFirst = xv[first[b_k] - 1] - (real_T)xj;
        yvFirst = yv[first[b_k] - 1] - (real_T)yj;
        if (xvFirst > 0.0) {
          if (yvFirst > 0.0) {
            quadFirst = 0;
          } else {
            quadFirst = 3;
          }
        } else if (yvFirst > 0.0) {
          quadFirst = 1;
        } else {
          quadFirst = 2;
        }

        xv2 = xvFirst;
        yv2 = yvFirst;
        quad2 = quadFirst;
        i = first[b_k];
        do {
          exitg1 = 0;
          if (i <= last[b_k] - 1) {
            xv1 = xv2;
            yv1 = yv2;
            xv2 = xv[i] - (real_T)xj;
            yv2 = yv[i] - (real_T)yj;
            quad1 = quad2;
            if (xv2 > 0.0) {
              if (yv2 > 0.0) {
                quad2 = 0;
              } else {
                quad2 = 3;
              }
            } else if (yv2 > 0.0) {
              quad2 = 1;
            } else {
              quad2 = 2;
            }

            QSMA_TAWS_contrib(xv1, yv1, xv2, yv2, quad1, quad2, scale[i - 1],
                              &b_dquad, &onj);
            if (onj) {
              inj = true;
              exitg1 = 1;
            } else {
              sdq += b_dquad;
              i++;
            }
          } else {
            QSMA_TAWS_contrib(xv2, yv2, xvFirst, yvFirst, quad2, quadFirst,
                              scale[last[b_k] - 1], &quad1, &onj);
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = 1;
        } else if (onj) {
          inj = true;
          exitg2 = 1;
        } else {
          sdq += quad1;
          b_k++;
        }
      } else {
        inj = (sdq != 0);
        exitg2 = 1;
      }
    } while (exitg2 == 0);
  }

  return inj;
}

/* Model step function */
void QSMA_TAWS_step(void)
{
  real_T scale;
  real_T absxk;
  real_T t;
  boolean_T out;
  int8_T sdq;
  int8_T quadFirst;
  int8_T quad2;
  real_T xv1;
  real_T yv1;
  int8_T quad1;
  int8_T b_dquad;
  int32_T rtb_Switch2;
  int32_T rtb_mode3;
  real_T rtb_Subtract;
  int32_T distance_runway;
  static const real_T b[5] = { 1600.0, 1850.0, 10100.0, 12000.0, 12000.0 };

  static const real_T c[5] = { 100.0, 300.0, 1958.0, 1958.0, 100.0 };

  static const int32_T d[5] = { 1, 0, 0, 0, 0 };

  static const int32_T e[5] = { 5, 0, 0, 0, 0 };

  static const real_T f[5] = { 2.298161660974074E-10, 4.4935888343644592E-9,
    1.4412404603092455E-8, 8.22542034484286E-9, 4.5297099404706387E-10 };

  static const real_T g[5] = { 1560.0, 2200.0, 5700.0, 12000.0, 12000.0 };

  static const real_T h[5] = { 100.0, 630.0, 2200.0, 2200.0, 100.0 };

  static const real_T i[5] = { 4.5710102369866945E-10, 3.7231884242316937E-9,
    1.2969625373671079E-8, 9.1926466438962962E-9, 4.5163872641751368E-10 };

  static const real_T b_0[6] = { 1.0, 1.0, 1.8, 2.3, 5.0, 5.0 };

  static const int16_T b_1[4] = { 207, 533, 7000, 7000 };

  static const uint8_T c_0[6] = { 10U, 80U, 150U, 170U, 170U, 10U };

  static const int16_T c_1[4] = { 60, 600, 600, 60 };

  static const real_T scale_0[6] = { 2.9976021664879227E-14,
    1.0724754417879012E-13, 2.1849189124623081E-13, 4.1333603206794578E-13,
    2.9976021664879227E-13, 1.9984014443252818E-14 };

  static const real_T scale_1[4] = { 8.1334938784038968E-11,
    1.5053958080102348E-9, 1.538769112130467E-9, 1.4402479209252306E-10 };

  boolean_T guard1 = false;
  int32_T exitg1;

  /* MATLAB Function: '<S5>/MATLAB Function' incorporates:
   *  Inport: '<Root>/pitch'
   */
  guard1 = false;
  if (QSMA_TAWS_U.pitch < 0.0) {
    if (QSMA_TAWS_scalarInpolygon(QSMA_TAWS_U.climb_rate,
         QSMA_TAWS_U.height_ground, b, c, 1, d, e, 1600.0, 12000.0, 100.0,
         1958.0, f)) {
      /* Switch: '<S3>/Switch' */
      rtb_Switch2 = 1;
    } else if (QSMA_TAWS_scalarInpolygon(QSMA_TAWS_U.climb_rate,
                QSMA_TAWS_U.height_ground, g, h, 1, d, e, 1560.0, 12000.0, 100.0,
                2200.0, i)) {
      /* Switch: '<S3>/Switch' */
      rtb_Switch2 = 2;
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    /* If: '<S2>/If' incorporates:
     *  Delay: '<S2>/Delay'
     *  Inport: '<Root>/height_ground'
     */
    if ((QSMA_TAWS_DW.Delay_DSTATE_l > 500) && (QSMA_TAWS_U.height_ground <= 500))
    {
      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem' incorporates:
       *  ActionPort: '<S8>/Action Port'
       */
      /* Switch: '<S3>/Switch' incorporates:
       *  Constant: '<S2>/Constant'
       *  Inport: '<S8>/In1'
       */
      rtb_Switch2 = 3;

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem' */
    } else {
      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem1' incorporates:
       *  ActionPort: '<S9>/Action Port'
       */
      /* Switch: '<S3>/Switch' incorporates:
       *  Constant: '<S2>/Constant1'
       *  Inport: '<S9>/In1'
       */
      rtb_Switch2 = 0;

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem1' */
    }

    /* End of If: '<S2>/If' */
  }

  /* End of MATLAB Function: '<S5>/MATLAB Function' */

  /* Sum: '<S4>/Subtract' incorporates:
   *  Inport: '<Root>/height_sea'
   *  Inport: '<Root>/runway_height_sea'
   */
  rtb_Subtract = (real_T)QSMA_TAWS_U.height_sea - QSMA_TAWS_U.runway_height_sea;

  /* MATLAB Function: '<S4>/MATLAB Function' incorporates:
   *  Inport: '<Root>/position_lat'
   *  Inport: '<Root>/position_lon'
   *  Inport: '<Root>/runway_pos_lat'
   *  Inport: '<Root>/runway_pos_long'
   */
  scale = 3.3121686421112381E-170;
  absxk = fabs(QSMA_TAWS_U.runway_pos_long - QSMA_TAWS_U.position_lon);
  if (absxk > 3.3121686421112381E-170) {
    xv1 = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    xv1 = t * t;
  }

  absxk = fabs(QSMA_TAWS_U.runway_pos_lat - QSMA_TAWS_U.position_lat);
  if (absxk > scale) {
    t = scale / absxk;
    xv1 = xv1 * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    xv1 += t * t;
  }

  xv1 = scale * sqrt(xv1);
  scale = xv1 * 60.0;

  /* If: '<S6>/If' incorporates:
   *  Constant: '<S6>/Constant'
   *  Delay: '<S6>/Delay'
   *  Inport: '<S12>/In1'
   *  MATLAB Function: '<S4>/MATLAB Function'
   */
  if (scale > QSMA_TAWS_DW.Delay_DSTATE) {
    /* Outputs for IfAction SubSystem: '<S6>/If Action Subsystem' incorporates:
     *  ActionPort: '<S12>/Action Port'
     */
    QSMA_TAWS_B.In1 = 1.0;

    /* End of Outputs for SubSystem: '<S6>/If Action Subsystem' */
  }

  /* End of If: '<S6>/If' */

  /* MATLAB Function: '<S6>/MATLAB Function' */
  rtb_mode3 = 0;

  /* MATLAB Function: '<S4>/MATLAB Function' */
  if ((scale <= 3.0) && (rtb_Subtract <= 1000.0)) {
    distance_runway = 1;
  } else if (scale >= 5.0) {
    distance_runway = 2;
  } else {
    distance_runway = 0;
  }

  /* MATLAB Function: '<S6>/MATLAB Function' incorporates:
   *  Inport: '<Root>/climb_rate'
   *  Inport: '<Root>/pitch'
   */
  if ((QSMA_TAWS_U.pitch < 0.0) && (QSMA_TAWS_B.In1 == 1.0) && (distance_runway ==
       1)) {
    out = false;
    if ((QSMA_TAWS_U.climb_rate >= 207) && (QSMA_TAWS_U.climb_rate <= 7000) &&
        (rtb_Subtract >= 60.0) && (rtb_Subtract <= 600.0)) {
      sdq = 0;
      if (207.0 - (real_T)QSMA_TAWS_U.climb_rate > 0.0) {
        quadFirst = 3;
      } else {
        quadFirst = 2;
      }

      absxk = 207.0 - (real_T)QSMA_TAWS_U.climb_rate;
      t = 60.0 - rtb_Subtract;
      quad2 = quadFirst;
      distance_runway = 1;
      do {
        exitg1 = 0;
        if (distance_runway <= 3) {
          xv1 = absxk;
          yv1 = t;
          absxk = (real_T)b_1[distance_runway] - (real_T)QSMA_TAWS_U.climb_rate;
          t = (real_T)c_1[distance_runway] - rtb_Subtract;
          quad1 = quad2;
          if (absxk > 0.0) {
            if (t > 0.0) {
              quad2 = 0;
            } else {
              quad2 = 3;
            }
          } else if (t > 0.0) {
            quad2 = 1;
          } else {
            quad2 = 2;
          }

          QSMA_TAWS_contrib(xv1, yv1, absxk, t, quad1, quad2,
                            scale_1[distance_runway - 1], &b_dquad, &out);
          if (out) {
            out = true;
            exitg1 = 1;
          } else {
            sdq += b_dquad;
            distance_runway++;
          }
        } else {
          QSMA_TAWS_contrib(absxk, t, 207.0 - (real_T)QSMA_TAWS_U.climb_rate,
                            60.0 - rtb_Subtract, quad2, quadFirst,
                            1.4402479209252306E-10, &quad1, &out);
          if (out) {
            out = true;
          } else {
            sdq += quad1;
            out = (sdq != 0);
          }

          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }

    if (out) {
      rtb_mode3 = 4;
    }
  }

  /* Switch: '<S3>/Switch1' */
  if (rtb_Switch2 <= 0) {
    rtb_Switch2 = rtb_mode3;
  }

  /* End of Switch: '<S3>/Switch1' */

  /* MATLAB Function: '<S7>/MATLAB Function' incorporates:
   *  MATLAB Function: '<S4>/MATLAB Function'
   */
  out = false;
  if ((scale >= 1.0) && (scale <= 5.0) && (rtb_Subtract >= 10.0) &&
      (rtb_Subtract <= 170.0)) {
    sdq = 0;
    absxk = 1.0 - scale;
    t = 10.0 - rtb_Subtract;
    quad2 = 2;
    rtb_mode3 = 1;
    do {
      exitg1 = 0;
      if (rtb_mode3 <= 5) {
        xv1 = absxk;
        yv1 = t;
        absxk = b_0[rtb_mode3] - scale;
        t = (real_T)c_0[rtb_mode3] - rtb_Subtract;
        quad1 = quad2;
        if (absxk > 0.0) {
          if (t > 0.0) {
            quad2 = 0;
          } else {
            quad2 = 3;
          }
        } else if (t > 0.0) {
          quad2 = 1;
        } else {
          quad2 = 2;
        }

        QSMA_TAWS_contrib(xv1, yv1, absxk, t, quad1, quad2, scale_0[rtb_mode3 -
                          1], &b_dquad, &out);
        if (out) {
          out = true;
          exitg1 = 1;
        } else {
          sdq += b_dquad;
          rtb_mode3++;
        }
      } else {
        QSMA_TAWS_contrib(absxk, t, 1.0 - scale, 10.0 - rtb_Subtract, quad2, 2,
                          1.9984014443252818E-14, &quad1, &out);
        if (out) {
          out = true;
        } else {
          sdq += quad1;
          out = (sdq != 0);
        }

        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  /* Switch: '<S3>/Switch2' */
  if (rtb_Switch2 > 0) {
    /* Switch: '<S3>/Switch3' incorporates:
     *  Outport: '<Root>/current alert'
     */
    QSMA_TAWS_Y.currentalert = (int8_T)rtb_Switch2;
  } else if (out) {
    /* MATLAB Function: '<S7>/MATLAB Function' */
    rtb_Switch2 = 5;

    /* Switch: '<S3>/Switch3' incorporates:
     *  Outport: '<Root>/current alert'
     */
    QSMA_TAWS_Y.currentalert = (int8_T)rtb_Switch2;
  } else {
    /* Outport: '<Root>/current alert' incorporates:
     *  Switch: '<S3>/Switch3'
     */
    QSMA_TAWS_Y.currentalert = 0;
  }

  /* End of Switch: '<S3>/Switch2' */

  /* Update for Delay: '<S2>/Delay' incorporates:
   *  Inport: '<Root>/height_ground'
   */
  QSMA_TAWS_DW.Delay_DSTATE_l = QSMA_TAWS_U.height_ground;

  /* Update for Delay: '<S6>/Delay' incorporates:
   *  MATLAB Function: '<S4>/MATLAB Function'
   */
  QSMA_TAWS_DW.Delay_DSTATE = scale;
}

/* Model initialize function */
void QSMA_TAWS_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void QSMA_TAWS_terminate(void)
{
  /* (no terminate code required) */
}


/*
 * File trailer for generated code.
 *
 * [EOF]
 */
