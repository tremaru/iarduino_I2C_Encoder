// ПРИМЕР НАСТРОЙКИ ПОКАЗАНИЙ ПОЗИЦИИ ВАЛА:       // * Строки со звёздочкой являются необязательными.
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
    Serial.begin(9600);                           //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                             // * Ждём завершения инициализации шины UART.
    enc.begin(&Wire); // &Wire1, &Wire2 ...       //   Инициируем работу с энкодером, указав ссылку на объект для работы с шиной I2C на которой находится энкодер (по умолчанию &Wire).
    enc.setPosSettings(5, false);                 //   Указываем считать до 5 полных оборотов, без отрицательных значений.
}                                                 //
                                                  //
void loop(){                                      //
    Serial.println( enc.getPosition() );          //   Выводим текущую позицию вала энкодера.
    delay(500);                                   // * Задержка позволяет медленнее заполнять монитор последовательного порта.
}                                                 //