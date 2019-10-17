// ПРИМЕР НАСТРОЙКИ ВЫХОДА В РЕЖИМ LOG ШИМ:       //   Режимы работы выхода модуля энергонезависимы (сохраняются и после отключения питания).
                                                  // * Строки со звёздочкой являются необязательными.
#include <Wire.h>                                 // * Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <iarduino_I2C_Encoder.h>                 //   Подключаем библиотеку для работы с энкодером I2C-flash.
iarduino_I2C_Encoder enc(0x09);                   //   Объявляем объект enc для работы с функциями и методами библиотеки iarduino_I2C_Encoder, указывая адрес модуля на шине I2C.
                                                  //   Если объявить объект без указания адреса (iarduino_I2C_Encoder enc;), то адрес будет найден автоматически.
void setup(){                                     //
    delay(500);                                   // * Ждём завершение переходных процессов связанных с подачей питания.
    enc.begin();                                  //   Инициируем работу с энкодером.
    enc.setPinOut(ENC_PIN_MODE_PWM_LOG, 2);       //   Указываем выводу модуля работать как выход с логарифмическим ШИМ,
}                                                 //   коэффициент заполнения которого меняется от 0% до 100%
                                                  //   за 2 полных оборота вала энкодера.
void loop(){}                                     //   Логарифмический ШИМ лучше подходит для управления светодиодами, чем обычный (линейный) ШИМ.