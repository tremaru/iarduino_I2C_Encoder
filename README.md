[![](https://iarduino.ru/img/logo.svg)](https://iarduino.ru)[![](https://wiki.iarduino.ru/img/git-shop.svg?3)](https://iarduino.ru) [![](https://wiki.iarduino.ru/img/git-wiki.svg?2)](https://wiki.iarduino.ru) [![](https://wiki.iarduino.ru/img/git-lesson.svg?2)](https://lesson.iarduino.ru)[![](https://wiki.iarduino.ru/img/git-forum.svg?2)](http://forum.trema.ru)

# iarduino\_I2C\_Encoder

**This is a library for Arduino IDE. It allows to work with [Encoder](https://iarduino.ru/shop/Expansion-payments/enkoder-potenciometr-i2c.html) module made by iArduino.ru**

**Данная библиотека для Arduino IDE позволяет работать с модулем [Энкодер, потенциометр](https://iarduino.ru/shop/Expansion-payments/enkoder-potenciometr-i2c.html) от [iArduino.ru](https://iarduino.ru)**

> Подробнее про установку библиотеки читайте в нашей [инструкции](https://wiki.iarduino.ru/page/Installing_libraries/).

> Подробнее про подключение к [Arduino UNO](https://iarduino.ru/shop/boards/arduino-uno-r3.html)/[Piranha UNO](https://iarduino.ru/shop/boards/piranha-uno-r3.html) читайте на нашей [wiki](https://wiki.iarduino.ru/page/potentiometer-i2c/#h3_3)

| Модель | Ссылка на магазин |
|---|---|
| <p></p> <img src="https://wiki.iarduino.ru/img/resources/1190/1190.svg" width="100px"></img>| https://iarduino.ru/shop/Expansion-payments/enkoder-potenciometr-i2c.html |


## Назначение функций:

**Подключаем библиотеку** 

```C++
#include <iarduino_I2C_Encoder> // Подключаем библиотеку iarduino_I2C_Encoder для работы с модулем.
```

**Создаём объект** 

```C++
iarduino_I2C_Encoder ОБЪЕКТ; // Создаём объект для работы с методами библиотеки без указания адреса модуля на шине I2C.
```

**Создаём объект** 

```C++
iarduino_I2C_Encoder ОБЪЕКТ( АДРЕС ); // Создаём объект для работы с методами библиотеки указывая адрес модуля на шине I2C.
```

**Инициализация работы** 

```C++
ОБЪЕКТ.begin(); // Инициализация работы с модулем.
```

**Изменение адреса модуля** 

```C++
ОБЪЕКТ.changeAddress( АДРЕС ); // Изменение адреса модуля на шине I2C.
```

**Перезагрузка модуля**

```C++
ОБЪЕКТ.reset(); // Перезагрузка модуля.
```

**Получение текущего** 

```C++
ОБЪЕКТ.getAddress(); // Получение текущего адреса модуля на шине I2C.
```

**Получение версии прошивки** 

```C++
ОБЪЕКТ.getVersion(); // Получение версии прошивки модуля.
```

**Получение состояний кнопки** 

```C++
ОБЪЕКТ.getButton( ЗАПРОС ); // Получение времени удержания, состояния, или события кнопки.
```

**Получение количества оборотов** 

```C++
ОБЪЕКТ.getEncoder( [ НАПРАВЛЕНИЕ ] ); // Получение количества тактов оборота энкодера.
```

**Получение текущей позиции** 

```C++
ОБЪЕКТ.getPosition(); // Получение текущей позиции вала энкодера.
```

**Сброс текущей позиции** 

```C++
ОБЪЕКТ.resPosition(); // Сброс текущей позиции вала энкодера.
```

**Настройка получения позиции** 

```C++
ОБЪЕКТ.setPosSettings( ОБОРОТЫ [, ЗНАК ] ); // Настройка получения позиции вала энкодера.
```

**Настройка работы выхода** 

```C++
ОБЪЕКТ.setPinOut( РЕЖИМ [, ОБОРОТЫ [, ЧАСТОТА]] ); // Настройка работы выхода модуля.
```
**Инверсия направления поворота вала энкодера**

ОБЪЕКТ.invEncoder( ФЛАГ );

**Ограничение углов сервопривода при управлении им в автономном режиме**

ОБЪЕКТ.setServoLimit( ШИРИНА, ШИРИНА );

**Запрос ширины импульсов текущего положения вала сервопривода**

ОБЪЕКТ.getServoWidth();
