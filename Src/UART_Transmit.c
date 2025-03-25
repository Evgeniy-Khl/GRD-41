#include "main.h"
#include "procedure.h"

#define SRT_MARKER	0xDD	// Начало блока
#define RESP		    0xEE	// Ответ блока
#define SPRG		    0xAA	// Начало блока программы

#define NO_COMM     0
#define SEND_RAM	  0x40    // 64
#define SET_EEPROM	0x42    // 66

extern union Byte portFlag;
extern struct Ds ds;
extern uint16_t set[];
extern uint16_t speedData[][2];
extern uint8_t modeCell, dsplPW, command;
extern uint8_t errors, first, second, countUart;
extern const char* modeName[];
extern UART_HandleTypeDef huart1;
extern RTC_TimeTypeDef sTime;

void transmitData(UART_HandleTypeDef *huart);


extern uint8_t RXBuffer[], myIp[];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  uint8_t i;
  if (huart == &huart1) {
    // Здесь обрабатываем принятые данные из RXBuffer
    first = RXBuffer[0]; second = RXBuffer[1]; countUart++; // Receive 2 bytes
    if(command==NO_COMM){
        if(first==SRT_MARKER) {command = second;}
        else if(second==SRT_MARKER) {command = first;}
    }
    switch (command){
        case SEND_RAM: 
          transmitData(&huart1);
          for (i=0; i<6; i++) myIp[i] = RXBuffer[i+2];
          command = NO_COMM;
          break;   
        case SET_EEPROM:  
          for (i = 0; i < 12; ++i) {
          // Предполагаем little-endian порядок байтов (младший байт первым)
          set[i] = (uint16_t)RXBuffer[2 * (i+1)] | ((uint16_t)RXBuffer[2 * (i+1) + 1] << 8);
          }
          if (RXBuffer[27]==1) setON(); 
          else if(RXBuffer[27]==0) setOFF();
          command = NO_COMM;
          break;   
    }
    
    // Запускаем прием следующей порции данных
    HAL_UART_Receive_IT(&huart1, RXBuffer, 28);
  }
}

// Функция для передачи данных по UART
void transmitData(UART_HandleTypeDef *huart) {
    uint8_t buffer[48], i, crc=0; // Буфер для хранения данных перед отправкой
    uint8_t *ptr = buffer; // Указатель для записи в буфер

    *ptr++ = SRT_MARKER;    // Начало блока  (1 байт)
    *ptr++ = 0x02;          // 1 байт ind=0  модель прибора
    *ptr++ = 0;             // 1 байт ind=1  сетевой номер прибора
    *ptr++ = modeCell;      // 1 байт ind=2  номер режима
    *ptr++ = portFlag.value;// 1 байт ind=3  Flags
                            // 8 байт ind=4-ind=11 ds.pvT[0] - ds.pvT[3]
    for (i = 0; i < 4; i++){
      *ptr++ = (uint8_t)(ds.pvT[i] & 0xFF); // Младший байт  
      *ptr++ = (uint8_t)(ds.pvT[i] >> 8); // Старший байт
    }
                            // 24байт ind=12-ind=35 Установки
    for (i = 0; i < 12; i++){
      *ptr++ = (uint8_t)(set[i] & 0xFF); // Младший байт
      *ptr++ = (uint8_t)(set[i] >> 8); // Старший байт
    }
    *ptr++ = set[VENT];     // 1 байт ind=36 скорость вращения вентилятора
    *ptr++ = relayOut.value;// 1 байт ind=37 активные выходы реле
    *ptr++ = dsplPW;        // 1 байт ind=38 мощность подаваемая на тены
    *ptr++ = errors;        // 1 байт ind=39 ошибки
    *ptr++ = sTime.Hours;   // 1 байт ind=40 часы
    *ptr++ = sTime.Minutes; // 1 байт ind=41 минуты
    *ptr++ = sTime.Seconds; // 1 байт ind=42 секунды
//    buffer[0] = SRT_MARKER;
//    for (i=1; i<47; i++){
//      buffer[i] = i;
//    }
    for (i=1; i<47; i++){
      crc ^= buffer[i];
    }
    buffer[47] = crc;
    // Отправка данных по UART
    HAL_UART_Transmit(huart, buffer, 48, 1000);
}

// Пример использования (в вашем основном коде)

