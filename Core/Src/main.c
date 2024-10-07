/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
typedef  __attribute((aligned())) struct pix
        {
        uint8_t B;
        uint8_t G;
        uint8_t R;
        uint8_t A;
//        uint8_t A;
        }pix_t;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

FDCAN_HandleTypeDef hfdcan1;

LTDC_HandleTypeDef hltdc;

SD_HandleTypeDef hsd2;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */
volatile pix_t *fb= (pix_t*)0xD0000000;

volatile uint32_t cam_buffer [325*256];
//volatile uint32_t cam_buffer_2 [325*256];

volatile uint16_t pix_index = 0;

volatile uint16_t * buffer;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_SDMMC2_SD_Init(void);
static void MX_FMC_Init(void);
static void MX_LTDC_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t odebrane[60];
volatile uint8_t indeks=0;
volatile uint8_t flag=1;
volatile FDCAN_RxHeaderTypeDef RxHeader;

volatile uint32_t szrajben = 1;

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if(flag>0)
    {
        uint8_t liczba=0;

        liczba = HAL_FDCAN_GetRxFifoFillLevel(hfdcan,FDCAN_RX_FIFO0);
        for ( uint8_t x=0; x<liczba; x++)
        {
            HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, odebrane+8*indeks);
            indeks+=1;
        }
    }
}

void nadaj(uint32_t adres,uint8_t *wiadomosc)
{
    FDCAN_TxHeaderTypeDef Txheader;


    uint32_t Txmailbox=0;

    Txheader.DataLength=FDCAN_DLC_BYTES_8;
    Txheader.Identifier = adres;
    Txheader.FDFormat = FDCAN_CLASSIC_CAN;
    Txheader.IdType =  FDCAN_STANDARD_ID;
    Txheader.TxFrameType = FDCAN_DATA_FRAME;
    Txheader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
//    HAL_CAN_AddTxMessage(&hcan1, &Txheader, wiadomosc, &Txmailbox);
    HAL_FDCAN_AddMessageToTxBuffer(&hfdcan1,&Txheader,wiadomosc,FDCAN_TX_BUFFER0);
    HAL_FDCAN_EnableTxBufferRequest(&hfdcan1,FDCAN_TX_BUFFER0);

    while(HAL_FDCAN_IsTxBufferMessagePending(&hfdcan1, Txmailbox));


}


void md5(uint32_t *M, unsigned const N, uint32_t *h)
{
    static uint_fast8_t const g[64] = {
            0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
            1,  6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12,
            5,  8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2,
            0,  7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9
    };

    static uint_fast8_t const s[64] = {
            7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
            5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
            4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
            6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };

    static uint32_t const K[64] = {
            0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
            0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
            0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
            0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
            0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
            0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
            0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
            0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
    };

    uint32_t H[4] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };

    uint32_t *m = M + N;
    *m++ = 0x00000080;
    unsigned n = N;
    while ((n & 15) != 13) *m++ = 0, ++n;
    *m++ = N << 5; *m++ = 0;

    for (; M < m; M += 16) {
        uint32_t A = H[0], B = H[1], C = H[2], D = H[3];
        for (n = 0; n < 64; ++n) {
            uint32_t E = A + K[n] + M[g[n]];
            switch (n >> 4) {
                case 0: E += (B & C) | (~B & D); break;
                case 1: E += (B & D) | (C & ~D); break;
                case 2: E += B ^ C ^ D;         break;
                case 3: E += (B | ~D) ^ C;       break;
            }
            A = D, D = C, C = B, B += (E << s[n]) | (E >> (32 - s[n]));
        }
        H[0] += A, H[1] += B, H[2] += C, H[3] += D;
    }

    h[0] = H[0], h[1] = H[1], h[2] = H[2], h[3] = H[3];
}

static uint32_t X[32];

void autoliv_sk(uint8_t* sk)
{
    static uint32_t const ik[16] = {
            0x575E597C, 0x10167A58, 0x58537A16, 0x77535F58, 0x36363636, 0x36363636, 0x36363636, 0x36363636,
            0x36363636, 0x36363636, 0x36363636, 0x36363636, 0x36363636, 0x36363636, 0x36363636, 0x36363636
    };

    uint8_t* k = sk;
    unsigned n;
    uint32_t* x = X; for (n = 0; n < 16; ++n) *x++ = ik[n];
    for (n = 0; n < 4; ++n, sk += 4) *x++ = sk[0] | (sk[1] << 8) | (sk[2] << 16) | (sk[3] << 24);
    md5(X, 20, X + 16);
    x = X; for (n = 0; n < 16; ++n) *x++ ^= 0x6A6A6A6A;
    md5(X, 20, X);
    x = X; for (n = 0; n < 4; ++n, ++x, k += 4) k[0] = *x, k[1] = *x >> 8, k[2] = *x >> 16, k[3] = *x >> 24;
}

void odblokuj(void)
{


    uint8_t wiadomosc[8]={0x8B,0x02,0x27,0x01,0x55,0x55,0x55,0x55};

    nadaj(0x657,wiadomosc);
    HAL_Delay(100);
    if(odebrane[25]!=0)
    {
        flag=0;
        uint8_t s[16];
        s[0] = odebrane[5]; s[1] = odebrane[6]; s[2] = odebrane[7];
        s[3] = odebrane[10]; s[4] = odebrane[11]; s[5] = odebrane[12]; s[6] = odebrane[13]; s[7] = odebrane[14]; s[8] = odebrane[15];
        s[9] = odebrane[18]; s[10] = odebrane[19]; s[11] = odebrane[20]; s[12] = odebrane[21]; s[13] = odebrane[22]; s[14] = odebrane[23];
        s[15] = odebrane[26];
        autoliv_sk(s);
        wiadomosc[0]=0x8B;
        wiadomosc[1]=0x10;
        wiadomosc[2]=0x12;
        wiadomosc[3]=0x27;
        wiadomosc[4]=0x02;
        wiadomosc[5]=s[0];
        wiadomosc[6]=s[1];
        wiadomosc[7]=s[2];
        nadaj(0x657,wiadomosc);

        HAL_Delay(10);
        wiadomosc[0]=0x8B;
        wiadomosc[1]=0x21;
        wiadomosc[2]=s[3];
        wiadomosc[3]=s[4];
        wiadomosc[4]=s[5];
        wiadomosc[5]=s[6];
        wiadomosc[6]=s[7];
        wiadomosc[7]=s[8];
        nadaj(0x657,wiadomosc);

        HAL_Delay(10);
        wiadomosc[0]=0x8B;
        wiadomosc[1]=0x22;
        wiadomosc[2]=s[9];
        wiadomosc[3]=s[10];
        wiadomosc[4]=s[11];
        wiadomosc[5]=s[12];
        wiadomosc[6]=s[13];
        wiadomosc[7]=s[14];
        nadaj(0x657,wiadomosc);


        HAL_Delay(10);
        wiadomosc[0]=0x8B;
        wiadomosc[1]=0x23;
        wiadomosc[2]=s[15];
        wiadomosc[3]=s[10];
        wiadomosc[4]=s[11];
        wiadomosc[5]=s[12];
        wiadomosc[6]=s[13];
        wiadomosc[7]=s[14];
        nadaj(0x657,wiadomosc);

        HAL_Delay(10);
        HAL_FDCAN_Stop(&hfdcan1);

    }
}


void przeslona (void)
{


    uint8_t wiadomosc[8]={0xC8, 0x64, 0x00, 0x00, 0x02 ,0x00, 0x00, 0x00};

    HAL_FDCAN_Start(&hfdcan1);

    //nadaj(0x401, wiadomosc);
    //HAL_Delay(300);

    HAL_Delay(100);

    wiadomosc[4]= 0x04;
    nadaj(0x401, wiadomosc);
    wiadomosc[4]= 0x00;
    nadaj(0x401, wiadomosc);
    HAL_Delay(50);

    HAL_FDCAN_Stop(&hfdcan1);
}

void dma_config()
{
    __HAL_RCC_DMA1_CLK_ENABLE();

}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FDCAN1_Init();
  MX_SDMMC2_SD_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_SDRAM_MspInit(&hsdram1);
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  hsdram1.Instance->SDCR[0] = 0b0100100111010100;
  hsdram1.Instance->SDCR[1] = 0b0100110111010100;
  hsdram1.Instance->SDTR[0] = 0xFFFFFFF;
  hsdram1.Instance->SDTR[1] = 0xFFFFFFF;
  hsdram1.Instance->SDCMR = 0b00000000110000000001001;
    HAL_Delay(10);
    hsdram1.Instance->SDCMR = 0b01010; //CTB and precharge
    hsdram1.Instance->SDCMR = 0b111101011; // 6
    hsdram1.Instance->SDCMR = 0b00000000110000000001100;//Load mode register
    hsdram1.Instance->SDRTR = 500;



    for(int i = 0; i< 640; i++)
    {
        for(int j =0; j<480; j++)
        {
            pix_t temp={0};
            temp.A = 0;
            temp.R = 0;
            temp.G = 0;
            temp.B = 0;
            if(j<300)
            {
                if(j>100)
                {
                    temp.R = (i/3%255);
                } else
                {
                    temp.G = (i/3%255);
                }
            } else{
                temp.B = (i/3%255);
            }
            uint32_t aux = temp.A<<24 |temp.B<<16 |temp.G<<8 | temp.R;
            *((uint32_t*)&fb[i*480+j]) = aux;

        }

    }

    for (int i = 0; i < 350 * 2; ++i) {
        *(uint32_t*)(0x30000000 + i * 4) = i < 350 ? 0xFFFF0000 : 0xFF00FF00;
    }

    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn,0,0);
    HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn,0,0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);

    HAL_FDCAN_Start(&hfdcan1);
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
//    HAL_FDCAN_EnableTxBufferRequest(&hfdcan1,FDCAN_TX_BUFFER0);

    HAL_Delay(500);
    odblokuj();
    przeslona();
    HAL_FDCAN_Stop(&hfdcan1);

    dma_config();
//    for(int  i = 0; i< 480*640; i++)
//    {
//        *(int32_t *)(0xD0000000+i*4) = 0xFFFF0000;
//    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    HAL_GPIO_WritePin(LCD_PWM_GPIO_Port,LCD_PWM_Pin,GPIO_PIN_SET);

//    for(int  i = 0; i< 480*640; i++)
//    {
//        *(int32_t *)(0xD0400000+i*4) = 0xFF00FF00;
//    }
    int loop_cnt;

    LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(TIM2);
    //LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t)&GPIOC->IDR, 0xD0000000, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    //LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 320);
//    LL_TIM_EnableDMAReq_UPDATE(TIM2);
    //LL_TIM_EnableDMAReq_CC1(TIM2);
    //LL_TIM_EnableIT_UPDATE(TIM2);
    //LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
    HAL_Delay(100);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    HAL_Delay(100);
    MX_LTDC_Init();
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    LL_DMA_EnableIT_TC(DMA1,LL_DMA_STREAM_0);
    MX_LTDC_Init();
    while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//      for(int i =0; i< 480*640; i++)
//      {
//          fb[i].R = 10;
//          fb[i].G = 20;
//          fb[i].B = 100;
//      }

//      volatile uint32_t * address =(uint32_t *) 0xD0000000;
//
//      *address = 0xAABBCCDD;
//
//      for(uint32_t i = 0; i<1e6; i++)
//      {
//          *address = i;
//          address++;
//      }
//
//      address =(uint32_t *) 0xD0000000;
//      for(uint32_t i = 0; i<1e6; i++)
//      {
//          if(*address != i)
//          {
//              HAL_GPIO_TogglePin(LED_ORANGE_GPIO_Port,LED_ORANGE_Pin);
//          }
//          address++;
//      }
//
        //memcpy((void*)0xD0000000, (void*)0xD0200000,320*480*4);
        //HAL_Delay(100);
        //HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);


//    for(int  i = 0; i< 480*640; i++)
//    {
//        *(int32_t *)(0xD0000000+i*4) = loop_cnt;
//    }
//      *(uint32_t*)0x500010ac = 0xD0400000;//    LTDC_Layer
    loop_cnt++;
//        memcpy((void *)0xD0000000, (void*)cam_buffer, sizeof (cam_buffer));


uint32_t dest=0xD0000000;
if(loop_cnt%2)
{
    dest = 0xD00A0000;
}
if(szrajben) {
    for (int lines = 0; lines <= 256; lines++) {
//            for(volatile int delay=0; delay<10000; delay++);
        for (int pixel = 0; pixel < 325; pixel++) {
            *(uint32_t *) (dest + 4 * pixel + 480 * lines * 4) = ((cam_buffer[pixel + 325 * lines]) << 8) | 0xFF;
        }

    }
}

HAL_Delay(70);
//        szrajben = 0;

//      HAL_Delay(10);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 5;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC|RCC_PERIPHCLK_FDCAN;
  PeriphClkInitStruct.PLL2.PLL2M = 2;
  PeriphClkInitStruct.PLL2.PLL2N = 60;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 10;
  PeriphClkInitStruct.PLL2.PLL2R = 3;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
  PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 41;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 15;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 15;
  hfdcan1.Init.DataTimeSeg2 = 2;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 3;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 1;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 3;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 1;
  hfdcan1.Init.TxBuffersNbr = 3;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 31;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 51;
  hltdc.Init.AccumulatedVBP = 24;
  hltdc.Init.AccumulatedActiveW = 531;
  hltdc.Init.AccumulatedActiveH = 664;
  hltdc.Init.TotalWidth = 557;
  hltdc.Init.TotalHeigh = 689;
  hltdc.Init.Backcolor.Blue = 50;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 640;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 255;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0xD0000000;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 640;
  pLayerCfg.Backcolor.Blue = 100;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief SDMMC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC2_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC2_Init 0 */
    return;
  /* USER CODE END SDMMC2_Init 0 */

  /* USER CODE BEGIN SDMMC2_Init 1 */

  /* USER CODE END SDMMC2_Init 1 */
  hsd2.Instance = SDMMC2;
  hsd2.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd2.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd2.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd2.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd2.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC2_Init 2 */

  /* USER CODE END SDMMC2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
  /**TIM2 GPIO Configuration
  PA0   ------> TIM2_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* TIM2 DMA Init */

  /* TIM2_CH1 Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_0, LL_DMAMUX1_REQ_TIM2_CH1);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_VERYHIGH);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_WORD);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_WORD);

  LL_DMA_EnableFifoMode(DMA1, LL_DMA_STREAM_0);

  LL_DMA_SetFIFOThreshold(DMA1, LL_DMA_STREAM_0, LL_DMA_FIFOTHRESHOLD_1_2);

  LL_DMA_SetMemoryBurstxfer(DMA1, LL_DMA_STREAM_0, LL_DMA_MBURST_SINGLE);

  LL_DMA_SetPeriphBurstxfer(DMA1, LL_DMA_STREAM_0, LL_DMA_PBURST_SINGLE);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 4294967295;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
  NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, LED_ORANGE_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_AUX_1_Pin|LCD_PWM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_ORANGE_Pin LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_ORANGE_Pin|LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_AUX_1_Pin LCD_PWM_Pin */
  GPIO_InitStruct.Pin = LCD_AUX_1_Pin|LCD_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PG9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC11 PC12 PC13
                           PC8 PC9 PC7 PC6
                           PC0 PC1 PC2 PC3
                           PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_7|GPIO_PIN_6
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PH15 PH14 PH10 PH11
                           PH12 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PC15 PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA12 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DES_LOCK_Pin IO_AUX_Pin PA3 */
  GPIO_InitStruct.Pin = DES_LOCK_Pin|IO_AUX_Pin|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_DET_A_Pin */
  GPIO_InitStruct.Pin = SD_DET_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SD_DET_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PH8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : SW_MODE_Pin SW_PICTURE_Pin */
  GPIO_InitStruct.Pin = SW_MODE_Pin|SW_PICTURE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);

/* USER CODE BEGIN MX_GPIO_Init_2 */
  /*  GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);*/
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
