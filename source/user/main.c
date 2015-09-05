#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "bsp.h"

#include "os.h"
#include "hal_driver.h"


//Ӳ��������
void HardFault_Handler(void)
{
	u32 i;
	u8 t = 0;
	//fault״̬�Ĵ���(@0XE000ED28)����:MMSR,BFSR,UFSR
	dprintf("CFSR:%8X\r\n", SCB->CFSR);	//��ʾ����ֵ
	dprintf("HFSR:%8X\r\n", SCB->HFSR);	//��ʾ����ֵ
	dprintf("DFSR:%8X\r\n", SCB->DFSR);	//��ʾ����ֵ
	dprintf("AFSR:%8X\r\n", SCB->AFSR);	//��ʾ����ֵ
	while (t < 5)
	{
		t++;
		for (i = 0; i < 0X1FFFFF; i++);
	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//16��������ռ���ȼ�

	hal_driver_init();

	os_init_tasks();

	for (;;)
	{
		os_run_system();
	}
}

