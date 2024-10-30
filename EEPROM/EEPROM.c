/*
 * EEPROM.c
 *
 *  Created on: Sep 30, 2024
 *      Author: Mazen
 */

#include "NVIC_driver.h"
#include "EEPROM.h"
#include "AppCfg.h"
#define EEPROM_IRQ_NUM  (NVIC_IRQType)29

uint8 Writing = 0;

// a custom function don't use unless you understand it
static uint8 STR_concatenate_sizeOrEOC(char *destination, const char *source, uint8 size) {
    while (*destination != '\0') {
        destination++;
    }

    while (*source != '\0' && size!=0) {
        *destination = *source;
        destination++;
        source++;
        size--;
    }

    *destination = '\0';

    if(size==0 && *(source-1) != '\0')
    {
        return 1;
    }else{
        return 0;
    }
}

static uint8 STR_Len(char* s){
    char * temp=s;
    while (*temp != '\0') {
        temp++;
    }
    return (uint8) (temp-s);
}



void concatStringToInt(char* str, uint32 num) {
    while (*str) {
        str++;
    }
    char temp1[256]="\0";
    char * temp=(char *)temp1;
    while(num>0){
        *(temp)=num%10 +'0';
        temp++;
        num/=10;
    }
    temp--;
    while(*temp){
        *str=*temp;
        str++;
        temp--;
    }
    *str = '\0';
}


void concatStringToString(char* str1, char* str2) {
    while (*str1) {
        str1++;
    }
    while(*str2){
       *str1=*str2;
       str1++;
       str2++;
    }
    *str1= '\0';
}


void EEPROM_Init(){
    SYSCTL_RCGCEEPROM_REG |=0x01;
    while(!(SYSCTL_PREEPROM_REG  & 0x01));
    FLASH_FCIM_REG|=0x04;
    EEPROM_EEINT_R=0x01;
    while((EEPROM_EEDONE_R  & 0x01));
    EEPROM_EEBLOCK_R=0;
    EEPROM_EEOFFSET_R=0;
    NVIC_EnableIRQ(EEPROM_IRQ_NUM);
    NVIC_SetPriorityIRQ(EEPROM_IRQ_NUM, EEPROM_IRQ_PRIORITY);

}

void EEPROM_IntHandler(volatile void (*PtrToFunc)(void)){
    FLASH_FCMISC_REG |= 0x04;
    if((EEPROM_Interrupt_CB!=NULL_PTR)&&Writing){
        EEPROM_Interrupt_CB();
    }

}

void EEPROM_SetInterruptCB(volatile void (*PtrToFunc)(void)){
    EEPROM_Interrupt_CB=PtrToFunc;
}
void EEPROM_SetWaitCB(volatile void (*PtrToFunc)(void)){
    EEPROM_Wait_CB=PtrToFunc;
}

void EEPROM_Write(char* s){
    Writing = 1;
    uint8 i = (STR_Len(s)/4) +1;
    uint8 curr_fourChar=0;
    while(curr_fourChar<i){
        EEPROM_EERDWRINC_R=*( ((uint32*)(s))+curr_fourChar);
        curr_fourChar++;
        EEPROM_Wait_CB();
        if(EEPROM_EEOFFSET_R==0){
            EEPROM_EEBLOCK_R++;
        }
    }
    Writing = 0;
}



void EEPROM_Read(char * Str){
    Str[0]='\0';
    uint32 four_char=0;
    do{
        four_char=EEPROM_EERDWRINC_R;
        uint8 a,b,c,d;
        a=four_char;
        b=four_char>>8;
        c=four_char>>16;
        d=four_char>>24;
        if(EEPROM_EEOFFSET_R==0){
            EEPROM_EEBLOCK_R++;
        }

    }while(STR_concatenate_sizeOrEOC(Str, (const char *)&four_char , 4));
}
