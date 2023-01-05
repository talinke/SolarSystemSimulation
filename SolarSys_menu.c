/***************************************************************************
 * SolarSys_menu.c
 * provides an application program interface for the solar system simulation
 * Tim Linke, talinke@ucdavis.edu
 * 12/22/2022
***************************************************************************/
#include "SolarSys.h"

int main() {

    int init = 0;

    daq = 0;
    duration = 300;
    Nobj = 5; // change this number based on how many bodies are in solar system

    action_t act;                               // action in the menu
    int quit = FALSE;                           // flag for quit or continue the menu

    while(quit==FALSE) {                        // continue the menu if not quit
        act = getActionMenu();                  // get an action
        switch(act) {

            case VISUALIZE:                     // visualize the flow field
                if (init == FALSE) initialize();
                daq = 0;
                displayOpen();
                plotData();
                sleep(2);
                for (int t = 0; t < duration; t++) {
                    runSim();
                    plotData();
                }
                displayClose();
                free(object);
		init = FALSE;
                break;

            case OBTAIN:
                if (init == FALSE) initialize();
		for (int i = 0; i < Nobj; i++) {
                	printf("[%d] - %s\t", i, (object+i)->name);
		}
		printf("\n");
		printf("Please enter the object number whose data is to be obtained: ");
                scanf("%d", &tracker);
                printf("Obtaining data for %s. ", (object+tracker)->name);
                estimateDuration();
                daq = 1;
                for (int t = 0; t < duration; t++) {
                    runSim();
                }
		printf("Results saved in 'Results.dat' file.\n");
                free(object);
		init = FALSE;
		break;

            case ADD:                         //change parameters interactively
                Nobj++;
                initialize();
                init = TRUE;
                addObject();
                break;

            case QUIT:                          // quit the program
                quit = TRUE;
                break;

            default:
                printf("\aInvalid input!\n");
                break;
        }
    }
    return 0;
}
