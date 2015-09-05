#ifndef _hal_driver_h_
#define _hal_driver_h_


/******************************************************************************
 * 事件定义
 */
enum
{
    STM_ENTRY_SIG = 0,
    STM_EXIT_SIG,

    /*
     * hal层任务事件
     */
    HAL_DIGITLED_EVT,
    HAL_BUZZER_EVT,
    HAL_LED_EVT,
    HAL_KEY_EVT,
    HAL_TIMEOUT,

    /*
     * task1事件
     */
    A1_EVT,
    A2_EVT,
    A3_EVT,
    A4_EVT,

    /*
     * task2事件
     */
    B1_EVT,
    B2_EVT,
    B3_EVT,
    B4_EVT,
    B5_EVT,
    B6_EVT,
};

/*
 * 任务ID定义
 */
enum
{
    HAL_ID	   = 0,
    TASK1_ID,
    TASK2_ID,
};

/*
 * 状态定义
 */
enum
{
    APP1_ACTIVE  = 0,
    APP1_SET,
    APP1_ON,
};
enum
{
    APP2_ACTIVE  = 0,
    APP2_CONNECT,
    APP2_DISCONNECT,
    APP2_ALARM,
};


extern uint32_t sys_tick;



/*
 * public function
 */
void hal_driver_init(void);

void hal_active(uint8_t e);
void hal_poll(void);

#endif
