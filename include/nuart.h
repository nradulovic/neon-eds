#ifndef NUART_H_
#define NUART_H_

#include "plat/uart.h"

#define UART_DATA_BITS_8                (0x1u << 0)
#define UART_DATA_BITS_9                (0x2u << 0)
#define UART_STOP_BITS_1                (0x1u << 2)
#define UART_STOP_BITS_2                (0x2u << 2)
#define UART_STOP_BITS_1_5              (0x3u << 2)
#define UART_PARITY_NONE                (0x1u << 4)
#define UART_PARITY_ODD                 (0x2u << 4)
#define UART_PARITY_EVEN                (0x2u << 4)

#endif /* NUART_H_ */
