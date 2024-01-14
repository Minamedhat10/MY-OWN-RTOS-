/*
 * scheduler.c
 *
 *  Created on: Nov 29, 2023
 *      Author: Mina
 */


#include "scheduler.h"
#include "MYRTOS._FIFO.h"


FIFO_Buf_t Ready_QUEUE;
Task_ref* Ready_QUEUE_FIFO[100];
Task_ref IDLE_Task; 				//define structure of Task_ref type

struct {

	Task_ref* OSTasks[100];
	unsigned int _S_MSP_Task;
	unsigned int _E_MSP_Task;
	unsigned int PSP_Task_locator;
	unsigned int No_Of_Active_Tasks;
	Task_ref* CurrentTask;
	Task_ref* NextTask;
	enum{
		OSsuspend,
		OSrunning

	}OSMode_ID;



}OS_Control; // it is the one structure with one variable of structure

void bubble_Sort()

{
	unsigned int i ,j,n ;
	Task_ref* temp;
	n=OS_Control.No_Of_Active_Tasks;

	for(i=0	; i<n -1 ; i++)

		// last i elements are ready in place

		for(j=0 ; j<n - i - 1 ; j++)

			if(OS_Control.OSTasks[j]->priority>OS_Control.OSTasks[j+1]->priority)
			{
				temp=OS_Control.OSTasks[j]; // that is mean to equal pointer to another pointer
				OS_Control.OSTasks[j]=OS_Control.OSTasks[j+1];
				OS_Control.OSTasks[j+1]=temp;
			}




}

void MyRTOS_Update_Schedule_Tables()
{

	Task_ref* temp=NULL ;
	Task_ref* Ptask ;
	Task_ref* PnextTask ;

	int i=0;
	//Bubble sort scheduler for tasks to arrange the priority in sch-table OS.control->OStasks[100]
	bubble_Sort();

	// free ready Queue
	while(FIFO_dequeue(&Ready_QUEUE,&temp /*pointer to pointer */)!=FIFO_EMPTY);

	// update ready Queue (which is high in scheduler and put it in queue)
	while( i < OS_Control.No_Of_Active_Tasks )
	{
		Ptask=OS_Control.OSTasks[i];
		PnextTask=OS_Control.OSTasks[i+1];

		if (Ptask->TaskState!=suspend)
		{

			// in case we reached to the end of available OS_STACKS
			if (PnextTask->TaskState==suspend)
			{
				FIFO_enqueue(&Ready_QUEUE, Ptask);
				Ptask->TaskState=Ready;
				break;
			}

			// if the Ptask priority > nexttask then (lowest number is meaning higher priority)
			if (Ptask->priority < PnextTask->priority)
			{
				FIFO_enqueue(&Ready_QUEUE, Ptask);
				Ptask->TaskState=Ready;
				break;

			}

			else if (Ptask->priority == PnextTask->priority)
			{

				// if the Ptask priority == nexttask then
				// push ptask to ready state
				// and make the ptask = nexttask and nexttask ++

				FIFO_enqueue(&Ready_QUEUE, Ptask);
				Ptask->TaskState=Ready;

			}
			else if (Ptask->priority > PnextTask->priority)
			{
				//not allowed to happen as we already recorded it by bubble sort
				break;
			}

		}

		i++;
	}

}

//Handler
void Decide_WhatNext()

{
	// if ready Queue is empty && OS_Control -> current task -> !=0  because it may be the current task at running mode and not in queue then u can't run the idle task
	if (Ready_QUEUE.counter ==0 && OS_Control.CurrentTask->TaskState !=suspend) //Fifo _Empty
	{

		OS_Control.CurrentTask-> TaskState= Running;
		// add current task again at Queue to make (round robin)
		FIFO_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
		OS_Control.NextTask= OS_Control.CurrentTask;

	}

	else
	{
		FIFO_dequeue(&Ready_QUEUE,&OS_Control.NextTask);
		OS_Control.NextTask->TaskState=Running;

		// update ready Queue (to keep round robin algo.happen)
		if ((OS_Control.CurrentTask->priority==OS_Control.NextTask->priority)&&(OS_Control.CurrentTask->TaskState!=suspend))
		{
			FIFO_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
			OS_Control.CurrentTask->TaskState = Ready;

		}
	}
}

__attribute ((naked)) void PendSV_Handler()
{
	//==================================
	// save context of current task
	//==================================

	//Get the Current Task "Current PSP from CPU Register" as cpu pushed xpsr,pc,.........,R0
	OS_GET_PSP(OS_Control.CurrentTask->Current_PSP);

	//using the PSP (as a pointer ) to store (r4 to r11)
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r4 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r5 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r6 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r7 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r8 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r9 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r10 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0,r11 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	//save the current value of new PSP
	//already saved in current_PSP



	//========================
	// Restore context of Next task
	//========================
	//

	OS_Control.CurrentTask=OS_Control.NextTask; // that u has stacked it before in activation process with ( MYRTOS_Stack_for_Task ) function
	OS_Control.NextTask=NULL;


	__asm volatile("mov r11,%0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r10,%0 " : : "r" (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r9,%0 " : : "r"  (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r8,%0 " : : "r"  (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r7,%0 " : : "r"  (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r6,%0 " : : "r"  (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r5,%0 " : : "r"  (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile("mov r5,%0 " : : "r"  (*(OS_Control.CurrentTask->Current_PSP)) );
	OS_Control.CurrentTask->Current_PSP++;

	//update PSP and exit set the value of PSP in the
	OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);
	__asm volatile ("BX LR");



}


///Handler Mode
void OS_SVC_services (int* StackFramePointer)

{
	//OS_SVC_Set stack is r0,r1,r2,r3,r12,lr,pc,xpsr

	unsigned char svc_number;
	svc_number= *((unsigned char*)((unsigned char*)StackFramePointer[6]-2));
	switch (svc_number)

	{
	case SVC_ActivateTask: // it is mean 0 which beside the svc instruction
	case SVC_TerminateTask: // it is mean 1 which beside the svc instruction

		//update scheduler table, Ready Queue

		MyRTOS_Update_Schedule_Tables();

		//OS is in Running state
		if (OS_Control.OSMode_ID==OSrunning)
		{
			if(strcmp(OS_Control.CurrentTask->TaskName,"idleTask")!=0)
			{

				//Decide what next
				Decide_WhatNext();

				//trigger OS_PendSV (SW Context/Restore)
				trigger_OS_PendSV();
			}
		}

		break;

	case SVC_TaskWaitingTime:

		MyRTOS_Update_Schedule_Tables();
		break;
	}

}

unsigned char IdleTaskLED;

void MYRTOS_Operation()
{

	while(1)
	{
		IdleTaskLED ^=1;
		__asm("wfe");
	}

}


void MYRTOS_CREAT_MAIN_STACK()
{
	OS_Control._S_MSP_Task = &_estack;
	OS_Control._E_MSP_Task	= OS_Control._S_MSP_Task-MainStackSize ; //MainStackSize is sized by own soc

	// Aligned 8 bytes spaces between Main Task and PSP tasks

	OS_Control.PSP_Task_locator = (OS_Control._E_MSP_Task-8);




}

MYRTOS_ErrorID MYRTOS_init()

{
	MYRTOS_ErrorID error = NOERROR;

	//update OS Mode (OS_Suspend)

	OS_Control.OSMode_ID= OSsuspend;

	// specify the Main stack of OS
	MYRTOS_CREAT_MAIN_STACK();

	// Create OS Ready Queue
	if (FIFO_Init(&Ready_QUEUE,Ready_QUEUE_FIFO, 100)!=FIFO_NO_ERROR)
	{
		error+=Ready_QUEUE_init_Error;
	}

	// Configure IDLE Task
	strcpy(IDLE_Task.TaskName,"idleTask");
	IDLE_Task.priority=255;
	IDLE_Task.P_TaskEntry=MYRTOS_Operation;
	IDLE_Task.Stack_Size=300;

	error+=MYRTOS_CreatTask(&IDLE_Task); // (if there is error the function MYRTOS_CreatTask will return error with +1 above error which is eq 0 )


	return error;
}



void MYRTOS_Stack_for_Task(Task_ref* tref)
{
	/* Task frame
	 * =======
	 * xpsr
	 * pc (next instruction should be run )
	 * LR (return register which stacked before branch to handler)
	 * r12
	 * r3
	 * r2
	 * r1
	 * r0
	 * ======
	 * r4,r5,r6,r7,r7,r8,r9,r10,r11 (saved/restored) Manually
	 */

	tref->Current_PSP =tref ->_S_PSP_Task;

	tref->Current_PSP--;
	*(tref->Current_PSP) =0x01000000;		 // XPSR saving then we decrement 4 byte by pointer to can put PC

	tref->Current_PSP--;
	*(tref->Current_PSP)=(unsigned int) tref->P_TaskEntry; 	// PC  (u need to know more about thAT )

	tref->Current_PSP--;
	*(tref->Current_PSP) = 0xFFFFFFFD; 		//	LR which stored in stack that refer to that the task before handler stacked LR which refer to previous task

	for (int j=0 ;j<13;	j++)
	{
		tref->Current_PSP--;
		*(tref->Current_PSP)=0;
	}

}


MYRTOS_ErrorID MYRTOS_CreatTask(Task_ref* tref)
{

	MYRTOS_ErrorID error= NOERROR;

	// create it's own psp stack
	tref->_S_PSP_Task= OS_Control.PSP_Task_locator;
	tref->_E_PSP_Task= tref->_S_PSP_Task - tref->Stack_Size;

	// check task stack size exceed the psp stack
	if (tref->_E_PSP_Task < (unsigned int)(&(_end)))
	{
		return Task_exceeded_stackSize;
	}
	// aligned 8 bytes spaces between task PSP and other
	OS_Control.PSP_Task_locator=(tref->_E_PSP_Task-8);

	// Initialize PSP task stack (the concept is for store the remaining general purpose registers like r4 to r11 which are not stacking when handling happens )
	MYRTOS_Stack_for_Task(tref);

	// update sch table
	//tref->TimingWaiting.Blocking=disable;
	OS_Control.OSTasks[OS_Control.No_Of_Active_Tasks]=tref;
	OS_Control.No_Of_Active_Tasks++;
	// task state update ->	suspend
	tref->TaskState=suspend;


	return error;
}



void MYRTOS_OS_SVC_Set(SVC_ID ID)

{
	switch (ID)
	{

	case SVC_ActivateTask:
		__asm("SVC #0x00");
		break;

	case SVC_TerminateTask:
		__asm("SVC #0x01");

		break;

	case SVC_TaskWaitingTime:
		__asm("SVC #0x02");

		break;

	}

}

void MYRTOS_ActivateTask(Task_ref* tref)
{
	tref->TaskState=Waiting;
	MYRTOS_OS_SVC_Set(SVC_ActivateTask);
}



void MYRTOS_TerminateTask(Task_ref* tref)

{
	tref->TaskState=suspend;
	MYRTOS_OS_SVC_Set(SVC_TerminateTask);


}

void MYRTOS_TaskWait(unsigned int NoTicks,Task_ref* SelfTref)
{
	//Task should be blocked
	SelfTref->TimingWaiting.Blocking= enable; // that is mean that the time is enable and count
	SelfTref->TimingWaiting.Ticks_Count=NoTicks;
	//it should be suspend
	SelfTref->TaskState=suspend;
	MYRTOS_OS_SVC_Set(SVC_TerminateTask); // terminate task that is mean it is suspend and blocking because it should to be terminated to be in suspend mode
	// and it will be treated as an activated that is mean it will be scheduled in update schedule tasks function



}


void MYRTOS_STARTOS()
{
	OS_Control.OSMode_ID=OSrunning;

	//set Default "Current Task" = Idle Task
	OS_Control.CurrentTask=&IDLE_Task;

	//Activate IDLE Task
	MYRTOS_ActivateTask(&IDLE_Task);

	//start the ticker
	start_ticker();

	OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);

	//switch thread mode sp from MSP to PSP
	OS_SHADOW_SP_TO_PSP;
	OS_SWITCH_to_unprivileged;
	IDLE_Task.P_TaskEntry();
}



void MYRTOS_Uptade_TasksWaitingTime()
{
	for (int i=0; i<OS_Control.No_Of_Active_Tasks;i++)
	{
		if(OS_Control.OSTasks[i]->TaskState==suspend) // it is blocking until meet the time
		{
			if (OS_Control.OSTasks[i]->TimingWaiting.Blocking==enable)
			{

				OS_Control.OSTasks[i]->TimingWaiting.Ticks_Count--;
				if (OS_Control.OSTasks[i]->TimingWaiting.Ticks_Count==1)
				{
					OS_Control.OSTasks[i]->TimingWaiting.Blocking=disable; // the time out and and the time waiting will be disable
					OS_Control.OSTasks[i]->TaskState=Waiting;				  // then the task should be in waiting state to can be scheduled that is important to can be ready in QUEUE
					MYRTOS_OS_SVC_Set(SVC_TaskWaitingTime);
				}

			}

		}
	}

}

