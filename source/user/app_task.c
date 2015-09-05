#include "os.h"
#include "hal_driver.h"

#include "usart.h"
#include "fcmd.h"

#include "app_task.h"


/******************************************************************************
*  任务1
*/
void task1_active(uint8_t e);
void task1_set(uint8_t e);
void task1_on(uint8_t e);

void task1_active(uint8_t e)
{
	switch (e)
	{
	case A1_EVT:
		os_stm_tran(task1_set);
		break;

	case STM_ENTRY_SIG:
		dputs("2015-9-2\n");
		dputs("1-active:entry\n");
		break;

	case STM_EXIT_SIG:
		dputs("1-active:exit\n");
		break;
	}
}

void task1_set(uint8_t e)
{
	switch (e)
	{
	case A2_EVT:
		os_stm_tran(task1_on);
		break;

	case STM_ENTRY_SIG:
		dputs("1-set:entry\n");
		break;

	case STM_EXIT_SIG:
		dputs("1-set:exit\n");
		break;
	}
}

void task1_on(uint8_t e)
{
	switch (e)
	{
	case A3_EVT:
		os_stm_tran(task1_set);
		break;

	case A4_EVT:
		os_stm_tran(task1_active);
		break;

	case STM_ENTRY_SIG:
		dputs("1-on:entry\n");
		break;

	case STM_EXIT_SIG:
		dputs("1-on:exit\n");
		break;
	}
}




/******************************************************************************
*  任务2
*/
void task2_active(uint8_t e);
void task2_connect(uint8_t e);
void task2_disconnect(uint8_t e);
void task2_alarm(uint8_t e);

void task2_active(uint8_t e)
{
	switch (e)
	{
	case B1_EVT:
		os_stm_tran(task2_connect);
		break;

	case B5_EVT:
		os_stm_tran(task2_disconnect);
		break;

	case STM_ENTRY_SIG:
		dputs("2-active:entry\n");
		break;

	case STM_EXIT_SIG:
		dputs("2-active:exit\n");
		break;
	}

}
void task2_connect(uint8_t e)
{
	switch (e)
	{
	case B3_EVT:
		os_stm_tran(task2_active);
		break;

	case B2_EVT:
		os_stm_tran(task2_disconnect);
		break;

	case STM_ENTRY_SIG:
		dputs("2-connect:entry\n");
		break;

	case STM_EXIT_SIG:
		dputs("2-connect:exit\n");
		break;
	}
}

void task2_disconnect(uint8_t e)
{
	switch (e)
	{
	case B4_EVT:
		os_stm_tran(task2_alarm);
		break;

	case STM_ENTRY_SIG:
		dputs("2-disconnect:entry\n");
		break;

	case STM_EXIT_SIG:
		dputs("2-disconnect:exit\n");
		break;
	}
}
void task2_alarm(uint8_t e)
{
	switch (e)
	{
	case B6_EVT:
		os_stm_tran(task2_connect);
		break;

	case STM_ENTRY_SIG:
		dputs("2-alarm:entry\n");
		break;

	case STM_EXIT_SIG:
		dputs("2-alarm:exit\n");
		break;
	}
}




/******************************************************************************
*
*/
void uart_callback(uint8_t *buf, uint8_t length)
{
	fcmd_exec(buf);
}


