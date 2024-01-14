/*
 * scheduler.h
 *
 *  Created on: Nov 29, 2023
 *      Author: Mina
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "CortexMX_OS_porting.h"

typedef enum{

	NOERROR,
	Ready_QUEUE_init_Error,
	Task_exceeded_stackSize

}MYRTOS_ErrorID;

typedef struct{

	unsigned int Stack_Size;
	unsigned int priority;
	void (*P_TaskEntry)(void);
	unsigned char Autostart ;
	unsigned int _S_PSP_Task;  //NoT Entered by user
	unsigned int _E_PSP_Task;  //NoT Entered by user
	unsigned int* Current_PSP; //NoT Entered by user
	char TaskName[30];
	enum {
		suspend,
		Running,
		Waiting,
		Ready

	}TaskState;

	struct
	{
		enum{
			enable,
			disable

		}Blocking;

		unsigned int Ticks_Count;

	}TimingWaiting;


}Task_ref;

typedef enum
	{
		SVC_ActivateTask,
		SVC_TerminateTask,
		SVC_TaskWaitingTime

	}SVC_ID;


MYRTOS_ErrorID MYRTOS_init();
MYRTOS_ErrorID MYRTOS_CreatTask(Task_ref* tref); // should be in the header because the user will use it
void MYRTOS_ActivateTask(Task_ref* tref);
void MYRTOS_TerminateTask(Task_ref* tref);
void MYRTOS_STARTOS(void);
void MYRTOS_TaskWait(unsigned int NoTicks,Task_ref* SelfTref);


#endif /* INC_SCHEDULER_H_ */
