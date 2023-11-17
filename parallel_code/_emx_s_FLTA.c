
#include "FLTA.h"
#include <math.h>
#include <stdio.h>
#include "_emx_main.h"
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

int intersection_duplicate2(Range path[100], point envelope[100], float Vground, char type) {
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
	
	EMX_Send32(2, 3, 4, -1, call_arg_p2);
	
	Vground_p2 = Vground_p2 * 0.514444;
	
	LookAheadTime_p2 = EMX_Recv32(3, 2, 5, -1);
	
	{
		double StraightDis_p2 = Vground_p2 * LookAheadTime_p2;
		point container_p2;
		static Range path_p2[100];
		
		EMX_Recv(3, 2, 8, -1, path_p2, 100 * sizeof(*path_p2));
		
		EMX_Recv(3, 2, 6, -1, &container_p2, sizeof(container_p2));
		
		{
			double DeltaX_p2;
			double DeltaY_p2;
			double Slope_p2;
			
			for (int i_p2 = 1; i_p2 <= LookAheadTime_p2; i_p2 = i_p2 + 1) {
				_Bool cond3_p2;
				int cond22_p2;
				int cond_p2;
				
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
			
			Slope_p2 = EMX_Recv64F(3, 2, 12, -1);
			
			
			EMX_Send(2, 3, 11, -1, path_p2, 100 * sizeof(*path_p2));
			
			EMX_Send(2, 3, 10, -1, &container_p2, sizeof(container_p2));
			
			EMX_Recv(3, 2, 7, -1, &container_p2, sizeof(container_p2));
			
			EMX_Recv(3, 2, 9, -1, path_p2, 100 * sizeof(*path_p2));
			
			for (int i_p2 = 1; i_p2 <= LookAheadTime_p2; i_p2 = i_p2 + 1) {
				_Bool cond4_p2;
				int cond32_p2;
				int cond2_p2;
				
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
	int LookAheadTime_p3;
	int call_arg_p3;
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
	
	call_arg_p3 = EMX_Recv32(2, 3, 4, -1);
	
	LookAheadTime_p3 = call_arg_p3;
	
	EMX_Send32(3, 2, 5, -1, LookAheadTime_p3);
	
	{
		point container_p3;
		static Range path_p3[100];
		
		container_p3.X = 0;
		container_p3.Y = 0;
		path_p3[0].center = container_p3;
		
		EMX_Send(3, 2, 8, -1, path_p3, 100 * sizeof(*path_p3));
		
		EMX_Send(3, 2, 6, -1, &container_p3, sizeof(container_p3));
		
		{
			int cond22_p3;
			double Slope_p3 = 3.14159265 - TrueTrack_p3 * rad_p3;
			_Bool cond3_p3;
			int cond_p3;
			
			call_arg10_p3 = fabs(TrueTrack_p3);
			
			cond22_p3 = !(call_arg10_p3 == 90);
			
			cond3_p3 = 1;
			
			EMX_Send64F(3, 2, 12, -1, Slope_p3);
			
			if (cond22_p3) {
				call_arg11_p3 = fabs(TrueTrack_p3);
				
				cond3_p3 = (call_arg11_p3 == 270);
			}
			
			cond_p3 = cond3_p3;
			
			EMX_Recv(2, 3, 11, -1, path_p3, 100 * sizeof(*path_p3));
			
			EMX_Recv(2, 3, 10, -1, &container_p3, sizeof(container_p3));
			
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
			
			EMX_Send(3, 2, 7, -1, &container_p3, sizeof(container_p3));
			
			EMX_Send(3, 2, 9, -1, path_p3, 100 * sizeof(*path_p3));
		}
	}
}

Range * TurningFlightPrediction_p2(float Vground_p2, float TrueTrack_p2, Pos position_p2, float YawRate_p2, float Clearance_p2) {
	int8_t sync_p2;
	double rad_p2 = 3.14159265 / 180.0;
	int call_arg_p2;
	double PredY_p2 = 0;
	double PredX_p2 = 0;
	double Alt_p2 = position_p2.alt;
	double a_p2 = TrueTrack_p2 * rad_p2;
	double call_arg4_p2;
	double call_arg5_p2;
	double call_arg7_p2;
	double call_arg6_p2;
	double call_arg8_p2;
	double call_arg9_p2;
	double call_arg12_p2;
	double call_arg11_p2;
	double call_arg10_p2;
	double call_arg14_p2;
	double call_arg13_p2;
	double call_arg16_p2;
	double call_arg15_p2;
	double call_arg17_p2;
	double call_arg18_p2;
	double call_arg20_p2;
	double call_arg19_p2;
	int LookAheadTime_p2;
	double call_arg22_p2;
	double call_arg29_p2;
	double call_arg25_p2;
	double call_arg28_p2;
	double call_arg27_p2;
	double call_arg34_p2;
	double call_arg35_p2;
	
	call_arg_p2 = CalculateLAT(Vground_p2);
	
	EMX_Send32(2, 3, 13, -1, call_arg_p2);
	
	Vground_p2 = Vground_p2 * 0.514444;
	
	LookAheadTime_p2 = EMX_Recv32(3, 2, 15, -1);
	
	EMX_Send64F(2, 3, 14, -1, a_p2);
	
	{
		point container_p2;
		static Range path_p2[100];
		
		container_p2.X = PredX_p2 + position_p2.lon;
		container_p2.Y = PredY_p2 + position_p2.lat;
		path_p2[0].center = container_p2;
		
		{
			double Radius_p2 = Vground_p2 * 360 / YawRate_p2 / 3.14159265 / 2;
			double theta_p2 = 3.14159265 - a_p2;
			int cond22_p2;
			_Bool cond3_p2;
			int cond_p2;
			
			EMX_Send64F(2, 3, 17, -1, theta_p2);
			
			call_arg4_p2 = fabs(TrueTrack_p2);
			
			cond22_p2 = !(call_arg4_p2 == 90);
			
			cond3_p2 = EMX_Recv8(3, 2, 18, -1);
			
			EMX_Send64F(2, 3, 16, -1, Radius_p2);
			
			if (cond22_p2) {
				call_arg5_p2 = fabs(TrueTrack_p2);
				
				cond3_p2 = (call_arg5_p2 == 270);
			}
			
			cond_p2 = cond3_p2;
			
			if (cond_p2) {
				double ClearanceY_p2 = PredY_p2 + Clearance_p2;
				
				container_p2.X = PredX_p2 + position_p2.lon;
				container_p2.Y = ClearanceY_p2 + position_p2.lat;
				path_p2[0].limit1 = container_p2;
				ClearanceY_p2 = PredY_p2 - Clearance_p2;
				container_p2.X = PredX_p2 + position_p2.lon;
				container_p2.Y = ClearanceY_p2 + position_p2.lat;
				path_p2[0].limit2 = container_p2;
			} else {
				double A1_p2;
				double A2_p2;
				double A3_p2;
				double sol1_p2;
				double sol2_p2;
				double ClearanceY_p2;
				
				call_arg7_p2 = tan(theta_p2);
				
				call_arg6_p2 = pow(call_arg7_p2, 2.0);
				
				A1_p2 = call_arg6_p2 + 1;
				
				call_arg8_p2 = tan(theta_p2);
				
				call_arg9_p2 = tan(theta_p2);
				
				A2_p2 = -2 * PredX_p2 - 2 * call_arg8_p2 * call_arg9_p2 * PredX_p2;
				
				call_arg12_p2 = tan(theta_p2);
				
				call_arg11_p2 = call_arg12_p2 * PredX_p2;
				
				call_arg10_p2 = pow(call_arg11_p2, 2.0);
				
				A3_p2 = PredX_p2 * PredX_p2 + call_arg10_p2 - Clearance_p2 * Clearance_p2;
				
				call_arg14_p2 = A2_p2 * A2_p2 - 4 * A1_p2 * A3_p2;
				
				call_arg13_p2 = sqrt(call_arg14_p2);
				
				sol1_p2 = (-A2_p2 + call_arg13_p2) / (2 * A1_p2);
				
				call_arg16_p2 = A2_p2 * A2_p2 - 4 * A1_p2 * A3_p2;
				
				call_arg15_p2 = sqrt(call_arg16_p2);
				
				sol2_p2 = (-A2_p2 - call_arg15_p2) / (2 * A1_p2);
				
				call_arg17_p2 = tan(theta_p2);
				
				ClearanceY_p2 = call_arg17_p2 * (sol1_p2 - PredX_p2) + PredY_p2;
				
				container_p2.X = sol1_p2 + position_p2.lon;
				container_p2.Y = ClearanceY_p2 + position_p2.lat;
				path_p2[0].limit1 = container_p2;
				
				call_arg18_p2 = tan(theta_p2);
				
				ClearanceY_p2 = call_arg18_p2 * (sol2_p2 - PredX_p2) + PredY_p2;
				
				container_p2.X = sol2_p2 + position_p2.lon;
				container_p2.Y = ClearanceY_p2 + position_p2.lat;
				path_p2[0].limit2 = container_p2;
			}
			
			{
				double DeltaArch_p2 = Vground_p2 * 1;
				double DeltaTheta_p2 = DeltaArch_p2 / Radius_p2;
				double chord_p2;
				double Slope_p2;
				double DeltaSlope_p2;
				
				EMX_Send64F(2, 3, 19, -1, DeltaTheta_p2);
				
				call_arg20_p2 = 0.5 * DeltaTheta_p2;
				
				call_arg19_p2 = sin(call_arg20_p2);
				
				chord_p2 = 2 * Radius_p2 * call_arg19_p2;
				
				Slope_p2 = EMX_Recv64F(3, 2, 22, -1);
				
				EMX_Send64F(2, 3, 21, -1, chord_p2);
				
				DeltaSlope_p2 = EMX_Recv64F(3, 2, 20, -1);
				
				for (int i_p2 = 1; sync_p2 = (i_p2 <= LookAheadTime_p2), EMX_SendSync(2, 3, 33, -1, sync_p2), sync_p2; i_p2 = i_p2 + 1) {
					EMX_Send64F(2, 3, 25, -1, DeltaSlope_p2);
					
					EMX_Send64F(2, 3, 24, -1, PredX_p2);
					
					{
						double DeltaY_p2;
						double DeltaX_p2;
						
						call_arg22_p2 = tan(DeltaSlope_p2);
						
						DeltaX_p2 = EMX_Recv64F(3, 2, 26, -1);
						
						DeltaY_p2 = call_arg22_p2 * (DeltaX_p2 - PredX_p2) + PredY_p2;
						
						PredX_p2 = DeltaX_p2;
						PredY_p2 = DeltaY_p2;
						DeltaSlope_p2 = DeltaSlope_p2 - DeltaTheta_p2;
						Slope_p2 = Slope_p2 - DeltaTheta_p2;
						
						EMX_Send64F(2, 3, 23, -1, Slope_p2);
						
						{
							double sol1Y_p2;
							double ClearanceX_p2;
							double sol1_p2;
							double sol2_p2;
							
							call_arg29_p2 = tan(Slope_p2);
							
							call_arg25_p2 = tan(Slope_p2);
							
							call_arg28_p2 = call_arg29_p2 * DeltaX_p2;
							
							call_arg27_p2 = pow(call_arg28_p2, 2.0);
							
							EMX_Send64F(2, 3, 28, -1, call_arg27_p2);
							
							EMX_Send64F(2, 3, 27, -1, call_arg25_p2);
							
							call_arg34_p2 = tan(Slope_p2);
							
							ClearanceX_p2 = EMX_Recv64F(3, 2, 31, -1);
							
							sol1Y_p2 = call_arg34_p2 * (ClearanceX_p2 - DeltaX_p2) + DeltaY_p2;
							
							ClearanceX_p2 = EMX_Recv64F(3, 2, 32, -1);
							
							sol2_p2 = EMX_Recv64F(3, 2, 30, -1);
							
							sol1_p2 = EMX_Recv64F(3, 2, 29, -1);
							
							{
								double sol2Y_p2;
								
								call_arg35_p2 = tan(Slope_p2);
								
								sol2Y_p2 = call_arg35_p2 * (ClearanceX_p2 - DeltaX_p2) + DeltaY_p2;
								
								container_p2.X = DeltaX_p2 + position_p2.lon;
								container_p2.Y = DeltaY_p2 + position_p2.lat;
								path_p2[i_p2].center = container_p2;
								container_p2.X = sol1_p2 + position_p2.lon;
								container_p2.Y = sol1Y_p2 + position_p2.lat;
								path_p2[i_p2].limit1 = container_p2;
								container_p2.X = sol2_p2 + position_p2.lon;
								container_p2.Y = sol2Y_p2 + position_p2.lat;
								path_p2[i_p2].limit2 = container_p2;
								path_p2[i_p2].distance = DeltaArch_p2 * i_p2;
							}
						}
					}
					
					
				}
				
				return path_p2;
			}
		}
	}
}

void TurningFlightPrediction_p3(float Clearance_p3) {
	int LookAheadTime_p3;
	int call_arg_p3;
	double call_arg3_p3;
	double call_arg2_p3;
	double PredX_p3 = 0;
	double PredY_p3 = 0;
	double a_p3;
	double call_arg21_p3;
	double call_arg24_p3;
	double call_arg26_p3;
	double call_arg23_p3;
	double call_arg25_p3;
	double call_arg27_p3;
	double call_arg31_p3;
	double call_arg33_p3;
	double call_arg30_p3;
	double call_arg32_p3;
	
	call_arg_p3 = EMX_Recv32(2, 3, 13, -1);
	
	LookAheadTime_p3 = call_arg_p3;
	
	EMX_Send32(3, 2, 15, -1, LookAheadTime_p3);
	
	a_p3 = EMX_Recv64F(2, 3, 14, -1);
	
	{
		_Bool cond3_p3;
		double theta_p3;
		double Xc_p3;
		double Radius_p3;
		double Yc_p3;
		
		theta_p3 = EMX_Recv64F(2, 3, 17, -1);
		
		cond3_p3 = 1;
		
		EMX_Send8(3, 2, 18, -1, cond3_p3);
		
		Radius_p3 = EMX_Recv64F(2, 3, 16, -1);
		
		call_arg3_p3 = tan(theta_p3);
		
		call_arg2_p3 = cos(theta_p3);
		
		Xc_p3 = PredX_p3 - Radius_p3 * call_arg2_p3;
		
		Yc_p3 = call_arg3_p3 * (Xc_p3 - PredX_p3) + PredY_p3;
		
		{
			double DeltaSlope_p3;
			double DeltaTheta_p3;
			double Slope_p3;
			double chord_p3;
			
			DeltaTheta_p3 = EMX_Recv64F(2, 3, 19, -1);
			
			DeltaSlope_p3 = 3.14159265 / 2.0 - a_p3 - DeltaTheta_p3 / 2.0;
			
			Slope_p3 = theta_p3;
			
			EMX_Send64F(3, 2, 22, -1, Slope_p3);
			
			chord_p3 = EMX_Recv64F(2, 3, 21, -1);
			
			EMX_Send64F(3, 2, 20, -1, DeltaSlope_p3);
			
			for (int i_p3 = 1; EMX_RecvSync(2, 3, 33, -1); i_p3 = i_p3 + 1) {
				DeltaSlope_p3 = EMX_Recv64F(2, 3, 25, -1);
				
				PredX_p3 = EMX_Recv64F(2, 3, 24, -1);
				
				{
					double DeltaX_p3;
					
					call_arg21_p3 = cos(DeltaSlope_p3);
					
					DeltaX_p3 = PredX_p3 + chord_p3 * call_arg21_p3;
					
					EMX_Send64F(3, 2, 26, -1, DeltaX_p3);
					
					Slope_p3 = EMX_Recv64F(2, 3, 23, -1);
					
					{
						double A2_p3;
						double A3_p3;
						double A1_p3;
						double sol1_p3;
						double sol2_p3;
						double ClearanceX_p3;
						
						call_arg24_p3 = tan(Slope_p3);
						
						call_arg26_p3 = tan(Slope_p3);
						
						call_arg23_p3 = pow(call_arg24_p3, 2.0);
						
						call_arg27_p3 = EMX_Recv64F(2, 3, 28, -1);
						
						call_arg25_p3 = EMX_Recv64F(2, 3, 27, -1);
						
						A2_p3 = -2 * DeltaX_p3 - 2 * call_arg25_p3 * call_arg26_p3 * DeltaX_p3;
						
						A3_p3 = DeltaX_p3 * DeltaX_p3 + call_arg27_p3 - Clearance_p3 * Clearance_p3;
						
						A1_p3 = call_arg23_p3 + 1;
						
						call_arg31_p3 = A2_p3 * A2_p3 - 4 * A1_p3 * A3_p3;
						
						call_arg33_p3 = A2_p3 * A2_p3 - 4 * A1_p3 * A3_p3;
						
						call_arg30_p3 = sqrt(call_arg31_p3);
						
						call_arg32_p3 = sqrt(call_arg33_p3);
						
						sol1_p3 = (-A2_p3 + call_arg30_p3) / (2 * A1_p3);
						
						sol2_p3 = (-A2_p3 - call_arg32_p3) / (2 * A1_p3);
						
						ClearanceX_p3 = sol1_p3;
						
						EMX_Send64F(3, 2, 31, -1, ClearanceX_p3);
						
						ClearanceX_p3 = sol2_p3;
						
						EMX_Send64F(3, 2, 32, -1, ClearanceX_p3);
						
						EMX_Send64F(3, 2, 30, -1, sol2_p3);
						
						EMX_Send64F(3, 2, 29, -1, sol1_p3);
					}
				}
			}
		}
	}
}




