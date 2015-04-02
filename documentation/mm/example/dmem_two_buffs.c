#include "mem/mem.h"

#define DATA_BUFFER_SIZE                2048
#define COMMAND_BUFFER_SIZE             512

int main (
    void) {

    static esPMemHandle_T myBigStorage;
    static char myReallyBigStorage[16384];
    static esDMemHandle_T dataHeap;
    static esDMemHandle_T commandHeap;
    static char * dataBuffer;
    static char * commandBuffer;

    esSMemInit(
        &myBigStorage,
        &myReallyBigStorage[0],
        sizeof(myReallyBigStorage));                        /* Initialize the static memory */

    dataBuffer = esSMemAllocI(
        DATA_BUFFER_SIZE);                                  /* Create a data buffer of 2048 bytes */
    commandBuffer = esSMemAllocI(
        COMMAND_BUFFER_SIZE);                               /* Create additional buffer of 512 bytes */

    esDMemInit(
        &dataHeap,
        dataBuffer,
        DATA_BUFFER_SIZE);                                  /* Initialize the dynamic memory for data */

    esDMemInit(
        &commandHeap,
        commandBuffer,
        COMMAND_BUFFER_SIZE);                               /* Initialize the dynamic memory for commands */

    while (TRUE) {
        int * data;
        int * command;

        data = esDMemAllocI(
            &dataHeap,
            sizeof(int) * 10u);                             /* Allocate an array of 10 integers */

        command = esDMemAllocI(
            &commandHeap,
            sizeof(int));

        /*
         * Do some stuff
         */

        esDMemDeAllocI(
            &commandHeap,
            command);                                       /* Delete the command */

        esDMemDeAllocI(
            &dataHeap,
            data);                                          /* Delete the data */
    }
}
