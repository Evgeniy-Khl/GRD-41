#include "main.h"

extern union Byte portFlag;
extern struct Ds ds;
extern uint16_t set[];
extern uint16_t speedData[][2];
extern uint8_t modeCell, dsplPW;
extern uint8_t errors;
extern const char* modeName[];
extern UART_HandleTypeDef huart1;
extern RTC_TimeTypeDef sTime;

uint8_t RXBuffer[2];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart1) {
    // Здесь обрабатываем принятые данные из RXBuffer
    uint8_t first, second;
    first = RXBuffer[0]; second = RXBuffer[1];  // Receive 2 bytes
    
    // Запускаем прием следующей порции данных
    HAL_UART_Receive_IT(&huart1, RXBuffer, 2);
  }
}

// Функция для передачи данных по UART
void transmitDataUART(UART_HandleTypeDef *huart) {
    uint8_t buffer[46], i; // Буфер для хранения данных перед отправкой
    uint8_t *ptr = buffer; // Указатель для записи в буфер

    *ptr++ = 0x02;          // 1 байт ind=0  модель прибора
    *ptr++ = 0;             // 1 байт ind=1  сетевой номер прибора
    *ptr++ = modeCell;      // 1 байт ind=2  номер режима
    *ptr++ = portFlag.value;// 1 байт ind=3  Flags
                            // 8 байт ind=4-ind=11 ds.pvT[0] - ds.pvT[3]
    for (i = 0; i < 4; i++){
        *ptr++ = (uint8_t)(ds.pvT[i] >> 8); // Старший байт
        *ptr++ = (uint8_t)(ds.pvT[i] & 0xFF); // Младший байт
    }
                            // 24байт ind=12-ind=35 Установки
    for (i = 0; i < 12; i++){
        *ptr++ = (uint8_t)(set[i] >> 8); // Старший байт
        *ptr++ = (uint8_t)(set[i] & 0xFF); // Младший байт
    }
    *ptr++ = set[VENT];     // 1 байт ind=36 скорость вращения вентилятора
    *ptr++ = relayOut.value;// 1 байт ind=37 активные выходы реле
    *ptr++ = dsplPW;        // 1 байт ind=38 мощность подаваемая на тены
    *ptr++ = errors;        // 1 байт ind=39 ошибки
    *ptr++ = sTime.Hours;   // 1 байт ind=40 часы
    *ptr++ = sTime.Minutes; // 1 байт ind=41 минуты
    *ptr++ = sTime.Seconds; // 1 байт ind=42 секунды

    // Отправка данных по UART
    HAL_UART_Transmit(huart, buffer, 46, 1000);
}

// Пример использования (в вашем основном коде)

