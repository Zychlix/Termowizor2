/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <memory.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
extern volatile uint32_t pix_index;
extern volatile uint32_t wlacznik;
extern volatile uint32_t jedziesz;
//static uint32_t buffer[350*2];

uint32_t *volatile read = (void *) (0x30000000);
uint32_t *volatile write = (void *) (0x30000000 + 350 * 4);
volatile uint32_t portc_when_hsync = 0;

uint32_t interrupt_times[1024];

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile uint32_t line_cnt;
extern volatile uint32_t cam_buffer[325 * 256];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern FDCAN_HandleTypeDef hfdcan1;
extern LTDC_HandleTypeDef hltdc;
extern DMA_HandleTypeDef hdma_tim5_ch4;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */
    LL_DMA_ClearFlag_TC0(DMA1);
    LL_DMA_ClearFlag_TE0(DMA1);
//  volatile uint32_t* ptr = read;
//  for(int i = 0; i < 320; i++)
//  {
    //     *(uint32_t *)(0xD0000000 + 480*4*line_cnt + i*4) = *(ptr + i);
    // }

  /* USER CODE END DMA1_Stream0_IRQn 0 */

  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles FDCAN1 interrupt 0.
  */
void FDCAN1_IT0_IRQHandler(void)
{
  /* USER CODE BEGIN FDCAN1_IT0_IRQn 0 */

  /* USER CODE END FDCAN1_IT0_IRQn 0 */
  HAL_FDCAN_IRQHandler(&hfdcan1);
  /* USER CODE BEGIN FDCAN1_IT0_IRQn 1 */

  /* USER CODE END FDCAN1_IT0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
    uint32_t port = GPIOC->IDR;
//    for (volatile int i = 0; i < 5000; ++i) {}
    LL_DMA_ClearFlag_TC0(DMA1);
    LL_DMA_ClearFlag_FE0(DMA1);
    LL_DMA_ClearFlag_TE0(DMA1);

    if (!wlacznik) return;

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != 0x00U) //Na VSYNC
    {
        if (line_cnt > 200)
            line_cnt = 0;


    } /*else if (!(port & GPIO_PIN_9))   //Ramka dobra
    {



          LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0,(0xD0000000 + 480*4* line_cnt));
          LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 320);
          line_cnt++;
          if(line_cnt>= 640)  line_cnt =0;
          LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);


        //LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t)&GPIOC->IDR, (0xD0000000 + 480*4* line_cnt), LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
        //LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t)&GPIOC->IDR, (0xD0200000), LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

        //__disable_irq();
        //   uint32_t* tmp = read;
        //  read = write;
//write = tmp;
        //__enable_irq();

        LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t) &GPIOC->IDR,
                               ((uint32_t) cam_buffer + 325 * 4 * line_cnt), LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
        //   LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH1);
        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 325);
        //  LL_TIM_EnableDMAReq_UPDATE(TIM2);
        LL_TIM_EnableDMAReq_CC1(TIM2);
        //LL_TIM_EnableIT_UPDATE(TIM2);

        //  LL_TIM_EnableCounter(TIM2);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
        LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_0);
        LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);
        line_cnt++;

    }*/
  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
    static uint32_t idx = 0;
//    interrupt_times[idx++] = DWT->CYCCNT;

//    if (idx >= 1024) {
//        idx = 0;
//    }

    LL_DMA_ClearFlag_TE0(DMA2);

    if (LL_DMA_IsActiveFlag_TC0(DMA2)) {
        LL_DMA_ClearFlag_TC0(DMA2);

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */
        if ((portc_when_hsync & GPIO_PIN_9)) {
            LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t) &GPIOC->IDR,
                                   ((uint32_t) cam_buffer + 325 * 4 * line_cnt), LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
            LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 325);
            LL_TIM_EnableDMAReq_CC1(TIM2);

            LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
            LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_0);
            LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);

            line_cnt++;
        }
    }
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */
    jedziesz = 1;

//  hltdc.Instance->IER = 0;
    hltdc.Instance->ICR = 0xFF;
    hltdc.Instance->ICR = 0;
    hltdc.Instance->ICR = 1 << 0;
    hltdc.Instance->ICR = 1 << 1;
    hltdc.Instance->ICR = 1 << 2;
    hltdc.Instance->ICR = LTDC_ICR_CLIF;
    hltdc.Instance->IER = 0;



    __HAL_LTDC_CLEAR_FLAG(&hltdc, LTDC_FLAG_LI);

    return;

  /* USER CODE END LTDC_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_IRQn 1 */
    hltdc.Instance->IER = LTDC_IER_LIE;
  /* USER CODE END LTDC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
