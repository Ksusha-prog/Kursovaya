/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл модуля communicator
*/
#include "communicator.h"
communicator::communicator(journal& log,base& base_r,uint p):l(log),b(base_r){
    port=p;
}
void communicator:: work(){
    start();
    while (true)
    {
        connect_to_client();
        authentification();
        data_handler handler(*this) ;
        handler.handle_calculation();
    }
}
void communicator::start(){
    data=b.get_personal_data();
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        l.write_journal("Ошибка при создании сокета сервера");
        throw fatal_error ("Сокет не был создан");
    }
    l.write_journal("Сокет для сервера создан");
    std::cout << "Сокет создан" << std::endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        l.write_journal("Ошибка при привязке сокета");
        throw fatal_error ("Сокет не был привязан");
    }
    l.write_journal("Cокет привязан");
    std::cout << "Сокет привязан" << std::endl;
}
void communicator::connect_to_client(){
    if (listen(serverSocket, 10) == 0) {
        std::cout << "Сервер слушает..." << std::endl;
        l.write_journal("Сервер встал на прослушку порта");
    } else {
        l.write_journal("Ошибка при прослушивании порта");
        std::cout << "Ошибка при прослушивании" << std::endl;
        throw fatal_error ("Север не встал на прослушку порта");
    }
    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
    if(clientSocket<0) {
        l.write_journal("Ошибка при аутентификации: ошибка принятия соединения клиента");
        std::cout<<"Ошибка принятия соединения клиента"<<std::endl;
        close(clientSocket);
    } else {
        l.write_journal("Соединение с клиентом установлено");
        std::cout << "Соединение установлено" << std::endl;
    }
    id=recv_data("Ошибка при приеме айди клиента");
    std::cout<<"Подсоединился пользователь: "+id<<std::endl;
}
int communicator::authentification(){
    std::string password;
    auto it = data.find(id);
    if (it != data.end()) {
        password=it->second;
    } else {
        send_data("ERR","Ошибка при отправке ERR");
        throw fatal_error("Пользователь не найден");
    }
    std::string salt=SALT_generate();
    l.write_journal("Начало фазы аутентификации для пользователя: "+id);
    std::cout << "Фаза аутентификации для пользователя: "<<id<< std::endl;
    send_data(salt,"Ошибка при отправке соли");
    l.write_journal("Отправлена соль");
    std::string cl_hash=recv_data("Ошибка при приеме хеша");
    l.write_journal("От клиента принят хеш");
    std::string serv_hash=hash_gen(salt,password);
    if(serv_hash!=cl_hash){
        l.write_journal("Ошибка при аутентификации: хеши не соответствуют друг другу");
        std::cout<<"Пароль неверный"<<std::endl;
        send_data("ERR","Ошибка при отправке ERR");
        close_sock();
        return 1;
    }
    std::cout<<"Пользователь "<<id<<" аутентифицорован"<<std::endl;
    send_data("OK","Ошибка при отправке OK");
    return 0;
}  
std::string communicator::recv_data(std::string messg){
    int rc=0;
    while (true) {
        buffer = std::unique_ptr<char[]>(new char[buflen]); 
        rc = recv(clientSocket, buffer.get(), buflen, MSG_PEEK);
        if (rc==0){
            close_sock();
            l.write_journal("Server: Клиент закрыл соединение");
        }
        else if (rc<0){
            close_sock();
            l.write_journal(messg);
        }
        if (rc < buflen) 
            break; 
        buflen *= 2; 
    }
    std::string msg(buffer.get(), rc);
    recv(clientSocket, nullptr, rc, MSG_TRUNC);
    return msg;
}
std::string communicator::SALT_generate()
{
    std::random_device rd;
    std::seed_seq seed{rd(), rd(), rd(), rd(), rd()};
    std::mt19937_64 rnd(seed);
    uint64_t int_salt=rnd();
    std::string hex_salt=convert_to_hex(int_salt);
    while(hex_salt.length()<16) {
        hex_salt="0"+hex_salt;
    }
    l.write_journal("Соль сгенерирована");
    return hex_salt;

}
std::string communicator::convert_to_hex(uint64_t x)
{
    std::stringstream ss;
    ss << std::hex << x;
    return ss.str();
}
void communicator::close_sock(){
    close(clientSocket);
    l.write_journal("Разорвано соединение с клиентом");
}
std::string communicator::hash_gen(std::string &salt,std::string &password){
    CryptoPP::Weak::MD5 hash;
    std::string serv_hash;
    CryptoPP::StringSource(salt+ password, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(serv_hash))));
    return serv_hash;
}
void communicator::send_data(const std::string& data, const std::string& msg) {
    int sb = send(clientSocket, data.c_str(), data.length(), 0);
    if (sb < 0) {
        l.write_journal(msg + ": Ошибка отправки данных");
        close_sock();
    }
}
