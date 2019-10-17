// ПРИМЕР НАСТРОЙКИ ПОКАЗАНИЙ ПОЗИЦИИ ВАЛА:       // * Строки со звёздочкой являются необязательными.
                                                  //
#include <Wire.h>                                 // * Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <iarduino_I2C_Encoder.h>                 //   Подключаем библиотеку для работы с энкодером I2C-flash.
iarduino_I2C_Encoder enc(0x09);                   //   Объявляем объект enc для работы с функциями и методами библиотеки iarduino_I2C_Encoder, указывая адрес модуля на шине I2C.
                                                  //   Если объявить объект без указания адреса (iarduino_I2C_Encoder enc;), то адрес будет найден автоматически.
void setup(){                                     //
    delay(500);                                   // * Ждём завершение переходных процессов связанных с подачей питания.
    Serial.begin(9600);                           //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                             // * Ждём завершения инициализации шины UART.
    enc.begin();                                  //   Инициируем работу с энкодером.
    enc.setPosSettings(5, false);                 //   Указываем считать до 5 полных оборотов, без отрицательных значений.
}                                                 //
                                                  //
void loop(){                                      //
    Serial.println( enc.getPosition() );          //   Выводим текущую позицию вала энкодера.
    delay(500);                                   // * Задержка позволяет медленнее заполнять монитор последовательного порта.
}                                                 //