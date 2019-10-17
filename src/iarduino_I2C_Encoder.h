//	Библиотека для работы с Trema-модулем энкодер-потенциометр, I2C-flash для Arduino: https://iarduino.ru/shop/Expansion-payments/enkoder-potenciometr-i2c.html
//  Версия: 1.0.1
//  Последнюю версию библиотеки Вы можете скачать по ссылке: https://iarduino.ru/file/516.html
//  Подробное описание функций бибилиотеки доступно по ссылке: https://wiki.iarduino.ru/page/potentiometer-i2c/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел.
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_I2C_Encoder_h																											//
#define iarduino_I2C_Encoder_h																											//
																																		//
#if defined(ARDUINO) && (ARDUINO >= 100)																								//
#include		<Arduino.h>																												//
#else																																	//
#include		<WProgram.h>																											//
#endif																																	//
																																		//
#include		<iarduino_I2C_Encoder_I2C.h>																							//	Подключаем файл iarduino_I2C_Encoder.h - для работы с шиной I2C		(используя функции структуры iI2C)
																																		//
#define			DEF_CHIP_ID_FLASH		0x3C																							//	ID линейки чипов - константа для всех чипов серии Flash (позволяет идентифицировать принадлежность чипа к серии).
#define			DEF_CHIP_ID_METRO		0xC3																							//	ID линейки чипов - константа для всех чипов серии Metro (позволяет идентифицировать принадлежность чипа к серии).
#define			DEF_MODEL_ENC			0x09																							//	Идентификатор модели - константа.
//				Адреса регистров модуля:																								//
#define			REG_FLAGS_0				0x00																							//	Адрес регистра флагов управления для чтения.
#define			REG_BITS_0				0x01																							//	Адрес регистра битов  управления для чтения и записи.
#define			REG_FLAGS_1				0x02																							//	Адрес регистра флагов управления для чтения.
#define			REG_BITS_1				0x03																							//	Адрес регистра битов  управления для чтения и записи.
#define			REG_MODEL				0x04																							//	Адрес регистра содержащего номер типа модуля.
#define			REG_VERSION				0x05																							//	Адрес регистра содержащего версию прошивки.
#define			REG_ADDRESS				0x06																							//	Адрес регистра содержащего текущий адрес модуля на шине I2C. Если адрес указан с флагом IF-PIN-ADDRES то адрес установится (и запишется в регистр) только при наличии 1 на входе PIN_ADDRES.
#define			REG_CHIP_ID				0x07																							//	Адрес регистра содержащего ID линейки чипов «Flash». По данному ID можно определить принадлежность чипа к линейки «Flash».
#define			REG_ENC_FREQUENCY_L		0x08																							//	Адрес регистра частоты ШИМ в Гц (младший байт).
#define			REG_ENC_FREQUENCY_H		0x09																							//	Адрес регистра частоты ШИМ в Гц (старший байт).
#define			REG_ENC_KEY				0x10																							//	Адрес регистра статусных флагов кнопки.
#define			REG_ENC_KEY_TIM			0x11																							//	Адрес регистра времени удержания/простоя кнопки (в десятых долях секунд).
#define			REG_ENC_PIN_SET			0x12																							//	Адрес регистра управления выводом модуля. Задает режим работы вывода и количество полных оборотов энкодера для достижения максимальных показаний на выводе.
#define			REG_ENC_RES_SET			0x13																							//	Адрес регистра управления счётчиком положения. Разрешает использовать знак, сбрасывает счётчик положения «REG_ENC_RES_CNT» и задает количество полных оборотов энкодера, которые способен считать счетчик положения «REG_ENC_RES_CNT». Дальнеёшее вращение в ту же сторону, не приведёт к приращению/уменьшению счётчика.
#define			REG_ENC_RES_CNT_L		0x14																							//	Адрес регистра счетчика положения «REG_ENC_RES_CNT» (младший байт). Хранит количество тактов поворота энкодера (в одном полном обороте 20 тактов). Значение может быть как со знаком (если «REG_RES_SIGN_EN»=1), так и без (если «REG_RES_SIGN_EN»=0).
#define			REG_ENC_RES_CNT_H		0x15																							//	Адрес регистра счетчика положения «REG_ENC_RES_CNT» (младший байт). Максимальное значение «REG_ENC_RES_CNT» = «ENC_RES_MAX_TURN» * 20. Значение счетчика положения позволяет работать с энкодером как с цифровым потенциометром.
#define			REG_ENC_ENC_CNT_LT		0x16																							//	Адрес регистра счётчика тактов энкодера. Хранит количество тактов поворота энкодера выполненных против ч.с. с момента последнего чтения данного регистра. Значение регистра сбрасывается автоматически после чтения.
#define			REG_ENC_ENC_CNT_RT		0x17																							//	Адрес регистра счётчика тактов энкодера. Хранит количество тактов поворота энкодера выполненных по     ч.с. с момента последнего чтения данного регистра. Значение регистра сбрасывается автоматически после чтения.
//				Позиция битов и флагов:																									//
#define			ENC_KEY_PUSHED			0x80																							//	Флаг нажатия на кнопку.			Устанавливается в момент нажатия на кнопку и остаётся установленным пока не будет прочитан. Флаг сбрасывается самостоятельно после чтении регистра «REG_KEY».
#define			ENC_KEY_RELEASED		0x40																							//	Флаг отпускания кнопки.			Устанавливается в момент отпускания кнопки и остаётся установленным пока не будет прочитан. Флаг сбрасывается самостоятельно после чтении регистра «REG_KEY».
#define			ENC_KEY_PRESSED			0x20																							//	Флаг состояния кнопки.			Установлен пока кнопка нажата и сброшен если она отпущена.
#define			ENC_KEY_TRIGGER			0x10																							//	Флаг триггер.					Меняет своё состояние с каждым новым нажатием на кнопку (нажали - установился, нажали - сбросился, нажали - установился, ...).
#define			ENC_KEY_HOLD_05			0x08																							//	Флаг удержания кнопки.			Устанавливается если кнопка удерживается дольше времени указанного в регистре «REG_TIME_HOLD_3» и сбрасывается при отпускании кнопки.
#define			ENC_KEY_HOLD_10			0x04																							//	Флаг удержания кнопки.			Устанавливается если кнопка удерживается дольше времени указанного в регистре «REG_TIME_HOLD_2» и сбрасывается при отпускании кнопки.
#define			ENC_KEY_HOLD_20			0x02																							//	Флаг удержания кнопки.			Устанавливается если кнопка удерживается дольше времени указанного в регистре «REG_TIME_HOLD_1» и сбрасывается при отпускании кнопки.
#define			ENC_KEY_CHANGED			0x01																							//	Флаг смены состояния кнопки.	Устанавливается в момент нажатия или отпускания кнопки и остаётся установленным пока не будет прочитан. Флаг сбрасывается самостоятельно после чтении регистра «REG_KEY».
#define			ENC_PIN_MAX_TURN		0xF0																							//	Биты кол. оборотов для вывода.	Значение определяет количество полных оборотов, требуемое для достижения максимальных показаний на выводе.
#define			ENC_PIN_MODE			0x0F																							//	Биты режима работы вывода.		Значение определяет режим работы вывода:
#define			ENC_PIN_MODE_OFF		0b0000																							//									0 - Отключён.
#define			ENC_PIN_MODE_KEY		0b0001																							//									1 - Кнопка.
#define			ENC_PIN_MODE_TRG		0b0010																							//									2 - Триггер.
#define			ENC_PIN_MODE_PWM		0b0011																							//									3 - ШИМ.
#define			ENC_PIN_MODE_PWM_LOG	0b0100																							//									4 - ШИМ логарифмический.
#define			ENC_PIN_MODE_DAC		0b0111																							//									7 - ЦАП.
#define			ENC_RES_SIGN_EN			0x80																							//	Бит  разрешает считать такты поворота резистора «REG_ENC_RES_CNT» со знаком (от 0 до ± «REG_RES_MAX»*20). Если бит сброшен, то «REG_ENC_RES_CNT» считает от 0 до + «REG_RES_MAX»*20.
#define			ENC_RES_RESET			0x40																							//	Бит  обнуляет «REG_ENC_RES_CNT».   Установка бита приведёт к обнулению счётчика тактов поворота резистора «REG_ENC_RES_CNT» и сбросу бита «RES_RESET».
#define			ENC_RES_MAX_TURN		0x3F																							//	Биты кол. оборотов резистора.  Значение (от 1 до 63) определяет количество полных оборотов резистора, которые влияют на счётчик «REG_ENC_RES_CNT».
#define			ENC_RES_SIGN			0x80																							//	Флаг отицательных значений.    Устанавливается если счётчик тактов поворота резистора «REG_ENC_RES_CNT» хранит отрицательное значение.
																																		//
#ifndef			KEY_PUSHED																												//
#define			KEY_PUSHED				1																								//	getButton( нажимается ).
#endif																																	//
																																		//
#ifndef			KEY_RELEASED																											//
#define			KEY_RELEASED			2																								//	getButton( отпускается ).
#endif																																	//
																																		//
#ifndef			KEY_PRESSED																												//
#define			KEY_PRESSED				3																								//	getButton( нажата ).
#endif																																	//
																																		//
#ifndef			KEY_TRIGGER																												//
#define			KEY_TRIGGER				4																								//	getButton( переключатель ).
#endif																																	//
																																		//
#ifndef			KEY_HOLD_05																												//
#define			KEY_HOLD_05				5																								//	getButton( удерживается дольше 0,5 секунд ).
#endif																																	//
																																		//
#ifndef			KEY_HOLD_10																												//
#define			KEY_HOLD_10				6																								//	getButton( удерживается дольше 1 секунды ).
#endif																																	//
																																		//
#ifndef			KEY_HOLD_20																												//
#define			KEY_HOLD_20				7																								//	getButton( удерживается дольше 2 секунд ).
#endif																																	//
																																		//
#ifndef			KEY_CHANGED																												//
#define			KEY_CHANGED				8																								//	getButton( состояние изменилось ).
#endif																																	//
																																		//
#ifndef			KEY_TIME_PRESSED																										//
#define			KEY_TIME_PRESSED		9																								//	getButton( текущее время удержания кнопки ).
#endif																																	//
																																		//
#ifndef			ENC_TURN																												//																						Пример: влево 2, вправо 5, вернёт +3.
#define			ENC_TURN				0																								//	getEncoder( количество совершённых тактов поворота ) - вернёт разницу со знаком.	Пример: влево 5, вправо 2, вернёт -3.
#endif																																	//
																																		//
#ifndef			ENC_TURN_LEFT																											//
#define			ENC_TURN_LEFT			1																								//	getEncoder( количество совершённых тактов поворота влево ) - вернёт значение от 0 до 255.
#endif																																	//
																																		//
#ifndef			ENC_TURN_RIGHT																											//
#define			ENC_TURN_RIGHT			2																								//	getEncoder( количество совершённых тактов поворота вправо ) - вернёт значение от 0 до 255.
#endif																																	//
																																		//
//				ENC_PIN_MODE_OFF		макрос определён выше																			//	setPinOut( Выход отключён ).
//				ENC_PIN_MODE_KEY		макрос определён выше																			//	setPinOut( Выход в режиме кнопки ).
//				ENC_PIN_MODE_TRG		макрос определён выше																			//	setPinOut( Выход в режиме переключателя ).
//				ENC_PIN_MODE_PWM		макрос определён выше																			//	setPinOut( Выход в режиме ШИМ                , количество полных оборотов от минимума до максимума [, частота ШИМ в Гц] ).
//				ENC_PIN_MODE_PWM_LOG	макрос определён выше																			//	setPinOut( Выход в режиме ШИМ для светодиодов, количество полных оборотов от минимума до максимума [, частота ШИМ в Гц] ).
//				ENC_PIN_MODE_DAC		макрос определён выше																			//	setPinOut( Выход в режиме ЦАП                , количество полных оборотов от минимума до максимума [, частота ШИМ в Гц] ).
																																		//
class iarduino_I2C_Encoder{																												//
	public:																																//
	/**	Конструктор класса **/																											//
		iarduino_I2C_Encoder							(uint8_t address=0){															//	Конструктор класса													(Параметр: адрес модуля на шине I2C, если не указан (=0), то адрес будет определён).
														if(address>0x7F){ address>>=1; }												//	Корректируем адрес, если он указан с учётом бита RW.
							valAddrTemp				=	address;																		//	Сохраняем переданный адрес модуля.
							objI2C					=	new iarduino_I2C;																//	Переопределяем указатель objI2C на объект производного класса iarduino_I2C.
		}																																//
	/**	Пользовательские функции **/																									//
		bool				begin						(void													);						//	Объявляем  функцию инициализации модуля								(Параметр:  отсутствует).
		bool				reset						(void													);						//	Объявляем  функцию перезагрузки модуля								(Параметр:  отсутствует).
		bool				changeAddress				(uint8_t												);						//	Объявляем  функцию смены адреса модуля на шине I2C					(Параметр:  новый адрес модуля).
		uint8_t				getAddress					(void													){ return valAddr;	}	//	Определяем функцию возвращающую текущий адрес модуля на шине I2C	(Параметр:  отсутствует).
		uint8_t				getVersion					(void													){ return valVers;	}	//	Определяем функцию возвращающую текущую версию прошивки модуля		(Параметр:  отсутствует).
		uint16_t			getButton					(uint8_t												);						//	Объявляем  функцию получения состояний/событий кнопки				(Параметр:  тип получаемого состояния или собития).
		int16_t				getEncoder					(uint8_t=ENC_TURN										);						//	Объявляем  функцию получения количества тактов энкодера				(Параметр:  направление вращения).
		int16_t				getPosition					(void													);						//	Объявляем  функцию получения текущей позиции вала энкодера			(Параметр:  отсутствует).
		bool				resPosition					(void													);						//	Объявляем  функцию сброса текущей позиции вала энкодера в 0			(Параметр:  отсутствует).
		bool				setPosSettings				(uint8_t, bool=false									);						//	Объявляем  функцию настроек получения позиции вала энкодера			(Параметры: количество полных оборотов, флаг разрешающий отрицательные значения).
		bool				setPinOut					(uint8_t, uint8_t=0, uint16_t=0							);						//	Объявляем  функцию программирования работы выхода модуля			(Параметры: режим работы выхода, количество полных оборотов от минимума до максимума [, частота ШИМ в Гц] ).
	private:																															//
	/**	Внутренние переменные **/																										//
		uint8_t				valAddrTemp				=	0;																				//	Определяем переменную для хранения адреса модуля на шине I2C который был указан, но не был проверен.
		uint8_t				valAddr					=	0;																				//	Определяем переменную для хранения адреса модуля на шине I2C.
		uint8_t				valVers					=	0;																				//	Определяем переменную для хранения версии прошивки модуля.
		uint8_t				val_REG_ENC_KEY			=	0;																				//	Определяем переменную для хранения значения регистра статусных флагов кнопки «REG_ENC_KEY». Благодаря данной переменной реализован сброс флагов «KEY_PUSHED», «KEY_RELEASED» и «KEY_CHANGED» не по чтению регистра «REG_ENC_KEY», а по обращению к функции getButton(флаг);
		uint16_t			val_REG_ENC_ENC_CNT_LT	=	0;																				//	Определяем переменную для хранения значения регистра тактов поворота против ч.с. «REG_ENC_ENC_CNT_LT». Благодаря данной переменной реализован сброс количества совершённых тактов, не по чтению регистра «REG_ENC_ENC_CNT_LT», а по обращению к функции getEncoder(направление);
		uint16_t			val_REG_ENC_ENC_CNT_RT	=	0;																				//	Определяем переменную для хранения значения регистра тактов поворота по     ч.с. «REG_ENC_ENC_CNT_RT». Благодаря данной переменной реализован сброс количества совершённых тактов, не по чтению регистра «REG_ENC_ENC_CNT_RT», а по обращению к функции getEncoder(направление);
		uint8_t				data[8];																									//	Объявляем  массив     для хранения получаемых/передаваемых данных.
		iarduino_I2C_BASE*	objI2C;																										//	Объявляем  указатель  на  объект полиморфного класса iarduino_I2C_BASE, но в конструкторе данного класса этому указателю будет присвоена ссылка на производный класс iarduino_I2C.
	/**	Внутренние функции **/																											//
		bool				_readBytes				(uint8_t	, uint8_t										);						//	Объявляем  функцию чтения данных в  массив  data					(Параметры:  номер первого регистра, количество байт).
		bool				_writeBytes				(uint8_t	, uint8_t, uint8_t=0							);						//	Объявляем  функцию записи данных из массива data					(Параметры:  номер первого регистра, количество байт, номер первого элемента массива data).
};																																		//
																																		//
#endif																																	//