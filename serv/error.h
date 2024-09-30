/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для модуля error
*/
#pragma once
#include <stdexcept>
#include <string>
/** @brief Класс ошибок
* @details Обозначает специфические ошибки сервера
*/
class fatal_error:public std::runtime_error{
    public:
    /** @brief Конструктор ошибки
    * @param [in] s Сообщение об ошибке
    */
    fatal_error(const std::string& s):std::runtime_error(s){}
};