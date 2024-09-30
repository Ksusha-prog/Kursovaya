/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля interface
*/
#pragma once
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "error.h"
namespace po = boost::program_options;
/** @brief Класс пользовательского интерфейса
* @details Комстрока разбирается в конструкторе, обработана ситуация, когда не введен ни один из параметров
* Поддерживается ввод нескольких значений в параметр. Считается последнее введенное
*/
class interface{
    public:
    /** @brief Описание запрашиваемых параметров в комстроке
    */
    po::options_description desc;
    /** @brief Разобранные значения параметров комстроки
    */
    po::variables_map vm;
    /**
    * @brief Конструктор инициализации
    * @details Разбор комстроки
    * @param [in] argc Количество аргументов комстроки
    * @param [in] argv Значения аргументов комстроки
    */
    interface(int argc, char* argv[]);
    /**
    * @brief Геттер порта
    * @return Значение порта
    * @throw fatal_error при системном порте
    */
    uint get_port();
    /**
    * @brief Геттер расоложения базы данных
    */
    std::string get_base_destination();
    /**
    * @brief Геттер расоложения файла журнала
    */
    std::string get_log_destination();
};