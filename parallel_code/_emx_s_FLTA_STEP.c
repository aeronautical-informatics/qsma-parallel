
#include <math.h>
#include <stdio.h>
#include "Data.h"
#include <string.h>
#include "FLTA_STEP.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "_emx_main.h"
int data_number;
INPUT FLTA_DATA;
extern int CalculateLAT_duplicate5(float Vground);
void step2Outline(float OriginalLat, float OriginalLon, Range * path) {
	char fileName[100];
	int call_arg;
	int call_arg2;
	double call_arg3;
	int call_arg4;
	double call_arg5;
	int call_arg6;
	
	call_arg5 = floor(OriginalLon);
	
	call_arg3 = floor(OriginalLat);
	
	call_arg6 = CalculateLAT_duplicate2(FLTA_DATA.Vground);
	
	call_arg4 = abs(call_arg5);
	
	call_arg2 = (int)call_arg4;
	
	call_arg = (int)call_arg3;
	
	getElevFile(FLTA_DATA.elevationData, path, call_arg6, data_number);
	
	sprintf(fileName, "%s%d%s%d%s", "N", call_arg, "W0", call_arg2, "_dem");
}

extern Range * StraightFlightPrediction_p2(float Vground_p2, float TrueTrack_p2, Pos position_p2, float Clearance_p2);
extern Range * TurningFlightPrediction_p2(float Vground_p2, float TrueTrack_p2, Pos position_p2, float YawRate_p2, float Clearance_p2);
void step1Outline_p2(Range * path_p2) {
	_Bool sync37_p2;
	
	sync37_p2 = (FLTA_DATA.YawRate < 0.1 && FLTA_DATA.YawRate > -0.1);
	
	EMX_SendSync(2, 3, 38, -1, sync37_p2);
	
	if (sync37_p2) {
		path_p2 = StraightFlightPrediction_p2(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position, FLTA_DATA.RTC);
	} else {
		path_p2 = TurningFlightPrediction_p2(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position, FLTA_DATA.YawRate, FLTA_DATA.RTC);
	}
}

extern void StraightFlightPrediction_p3(float TrueTrack_p3, float Clearance_p3);
extern void TurningFlightPrediction_p3(float Clearance_p3);
void step1Outline_p3(void) {
	if (EMX_RecvSync(2, 3, 38, -1)) {
		StraightFlightPrediction_p3(FLTA_DATA.TrueTrack, FLTA_DATA.RTC);
	} else {
		TurningFlightPrediction_p3(FLTA_DATA.RTC);
	}
}

void fltastep_p0() {
}

void fltastep_p1() {
}

extern double xMeter(double lat, double lon);
extern double yMeter(double lat);
extern int CalculateLAT_duplicate2(float Vground);
extern point * DescendEnvelopGeneration(int RTC, float Vground, Pos position, float VerticalSpeed);
extern point * LevelEnvelopGeneration(int RTC, float Vground, Pos position);
extern int intersection_duplicate2(Range path[100], point envelope[100], float Vground, char type);
extern void intersection_p2(void);
void fltastep_p2() {
	_Bool sync39_p2;
	double call_arg_p2;
	double call_arg2_p2;
	float OriginalLat_p2;
	float OriginalLon_p2;
	int call_arg3_p2;
	int call_arg4_p2;
	int call_arg5_p2;
	
	call_arg_p2 = xMeter(FLTA_DATA.position.lat, FLTA_DATA.position.lon);
	
	FLTA_DATA.position.lon = call_arg_p2 * 1000.0;
	
	call_arg2_p2 = yMeter(FLTA_DATA.position.lat);
	
	FLTA_DATA.position.lat = call_arg2_p2 * 1000.0;
	
	OriginalLat_p2 = EMX_Recv32F(3, 2, 6, -1);
	
	OriginalLon_p2 = EMX_Recv32F(3, 2, 5, -1);
	
	EMX_Send(2, 3, 0, -1, &FLTA_DATA, sizeof(FLTA_DATA));
	
	{
		Range path_p2[100];
		
		step1Outline_p2(path_p2);
		
		EMX_Wait(3, 2, 3, -1);
		
		printf("VerticalSpeed %f\n", FLTA_DATA.VerticalSpeed);
		
		step2Outline(OriginalLat_p2, OriginalLon_p2, path_p2);
		
		call_arg3_p2 = CalculateLAT_duplicate2(FLTA_DATA.Vground);
		
		getElevFile(FLTA_DATA.elevationData, path_p2, call_arg3_p2, data_number);
		
		EMX_Send(2, 3, 7, -1, path_p2, 100 * sizeof(*path_p2));
		
		EMX_Send(2, 3, 1, -1, &FLTA_DATA, sizeof(FLTA_DATA));
		
		{
			point envelope_p2[4];
			
			if (FLTA_DATA.VerticalSpeed < -1) {
				envelope_p2 = DescendEnvelopGeneration(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position, FLTA_DATA.VerticalSpeed);
				
				{
					char EnvelopePoints_p2[1500] = "";
					char TempEnv_p2[317] = "";
					
					sprintf(TempEnv_p2, "%f %f%s%f %f%s%f %f%s%f %f%s", envelope_p2[0].X, envelope_p2[0].Y, ",", envelope_p2[1].X, envelope_p2[1].Y, ",", envelope_p2[2].X, envelope_p2[2].Y, ",", envelope_p2[3].X, envelope_p2[3].Y, ",");
					
					strcat(EnvelopePoints_p2, TempEnv_p2);
					
					strcat(EnvelopePoints_p2, "\n\0");
				}
				printf("Descend\n");
			} else {
				printf("Level\n");
				
				envelope_p2 = LevelEnvelopGeneration(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position);
				
				{
					char EnvelopePoints_p2[1500] = "";
					char TempEnv_p2[1909] = "";
					
					sprintf(TempEnv_p2, "%f %f%s%f %f%s%f %f%s", envelope_p2[0].X, envelope_p2[0].Y, ",", envelope_p2[1].X, envelope_p2[1].Y, ",", envelope_p2[2].X, envelope_p2[2].Y, ",");
					
					strcat(EnvelopePoints_p2, TempEnv_p2);
					
					strcat(EnvelopePoints_p2, "\n\0");
					
					printf("String: %s\n", EnvelopePoints_p2);
				}
			}
			
			EMX_Send(2, 3, 34, -1, envelope_p2, 4 * sizeof(*envelope_p2));
			
			EMX_Signal(2, 3, 2, -1);
			
			sync39_p2 = (FLTA_DATA.VerticalSpeed < -1);
			
			EMX_SendSync(2, 3, 40, -1, sync39_p2);
			
			if (sync39_p2) {
				call_arg4_p2 = 'd';
				
				intersection_duplicate2(path_p2, envelope_p2, FLTA_DATA.Vground, call_arg4_p2);
			} else {
				call_arg5_p2 = EMX_Recv32(3, 2, 35, -1);
				
				intersection_p2();
				
				EMX_Wait(3, 2, 4, -1);
			}
		}
		fflush(stdout);
		
		fflush(stdout);
	}
}

extern int intersection_p3(Range path_p3[100], point envelope_p3[100], float Vground_p3, char type_p3);
void fltastep_p3() {
	int call_arg5_p3;
	float OriginalLon_p3 = FLTA_DATA.position.lon;
	float OriginalLat_p3 = FLTA_DATA.position.lat;
	
	EMX_Send32F(3, 2, 6, -1, OriginalLat_p3);
	
	EMX_Send32F(3, 2, 5, -1, OriginalLon_p3);
	
	EMX_Recv(2, 3, 0, -1, &FLTA_DATA, sizeof(FLTA_DATA));
	
	{
		Range path_p3[100];
		
		step1Outline_p3();
		
		printf("Position %f %f\n", FLTA_DATA.position.lon, FLTA_DATA.position.lat);
		
		printf("Vground %f\n", FLTA_DATA.Vground);
		
		printf("TrueTrack %f\n", FLTA_DATA.TrueTrack);
		
		printf("YawRate %f\n", FLTA_DATA.YawRate);
		
		EMX_Signal(3, 2, 3, -1);
		
		EMX_Recv(2, 3, 7, -1, path_p3, 100 * sizeof(*path_p3));
		
		EMX_Recv(2, 3, 1, -1, &FLTA_DATA, sizeof(FLTA_DATA));
		
		{
			point envelope_p3[4];
			
			EMX_Recv(2, 3, 34, -1, envelope_p3, 4 * sizeof(*envelope_p3));
			
			EMX_Wait(2, 3, 2, -1);
			
			if (EMX_RecvSync(2, 3, 40, -1)) {
			} else {
				call_arg5_p3 = 'l';
				
				EMX_Send32(3, 2, 35, -1, call_arg5_p3);
				
				intersection_p3(path_p3, envelope_p3, FLTA_DATA.Vground, call_arg5_p3);
				
				EMX_Signal(3, 2, 4, -1);
			}
		}
	}
}




