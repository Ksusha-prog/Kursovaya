/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля communicator
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include "data_handler.h"
#include <random>
#include <chrono>
#include <thread>
#include <limits>
#include "base.h"
#include "journal.h"
#include "error.h"
#include "interface.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
/** @brief Класс коммуникатора
* @details Порт передается с помощью метода класса interface,работу сервера запускает метод work \n
* Необходимые для работы объекты journal и base передаются по ссылке.
*/
class communicator{
    private:
        /**
        * @brief Структуры адреса сервера и клиента
        */
        struct sockaddr_in serverAddr, clientAddr;
        /**
        * @brief Размер адреса
        */
        socklen_t addr_size;
        /**
        * @brief Словарь персональных данных
        */
        std::map<std::string,std::string> data;
        /**
        * @brief Размер буффера
        */
        size_t buflen =1024;
        /**
        * @brief Unique_ptr для приема и отправки сообщений
        */
        std::unique_ptr <char[]> buffer{new char[buflen]};
        /**
        * @brief Порт
        */
        uint port;
    public:
        /**
        * @brief Объект класса journal для ведения записей
        */
        journal& l;
        /**
        * @brief Объект класса base для получения персональных данных
        */
        base &b;
        /**
        * @brief id, присланное клиентом
        */
        std::string id;
        /**
        * @brief Переменная для сокетов сервера и клиента
        */
        int serverSocket, clientSocket;
        /**
        * @brief Соединение с клиентом
        * @details Сервер ожидает подключение клиента
        * @return Функция ничего не возвращает
        * @throw fatal_error, если не удалось встать в режим прослушки
        */
        void connect_to_client();
        /**
        * @brief Аутентификация клиента
        * @details Сравнение присланного хеша со сгенерированным на основе пароля из базы данных
        * При ошибке аутентификации закрывается соединение с клиентом
        * @return Функция возвращает 1, если не удалось аутентифицировать клиента, 0, если аутентификация успешна
        */
        int authentification();
        /**
        * @brief Методи генерации соли
        * @details 64 разрядное число конвертируется в 16-ричное с дополнением 0 до длины 16
        * @return Соль
        */
        std::string SALT_generate();
        /**
        * @brief Конвертация числа в 16-ричную сс
        * @details Число с помощью потока stringstream конвертируется в 16-ричную сс
        * @param [in] d_salt Сгенерированное число
        * @return 64-разрядное число
        */
        std::string convert_to_hex(uint64_t);
        /**
        * @brief Отправка сообщения клиенту
        * @details Отправляется сообщение клиенту, конвертируя string в cstr
        * @param [in] data Строка для отправки
        * @param [in] log_msg Сообщение для журнала
        * @throw При ошибке закрывается соединение с клиетом
        */
        void send_data(const std::string& data, const std::string& msg);
        /**
        * @brief Прием данных от клиента
        * @details В цикле принимается сообщение, если оно превышает размер буфера, то продолжается прием оставшейся части
        * @param [in] msg Строка для хранения принятого сообщения
        * @throw При ошибке закрывается соединение с клиентом
        */
        std::string recv_data(std::string messg);
        /**
        * @brief Генерация хеша
        * @details Хеш генерируется на основе соли и пароля
        * @param [in] salt Соль
        * @param [in] pswd Пароль клиента
        * @return Функция возвращает сгенерированный хеш
        */
        std::string hash_gen(std::string &salt,std::string &password);
        /**
        * @brief Метод закрытия соединения с клиентом
        * Соответствующе сообщение записывается в журнал
        */
        void close_sock();
        /**
        * @brief Запуск сервера
        * @details Создание сокета и привязка к локальному адресу, запись персональных данных в словарь с помощью объекта base
        * @throw critical_error, если не удалось создать или привязать сокет
        */
        void start();
        /**
        * @brief Метод начала работы сервера
        * @details Запуск сервера, прием соединения от клиента, аутентификация, обработка присланных данных
        * @param [in] log Объект класса journal для записи сообщений в журнал
        * @param [in] base_r Объект класса base для получения персональных данных из базы
        * @throw fatal_error или закрытие соединения с клиентом в зависимости от характера ошибки
        */
        void work();
        /**
        * @brief Конструктор инициализации
        * @details Инициализация объектов journal, base по ссылке, установление порта
        */
        communicator(journal& log,base& base_r,uint p);

};
