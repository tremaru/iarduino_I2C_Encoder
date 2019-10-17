// ПРИМЕР ЧТЕНИЯ ТАКТОВ ПОВОРОТА ЭНКОДЕРА:        //   В данном примере считываются такты поворотов энкодера, отдельно вправо и отдельно влево.
                                                  // * Строки со звёздочкой являются необязательными.
#include <Wire.h>                                 // * Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <iarduino_I2C_Encoder.h>                 //   Подключаем библиотеку для работы с энкодером I2C-flash.
iarduino_I2C_Encoder enc(0x09);                   //   Объявляем объект enc для работы с функциями и методами библиотеки iarduino_I2C_Encoder, указывая адрес модуля на шине I2C.
                                                  //   Если объявить объект без указания адреса (iarduino_I2C_Encoder enc;), то адрес будет найден автоматически.
void setup(){                                     //
    delay(500);                                   // * Ждём завершение переходных процессов связанных с подачей питания.
    Serial.begin(9600);                           //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                             // * Ждём завершения инициализации шины UART.
    enc.begin();                                  //   Инициируем работу с энкодером.
}                                                 //
                                                  //
void loop(){                                      //
    int turnL=0, turnR=0;                         //   Определяем переменные для чтения количества тактов поворота энкодера.
//  Считываем такты поворота энкодера:            //
    turnL=enc.getEncoder(ENC_TURN_LEFT);          //   Считываем количество тактов поворота энкодера влево  (против часовой стрелки).
    turnR=enc.getEncoder(ENC_TURN_RIGHT);         //   Считываем количество тактов поворота энкодера вправо (по     часовой стрелке).
//  Выводим считанные данные:                     //
    if( turnL ){                                  //
        Serial.print("-");                        //   Считаем что против часовой стрелки, это отрицательные значения.
        Serial.println(turnL);                    //   Выводим количество совершённых тактов (в одном полном обороте 20 тактов).
    }                                             //
    if( turnR ){                                  //
        Serial.print("+");                        //   Считаем что по часовой стрелке, это положительные значения.
        Serial.println(turnR);                    //   Выводим количество совершённых тактов (в одном полном обороте 20 тактов).
    }                                             //
//  Приостанавливаем выполнение скетча:           //
    delay(100);                                   // * Без задержки в мониторе будут появляться только ±1.
}                                                 //