#ifndef NGPIO_H_
#define NGPIO_H_

#include <stdbool.h>
#include "plat/gpio.h"

#ifndef GPIO_PIN_ID
#define GPIO_PIN_ID(port, pin)          ((port) * PORT_PINS_PER_GPIO + (pin))
#endif

#define GPIO_INPUT                      (0x0ul << 0)
#define GPIO_OUTPUT                     (0x1ul << 0)
#define GPIO_OUTPUT_OPEN_DRAIN          (0x2ul << 0)
#define GPIO_PULL_UP                    (0x1ul << 2)
#define GPIO_PULL_DOWN                  (0x2ul << 2)

struct gpio_port;

struct gpio_pin_config 
{
    uint32_t                    flags;
};

struct gpio_pin
{
    struct gpio_port *          port;
    uint32_t                    pin_mask;
};

void gpio_driver_init(void);

void gpio_init(uint32_t pin_id, const struct gpio_pin_config * config);
void gpio_term(uint32_t pin_id);
bool gpio_get(uint32_t pin_id);
void gpio_set(uint32_t pin_id);
void gpio_clear(uint32_t pin_id);
bool gpio_request(uint32_t pin_id);
void gpio_release(uint32_t pin_id);
bool gpio_change_notice_request(uint32_t pin_id, void (* change_handler)(uint32_t pin_id));
bool gpio_change_notice_release(uint32_t pin_id);

#define /* NGPIO_H_ */
