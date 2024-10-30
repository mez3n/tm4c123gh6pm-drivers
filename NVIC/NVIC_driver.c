/*
 * Nvic_driver.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Mazen
 */
#include "NVIC_driver.h"



/**
 * @brief Gets the enable register address for the specified IRQ.
 * @param IRQ_Num The IRQ number.
 * @return Pointer to the enable register for the specified IRQ.
 */
static uint32* NVIC_GetEnReg(NVIC_IRQType IRQ_Num) {
    int reg = (((int)IRQ_Num) / 32);
    return (uint32*)((&NVIC_EN0_REG) + reg);
}

/**
 * @brief Gets the disable register address for the specified IRQ.
 * @param IRQ_Num The IRQ number.
 * @return Pointer to the disable register for the specified IRQ.
 */
static uint32* NVIC_GetDisReg(NVIC_IRQType IRQ_Num) {
    uint32 reg = (((uint32)IRQ_Num) / 32);
    return (uint32*)((&NVIC_DIS0_REG) + reg);
}

/**
 * @brief Gets the priority register address for the specified IRQ.
 * @param IRQ_Num The IRQ number.
 * @return Pointer to the priority register for the specified IRQ.
 */
static uint32* NVIC_GetPrReg(NVIC_IRQType IRQ_Num) {
    uint32 reg = (uint32)IRQ_Num / 4;
    uint32* base = (uint32*)&NVIC_PRI0_REG;
    return base + reg;
}

/**
 * @brief Gets the priority register address for the specified system exception.
 * @param IRQ_Num The IRQ number.
 * @return Pointer to the priority register for the specified system exception.
 */
static uint32* NVIC_GetExceptionPrReg(NVIC_IRQType IRQ_Num) {
    uint32 reg = (uint32)IRQ_Num / 3;
    uint32* base = (uint32*)&NVIC_SYSTEM_PRI1_REG;
    return base + reg;
}



/**
 * @brief Enables the specified IRQ.
 * @param IRQ_Num The IRQ number to enable.
 * @return None
 */
void NVIC_EnableIRQ(NVIC_IRQType IRQ_Num) {
    uint32* NVIC_En_Reg = NVIC_GetEnReg(IRQ_Num);
    int bit_pos = (((int)IRQ_Num) % 32);
    *NVIC_En_Reg = (1 << bit_pos);
}

/**
 * @brief Disables the specified IRQ.
 * @param IRQ_Num The IRQ number to disable.
 * @return None
 */
void NVIC_DisableIRQ(NVIC_IRQType IRQ_Num) {
    uint32* NVIC_Dis_Reg = NVIC_GetDisReg(IRQ_Num);
    int bit_pos = (((int)IRQ_Num) % 32);
    *NVIC_Dis_Reg = (1 << bit_pos);
}

/**
 * @brief Sets the priority of the specified IRQ.
 * @param IRQ_Num The IRQ number for which the priority is set.
 * @param IRQ_Priority The priority level to set for the IRQ.
 * @return None
 */
void NVIC_SetPriorityIRQ(NVIC_IRQType IRQ_Num, NVIC_IRQPriorityType IRQ_Priority) {
    uint32* NVIC_Pr_Reg = NVIC_GetPrReg(IRQ_Num);
    int bit_pos = 5 + 8 * (((int)IRQ_Num) % 4);
    *NVIC_Pr_Reg |= (IRQ_Priority << bit_pos);
}

/**
 * @brief Enables the specified system exception.
 * @param Exception_Num The system exception number to enable.
 * @return None
 */
void NVIC_EnableException(NVIC_ExceptionType Exception_Num) {
    NVIC_SYSTEM_SYSHNDCTRL |= (1 << ((int)Exception_Num + 13));
}

/**
 * @brief Disables the specified system exception.
 * @param Exception_Num The system exception number to disable.
 * @return None
 */
void NVIC_DisableException(NVIC_ExceptionType Exception_Num) {
    NVIC_SYSTEM_SYSHNDCTRL &= ~(1 << ((int)Exception_Num + 13));
}

/**
 * @brief Sets the priority of the specified system exception.
 * @param Exception_Num The system exception number for which the priority is set.
 * @param Exception_Priority The priority level to set for the system exception.
 * @return None
 */
void NVIC_SetPriorityException(NVIC_ExceptionType Exception_Num, NVIC_ExceptionPriorityType Exception_Priority) {
    switch (Exception_Num) {
        case EXCEPTION_MEM_FAULT_TYPE:
            NVIC_SYSTEM_PRI1_REG |= (Exception_Priority << MEM_FAULT_PRIORITY_BITS_POS);
            break;

        case EXCEPTION_BUS_FAULT_TYPE:
            NVIC_SYSTEM_PRI1_REG |= (Exception_Priority << BUS_FAULT_PRIORITY_BITS_POS);
            break;

        case EXCEPTION_USAGE_FAULT_TYPE:
            NVIC_SYSTEM_PRI1_REG |= (Exception_Priority << USAGE_FAULT_PRIORITY_BITS_POS);
            break;

        case EXCEPTION_SVC_TYPE:
            NVIC_SYSTEM_PRI2_REG |= (Exception_Priority << SVC_PRIORITY_BITS_POS);
            break;

        case EXCEPTION_DEBUG_MONITOR_TYPE:
            NVIC_SYSTEM_PRI3_REG |= (Exception_Priority << DEBUG_MONITOR_PRIORITY_BITS_POS);
            break;

        case EXCEPTION_PEND_SV_TYPE:
            NVIC_SYSTEM_PRI3_REG |= (Exception_Priority << PENDSV_PRIORITY_BITS_POS);
            break;

        case EXCEPTION_SYSTICK_TYPE:
            NVIC_SYSTEM_PRI3_REG |= (Exception_Priority << SVC_PRIORITY_BITS_POS);
            break;
    }
}


