#include "mem/mem.h"

#define BUFFER_SIZE                     1024

int main (
    void) {

    static char buffer[BUFFER_SIZE];
    static esDMemHandle_T myHeap;

    esDMemInit(
        &myHeap,
        &buffer[0],
        sizeof(buffer));                                    /* Initialize the dynamic memory */

    while (TRUE) {
        int * myArray;

        myArray = esDMemAllocI(
            &myHeap,
            sizeof(int) * 10u);                             /* Allocate an array of 10 integers */

        /*
         * Do some stuff
         */

        esDMemDeAllocI(
            &myHeap,
            myArray);                                       /* Delete the array */
    }
}
