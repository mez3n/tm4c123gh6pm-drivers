#ifndef UART0_H_
#define UART0_H_

#include "std_types.h"



#define UART_DATA_5BITS          0x0
#define UART_DATA_6BITS          0x1
#define UART_DATA_7BITS          0x2
#define UART_DATA_8BITS          0x3
#define UART_LCRH_WLEN_BITS_POS  5
#define UART_CTL_UARTEN_MASK     0x00000001
#define UART_CTL_TXE_MASK        0x00000100
#define UART_CTL_RXE_MASK        0x00000200
#define UART_FR_TXFE_MASK        0x00000080
#define UART_FR_RXFE_MASK        0x00000010
#define UART_IM_RXIM_MASK        0x00000010
#define NVIC_PR1_UART_MASK       0x00001000  //give UART0 priority 1
#define NVIC_EN0_UART_MASK       0x00000020
/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

extern void UART0_Init(void);

extern void UART0_SendByte(uint8 data);

extern uint8 UART0_ReceiveByte(void);



#endif
