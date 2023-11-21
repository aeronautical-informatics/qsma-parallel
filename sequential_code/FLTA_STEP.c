#define _POSIX_C_SOURCE 200809

#include <math.h>
#include <stdio.h>
#include "Data.h"
#include <string.h>
#include "FLTA_STEP.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


#define READ 0
#define WRITE 1


int data_number;
INPUT FLTA_DATA;

void step1Outline(Range * path);
void step2Outline(float OriginalLat, float OriginalLon, Range * path);
void step3Outline(point *envelope);
void step4Outline(Range * path, point * envelope);

int init(){
	//Loading elevation files into memory
	FLTA_DATA.elevationData = (point*) malloc(12967202 * sizeof(point));
	int fd = open(FLTA_DATA.fileName, O_RDONLY);
	if (fd == -1)
	{
			perror("Error opening file desc");
			return 1;
	}

	FILE *file = fdopen(fd, "r");
	if (file == NULL) {
		perror("Error converting file descriptor to FILE *");
		close(fd);
		return 1;
	}

	// Convert the file descriptor to a FILE * stream
	file = fdopen(fd, "r");
	if (file == NULL) {
	   perror("Error converting file descriptor to FILE *");
	   close(fd);
	   return 1;
	}

	struct stat st;
	if (fstat(fd, &st) != 0) {
		perror("Error getting file size");
		close(fd);
		return 1;
	}
	//printf("filesize %lld\n",st.st_size);

	char *buffer = (char*)malloc(580417193);
	if (buffer == NULL) {
		perror("malloc failed");
		return 1;
	}
	int total_bytes_read = 0;
	char chunk[8192];
	while (total_bytes_read < 580417193) {
		int n_read = read(fd, chunk, 8192);

		// Check for end of file or error
		if (n_read <= 0) {
			if (n_read == 0) {
				// EOF reached
				break;
			} else {
				perror("Error reading from file");
				free(buffer);
				return 1;
			}
		}

		// Copy the chunk to the main buffer
		if (total_bytes_read + n_read > 580417193) {
			// This ensures you don't overflow your buffer if for some reason
			// the file has more data than you expect.
			n_read = 580417193 - total_bytes_read;
		}
		memcpy(buffer + total_bytes_read, chunk, n_read);

		total_bytes_read += n_read;
	}

	printf("%c", buffer[580417189]);
	printf("%c", buffer[580417190]);
	printf("%c\n", buffer[580417191]);



	char* line = get_line_from_buffer(buffer, 12967201);
	if (line) {
		printf("Line %d: %s\n", 12967201, line);
		free(line);  // Remember to free the dynamically allocated memory for the line
	} else {
		printf("Failed to retrieve line %d from buffer.\n", 12967201);
	}


	//populate the elevation data into structured memory
	if (FLTA_DATA.elevationData == NULL) {
		perror("lines malloc failed");
		return 1;
	}
	data_number=process_each_line(buffer,FLTA_DATA.elevationData);
	free(buffer);
	printf("Number of Elevation Data %d\n",data_number);
	printf("%f\n",FLTA_DATA.elevationData[1].Z);

	 /////////////Plotting Implementation////////////////////
	    //int sock=PlotServer();
	    int client_fd,valread;
}

int fltastep() {

    //////////////The main run////////////////////////////
	float OriginalLon=FLTA_DATA.position.lon;
	float OriginalLat=FLTA_DATA.position.lat;
	FLTA_DATA.position.lon = xMeter(FLTA_DATA.position.lat, FLTA_DATA.position.lon) * 1000.0;
	FLTA_DATA.position.lat = yMeter(FLTA_DATA.position.lat) * 1000.0;
	Range path[100];

	//FG live input
	/*
	Pos position;
	position.alt = getAltft()/3.281; //from feet to meter
	position.lon = getlong(); //X
	position.lat = getlat(); //Y
	float OriginalLon=position.lon;
	float OriginalLat=position.lat;
	float Vground = getGroundSpeed();
	//float Vground = 100;
	float VerticalSpeed = getVerticalSpeed()/3.281; //from feet-per-second to meter per second
	float RTC = 500 * 0.3048;
	float TrueTrack = getTrueTrack();
	position.lon = xMeter(position.lat, position.lon) * 1000.0;
	position.lat = yMeter(position.lat) * 1000.0;
	Range *path;
	float YawRate=getYawRate();
	*/

	//Reconnect with the plotting server
//        if (sock==-1)
//        {
//            printf("Reconnecting\n\n");
//            sock=PlotServer();
//        }

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
	fflush(stdout);

//	char fileName[100];
//	sprintf(fileName,"%s%d%s%d%s","N",(int)floor(OriginalLat),"W0",(int)abs(floor(OriginalLon)),"_dem");

	//TODO: Implement error detection for not finding the predicted path in the elevation data
//	getElevFile(FLTA_DATA.elevationData,path,CalculateLAT(FLTA_DATA.Vground),data_number);
//
//	fflush(stdout);

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

//	point *envelope;
//	if (FLTA_DATA.VerticalSpeed<-1) {
//		printf("Descend\n");
//		envelope = DescendEnvelopGeneration(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position, FLTA_DATA.VerticalSpeed);
//
//		//////////The C Version
//		char EnvelopePoints[1500]="";
//		//sending the envelope sign or mark
//		//send(sock, "\n", 1, 0);
//		//send(sock, "env\n", 4, 0);
//
//		char TempEnv[50]="";
//		sprintf(TempEnv,"%f %f%s%f %f%s%f %f%s%f %f%s",
//				envelope[0].X,envelope[0].Y,",",
//				envelope[1].X,envelope[1].Y,",",
//				envelope[2].X,envelope[2].Y,",",
//				envelope[3].X,envelope[3].Y,",");
//		strcat(EnvelopePoints,TempEnv);
//		strcat(EnvelopePoints,"\n\0");
//
//		//send(sock, EnvelopePoints, str_length(EnvelopePoints), 0);
//		///////////////////////
//
//	}else
//	{
//		printf("Level\n");
//		envelope = LevelEnvelopGeneration(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position);
//
//		//////////The C Version
//		char EnvelopePoints[1500]="";
//		//sending the envelope sign or mark
//		//send(sock, "\n", 1, 0);
//		//send(sock, "env\n", 4, 0);
//
//		char TempEnv[50]="";
//		sprintf(TempEnv,"%f %f%s%f %f%s%f %f%s",
//				envelope[0].X,envelope[0].Y,",",
//				envelope[1].X,envelope[1].Y,",",
//				envelope[2].X,envelope[2].Y,",");
//		strcat(EnvelopePoints,TempEnv);
//		strcat(EnvelopePoints,"\n\0");
//
//		//send(sock, EnvelopePoints, str_length(EnvelopePoints), 0);
//		printf("String: %s\n", EnvelopePoints);
//		///////////////////////
//
//	}

	//Step 4 intersect the path with the envelope

	step4Outline(path, envelope);
//	if (FLTA_DATA.VerticalSpeed<-1)
//		intersection(path, envelope, FLTA_DATA.Vground, 'd');
//	else
//		intersection(path, envelope, FLTA_DATA.Vground, 'l');
}

void step1Outline(Range * path) {
	if ((FLTA_DATA.YawRate < 0.1) && (FLTA_DATA.YawRate > -0.1)) {
		path = StraightFlightPrediction(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position, FLTA_DATA.RTC);
	}else{
		path = TurningFlightPrediction(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position,FLTA_DATA.YawRate, FLTA_DATA.RTC);
	}
}

void step2Outline(float OriginalLat, float OriginalLon, Range * path) {
	char fileName[100];
	sprintf(fileName,"%s%d%s%d%s","N",(int)floor(OriginalLat),"W0",(int)abs(floor(OriginalLon)),"_dem");

	getElevFile(FLTA_DATA.elevationData,path,CalculateLAT(FLTA_DATA.Vground),data_number);
}

void step3Outline(point *envelope) {
	if (FLTA_DATA.VerticalSpeed<-1) {
		printf("Descend\n");
		envelope = DescendEnvelopGeneration(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position, FLTA_DATA.VerticalSpeed);

		//////////The C Version
		char EnvelopePoints[1500]="";
		//sending the envelope sign or mark
		//send(sock, "\n", 1, 0);
		//send(sock, "env\n", 4, 0);

		char TempEnv[50]="";
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
		envelope = LevelEnvelopGeneration(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position);

		//////////The C Version
		char EnvelopePoints[1500]="";
		//sending the envelope sign or mark
		//send(sock, "\n", 1, 0);
		//send(sock, "env\n", 4, 0);

		char TempEnv[50]="";
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

void step4Outline(Range * path, point * envelope) {
	if (FLTA_DATA.VerticalSpeed<-1)
			intersection(path, envelope, FLTA_DATA.Vground, 'd');
		else
			intersection(path, envelope, FLTA_DATA.Vground, 'l');
}

