#include <stdint.h>
#include "mem/mem.h"

#define DATA_ELEMENTS                   200u                /* Number of blocks in data pool */
#define COMMAND_ELEMENTS                100u                /* Number of blocks in command pool */

struct dataElement {                                        /* A sample structure representing application data */
    uint32_t data01;
    uint32_t data02;
};

int main (
    void) {

    static esPMemHandle_T dataPool;
    static esPMemHandle_T commandPool;
    static struct dataElement dataStorage[DATA_ELEMENTS];   /* This will hold application data */
    static int commandStorage[COMMAND_ELEMENTS];            /* This will hold application commands, each command is one integer */

    esPMemInit(
        &dataPool,
        &dataStorage,
        sizeof(dataStorage),
        sizeof(struct dataElement));                        /* Initialize the pool memory for data */

    esPMemInit(
        &commandPool,
        &commandStorage,
        sizeof(commandStorage),
        sizeof(int));                                       /* Initialize the pool memory for commands */

    while (TRUE) {
        int * data;
        int * command;

        data = esPMemAllocI(
            &dataPool);                                     /* Allocate memory for data */

        command = esPMemAllocI(
            &commandPool);                                  /* Allocate memory for command */

        /*
         * Do some stuff
         */

        esPMemDeAllocI(
            &commandPool,
            command);                                       /* Delete the command */

        esPMemDeAllocI(
            &dataPool,
            data);                                          /* Delete the data */
    }
}
