/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Input0_Pin GPIO_PIN_0
#define Input0_GPIO_Port GPIOA
#define T_IRQ_Pin GPIO_PIN_3
#define T_IRQ_GPIO_Port GPIOA
#define T_CS_Pin GPIO_PIN_4
#define T_CS_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_7
#define LED_GPIO_Port GPIOA
#define OneWR_Pin GPIO_PIN_11
#define OneWR_GPIO_Port GPIOB
#define TFT_RST_Pin GPIO_PIN_12
#define TFT_RST_GPIO_Port GPIOB
#define TFT_DC_Pin GPIO_PIN_12
#define TFT_DC_GPIO_Port GPIOA
#define TFT_CS_Pin GPIO_PIN_15
#define TFT_CS_GPIO_Port GPIOA
#define Beep_Pin GPIO_PIN_6
#define Beep_GPIO_Port GPIOB
#define Input1_Pin GPIO_PIN_7
#define Input1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define DIAGONAL    28          // 24 -> ��� �������� 2,4"; 28 -> ��� �������� 2,8"; 32 -> ��� �������� 3,2"
#define TOUCHMODE   0           // 0 ��� 1
#define MAX_SENSOR  4
#define MAX_MODE    4
#define MAX_SET     8
#define MAX_OTHER   7
#define MAX_SPEED   8
#define ON          1
#define OFF         0

#define T0    0 // ������� T1 ���. 
#define T1    1 // ������� T2 ���. 
#define T2    2 // ������� T3 ���. (���)
#define T3    3 // ������� T4 ���. (�������)
#define TMR0  4 // ������������ ������ ���.
#define VENT  5 // �������� ����������� %
#define TMON  6 // ������ ON ���.
#define TMOFF 7 // ������ OFF ���.
#define TMR1  8 // ������������ �������� ���.
#define ALRM  9 // ������ ���.
#define HIST  10 // ���������� ���/10
#define CHILL 11 // ����������

#define ERR1  0x0010  //
#define ERR2  0x0020  //
#define ERR3  0x0040  // �������� � �����I
#define ERR4  0x0080  // �������� � �������I
#define ERR5  0x0100  // �I��I����� ����������� � �����I
#define ERR6  0x0200  // �I��I����� ����������� ���A
#define ERR7  0x0400  //
#define ERR8  0x0800  // �� ������ ����������





//#define MANUAL_CHECK

#ifdef MANUAL_CHECK
  #define CHKSMOKE  180 // (3 min.) waiting for smoke temperature check in sec.
#else
  #define CHKSMOKE  1500 // (25 min.) waiting for smoke temperature check in sec.
#endif
#define BEGINCOOL 400 // ����������� 40 ���. ���� ������� ��������� ��������� ����������
#define BEGINHUM  400 // ������ ���������� ��� ����������� ���� 40 ���.


/* ---��������� � �������� ������ -----*/
struct byte {
    unsigned a0: 1;
    unsigned a1: 1;
    unsigned a2: 1;
    unsigned a3: 1;
    unsigned a4: 1;
    unsigned a5: 1;
    unsigned a6: 1;
    unsigned a7: 1;
};
 
union Byte {
    unsigned char value;
    struct byte bitfield;
};

struct Ds{
  int16_t pvT[MAX_SENSOR];
  uint8_t err[MAX_SENSOR];
};

extern struct Ds ds;

typedef struct {
    float Ki, iPart;  // ������������ PID
    int32_t pPart, dPart, prev_error, output;
    uint16_t Kp, Kd;
} PIDController;

extern PIDController pid;

#define CHECK   portFlag.bitfield.a0  // Start of all checks
#define SPEED   portFlag.bitfield.a1  // Speed Ok.
#define WORK 	  portFlag.bitfield.a2  // At work flag
#define NEWBUTT portFlag.bitfield.a3  // New screen flag
#define VENTIL	portFlag.bitfield.a4  // Fan speed flag
#define PERFECT	portFlag.bitfield.a5  // �������� �������� �����������
#define RESERVE portFlag.bitfield.a6  // ������
#define PURGING portFlag.bitfield.a7  // ��������

#define TRIAC   relayOut.bitfield.a0  // SSR-25DA
#define HEATER  relayOut.bitfield.a1  // �����������
#define TIMER 	relayOut.bitfield.a2  // ������
#define HUMIDI	relayOut.bitfield.a3  // �����������
#define ELECTRO	relayOut.bitfield.a4  // �������������
#define SMOKE   relayOut.bitfield.a5  // ������ ����
#define WATER 	relayOut.bitfield.a6  // ������ ����
#define ALARM   relayOut.bitfield.a7  // �������

extern union Byte portFlag;
extern union Byte relayOut;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
