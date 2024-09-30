/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Главный файл проекта
*/
#include "journal.h"
#include "base.h"
#include "communicator.h"
#include "interface.h"
#include "error.h"
#include "data_handler.h"
int main(int argc, char* argv[])
{   
    try{
        interface UI(argc,argv);
        journal logger(UI.get_log_destination());
        base base_reader(logger,UI.get_base_destination());
        communicator serv(logger,base_reader,UI.get_port());
        serv.work();
    }catch (po::error& e) {
        std::cout << e.what() << std::endl;
    }
    catch(fatal_error &e){
        std::cout<<"Ошибка: "<<e.what()<<std::endl;
    }
    return 0;
}
