#include "bsp.h"

#include "os.h"
#include "hal_driver.h"


uint32_t sys_tick;
void hal_driver_init(void)
{
    PORT_SR_ALLOC();
    PORT_CPU_DISABLE();

    jtag_set(JTAG_SWD_ENABLE);//JTAG_SWD_DISABLE
    delay_init(100);//滴答时钟，1秒100次中断

#if UART_DEBUG
    uart_init(115200);
#endif

    //按键
    ;

    //adc
    ;

    PORT_CPU_ENABLE();
}
void hal_poll(void)
{
#if UART_DEBUG
    uart_poll();
#endif

    //usb_poll();
}

/******************************************************************************
*  驱动任务状态机，这个状态机作为0号状态机，专门用来处理驱动程序
*/
void hal_active(uint8_t e)
{
    switch (e)
    {
    case HAL_KEY_EVT:
        dputs("key\r\n");
        break;

    case HAL_TIMEOUT:
        dputs("0-timeout\r\n");
        os_timer_set(HAL_ID, HAL_TIMEOUT, 100);
        break;

    case STM_ENTRY_SIG:
        {
            dputs("0-entry\n");
            os_timer_set(HAL_ID, HAL_TIMEOUT, 100);
            os_power_task_state(HAL_ID, POWER_HOLD);//保持供电，不睡眠
        }
        break;

    case STM_EXIT_SIG:
        dputs("0-exit\n");
        break;
    }
}
