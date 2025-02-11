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

__attribute__((section(".ram1section"))) uint32_t cam_buffer_2 [325*200];


volatile uint8_t buffer_index = 0;


//volatile uint8_t buffer_index = 0;

volatile uint16_t * buffer;



//kolory

const uint32_t paleta1 [256] = { 0x7f0000, 0x840000, 0x880000, 0x8d0000, 0x910000, 0x960000, 0x9a0000, 0x9f0000, 0xa30000, 0xa80000,
                                 0xac0000, 0xb10000, 0xb60000, 0xba0000, 0xbf0000, 0xc30000, 0xc80000, 0xcc0000, 0xd10000, 0xd50000,
                                 0xda0000, 0xde0000, 0xe30000, 0xe80000, 0xec0000, 0xf10000, 0xf50000, 0xfa0000, 0xfe0000, 0xff0000,
                                 0xff0000, 0xff0000, 0xff0000, 0xff0400, 0xff0800, 0xff0c00, 0xff1000, 0xff1400, 0xff1800, 0xff1c00,
                                 0xff2000, 0xff2400, 0xff2800, 0xff2c00, 0xff3000, 0xff3400, 0xff3800, 0xff3c00, 0xff4000, 0xff4400,
                                 0xff4800, 0xff4c00, 0xff5000, 0xff5400, 0xff5800, 0xff5c00, 0xff6000, 0xff6400, 0xff6800, 0xff6c00,
                                 0xff7000, 0xff7400, 0xff7800, 0xff7c00, 0xff8000, 0xff8400, 0xff8800, 0xff8c00, 0xff9000, 0xff9400,
                                 0xff9800, 0xff9c00, 0xffa000, 0xffa400, 0xffa800, 0xffac00, 0xffb000, 0xffb400, 0xffb800, 0xffbc00,
                                 0xffc000, 0xffc400, 0xffc800, 0xffcc00, 0xffd000, 0xffd400, 0xffd800, 0xfedc00, 0xfae000, 0xf7e400,
                                 0xf4e802, 0xf1ec05, 0xedf008, 0xeaf40c, 0xe7f80f, 0xe4fc12, 0xe1ff15, 0xddff18, 0xdaff1c, 0xd7ff1f,
                                 0xd4ff22, 0xd0ff25, 0xcdff29, 0xcaff2c, 0xc7ff2f, 0xc3ff32, 0xc0ff36, 0xbdff39, 0xbaff3c, 0xb7ff3f,
                                 0xb3ff42, 0xb0ff46, 0xadff49, 0xaaff4c, 0xa6ff4f, 0xa3ff53, 0xa0ff56, 0x9dff59, 0x9aff5c, 0x96ff5f,
                                 0x93ff63, 0x90ff66, 0x8dff69, 0x89ff6c, 0x86ff70, 0x83ff73, 0x80ff76, 0x7dff79, 0x79ff7c, 0x76ff80,
                                 0x73ff83, 0x70ff86, 0x6cff89, 0x69ff8d, 0x66ff90, 0x63ff93, 0x5fff96, 0x5cff9a, 0x59ff9d, 0x56ffa0,
                                 0x53ffa3, 0x4fffa6, 0x4cffaa, 0x49ffad, 0x46ffb0, 0x42ffb3, 0x3fffb7, 0x3cffba, 0x39ffbd, 0x36ffc0,
                                 0x32ffc3, 0x2fffc7, 0x2cffca, 0x29ffcd, 0x25ffd0, 0x22ffd4, 0x1fffd7, 0x1cffda, 0x18ffdd, 0x15ffe0,
                                 0x12ffe4, 0x0fffe7, 0x0cffea, 0x08ffed, 0x05fcf1, 0x02f8f4, 0x00f4f7, 0x00f0fa, 0x00edfe, 0x00e9ff,
                                 0x00e5ff, 0x00e2ff, 0x00deff, 0x00daff, 0x00d7ff, 0x00d3ff, 0x00cfff, 0x00cbff, 0x00c8ff, 0x00c4ff,
                                 0x00c0ff, 0x00bdff, 0x00b9ff, 0x00b5ff, 0x00b1ff, 0x00aeff, 0x00aaff, 0x00a6ff, 0x00a3ff, 0x009fff,
                                 0x009bff, 0x0098ff, 0x0094ff, 0x0090ff, 0x008cff, 0x0089ff, 0x0085ff, 0x0081ff, 0x007eff, 0x007aff,
                                 0x0076ff, 0x0073ff, 0x006fff, 0x006bff, 0x0067ff, 0x0064ff, 0x0060ff, 0x005cff, 0x0059ff, 0x0055ff,
                                 0x0051ff, 0x004dff, 0x004aff, 0x0046ff, 0x0042ff, 0x003fff, 0x003bff, 0x0037ff, 0x0034ff, 0x0030ff,
                                 0x002cff, 0x0028ff, 0x0025ff, 0x0021ff, 0x001dff, 0x001aff, 0x0016ff, 0x0012fe, 0x000ffa, 0x000bf5,
                                 0x0007f1, 0x0003ec, 0x0000e8, 0x0000e3, 0x0000de, 0x0000da, 0x0000d5, 0x0000d1, 0x0000cc, 0x0000c8,
                                 0x0000c3, 0x0000bf, 0x0000ba, 0x0000b6, 0x0000b1, 0x0000ac, 0x0000a8, 0x0000a3, 0x00009f, 0x00009a,
                                 0x000096, 0x000091, 0x00008d, 0x000088, 0x000084, 0x00007f};

const uint32_t zajepaleta [256] = {3150395,3282243,3348554,3414865,3481176,3547487,3613798,3680109,3746419,3812729,3878784,3945094,4011403,4077713,4144023,4144796,4210850,4277159,4277932,4344241,4344757,4411066,4477375,4478147,4478663,4544971,4545743,4546259,4612566,4613338,4613853,4614624,4615139,4681446,4682217,4682731,4683502,4684016,4684786,4619764,4620534,4621048,4621818,4622331,4557564,4558077,4493310,4428286,4363519,4298495,4233727,4103166,4038398,3907837,3843068,3712507,3647738,3517432,3386871,3256309,3126004,3060978,2930672,2800110,2669803,2604777,2474215,2343908,2278882,2148319,2083293,2018266,1887704,1822933,1757906,1758416,1693133,1628106,1628616,1629125,1629634,1629888,1630397,1696443,1696697,1762486,1894068,1959858,2091439,2157228,2288554,2485415,2616996,2813857,2945182,3142043,3338904,3535764,3732625,3995022,4191882,4454279,4651140,4913280,5175677,5438074,5634678,5897075,6159471,6421612,6684009,6946150,7208546,7470687,7732828,7994969,8257366,8453971,8716113,8978254,9174859,9437001,9633607,9895492,10092098,10288704,10485055,10616125,10812476,11009082,11139897,11336504,11532855,11663670,11860022,12056373,12187189,12383540,12514356,12710708,12841268,13037620,13168436,13364532,13495348,13691444,13822261,13952821,14148917,14279734,14410294,14540855,14671671,14802231,14932792,15063352,15193913,15324473,15455033,15520058,15650618,15715642,15846202,15911226,16041786,16106810,16171834,16236858,16301625,16366649,16431673,16496696,16496183,16560950,16560438,16625205,16624692,16689459,16688946,16688177,16687408,16686639,16685869,16685356,16684587,16683818,16683049,16616743,16615974,16549669,16548899,16482594,16481825,16415519,16349214,16348445,16282140,16215834,16149529,16083224,16016919,15950613,15884308,15818003,15751954,15685649,15553808,15487759,15421454,15355405,15223564,15157516,15025931,14959882,14828298,14762249,14630664,14499080,14433031,14301447,14169862,14038278,13906693,13775109,13643525,13511940,13380356,13249028,13117443,12920323,12788995,12657410,12460290,12328962,12131842,12000514,11803393,11672065,11474945,11278081,11081217,10949633,10752769,10555905,10358785,10161921,9965057,9768193,9571073,9308673,9111810,8914946,8718082,8455682,8258818,7996419};


void wyswietl(uint32_t *paleta,uint8_t sposob, uint32_t * surowy)
{
    volatile uint32_t *wekran=(uint32_t*)0xD0000000;
   uint8_t luthist[16384]={0};
    uint32_t histogram[20000] = {0};
    //		uint32_t histogramz[16384] = {0};

    uint32_t chwilowa=0;
  volatile  uint32_t srednia=0;

    uint32_t niezerowa=0;
    uint32_t ostatnia=0;
    uint32_t niezeradr=0;
    uint32_t ostatniaadr=0;

    uint32_t gorka=0;
    uint32_t min=100000;
    uint32_t max=0;
    uint8_t f2=0;







        for(uint32_t i=20000; i<80000; i++)
        {
      //      histogram[(*(surowy+i))&0xffff]+=1;
    //        srednia+=((*(surowy+i))&0xffff);
            if(((*(surowy+i))&0x3fff)<min && ((*(surowy+i))&0x3fff)>2000)
                min=(*(surowy+i)&0x3fff);
            if(((*(surowy+i))&0x3fff)>max)
                max=(*(surowy+i)&0x3fff);
        }

       // srednia=srednia/80000;
      //  srednia+=1;

   /*     for(uint32_t i=1; i<16384; i++)
        {
            if(histogram[i]>300)
            {
                gorka+=histogram[i]-300;
                histogram[i]=300;
            }
            if((histogram[i]<8)&&(f2==0))
            {
                niezerowa=histogram[i];
                niezeradr=i;
            }
            else
                f2=1;
            if(histogram[i]>10)
            {
                ostatnia=histogram[i];
                ostatniaadr=i;
            }

        }

        switch (sposob)
        {
            case 0:
                if((ostatniaadr-niezeradr)<500)
                    ostatniaadr=niezeradr+500;
                break;
            case 1:
                if((ostatniaadr-niezeradr)<256)
                    ostatniaadr=niezeradr+256;
                break;
        }
        for(uint32_t h=niezeradr; h<ostatniaadr; h++)
        {
            histogram[h]+=(gorka/(ostatniaadr-niezeradr));
        }



        for(uint32_t i=0; i<16384; i++)
        {

            chwilowa+=histogram[i];
            histogram[i]=chwilowa;
        }


        for(uint32_t i=0; i<16384; i++)
        {
            //	 	 	 if(histogramz[i]<(histogramz[16383]/90))
            // 	 		 min=i;

            luthist[i]=255*(histogram[i])/(histogram[16383]);
            //luthist[i]=255*(histogramz[i]-niezerowa)/(ostatnia-niezerowa);

            //	lutpal[i]=zajepaleta[100];//[255*histogramz[i]/histogramz[16383]];
            // 	luthist[i]=255*(histogramz[i])/(histogramz[16383]);
        }

        //max+=100;

        int16_t kolor=0;
  */      for(uint16_t i=0; i<250; i++)
        {
            for(uint16_t x=0; x<325; x++)
            {

                switch (sposob)

                //*(uint32_t*)(dest + 4 * pixel + 480 * lines*4)=((cam_buffer_2[pixel + 325*lines])<<8)|0xFF;
                {
                    case 0:
    //                    *(uint32_t*)(wekran + x + 480 * i)= paleta[luthist[(surowy[x+325*i])&0xffff]];//((R<<16)+(G<<8)+B)|0xFF000000;
                        break;
                    case 1:
         //               kolor=250*(((surowy[x+325*i])&0xffff)-niezeradr)/(ostatniaadr-niezeradr);
         //               if (kolor<1) kolor=0;
         //               if(kolor>254) kolor=255;
//                        *(uint32_t*)(wekran + 4 * x + 480 * i*4)= paleta[kolor]|0xFF000000;

                        //if(buffer_index != 0)
                        {
//*(uint32_t*)(wekran )= paleta[(surowy[x+325*i]-min)];
*(uint32_t*)(wekran )= paleta[(255*((*(surowy+x+325*i))-min)/(max-min))%255];
                        }
                        break;
                }

            wekran+=1;
            }
         wekran+=155;
        }

//        for(uint16_t x=0; x<5; x++)
//        {
//
//            *(wekran+(480*x)+1926+(16383/35))=LCD_COLOR_GREEN;
//            *(wekran+(480*x)+1920+(srednia/35))=LCD_COLOR_RED;
//        }
        wekran+=480*5;
//        for(uint16_t x=0; x<5; x++)
//        {
//
//            //										    *(wekran+(480*x)+1926+(16383/35))=LCD_COLOR_GREEN;
//            //									*(wekran+(480*x)+1920+(srednia/35))=LCD_COLOR_RED;
//            //	*(wekran+(480*x)+1921+(srednia/35))=LCD_COLOR_RED;
//            //	*(wekran+(480*x)+1920+(niezeradr/35))=LCD_COLOR_GREEN;
//            *(wekran+(480*x)+1921+(niezeradr/35))=LCD_COLOR_GREEN;
//            //	*(wekran+(480*x)+1920+(ostatniaadr/35))=LCD_COLOR_CYAN;
//            *(wekran+(480*x)+1921+(ostatniaadr/35))=LCD_COLOR_CYAN;
//        }


    }
//}

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

volatile uint32_t wlacznik = 1;

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
        *(uint32_t*)(0xD0000000 + i * 4) = i < 350 ? 0xFFFF0000 : 0xFF00FF00;
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
//        for(int lines=0; lines <= 256; lines++)
//        {
//            for(int pixel=0; pixel < 325; pixel++)
//            {
////                while (buffer_index==0);
////                *(uint32_t*)(dest + 4 * pixel + 480 * lines*4)=((cam_buffer[pixel + 325*lines])<<8)|0xFF;
//
//            if(buffer_index == 0)
//            {
////                *(uint32_t*)(dest + 4 * pixel + 480 * lines*4)=((cam_buffer_2[pixel + 325*lines])<<8)|0xFF;
//            } else
//            {
//                *(uint32_t*)(dest + 4 * pixel + 480 * lines*4)=((cam_buffer[pixel + 325*lines])<<8)|0xFF;
//            }
//            }
//
//
//        }
        wyswietl(paleta1,1,cam_buffer);
//HAL_Delay(33);
//        wlacznik = 0;

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
  pLayerCfg.WindowX1 = 266;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 325;
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
