
#include "Data.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include <sys/types.h>


#include <errno.h>

#include <math.h>

#include <string.h>

#define READ   0
#define WRITE  1

#define PI 3.14159265
#define HeightAboveNstAirport 140


typedef struct {
    int id;
    float lon;
    float lat;
}Elevation;


double toRadians(const double degree)
{
    // cmath library in C++
    // defines the constant
    // M_PI as the value of
    // pi accurate to 1e-30
    double one_deg = (PI) / 180;
    return (one_deg * degree);
}

double distance(double lat1, double long1,
                     double lat2, double long2)
{
    // Convert the latitudes
    // and longitudes
    // from degree to radians.
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);

    // Haversine Formula
    double dlong = long2 - long1;
    double dlat = lat2 - lat1;

    double ans = pow(sin(dlat / 2), 2) +
        cos(lat1) * cos(lat2) *
            pow(sin(dlong / 2), 2);

    ans = 2 * asin(sqrt(ans));

    // Radius of Earth in
    // Kilometers, R = 6371
    // Use R = 3956 for miles
    double R = 6371;

    // Calculate the result
    ans = ans * R;

    return ans;
}



void ElevDataInit(point *data, const char *file) {

    FILE *demFile = fopen(file, "r");
    if (demFile == NULL) {
        perror("Error opening file");
        return;
    }

    char line[100]; // Adjust the buffer size as needed
    int index = 0;

    while (fgets(line, sizeof(line), demFile)) {
        float lonData, latData;
        int elevation;

        sscanf(line, "%f %f %d", &lonData, &latData, &elevation);

        (data)[index].X = lonData;
        (data)[index].Y = latData;
        (data)[index].Z = (int)elevation;
        index++;
        //printf("%f %f %d\n",(data)[index].X,(data)[index].Y,(data)[index].Z);
    }

    printf("%d\n", index);

    fclose(demFile);
}


void getElevFile(point *data,Range path[],int pathSize,int data_number)
{


	float comp_res=0.0004; //ideally 0.0002
	int rangSeeked=pathSize*5000;
    Range coords[100];
    Points PointsFound[100];
    for (int i = 0; i < pathSize + 1; i++) {

        PointsFound[i].center=0;
        PointsFound[i].limit1=0;
        PointsFound[i].limit2=0;

        //Marking the unfounded coordinates
        path[i].center.Z=0.0;
        path[i].limit1.Z=0.0;
        path[i].limit2.Z=0.0;

        //Inverting the conversion of the coordinates to the original geo coordinates
        coords[i].center.X=xMeterI(yMeterI(path[i].center.Y / 1000.0), path[i].center.X / 1000.0);

        coords[i].center.Y=yMeterI(path[i].center.Y / 1000.0);

        coords[i].limit1.X=xMeterI(yMeterI(path[i].limit1.Y / 1000.0), path[i].limit1.X / 1000.0);

        coords[i].limit1.Y=yMeterI(path[i].limit1.Y / 1000.0);

        coords[i].limit2.X= xMeterI(yMeterI(path[i].limit2.Y / 1000.0), path[i].limit2.X / 1000.0);

        coords[i].limit2.Y=yMeterI(path[i].limit2.Y / 1000.0);
    }

    //Finding the middle point of the path in the elivation data
    int low = 0;
	int high = data_number - 1;
	int middle=0;
	while (low <= high) {
		int mid = (low + high) / 2;
		//printf("The middle %d between %d-%d , and pathSize is %d and the coordinates is %f. Elevation at mid %f \n", mid, low,high,(int)pathSize/2,coords[(int)pathSize/2].center.Y,data[mid].Y);
		if (fabs(coords[(int)pathSize/2].center.Y-data[mid].Y)<=0.0002) {
			middle=mid;  // Element found
			break;
		} else if (coords[pathSize/2].center.Y-data[mid].Y<0) {
			low = mid + 1;  // Search in the right half
		} else {
			high = mid - 1;  // Search in the left half
		}
	}
	//printf("The middle %d", middle);

    clock_t start_time = clock();  // Mark the start time

    for (int i=middle-rangSeeked;i<=middle+rangSeeked;i++)
	{
		for (int j = 0; j < pathSize + 1; j++) {

			if ((!PointsFound[j].center)&&(fabs(coords[j].center.Y-data[i].Y)<=comp_res)&&(fabs(coords[j].center.X-data[i].X)<=comp_res)) //The Y(lat) matched go on to match the X
			{
				//cout<<"results "<<coords[j].center.Y-data[i].Y<<"\n";
				path[j].center.Z=data[i].Z;
				PointsFound[j].center=1;
				//break;
			}
			if ((!PointsFound[j].limit1)&&(fabs(coords[j].limit1.Y-data[i].Y)<=comp_res)&&(fabs(coords[j].limit1.X-data[i].X)<=comp_res)) //The Y(lat) matched go on to match the X
			{
				path[j].limit1.Z=data[i].Z;
				PointsFound[j].limit1=1;
				//break;
			}
			if ((!PointsFound[j].limit2)&&(fabs(coords[j].limit2.Y-data[i].Y)<=comp_res)&&(fabs(coords[j].limit2.X-data[i].X)<=comp_res)) //The Y(lat) matched go on to match the X
			{
				path[j].limit2.Z=data[i].Z;
				PointsFound[j].limit2=1;
				//break;
			}
		}
	}




    clock_t end_time = clock();  // Mark the end time
	double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	printf("\nTime spent on cycling thru the data: %.6f seconds\n", time_spent);

    for (int j = 0; j < pathSize + 1; j++)
    {
        printf("%f %f %f\n", coords[j].center.X, coords[j].center.Y, path[j].center.Z);
        printf("%f %f %f\n", coords[j].limit1.X, coords[j].limit1.Y, path[j].limit1.Z);
        printf("%f %f %f\n", coords[j].limit2.X, coords[j].limit2.Y, path[j].limit2.Z);
    }

}


//int PlotServer(){
//
//	/////////////////////////////////////////////////////////////////
//    int sock = 0,client_fd;
//    struct sockaddr_in serv_addr;
//
//    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//        printf("\n Socket creation error \n");
//        return -1;
//    }
//
//    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_port = htons(5001);
//
//    // Convert IPv4 and IPv6 addresses from text to binary
//    // form
//    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
//        <= 0) {
//        printf(
//            "\nInvalid address/ Address not supported \n");
//        return -1;
//    }
//
//    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
//        printf("\nConnection Failed \n");
//        return -1;
//    }
//    else
//        return sock;
//	return 0;
//}

int str_length(char str[]) {
    // initializing count variable (stores the length of the string)
    int count;

    // incrementing the count till the end of the string
    for (count = 0; str[count] != '\0'; ++count);

    // returning the character count of the string
    return count;
}

char* get_line_from_buffer(char* buffer, int line_number) {
    int current_line = 0;
    char* line_start = buffer;
    char* ptr = buffer;

    while (*ptr != '\0') {
        if (*ptr == '\n') {
            current_line++;

            if (current_line == line_number) {
                // Found the desired line
                int len = ptr - line_start + 1;
                char* line = (char*)malloc(len + 1);  // +1 for the null terminator
                if (line == NULL) {
                    perror("Failed to allocate memory for line");
                    return NULL;
                }
                strncpy(line, line_start, len);
                line[len] = '\0';
                return line;
            }

            // Set line_start to the start of the next line
            line_start = ptr + 1;
        }

        ptr++;
    }

    // If the desired line number was not found
    return NULL;
}

int process_each_line(char* buffer,point *lines) {
    char *line_start = buffer, *ptr = buffer+42;
    int line_number = 0;
    char *temp_line[50];
    clock_t start_time = clock();  // Mark the start time
    float lonData, latData;
   	int elevation;
   	int line_length;
    while (*ptr != '\0') {
        if (*ptr == '\n' || *(ptr + 1) == '\0') {  // Check for newline or end of buffer

            line_number++;
            line_length = ptr - line_start;

            // Print or process the line (for this example, we'll print it)
//            printf("Line %d: ", line_number);
//            fwrite(line_start, sizeof(char), line_length, stdout);
//            printf("\n");

            // Store the line in a temporary variable
            //printf("Allocating memory for line of size: %d\n", line_length + 1);

			strncpy(temp_line, line_start, line_length);
			temp_line[line_length] = '\0';


			sscanf(temp_line, "%f %f %d", &lonData, &latData, &elevation);

			(lines)[line_number].X = lonData;
			(lines)[line_number].Y = latData;
			(lines)[line_number].Z = (int)elevation;


            // Move to the start of the next line
            line_start = ptr + 1;
            ptr=ptr+160;
        }
        ptr++;
    }

    clock_t end_time = clock();  // Mark the end time

    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nTime spent processing lines: %.6f seconds with %d lines\n", time_spent,line_number);

    return line_number;
}

void visit_every_elevation(char* buffer,point *lines) {
    clock_t start_time = clock();  // Mark the start time

    //process


    clock_t end_time = clock();  // Mark the end time

    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nTime spent processing lines: %.6f seconds \n", time_spent);
}

char** process_each_line2(char* buffer, int* num_lines) {
    char *line_start = buffer, *ptr = buffer;
    char** lines = NULL;
    *num_lines = 0;

    clock_t start_time = clock();  // Mark the start time

    while (*ptr) {
        if (*ptr == '\n' || *(ptr + 1) == '\0') {
            int line_length = ptr - line_start;

            char* line = (char*)malloc(line_length + 1);
            if(!line) {
                perror("Failed to allocate memory for a line");
                exit(EXIT_FAILURE);
            }

            strncpy(line, line_start, line_length);
            line[line_length] = '\0';

            (*num_lines)++;
            char** temp = (char**)realloc(lines, (*num_lines) * sizeof(char*));
            if(!temp) {
                perror("Failed to reallocate memory for lines");
                exit(EXIT_FAILURE);
            }
            lines = temp;
            lines[(*num_lines) - 1] = line;

            line_start = ptr + 1;
        }
        ptr++;
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Number of lines: %d\n", *num_lines);
    printf("Time spent processing lines: %.6f seconds\n", time_spent);

    return lines;
}

/*
double getDesRate()
{
    int status=0;
    double h1=0;
    double t1=0;
    std::string url1="http://localhost:5480/json/position/altitude-ft";
    std::string url2="http://localhost:5480/json/sim/time/elapsed-sec";
    fgget fgdata;
    status=fgdata.fggetparam(url1);
    if(status==1)
        std::cout<<"Error Getting Data";
    else
    {
        h1=fgdata.value;
        status=fgdata.fggetparam(url2);
        t1=fgdata.value;
        status=fgdata.fggetparam(url1);
        h1=h1-fgdata.value;
        status=fgdata.fggetparam(url2);
        t1=t1-fgdata.value;
    }
//	cout<<"Data status:"<<status<<endl;
//	cout<<"Data values\nName:"<<fgdata.nameString<<"\nValue:"<<fgdata.value<<endl;
    return h1/t1;
}
 double getTCRate()
{
    int status=0;
    double h1=0;
    double t1=0;
    std::string url1="http://localhost:5480/json/position/altitude-agl-ft";
    std::string url2="http://localhost:5480/json/sim/time/elapsed-sec";
    fgget fgdata;
    status=fgdata.fggetparam(url1);
    if(status==1)
        std::cout<<"Error Getting Data";
    else
    {
        h1=fgdata.value;
        status=fgdata.fggetparam(url2);
        t1=fgdata.value;
        status=fgdata.fggetparam(url1);
        h1=h1-fgdata.value;
        status=fgdata.fggetparam(url2);
        t1=t1-fgdata.value;
    }
//	cout<<"Data status:"<<status<<endl;
//	cout<<"Data values\nName:"<<fgdata.nameString<<"\nValue:"<<fgdata.value<<endl;
    return h1/t1;
}
 */
