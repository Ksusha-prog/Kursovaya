/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл модуля journal
*/
#include "journal.h"
journal::journal(std::string destination){
    if(!boost::filesystem::exists(destination)){
        std::cout<<"Файла журнала с заданным именем не существует"<<std::endl;
        throw fatal_error ("Не удалось открыть лог файл");
    }
    l.open(destination, std::ios::app | std::ios::out);
}
int journal::write_journal(std::string message){
    if (!l.is_open()) {
        std::cerr << "Ошибка: файл лога не открыт для записи!" << std::endl;
        throw fatal_error ("Файл лога не открыт для записи");
        return 1;
    }
    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    std::string time = std::ctime(&end_time);
    time.pop_back();
    l<<time<<" ===> "<<message<<'\n';
    l.flush();
    return 0;
}
journal::~journal(){
    l.close();
}