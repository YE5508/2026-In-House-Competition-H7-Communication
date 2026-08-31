/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for AlarmTask */
osThreadId_t AlarmTaskHandle;
const osThreadAttr_t AlarmTask_attributes = {
  .name = "AlarmTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Chassis_Control */
osThreadId_t Chassis_ControlHandle;
const osThreadAttr_t Chassis_Control_attributes = {
  .name = "Chassis_Control",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BigBlock_Contro */
osThreadId_t BigBlock_ControHandle;
const osThreadAttr_t BigBlock_Contro_attributes = {
  .name = "BigBlock_Contro",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SkyBlock_Contro */
osThreadId_t SkyBlock_ControHandle;
const osThreadAttr_t SkyBlock_Contro_attributes = {
  .name = "SkyBlock_Contro",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Ball_Control */
osThreadId_t Ball_ControlHandle;
const osThreadAttr_t Ball_Control_attributes = {
  .name = "Ball_Control",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Alarm_Task(void *argument);
void Chassis_Control_Task(void *argument);
void BigBlock_Control_Task(void *argument);
void SkyBlock_Control_Task(void *argument);
void Ball_Control_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* 任务在 CubeMX 里创建,这里不需要额外初始化 */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of AlarmTask */
  AlarmTaskHandle = osThreadNew(Alarm_Task, NULL, &AlarmTask_attributes);

  /* creation of Chassis_Control */
  Chassis_ControlHandle = osThreadNew(Chassis_Control_Task, NULL, &Chassis_Control_attributes);

  /* creation of BigBlock_Contro */
  BigBlock_ControHandle = osThreadNew(BigBlock_Control_Task, NULL, &BigBlock_Contro_attributes);

  /* creation of SkyBlock_Contro */
  SkyBlock_ControHandle = osThreadNew(SkyBlock_Control_Task, NULL, &SkyBlock_Contro_attributes);

  /* creation of Ball_Control */
  Ball_ControlHandle = osThreadNew(Ball_Control_Task, NULL, &Ball_Control_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Alarm_Task */
/**
  * @brief  Function implementing the AlarmTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Alarm_Task */
__weak void Alarm_Task(void *argument)
{
  /* USER CODE BEGIN Alarm_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Alarm_Task */
}

/* USER CODE BEGIN Header_Chassis_Control_Task */
/**
* @brief Function implementing the Chassis_Control thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Chassis_Control_Task */
void Chassis_Control_Task(void *argument)
{
  /* USER CODE BEGIN Chassis_Control_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Chassis_Control_Task */
}

/* USER CODE BEGIN Header_BigBlock_Control_Task */
/**
* @brief Function implementing the BigBlock_Contro thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BigBlock_Control_Task */
void BigBlock_Control_Task(void *argument)
{
  /* USER CODE BEGIN BigBlock_Control_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END BigBlock_Control_Task */
}

/* USER CODE BEGIN Header_SkyBlock_Control_Task */
/**
* @brief Function implementing the SkyBlock_Contro thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SkyBlock_Control_Task */
void SkyBlock_Control_Task(void *argument)
{
  /* USER CODE BEGIN SkyBlock_Control_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SkyBlock_Control_Task */
}

/* USER CODE BEGIN Header_Ball_Control_Task */
/**
* @brief Function implementing the Ball_Control thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Ball_Control_Task */
void Ball_Control_Task(void *argument)
{
  /* USER CODE BEGIN Ball_Control_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Ball_Control_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

