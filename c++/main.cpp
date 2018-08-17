/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: rohitsharma
 *
 * Created on 2 July, 2018, 3:22 PM
 */

#include <cstdlib>

#include "ThreadPool.h"

using namespace std;
#include "ThreadPool.h"

/*
 * 
 */
int main(int argc, char** argv) {

    ThreadPool< int, 4 > pool(4);
    pool.start_pool();
    try {
        auto f = pool.submit_task([]() ->int {
            return (3); });
    }    
    catch (std::exception ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}

