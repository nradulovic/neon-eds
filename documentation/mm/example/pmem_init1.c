#include <stdint.h>
#include "mem/mem.h"

struct dataBlock {                                          /* Some application data structure*/
    uint32_t data01;
    uint32_t data02;
    uint16_t command;
};

#define POOL_ELEMENTS                   10u                 /* Specification of pool */

int main(
    void) {

    static esPMemHandle_T myPool;                           /* myPool pool handle */
    static struct dataBlock poolStorage[POOL_ELEMENTS];     /* pool storage */

    esPMemInit(
        &myPool,
        &poolStorage,
        sizeof(poolStorage),
        sizeof(struct dataBlock));                          /* Initialize myPool pool memory */

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
