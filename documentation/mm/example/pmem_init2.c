#include <stddef.h>
#include "mem/mem.h"

struct dataBlock {                                          /* Some application data structure*/
    uint32_t data01;
    uint32_t data02;
    uint16_t command;
};

#define POOL_ELEMENTS                   10u                 /* Specification of pool */
#define POOL_SIZE                       (ES_BIT_ALIGN_UP(sizeof(struct dataBlock), sizeof(portReg_T)) * POOL_ELEMENTS)

int main(
    void) {

    static esPMemHandle_T myBigStorage;                     /* Static memory handle */
    static char myReallyBigStorage[16384];                  /* Static memory storage */
    static esPMemHandle_T myPool;                           /* myPool pool handle */
    static void * poolStorage;                              /* pointer to pool storage */

    esSMemInit(
        &myBigStorage,
        &myReallyBigStorage[0],
        sizeof(myReallyBigStorage));                        /* Initialize the static memory */

    poolStorage = esSMemAllocI(
        POOL_SIZE);                                         /* Allocate pool storage memory */
    esPMemInit(
        &myPool,
        poolStorage,
        POOL_SIZE,
        ES_BIT_ALIGN_UP(sizeof(struct dataBlock), sizeof(portReg_T))); /* Initialize myPool pool memory */

    while (TRUE) {
        struct dataBlock * data;

        data = esPMemAllocI(
            &myPool);                                       /* Allocate a block from pool memory */

        /*
         * Do something
         */

        esPMemDeAllocI(
            &myPool,
            data);                                          /* Return back the block */
    }
}
