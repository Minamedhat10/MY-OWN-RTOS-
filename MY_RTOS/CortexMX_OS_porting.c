/*
 * CortexMX_OS_porting.c
 *
 *  Created on: Nov 29, 2023
 *      Author: Mina
 */

#include "CortexMX_OS_porting.h"

unsigned char SystickLED;


void HardFault_Handler (void)
{
	while(1);
}
void	MemManage_Handler(void)
{
	while(1);

}
void	BusFault_Handler(void)
{
	while(1);

}
void	UsageFault_Handler(void)
{
	while(1);

}


__attribute ((naked)) void SVC_Handler() // to make this function not take size at stack

{

	__asm("tst lr,#4 \n\t"
			"ITE EQ \n\t"
			"mrseq r0,msp \n\t"
			"mrsne r0,psp \n\t"
			"B OS_SVC_services");

}

void HW_init()
{


	// initialize clock tree (RCC for cpu & timer ) distribute the clocking bet each one to meet in time line but here in m3 there is \
	systick inside cpu then it operate on 8MHZ

	//decrease pendsv interrupt priority to be smaller than or equal systimer
	//systick have a priority 14

	__NVIC_SetPriority(PendSV_IRQn,15);

}

void trigger_OS_PendSV()
{
	SCB->ICSR |=SCB_ICSR_PENDSVSET_Msk;

}


void start_ticker()
{
	//this clock tree should be defined in HW_init()
	// 8 MHZ
	// 1 count -> 0.125 us
	// x count -> 1ms
	// X =8000 count

	SysTick_Config(8000);
}

void SysTick_Handler(void)
{
	SystickLED ^=1;
	MYRTOS_Uptade_TasksWaitingTime(); // that is mean when sys time happened it will update any tasks on waiting mode
	// determine Pcurrent & Pnext

	Decide_WhatNext();
	// switch Context & Restore
	trigger_OS_PendSV();

}



