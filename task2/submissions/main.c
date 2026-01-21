#include "gpio.h"

int main(void)
{
    gpio_init(6);

    while (1)
    {
        gpio_set(6);
        delay_ms(500);
        gpio_clear(6);
        delay_ms(500);
    }
}

// int main(void)
// {
//     uart_init(115200);

//     uart_print_string("Hello Vedanta! UART Initialized\r\n");

//     int counter = 0;

//     // while true - doesnt work in C, so we use while(1)
//     while (1)
//     {

//         //\r - Carriage Return (CR)
//         //\r → go to start of line
//         // printf("Counter value: %d\r\n", counter); --Printf Doesnt work

//         char buffer[30];
//         itoa(counter, buffer, 10);
//         uart_print_string("Counter value: ");
//         uart_print_string(buffer);
//         uart_print_string("\r\n");
//         counter++;
//         delay_ms(1000);
//     }
// }
