/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyListener.h
 * Author: rohitsharma
 *
 * Created on 23 June, 2018, 11:15 PM
 */

#ifndef MYLISTENER_H
#define MYLISTENER_H

#include "Listener.h"
#include <unordered_map>
#include <iostream>

/* 
 * At all functions The following invariant holds
 * Min POV = COV + negDeltas
 * Max POV = COV + posDeltas
 * 
 * On new Order : 
 *  posDeltas += orderValue
 * 
 * On new Order Confirmation : 
 *  COV += orderValue
 *  posDeltas -= orderValue
 * 
 * On new Order Rejection : 
 *  posDeltas -= orderValue
 * 
 * On Replacement Order  : 
 *  if orderValue >= 0 :
 *      posDeltas += orderValue
 *  else if orderValue <  0 :
 *      negDeltas += orderValue
 * 
 * On Replacement Order Confirmation: 
 *  if orderValue >= 0 :
 *      COV += orderValue
 *      posDeltas -= orderValue 
 *  else if orderValue <  0 :
 *      COV += orderValue
 *      negDeltas -= orderValue  
 * 
 * On Replacement Order Rejection : 
 *  if orderValue >= 0 :
 *      posDeltas -= orderValue
 *  else if orderValue <  0 :
 *      negDeltas -= orderValue
 * 
 */

/*
 *  Assumptions
 * Can update only a previous transaction. If no old order is found neglect the update request.
 * 
 */

class Order {
public:

    Order() : id(0), oldId(-1), side(' '), price(0.0), qty(0), status(' ') {

    }

    Order(int id, int oldId, char side, double price, int qty, char status) :
    id(id), oldId(oldId), side(side), price(price), qty(qty), status(status) {
    }

    Order(const Order& other) :
    id(other.id), oldId(other.oldId), side(other.side), price(other.price), qty(other.qty),
    status(other.status) {
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Order& obj) {
        // Write obj to stream
        os << obj.id << ";";
        os << obj.oldId << ";";
        os << obj.side << ";";
        os << obj.qty << ";";
        os << obj.price << ";";
        os << obj.status << ";";
        return os;
    }

    
    int id;
    int oldId;
    char side;
    double price;
    int qty;
    char status; // New {N} , Accepted {A} , Rejected {R}
};

typedef std::unordered_map<int, Order> Orders;
struct RunningTotal {
        int nfq;

        struct bid {
            double cov;
            double min_pov;
            double max_pov;
            double negDeltas;
            double posDeltas;
        } bid;

        struct offer {
            double cov;
            double min_pov;
            double max_pov;
            double negDeltas;
            double posDeltas;
        } offer;
    } ;

class MyListener : public Listener {
public:
        
    MyListener();
    MyListener(const MyListener& orig);
    virtual ~MyListener();

    // These two callbacks represent client requests.
    // Indicates the client has sent a new order request to the market. Exactly one callback will follow:

    // * 'OnOrderRequestAcknowledged', in which case order 'id' is active in the market; or
    // * 'OnRequestRejected', in which case the order was never active in the market
    virtual void OnInsertOrderRequest(
            int id,
            char side, // 'B' for bid, 'O' for offer
            double price,
            int quantity);

    // Indicates the client has sent a request to change the quantity of an order.
    // Exactly one callback will follow:

    // * 'OnRequestAcknowledged', in which case the order quantity was modified and the
    // order is now tracked by ID 'newId'; or
    // * 'OnRequestRejected', in which case the order was not modified and remains
    // tracked by ID 'oldId'.

    virtual void OnReplaceOrderRequest(
            int oldId, // The existing order to modify
            int newId, // The new order ID to use if the modification succeeds
            int deltaQuantity); // How much the quantity should be increased/decreased

    // These three callbacks represent market confirmations.
    // Indicates the insert or modify request was accepted.

    virtual void OnRequestAcknowledged(
            int id);

    // Indicates the insert or modify request was rejected.

    virtual void OnRequestRejected(
            int id);

    // Indicates that the order quantity was reduced (and filled) by 'quantityFilled'.

    virtual void OnOrderFilled(
            int id,
            int quantityFilled);

    RunningTotal GetRunningTotal() const {
        return runningTotal;
    }
    
    void printOrderTable(){
        std::cout << std::endl << "Order Table = " << std::endl;
        std::cout << std::endl << "Id;" << "OldId;" << "Side;" << "Price;" << "Qty;" << "Status" << std::endl;
        Orders::iterator it = orders.begin();
        for (it =orders.begin(); it !=orders.end();it++ ){
            std::cout << it->second << std::endl;
        }
    }
    
private:
    RunningTotal runningTotal;
    Orders orders;
};

#endif /* MYLISTENER_H */

