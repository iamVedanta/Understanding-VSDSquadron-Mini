#ifndef gpio_h
#define gpio_h

#include <stdint.h>

void gpio_init(int pin);
void delay_ms(uint32_t ms);

void gpio_set(int pin);
void gpio_clear(int pin);

void uart_init(int baudrate);
void uart_print_string(const char *str);

#endif /* gpio_h */
