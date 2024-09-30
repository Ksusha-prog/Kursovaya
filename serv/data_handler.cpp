/** @file
* @author Маркина К.А.
* @version 1.0
* @date 21.06.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл модуля data_handler
*/
#include "data_handler.h"
int data_handler::handle_calculation(){
    int recv_b = recv(s.clientSocket,&num_of_vec,sizeof num_of_vec,0);
    for (uint32_t i = 0; i < num_of_vec; i++)
    {
        int64_t res=1;
        bool is_first = true; 
        uint32_t size_of_vec;
        int recv_b = recv(s.clientSocket,&size_of_vec,sizeof size_of_vec,0);
        if(recv_b<0){
                s.l.write_journal("Ошибка принятия размера вектора");
                return 1;
            }
        s.l.write_journal("Получен размер вектора: "+std::to_string(size_of_vec));
        for (uint32_t j = 0; j < size_of_vec; j++)
        {
            int64_t data;
            std::chrono::milliseconds duration(100);
            std::this_thread::sleep_for(duration);
            int rec = recv(s.clientSocket,&data,sizeof data,MSG_DONTWAIT);
            if(rec!=8){
                s.l.write_journal("Получен неверный тип данных");
                s.close_sock();
                return 1;
            }
            s.l.write_journal("Получены данные вектора");
            if (is_first) {
                res = data;
                is_first = false;
            } else {
                res = calculator(res, data);
            }
        }
        int send_b=send(s.clientSocket,&res,sizeof res,0);
        if(send_b<0){
                s.l.write_journal("Ошибка отправки результата вычислений");
                s.close_sock();
                return 1;
            }
        s.l.write_journal("Отправлен результат вычислений: "+std::to_string(res));
    }
    
}
int64_t data_handler::calculator(int64_t number1, int64_t number2) {
    if (number1 == 0 or number2 == 0) {
        return 0;
    }
    if (number1 > 0 and number2 > 0 and number1 > (std::numeric_limits<int64_t>::max() / number2)) {
        return std::numeric_limits<int64_t>::max();
    }
    if (number1 < 0 and number2 < 0 and (number1 < std::numeric_limits<int64_t>::min() or number2 < std::numeric_limits<int64_t>::min())) {
        return std::numeric_limits<int64_t>::max();
    }
    if ((number1 > 0 and number2 < 0 and number2 < (std::numeric_limits<int64_t>::min() / number1)) or
        (number1 < 0 and number2 > 0 and number1 < (std::numeric_limits<int64_t>::min() / number2))) {
        return std::numeric_limits<int64_t>::min();
    }
    return number1 * number2;
}
