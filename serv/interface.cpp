/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл модуля interface
*/
#include "interface.h"
#include "error.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
interface::interface(int argc, char* argv[]){
    desc.add_options()
    ("help,h", "Помощь")
    ("Log_destination,l", po::value<std::vector<std::string>>()->multitoken(), "Расположение файла журнала")
    ("Port,p", po::value<std::vector<uint>>()->multitoken(), "Порт сервера")
    ("Base_destination,b", po::value<std::vector<std::string>>()->multitoken(), "Расположение базы данных");
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
            std::cout << desc << std::endl;
            exit(0);  
        }
}
uint interface::get_port()
{
    if (vm.count("Port") and !vm["Port"].as<std::vector<uint>>().empty() ) {
        const std::vector<uint>& ports = vm["Port"].as<std::vector<uint>>();
        if(ports.back()<1024){
            throw fatal_error("Выбран системный порт");
            
        }
        return ports.back();
    } else {
        std::cout<<"Ошибка при вводе параметров, воспользуйтесь командой help"<<std::endl;
        exit(1);
        return 1;
    }
}
std::string interface::get_base_destination()
{
    if (vm.count("Base_destination") and !vm["Base_destination"].as<std::vector<std::string>>().empty()) {
        const std::vector<std::string>& base_loc = vm["Base_destination"].as<std::vector<std::string>>();
        return base_loc.back();
    } else {
        std::cout<<"Ошибка при вводе параметров, воспользуйтесь командой help"<<std::endl;
        exit(1);
        return "";
    }
}
std::string interface::get_log_destination()
{
    if (vm.count("Log_destination") and !vm["Log_destination"].as<std::vector<std::string>>().empty()) {
        const std::vector<std::string>& log_loc = vm["Log_destination"].as<std::vector<std::string>>();
        return log_loc.back();
    } else {
        std::cout<<"Ошибка при вводе параметров, воспользуйтесь командой help"<<std::endl;
        exit(1);
        return "";
    }
}