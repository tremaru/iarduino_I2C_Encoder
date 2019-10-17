#include "iarduino_I2C_Encoder.h"																								//
																																//
//		Инициализация модуля:																									//	Возвращаемое значение: результат инициализации.
bool	iarduino_I2C_Encoder::begin			(void){																				//	Параметр: отсутствует
		//	Инициируем работу с шиной I2C:																						//
			objI2C->begin(100);																									//	Инициируем передачу данных по шине I2C на скорости 100 кГц.
		//	Если адрес не указан, то ищим модуль на шине I2C:																	//
			if(valAddrTemp==0){																									//
				for(int i=1; i<127; i++){																						//	Проходим по всем адресам на шине I2C
					if( objI2C->checkAddress(i)											){	valAddr=i; delay(2);				//	Если на шине I2C есть устройство с адресом i, то используем этот адрес для проверки найденного модуля...
					if(_readBytes(REG_MODEL,4)											){										//	Читаем 4 байта начиная с регистра «REG_MODEL» в массив «data».
					if( data[0]     == DEF_MODEL_ENC									){										//	Если у модуля с адресом i в регистре «MODEL»   (data[0]) хранится значение DEF_MODEL_ENC, то ...
					if((data[2]>>1) == i                 || data[2] == 0xFF				){										//	Если у модуля с адресом i в регистре «ADDRESS» (data[2]) хранится значение i (адрес+младший бит) или 0xFF (адрес не задавался), то ...
					if( data[3]     == DEF_CHIP_ID_FLASH || data[3] == DEF_CHIP_ID_METRO){										//	Если у модуля с адресом i в регистре «CHIP_ID» (data[3]) хранится значение DEF_CHIP_ID_FLASH (идентификатор модулей Flash), или DEF_CHIP_ID_METRO (идентификатор модулей Metro), то ...
						valAddrTemp=i; i=128;																					//	Считаем что модуль обнаружен, сохраняем значение i как найденный адрес и выходим из цикла.
					}}}}}																										//
				}																												//
			}																													//
		//	Если модуль не найден, то возвращаем ошибку инициализации:															//
			if( valAddrTemp == 0														){	valAddr=0; return false;}			//
		//	Проверяем наличие модуля на шине I2C:																				//
			if( objI2C->checkAddress(valAddrTemp) == false								){	valAddr=0; return false;}			//	Если на шине I2C нет устройств с адресом valAddrTemp, то возвращаем ошибку инициализации
			valAddr=valAddrTemp;																								//	Сохраняем адрес модуля на шине I2C.
		//	Проверяем значения регистров модуля:																				//
			if(_readBytes(REG_MODEL,4)==false											){	valAddr=0; return false;}			//	Если не удалось прочитать 4 байта в массив «data» из модуля начиная с регистра «REG_MODEL», то возвращаем ошибку инициализации.
			if( data[0]     != DEF_MODEL_ENC											){	valAddr=0; return false;}			//	Если значение  регистра «MODEL»   (data[0]) не совпадает со значением DEF_MODEL_ENC, то возвращаем ошибку инициализации.
			if((data[2]>>1) != valAddrTemp       && data[2] !=0xFF						){	valAddr=0; return false;}			//	Если значение  регистра «ADDRESS» (data[2]) не совпадает с адресом модуля и не совпадает со значением 0xFF, то возвращаем ошибку инициализации.
			if( data[3]     != DEF_CHIP_ID_FLASH && data[3] != DEF_CHIP_ID_METRO		){	valAddr=0; return false;}			//	Если значение  регистра «CHIP_ID» (data[3]) не совпадает со значением DEF_CHIP_ID_FLASH и DEF_CHIP_ID_METRO, то возвращаем ошибку инициализации.
			valVers=data[1];																									//	Сохраняем байт регистра «VERSION» (data[1]) в переменую «valVers».
		//	Перезагружаем модуль устанавливая его регистры в значение по умолчанию:												//
			reset();																											//	Выполняем программную перезагрузку.
			delay(5);																											//
			return true;																										//	Возвращаем флаг успешной инициализаии.
}																																//
																																//
//		Перезагрузка модуля:																									//	Возвращаемое значение:	результат перезагрузки.
bool	iarduino_I2C_Encoder::reset			(void){																				//	Параметр:				отсутствует.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Устанавливаем бит перезагрузки:																					//
				if(_readBytes(REG_BITS_0,1)==false){return false;}																//	Читаем 1 байт регистра «BITS_0» в массив «data».
				data[0] |= 0b10000000;																							//	Устанавливаем бит «SET_RESET»
				if(_writeBytes(REG_BITS_0,1)==false){return false;}																//	Записываем 1 байт в регистр «BITS_0» из массива «data».
			//	Ждём установки флага завершения перезагрузки:																	//
				do{ if(_readBytes(REG_FLAGS_0,1)==false){return false;} }														//	Читаем 1 байт регистра «REG_FLAGS_0» в массив «data».
				while( (data[0]&0b10000000) == 0);																				//	Повторяем чтение пока не установится флаг «FLG_RESET».
				return true;																									//
			}else{																												//	Иначе, если энкодер не инициализирован, то ...
				return false;																									//	Возвращаем ошибку
			}																													//
}																																//
																																//
//		Смена адреса модуля:																									//	Возвращаемое значение:	резульат смены адреса.
bool	iarduino_I2C_Encoder::changeAddress	(uint8_t newAddr){																	//	Параметр:				newAddr - новый адрес модуля (0x07 < адрес < 0x7F).
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Проверяем новый адрес:																							//
				if(newAddr>0x7F){newAddr>>=1;}																					//	Корректируем адрес, если он указан с учётом бита RW.
				if(newAddr==0x00 || newAddr==0x7F){return false;}																//	Запрещаем устанавливать адрес 0x00 и 0x7F.
			//	Записываем новый адрес:																							//
				if(_readBytes(REG_BITS_0,1)==false){return false;}																//	Читаем 1 байт регистра «BITS_0» в массив «data».
				data[0] |= 0b00000010;																							//	Устанавливаем бит «SAVE_ADR_EN»
				if(_writeBytes(REG_BITS_0,1)==false){return false;}																//	Записываем 1 байт в регистр «BITS_0» из массива «data».
				data[0] = (newAddr<<1)|0x01;																					//	Готовим новый адрес к записи в модуль, установив бит «SAVE_FLASH».
				if(_writeBytes(REG_ADDRESS,1)==false){return false;}															//	Записываем 1 байт в регистр «ADDRESS» из массива «data».
				delay(200);																										//	Даём более чем достаточное время для применения модулем нового адреса.
			//	Проверяем наличие модуля с новым адресом на шине I2C:															//
				if(objI2C->checkAddress(newAddr)==false){return false;}															//	Если на шине I2C нет модуля с адресом newAddr, то возвращаем ошибку.
				valAddr     = newAddr;																							//	Сохраняем новый адрес как текущий.
				valAddrTemp = newAddr;																							//	Сохраняем новый адрес как указанный.
				return true;																									//	Возвращаем флаг успеха.
			}else{																												//	Иначе, если энкодер не инициализирован, то ...
				return false;																									//	Возвращаем ошибку
			}																													//
}																																//
																																//
//		Получение времени удержания, состояния и события кнопки энкодера:														//	Возвращаемое значение:	время удержания, состояние или событие.
uint16_t iarduino_I2C_Encoder::getButton	(uint8_t type){																		//	Параметр:				тип получаемого состояния или собития.
			uint16_t f=0;																										//	
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Дублируем состояние битов регистра «REG_ENC_KEY» в переменную «val_REG_ENC_KEY»:								//
				if( _readBytes(REG_ENC_KEY,2) ){																				//	Читаем 2 байта из регистров «REG_ENC_KEY» и «REG_ENC_KEY_TIM» в массив «data».
					val_REG_ENC_KEY	|= ( data[0] & ENC_KEY_PUSHED   );															//	Устанавливаем флаг события   кнопки - «нажимается».						Если бит ENC_KEY_PUSHED   в переменной val_REG_ENC_KEY был установлен, то он и останется установленным!
					val_REG_ENC_KEY	|= ( data[0] & ENC_KEY_RELEASED );															//	Устанавливаем флаг события   кнопки - «отпускается».					Если бит ENC_KEY_RELEASED в переменной val_REG_ENC_KEY был установлен, то он и останется установленным!
					if(data[0]&ENC_KEY_PRESSED){val_REG_ENC_KEY|=ENC_KEY_PRESSED;}else{val_REG_ENC_KEY&=~ENC_KEY_PRESSED;}		//	Определяем    флаг состояния кнопки - «нажата».
					if(data[0]&ENC_KEY_TRIGGER){val_REG_ENC_KEY|=ENC_KEY_TRIGGER;}else{val_REG_ENC_KEY&=~ENC_KEY_TRIGGER;}		//	Определяем    флаг состояния кнопки - «переключатель»					Меняет состояние с каждым новым нажатием.
					if(data[0]&ENC_KEY_HOLD_05){val_REG_ENC_KEY|=ENC_KEY_HOLD_05;}else{val_REG_ENC_KEY&=~ENC_KEY_HOLD_05;}		//	Определяем    флаг состояния кнопки - «удерживается» дольше 0,5 сек.
					if(data[0]&ENC_KEY_HOLD_10){val_REG_ENC_KEY|=ENC_KEY_HOLD_10;}else{val_REG_ENC_KEY&=~ENC_KEY_HOLD_10;}		//	Определяем    флаг состояния кнопки - «удерживается» дольше 1,0 сек.
					if(data[0]&ENC_KEY_HOLD_20){val_REG_ENC_KEY|=ENC_KEY_HOLD_20;}else{val_REG_ENC_KEY&=~ENC_KEY_HOLD_20;}		//	Определяем    флаг состояния кнопки - «удерживается» дольше 2,0 сек.
					val_REG_ENC_KEY	|= ( data[0] & ENC_KEY_CHANGED );															//	Устанавливаем флаг события   кнопки - «состояние изменилось».			Если бит ENC_KEY_CHANGED  в переменной val_REG_ENC_KEY был установлен, то он и останется установленным!
				}																												//
			//	Определяем возвращаемое значение:																				//
				switch(type){																									//
					case KEY_PUSHED		:	f=(val_REG_ENC_KEY & ENC_KEY_PUSHED  );	val_REG_ENC_KEY&=~ENC_KEY_PUSHED;	break;	//	Возвращаемым значениям является флаг  события   кнопки - «нажимается».
					case KEY_RELEASED	:	f=(val_REG_ENC_KEY & ENC_KEY_RELEASED);	val_REG_ENC_KEY&=~ENC_KEY_RELEASED;	break;	//	Возвращаемым значениям является флаг  события   кнопки - «отпускается».
					case KEY_PRESSED	:	f=(val_REG_ENC_KEY & ENC_KEY_PRESSED );										break;	//	Возвращаемым значениям является флаг  состояния кнопки - «нажата».
					case KEY_TRIGGER	:	f=(val_REG_ENC_KEY & ENC_KEY_TRIGGER );										break;	//	Возвращаемым значениям является флаг  триггера  кнопки - «переключатель».
					case KEY_HOLD_05	:	f=(val_REG_ENC_KEY & ENC_KEY_HOLD_05 );										break;	//	Возвращаемым значениям является флаг  состояния кнопки - «удерживается» дольше 0,5 сек.
					case KEY_HOLD_10	:	f=(val_REG_ENC_KEY & ENC_KEY_HOLD_10 );										break;	//	Возвращаемым значениям является флаг  состояния кнопки - «удерживается» дольше 1,0 сек.
					case KEY_HOLD_20	:	f=(val_REG_ENC_KEY & ENC_KEY_HOLD_20 );										break;	//	Возвращаемым значениям является флаг  состояния кнопки - «удерживается» дольше 2,0 сек.
					case KEY_CHANGED	:	f=(val_REG_ENC_KEY & ENC_KEY_CHANGED );	val_REG_ENC_KEY&=~ENC_KEY_CHANGED;	break;	//	Возвращаемым значениям является флаг  события   кнопки - «состояние изменилось».
					case KEY_TIME_PRESSED :	f=(val_REG_ENC_KEY & ENC_KEY_PRESSED )? (uint16_t)data[1]*100:0;			break;	//	Возвращаемым значениям является время удержания кнопки в миллисекундах.
				}																												//
			//	Устранение повторных срабатываний флагов:																		//
				if( type == KEY_PUSHED | type == KEY_RELEASED | type == KEY_CHANGED ){delay(1);}								//	Флагам ENC_KEY_PUSHED, ENC_KEY_RELEASED и ENC_KEY_CHANGED может потребоваться время для сброса в регистре REG_ENC_KEY.
			}																													//
		//	Возвращаем результат:																								//
			return f;																											//
}																																//
																																//
//		Получение количества совершённых тактов оборота энкодера:																//	Возвращаемое значение:	количество тактов.
int16_t	iarduino_I2C_Encoder::getEncoder	(uint8_t dir){																		//	Параметр:				направление вращения.
			int16_t f=0; data[0]=0; data[1]=0;																					//
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Читаем количество тактов поворота энкодера:																		//
				_readBytes(REG_ENC_ENC_CNT_LT,2);																				//	Читаем 2 байта из регистров «REG_ENC_ENC_CNT_LT» и «REG_ENC_ENC_CNT_RT» в массив «data».
				val_REG_ENC_ENC_CNT_LT	+=	data[0]; if(val_REG_ENC_ENC_CNT_LT>0xFF){val_REG_ENC_ENC_CNT_LT=0xFF;}				//	Приращаем прочитанное значение тактов поворота энкодера влево  (против ч.с.) к предыдущему
				val_REG_ENC_ENC_CNT_RT	+=	data[1]; if(val_REG_ENC_ENC_CNT_RT>0xFF){val_REG_ENC_ENC_CNT_RT=0xFF;}				//	Приращаем прочитанное значение тактов поворота энкодера вправо (по    ч.с.) к предыдущему
			//	Определяем возвращаемое значение:																				//
				switch(dir){																									//
					case ENC_TURN_LEFT	:	f = val_REG_ENC_ENC_CNT_LT;		val_REG_ENC_ENC_CNT_LT = 0;	break;					//	Возвращаемым значениям является количество тактов поворота энкодера влево  (против ч.с.).
					case ENC_TURN_RIGHT	:	f = val_REG_ENC_ENC_CNT_RT;		val_REG_ENC_ENC_CNT_RT = 0;	break;					//	Возвращаемым значениям является количество тактов поворота энкодера вправо (по    ч.с.).
					case ENC_TURN		:	f = val_REG_ENC_ENC_CNT_RT - 	val_REG_ENC_ENC_CNT_LT;								//	Возвращаемым значениям является разница количества тактов поворота энкодера.
												val_REG_ENC_ENC_CNT_RT = 0;	val_REG_ENC_ENC_CNT_LT = 0;	break;					//
				}																												//
			//	Устранение повторных чтений тактов поворота энкодера:															//
				if( data[0] || data[1] ){delay(1);}																				//	Если считан хотя бы один такт поворота, то ждём пока он не будет сброшен.
			}																													//
		//	Возвращаем результат:																								//
			return f;																											//
}																																//
																																//
//		Получение текущей позиции вала энкодера:																				//	Возвращаемое значение:	текущая позиция вала энкодера.
int16_t	iarduino_I2C_Encoder::getPosition	(void){																				//	Параметр:				отсутствует.
			int16_t f=0; data[0]=0; data[1]=0;																					//
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Читаем текущую позицию вала энкодера:																			//
				_readBytes(REG_ENC_RES_CNT_L,2);																				//	Читаем 2 байта из регистров «REG_ENC_RES_CNT_L» и «REG_ENC_RES_CNT_H» в массив «data».
				f = ((int16_t)(data[1]&~ENC_RES_SIGN)<<8)|data[0]; if( data[1] & ENC_RES_SIGN ){ f*= -1; }						//	Объеденяем два прочитанных байта в одно число и добавляем знак.
			}																													//
		//	Возвращаем результат:																								//
			return f;																											//
}																																//
																																//
//		Сброс текущей позиции вала энкодера:																					//	Возвращаемое значение:	результат сброса.
bool	iarduino_I2C_Encoder::resPosition	(void){																				//	Параметр:				отсутствует.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Читаем текущие настройки счётчика положения:																	//
				if(_readBytes(REG_ENC_RES_SET,1)==false){return false;}															//	Читаем 1 байт из регистра «REG_ENC_RES_SET» в массив «data».
				data[0] |= ENC_RES_RESET;																						//	Устанавливаем в прочитанных данных флаг «ENC_RES_RESET».
			//	Сохраняем новые настройки в модуль:																				//
				if(_writeBytes(REG_ENC_RES_SET, 1)==false){return false;}														//	Записываем 1 байт из массива «data» в регистр «REG_ENC_RES_SET».
			//	Возвращаем результат:																							//
				return true;																									//
			}																													//
			return false;																										//
}																																//
																																//
//		Настройка получения позиции вала энкодера:																				//	Возвращаемое значение:	результат применения новых настроек.
bool	iarduino_I2C_Encoder::setPosSettings(uint8_t turn, bool sign){															//	Параметры: количество полных оборотов, флаг разрешающий вывод отрицательных позиций вала энкодера.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Читаем текущие настройки счётчика положения:																	//
				if(_readBytes(REG_ENC_RES_SET,1)==false){return false;}															//	Читаем 1 байт из регистра «REG_ENC_RES_SET» в массив «data».
				data[0] &= ~(ENC_RES_SIGN_EN | ENC_RES_MAX_TURN);																//	Сбрасываем в прочитанных данных биты «ENC_RES_SIGN_EN» и «ENC_RES_MAX_TURN[5-0]».
				if( sign ){ data[0] |= ENC_RES_SIGN_EN; }																		//	Разрешаем использовать отрицательные значения при выводе позиции вала энкодера.
				data[0] |= (turn & ENC_RES_MAX_TURN);																			//	Указываем максимальное количество полных оборотов, такты которых требуется считать.
			//	Сохраняем новые настройки в модуль:																				//
				if(_writeBytes(REG_ENC_RES_SET, 1)==false){return false;}														//	Записываем 1 байт из массива «data» в регистр «REG_ENC_RES_SET».
			//	Возвращаем результат:																							//
				return true;																									//
			}																													//
			return false;																										//
}																																//
																																//
//		Настройка работы выхода модуля:																							//	Возвращаемое значение:	результат применения новых настроек.
bool	iarduino_I2C_Encoder::setPinOut		(uint8_t mode, uint8_t turn, uint16_t freq){										//	Параметры: режим работы выхода [, количество полных оборотов от минимума до максимума [, частота ШИМ в Гц ]].
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Читаем текущие настройки работы выхода:																			//
				if(_readBytes(REG_ENC_PIN_SET,1)==false){return false;}															//	Читаем 1 байт из регистра «REG_ENC_PIN_SET» в массив «data».
			//	Устанавливаем новый режим работы выхода:																		//
				data[0] &= ~ENC_PIN_MODE;																						//	Сбрасываем в прочитанных данных биты «ENC_PIN_MODE[3-0]».
				data[0] |= (ENC_PIN_MODE & mode);																				//	Меняем биты «ENC_PIN_MODE[3-0]» на значение «mode».
			//	Меняем количество оборотов энкодера требуемое для смены состояния выхода от минимума к максимуму:				//
				if(turn){																										//	Если указано новое значение количества оборотов, то ...
					data[0] &= ~ENC_PIN_MAX_TURN;																				//	Сбрасываем в прочитанных данных биты «ENC_PIN_MAX_TURN[7-4]».
					data[0] |= (ENC_PIN_MAX_TURN & turn<<4);																	//	Меняем биты «ENC_PIN_MAX_TURN[7-4]» на значение «turn».
				}																												//
			//	Сохраняем новые настройки в модуль:																				//
				if(_writeBytes(REG_ENC_PIN_SET, 1)==false){return false;}														//	Записываем 1 байт из массива «data» в регистр «REG_ENC_PIN_SET».
			//	Меняем частоту ШИМ на выходе модуля:																			//
				if(freq){																										//	Если указано новое значение частоты ШИМ, то ...
				if(freq<12000){																									//	Если новое значение частоты ШИМ не превышает 12 кГц, то ...
				//	Готовим два байта для записи:																				//
					data[0]		= uint8_t(freq & 0x00FF);																		//	Устанавливаем младший байт значения «freq» для регистра «REG_ENC_FREQUENCY_L».
					data[1]		= uint8_t(freq >> 8);																			//	Устанавливаем старший байт значения «freq» для регистра «REG_ENC_FREQUENCY_H».
				//	Записываем новую частоту в модуль:																			//
					if(_writeBytes(REG_ENC_FREQUENCY_L, 2)==false){return false;}												//	Записываем 2 байта из массива «data» в модуль, начиная с регистра «REG_ENC_FREQUENCY_L».
				}																												//
				}																												//
				delay(50);																										//	Ждём 50 мс для сохранения настроек в flash память модуля.
			//	Возвращаем результат:																							//
				return true;																									//
			}																													//
			return false;																										//
}																																//
																																//
//		Чтение данных из регистров в массив data:																				//	Возвращаемое значение:	результат чтения (true/false).
bool	iarduino_I2C_Encoder::_readBytes		(uint8_t reg, uint8_t sum){														//	Параметры:				reg - номер первого регистра, sum - количество читаемых байт.
			bool	result=false;																								//	Определяем флаг       для хранения результата чтения.
			uint8_t	sumtry=10;																									//	Определяем переменную для подсчёта количества оставшихся попыток чтения.
			do{	result = objI2C->readBytes(valAddr, reg, data, sum);															//	Считываем из модуля valAddr, начиная с регистра reg, в массив data, sum байт.
				sumtry--;	if(!result){delay(1);}																				//	Уменьшаем количество попыток чтения и устанавливаем задержку при неудаче.
			}	while		(!result && sumtry>0);																				//	Повторяем чтение если оно завершилось неудачей, но не более sumtry попыток.
			delayMicroseconds(500);																								//	Между пакетами необходимо выдерживать паузу.
			return result;																										//	Возвращаем результат чтения (true/false).
}																																//
																																//
//		Запись данных в регистры из массива data:																				//	Возвращаемое значение:	результат записи (true/false).
bool	iarduino_I2C_Encoder::_writeBytes	(uint8_t reg, uint8_t sum, uint8_t num){											//	Параметры:				reg - номер первого регистра, sum - количество записываемых байт, num - номер первого элемента массива data.
			bool	result=false;																								//	Определяем флаг       для хранения результата записи.
			uint8_t	sumtry=10;																									//	Определяем переменную для подсчёта количества оставшихся попыток записи.
			do{	result = objI2C->writeBytes(valAddr, reg, &data[num], sum);														//	Записываем в модуль valAddr начиная с регистра reg, sum байи из массива data начиная с элемента num.
				sumtry--;	if(!result){delay(1);}																				//	Уменьшаем количество попыток записи и устанавливаем задержку при неудаче.
			}	while		(!result && sumtry>0);																				//	Повторяем запись если она завершилась неудачей, но не более sumtry попыток.
			delay(10);																											//	Ждём применения модулем записанных данных.
			return result;																										//	Возвращаем результат записи (true/false).
}																																//
																																//