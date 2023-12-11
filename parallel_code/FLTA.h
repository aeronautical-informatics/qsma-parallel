/*
 * ============================================================================
 *
 *       Filename:  FLTA.h
 *
 *    Description:
 *
 *        Created:  18/08/2022
 *       Compiler:  gcc
 *
 *         Author:  Mohamad Ibrahim
 *   Organization:  TU Clausthal
 *
 * ============================================================================
 */

#ifndef FLTA_H
#define FLTA_H


typedef struct {
	float Vnorth;
	float Veast;
	float Vdown;
}Vned;
typedef struct {
	float lat;
	float lon;
	float alt;
}Pos;
typedef struct {
	float X;
	float Y;
    float Z;
}point;

typedef struct {
	point center;
	point limit1;
	point limit2;
    float distance;
}Range;

//External inputs
typedef struct {
	point* elevationData;
	Pos position;
	float Vground;
	float VerticalSpeed;
	float RTC; //clearnce required in meters
	float TrueTrack;
	float YawRate;
	char* fileName;
}INPUT;

//External outputs
typedef struct {
	signed char alert;
}OUTPUT;

extern INPUT EXT_INPUT;
extern OUTPUT EXT_OUTPUT;

//entry point functions
extern void step(Range path[100], point envelope[4], Pos position,float Vground,float VerticalSpeed, float RTC,float TrueTrack);
extern void terminate(void);


int CalculateLAT(float Vground);
void StraightFlightPrediction(Range path[100], float Vground, float TrueTrack, Pos position, float Clearance);
void TurningFlightPrediction(Range path[100], float Vground, float TrueTrack, Pos position, float YawRate,float Clearance);
void LateralEnvelopGeneration(Vned speed, float TrueTrack, Pos position, float YawRate, int LookAheadTime, float Clearance);
void AltitudeLostDuringPullupManeuver(double GForce, Vned speed);
void AltitudeLostDueToPilotDelay(int PilotResponseTime1, Vned speed);
void WarningLookDownDistance(Vned speed);
void TheDesiredVerticalSpeedForSixDegreeClimb(Vned speed,int ClimbAngle1);
void AltitudeLostDuringDescendToLevelFlight(Vned speed);
void AltitudeLostDuringLevelToClimbFlight(Vned speed);
void AltitudeLostDuringClimbToAdjustTheClimbRate(Vned speed);
void AltitudeLostWhenDescendingWithCurrentVerticalSpeedForOneSecond(Vned speed);
void AltitudeGainedIfAircraftClimbedWithDesiredClimbGradientForOneSecond(Vned speed);
void AltitudeGainedIfAircraftClimbedWithVdesiredGradientForOneSecond(Vned speed);
double xAxisProjection(double input);
double yAxisProjection(double input);
double xMeter(double lat,double lon);
double yMeter(double lat);
double xMeterI(double lat,double lon);
double yMeterI(double lat);
void LevelEnvelopGeneration(point envelope[4], int RTC, float Vground,Pos position);
void DescendEnvelopGeneration(point envelope[4], int RTC, float Vground, Pos, float VerticalSpeed);
int intersection(Range path[], point envelope[], float Vground, char type);
int LevelOrDescent(float VerticalSpeed);

#endif /* FLTA_H_ */


