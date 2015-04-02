#include "mem/mem.h"

#define BUFFER_SIZE                     1024

int main (
    void) {

    static esPMemHandle_T myBigStorage;
    static char myReallyBigStorage[16384];
    static esDMemHandle_T myHeap;
    static char * buffer;

    esSMemInit(
        &myBigStorage,
        &myReallyBigStorage[0],
        sizeof(myReallyBigStorage));                        /* Initialize the static memory */

    buffer = esSMemAllocI(
        BUFFER_SIZE);                                       /* Allocate a buffer of 1024 bytes from static memory*/

    esDMemInit(
        &myHeap,
        buffer,
        BUFFER_SIZE);                                       /* Initialize the dynamic memory */

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
