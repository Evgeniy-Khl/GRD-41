/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : GRD 4.0 inch 22.01.2025
  ******************************************************************************
  *
  * Program Size: Code=31148 RO-data=11208 RW-data=232 ZI-data=2584  
  * certutil -hashfile e:\!PROJECTS\!STM32\2025\GRD_40\MDK-ARM\GRD_40\GRD_40.hex SHA1
  * d4f86b9297aaa9c98f087b2d9cfff38c926e7753
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "..\Lib\st7796\myLCD.h"
#include "..\Lib\st7796\myGUI.h"
#include "..\Lib\Touch\XPT2046_touch.h"
#include "..\Lib\ds18b20\ds18b20.h"
#include "tft_proc.h"
#include "procedure.h"
#include "displ.h"
#include "nvRam.h"

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
CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

char buffTFT[40];
const char* modeName[4]={"�������","�������","�������","��������"};
const char* setName[MAX_SET]={"t ������","t ��������","t ����","����������","���������","����.ON","����.OFF","����"};
const char* otherName[MAX_OTHER]={"����������","������","��������","�������.","Prop","Integ","Diff"};
const char* relayName[7]={"���","������","������","������","�������","��.����","��.����"};
//const char* analogName[2]={"������.","����"};
//        2.00V        3.15V        4.30V        5.45V        6.60V        7.75V        8.90V        10.00V
//={{1000,0x2F4},{1200,0x4A6},{1400,0x658},{1600,0x80A},{1800,0x9BC},{2000,0xB6E},{2200,0xD20},{2400,0xFFF}};//d=434->1.15V
//={{1000,0x2F4},{1200,0x4A6},{1400,0x655},{1600,0x804},{1800,0x9B6},{2000,0xB65},{2200,0xD14},{2400,0xFFF}};//d=434+���������
struct Ds ds;
uint16_t speedData[MAX_SPEED][2], errors, arhCount, arhErrors[15];
int16_t pvTH, pvRH, tmrCounter;
uint16_t set[INDEX], touch_x, touch_y, Y_str, X_left, Y_top, Y_bottom, fillScreen, color0, color1, checkTime, checkSmoke;
uint8_t displ_num=0, modeCell, oldNumSet, buttonAmount, lost;
uint8_t timer10ms, tmrVent, ticBeep, pwTriac, invers, dsplPW;
uint8_t familycode[MAX_SENSOR][8];
int8_t ds18b20_amount, numSet=0, resetDispl=0, tmrWater;
int8_t relaySet[8]={-1,-1,-1,-1,-1,-1,-1,-1};
int8_t analogSet[2]={-1,-1};
uint8_t analogOut[2]={0};
union Byte portFlag;
union Byte relayOut;
PIDController pid;
//#ifdef MANUAL_CHECK
  float flT0=320, dpv0;
//#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM1_Init(void);
static void MX_CRC_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//-------- �������� ����� � �������� �������� --------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance == TIM1){ //check if the interrupt comes from TIM1 (10 ms)
    checkTime++; timer10ms++;
    if(pwTriac) --pwTriac; else {
      TRIAC = OFF;                    // ��������� (SSR-25DA)
      invers = ~relayOut.value;
      HAL_I2C_Master_Transmit(&hi2c1,0x4E,&invers,1,1000);
    }
    if(ticBeep){ --ticBeep; HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);}// ��������� �������
    else {HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);}
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  int16_t i16;
  uint16_t u16;
//  uint8_t temp=0, pvspeed=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_CRC_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  u16 = sendToI2c(0);//  ���������� �����������
  
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);

  fillScreen = BLACK; color0 = WHITE;
  Y_bottom=lcddev.height-22; Y_str = 5;
  LCD_Init(USE_VERTICAL0);
  GUI_Clear(fillScreen);
  if((lcddev.dir&1)==0) X_left = 20; else X_left = 100;
  GUI_WriteString(35, Y_str, "GRD Max", Font_16x26, WHITE, fillScreen);
  GUI_WriteString(165, Y_str+5, " v 4.1", Font_11x18, WHITE, fillScreen);
  Y_str = Y_str+18+35;
  
  i16 = initData();
  ds18b20_port_init();      // ����� 1-Wire
  ds18b20_checkSensor(4);   // check DS18B20 sensors
 
  switch (i16){
  	case 0: GUI_WriteString(5, Y_str, "������������� �������.", Font_11x18, GREEN, BLACK);	break;
  	case 1: GUI_WriteString(5, Y_str, "�������� �������������.", Font_11x18, YELLOW, BLACK);	break;
    case 3: GUI_WriteString(5, Y_str, "������� ������� FLASH!", Font_11x18, YELLOW, RED);	break;
  	default:GUI_WriteString(5, Y_str, "�������� �������!", Font_11x18, MAGENTA, BLACK);	break;
  }
  Y_str = Y_str+18+5;
  
  if(i16){
    sprintf(buffTFT,"Check sum: 0x%08X",dataRAM.config.checkSum);
    GUI_WriteString(5, Y_str, buffTFT, Font_11x18, WHITE, BLACK);
    Y_str = Y_str+18+5;
    sprintf(buffTFT,"Number of saves: %u",dataRAM.config.countSave);
    GUI_WriteString(5, Y_str, buffTFT, Font_11x18, WHITE, BLACK);
    Y_str = Y_str+18+5;
    HAL_Delay(1000);
  }
  //---------------------------- ����� 1-Wire -----------------------------------
  if(ds18b20_amount){
    for(uint8_t i=0;i<ds18b20_amount;i++) ds.pvT[i]=1999;
    ds18b20_Convert_T();
  }
  sprintf(buffTFT,"�������� �����������: %d ��.",ds18b20_amount);
  GUI_WriteString(5, Y_str, buffTFT, Font_11x18, CYAN, BLACK);
  Y_str = Y_str+18+5;
  
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);
  
  HAL_RTCEx_SetSecond_IT(&hrtc);          /* ------  ������ 1��.  ������  1 ���.    ----*/
  HAL_TIM_Base_Start_IT(&htim1);          /* ------  ������ 100��.  ������  10 ��.  ----*/
  
  NEWBUTT = ON;
  #ifdef MANUAL_CHECK
      sprintf(buffTFT,"WIDTH: %u; HEIGHT: %u",lcddev.width,lcddev.height);
      GUI_WriteString(5, Y_str, buffTFT, Font_11x18, YELLOW, BLACK);
      Y_str = Y_str+18+5;
      sprintf(buffTFT,"Kp=%3u; Ki=%2.2f; Kd=%3u;",pid.Kp,pid.Ki,pid.Kd);
      GUI_WriteString(5, Y_str, buffTFT, Font_11x18, YELLOW, BLACK);
      Y_str = Y_str+18+5;
      ds.pvT[1]=220; ds.pvT[2]=150; ds.pvT[3]=200;
      int8_t dpv1 = 2, dpv2 = 2, dpv3 = 2, count;
      
  #endif
  HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Y_str = 5; X_left = 5;
    //-------------------------- �������� ---------------------------------------
    if(XPT2046_TouchPressed()&& checkTime>40){
      uint8_t butt_num;
      if(XPT2046_TouchGetCoordinates(&touch_x, &touch_y)){
        for (butt_num=0; butt_num<buttonAmount; butt_num++){
            if(contains(touch_x, touch_y, butt_num)) break; // �������� ��������� ����� ���������� � ������� ������
        }
      }
      checkButtons(butt_num);                           // �������� ������� ������
      checkTime = 0; CHECK = ON;
    }
    
    // ----------- ����������� ������ � ������ ����� modeCell==2 � TMON && TMOFF !=0 --------------------
    if(WORK && modeCell==2){
      if(set[TMON]!=0 && set[TMOFF]!=0){
        if(timer10ms){                  // ��� ������� ��������� ������� 10 �������.
          timer10ms=0; 
          HUMIDI=humidifier(HUMIDI);    // �������� ����� �� �����������
          invers = ~relayOut.value;
          HAL_I2C_Master_Transmit(&hi2c1,0x4E,&invers,1,1000);
        }
      }
    }
    
    //-------------- ������ �������� ������ 1 ���. -----------------------
    if(CHECK){ CHECK = OFF; errors=0;  //if(++temp>10) {temp=0; ++pvspeed; pvspeed&=7; ds.pvT[1] = speedData[pvspeed][0]; sendToI2c(speedData[pvspeed][1]);}
    dsplPW = 0;  
    if(resetDispl) --resetDispl; else if(displ_num){displ_num = 0; NEWBUTT = 1;}  // ������� � �������� �������
    #ifndef MANUAL_CHECK
      temperature_check();
    #endif
      //---------------------------------- �������� ������ ����������� -------------------------------------
      if(VENTIL){
        if(HAL_GPIO_ReadPin(Input0_GPIO_Port, Input0_Pin) == GPIO_PIN_RESET) {SPEED=ON; tmrVent=0;} // ���� ������� �������
        else SPEED=OFF;
    #ifdef MANUAL_CHECK
        SPEED=ON; tmrVent=0;
    #endif
        if(tmrVent) --tmrVent;    // ������� ��������� �������� ���������� ���������������
        else if(SPEED) WORK=ON;
        else {errors |= ERR8; WORK=OFF; relayOut.value=OFF;}  // �� ������ ����������
      }
      //------------------------------------------- � ������ -----------------------------------------------
      if(WORK){
        TIMER=ON;         // ������ ������� � ������
        if(modeCell==2){  // ������ � ������ ����� modeCell==2
          if(HAL_GPIO_ReadPin(Input1_GPIO_Port, Input1_Pin) == GPIO_PIN_RESET){
            if(++tmrWater>5) {tmrWater=5; WATER=ON;}
          }
          else {
            if(--tmrWater<0) {tmrWater=0; WATER=OFF;}
          }
        }
        else WATER=OFF;
        //------------ ������������� color0 � ����������� � ����������� ------------------------
        i16 = set[T0]*10 - ds.pvT[0];           // �������� ������ ������������� ������� 0
        uint16_t abs16 = abs(i16);
        if(abs16<set[HIST]) PERFECT=ON;         // ����� �� �������� �����������
        u16 = set[ALRM]*10;                     // �������� � ������
        
        if(i16<=0){
          if(abs16<u16) color0 = GREEN;         // �����
          else if(abs16>=u16 && abs16<u16*2){errors|=ERR5; color0 = MAGENTA;} // ²�ղ����� �����������
          else {errors|=ERR3; color0 = RED;}    // �������� � ������
        }
        else {
          if(abs16<u16) color0 = GREEN;         // �����
          else if(abs16>=u16*2){
            color0 = CYAN;                      // ���� �����
            if(PERFECT) errors|=ERR5;           // ²�ղ����� �����������
          }
        }
        
        //------------ ������������� color1 � ����������� � ����������� -------------------------
        i16 = set[T1]*10 - ds.pvT[1];           // �������� ������ ������������� ������� 1
        abs16 = abs(i16);
        
        if(i16<=0){
          if(abs16<u16/2)color1 = GREEN;        // �����
          else if(abs16>=u16/2 && abs16<u16) color1 = MAGENTA;  // ²�ղ����� �����������
          else {errors|=ERR4; color1 = RED;}    // �������� � ��������
        }
        else {
          if(abs16<u16/2)color1 = GREEN;        // �����
          else if(abs16>=u16) color1 = CYAN;    // ���� �����
        }
        
        // ---------------------------------------- ����������� / ���������� -------------------------------------
        //------ �������� ��� �����������
        if(ds.pvT[0]<1999 && ds.pvT[0]>1){
          i16 = Relay(set[T0]*10 - ds.pvT[0], set[HIST]);   // �������� ������ ����������� �������
          pwTriac = UpdatePID(&pid,0);                      // ��� �����������
        }
        if(ds18b20_amount>1 && ds.pvT[1]<1999){             // �������� ������ ����������s �����
          if(i16==ON) i16 = Relay(set[T1]*10 - ds.pvT[1], set[HIST]/2);
//          if(i16==OFF) pwTriac = OFF;                       // ����������� ����� �������� �������� ��������
        }
        dsplPW = pwTriac;
        if(pwTriac) TRIAC = ON;                             // �������� (SSR-25DA)
        //------ �������� ��� ����������
        if(set[CHILL]&1){  
          i16 = Relay(ds.pvT[0] - set[T0]*10, set[HIST]);
          if(ds.pvT[0] > BEGINCOOL) i16 = OFF;              // ����������� ���� ������� ��������� ��������� ����������
        }
        switch (i16){
          case ON:  HEATER = ON;  break;
          case OFF: HEATER = OFF; break;
        }
        
        //-------------------------- ������ ��� ������ �������� ---------------------------------
        if(modeCell==3){
          ELECTRO = ignition(ELECTRO);
          i16 = set[T2]*10 - ds.pvT[2];   // �������� ������ ������������� ������� 2 (���)
          if(++checkSmoke>CHKSMOKE){      // (��������� 2 ���.�) ����������� ����
            checkSmoke=CHKSMOKE;
            if(abs(i16)>set[ALRM]*10*2) errors|=ERR6;
          }
          u16 = Relay(i16, set[HIST]);  // �������� ������ ����������� ����
          switch (u16){
            case ON:  SMOKE = ON;  break;
            case OFF: SMOKE = OFF; break;
          }
        }
        
        //-------------------------- ������ ��� ������ ������� ---------------------------------
        if(modeCell==2){
          if(set[TMON]==0 || set[TMOFF]==0){
            i16 = set[T3]*10 - ds.pvT[3];     // �������� ������ ������������� ������� 3 (���������)
            u16 = Relay(i16, set[HIST]);      // ��������� ������ - ������ ���������
            if(ds.pvT[0] < BEGINHUM) u16=OFF; // ������ ���������� ��� ����������� ���� 40 ���.
            switch (u16){
              case ON:  HUMIDI = ON;  break;
              case OFF: HUMIDI = OFF; break;
            }
          }
        }
//        else HUMIDI = OFF;

#ifdef MANUAL_CHECK
        //?????? ���������� ������� ���������� ??????????
        count++;
        //-----����������� �������------
        dpv0 = (float)pid.pPart/500 + (float)(pid.output-5)/100;
        flT0+=dpv0;
        ds.pvT[0] = flT0;
        int16_t pverr = set[T0]*10 - ds.pvT[0];
        //----����������� �����------
        if(count>3){ count=0;
          pverr = set[T1]*10 - ds.pvT[1];
          dpv1 =-1;
          if(pverr>200) dpv1 = 6;
          else if(pverr>100) dpv1 = 4;
          else if(pverr>50) dpv1 = 2;
          else if(pverr>10) dpv1 = 1;
          ds.pvT[1]+=dpv1;
        }
        //-----����������� ����---------
        pverr = set[T2]*10 - ds.pvT[2];
        if(pverr>50) dpv2 = 5;
        else if(pverr>25) dpv2 = 1;
        else if(pverr<-25) dpv2 = -1;
        if(i16==OFF) dpv2=0;
        ds.pvT[2]+=dpv2;
        //----������� ������--------
        pverr = set[T3]*10 - ds.pvT[3];
        if(pverr>150) dpv3 = 5;
        else if(HUMIDI==ON) dpv3 = 1;
        else if(HUMIDI==OFF) dpv3 = -1;
        ds.pvT[3]+=dpv3;
        //????????????????????????????????????????????????
#endif
        
        //------------------------- ��������� �������� ������ ----------------------------------
        if(set[TMR0]>0){                                // ���� TMR0>0 �� ���������� ������ �� �������
          u16 = sTime.Hours*60 + sTime.Minutes;         // ����� � �������
          i16 = (set[TMR0] - u16)*60 - sTime.Seconds;   // �������� �� ���������� � ��������
          if(i16<30) ticBeep = 5;                       // �������� ������
          if(i16<=0){                                   // ���������� ������
            portFlag.value = OFF; PURGING=ON; relayOut.value=OFF; ticBeep=200;
            //------- ����� �������� ---------
              sTime.Hours=0; sTime.Minutes=0; sTime.Seconds=0;
              HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
          }
        }
        else if(ds18b20_amount==1){      // ���� ������ 1 ������ � ����������������� 0 �� ���������� �� ����������� ������.          
          i16 = Relay(set[T0]*10 - ds.pvT[0], 0);   // ����������� ������
          if(i16==OFF){
            portFlag.value = OFF; PURGING=ON; relayOut.value=OFF; ticBeep=200;
            //------- ����� �������� ---------
            sTime.Hours=0; sTime.Minutes=0; sTime.Seconds=0;
            HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
          }
        }
        else if(ds18b20_amount>1){      // ���� �������� ����� � ����������������� 0 �� ���������� �� ����������� �����.          
          i16 = Relay(set[T1]*10 - ds.pvT[1], 0);   // ����������� ������
          if(i16==OFF){
            portFlag.value = OFF; PURGING=ON; relayOut.value=OFF; ticBeep=200;
            //------- ����� �������� ---------
            sTime.Hours=0; sTime.Minutes=0; sTime.Seconds=0;
            HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
          }
        }
        
      } //--------------------------- ����� � ������ -----------------------------------------------
      else if(PURGING){
        u16 = sTime.Minutes*60+sTime.Seconds;           // ����� � ��������
        if(u16>=set[TMR1]) {PURGING=OFF; sendToI2c(0); NEWBUTT=ON; ticBeep=200;}
      }
      //------ �������� �� ������ ���������� ---------------------------------------
      for (i16=0;i16<7;i16++){
          if(relaySet[i16]==1) relayOut.value |= (1<<(i16)); // ������ On
          if(relaySet[i16]==0) relayOut.value &= ~(1<<(i16));// ������ Off
      }
      invers = ~relayOut.value;
      HAL_I2C_Master_Transmit(&hi2c1,0x4E,&invers,1,1000);
//      for (i16=0;i16<2;i16++){  // ������ ���������� ����������� ��������
//        if(analogSet[i16]>-1) analogOut[i16]=analogSet[i16];
//      }
      if(errors && (set[CHILL]&2)==0){    // 2-��������� ��������� �������� �������
        arhErrors[arhCount] = errors;
        if(++arhCount>15) arhCount = 0;
        switch (errors){
          case 0x01: ticBeep = 80; break; // ������� ������� N1
          case 0x02: ticBeep = 80; break; // ������� ������� N2
          case 0x04: ticBeep = 80; break; // ������� ������� N3
          case 0x08: ticBeep = 80; break; // ������� ������� N4
          case ERR3: ticBeep = 80; break; // �������� � �����I
          case ERR4: ticBeep =120; break; // �������� � �������I
          case ERR5: ticBeep = 10; break; // ���������� �����������
          case ERR6: ticBeep = 20; break; // ���������� ����������� ���A
          case ERR7: ticBeep = 60; break; //
          case ERR8: ticBeep = 60; break; // �� ������ ����������
          default: 
            if(errors==0x0C) ticBeep = 80;
            else ticBeep =200;
          break;
        }
      }
      if(errors) ALARM = ON; else ALARM = OFF;  // �������� ������ ������

      display();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 719;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, T_CS_Pin|TFT_DC_Pin|TFT_CS_Pin, GPIO_PIN_RESET);
  
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TFT_RST_Pin|Beep_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Input0_Pin */
  GPIO_InitStruct.Pin = Input0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Input0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : T_IRQ_Pin */
  GPIO_InitStruct.Pin = T_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(T_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : T_CS_Pin TFT_CS_Pin */
  GPIO_InitStruct.Pin = T_CS_Pin|TFT_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Pin TFT_DC_Pin */
  GPIO_InitStruct.Pin = LED_Pin|TFT_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OneWR_Pin Input1_Pin */
  GPIO_InitStruct.Pin = OneWR_Pin|Input1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_RST_Pin */
  GPIO_InitStruct.Pin = TFT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_DC_Pin */
  GPIO_InitStruct.Pin = TFT_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Beep_Pin */
  GPIO_InitStruct.Pin = Beep_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Beep_GPIO_Port, &GPIO_InitStruct);
  
/* USER CODE BEGIN MX_GPIO_Init_2 */
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
