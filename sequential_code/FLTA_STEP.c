#define _POSIX_C_SOURCE 200809

#include <math.h>
#include <stdio.h>
#include "Data.h"
#include <string.h>
#include "FLTA_STEP.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define READ 0
#define WRITE 1

extern point ElevationData[];

int data_number;
INPUT FLTA_DATA;

void step1Outline(Range path[100]);
void step2Outline(float OriginalLat, float OriginalLon, Range path[100]);
void step3Outline(point envelope[4]);

int init(){
	//Loading elevation files into memory
	FLTA_DATA.elevationData =  ElevationData;
	data_number = 12967201; // TODO should this be hardcoded?
}

int fltastep() {

    //////////////The main run////////////////////////////
	float OriginalLon=FLTA_DATA.position.lon;
	float OriginalLat=FLTA_DATA.position.lat;
	FLTA_DATA.position.lon = xMeter(FLTA_DATA.position.lat, FLTA_DATA.position.lon) * 1000.0;
	FLTA_DATA.position.lat = yMeter(FLTA_DATA.position.lat) * 1000.0;
	Range path[100];



	//debugging
	printf("Position %f %f\n", FLTA_DATA.position.lon, FLTA_DATA.position.lat);
	printf("Vground %f\n", FLTA_DATA.Vground);
	printf("TrueTrack %f\n", FLTA_DATA.TrueTrack);
	printf("YawRate %f\n", FLTA_DATA.YawRate);
	printf("VerticalSpeed %f\n", FLTA_DATA.VerticalSpeed);


	//Step 1 getting the lateral prediction points

	step1Outline(path);

//	if ((FLTA_DATA.YawRate < 0.1) && (FLTA_DATA.YawRate > -0.1)) {
//		path = StraightFlightPrediction(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position, FLTA_DATA.RTC);
//		printf("Straight\n");
//	}else{
//		path = TurningFlightPrediction(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position,FLTA_DATA.YawRate, FLTA_DATA.RTC);
//		printf("turning\n");
//	}

	//Step 2 Getting the elevation data for the predicted path

	step2Outline(OriginalLat, OriginalLon, path);

//	char fileName[100];
//	sprintf(fileName,"%s%d%s%d%s","N",(int)floor(OriginalLat),"W0",(int)abs(floor(OriginalLon)),"_dem");

	//TODO: Implement error detection for not finding the predicted path in the elevation data
	//getElevFile(FLTA_DATA.elevationData,path,CalculateLAT(FLTA_DATA.Vground),data_number);

	//fflush(stdout);

	//send the path points for plotting
	/*
	char PathPoints[15000] = "";
	//char *PathPoints = (char*) malloc(sizeof(char) * 1500);
	for (int i = 0; i < CalculateLAT(Vground) + 1; i++) {
		char PathCenter[50]= "";
		sprintf(PathCenter,"%f %f %f %f%s\0",path[i].center.X,path[i].center.Y,path[i].center.Z,path[i].distance,",");
		strcat(PathPoints,PathCenter);

		char PathLimit1[50]= "";
		sprintf(PathLimit1,"%f %f %f %f%s\0",path[i].limit1.X,path[i].limit1.Y,path[i].limit1.Z,path[i].distance,",");
		strcat(PathPoints,PathLimit1);

		char PathLimit2[50]= "";
		sprintf(PathLimit2,"%f %f %f %f%s\0",path[i].limit2.X,path[i].limit2.Y,path[i].limit2.Z,path[i].distance,",");
		strcat(PathPoints,PathLimit2);
	}
	strcat(PathPoints,"\n\0");

	int wr=send(sock, PathPoints, str_length(PathPoints), 0);
	if(wr<1)
	{
		printf("Reconnecting\n\n");
		sock=PlotServer();
	}
	*/

	//Step 3 getting the vertical envelope that supposed to be clear and sending it for plotting
	point envelope[4];
	step3Outline(envelope);


	//Step 4 intersect the path with the envelope
	if (FLTA_DATA.VerticalSpeed<-1)
		intersection(path, envelope, FLTA_DATA.Vground, 'd');
	else
		intersection(path, envelope, FLTA_DATA.Vground, 'l');
}

void step1Outline(Range path[100]) {
	if ((FLTA_DATA.YawRate < 0.1) && (FLTA_DATA.YawRate > -0.1)) {
		StraightFlightPrediction(path, FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position, FLTA_DATA.RTC);
	}else{
		TurningFlightPrediction(path, FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position,FLTA_DATA.YawRate, FLTA_DATA.RTC);
	}
}

void step2Outline(float OriginalLat, float OriginalLon, Range path[100]) {
	char fileName[100];
	sprintf(fileName,"%s%d%s%d%s","N",(int)floor(OriginalLat),"W0",(int)abs(floor(OriginalLon)),"_dem");

	getElevFile(FLTA_DATA.elevationData,path,CalculateLAT(FLTA_DATA.Vground),data_number);
	fflush(stdout);
}

void step3Outline(point envelope[4]) {
	if (FLTA_DATA.VerticalSpeed<-1) {
			printf("Descend\n");
			DescendEnvelopGeneration(envelope, FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position, FLTA_DATA.VerticalSpeed);

			//////////The C Version
			char EnvelopePoints[1500]="";
			//sending the envelope sign or mark
			//send(sock, "\n", 1, 0);
			//send(sock, "env\n", 4, 0);

			char TempEnv[317]="";
			sprintf(TempEnv,"%f %f%s%f %f%s%f %f%s%f %f%s",
					envelope[0].X,envelope[0].Y,",",
					envelope[1].X,envelope[1].Y,",",
					envelope[2].X,envelope[2].Y,",",
					envelope[3].X,envelope[3].Y,",");
			strcat(EnvelopePoints,TempEnv);
			strcat(EnvelopePoints,"\n\0");

			//send(sock, EnvelopePoints, str_length(EnvelopePoints), 0);
			///////////////////////

		}else
		{
			printf("Level\n");
			LevelEnvelopGeneration(envelope, FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position);

			//////////The C Version
			char EnvelopePoints[1500]="";
			//sending the envelope sign or mark
			//send(sock, "\n", 1, 0);
			//send(sock, "env\n", 4, 0);

			char TempEnv[1909]="";
			sprintf(TempEnv,"%f %f%s%f %f%s%f %f%s",
					envelope[0].X,envelope[0].Y,",",
					envelope[1].X,envelope[1].Y,",",
					envelope[2].X,envelope[2].Y,",");
			strcat(EnvelopePoints,TempEnv);
			strcat(EnvelopePoints,"\n\0");

			//send(sock, EnvelopePoints, str_length(EnvelopePoints), 0);
			printf("String: %s\n", EnvelopePoints);
			///////////////////////

		}
}
