#include "gpio.h"
#include <ch32v00x.h>



// The "Power Switch" for peripherals
#define RCC_APB2PCENR (*(volatile uint32_t *)0x40021018)

// Port D Configuration (The "Mode" selector)
#define GPIOD_CFGLR (*(volatile uint32_t *)0x40011400)

// Port D Bit Set/Reset (The "ON/OFF" switch)
#define GPIOD_BSHR (*(volatile uint32_t *)0x40011410)

#define USART1_BASE 0x40013800
#define USART1_SR (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1 (*(volatile uint32_t *)(USART1_BASE + 0x0C))

static void uart_send_char(char c)
{
    // What will happen if we write to USART1_DR without waiting for TXE?
    // we would lose the data present in the DR if it is not sent to the shift register
    // Data corruption / overwrite is the real risk.
    // Never write to USART_DR unless TXE = 1
    while (!(USART1_SR & (1 << 7)))
        ;

    USART1_DR = c;
}

void uart_print_string(const char *str)
{
    // while (*str)
    // {
    //     uart_send_char(*str++);
    // }
    while (*str != '\0')
    {
        uart_send_char(*str);
        str++;
    }
}

// Why static? - Limits scope to this file
// Prevents accidental use elsewhere

void uart_init(int baudrate)
{
    // Enable clock for USART1 (bit 14 in RCC_APB2PCENR)
    // Configure UART pins
    // Set the baud rate - USART1_BRR (baud rate Number= FCPU clock ÷ baud rate)
    // Enable USART -  USART itself, Transmitter, Receiver

    /* Enable GPIO D clock */
    RCC_APB2PCENR |= (1 << 5);

    /* Enable USART1 clock */
    RCC_APB2PCENR |= (1 << 14);

    //     At this moment:
    // GPIO hardware is alive
    // USART hardware is alive
    // Pins are still unconfigured
    // UART is still disabled

    // the board has ONE main clock source -24 MHz internal
    // Then why do we “enable clocks” for GPIO, UART, etc.?
    //  Answer: Because the MCU has a clock distribution system.
    //  The single clock is gated, divided, and routed to different hardware blocks.

    // Which physical pin on your board carries USART1_TX?
    // Answer: Pin D5
    // How does PD5 become “USART1_TX”? -  Pin Multiplexing
    // Each pin can have multiple functions. The function is selected by configuring the pin mode.

    GPIOD_CFGLR &= ~(0xF << (5 * 4)); // Clear PD5 config

    GPIOD_CFGLR |= (0xB << (5 * 4));
    // PD5 is electrically connected to USART1 TX hardware

    // 24MHz clock
    USART1_BRR = 24000000 / baudrate;

    // Enable Transmitter and USART
    USART1_CR1 |= (1 << 3);  // TE: Transmitter enable
    USART1_CR1 |= (1 << 13); // UE: USART enable
}

void gpio_init(int pin)
{
    // First we are waking up the clock for PORT D
    RCC_APB2PCENR |= (1 << 5);

    // Clear the existeing setting or configuration
    // this is called BIT MASKING - mask of four 1s (0xf or 1111 in binary)
    // Since each pin uses 4 bits, the starting bit for any pin is simply pin * 4. For Pin 6, that's bit 24.

    // 0x -  this is just the prefix for hexadecimal numbers
    // 0xf = 1111 - the number 15
    GPIOD_CFGLR &= ~(0xf << (pin * 4)); // Clear existing configuration

    // we need to write our new mode: Push-Pull Output. On the CH32V003, the value for this is 0x1
    GPIOD_CFGLR |= (0x1 << (pin * 4));
}

void gpio_set(int pin)
{
    // To turn on the pin, we write a 1 to the corresponding bit in the BSHR register
    // Bit Set/Reset Register (BSHR)
    // This is write only  register- we need not read it first to change one bit
    GPIOD_BSHR = (1 << pin);
}

void gpio_clear(int pin)
{
    GPIOD_BSHR = (1 << (pin + 16));
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 8000; i++)
        __asm__("nop");
}


// int main(void)
// {
//     uart_init(9600);

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

// Which two clocks must be enabled for UART to work on your board?
// GPIO peripheral clock and USART peripheral clock must be enabled.

// Clock ON
//  ├── GPIO peripheral
//  │     └── Pins become usable
//  └── USART peripheral
//        └── UART communication works
