/************************************************************
 * SolarSys_func.c
 * contains functions for Solar System Simulation Solver
 * Tim Linke, talinke@ucdavis.edu
 * 12/22/2022
************************************************************/
#include "SolarSys.h"

#define G 6.6743015e-11


clock_t t;                                                                          //duration estimate

//variables needed across functions
int        i, j, k;
double     dt, dx, dy, dz;
double     time_taken;


//Files for writing & reading
FILE *gnuplot_fd;

char *simdat = "Vis.dat";
char simfile[BUF1];
FILE *f_sim;

char *resdat = "Results.dat";
char resfile[BUF1];
FILE *f_res;



/*-----------------------------------------------------------------
 * getActionMenu() gets the action call from user.
 *-----------------------------------------------------------------*/
action_t getActionMenu(void) {
    action_t act;                                                                   // action to be taken
    const char *prompt = "SolarSys> ";                                              // prompt for the menu
    char str[128];                                                                  // user input

    //print main menu
    printf("\n");
    printf("------------------------------------------------------------------\n");
    printf("Simulate the solar system! \n");
    printf("------------------------------------------------------------------\n");
    printf("v, visualize: watch the objects take their path\n");
    printf("o, obtain:    print out data for a specific object\n");
    printf("a, add:       add new object to solar system\n");
    printf("q, quit:      quit the program\n");
    printf("------------------------------------------------------------------\n\n");

    fflush(stdin);
    printf("%s", prompt);
    fgets(str,128,stdin);                                                           // get the user input
    str[strlen(str)-1] = '\0';                                                      // get rid of 'enter'

    //select action based on user input
    if(!strcmp(str, "v") || !strcmp(str, "visualize")) {
        act = VISUALIZE;
    }
    else if(!strcmp(str, "o") || !strcmp(str, "obtain")) {
        act = OBTAIN;
    }
    else if(!strcmp(str, "a") || !strcmp(str, "add")) {
        act = ADD;
    }
    else if(!strcmp(str, "q") || !strcmp(str, "quit")) {
        act = QUIT;
    }
    else {
        act = UNDEFINED;
    }
    return act;
}

/*---------------------------------------------------------------------
 * initialize() sets up simulation by defining positions and velocities
 *---------------------------------------------------------------------*/
void initialize() {

    object = (Body *)malloc(Nobj * sizeof(Body));                                 //dynamic memory allocation, as huge number of bodies can be expected
    dx = 0;
    dy = 0;
    dz = 0;

    //-------------------------------
    // The following data is taken from the NASA planetary fact sheet
    // https://nssdc.gsfc.nasa.gov/planetary/factsheet/
    //-------------------------------

    //Color Palette from GNUPLOT
    // 1: purple
    // 2: green
    // 3: cyan
    // 4: orange
    // 5: yellow
    // 6: dark blue
    // 7: red
    // 8: black


    //Sun
    //-------
    strcpy((object+0)->name, "Sun");
    //Position
    (object+0)->r.x   = 0;
    (object+0)->r.y   = 0;
    (object+0)->r.z   = 0;
    //Velocity
    (object+0)->v.x   = 0;
    (object+0)->v.y   = 0;
    (object+0)->v.z   = 0;
    //Mass
    (object+0)->m     = 1.988e30;
    //Plotting
    (object+0)->psize = 5; //pointsize
    (object+0)->color = 5; //point color
    //-------

    //Mercury
    //-------
    strcpy((object+1)->name, "Mercury");
    //Position
    (object+1)->r.x   = 57.9e9;
    (object+1)->r.y   = 0;
    (object+1)->r.z   = 0;
    //Velocity
    (object+1)->v.x   = 0;
    (object+1)->v.y   = 2*M_PI*(object+1)->r.x / (24*3600*88);
    (object+1)->v.z   = 0;
    //Mass
    (object+1)->m     = 3.3022e23;
    //Plotting
    (object+1)->psize = 1;
    (object+1)->color = 8;
    //-------


    //Venus
    //-------
    strcpy((object+2)->name, "Venus");
    //Position
    (object+2)->r.x   = 108.2e9;
    (object+2)->r.y   = 0;
    (object+2)->r.z   = 0;
    //Velocity
    (object+2)->v.x   = 0;
    (object+2)->v.y   = 2*M_PI*(object+2)->r.x / (24*3600*224.7);
    (object+2)->v.z   = 0;
    //Mass
    (object+2)->m     = 4.868e24;
    //Pointsize
    (object+2)->psize = 1.5;
    (object+2)->color = 4;
    //-------


    //Earth
    //-------
    strcpy((object+3)->name, "Earth");
    //Position
    (object+3)->r.x   = 149.6e9;
    (object+3)->r.y   = 0;
    (object+3)->r.z   = 0;
    //Velocity
    (object+3)->v.x   = 0;
    (object+3)->v.y   = 2*M_PI*(object+3)->r.x / (24*3600*365.2);
    (object+3)->v.z   = 0;
    //Mass
    (object+3)->m     = 5.9722e24;
    //Pointsize
    (object+3)->psize = 1.5;
    (object+3)->color = 3;
    //-------


    //Mars
    //-------
    strcpy((object+4)->name, "Mars");
    //Position
    (object+4)->r.x   = 228e9;
    (object+4)->r.y   = 0;
    (object+4)->r.z   = 0;
    //Velocity
    (object+4)->v.x   = 0;
    (object+4)->v.y   = 2*M_PI*(object+4)->r.x / (24*3600*687);
    (object+4)->v.z   = 0;
    //Mass
    (object+4)->m     = 6.4169e23;
    //Pointsize
    (object+4)->psize = 1.5;
    (object+4)->color = 7;
    //-------

    /*
    //Jupiter
    //-------
    strcpy((object+5)->name, "Jupiter");
    //Position
    (object+5)->r.x   = 778.5e9;
    (object+5)->r.y   = 0;
    (object+5)->r.z   = 0;
    //Velocity
    (object+5)->v.x   = 0;
    (object+5)->v.y   = 2*M_PI*(object+5)->r.x / (24*3600*4331);
    (object+5)->v.z   = 0;
    //Mass
    (object+5)->m     = 1898e24;
    //Pointsize
    (object+5)->psize = 2.5;
    (object+5)->color = 1;
    //-------


    //Saturn
    //-------
    strcpy((object+6)->name, "Saturn");
    //Position
    (object+6)->r.x   = 1432e9;
    (object+6)->r.y   = 0;
    (object+6)->r.z   = 0;
    //Velocity
    (object+6)->v.x   = 0;
    (object+6)->v.y   = 2*M_PI*(object+6)->r.x / (24*3600*10747);
    (object+6)->v.y   = (object+6)->v.y / 10747;
    (object+6)->v.z   = 0;
    //Mass
    (object+6)->m     = 568e24;
    //Pointsize
    (object+6)->psize = 3;
    (object+6)->color = 1;
    //-------

    //Uranus
    //-------
    strcpy((object+7)->name, "Uranus");
    //Position
    (object+7)->r.x   = 2867e9;
    (object+7)->r.y   = 0;
    (object+7)->r.z   = 0;
    //Velocity
    (object+7)->v.x   = 0;
    (object+7)->v.y   = 2*M_PI*(object+7)->r.x / (24*3600);
    (object+7)->v.y   = (object+7)->v.y / 30589;
    (object+7)->v.z   = 0;
    //Mass
    (object+7)->m     = 86.811e24;
    //Pointsize
    (object+7)->psize = 2;
    (object+7)->color = 1;
    //-------

    //Neptune
    //-------
    strcpy((object+8)->name, "Neptune");
    //Position
    (object+8)->r.x   = 4515e9;
    (object+8)->r.y   = 0;
    (object+8)->r.z   = 0;
    //Velocity
    (object+8)->v.x   = 0;
    (object+8)->v.y   = 2*M_PI*(object+8)->r.x / (24*3600);
    (object+8)->v.y   = (object+8)->v.y / 59800;
    (object+8)->v.z   = 0;
    //Mass
    (object+8)->m     = 102.4e24;
    //Pointsize
    (object+8)->psize = 1;
    (object+8)->color = 1;
    //-------

    //Pluto
    //-------
    strcpy((object+9)->name, "Pluto");
    //Position
    (object+9)->r.x   = 5906.4e9;
    (object+9)->r.y   = 0;
    (object+9)->r.z   = 0;
    //Velocity
    (object+9)->v.x   = 0;
    (object+9)->v.y   = 2*M_PI*(object+9)->r.x / (24*3600);
    (object+9)->v.y   = (object+9)->v.y / 90560;
    (object+9)->v.z   = 0;
    //Mass
    (object+9)->m     = 0.01303e24;
    //Pointsize
    (object+9)->psize = 1;
    (object+9)->color = 1;
    //-------

    */
}

/*-----------------------------------------------------------------
 * estimateDuration() gives the user an educated guess about
 * how long the simulation will take
 *-----------------------------------------------------------------*/
void estimateDuration() {
    t = clock();                                                                //start timer
    sprintf(simfile, "%s", simdat);
    if ((f_sim = fopen(simfile, "w")) == NULL) {
        fprintf(stdout, "Can't open Visualization file\n");
        fflush(stdout);
        exit(1);
    }

    for (j = 1; j < Nobj; j++) {                                                //run a single for-loop
        dx = (object + j)->r.x - (object)->r.x;
        dy = (object + j)->r.y - (object)->r.y;
        dz = (object + j)->r.z - (object)->r.z;

        //update velocities
        (object)->v.x += dt * dx * Fg(0, j) / (object)->m;
        (object)->v.y += dt * dy * Fg(0, j) / (object)->m;
        (object)->v.z += dt * dz * Fg(0, j) / (object)->m;

        fprintf(f_sim, "%Le %Le %Le %f %d\n", (object + i)->r.x, (object + i)->r.y, (object + i)->r.z, (object + i)->psize, (object + i)->color);
        fflush(f_sim);
    }
    t = clock() - t;                                                            //obtain duration
    time_taken = ((double)t)/CLOCKS_PER_SEC;                                    //convert to seconds
    printf("Estimated duration of simulation is %.1e seconds.", time_taken*Nobj*duration);   //obtain estimate
    fclose(f_sim);
}

/*-----------------------------------------------------------------
 * runSim() runs the n-body simulation. It chooses
 * between printing  position values to a file for visualization
 * (daq=0) or obtaining physically relevant data (daq=1)
 *-----------------------------------------------------------------*/
void runSim() {

    //open file for printing data
    sprintf(simfile, "%s", simdat);
    if ((f_sim = fopen(simfile, "w")) == NULL) {
        fprintf(stdout, "Can't open Visualization file\n");
        fflush(stdout);
        exit(1);
    }

    sprintf(resfile, "%s", resdat);
    if ((f_res = fopen(resfile, "a+")) == NULL) {
        fprintf(stdout, "Can't open Results.dat file\n");
        fflush(stdout);
        exit(1);
    }

    //dt = 1e-3;
    dt = 100000;

    //object interactions to obtain new velocities
    for (i = 0; i < Nobj; i++) {
        for (j = 0; j < Nobj; j++) {
            if (i != j) {
                //calculate positions
                dx = (object + j)->r.x - (object + i)->r.x;
                dy = (object + j)->r.y - (object + i)->r.y;
                dz = (object + j)->r.z - (object + i)->r.z;

                //update velocities
                (object + i)->v.x += dt * dx * Fg(i, j) / (object + i)->m;
                (object + i)->v.y += dt * dy * Fg(i, j) / (object + i)->m;
                (object + i)->v.z += dt * dz * Fg(i, j) / (object + i)->m;
            }
        }
    }

    //update position vectors
    for (i = 0; i < Nobj; i++) {
        //printf("V of object %d is x= %Le, y= %Le, z= %Le in m/s \n", i, (object + i)->v.x, (object + i)->v.y, (object + i)->v.z);
        (object + i)->r.x += dt * (object + i)->v.x;
        (object + i)->r.y += dt * (object + i)->v.y;
        (object + i)->r.z += dt * (object + i)->v.z;

        //visualize
        if (daq==0) {
            //printf("%Le\n", (object + i)->r.x);
            //fprintf(f_sim, "%Le %Le %Le\n", (object + i)->r.x, (object + i)->r.y, (object + i)->r.z);        //print positions to visualize
            fprintf(f_sim, "%Le %Le %Le %f %d\n", (object + i)->r.x, (object + i)->r.y, (object + i)->r.z, (object + i)->psize, (object + i)->color);
            fflush(f_sim);
        }
        if (daq==1 && i == tracker) {
            fprintf(f_res, "%Le %Le %Le\n", (object + i)->r.x, (object + i)->r.y, (object + i)->r.z);        //print positions to file
            fflush(f_res);
        }
    }
    fclose(f_sim);
    fclose(f_res);
}

/*--------------------------------------------------------------------
 * Fg() calculates the gravitational force magnitude between two bodies
 *--------------------------------------------------------------------*/
double Fg(int i, int j) {
    double F;
    F = G * (object + i)->m * (object + j)->m / pow( sqrt(dx*dx + dy*dy + dz+dz) , 3); //functionality verified. No overflow.
    return F;
}

/*-----------------------------------------------------------------
 * addObject() allows the user to add an object into the solar system
 *-----------------------------------------------------------------*/
void addObject(){
    printf("Let's add a new body to our solar system. Please enter the prompted information in SI units (m, m/s, kg). Information must be relative to Sun.\n");
    printf("Name: ");
    fgets((object+Nobj-1)->name, sizeof((object+Nobj-1)->name), stdin);
    (object+Nobj-1)->name[strlen((object+Nobj-1)->name)-1] = '\0';
    printf("Initial x position: ");
    scanf("%Le", &((object+Nobj-1)->r.x));
    printf("Initial y position: ");
    scanf("%Le", &((object+Nobj-1)->r.y));
    printf("Initial z position: ");
    scanf("%Le", &((object+Nobj-1)->r.z));
    printf("Initial x velocity: ");
    scanf("%Le", &((object+Nobj-1)->v.x));
    printf("Initial y velocity: ");
    scanf("%Le", &((object+Nobj-1)->v.y));
    printf("Initial z velocity: ");
    scanf("%Le", &((object+Nobj-1)->v.z));
    printf("Mass: ");
    scanf("%le", &((object+Nobj-1)->m));

    (object+Nobj-1)->color = 8;
    (object+Nobj-1)->psize = 0.5;

    printf("New object added. %s has initial position (%.1Le,%.1Le,%.1Le) and velocity (%.1Le,%.1Le,%.1Le). Its mass is %.1e.\n",
           (object+Nobj-1)->name, (object+Nobj-1)->r.x, (object+Nobj-1)->r.y, (object+Nobj-1)->r.z, (object+Nobj-1)->v.x, (object+Nobj-1)->v.y, (object+Nobj-1)->v.z, (object+Nobj-1)->m);
}

/*-----------------------------------------------------------------
 *-------------------------- Plotting -----------------------------
 *-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
 * displayOpen() opens up a pipe to an external plotting program
 * called gnuplot. The script allows for real-time plotting of
 * the obtained data, a non-trivial task in C. The pipe is
 * maintained open until displayClose() is called.
 *-----------------------------------------------------------------*/
void displayOpen()
{

    if ( ( gnuplot_fd = popen( "gnuplot -p", "w" ) ) == NULL )
    {
        fprintf( stderr, "Error opening pipe to gnuplot\n" );
        exit(1);
    }
    fprintf( gnuplot_fd, "set title \'A N I M A T I O N\'\n" );
    fprintf( gnuplot_fd, "unset key\n" );
    //fprintf( gnuplot_fd, "set pointsize variable\n" );
    fprintf( gnuplot_fd, "set xrange [-300000000000:300000000000]\n");
    fprintf( gnuplot_fd, "set yrange [-300000000000:300000000000]\n");
    //fprintf( gnuplot_fd, "set xrange [-10000000000000:10000000000000]\n");
    //fprintf( gnuplot_fd, "set yrange [-10000000000000:10000000000000]\n");
    fprintf( gnuplot_fd, "set zrange [-10000:10000]\n");
}

/*-----------------------------------------------------------------
 * plotData() plots the data that is created. It allows for real-
 * time plotting through piping to gnuplot.
 *-----------------------------------------------------------------*/
void plotData() {
    //fprintf( gnuplot_fd, "splot \'%s\' using 1:2:3 with points pointtype 7\n", simdat);
    fprintf( gnuplot_fd, "splot \'%s\' using 1:2:3:4:5 with points pointtype 7 pointsize variable lc variable\n", simdat);
    fflush( gnuplot_fd );

    struct timespec tim, tim2;
    tim.tv_sec = 0;                                 //sleep in seconds
    tim.tv_nsec = 50000000;                         //sleep in nanoseconds
    //tim.tv_nsec = 0;

    nanosleep(&tim , &tim2);                        //introduce artificial pause to observe changes between plots
}

/*-----------------------------------------------------------------
 * displayClose() closes the pipe to gnuplot
 *-----------------------------------------------------------------*/
void displayClose()
{
    fprintf( gnuplot_fd, "exit\n" );
    pclose( gnuplot_fd );
}

