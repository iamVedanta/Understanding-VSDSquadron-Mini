This is first time I am coming accross a code like this.
I tried my best to understand the steps which are required to run a code, power a pin, use it as an output pin, how to configure functions, how to use the clock present in the Microcontroller to write the delay method

I tried to implement how to enable the PIN for UART output. First we have to Start the clock, this will enable the Peripherals of the registers.
Although there is only one Main clock, we have to start the clock separetely for each peripheral- GPIO, USART etc as the clock distribution system manages this.

After the clock is enabled for gpio, we need to configure the pin mode. Here we have configured to push-pull mode whic allows us to toggle between HIGH or LOW using the Bit Set/Reset Resigter which directly toggles the pin voltage.

For UART output, we have to enable the USART peripheral clock(PD5) and configure the pin connected to TX using alternate function mode. We then set the baud rate number to USART_BRR register. And then we enable the transmitter and the USART module

We created delay by using NOP(No Operation). It is a standard assembly instruction that literally tells the cpu to "DO NOTHING for one Clock Cycle"

GPIO Pin Chosen
Pin: PD6 - datasheet shows that it is mapped to LED onboard

Pin: PD5- USART clock - USART1_TX

How to build and flash?

pio run
pio run -t upload
pio device monitor -b 115200

or

Just use the tick mark on the bottom of the VS code to compile
right arrow symbol - for upload
plug - for serial terminal
