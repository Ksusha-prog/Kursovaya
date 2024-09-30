/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля data_handler
*/
#pragma once
#include "communicator.h"
#include "error.h"
#include "journal.h"
#include <limits>
#include <iostream>
#include <chrono>
#include <thread>
class communicator;
/** @brief Класс обработки данных
* @details Прием векторов от клиента и выполнение соответствующих вычислений, отправка результата вычислений
*/
class data_handler{
    public:
    /**
    * @brief Объект класса communicator для отправки результата и приема значений вектора
    */
    communicator& s;
    /**
    * @brief Количество векторов
    */
    uint32_t num_of_vec;
    /**
    * @brief Метод вычисления произведения вектора
    * @param [in] number1 Первый множитель
    * @param [in] number2 Второй множитель
    * @warning При переполнении возвращается максимум или минимум типа int64_t
    * @return Произведение двух чисел
    */
    int64_t calculator(int64_t number1, int64_t number2);
    /**
    * @brief Обработка данных от клиента
    * @details Прием значений вектора, выполнение соответствующих вычислений
    * @warning Если клиент отправил неверный тип данных, то соединение закрывается
    */
    int handle_calculation();
    /**
    * @brief Конструктор инициализации
    * @details Инициализация объектов communicator по ссылке
    */
    data_handler(communicator& serv):s(serv){}
};