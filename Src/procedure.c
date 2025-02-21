#include "main.h"
#include "..\Lib\st7796\myLCD.h"
#include "procedure.h"
#include "nvRam.h"
#include "rtc.h"

extern I2C_HandleTypeDef hi2c1;
extern uint16_t set[INDEX], color0, color1, checkSmoke;
extern int16_t pvRH, tmrCounter;
extern uint16_t speedData[MAX_SPEED][2];
extern uint8_t familycode[MAX_SENSOR][8], ds18b20_amount, ticBeep, errors, tmrVent;

union b2{
    uint16_t val;
    uint8_t data[2];
  } mcp;

void startPrg(void)
{
  if(WORK|VENTIL|PURGING){
    portFlag.value = OFF; CHECK = ON; NEWBUTT=ON;   // ���� ��� � ������ - ��� ���������.
    sendToI2c(0);
    relayOut.value=OFF; color0 = WHITE; color1 = WHITE; ticBeep=100;
  }
  else {          // ����� ������� ������ ����
    VENTIL=ON; sendToI2c(speedData[set[VENT]][1]); tmrVent=20;// 20 ���. �������� ������� �����������
    ticBeep=100; errors=0; tmrCounter=2; checkSmoke=0; // (2���.) ������������ �������� �������� ������ 0
//    if(set[TMR0]){INSIDE=OFF;}
//    else if(ds18b20_amount>1) INSIDE=ON; // ���� ���� ������ ������������� ������ �� ����������� ��������.
    sTime.Hours=0; sTime.Minutes=0; sTime.Seconds=0;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  }
}

uint8_t Relay(int16_t err, uint8_t hst) // [n] ����� � 1 ��� 2
{
  uint8_t x=2;
  if(err>hst) x = ON;         // ��������
  if(err <= 0) x = OFF;       // ���������
  return x;
}

uint8_t humidifier(uint8_t value){
   if(value){
      if(tmrCounter) tmrCounter--; else {value=OFF; tmrCounter=set[TMOFF]*10;} // ������������ ����� 18*10=180*10msek.=1.8sek
   }
   else {
      if(tmrCounter) tmrCounter--; else {value=ON; tmrCounter=set[TMON]*10;}  // ������������ ������� 6*10=60*10msek.=0.6sek
   }
  if(ds.pvT[0] < BEGINHUM) value=OFF;        // ������ ���������� ��� ����������� ���� 40 ���.
  return value;
}

uint8_t ignition(uint8_t value){
  if(tmrCounter>=0){
    if(value){
        if(tmrCounter) tmrCounter--; else {value=OFF; tmrCounter=-1;}         // ������ �� ����������
    }
    else {
        if(tmrCounter) tmrCounter--; else {value=ON; tmrCounter=set[TMON];}   // ������������ ������� 180���.
    }
  }
  else value=OFF;
  return value;
}

//uint8_t UpdatePID(uint8_t cn){
// int err;
// float pPart, Ud;
// static float iPart;
//  err = set[cn]*10 - ds.pvT[cn];
//  pPart = (float) err * dataRAM.config.koff[0];                  // ������ ���������������� �����
////---- ������� ����������� pPart ---------------
//  if (pPart < 0) pPart = 0;
//  else if (pPart > 100) pPart = 100;             // ������� �����������
////----------------------------------------------
//  iPart += (float) dataRAM.config.koff[0] / dataRAM.config.koff[1] * err;      // ���������� ������������ �����
//  Ud = pPart + iPart;                            // ����� ���������� �� �����������
////---- ������� ����������� Ud ------------------
//  if (Ud < 0) Ud = 0;
//  else if (Ud > 100) Ud = 100;                   // ������� �����������
//  iPart = Ud - pPart;                            // "��������������" ��������
//  err = Ud;
//  return err;
//};

uint8_t UpdatePID(PIDController *pid, uint8_t cn){
 int16_t error, derivative;
  // ���������� ������
  error = set[cn]*10 - ds.pvT[cn];
  // ���������������� ������������
  pid->pPart = error * pid->Kp;
  // ������������ ������������
  pid->iPart += (float)error * pid->Ki;// * dt;
  // ���������������� ������������
  derivative = (error - pid->prev_error);// / dt;
  pid->dPart = pid->Kd * derivative;
  // ��������� ������� ������ ��� ���������� ������
  pid->prev_error = error;
  // ��������� ����������� �����������
  pid->output = pid->pPart + pid->iPart + pid->dPart;
  // ����������� ��������� �������� � �������������
  if (pid->output > 100) pid->output = 100;
  else if (pid->output < 0) pid->output = 0;
  if (pid->pPart >= 100) pid->iPart = 0; // ����� ���������
  else if (pid->pPart <= -100) pid->iPart = 0; // ����� ���������

  error = pid->output;
  return error;
}

void permutation (char a, char b){
  uint8_t i, buff;
  for (i=0;i<9;i++) {
     buff = familycode[a][i];
     familycode[a][i] = familycode[b][i];
     familycode[b][i] = buff; 
  };
}

uint8_t sendToI2c(uint16_t val){
  uint8_t u8arr[3];
  mcp.val = val;
  mcp.val <<= 4;
  u8arr[0] = 0x40;
  u8arr[1] = mcp.data[1];
  u8arr[2] = mcp.data[0];
  val = HAL_I2C_Master_Transmit(&hi2c1,0xC0,u8arr,3,1000);
  return val;
}

