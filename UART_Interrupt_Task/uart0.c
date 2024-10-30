
#include "uart0.h"
#include "tm4c123gh6pm_registers.h"




static void GPIO_SetupUART0Pins(void)
{
    SYSCTL_RCGCGPIO_REG  |= 0x01;
    while(!(SYSCTL_PRGPIO_REG & 0x01));
    GPIO_PORTA_AMSEL_REG &= 0xFC;
    GPIO_PORTA_DIR_REG   &= 0xFE;
    GPIO_PORTA_DIR_REG   |= 0x02;
    GPIO_PORTA_AFSEL_REG |= 0x03;
    GPIO_PORTA_PCTL_REG  = (GPIO_PORTA_PCTL_REG & 0xFFFFFF00) | 0x00000011;
    GPIO_PORTA_DEN_REG   |= 0x03;
}

void UART0_Init(void)
{

    GPIO_SetupUART0Pins();
    SYSCTL_RCGCUART_REG |= 0x01;
    while(!(SYSCTL_PRUART_REG & 0x01));
    UART0_CTL_REG = 0;
    UART0_CC_REG  = 0;
    /* To Configure UART0 with Baud Rate 9600 */
    UART0_IBRD_REG = 104;
    UART0_FBRD_REG = 11;

    /* UART Line Control Register Settings
     * BRK = 0 Normal Use
     * PEN = 0 Disable Parity
     * EPS = 0 No affect as the parity is disabled
     * STP2 = 0 1-stop bit at end of the frame
     * FEN = 0 FIFOs are disabled
     * WLEN = 0x3 8-bits data frame
     * SPS = 0 no stick parity
     */
    UART0_LCRH_REG = (UART_DATA_8BITS << UART_LCRH_WLEN_BITS_POS);

    /* UART Control Register Settings
     * RXE = 1 Enable UART Receive
     * TXE = 1 Enable UART Transmit
     * HSE = 0 The UART is clocked using the system clock divided by 16
     * UARTEN = 1 Enable UART
     */
    UART0_CTL_REG = UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK | UART_CTL_RXE_MASK;

    UART0_IM_REG|=UART_IM_RXIM_MASK ;
    NVIC_PRI1_REG=NVIC_PR1_UART_MASK ;
    NVIC_EN0_REG=NVIC_EN0_UART_MASK ;

}

uint8 UART0_ReceiveByte(void)
{
    return UART0_DR_REG&(0xFF);
}

void UART0_SendByte(uint8 data)
{
    while(!(UART0_FR_REG&UART_FR_TXFE_MASK));
    UART0_DR_REG=data ;
}




