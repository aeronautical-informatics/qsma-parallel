#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "FLTA.h"

#ifndef DATA_H
#define DATA_H

typedef struct {
    int center;
    int limit1;
    int limit2;
} Points;


int flightEnvironment(void);
void getElevation(char* coordinates, char** results, int* resultsSize);
void getElev(const char* query, char** results, int* numResults);
void plot(char* query);
int PlotServer();
void getElevFile(point* data, Range path[], int pathSize,int data_number);
double toRadians(const double degree);
double getFGValue(const char *url);
void ElevDataInit(point *data, const char *file);
int str_length(char str[]);

char* get_line_from_buffer(char* buffer, int line_number);
int process_each_line(char* buffer,point *lines);
char** process_each_line2(char* buffer, int* num_lines);
void visit_every_elevation(char* buffer,point *lines);

#endif // DATA_H
