// ПРИМЕР НАСТРОЙКИ ВЫХОДА В РЕЖИМ СЕРВО:                    //   Режимы работы выхода модуля энергонезависимы (сохраняются и после отключения питания).
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
    enc.begin(&sWire);                                       //   Инициируем работу с энкодером, указав ссылку на объект для работы с шиной I2C на которой находится энкодер (по умолчанию &Wire).
//  enc.setServoLimit(450, 2450);                            // * Задаём границы поворота сервопривода, указав ширину импульсов в мкс для минимального и максимального угла.
    enc.setPinOut(ENC_PIN_MODE_SER, 2);                      //   Указываем выводу модуля работать как выход управления сервоприводом,
}                                                            //   угол поворота вала которого изменится от минимума до максимума,
                                                             //   за 2 полных оборота ручки энкодера.
void loop(){}                                                //   Как найти ширину импульсов для функции setServoLimit() рассказано в примере "Установка ограничений поворота вала сервопривода"
                                                             //   на странице https://wiki.iarduino.ru/page/potentiometer-i2c/