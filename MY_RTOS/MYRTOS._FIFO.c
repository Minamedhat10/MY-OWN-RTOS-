/*
 * MYRTOS._FIFO.c
 *
 *  Created on: Nov 29, 2023
 *      Author: Mina
 */

#include "MYRTOS._FIFO.h"


Buffer_status FIFO_Init(FIFO_Buf_t* fifo , element_type* buff , unsigned int length)
{
	if(!buff)
		return FIFO_NULL ;


	fifo->base = buff;
	fifo->head = fifo->base;
	fifo->tail = fifo->base;
	fifo->length = length;
	fifo->counter = 0 ;

	return FIFO_NO_ERROR;
}

Buffer_status FIFO_enqueue(FIFO_Buf_t* fifo , element_type item)
{
	if(!fifo->base || !fifo->length )
		return FIFO_NULL;


	if ((fifo->head == fifo->tail)&&(fifo->counter == fifo->length))
		return FIFO_FULL;

	*(fifo->tail) = item;
	fifo->counter ++;
	//circular fifo


	if(fifo->tail == (((unsigned int)fifo->base + (4*fifo->length))-4))

		fifo->tail = fifo->base ;

	else
		fifo->tail++;

	return FIFO_NO_ERROR ;

}

Buffer_status FIFO_dequeue(FIFO_Buf_t* fifo , element_type* item)
{
	if(!fifo->base || !fifo->length )
			return FIFO_NULL;

	//check fifo is empty
	if (fifo->head == fifo->tail)
			return FIFO_EMPTY;


	*item = *(fifo->head) ;
	fifo->counter--;

	//circular fifo

	if(fifo->head == (((unsigned int)fifo->base + (4*fifo->length))-4))
		fifo->head = fifo->base ;

	else
		fifo->head++;

	return FIFO_NO_ERROR ;

}
Buffer_status FIFO_IS_FULL(FIFO_Buf_t* fifo)
{
	if(!fifo->head || !fifo->base || !fifo->tail)
		return FIFO_NULL;

	if(fifo->counter == fifo->length)
		return FIFO_NULL;

	return FIFO_NO_ERROR ;
}
