/*
 * ============================================================================
 *
 *       Filename:  FLTA.c
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

#include "FLTA.h"
#include <math.h>

#include <stdio.h>

#define PI 3.14159265
#define G 32.1741


//Forward declaration for local functions (Helper/input-level functions)


//Forward declaration for local functions (Intermediate-level functions)
static void ClimbingEnvelopGeneration();

//Forward declaration for local functions (Deep-level functions)
static void EnvelopCalc();
static void Intersection();

//Forward declaration for local functions (Root-level function)
static void Step();

//Functions for Helper Component
int CalculateLAT(float Vground)
{
	//Calculate ground speed
	if (Vground <= 100) {
	    return 20;
	} else if (Vground <= 150) {
		return 20;
	}else if (Vground <= 200) {
		return 25;
	}else if (Vground <= 250) {
		return 27;
	}else if (Vground <= 300) {
		return 31;
	}else if (Vground <= 350) {
		return 35;
	}else if (Vground <= 450) {
		return 32;
	}else if (Vground <= 550) {
		return 27;
	}else if (Vground <= 600) {
		return 25;
	}else{
		return 15;
	}

}


float VectorAngleCosine(float u1, float u2, float v1, float v2)
{
    // Calculate the length
    float u = sqrtf(powf(u1, 2.0f) + powf(u2, 2.0f));
    float v = sqrtf(powf(v1, 2.0f) + powf(v2, 2.0f));
    // Calculate the dot product
    float dot = u1 * v1 + u2 * v2;
    // Angle formula
    return dot / (u * v);
}

Range* StraightFlightPrediction(float Vground, float TrueTrack, Pos position, float Clearance) {
    double rad = PI / 180.0;
    float a = TrueTrack * rad;
    int LookAheadTime = CalculateLAT(Vground);
    Vground = Vground * 0.514444; // Convert from knots to meters per second
    double StraightDis = Vground * LookAheadTime;

    static Range path[100];
    point container;

    // Initialize the center position
    container.X = 0;
    container.Y = 0;
    path[0].center = container;

    // Calculate the new coordinates of the chord
    double DeltaX, DeltaY;
    double Slope = PI - TrueTrack * rad;
    for (int i = 1; i <= LookAheadTime; i++) {
        if ((fabs(TrueTrack) == 0) || (fabs(TrueTrack) == 180)) {
            DeltaX = path[0].center.X;
            DeltaY = (Vground * i + fabs(path[0].center.Y)) * cos(a);
        } else {
            DeltaX = path[0].center.X + (Vground * i * cos(PI / 2 - a));
            DeltaY = tan(PI / 2 - a) * (DeltaX - path[0].center.X) + path[0].center.Y;
        }
        container.X = DeltaX;
        container.Y = DeltaY;
        path[i].center = container;
        path[i].distance = Vground * i;
    }

    // Calculate clearance for the aircraft current position
    if ((fabs(TrueTrack) == 90) || (fabs(TrueTrack) == 270)) {
        double ClearanceY = path[0].center.Y + Clearance;

        container.X = path[0].center.X;
        container.Y = ClearanceY;
        path[0].limit1 = container;

        ClearanceY = path[0].center.Y - Clearance;

        container.X = path[0].center.X;
        container.Y = ClearanceY;
        path[0].limit2 = container;
    } else {
        double A1 = tan(Slope) * tan(Slope) + 1;
        double A2 = (-2 * path[0].center.X) - 2 * tan(Slope) * tan(Slope) * path[0].center.X;
        double A3 = path[0].center.X * path[0].center.X + pow(tan(Slope) * path[0].center.X, 2) - Clearance * Clearance;

        double sol1 = (-A2 + sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);
        double sol2 = (-A2 - sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);

        double ClearanceX = sol1;
        double ClearanceY = tan(Slope) * (ClearanceX - path[0].center.X) + path[0].center.Y;

        container.X = ClearanceX;
        container.Y = ClearanceY;
        path[0].limit1 = container;

        ClearanceX = sol2;
        ClearanceY = tan(Slope) * (ClearanceX - path[0].center.X) + path[0].center.Y;

        container.X = ClearanceX;
        container.Y = ClearanceY;
        path[0].limit2 = container;
    }

    // Calculate clearance for the aircraft predicted position
    for (int i = 1; i <= LookAheadTime; i++) {
        if ((fabs(TrueTrack) == 90.0) || (fabs(TrueTrack) == 270.0)) {
            double ClearanceY = path[i].center.Y + Clearance;

            container.X = path[i].center.X;
            container.Y = ClearanceY;
            path[i].limit1 = container;

            ClearanceY = path[i].center.Y - Clearance;

            container.X = path[i].center.X;
            container.Y = ClearanceY;
            path[i].limit2 = container;
        } else {
            double A1 = tan(Slope) * tan(Slope) + 1;
            double A2 = (-2 * path[i].center.X) - 2 * tan(Slope) * tan(Slope) * path[i].center.X;
            double A3 = path[i].center.X * path[i].center.X + pow(tan(Slope) * path[i].center.X, 2) - Clearance * Clearance;

            double temp = A2 * A2 - 4 * A1 * A3;
            double sol1 = (-A2 + sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);
            double sol2 = (-A2 - sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);

            double ClearanceX = sol1;
            double ClearanceY = tan(Slope) * (ClearanceX - path[i].center.X) + path[i].center.Y;

            container.X = ClearanceX;
            container.Y = ClearanceY;
            path[i].limit1 = container;

            ClearanceX = sol2;
            ClearanceY = tan(Slope) * (ClearanceX - path[i].center.X) + path[i].center.Y;

            container.X = ClearanceX;
            container.Y = ClearanceY;
            path[i].limit2 = container;
        }
    }

    // Bring back the points to the original reference
    for (int i = 0; i <= LookAheadTime; i++) {
        path[i].center.X += position.lon;
        path[i].center.Y += position.lat;
        path[i].limit1.X += position.lon;
        path[i].limit1.Y += position.lat;
        path[i].limit2.X += position.lon;
        path[i].limit2.Y += position.lat;
    }

    return path;
}

Range* TurningFlightPrediction(float Vground, float TrueTrack, Pos position, float YawRate, float Clearance) {
    double rad = PI / 180.0;
    double PredY = 0;
    double PredX = 0;
    double Alt = position.alt;
    double a = TrueTrack * rad;
    int LookAheadTime = CalculateLAT(Vground);

    Vground = Vground * 0.514444; // Convert from knots to meters per second

    static Range path[100];
    point container;
    container.X = PredX + position.lon;
    container.Y = PredY + position.lat;
    path[0].center = container;

    double Radius = ((Vground * 360 / YawRate) / PI) / 2;
    double theta = PI - a;
    double Xc = PredX - (Radius * cos(theta));
    double Yc = tan(theta) * (Xc - PredX) + PredY;

    if ((fabs(TrueTrack) == 90) || (fabs(TrueTrack) == 270)) {
        double ClearanceY = PredY + Clearance;
        container.X = PredX + position.lon;
        container.Y = ClearanceY + position.lat;
        path[0].limit1 = container;

        ClearanceY = PredY - Clearance;
        container.X = PredX + position.lon;
        container.Y = ClearanceY + position.lat;
        path[0].limit2 = container;
    } else {
        double A1 = pow(tan(theta), 2.0) + 1;
        double A2 = (-2 * PredX) - 2 * tan(theta) * tan(theta) * PredX;
        double A3 = PredX * PredX + pow(tan(theta) * PredX, 2.0) - Clearance * Clearance;

        double sol1 = (-A2 + sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);
        double sol2 = (-A2 - sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);

        double ClearanceY = tan(theta) * (sol1 - PredX) + PredY;
        container.X = sol1 + position.lon;
        container.Y = ClearanceY + position.lat;
        path[0].limit1 = container;

        ClearanceY = tan(theta) * (sol2 - PredX) + PredY;
        container.X = sol2 + position.lon;
        container.Y = ClearanceY + position.lat;
        path[0].limit2 = container;
    }

    double DeltaArch = Vground * 1;
    double DeltaTheta = DeltaArch / Radius;
    double chord = 2 * Radius * sin(0.5 * DeltaTheta);
    double Slope = theta;
    double DeltaSlope = (PI / 2.0) - a - (DeltaTheta / 2.0);

    for (int i = 1; i <= LookAheadTime; i++) {
        double DeltaX = PredX + (chord * cos(DeltaSlope));
        double DeltaY = tan(DeltaSlope) * (DeltaX - PredX) + PredY;

        PredX = DeltaX;
        PredY = DeltaY;
        DeltaSlope = DeltaSlope - DeltaTheta;
        Slope = Slope - DeltaTheta;

        double A1 = pow(tan(Slope), 2.0) + 1;
        double A2 = (-2 * DeltaX) - 2 * tan(Slope) * tan(Slope) * DeltaX;
        double A3 = DeltaX * DeltaX + pow(tan(Slope) * DeltaX, 2.0) - Clearance * Clearance;

        double sol1 = (-A2 + sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);
        double sol2 = (-A2 - sqrt(A2 * A2 - 4 * A1 * A3)) / (2 * A1);

        double ClearanceX = sol1;
        double sol1Y = tan(Slope) * (ClearanceX - DeltaX) + DeltaY;

        ClearanceX = sol2;
        double sol2Y = tan(Slope) * (ClearanceX - DeltaX) + DeltaY;

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

    return path;
}

//Intermediate-level functions
void LateralEnvelopGeneration(Vned speed, float TrueTrack, Pos position, float YawRate, int LookAheadTime, float Clearance)
{
	/*
	double rad = PI/180.0; //multiply to convert to radians and divide to revert to degrees

	////Debugging and Plotting
	printf("%f", position.lon);
	printf(", ");
	printf("%f", position.lat);
	printf(";");
	puts("\n");
	//Decide a straight flight or turning flight
	if ((YawRate < 0.1) && (YawRate > -0.1))
	{
		//Calculate for the aircraft current position
		double Slope=PI-TrueTrack*rad;
		double A1=pow(tan(Slope),2.0)+1;
		double A2=(-2*position.lon)-2*tan(Slope)*tan(Slope)*position.lon;
		double A3=position.lon*position.lon+pow(tan(Slope)*position.lon,2.0)-Clearance*Clearance;

		double sol1=(-A2+sqrt(A2*A2-4*A1*A3))/(2*A1);
		double sol2=(-A2-sqrt(A2*A2-4*A1*A3))/(2*A1);

		double ClearanceX=sol1;
		double ClearanceY=tan(Slope)*(ClearanceX-position.lon)+position.lat;

		////Debugging and Plotting
		printf("%f", ClearanceX);
		printf(", ");
		printf("%f", ClearanceY);
		printf(";");
		puts("\n");
		ClearanceX=sol2;
		ClearanceY=tan(Slope)*(ClearanceX-position.lon)+position.lat;

		////Debugging and Plotting
		printf("%f", ClearanceX);
		printf(", ");
		printf("%f", ClearanceY);
		printf(";");
		puts("\n");

		//Calculate for the aircraft predicted position
		point PredP=StraightFlightPrediction(speed, TrueTrack, position, LookAheadTime);
		Slope=PI-TrueTrack*rad;
		A1=pow(tan(Slope),2.0)+1;
		A2=(-2*PredP.X)-2*tan(Slope)*tan(Slope)*PredP.X;
		A3=PredP.X*PredP.X+pow(tan(Slope)*PredP.X,2.0)-Clearance*Clearance;

		sol1=(-A2+sqrt(A2*A2-4*A1*A3))/(2*A1);
		sol2=(-A2-sqrt(A2*A2-4*A1*A3))/(2*A1);

		ClearanceX=sol1;
		ClearanceY=tan(Slope)*(ClearanceX-PredP.X)+PredP.Y;

		////Debugging and Plotting
		printf("%f", ClearanceX);
		printf(", ");
		printf("%f", ClearanceY);
		printf(";");
		puts("\n");
		ClearanceX=sol2;
		ClearanceY=tan(Slope)*(ClearanceX-PredP.X)+PredP.Y;

		////Debugging and Plotting
		printf("%f", ClearanceX);
		printf(", ");
		printf("%f", ClearanceY);
		printf(";");
		puts("\n");

	}else
	{

	}

	return;
	*/
}

point* LevelEnvelopGeneration(int RTC, float Vground, Pos position) {
    Vground = Vground * 0.514444; // Convert from knots to meters per second
    int LookAheadTime = CalculateLAT(Vground);

    point A, B, C;
    A.X = 0;
    A.Y = position.alt - RTC;
    printf("Debug %f %d %f\n", Vground, LookAheadTime, Vground * LookAheadTime);

    B.X = Vground * LookAheadTime;
    B.Y = A.Y;

    C.X = B.X;
    C.Y = position.alt;

    static point envelope[4];
    envelope[0] = A;
    envelope[1] = B;
    envelope[2] = C;

    return envelope;
}

point* DescendEnvelopGeneration(int RTC, float Vground, Pos position, float VerticalSpeed) {
    Vground = Vground * 0.514444; // Convert from knots to meters per second
    int LookAheadTime = CalculateLAT(Vground);

    float AltitudeLoss;
    int timeToLevel;
    point A, B, C, D;
    A.X = 0;
    A.Y = position.alt - RTC;

    // 3-seconds pilot delay
    AltitudeLoss = VerticalSpeed * 3 * (-1);
    timeToLevel = 3;

    // Altitude lost to level the plane. 1 for 1G maneuver
    AltitudeLoss = AltitudeLoss + (0.5 * VerticalSpeed * VerticalSpeed / (1 * G));
    timeToLevel = timeToLevel + (0.5 * VerticalSpeed / (1 * G));

    B.X = timeToLevel * Vground;
    B.Y = position.alt - RTC - AltitudeLoss;

    C.X = Vground * LookAheadTime;
    C.Y = B.Y;

    D.X = C.X;
    D.Y = position.alt;

    static point envelope[4];
    envelope[0] = A;
    envelope[1] = B;
    envelope[2] = C;
    envelope[3] = D;

    return envelope;
}

int intersection(Range path[100], point envelope[100], float Vground, char type) {
	int call_arg;
	int call_arg2;

    // Level: 'l'
    // Descend: 'd'
    // Imminent Terrain Impact: 'i'

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
		// Calculate the A-B line equation: y − y1 = m(x − x1)
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

//int intersection(Range path[], point envelope[], float Vground, char type) {
//    // Level: 'l'
//    // Descend: 'd'
//    // Imminent Terrain Impact: 'i'
//
//    if (type == 'l') {
//        for (int i = 0; i < CalculateLAT(Vground) + 1; i++) {
//            if (path[i].center.Z > envelope[1].Y || path[i].limit1.Z > envelope[1].Y || path[i].limit2.Z > envelope[1].Y) {
//                printf("WARNING!!! %d %f %f", i, envelope[1].Y, path[i].center.Z);
//                return 1;
//            }
//        }
//    }
//
//    if (type == 'd') {
//        // Calculate the A-B line equation: y − y1 = m(x − x1)
//        float slope = (envelope[0].Y - envelope[1].Y) / (envelope[0].X - envelope[1].X);
//
//        for (int i = 0; i < CalculateLAT(Vground) + 1; i++) {
//            if ((envelope[0].X + path[i].distance) < envelope[1].X) {  // Point between A & B
//                // Calculate the y (elevation) by plugging the x from every point in the A-B equation
//                // and then compare it to the y of the point on the path
//                float intersectionY = slope * ((envelope[0].X + path[i].distance) - envelope[1].X) + envelope[1].Y;
//                if (path[i].center.Z > intersectionY || path[i].limit1.Z > intersectionY || path[i].limit2.Z > intersectionY) {
//                    printf("WARNING!!! %d %f %f\n", i, path[i].distance, intersectionY);
//                    return 1;
//                }
//            } else {  // Point between B & C
//                if (path[i].center.Z > envelope[1].Y || path[i].limit1.Z > envelope[1].Y || path[i].limit2.Z > envelope[1].Y) {
//                    printf("WARNING!!! %d %f\n", i, path[i].distance);
//                    return 1;
//                }
//            }
//        }
//    }
//
//    return 0;  // No intersection found
//}



double xMeter(double lat,double lon) {
    double rad = PI/180.0;
    return lon*40075*(cos(lat*rad)/360);
}
double yMeter(double lat) {
    double rad = PI/180.0;
    return lat*(111.32);
}

double xMeterI(double lat,double lon) {
    double rad = PI/180.0;
    //return acos((lon*360)/(lat*40075))/rad;
	return (lon*360)/(40075*cos(lat*rad));
}
double yMeterI(double lat) {
    return lat/(111.32);
}

int LevelOrDescent(float VerticalSpeed)
{
    printf("%f\n",VerticalSpeed*60); // it is per second and we convert to feet per minute
    if (((VerticalSpeed*60)>-500)&&((VerticalSpeed*60)<500))
        return 0;//Level flight
    return 1; //Descent flight
}

void step(Pos position, float Vground, float VerticalSpeed, float RTC, float TrueTrack) {
    // Every second, do this

    // Get the plane position and transform it to our framework of km then meters
    position.lon = xMeter(position.lat, position.lon) * 1000.0;
    position.lat = yMeter(position.lat) * 1000.0;
    printf("%f\n", xMeterI(yMeterI(position.lat / 1000.0), position.lon / 1000.0));
    printf("%f\n", yMeterI(position.lat / 1000.0));

    // Calculate the flight path
    Range *path = TurningFlightPrediction(Vground, TrueTrack, position, 5.85, RTC);

    for (int i = 0; i < CalculateLAT(Vground) + 1; i++) {
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
    }

    // Generate the envelope
    point *envelope = LevelEnvelopGeneration(RTC, Vground, position);

    // Perform intersection check
    int result = intersection(path, envelope, Vground, 'l');  // Check for intersection with level envelope

    if (result == 1) {
        // Intersection detected, take appropriate action
        // ...
    }
}


/*Deprecated Code
 * float TurningFlightPrediction(Vned speed, float TrueTrack, Pos position, float YawRate, float PathPredSampleTime)
{
	double rad = PI/180.0; //multiply to convert to radians and divide to revert to degrees
	//change this to constant in the header
	double REarth=6371000.0; //meter
	//speed need to change
	double Vground=300.0;
	//Input: time to predict from look a head (Get it from the table)
	double time=30.0; //this should come from the look ahead calculate
	//Input: sample time
	double PredLat=0; //(2.29)
	double PredLon=0; //(2.29)
	double CumTrackAngle=TrueTrack*rad; //(2.29)
	double Alt=1000.0;
	double a=0; //Alpha angle of the turning path (arch or curve)
	//calculate new centre angle and update cumulated track angle
	double YawR=YawRate;
	for (int i=0;i<(int)time/PathPredSampleTime;i++)
	{

		a=CumTrackAngle-(90*rad); //(2.30)
		CumTrackAngle=CumTrackAngle+YawR*PathPredSampleTime;//(2.30)
		//Calculate the orthodrome segment
		double dn=(Vground*(-sin(a)))-(0.5*Vground*YawR*(-cos(a)));
		double de=(Vground*(cos(a)))-(0.5*Vground*YawR*(-sin(a)));
		double Dx=sqrt(pow (dn, 2.0)+pow (de, 2.0));
		puts(" ");
		//Calculate the new predicted position
		double x=Dx/(REarth+Alt); //(2.32)
		double y=atan(de/dn)/rad;//(2.33)
		//printf("Angles: %f, %f", a/rad,CumTrackAngle/rad);
		//printf("Vector: %f, %f", de,dn);
		//printf("Angles: %f, %f", x/rad,y/rad);
		//puts("");
		double oldLat=PredLat;
		double oldlon=PredLon;
		double PredLat=oldLat+asin( (sin(oldLat*rad)*cos(x*rad)) + (cos(oldLat*rad)*sin(x*rad)*cos(y*rad)))/rad;
		double PredLon=oldlon+atan((sin(y*rad)*sin(x*rad)*cos(oldLat*rad))/(cos(x*rad)-sin(oldLat*rad)*sin(PredLat*rad)))/rad;
		printf("%f", PredLat*1);
		printf(",");
		printf("%f", PredLon*1);
		printf(";");

	}
	puts("\n");
}
*/

