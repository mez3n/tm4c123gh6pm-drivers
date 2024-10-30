/*
 * EEPROM.h
 *
 *  Created on: Sep 30, 2024
 *      Author: Mazen
 */

#ifndef FREE_RTOS_FILES_MCAL_EEPROM_EEPROM_H_
#define FREE_RTOS_FILES_MCAL_EEPROM_EEPROM_H_

volatile void (*EEPROM_Interrupt_CB)(void);
volatile void (*EEPROM_Wait_CB)(void);




void EEPROM_IntHandler();
void EEPROM_Init();
void EEPROM_Write(char* s);
void EEPROM_Read(char * Str);
void EEPROM_SetInterruptCB(volatile void (*PtrToFunc)(void));
void EEPROM_SetWaitCB(volatile void (*PtrToFunc)(void));
void concatStringToInt();
void concatStringToString();



#endif /* FREE_RTOS_FILES_MCAL_EEPROM_EEPROM_H_ */
