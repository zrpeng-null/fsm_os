#ifndef _os_h_
#define _os_h_

#include "stdint.h"
#include <string.h>

typedef void (*stm_handler)( uint8_t e );

#define INVALID_TASK_ID			0xFF

#define BV(x)  (1<<(x))

enum
{
	RET_SUCCESS			= 0,
	STM_TRAN,
	QUEUE_FULLED,
	QUEUE_EMPTY,
	FAILURE,
	TIMER_UNSET_OK,
	TIMER_SET_OK,
	TIMER_FAIL,
};

/*
 * ���ú�
 */
#define POWER_SAVING
#define TASKS_CNT		3

/*
 * �ж��ٽ����궨��
 */
typedef unsigned char            CPU_SR;

CPU_SR  CPU_SR_Save (void);
void    CPU_SR_Restore (CPU_SR sr);

#define PORT_SR_ALLOC()         CPU_SR cpu_sr = (CPU_SR)0
#define PORT_CPU_DISABLE()      { cpu_sr = CPU_SR_Save();}      /* disable cpu interrupt */
#define PORT_CPU_ENABLE()       { CPU_SR_Restore(cpu_sr);}      /* enable cpu interrupt */

/*
 * ��Դ�궨��
 */
#define POWER_HOLD				1
#define POWER_SLEEP				0


/******************************************************************************
*  ��ʱ��
*/
#define TIMER_CNT	8
#define TIMER_RELOAD_FLAG 0x80
typedef struct
{
	uint8_t sig;
	uint8_t task_id;
	uint8_t flag;
	uint16_t timeout;
	uint16_t reload;
} timer_t;


/******************************************************************************
* ��Ϣ����
*/
#define QUEUE_SIZE 4
typedef struct
{
	uint8_t head;
	uint8_t tail;
	uint8_t used;
	uint8_t buf[QUEUE_SIZE];
} queue_t;


/******************************************************************************
* ˫���������
*/
typedef struct list_node
{
    struct list_node *next;
    struct list_node *prev;
} list_t;

#define list_entry(node, type, member)    ( (type *)( ( uint8_t *)(node) - (uint32_t)(&((type *)0)->member) ) )

/*
 * return TRUE if the list is empty
 */
#define list_is_empty(list)   ((list)->next == (list))

/*
 * List initialization
 */
#define list_init(list_head)  \
            do {\
                (list_head)->next = (list_head);\
                (list_head)->prev = (list_head);\
            } while (0)

/*
 * add elem to node's before.
 */
#define list_insert(node, elem) \
            do {\
                (elem)->prev       = (node)->prev;\
                (elem)->next       = (node);\
                (node)->prev->next = (elem);\
                (node)->prev       = (elem);\
             } while (0)

/*
 * add elem to node's after.
 */
#define list_insert_after(node, elem) \
            do {\
                (elem)->prev       = (node);\
                (elem)->next       = (node)->next;\
                (node)->next->prev = (elem);\
                (node)->next       = (elem);\
             } while (0)

/*
 * delete elem from list
 */
#define list_delete(elem) \
            do {\
                (elem)->prev->next = (elem)->next;\
                (elem)->next->prev = (elem)->prev;\
            } while (0)


/******************************************************************************
* public function
*/
void os_run_system(void);
void os_init_tasks(void);

uint8_t os_post_message(uint8_t task_id, uint8_t sig);

void os_timer_set(uint8_t task_id, uint8_t sig, uint16_t timeout);
void os_reload_timer_set(uint8_t task_id, uint8_t sig, uint16_t timeout);
void os_timer_del(uint8_t task_id, uint8_t sig);

void os_power_task_state(uint8_t task_id, uint8_t state);
void os_stm_tran(stm_handler state);

#endif
