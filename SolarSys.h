/************************************************************
* SolarSys.h
* contains macros, data structures and function prototypes
* for the Solar System Simulation library
* Tim Linke, talinke@ucdavis.edu
* 12/22/2022
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define BUF1 1024



// enumerated type for actions from the menu driven user input
typedef enum Action {
    UNDEFINED,  // undefined action
    VISUALIZE,  // visualize flow field
    OBTAIN,     // obtain an object's data
    ADD,        // change simulation parameters interactively
    QUIT        // quit
} action_t;

typedef struct {
    long double x, y, z;
} Vec;

typedef struct {
    Vec r, v;
    double m;
    char name[32];
    double psize;
    int color;
} Body;

int Nobj;
int daq;
int duration;
int tracker;

Body *object;

extern action_t getActionMenu(void);

extern void initialize();
extern void estimateDuration();
extern void runSim();
extern double Fg(int i, int j);
extern void addObject();
extern void plotData();
extern void displayOpen();
extern void displayClose();
