// ДАННЫЙ ПРИМЕР ИЩЕТ МОДУЛИ НА ШИНЕ И ВЫВОДИТ ИНФОРМАЦИЮ О НИХ.              // * Строки со звёздочкой являются необязательными.
                                                                              //
#include <Wire.h>                                                             //   Подключаем библиотеку для работы с аппаратной шиной I2C.
                                                                              //
void setup(){                                                                 //
    delay(500);                                                               // * Ждём завершение переходных процессов связанных с подачей питания.
    uint8_t i,j,k[4],f=1;                                                     //   Объявляем переменные.
    Wire.begin();                                                             //   Инициируем работу по шине I2C в качестве мастера.
    Serial.begin(9600);                                                       //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                                                         // * Ждём завершения инициализации шины UART.
    Serial.println("Поиск устройств на шине I2C ... ");                       //
//  Проходим по всем доступным адресам на шине I2C:                           //
    for(i=7; i<127; i++){                                                     //   Проходим по всем доступным адресам на шине I2C ...
      Wire.beginTransmission(i);                                              //   Инициируем передачу данных по шине I2C к устройству с адресом i и битом RW=0 (запись). При этом сама передача не начнётся.
      Wire.write(0x04);                                                       //   Указываем единственный байт который мы хотим передать (это номер регистра).
    j=Wire.endTransmission();                                                 //   Выполняем инициированную ранее передачу данных.
      if(j==0){                                                               //   Если передача выполнена успешно (0-передача успешна / 1 - переполнен буфер для передачи / 2 - получен NACK при передаче адреса / 3 - получен NACK при передаче данных / 4 - другая ошибка), то ...
        f=0;                                                                  //   Сбрасываем флаг f указывающий на отсутствие устройств на шине I2C.
        Serial.print(F("Устройство с адресом "));                             //
        Serial.print(i);                                                      //   Выводим адрес i.
        Serial.print(" (0x"); Serial.print(i,HEX); Serial.print(")");         //   Выводим адрес i в 16-тиричной системе счисления.
        Wire.requestFrom( i, (uint8_t) 4 );                                   //   Инициируем получение данных по шине I2C от устройства с адресом i и битом RW=1 (чтение). Второй аргумент функции указывает количество байт которое мы желаем прочитать.
        while(Wire.available()){k[j]=Wire.read(); j++;}                       //   Сохраняем все полученные данные в массив k. (k[0]-тип модуля / k[1]-версия прошивки / k[2]-адрес на шине / k[2]-принадлежность к линейке «Metro» или серии «Flash»).
        if(((k[2]>>1)==i) && (k[3]==0xC3 || k[3]==0x3C)){                     //   Если значение k[2] (без последнего бита) совпадает с адресом устройства i, а значение k[3] совпадает с идентификатором 0xC3 (модуль «Metro»), или 0x3С (модуль «Flash»), то ...
                         Serial.print(F(" является модулем "        ));       //
          if(k[3]==0xC3){Serial.print(F("линейки «Metro»"           )); }     // * Если в 3 элементе массива k хранится значение 0xC3, значит это модуль «Metro».
          if(k[3]==0x3C){Serial.print(F("серии «Flash»"             )); }     // * Если в 3 элементе массива k хранится значение 0x3C, значит это модуль «Flash».
                         Serial.print(F(" - "                       ));       //
          if(k[0]==0x01){Serial.print(F("кнопка"                    )); }else //   Если в 0 элементе массива k хранится значение 0x01, значит это кнопка.
          if(k[0]==0x02){Serial.print(F("светодиод"                 )); }else //   Если в 0 элементе массива k хранится значение 0x02, значит это RGB светодиод.
          if(k[0]==0x03){Serial.print(F("потенциометр"              )); }else //   Если в 0 элементе массива k хранится значение 0x03, значит это потенциометр.
          if(k[0]==0x04){Serial.print(F("звукоизлучатель"           )); }else //   Если в 0 элементе массива k хранится значение 0x04, значит это звукоизлучатель.
          if(k[0]==0x05){Serial.print(F("датчик влажности DHT"      )); }else //   Если в 0 элементе массива k хранится значение 0x05, значит это датчик влажности и температуры.
          if(k[0]==0x06){Serial.print(F("датчик освещённости DSL"   )); }else //   Если в 0 элементе массива k хранится значение 0x06, значит это датчик освещённости.
          if(k[0]==0x07){Serial.print(F("расширитель выводов"       )); }else //   Если в 0 элементе массива k хранится значение 0x07, значит это расширитель выводов.
          if(k[0]==0x08){Serial.print(F("светодиодная матрица 8x8"  )); }else //   Если в 0 элементе массива k хранится значение 0x08, значит это светодиодная матрица 8x8.
          if(k[0]==0x09){Serial.print(F("энкодер"                   )); }else //   Если в 0 элементе массива k хранится значение 0x09, значит это энкодер с поддержкой функции потенциометра.
          if(k[0]==0x0A){Serial.print(F("реле на 2 канала"          )); }else //   Если в 0 элементе массива k хранится значение 0x0A, значит это электромеханическое реле на 2 канала.
          if(k[0]==0x0B){Serial.print(F("реле на 4 канала"          )); }else //   Если в 0 элементе массива k хранится значение 0x0B, значит это твердотельное реле на 4 канала.
          if(k[0]==0x0C){Serial.print(F("силовой ключ на 4N-канала" )); }else //   Если в 0 элементе массива k хранится значение 0x0C, значит это силовой ключ на 4 N-канала.
          if(k[0]==0x0D){Serial.print(F("силовой ключ на 4P-канала" )); }else //   Если в 0 элементе массива k хранится значение 0x0D, значит это силовой ключ на 4 P-канала.
          if(k[0]==0x0E){Serial.print(F("силовой ключ на 4N-канала" )); }else //   Если в 0 элементе массива k хранится значение 0x0E, значит это силовой ключ на 4 N-канала, с измерением тока.
          if(k[0]==0x0F){Serial.print(F("силовой ключ на 4P-канала" )); }else //   Если в 0 элементе массива k хранится значение 0x0F, значит это силовой ключ на 4 P-канала, с измерением тока.
          if(k[0]==0x10){Serial.print(F("бампер с датчиками линий"  )); }else //   Если в 0 элементе массива k хранится значение 0x10, значит это бампер с датчиками линий.
          if(k[0]==0x11){Serial.print(F("джойстик"                  )); }else //   Если в 0 элементе массива k хранится значение 0x11, значит это джойстик.
          if(k[0]==0x12){Serial.print(F("LCD конвертер в I2C"       )); }else //   Если в 0 элементе массива k хранится значение 0x12, значит это адаптер для LCD дисплеев 1602/2004.
          if(k[0]==0x13){Serial.print(F("клавиатура"                )); }else //   Если в 0 элементе массива k хранится значение 0x12, значит это клавиатура.
                        {Serial.print(F("тип модуля не определён"   )); }     //   Если в 0 элементе массива k хранится иное значение, значит этот модуль был создан позднее чем данный скетч, или на одном адресе находятся несколько устройств.
                         Serial.print(F(" (версия прошивки "        ));       // *
                         Serial.print(k[1]                           );       // * В первом элементе массива k хранится версия прошивки модуля.
                         Serial.print(F(").\r\n"                    ));       // *
        }else           {Serial.print(F(" не опознано.\r\n"         )); }     //
      }                                                                       //
    }                                                                         //
    if( f )             {Serial.print(F("Устройства не найдены.\r\n")); }     //
}                                                                             //
                                                                              //
void loop(){                                                                  //
}                                                                             //
