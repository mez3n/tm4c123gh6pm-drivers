/*
 * ADC.c
 *
 *  Created on: Sep 22, 2024
 *      Author: Mazen
 */
#include "ADC.h"
#include "NVIC_driver.h"
#define ADC0_SS3_INTERRUPT_NUMBER     (NVIC_IRQType)17
#define ADC1_SS3_INTERRUPT_NUMBER     (NVIC_IRQType)51
void AIN0_Init()
{
    SYSCTL_RCGCGPIO_REG |= 0x10;
    while(!(SYSCTL_PRGPIO_REG & 0x10));

    GPIO_PORTE_AMSEL_REG |= (1<<3);       /*  Enable Analog on PE3 */
    GPIO_PORTE_PCTL_REG  &= 0xFFFF0FFF;   /* Clear PMCx bits for PF4 to use it as GPIO pin */
    GPIO_PORTE_DIR_REG   &= ~(1<<3);      /* Configure PE3 as input pin */
    GPIO_PORTE_AFSEL_REG &= ~(1<<3);      /* Disable alternative function on PE3 */
    GPIO_PORTE_PUR_REG   |= (1<<3);       /* Enable pull-up on PE3 */
    GPIO_PORTE_DEN_REG   &= ~(1<<3);      /* Disable Digital I/O on PE3 */
}


void AIN1_Init()
{
    SYSCTL_RCGCGPIO_REG |= 0x10;
    while(!(SYSCTL_PRGPIO_REG & 0x10));

    GPIO_PORTE_AMSEL_REG |= (1<<2);       /*  Enable Analog on PE2 */
    GPIO_PORTE_PCTL_REG  &= 0xFFFFF0FF;   /* Clear PMCx bits for PE2 to use it as GPIO pin */
    GPIO_PORTE_DIR_REG   &= ~(1<<2);      /* Configure PE2 as input pin */
    GPIO_PORTE_AFSEL_REG &= ~(1<<2);      /* Disable alternative function on PE2 */
    GPIO_PORTE_PUR_REG   |= (1<<2);       /* Enable pull-up on PE2 */
    GPIO_PORTE_DEN_REG   &= ~(1<<2);      /* Disable Digital I/O on PE2 */
}


void ADC0_Init(){
    SYSCTL_RCGCADC_REG |= 0x01;
    while(!(SYSCTL_PRADC_REG  & 0x01));

    ADC0_ACTSS_R &= ~(1<<3); //disable ss3
    ADC0_IM_R |= 1<<3;    //enbale interrupt for ss3
    ADC0_EMUX_R &= ~(0xF000); //make the adc input a gpio pin
    ADC0_SSMUX3_R=0; //make the adc input PE3
    ADC0_SSCTL3_R |= 6<<0;
    NVIC_EnableIRQ(ADC0_SS3_INTERRUPT_NUMBER);
    NVIC_SetPriorityIRQ(ADC0_SS3_INTERRUPT_NUMBER, ADC0_INT_PRIORITY);
    ADC0_PC_R =0x1;
    ADC0_ACTSS_R |=(1<<3); //enable ss3
}

void ADC1_Init(){
    SYSCTL_RCGCADC_REG |= 0x02;
    while(!(SYSCTL_PRADC_REG  & 0x02));

    ADC1_ACTSS_R &= ~(1<<3); //disable ss3
    ADC1_IM_R |= 1<<3;    //enbale interrupt for ss3
    ADC1_EMUX_R &= ~(0xF000); //make the adc input a gpio pin
    ADC1_SSMUX3_R=1; //make the adc input PE2
    ADC1_SSCTL3_R |= 6<<0;
    NVIC_EnableIRQ(ADC1_SS3_INTERRUPT_NUMBER);
    NVIC_SetPriorityIRQ(ADC1_SS3_INTERRUPT_NUMBER, ADC1_INT_PRIORITY);
    ADC1_PC_R =0x1;
    ADC1_ACTSS_R |=(1<<3); //enable ss3
}




uint16 ADC0_Read_SS3(){
    return (uint16) (ADC0_SSFIFO3_R &0x00000FFF);
}

void ADC0_RequestRead(){
    ADC0_PSSI_R |=1<<3;   // start sampling in ss3
}

void ADC0_SetCallBack(volatile void (*PtrToFunc)(void)){
    ADC0_CallBack_Ptr=PtrToFunc;
}

void ADC0_IntHandler(){
    ADC0_ISC_R |= 1<<3;
    if(ADC0_CallBack_Ptr!=NULL_PTR){
        ADC0_CallBack_Ptr();
    }
}


uint16 ADC1_Read_SS3(){
    return (uint16) (ADC1_SSFIFO3_R &0x00000FFF);
}

void ADC1_RequestRead(){
    ADC1_PSSI_R |=1<<3;   // start sampling in ss3
}

void ADC1_SetCallBack(volatile void (*PtrToFunc)(void)){
    ADC1_CallBack_Ptr=PtrToFunc;
}

void ADC1_IntHandler(){
    ADC1_ISC_R |= 1<<3;
    if(ADC1_CallBack_Ptr!=NULL_PTR){
        ADC1_CallBack_Ptr();
    }
}
