#ifndef _LANG_H
#define _LANG_H

// Раскомментируйте только одну строку для выбора языка
#define LANG_UA
// #define LANG_RU
// #define LANG_EN

// ======================= УКРАЇНСЬКА =======================
#if defined(LANG_UA)
    #define STR_GRD_MAX       "GRD Max"
    #define STR_INIT_OK       "Ыныцыалызацыя успышна."        //Ініціалізація успішна.
    #define STR_INIT_FIRST    "Первинна ыныцыалызацыя."       //Первинна ініціалізація.
    #define STR_ERR_FLASH     "Помилки читання FLASH!"
    #define STR_ERR_UNKNOWN   "Невыдома помилка!"
    #define STR_SENSORS_COUNT "Датчикыв температури: %d шт."
    #define STR_PWR_FAIL      "Виявлений збый живлення!"      //Виявлений збій живлення!
    #define STR_RESTORING     "Выдновлення з %u год. %u хв."  //Відновлення з %u год. %u хв.
    #define STR_NORMAL_START  "Звичайний старт!"
    #define STR_SENSOR_N      "Датчик N%u = %3.1f$ "
    // Кнопки
    #define STR_BTN_START     "ПУСК"
    #define STR_BTN_STOP      "СТОП"
    #define STR_BTN_CONTROL   "Керуван."
    #define STR_BTN_SETTINGS  "Налаштув."
    #define STR_BTN_EXIT      "Вихыд"     // Сохранено авторское написание
    #define STR_BTN_SELECT    "Вибыр"
    #define STR_BTN_CANCEL    "Отм."
    #define STR_BTN_SAVE      "Зап."
    #define STR_BTN_CORRECT   "Корек"

    // Главный экран
    #define STR_MODE_LABEL    "РЕЖИМ:"
    #define STR_CAMERA        "  КАМЕРА  "
    #define STR_PRODUCT       "  ПРОДУКТ "
    #define STR_ERROR         " ПОМИЛКА  "
    #define STR_OVERHEAT      " ПЕРЕГРIВ "
    #define STR_DEVIATION     "ВIДХIЛЕННЯ"
    #define STR_DURATION      "   ТРИВАЛIСТЬ РЕЖИМУ   "
    #define STR_TIME_UNIT_H   "год."
    #define STR_TIME_UNIT_M   "хвл."
    #define STR_TIME_UNIT_S   "сек."
    
    // Предупреждения (заслонки)
    #define STR_CLOSE_DAMP    "ЗАКРИЙТЕ ЗАСЛЫНКИ"
    #define STR_OPEN_DAMP     "ВЫДКРИЙТЕ ЗАСЛЫНКИ"
    #define STR_VENT_LABEL    "вентиляцыъ!"

    // Датчики и специфические ошибки
    #define STR_SENSOR_ERR    "ПОМИЛКА ДАТЧИКА"
    #define STR_HUMID_SENS    "ВОЛОГИЙ ДАТЧИК "
    #define STR_SMOKE_SENS    "       ДАТЧИК ДИМУ     "
    #define STR_SMOKE_LOW     "ДИМ НИЗЬКОЪ ТЕМПЕРАТУРИ"
    #define STR_SMOKE_HIGH    "ДИМ ВИСОКОЪ ТЕМПЕРАТУРИ"
    #define STR_FAN_FAIL      "  НЕ ПРАЦЮЭ ВЕНТИЛЯТОР  "
    #define STR_SPEED_UNIT    "об/хвл."

    // Состояния и меню
    #define STR_STATUS_OUT    "СТАН ВИХОДЫВ"
    #define STR_INPUT         "ВХЫД N"
    #define STR_T_HUMID       "t ВОЛОГОГО"
    #define STR_VALUE         "Значення:"
    #define STR_UNIT_STEPS    "выд.один."
    
    #define STR_SENS_MISSING  "Датчик N%u выдсутный!"
    #define STR_SENS_ADD      "Потрыбно додати датчикыв!"
    #define STR_SENS_DISCONN  "Зараз выдключыть усы датчики"
    #define STR_SENS_CONN_ONE "Пыдключыть тыкы датчик N%u"
    #define STR_SENS_READ_OK  "Датчик N%u прочитаний."
    #define STR_SENS_READY    "Датчик N%u готовий до роботи. "
    #define STR_SENS_ANOTHER  "Це датчик N%u потрыбен ынший!"
    #define STR_SENS_DISC_ONE "Тепер выд'эднайте цей датчик"
    #define STR_SENS_CONNECT  "Тепер пыдключыть усы датчики"
    #define STR_SENS_ALL_OK   "Усы датчики пыдключены."
    #define STR_FLASH_WRITE   "ВИКОНАЮ ЗАПИС!"
    
    static const char* modeName[] = {"СУШЫННЯ","ОБЖАРКА","ВАРЫННЯ","КОПЧЕННЯ"};
    static const char* setName[]  = {"t КАМЕРИ","t ПРОДУКТА","t ДИМА","ТРИВАЛЫСТЬ","ШВИДКЫСТЬ","ТАЙМ.ON","ТАЙМ.OFF","ЫНШЕ"};
    static const char* otherName[]= {"ПРОДУВАННЯ","АВАРЫЯ","ГЫСТЕРЕЗ","ОХОЛОДЖ.","Prop","Integ","Diff"};
    static const char* relayName[]= {"ПЫД","НАГРЫВ","ТАЙМЕР","ВОЛОГА","ЕЛЕКТРО","Кл.ДИМА","Кл.ВОДИ"};

// ========================= РУССКИЙ =========================
#elif defined(LANG_RU)
    #define STR_GRD_MAX       "GRD Max"
    #define STR_INIT_OK       "Инициализация успешна."
    #define STR_INIT_FIRST    "Первичная инициализация."
    #define STR_ERR_FLASH     "Ошибки чтения FLASH!"
    #define STR_ERR_UNKNOWN   "Неизвестная ошибка!"
    #define STR_SENSORS_COUNT "Датчиков температуры: %d шт."
    #define STR_PWR_FAIL      "Обнаружен сбой питания!"
    #define STR_RESTORING     "Восстановление с %u ч. %u мин."
    #define STR_NORMAL_START  "Обычный старт!"
    #define STR_SENSOR_N      "Датчик N%u = %3.1f$ "
    // Кнопки
    #define STR_BTN_START     "ПУСК"
    #define STR_BTN_STOP      "СТОП"
    #define STR_BTN_CONTROL   "Управл."
    #define STR_BTN_SETTINGS  "Настройк."
    #define STR_BTN_EXIT      "Выход"
    #define STR_BTN_SELECT    "Выбор"
    #define STR_BTN_CANCEL    "Отм."
    #define STR_BTN_SAVE      "Зап."
    #define STR_BTN_CORRECT   "Коррек"

    // Главный экран
    #define STR_MODE_LABEL    "РЕЖИМ:"
    #define STR_CAMERA        "  КАМЕРА  "
    #define STR_PRODUCT       "  ПРОДУКТ "
    #define STR_ERROR         "  ОШИБКА  "
    #define STR_OVERHEAT      " ПЕРЕГРЕВ "
    #define STR_DEVIATION     "ОТКЛОНЕНИЕ"
    #define STR_DURATION      " Продолжительность  "
    #define STR_TIME_UNIT_H   "ч."
    #define STR_TIME_UNIT_M   "мин."
    #define STR_TIME_UNIT_S   "сек."
    
    // Предупреждения
    #define STR_CLOSE_DAMP    "ЗАКРОЙТЕ ЗАСЛОНКИ"
    #define STR_OPEN_DAMP     "ОТКРОЙТЕ ЗАСЛОНКИ"
    #define STR_VENT_LABEL    "вентиляции!"

    // Датчики
    #define STR_SENSOR_ERR    "ОШИБКА ДАТЧИКА"
    #define STR_HUMID_SENS    "ВЛАЖНЫЙ ДАТЧИК"
    #define STR_SMOKE_SENS    "      ДАТЧИК ДЫМА      "
    #define STR_SMOKE_LOW     "ДЫМ НИЗКОЙ ТЕМПЕРАТУРЫ"
    #define STR_SMOKE_HIGH    "ДЫМ ВЫСОКОЙ ТЕМПЕРАТУРЫ"
    #define STR_FAN_FAIL      "  ВЕНТИЛЯТОР НЕ РАБОТАЕТ"
    #define STR_SPEED_UNIT    "об/мин."

    // Состояния
    #define STR_STATUS_OUT    "СОСТОЯНИЕ ВЫХОДОВ"
    #define STR_INPUT         "ВХОД N"
    #define STR_T_HUMID       "t ВЛАЖНОГО"
    #define STR_VALUE         "Значение:"
    #define STR_UNIT_STEPS    "отн.ед."
    
    #define STR_SENS_MISSING  "Датчик N%u отсутствует!"
    #define STR_SENS_ADD      "Нужно добавить датчики!"
    #define STR_SENS_DISCONN  "Сейчас отключите все датчики"
    #define STR_SENS_CONN_ONE "Подключите только датчик N%u"
    #define STR_SENS_READ_OK  "Датчик N%u прочитан."
    #define STR_SENS_READY    "Датчик N%u готов к работе."
    #define STR_SENS_ANOTHER  "Это датчик N%u нужен другой!"
    #define STR_SENS_DISC_ONE "Теперь отсоедините этот датчик"
    #define STR_SENS_CONNECT  "Теперь подключите все датчики"
    #define STR_SENS_ALL_OK   "Все датчики подключены."
    #define STR_FLASH_WRITE   "ВЫПОЛНЯЮ ЗАПИСЬ!"

    static const char* modeName[]  = {"СУШКА","ОБЖАРКА","ВАРКА","КОПЧЕНИЕ"};
    static const char* setName[]   = {"t КАМЕРЫ","t ПРОДУКТА","t ДЫМА","ДЛИТЕЛЬНОСТЬ","СКОРОСТЬ","ТАЙМ.ON","ТАЙМ.OFF","ПРОЧЕЕ"};
    static const char* otherName[] = {"ПРОДУВКА","АВАРИЯ","ГИСТЕРЕЗИС","ОХЛАЖД.","Prop","Integ","Diff"};
    static const char* relayName[] = {"ПИД","НАГРЕВ","ТАЙМЕР","ВЛАГА","ЭЛЕКТРО","Кл.ДЫМА","Кл.ВОДЫ"};

#endif

#endif // _LANG_H
