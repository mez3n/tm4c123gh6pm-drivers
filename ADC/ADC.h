/*
 * ADC.h
 *
 *  Created on: Sep 22, 2024
 *      Author: Mazen
 */

#include "tm4c123gh6pm_registers.h"
#include "AppCfg.h"

#ifndef ADC_H_
#define ADC_H_

volatile void (*ADC0_CallBack_Ptr)(void);
volatile void (*ADC1_CallBack_Ptr)(void);

void AIN0_Init();
void ADC0_Init();
uint16 ADC0_Read_SS3();
void ADC0_RequestRead();
void ADC0_SetCallBack(volatile void (*PtrToFunc)(void));
void ADC0_IntHandler();

void AIN1_Init();
void ADC1_Init();
uint16 ADC1_Read_SS3();
void ADC1_RequestRead();
void ADC1_SetCallBack(volatile void (*PtrToFunc)(void));
void ADC1_IntHandler();






#endif /* ADC_H_ */
