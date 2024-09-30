#include <iostream>
#include <limits>
#include "includer.h"
#include <UnitTest++/UnitTest++.h>
#include <random>
#include <thread>
#include <map>
SUITE(logger_test){
    TEST(closed_file){
        journal log("l.txt");
        log.l.close();
        CHECK_THROW(log.write_journal("closed_test"),fatal_error);
    }
}
SUITE(data_handler_test){
    TEST(max_overflow){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        data_handler handle(serv);
        int64_t temp = std::numeric_limits<int64_t>::max();
        int64_t res = handle.calculator(temp,temp);
        CHECK_EQUAL(std::numeric_limits<int64_t>::max(),res);

    }
    TEST(min_overflow){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        data_handler handle(serv);
        int64_t temp = std::numeric_limits<int64_t>::min();
        int64_t temp1 = std::numeric_limits<int64_t>::max();
        int64_t res = handle.calculator(temp,temp1);
        CHECK_EQUAL(std::numeric_limits<int64_t>::min(),res);

    }
    TEST(standart_numbers){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        data_handler handle(serv);
        int64_t temp = 123;
        int64_t temp1 = 345;
        int64_t res = handle.calculator(temp,temp1);
        CHECK_EQUAL(temp*temp1,res);

    }
    TEST(negative_nums){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        data_handler handle(serv);
        int64_t temp = -123;
        int64_t temp1 = -345;
        int64_t res = handle.calculator(temp,temp1);
        CHECK_EQUAL(temp*temp1,res);
    }
    TEST(num_with_point){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        data_handler handle(serv);
        int64_t temp = 6;
        int64_t temp1 = 6.6;
        int64_t res = handle.calculator(temp,temp);
        CHECK_EQUAL(36,res);
    }
}
SUITE(server_test){
    TEST(salt_gen_test){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        std::string salt1 = serv.SALT_generate();
        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);
        std::string salt2 = serv.SALT_generate();
        if(salt1==salt2){
            CHECK(false);
        }
        else{
            CHECK(true);
        }
    }
    TEST(length_of_salt){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        std::string salt1 = serv.SALT_generate();
        if(salt1.length()==16){
            CHECK(true);
        }
        else{
            CHECK(false);
        }
    }
    TEST(different_passes_hash_gen){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        std::string salt1 = serv.SALT_generate();
        std::string p1="zxcvb";
        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);
        std::string salt2 = serv.SALT_generate();
        std::string p2="fghjk";
        if(serv.hash_gen(salt1,p1)!=serv.hash_gen(salt2,p2)){
            CHECK(true);
        }
        else{
            CHECK(false);
        }

    }
    TEST(equal_passes_hash_gen){
        journal log("l.txt");
        base bas(log,"b.txt");
        communicator serv(log,bas,33333);
        std::string salt1 = serv.SALT_generate();
        std::string p1="qwerty";
        if(serv.hash_gen(salt1,p1)==serv.hash_gen(salt1,p1)){
            CHECK(true);
        }
        else{
            CHECK(false);
        }
    }

}
SUITE(base_test){
    TEST(data_getter){
        journal log("l.txt");
        base bas(log,"b.txt");
        std::map<std::string,std::string> data=bas.get_personal_data();
        CHECK_EQUAL("password",data["login"]);
    }
}
int main(){
    return UnitTest::RunAllTests();
}