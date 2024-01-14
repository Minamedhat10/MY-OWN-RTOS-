/*
 * CortexMX_OS_porting.h
 *
 *  Created on: Nov 29, 2023
 *      Author: Mina
 */



#ifndef INC_CORTEXMX_OS_PORTING_H_
#define INC_CORTEXMX_OS_PORTING_H_


#include "core_cm3.h"

extern int _estack;
extern int _end;

#define MainStackSize 3072

#define OS_SET_PSP(add)  		__asm volatile("mov r0,%0 \n\t msr psp,r0" : :"r" (add) )
#define OS_GET_PSP(add)  		__asm volatile("mrs r0,psp \n\t mov %0,r0" :"=r" (add) )


#define OS_SHADOW_SP_TO_MSP 	__asm volatile("mrs r1,CONTROL \n\t Mvn r0,#0x02 \n\t and r1,r1,r0 \n\t msr CONTROL,r1")
#define OS_SHADOW_SP_TO_PSP 	__asm volatile("mrs r1,CONTROL \n\t ORR r1,r1,#0x02 \n\t msr CONTROL,r1")



// clear bin 0 in control register
#define OS_SWITCH_to_privileged     __asm("mrs r3, CONTROL \n\t" \
										"lsr r3,r3,#0x1 \n\t" \
										"lsl r3,r3,#0x1 \n\t" \
										"msr CONTROL, r3 ")
// set bin 0 in control register

#define OS_SWITCH_to_unprivileged 	__asm("mrs r3, CONTROL \n\t" \
											"orr r3,r3,#0x1 \n\t" \
											"msr CONTROL, r3")
void trigger_OS_PendSV();
void start_ticker();
void MYRTOS_Uptade_TasksWaitingTime();
void Decide_WhatNext();
#endif /* INC_CORTEXMX_OS_PORTING_H_ */
