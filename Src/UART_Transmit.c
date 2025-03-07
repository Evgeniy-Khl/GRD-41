#include "main.h"

extern union Byte portFlag;
extern struct Ds ds;
extern uint16_t set[];
extern uint16_t speedData[][2];
extern uint8_t modeCell;
extern uint8_t errors;
extern const char* modeName[];
extern UART_HandleTypeDef huart1;

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
    uint8_t buffer[64]; // Буфер для хранения данных перед отправкой
    uint8_t *ptr = buffer; // Указатель для записи в буфер

    // 1. portFlag
    *ptr++ = portFlag.value;

    // 2. modeName[modeCell]
    const char* selectedMode = modeName[modeCell];
    for (int i = 0; selectedMode[i] != '\0'; i++) {
        *ptr++ = selectedMode[i];
    }
    *ptr++ = '\0'; // Додаємо нульовий термінатор

    // 3. ds.pvT[0] - ds.pvT[3]
    for (int i = 0; i < 4; i++) {
        *ptr++ = (uint8_t)(ds.pvT[i] >> 8); // Старший байт
        *ptr++ = (uint8_t)(ds.pvT[i] & 0xFF); // Младший байт
    }

    // 4. set[4], set[8], set[5]
    *ptr++ = (uint8_t)(set[4] >> 8);
    *ptr++ = (uint8_t)(set[4] & 0xFF);

    *ptr++ = (uint8_t)(set[8] >> 8);
    *ptr++ = (uint8_t)(set[8] & 0xFF);

    *ptr++ = (uint8_t)(set[5] >> 8);
    *ptr++ = (uint8_t)(set[5] & 0xFF);

    // 5. speedData[set[5]][0]
    *ptr++ = (uint8_t)(speedData[set[5]][0] >> 8);
    *ptr++ = (uint8_t)(speedData[set[5]][0] & 0xFF);

    // 6. errors
    *ptr++ = errors;

    // Отправка данных по UART
    HAL_UART_Transmit(huart, buffer, ptr - buffer, HAL_MAX_DELAY);
}

// Пример использования (в вашем основном коде)
// transmitDataUART(&huart1); // Передача данных по UART1
