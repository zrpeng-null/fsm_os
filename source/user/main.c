#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "bsp.h"

#include "os.h"
#include "hal_driver.h"


//硬件错误处理
void HardFault_Handler(void)
{
	u32 i;
	u8 t = 0;
	//fault状态寄存器(@0XE000ED28)包括:MMSR,BFSR,UFSR
	dprintf("CFSR:%8X\r\n", SCB->CFSR);	//显示错误值
	dprintf("HFSR:%8X\r\n", SCB->HFSR);	//显示错误值
	dprintf("DFSR:%8X\r\n", SCB->DFSR);	//显示错误值
	dprintf("AFSR:%8X\r\n", SCB->AFSR);	//显示错误值
	while (t < 5)
	{
		t++;
		for (i = 0; i < 0X1FFFFF; i++);
	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//16个都是抢占优先级

	hal_driver_init();

	os_init_tasks();

	for (;;)
	{
		os_run_system();
	}
}

