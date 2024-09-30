/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля journal
*/
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>
#include "error.h"
#include <boost/filesystem.hpp>
/** @brief Класс записи сообщений в журнал
* @details Обработана ситуация при закрытом файле после открытия, запись сообщений в методе write_journal
* При ошибке открытия файла выключается сервер
*/
class journal{
    public:
        /** @brief Объект ofstream для открытия файла журнала
        */
        std::ofstream l;
        /**
        * @brief Конструктор инициализации
        * @param [in] destination Расположение файла журнала
        * @details Открывается файл
        * @throw fatal_error, если файла не существует
        */
        journal(std::string destination);
        /**
        * @brief Метод записи сообщения в лог файл
        * @details Записывается сообщение в формате время ===> сообщение
        * Файл закрывается в конце работы
        * @param [in] message Сообщение для записи
        * @throw fatal_error, если файл закрыт
        */
        int write_journal(std::string message);
        /**
        * @brief Деструктор
        * @details Закрывается файл
        */
        ~journal();
};       