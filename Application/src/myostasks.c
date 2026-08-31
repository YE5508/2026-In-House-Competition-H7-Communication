/**
 * @file    tasks.c
 * @brief   任务函数实现(强定义覆盖 freertos.c 里的 __weak 占位)。
 *
 * 新建任务的标准流程:
 *   1. CubeMX 里创建任务(生成 freertos.c 的任务创建代码和 __weak 任务函数);
 *   2. 在 CubeMX 任务函数的 USER CODE 里写业务,或者在本文件写强定义覆盖。
 */
#include "includes.h"
#include "myostasks.h"
#include "app_config.h"
#include "board_config.h"
#include "protocol.h"
#include "FD_Canqueue.h"

#include "bsp_led.h"
#include "vofa.h"
#include "UnitreeMotor.h"
#include "protocol.h"

/* CubeMX 默认任务 LEDTask:四灯流水 */
void Alarm_Task(void *argument)
{
    (void)argument;

    for (;;)
    {
        for (uint8_t i = 0; i < BOARD_LED_NUM; i++)
        {
            BspLed_On(i);
            osDelay(APP_LED_TASK_PERIOD_MS);
            BspLed_Off(i);
            osDelay(APP_LED_TASK_PERIOD_MS);
        }
    }
}

void Chassis_Control_Task(void *argument)
{
  for(;;)
  {
    CAN_DequeueTx(&Chassis_queue);
    osDelay(1);
  }
}

void BigBlock_Control_Task(void *argument)
{
  for(;;)
  {
    CAN_DequeueTx(&BigBlock_queue);
    osDelay(1);
  }
}

void SkyBlock_Control_Task(void *argument)
{
  for(;;)
  {
    CAN_DequeueTx(&Sky_queue);
    osDelay(1);
  }
}

void Ball_Control_Task(void *argument)
{
  for(;;)
  {
    CAN_DequeueTx(&Ball_queue);
    osDelay(1);
  }
}
