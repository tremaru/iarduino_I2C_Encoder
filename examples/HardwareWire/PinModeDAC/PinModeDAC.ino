// ПРИМЕР НАСТРОЙКИ ВЫХОДА В АНАЛОГОВЫЙ РЕЖИМ:    //   Режимы работы выхода модуля энергонезависимы (сохраняются и после отключения питания).
                                                  // * Строки со звёздочкой являются необязательными.
                                                  //
// Энкодер, потенциометр, FLASH-I2C Trema-модуль: //   https://iarduino.ru/shop/Expansion-payments/enkoder-potenciometr-i2c.html
// Информация о подключении модулей к шине I2C:   //   https://wiki.iarduino.ru/page/i2c_connection/
// Информация о модуле и описание библиотеки:     //   https://wiki.iarduino.ru/page/potentiometer-i2c/
                                                  //
#include <Wire.h>                                 //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Encoder.
#include <iarduino_I2C_Encoder.h>                 //   Подключаем библиотеку для работы с энкодером I2C-flash.
iarduino_I2C_Encoder enc(0x09);                   //   Объявляем объект enc для работы с функциями и методами библиотеки iarduino_I2C_Encoder, указывая адрес модуля на шине I2C.
                                                  //   Если объявить объект без указания адреса (iarduino_I2C_Encoder enc;), то адрес будет найден автоматически.
void setup(){                                     //
    delay(500);                                   // * Ждём завершение переходных процессов связанных с подачей питания.
    enc.begin(&Wire); // &Wire1, &Wire2 ...       //   Инициируем работу с энкодером, указав ссылку на объект для работы с шиной I2C на которой находится энкодер (по умолчанию &Wire).
    enc.setPinOut(ENC_PIN_MODE_DAC, 2);           //   Указываем выводу модуля работать как аналоговый выход,
}                                                 //   напряжение которого меняется от 0 до 3.3В
                                                  //   за 2 полных оборота вала энкодера.
void loop(){}                                     //