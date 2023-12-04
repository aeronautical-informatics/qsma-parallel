
#include "FLTA.h"
#include <math.h>
#include <stdio.h>
#include "_emx_main.h"
static void ClimbingEnvelopGeneration();
static void EnvelopCalc();
static void Intersection();
static void Step();
int CalculateLAT(float Vground) {
	if (Vground <= 100) {
		return 20;
	} else if (Vground <= 150) {
		return 20;
	} else if (Vground <= 200) {
		return 25;
	} else if (Vground <= 250) {
		return 27;
	} else if (Vground <= 300) {
		return 31;
	} else if (Vground <= 350) {
		return 35;
	} else if (Vground <= 450) {
		return 32;
	} else if (Vground <= 550) {
		return 27;
	} else if (Vground <= 600) {
		return 25;
	} else {
		return 15;
	}
	
	
	
	
	
	
	
	
}

float VectorAngleCosine(float u1, float u2, float v1, float v2) {
	float u;
	float v;
	float dot;
	float call_arg;
	float call_arg2;
	float call_arg3;
	float call_arg4;
	float call_arg5;
	float call_arg6;
	float call_arg7;
	float call_arg8;
	
	call_arg3 = powf(u1, 2.0f);
	
	call_arg4 = powf(u2, 2.0f);
	
	call_arg2 = call_arg3 + call_arg4;
	
	call_arg = sqrtf(call_arg2);
	
	u = call_arg;
	
	call_arg7 = powf(v1, 2.0f);
	
	call_arg8 = powf(v2, 2.0f);
	
	call_arg6 = call_arg7 + call_arg8;
	
	call_arg5 = sqrtf(call_arg6);
	
	v = call_arg5;
	
	dot = u1 * v1 + u2 * v2;
	
	return dot / (u * v);
}

Range * TurningFlightPrediction(float Vground, float TrueTrack, Pos position, float YawRate, float Clearance) {
	double rad = 3.14159265 / 180.0;
	double PredY = 0;
	double PredX = 0;
	double a = TrueTrack * rad;
	int LookAheadTime;
	int call_arg;
	double call_arg2;
	double call_arg3;
	double call_arg4;
	double call_arg5;
	double call_arg6;
	double call_arg7;
	double call_arg8;
	double call_arg9;
	double call_arg10;
	double call_arg11;
	double call_arg12;
	double call_arg13;
	double call_arg14;
	double call_arg15;
	double call_arg16;
	double call_arg17;
	double call_arg18;
	double call_arg19;
	double call_arg20;
	double call_arg21;
	double call_arg22;
	double call_arg23;
	double call_arg24;
	double call_arg25;
	double call_arg26;
	double call_arg27;
	double call_arg28;
	double call_arg29;
	double call_arg30;
	double call_arg31;
	double call_arg32;
	double call_arg33;
	double call_arg34;
	double call_arg35;
	
	call_arg = CalculateLAT(Vground);
	
	LookAheadTime = call_arg;
	
	Vground = Vground * 0.514444;
	
	{
		static Range path[100];
		point container;
		
		container.X = PredX + position.lon;
		container.Y = PredY + position.lat;
		path[0].center = container;
		
		{
			double Radius = Vground * 360 / YawRate / 3.14159265 / 2;
			double theta = 3.14159265 - a;
			double Xc;
			double Yc;
			_Bool cond;
			_Bool cond3;
			_Bool cond22;
			
			call_arg2 = cos(theta);
			
			Xc = PredX - Radius * call_arg2;
			
			call_arg3 = tan(theta);
			
			Yc = call_arg3 * (Xc - PredX) + PredY;
			
			cond3 = 1;
			
			call_arg4 = fabs(TrueTrack);
			
			cond22 = !(call_arg4 == 90);
			
			if (cond22) {
				call_arg5 = fabs(TrueTrack);
				
				cond3 = (call_arg5 == 270);
			}
			
			cond = cond3;
			
			if (cond) {
				double ClearanceY = PredY + Clearance;
				
				container.X = PredX + position.lon;
				container.Y = ClearanceY + position.lat;
				path[0].limit1 = container;
				ClearanceY = PredY - Clearance;
				container.X = PredX + position.lon;
				container.Y = ClearanceY + position.lat;
				path[0].limit2 = container;
			} else {
				double A1;
				double A2;
				double A3;
				double sol1;
				double sol2;
				double ClearanceY;
				
				call_arg7 = tan(theta);
				
				call_arg6 = pow(call_arg7, 2.0);
				
				A1 = call_arg6 + 1;
				
				call_arg8 = tan(theta);
				
				call_arg9 = tan(theta);
				
				A2 = -2 * PredX - 2 * call_arg8 * call_arg9 * PredX;
				
				call_arg12 = tan(theta);
				
				call_arg11 = call_arg12 * PredX;
				
				call_arg10 = pow(call_arg11, 2.0);
				
				A3 = PredX * PredX + call_arg10 - Clearance * Clearance;
				
				call_arg14 = A2 * A2 - 4 * A1 * A3;
				
				call_arg13 = sqrt(call_arg14);
				
				sol1 = (-A2 + call_arg13) / (2 * A1);
				
				call_arg16 = A2 * A2 - 4 * A1 * A3;
				
				call_arg15 = sqrt(call_arg16);
				
				sol2 = (-A2 - call_arg15) / (2 * A1);
				
				call_arg17 = tan(theta);
				
				ClearanceY = call_arg17 * (sol1 - PredX) + PredY;
				
				container.X = sol1 + position.lon;
				container.Y = ClearanceY + position.lat;
				path[0].limit1 = container;
				
				call_arg18 = tan(theta);
				
				ClearanceY = call_arg18 * (sol2 - PredX) + PredY;
				
				container.X = sol2 + position.lon;
				container.Y = ClearanceY + position.lat;
				path[0].limit2 = container;
			}
			
			{
				double DeltaArch = Vground * 1;
				double DeltaTheta = DeltaArch / Radius;
				double chord;
				double Slope;
				double DeltaSlope;
				
				call_arg20 = 0.5 * DeltaTheta;
				
				call_arg19 = sin(call_arg20);
				
				chord = 2 * Radius * call_arg19;
				
				Slope = theta;
				
				DeltaSlope = 3.14159265 / 2.0 - a - DeltaTheta / 2.0;
				
				for (int i = 1; i <= LookAheadTime; i = i + 1) {
					double DeltaX;
					double DeltaY;
					
					call_arg21 = cos(DeltaSlope);
					
					DeltaX = PredX + chord * call_arg21;
					
					call_arg22 = tan(DeltaSlope);
					
					DeltaY = call_arg22 * (DeltaX - PredX) + PredY;
					
					PredX = DeltaX;
					PredY = DeltaY;
					DeltaSlope = DeltaSlope - DeltaTheta;
					Slope = Slope - DeltaTheta;
					
					{
						double A1;
						double A2;
						double A3;
						double sol1;
						double sol2;
						double ClearanceX;
						double sol1Y;
						
						call_arg24 = tan(Slope);
						
						call_arg23 = pow(call_arg24, 2.0);
						
						A1 = call_arg23 + 1;
						
						call_arg25 = tan(Slope);
						
						call_arg26 = tan(Slope);
						
						A2 = -2 * DeltaX - 2 * call_arg25 * call_arg26 * DeltaX;
						
						call_arg29 = tan(Slope);
						
						call_arg28 = call_arg29 * DeltaX;
						
						call_arg27 = pow(call_arg28, 2.0);
						
						A3 = DeltaX * DeltaX + call_arg27 - Clearance * Clearance;
						
						call_arg31 = A2 * A2 - 4 * A1 * A3;
						
						call_arg30 = sqrt(call_arg31);
						
						sol1 = (-A2 + call_arg30) / (2 * A1);
						
						call_arg33 = A2 * A2 - 4 * A1 * A3;
						
						call_arg32 = sqrt(call_arg33);
						
						sol2 = (-A2 - call_arg32) / (2 * A1);
						
						ClearanceX = sol1;
						
						call_arg34 = tan(Slope);
						
						sol1Y = call_arg34 * (ClearanceX - DeltaX) + DeltaY;
						
						ClearanceX = sol2;
						
						{
							double sol2Y;
							
							call_arg35 = tan(Slope);
							
							sol2Y = call_arg35 * (ClearanceX - DeltaX) + DeltaY;
							
							container.X = DeltaX + position.lon;
							container.Y = DeltaY + position.lat;
							path[i].center = container;
							container.X = sol1 + position.lon;
							container.Y = sol1Y + position.lat;
							path[i].limit1 = container;
							container.X = sol2 + position.lon;
							container.Y = sol2Y + position.lat;
							path[i].limit2 = container;
							path[i].distance = DeltaArch * i;
						}
					}
				}
				
				return path;
			}
		}
	}
}

void LateralEnvelopGeneration(Vned speed, float TrueTrack, Pos position, float YawRate, int LookAheadTime, float Clearance) {
}

point * LevelEnvelopGeneration(int RTC, float Vground, Pos position) {
	int call_arg;
	float call_arg2;
	
	Vground = Vground * 0.514444;
	
	{
		int LookAheadTime;
		point A;
		point B;
		point C;
		
		call_arg = CalculateLAT(Vground);
		
		LookAheadTime = call_arg;
		
		A.X = 0;
		A.Y = position.alt - RTC;
		
		call_arg2 = Vground * LookAheadTime;
		
		printf("Debug %f %d %f\n", Vground, LookAheadTime, call_arg2);
		
		B.X = Vground * LookAheadTime;
		B.Y = A.Y;
		C.X = B.X;
		C.Y = position.alt;
		
		{
			static point envelope[4];
			
			envelope[0] = A;
			envelope[1] = B;
			envelope[2] = C;
			return envelope;
		}
	}
}

point * DescendEnvelopGeneration(int RTC, float Vground, Pos position, float VerticalSpeed) {
	int call_arg;
	
	Vground = Vground * 0.514444;
	
	{
		int LookAheadTime;
		float AltitudeLoss;
		int timeToLevel;
		point A;
		point B;
		point C;
		point D;
		
		call_arg = CalculateLAT(Vground);
		
		LookAheadTime = call_arg;
		
		A.X = 0;
		A.Y = position.alt - RTC;
		AltitudeLoss = VerticalSpeed * 3 * -1;
		timeToLevel = 3;
		AltitudeLoss = AltitudeLoss + 0.5 * VerticalSpeed * VerticalSpeed / (1 * 32.1741);
		timeToLevel = timeToLevel + 0.5 * VerticalSpeed / (1 * 32.1741);
		B.X = timeToLevel * Vground;
		B.Y = position.alt - RTC - AltitudeLoss;
		C.X = Vground * LookAheadTime;
		C.Y = B.Y;
		D.X = C.X;
		D.Y = position.alt;
		
		{
			static point envelope[4];
			
			envelope[0] = A;
			envelope[1] = B;
			envelope[2] = C;
			envelope[3] = D;
			return envelope;
		}
	}
}

int intersection(Range path[100], point envelope[100], float Vground, char type) {
	int call_arg;
	int call_arg2;
	
	if (type == 'l') {
		int i;
		int cond;
		
		i = 0;
		
		call_arg = CalculateLAT(Vground);
		
		cond = (i < call_arg + 1);
		
		for (; cond; ) {
			if ((path[i].center.Z > envelope[1].Y || path[i].limit1.Z > envelope[1].Y) || path[i].limit2.Z > envelope[1].Y) {
				printf("WARNING!!! %d %f %f", i, envelope[1].Y, path[i].center.Z);
				
				return 1;
			}
			
			i = i + 1;
			
			call_arg = CalculateLAT(Vground);
			
			cond = (i < call_arg + 1);
		}
	}
	
	if (type == 'd') {
		float slope = (envelope[0].Y - envelope[1].Y) / (envelope[0].X - envelope[1].X);
		int i;
		int cond;
		
		i = 0;
		
		call_arg2 = CalculateLAT(Vground);
		
		cond = (i < call_arg2 + 1);
		
		for (; cond; ) {
			if (envelope[0].X + path[i].distance < envelope[1].X) {
				float intersectionY = slope * (envelope[0].X + path[i].distance - envelope[1].X) + envelope[1].Y;
				
				if ((path[i].center.Z > intersectionY || path[i].limit1.Z > intersectionY) || path[i].limit2.Z > intersectionY) {
					printf("WARNING!!! %d %f %f\n", i, path[i].distance, intersectionY);
					
					return 1;
				}
			} else if ((path[i].center.Z > envelope[1].Y || path[i].limit1.Z > envelope[1].Y) || path[i].limit2.Z > envelope[1].Y) {
				printf("WARNING!!! %d %f\n", i, path[i].distance);
				
				return 1;
			}
			
			
			i = i + 1;
			
			call_arg2 = CalculateLAT(Vground);
			
			cond = (i < call_arg2 + 1);
		}
	}
	
	return 0;
}

double xMeter(double lat, double lon) {
	double rad = 3.14159265 / 180.0;
	double call_arg;
	double call_arg2;
	
	call_arg2 = lat * rad;
	
	call_arg = cos(call_arg2);
	
	return lon * 40075 * (call_arg / 360);
}

double yMeter(double lat) {
	return lat * 111.32;
}

double xMeterI(double lat, double lon) {
	double rad = 3.14159265 / 180.0;
	double call_arg;
	double call_arg2;
	
	call_arg2 = lat * rad;
	
	call_arg = cos(call_arg2);
	
	return lon * 360 / (40075 * call_arg);
}

double yMeterI(double lat) {
	return lat / 111.32;
}

int LevelOrDescent(float VerticalSpeed) {
	float call_arg;
	
	call_arg = VerticalSpeed * 60;
	
	printf("%f\n", call_arg);
	
	if (VerticalSpeed * 60 > -500 && VerticalSpeed * 60 < 500) {
		return 0;
	}
	
	return 1;
}

void step(Pos position, float Vground, float VerticalSpeed, float RTC, float TrueTrack) {
	double call_arg;
	double call_arg2;
	double call_arg3;
	double call_arg4;
	double call_arg5;
	double call_arg6;
	double call_arg7;
	double call_arg8;
	Range * call_arg9;
	double call_arg10;
	int call_arg11;
	point * call_arg12;
	int call_arg13;
	int call_arg14;
	
	call_arg = xMeter(position.lat, position.lon);
	
	position.lon = call_arg * 1000.0;
	
	call_arg2 = yMeter(position.lat);
	
	position.lat = call_arg2 * 1000.0;
	
	call_arg6 = position.lat / 1000.0;
	
	call_arg4 = yMeterI(call_arg6);
	
	call_arg5 = position.lon / 1000.0;
	
	call_arg3 = xMeterI(call_arg4, call_arg5);
	
	printf("%f\n", call_arg3);
	
	call_arg8 = position.lat / 1000.0;
	
	call_arg7 = yMeterI(call_arg8);
	
	printf("%f\n", call_arg7);
	
	{
		Range * path;
		int i;
		_Bool cond;
		
		call_arg10 = 5.85;
		
		call_arg9 = TurningFlightPrediction(Vground, TrueTrack, position, call_arg10, RTC);
		
		path = call_arg9;
		
		i = 0;
		
		call_arg11 = CalculateLAT(Vground);
		
		cond = (i < call_arg11 + 1);
		
		for (; cond; ) {
			printf("%f", path[i].center.X);
			
			printf(", ");
			
			printf("%f", path[i].center.Y);
			
			printf(";\n");
			
			printf("%f", path[i].limit1.X);
			
			printf(", ");
			
			printf("%f", path[i].limit1.Y);
			
			printf(";\n");
			
			printf("%f", path[i].limit2.X);
			
			printf(", ");
			
			printf("%f", path[i].limit2.Y);
			
			printf(";\n");
			
			i = i + 1;
			
			call_arg11 = CalculateLAT(Vground);
			
			cond = (i < call_arg11 + 1);
		}
		
		{
			point * envelope;
			int result;
			
			call_arg12 = LevelEnvelopGeneration(RTC, Vground, position);
			
			envelope = call_arg12;
			
			call_arg14 = 'l';
			
			call_arg13 = intersection(path, envelope, Vground, call_arg14);
			
			result = call_arg13;
			
			if (result == 1) {
			}
		}
	}
}

Range * StraightFlightPrediction_p2(float Vground_p2, float TrueTrack_p2, Pos position_p2, float Clearance_p2) {
	double rad_p2 = 3.14159265 / 180.0;
	int call_arg_p2;
	float a_p2 = TrueTrack_p2 * rad_p2;
	int LookAheadTime_p2;
	double call_arg2_p2;
	double call_arg3_p2;
	double call_arg4_p2;
	double call_arg5_p2;
	double call_arg7_p2;
	double call_arg6_p2;
	double call_arg9_p2;
	double call_arg8_p2;
	double call_arg25_p2;
	double call_arg26_p2;
	double call_arg27_p2;
	double call_arg28_p2;
	double call_arg29_p2;
	double call_arg30_p2;
	double call_arg33_p2;
	double call_arg32_p2;
	double call_arg31_p2;
	double call_arg35_p2;
	double call_arg34_p2;
	double call_arg37_p2;
	double call_arg36_p2;
	double call_arg38_p2;
	double call_arg39_p2;
	
	call_arg_p2 = CalculateLAT(Vground_p2);
	
	Vground_p2 = Vground_p2 * 0.514444;
	
	LookAheadTime_p2 = call_arg_p2;
	
	{
		double StraightDis_p2 = Vground_p2 * LookAheadTime_p2;
		point container_p2;
		static Range path_p2[100];
		
		container_p2.X = 0;
		container_p2.Y = 0;
		path_p2[0].center = container_p2;
		
		{
			double DeltaX_p2;
			double DeltaY_p2;
			double Slope_p2;
			
			for (int i_p2 = 1; i_p2 <= LookAheadTime_p2; i_p2 = i_p2 + 1) {
				_Bool cond3_p2;
				_Bool cond22_p2;
				_Bool cond_p2;
				
				cond3_p2 = 1;
				
				call_arg2_p2 = fabs(TrueTrack_p2);
				
				cond22_p2 = !(call_arg2_p2 == 0);
				
				if (cond22_p2) {
					call_arg3_p2 = fabs(TrueTrack_p2);
					
					cond3_p2 = (call_arg3_p2 == 180);
				}
				
				cond_p2 = cond3_p2;
				
				if (cond_p2) {
					DeltaX_p2 = path_p2[0].center.X;
					
					call_arg4_p2 = fabs(path_p2[0].center.Y);
					
					call_arg5_p2 = cos(a_p2);
					
					DeltaY_p2 = (Vground_p2 * i_p2 + call_arg4_p2) * call_arg5_p2;
				} else {
					call_arg7_p2 = 3.14159265 / 2 - a_p2;
					
					call_arg6_p2 = cos(call_arg7_p2);
					
					DeltaX_p2 = path_p2[0].center.X + Vground_p2 * i_p2 * call_arg6_p2;
					
					call_arg9_p2 = 3.14159265 / 2 - a_p2;
					
					call_arg8_p2 = tan(call_arg9_p2);
					
					DeltaY_p2 = call_arg8_p2 * (DeltaX_p2 - path_p2[0].center.X) + path_p2[0].center.Y;
				}
				
				container_p2.X = DeltaX_p2;
				container_p2.Y = DeltaY_p2;
				path_p2[i_p2].center = container_p2;
				path_p2[i_p2].distance = Vground_p2 * i_p2;
			}
			
			Slope_p2 = EMX_Recv64F(3, 2, 4, -1);
			
			
			EMX_Send(2, 3, 3, -1, path_p2, 100 * sizeof(*path_p2));
			
			EMX_Send(2, 3, 2, -1, &container_p2, sizeof(container_p2));
			
			EMX_Recv(3, 2, 5, -1, &container_p2, sizeof(container_p2));
			
			EMX_Recv(3, 2, 6, -1, path_p2, 100 * sizeof(*path_p2));
			
			for (int i_p2 = 1; i_p2 <= LookAheadTime_p2; i_p2 = i_p2 + 1) {
				_Bool cond4_p2;
				_Bool cond32_p2;
				_Bool cond2_p2;
				
				cond4_p2 = 1;
				
				call_arg25_p2 = fabs(TrueTrack_p2);
				
				cond32_p2 = !(call_arg25_p2 == 90.0);
				
				if (cond32_p2) {
					call_arg26_p2 = fabs(TrueTrack_p2);
					
					cond4_p2 = (call_arg26_p2 == 270.0);
				}
				
				cond2_p2 = cond4_p2;
				
				if (cond2_p2) {
					double ClearanceY_p2 = path_p2[i_p2].center.Y + Clearance_p2;
					
					container_p2.X = path_p2[i_p2].center.X;
					container_p2.Y = ClearanceY_p2;
					path_p2[i_p2].limit1 = container_p2;
					ClearanceY_p2 = path_p2[i_p2].center.Y - Clearance_p2;
					container_p2.X = path_p2[i_p2].center.X;
					container_p2.Y = ClearanceY_p2;
					path_p2[i_p2].limit2 = container_p2;
				} else {
					double A1_p2;
					double A2_p2;
					double A3_p2;
					double temp_p2;
					double sol1_p2;
					double sol2_p2;
					double ClearanceX_p2;
					double ClearanceY_p2;
					
					call_arg27_p2 = tan(Slope_p2);
					
					call_arg28_p2 = tan(Slope_p2);
					
					A1_p2 = call_arg27_p2 * call_arg28_p2 + 1;
					
					call_arg29_p2 = tan(Slope_p2);
					
					call_arg30_p2 = tan(Slope_p2);
					
					A2_p2 = -2 * path_p2[i_p2].center.X - 2 * call_arg29_p2 * call_arg30_p2 * path_p2[i_p2].center.X;
					
					call_arg33_p2 = tan(Slope_p2);
					
					call_arg32_p2 = call_arg33_p2 * path_p2[i_p2].center.X;
					
					call_arg31_p2 = pow(call_arg32_p2, 2);
					
					A3_p2 = path_p2[i_p2].center.X * path_p2[i_p2].center.X + call_arg31_p2 - Clearance_p2 * Clearance_p2;
					
					temp_p2 = A2_p2 * A2_p2 - 4 * A1_p2 * A3_p2;
					
					call_arg35_p2 = A2_p2 * A2_p2 - 4 * A1_p2 * A3_p2;
					
					call_arg34_p2 = sqrt(call_arg35_p2);
					
					sol1_p2 = (-A2_p2 + call_arg34_p2) / (2 * A1_p2);
					
					call_arg37_p2 = A2_p2 * A2_p2 - 4 * A1_p2 * A3_p2;
					
					call_arg36_p2 = sqrt(call_arg37_p2);
					
					sol2_p2 = (-A2_p2 - call_arg36_p2) / (2 * A1_p2);
					
					ClearanceX_p2 = sol1_p2;
					
					call_arg38_p2 = tan(Slope_p2);
					
					ClearanceY_p2 = call_arg38_p2 * (ClearanceX_p2 - path_p2[i_p2].center.X) + path_p2[i_p2].center.Y;
					
					container_p2.X = ClearanceX_p2;
					container_p2.Y = ClearanceY_p2;
					path_p2[i_p2].limit1 = container_p2;
					ClearanceX_p2 = sol2_p2;
					
					call_arg39_p2 = tan(Slope_p2);
					
					ClearanceY_p2 = call_arg39_p2 * (ClearanceX_p2 - path_p2[i_p2].center.X) + path_p2[i_p2].center.Y;
					
					container_p2.X = ClearanceX_p2;
					container_p2.Y = ClearanceY_p2;
					path_p2[i_p2].limit2 = container_p2;
				}
			}
			
			for (int i_p2 = 0; i_p2 <= LookAheadTime_p2; i_p2 = i_p2 + 1) {
				path_p2[i_p2].center.X = path_p2[i_p2].center.X + position_p2.lon;
				path_p2[i_p2].center.Y = path_p2[i_p2].center.Y + position_p2.lat;
				path_p2[i_p2].limit1.X = path_p2[i_p2].limit1.X + position_p2.lon;
				path_p2[i_p2].limit1.Y = path_p2[i_p2].limit1.Y + position_p2.lat;
				path_p2[i_p2].limit2.X = path_p2[i_p2].limit2.X + position_p2.lon;
				path_p2[i_p2].limit2.Y = path_p2[i_p2].limit2.Y + position_p2.lat;
			}
			
			return path_p2;
		}
	}
}

void StraightFlightPrediction_p3(float TrueTrack_p3, float Clearance_p3) {
	double rad_p3 = 3.14159265 / 180.0;
	double call_arg10_p3;
	double call_arg11_p3;
	double call_arg12_p3;
	double call_arg13_p3;
	double call_arg14_p3;
	double call_arg15_p3;
	double call_arg18_p3;
	double call_arg17_p3;
	double call_arg16_p3;
	double call_arg20_p3;
	double call_arg19_p3;
	double call_arg22_p3;
	double call_arg21_p3;
	double call_arg23_p3;
	double call_arg24_p3;
	static Range path_p3[100];
	point container_p3;
	_Bool cond22_p3;
	double Slope_p3 = 3.14159265 - TrueTrack_p3 * rad_p3;
	_Bool cond3_p3;
	_Bool cond_p3;
	
	call_arg10_p3 = fabs(TrueTrack_p3);
	
	cond22_p3 = !(call_arg10_p3 == 90);
	
	cond3_p3 = 1;
	
	EMX_Send64F(3, 2, 4, -1, Slope_p3);
	
	if (cond22_p3) {
		call_arg11_p3 = fabs(TrueTrack_p3);
		
		cond3_p3 = (call_arg11_p3 == 270);
	}
	
	cond_p3 = cond3_p3;
	
	EMX_Recv(2, 3, 3, -1, path_p3, 100 * sizeof(*path_p3));
	
	EMX_Recv(2, 3, 2, -1, &container_p3, sizeof(container_p3));
	
	if (cond_p3) {
		double ClearanceY_p3 = path_p3[0].center.Y + Clearance_p3;
		
		container_p3.X = path_p3[0].center.X;
		container_p3.Y = ClearanceY_p3;
		path_p3[0].limit1 = container_p3;
		ClearanceY_p3 = path_p3[0].center.Y - Clearance_p3;
		container_p3.X = path_p3[0].center.X;
		container_p3.Y = ClearanceY_p3;
		path_p3[0].limit2 = container_p3;
	} else {
		double A1_p3;
		double A2_p3;
		double A3_p3;
		double sol1_p3;
		double sol2_p3;
		double ClearanceX_p3;
		double ClearanceY_p3;
		
		call_arg12_p3 = tan(Slope_p3);
		
		call_arg13_p3 = tan(Slope_p3);
		
		A1_p3 = call_arg12_p3 * call_arg13_p3 + 1;
		
		call_arg14_p3 = tan(Slope_p3);
		
		call_arg15_p3 = tan(Slope_p3);
		
		A2_p3 = -2 * path_p3[0].center.X - 2 * call_arg14_p3 * call_arg15_p3 * path_p3[0].center.X;
		
		call_arg18_p3 = tan(Slope_p3);
		
		call_arg17_p3 = call_arg18_p3 * path_p3[0].center.X;
		
		call_arg16_p3 = pow(call_arg17_p3, 2);
		
		A3_p3 = path_p3[0].center.X * path_p3[0].center.X + call_arg16_p3 - Clearance_p3 * Clearance_p3;
		
		call_arg20_p3 = A2_p3 * A2_p3 - 4 * A1_p3 * A3_p3;
		
		call_arg19_p3 = sqrt(call_arg20_p3);
		
		sol1_p3 = (-A2_p3 + call_arg19_p3) / (2 * A1_p3);
		
		call_arg22_p3 = A2_p3 * A2_p3 - 4 * A1_p3 * A3_p3;
		
		call_arg21_p3 = sqrt(call_arg22_p3);
		
		sol2_p3 = (-A2_p3 - call_arg21_p3) / (2 * A1_p3);
		
		ClearanceX_p3 = sol1_p3;
		
		call_arg23_p3 = tan(Slope_p3);
		
		ClearanceY_p3 = call_arg23_p3 * (ClearanceX_p3 - path_p3[0].center.X) + path_p3[0].center.Y;
		
		container_p3.X = ClearanceX_p3;
		container_p3.Y = ClearanceY_p3;
		path_p3[0].limit1 = container_p3;
		ClearanceX_p3 = sol2_p3;
		
		call_arg24_p3 = tan(Slope_p3);
		
		ClearanceY_p3 = call_arg24_p3 * (ClearanceX_p3 - path_p3[0].center.X) + path_p3[0].center.Y;
		
		container_p3.X = ClearanceX_p3;
		container_p3.Y = ClearanceY_p3;
		path_p3[0].limit2 = container_p3;
	}
	
	EMX_Send(3, 2, 5, -1, &container_p3, sizeof(container_p3));
	
	EMX_Send(3, 2, 6, -1, path_p3, 100 * sizeof(*path_p3));
}




