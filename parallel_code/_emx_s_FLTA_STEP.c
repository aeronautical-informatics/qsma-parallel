
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
int init() {
	void * call_arg;
	int call_arg2;
	int call_arg3;
	int call_arg4;
	void * call_arg5;
	ssize_t call_arg6;
	char * call_arg7;
	char * call_arg8;
	
	call_arg = malloc(12967202 * sizeof(point));
	
	FLTA_DATA.elevationData = (point *)call_arg;
	
	{
		int fd;
		
		call_arg2 = open(FLTA_DATA.fileName, 0);
		
		fd = call_arg2;
		
		if (fd == -1) {
			perror("Error opening file desc");
			
			return 1;
		}
		
		{
			FILE * file;
			
			call_arg3 = fdopen(fd, "r");
			
			file = call_arg3;
			
			if (file == (void *)0) {
				perror("Error converting file descriptor to FILE *");
				
				close(fd);
				
				return 1;
			}
			
			file = fdopen(fd, "r");
			
			if (file == (void *)0) {
				perror("Error converting file descriptor to FILE *");
				
				close(fd);
				
				return 1;
			}
			
			{
				struct stat st;
				int cond;
				
				call_arg4 = fstat(fd, &st);
				
				cond = (call_arg4 != 0);
				
				if (cond) {
					perror("Error getting file size");
					
					close(fd);
					
					return 1;
				}
				
				{
					char * buffer;
					
					call_arg5 = malloc(580417193);
					
					buffer = (char *)call_arg5;
					
					if (buffer == (void *)0) {
						perror("malloc failed");
						
						return 1;
					}
					
					{
						int total_bytes_read = 0;
						char chunk[8192];
						
						while (total_bytes_read < 580417193) {
							int n_read;
							
							call_arg6 = read(fd, chunk, 8192);
							
							n_read = call_arg6;
							
							if (n_read <= 0) {
								if (n_read == 0) {
									break;
								} else {
									perror("Error reading from file");
									
									free(buffer);
									
									return 1;
								}
							}
							
							if (total_bytes_read + n_read > 580417193) {
								n_read = 580417193 - total_bytes_read;
							}
							
							call_arg7 = buffer + total_bytes_read;
							
							memcpy(call_arg7, chunk, n_read);
							
							total_bytes_read = total_bytes_read + n_read;
						}
						
						printf("%c", buffer[580417189]);
						
						printf("%c", buffer[580417190]);
						
						printf("%c\n", buffer[580417191]);
						
						{
							char * line;
							
							call_arg8 = get_line_from_buffer(buffer, 12967201);
							
							line = call_arg8;
							
							if (line) {
								printf("Line %d: %s\n", 12967201, line);
								
								free(line);
							} else {
								printf("Failed to retrieve line %d from buffer.\n", 12967201);
							}
							
							if (FLTA_DATA.elevationData == (void *)0) {
								perror("lines malloc failed");
								
								return 1;
							}
							
							data_number = process_each_line(buffer, FLTA_DATA.elevationData);
							
							free(buffer);
							
							printf("Number of Elevation Data %d\n", data_number);
							
							printf("%f\n", FLTA_DATA.elevationData[1].Z);
							
						}
					}
				}
			}
		}
	}
}

int fdopen();
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
	
	call_arg6 = CalculateLAT_duplicate5(FLTA_DATA.Vground);
	
	call_arg4 = abs(call_arg5);
	
	call_arg2 = (int)call_arg4;
	
	call_arg = (int)call_arg3;
	
	getElevFile(FLTA_DATA.elevationData, path, call_arg6, data_number);
	
	sprintf(fileName, "%s%d%s%d%s", "N", call_arg, "W0", call_arg2, "_dem");
}

extern point * DescendEnvelopGeneration(int RTC, float Vground, Pos position, float VerticalSpeed);
extern point * LevelEnvelopGeneration_duplicate2(int RTC, float Vground, Pos position);
point * step3Outline(point * envelope) {
	if (FLTA_DATA.VerticalSpeed < -1) {
		printf("Descend\n");
		
		envelope = DescendEnvelopGeneration(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position, FLTA_DATA.VerticalSpeed);
		
		{
			char EnvelopePoints[1500] = "";
			char TempEnv[317] = "";
			
			sprintf(TempEnv, "%f %f%s%f %f%s%f %f%s%f %f%s", envelope[0].X, envelope[0].Y, ",", envelope[1].X, envelope[1].Y, ",", envelope[2].X, envelope[2].Y, ",", envelope[3].X, envelope[3].Y, ",");
			
			strcat(EnvelopePoints, TempEnv);
			
			strcat(EnvelopePoints, "\n\0");
		}
	} else {
		printf("Level\n");
		
		envelope = LevelEnvelopGeneration_duplicate2(FLTA_DATA.RTC, FLTA_DATA.Vground, FLTA_DATA.position);
		
		{
			char EnvelopePoints[1500] = "";
			char TempEnv[1909] = "";
			
			sprintf(TempEnv, "%f %f%s%f %f%s%f %f%s", envelope[0].X, envelope[0].Y, ",", envelope[1].X, envelope[1].Y, ",", envelope[2].X, envelope[2].Y, ",");
			
			strcat(EnvelopePoints, TempEnv);
			
			strcat(EnvelopePoints, "\n\0");
			
			printf("String: %s\n", EnvelopePoints);
		}
	}
	
	return envelope;
}

extern int intersection(Range path[100], point envelope[100], float Vground, char type);
void step4Outline(Range * path, point * envelope) {
	int call_arg;
	int call_arg2;
	
	if (FLTA_DATA.VerticalSpeed < -1) {
		call_arg = 'd';
		
		intersection(path, envelope, FLTA_DATA.Vground, call_arg);
	} else {
		call_arg2 = 'l';
		
		intersection(path, envelope, FLTA_DATA.Vground, call_arg2);
	}
}

extern Range * StraightFlightPrediction_p2(float Vground_p2, float TrueTrack_p2, Pos position_p2, float Clearance_p2);
extern Range * TurningFlightPrediction_duplicate2_p2(float Vground_p2, float TrueTrack_p2, Pos position_p2, float YawRate_p2, float Clearance_p2);
Range * step1Outline_p2(Range * path_p2) {
	_Bool sync30_p2;
	
	sync30_p2 = (FLTA_DATA.YawRate < 0.1 && FLTA_DATA.YawRate > -0.1);
	
	EMX_SendSync(2, 3, 31, -1, sync30_p2);
	
	if (sync30_p2) {
		path_p2 = StraightFlightPrediction_p2(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position, FLTA_DATA.RTC);
	} else {
		path_p2 = TurningFlightPrediction_duplicate2_p2(FLTA_DATA.Vground, FLTA_DATA.TrueTrack, FLTA_DATA.position, FLTA_DATA.YawRate, FLTA_DATA.RTC);
	}
	
	return path_p2;
}

extern void StraightFlightPrediction_p3(float TrueTrack_p3, float Clearance_p3);
extern void TurningFlightPrediction_duplicate2_p3(float Clearance_p3);
void step1Outline_p3(void) {
	if (EMX_RecvSync(2, 3, 31, -1)) {
		StraightFlightPrediction_p3(FLTA_DATA.TrueTrack, FLTA_DATA.RTC);
	} else {
		TurningFlightPrediction_duplicate2_p3(FLTA_DATA.RTC);
	}
}

void fltastep_p0() {
}

void fltastep_p1() {
}

extern double xMeter_duplicate2(double lat, double lon);
extern double yMeter_duplicate2(double lat);
extern int CalculateLAT_duplicate2(float Vground);
extern int intersection_duplicate2(Range path[100], point envelope[100], float Vground, char type);
extern int intersection_duplicate3(Range path[100], point envelope[100], float Vground, char type);
void fltastep_p2() {
	double call_arg_p2;
	double call_arg2_p2;
	float OriginalLat_p2;
	float OriginalLon_p2;
	int call_arg3_p2;
	int call_arg4_p2;
	int call_arg5_p2;
	
	call_arg_p2 = xMeter_duplicate2(FLTA_DATA.position.lat, FLTA_DATA.position.lon);
	
	FLTA_DATA.position.lon = call_arg_p2 * 1000.0;
	
	call_arg2_p2 = yMeter_duplicate2(FLTA_DATA.position.lat);
	
	FLTA_DATA.position.lat = call_arg2_p2 * 1000.0;
	
	OriginalLat_p2 = EMX_Recv32F(3, 2, 3, -1);
	
	OriginalLon_p2 = EMX_Recv32F(3, 2, 2, -1);
	
	EMX_Signal(2, 3, 0, -1);
	
	{
		Range * path_p2;
		
		path_p2 = step1Outline_p2(path_p2);
		
		EMX_Wait(3, 2, 1, -1);
		
		printf("VerticalSpeed %f\n", FLTA_DATA.VerticalSpeed);
		
		step2Outline(OriginalLat_p2, OriginalLon_p2, path_p2);
		
		call_arg3_p2 = CalculateLAT_duplicate2(FLTA_DATA.Vground);
		
		getElevFile(FLTA_DATA.elevationData, path_p2, call_arg3_p2, data_number);
		
		{
			point * envelope_p2;
			
			envelope_p2 = step3Outline(envelope_p2);
			
			if (FLTA_DATA.VerticalSpeed < -1) {
				call_arg4_p2 = 'd';
				
				intersection_duplicate2(path_p2, envelope_p2, FLTA_DATA.Vground, call_arg4_p2);
			} else {
				call_arg5_p2 = 'l';
				
				intersection_duplicate3(path_p2, envelope_p2, FLTA_DATA.Vground, call_arg5_p2);
			}
		}
	}
}

void fltastep_p3() {
	float OriginalLon_p3 = FLTA_DATA.position.lon;
	float OriginalLat_p3 = FLTA_DATA.position.lat;
	
	EMX_Send32F(3, 2, 3, -1, OriginalLat_p3);
	
	EMX_Send32F(3, 2, 2, -1, OriginalLon_p3);
	
	EMX_Wait(2, 3, 0, -1);
	
	step1Outline_p3();
	
	printf("Position %f %f\n", FLTA_DATA.position.lon, FLTA_DATA.position.lat);
	
	printf("Vground %f\n", FLTA_DATA.Vground);
	
	printf("TrueTrack %f\n", FLTA_DATA.TrueTrack);
	
	printf("YawRate %f\n", FLTA_DATA.YawRate);
	
	EMX_Signal(3, 2, 1, -1);
	
	fflush(stdout);
	
	fflush(stdout);
}




