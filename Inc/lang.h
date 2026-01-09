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

    static const char* modeName[]  = {"СУШКА","ОБЖАРКА","ВАРКА","КОПЧЕНИЕ"};
    static const char* setName[]   = {"t КАМЕРЫ","t ПРОДУКТА","t ДЫМА","ДЛИТЕЛЬНОСТЬ","СКОРОСТЬ","ТАЙМ.ON","ТАЙМ.OFF","ПРОЧЕЕ"};
    static const char* relayName[] = {"ПИД","НАГРЕВ","ТАЙМЕР","ВЛАГА","ЭЛЕКТРО","Кл.ДЫМА","Кл.ВОДЫ"};

#endif

#endif // _LANG_H
