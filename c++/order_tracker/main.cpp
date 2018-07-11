//
//  main.cpp
//  Order Tracking
//
//  Created by rohit sharma on 23/06/18.
//  Copyright © 2018 rohit sharma. All rights reserved.
//
#include <iostream>
#include "MyListener.h"

void printTotal(RunningTotal&& runningTotal) {
    std::cout << runningTotal.nfq;
    std::cout << "; " << runningTotal.bid.cov;
    std::cout << "; " << runningTotal.offer.cov;
    std::cout << "; " << runningTotal.bid.min_pov;
    std::cout << "; " << runningTotal.bid.max_pov;
    std::cout << "; " << runningTotal.offer.min_pov;
    std::cout << "; " << runningTotal.offer.max_pov << std::endl;
}


int main(int argc, const char * argv[]) {
    
    MyListener my_listener;

    my_listener.OnInsertOrderRequest(1, 'B', 10.0, 10);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnRequestAcknowledged(1);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnInsertOrderRequest(2, 'O', 15.0, 25);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnRequestAcknowledged(2);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnOrderFilled(1, 5);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnOrderFilled(1, 5);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnReplaceOrderRequest(2, 3, 10);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnOrderFilled(2, 25);
    printTotal(my_listener.GetRunningTotal());
    my_listener.OnRequestAcknowledged(3);
    printTotal(my_listener.GetRunningTotal());
    
    //For Debugging
    my_listener.printOrderTable();

    return 0;
}
