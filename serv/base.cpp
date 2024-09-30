/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл модуля base
*/
#include "base.h"
base::base(journal& log,std::string loc):l(log){
   base_loc=loc;
   database.open(base_loc, std::ios::in);
   if(database.is_open()){
        l.write_journal("Файл базы данных успешно открыт");
        read();
        
    }
    else{
        l.write_journal("Ошибка чтения файла базы данных");
        std::cout<<"Указанного файла базы данных не существует"<<std::endl;
        throw fatal_error ("Не удалось прочитать базу данных");
    }
}
void base::read(){
    std::string temp;
    if(boost::filesystem::file_size(base_loc)==0){
        throw fatal_error("Файл с базой данных пустой");
    }
    while(getline(database,temp)){
        if (temp.empty()){
            continue;
        }
        if(std::count(temp.begin(), temp.end(), '/') != 1){
            l.write_journal("Неверный формат данных о пользователе");
            throw fatal_error("Неверный формат данных о пользователе");
        }
        else{
            data.insert(std::make_pair(temp.substr(0, temp.find("/")), 
                                   temp.substr(temp.find("/") + 1)));
        }
    }
}
std::map<std::string,std::string> base::get_personal_data(){
    return data;
}
base::~base(){
    database.close();
}
