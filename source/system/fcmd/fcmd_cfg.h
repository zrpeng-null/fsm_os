#ifndef _FCMD_CFG_H_
#define _FCMD_CFG_H_
/*******************************************************************************
* 用户函数命令头文件包含，函数声明
*/

#include "cmd_mem.h"
#include "os.h"




/*******************************************************************************
 * 自定义函数命令表
 */
CmdTbl_t CmdTbl[] =
{
    //系统命令, SYSTEM_CMD_NUM和系统命令个数保持一致
    "ls",  0,
    "addr", 0,
    "help", 0,


    //用户命令
    "void md(int addr, int elem_cnt, int elem_size)", (void(*)(void))md,
    "int cmp(void *addr1, void *addr2, int elem_cnt, int elem_size)", (void(*)(void))cmp,
    "void memset(void *s, int c, size_t n)", (void(*)(void))memset,
    "void *memcpy(void *s1, const void *s2, size_t n)", (void(*)(void))memcpy,

    "uint8_t os_post_message(uint8_t task_id, uint8_t sig)", 						(void(*)(void))os_post_message,
    "void os_timer_set(uint8_t task_id, uint8_t sig, uint16_t timeout)", 			(void(*)(void))os_timer_set,
    "void os_reload_timer_set(uint8_t task_id, uint8_t sig, uint16_t timeout)",		(void(*)(void))os_reload_timer_set,
    "void os_timer_del(uint8_t task_id, uint8_t sig)",								(void(*)(void))os_timer_del,
    "void os_power_task_state(uint8_t task_id, uint8_t state)", 					(void(*)(void))os_power_task_state,

};
uint8_t CmdTblSize = sizeof(CmdTbl) / sizeof(CmdTbl[0]);

#endif
