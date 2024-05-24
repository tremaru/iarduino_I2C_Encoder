// ПРИМЕР РАБОТЫ С КНОПКОЙ ЭНКОДЕРА:                         //   Светодиод на плате Arduino будет светиться пока нажата кнопка.
                                                             // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h> //   https://iarduino.ru/file/627.html
                                                             //
// Энкодер, потенциометр, FLASH-I2C Trema-модуль:            //   https://iarduino.ru/shop/Expansion-payments/enkoder-potenciometr-i2c.html
// Информация о подключении модулей к шине I2C:              //   https://wiki.iarduino.ru/page/i2c_connection/
// Информация о модуле и описание библиотеки:                //   https://wiki.iarduino.ru/page/potentiometer-i2c/
                                                             //
#include <iarduino_I2C_Software.h>                           //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_Encoder.
SoftTwoWire sWire(3,4);                                      //   Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                             //
#include <iarduino_I2C_Encoder.h>                            //   Подключаем библиотеку для работы с энкодером I2C-flash.
iarduino_I2C_Encoder enc(0x09);                              //   Объявляем объект enc для работы с функциями и методами библиотеки iarduino_I2C_Encoder, указывая адрес модуля на шине I2C.
                                                             //   Если объявить объект без указания адреса (iarduino_I2C_Encoder enc;), то адрес будет найден автоматически.
void setup(){                                                //
    delay(500);                                              // * Ждём завершение переходных процессов связанных с подачей питания.
    pinMode(LED_BUILTIN, OUTPUT);                            //   Конфигурируем вывод к которому подключён светодиод на плате Arduino в режим работы на выход.
    enc.begin(&sWire);                                       //   Инициируем работу с энкодером, указав ссылку на объект для работы с шиной I2C на которой находится энкодер (по умолчанию &Wire).
}                                                            //
                                                             //
void loop(){                                                 //
    bool f = enc.getButton(KEY_PRESSED);                     //   Считываем состояние кнопки энкодера в переменную f.
    digitalWrite(LED_BUILTIN, f);                            //   Устанавливаем состояние светодиода в соответствии со значением f.
}                                                            //