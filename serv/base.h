/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля base
*/
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include "journal.h"
#include "error.h"
#include <boost/filesystem.hpp>
/** @brief Класс базы данных
* @details Осуществляется работа с базой данных. Получение словаря с ключами - логинами, значеняими - паролями
*/
class base{
    public:
    /**
    * @brief Расположение файла базы данных
    */
    std::string base_loc;
    /**
    * @brief Объект потока ifstream для чтения базы данных
    */
    std::ifstream database;
    /**
    * @brief Словарь с персональными данными. Ключ - логин, пароль - значение
    */
    std::map<std::string,std::string> data;
    /**
    * @brief Объект класса journal для записи сообщений в журнал
    */
    journal& l;
    /**
    * @brief Конструктор инициализации
    * @details Открывается файл базы данных,затем считывается содержимое,которое распределяется по словарю
    * @param [in] log Объект класса journal, передаваемый по ссылке
    * @param [in] loc Расположения файла базы данных
    * @throw critical_error, если не удалось открыть файл с базой данных
    */
    base(journal& log,std::string loc);
    /**
    * @brief Метод чтения базы данных
    * @details Считывается содержимое, предстваленное в формате login/password
    * @throw critical_error, если содержимое базы данных не соответствует заданному в коде формату
    */
    void read();
    /**
    * @brief Геттер персональных данных
    * @return Словарь с персональными данными: логины, пароли
    */
    std::map<std::string,std::string> get_personal_data();
    /**
    * @brief Деструктор модуля
    * @details Закрывается файл с базой данных
    */
    ~base();
};