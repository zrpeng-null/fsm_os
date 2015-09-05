/*******************************************************************************
--有限状态机系统
--0.01
--zrpeng
--2015-6-13
--
--note:
	
--history:
	0.01	2015-6-6
	在stm32上面运行正常，支持低功耗__wfe模式,运行正常，仿照CC2540的osal层写的，
	增加了有限状态机跳转操作。
*******************************************************************************/

#include "stdint.h"
#include "os.h"
#include "hal_driver.h"
#include "app_task.h"

//状态机,数组偏移就是状态机的id号
stm_handler tasks_pool[TASKS_CNT] =
{
	hal_active,
	task1_active,
	task2_active
};
queue_t tasks_queue[TASKS_CNT];	/* 每个状态机对应的消息队列 */
uint8_t stm_ret_value;          /* 状态机返回值 */
stm_handler stm_state_temp;     /* 临时状态机变量 */

//定时器
timer_t timer_pool[TIMER_CNT];
uint32_t tick_previous;

//电源
#ifdef POWER_SAVING
uint32_t power_task_state;		/* 32个位，支持32个fsm,可以扩大 */
#endif

/*
 * static function
 */
static void os_timer_update(void);
static void os_power_sleep(void);

/******************************************************************************
*       事件循环
*/
void os_run_system(void)
{
	uint8_t tasks_idx;
	os_timer_update();
	hal_poll();
	
	for (tasks_idx = 0; tasks_idx < TASKS_CNT; tasks_idx++)
		if (tasks_queue[tasks_idx].used)
			break;

	//处理事件
	if (tasks_idx < TASKS_CNT)
	{
		uint8_t e;
		
		PORT_SR_ALLOC();
		PORT_CPU_DISABLE();//disable
		
		tasks_queue[tasks_idx].used--;
		e = tasks_queue[tasks_idx].buf[ tasks_queue[tasks_idx].head ];
		tasks_queue[tasks_idx].head++;
		if (tasks_queue[tasks_idx].head >= QUEUE_SIZE)
		{
			tasks_queue[tasks_idx].head = 0;
		}
		PORT_CPU_ENABLE();//enable
		
		(tasks_pool[tasks_idx])(e);
		if (stm_ret_value == STM_TRAN)
		{
			stm_ret_value = 0;
			(tasks_pool[tasks_idx])(STM_EXIT_SIG);
			stm_state_temp(STM_ENTRY_SIG);
			tasks_pool[tasks_idx] = stm_state_temp;
		}
	}
#ifdef POWER_SAVING
	else
	{
		os_power_sleep();
	}
#endif
}

void os_init_tasks(void)
{
	uint8_t i;
	for (i = 0; i < TASKS_CNT; i++)
		os_post_message(i, STM_ENTRY_SIG);
	for (i = 0; i < TIMER_CNT; i++)
		timer_pool[i].task_id = INVALID_TASK_ID;
}

/******************************************************************************
*   事件
*/
uint8_t os_post_message(uint8_t task_id, uint8_t e)
{
	PORT_SR_ALLOC();
	PORT_CPU_DISABLE();//disable
	if (tasks_queue[task_id].used >= QUEUE_SIZE)
	{
		PORT_CPU_ENABLE();//enable
		return QUEUE_FULLED;
	}
	tasks_queue[task_id].buf[ tasks_queue[task_id].tail ] = e;
	tasks_queue[task_id].tail++;
	if (tasks_queue[task_id].tail >= QUEUE_SIZE)
	{
		tasks_queue[task_id].tail = 0;
	}
	tasks_queue[task_id].used++;
	PORT_CPU_ENABLE();//enable
	
	return RET_SUCCESS;
}

/******************************************************************************
*  定时器
*/
static void os_timer_update(void)
{	
	uint16_t elapse_ms;
	uint8_t i;
	
	if (sys_tick != tick_previous)
	{
		elapse_ms = sys_tick - tick_previous;
		tick_previous = sys_tick;
		
		for (i=0; i<TIMER_CNT; i++)
		{
			if (timer_pool[i].task_id != INVALID_TASK_ID)
			{
				if (timer_pool[i].timeout > elapse_ms)
				{
					timer_pool[i].timeout -= elapse_ms;
				}
				else
				{
					os_post_message(timer_pool[i].task_id, timer_pool[i].sig);
					if (timer_pool[i].flag & TIMER_RELOAD_FLAG)
					{
						timer_pool[i].timeout = timer_pool[i].reload;
					}
					else
					{
						timer_pool[i].task_id = INVALID_TASK_ID;
					}
				}
			}
		}
	}
}

void os_timer_set(uint8_t task_id, uint8_t sig, uint16_t timeout)
{
	uint8_t i;
	
	PORT_SR_ALLOC();
	PORT_CPU_DISABLE();//disable
	//申请定时器
	for (i = 0; i < TIMER_CNT; i++)
	{
		if (timer_pool[i].task_id == task_id &&
			timer_pool[i].sig == sig)		//已存在定时器
		{
			break;
		}
		else if (timer_pool[i].task_id == INVALID_TASK_ID)
		{
			timer_pool[i].sig = sig;
			timer_pool[i].timeout = timeout;
			timer_pool[i].task_id = task_id;
			timer_pool[i].flag = 0;
			break;
		}
	}
	PORT_CPU_ENABLE();//enable
}

void os_reload_timer_set(uint8_t task_id, uint8_t sig, uint16_t timeout)
{
	uint8_t i;
	
	PORT_SR_ALLOC();
	PORT_CPU_DISABLE();//disable
	//申请定时器
	for (i = 0; i < TIMER_CNT; i++)
	{
		if (timer_pool[i].task_id == task_id &&
			timer_pool[i].sig == sig)		//已存在定时器
		{
			break;
		}
		else if (timer_pool[i].task_id == INVALID_TASK_ID)
		{
			timer_pool[i].sig = sig;
			timer_pool[i].timeout = timeout;
			timer_pool[i].reload = timeout;
			timer_pool[i].task_id = task_id;
			timer_pool[i].flag |= TIMER_RELOAD_FLAG;
			break;
		}
	}
	PORT_CPU_ENABLE();//enable
}

void os_timer_del(uint8_t task_id, uint8_t sig)
{
	uint8_t i;
	
	PORT_SR_ALLOC();
	PORT_CPU_DISABLE();//disable
	//删除定时器
	for (i = 0; i < TIMER_CNT; i++)
	{
		if (timer_pool[i].task_id == task_id &&
			timer_pool[i].sig == sig)		//删除存在定时器
		{
			timer_pool[i].task_id = INVALID_TASK_ID;
			timer_pool[i].flag = 0;
			break;
		}
	}
	PORT_CPU_ENABLE();//enable
}

#ifdef POWER_SAVING
/******************************************************************************
*   电源低功耗
*/
static void os_power_sleep(void)
{
	if (power_task_state == 0)
	{
		__wfe();	//m3内核停机省电
	}
}
void os_power_task_state(uint8_t task_id, uint8_t state)
{
	if (state == POWER_HOLD)
	{
		power_task_state |= 1 << task_id;
	}
	else if (state == POWER_SLEEP)
	{
		power_task_state &= ~(1 << task_id);
	}
}
#endif

/******************************************************************************
*  有限状态机跳转
*/
void os_stm_tran(stm_handler state)
{
	stm_ret_value = STM_TRAN;
	stm_state_temp = state;
}

