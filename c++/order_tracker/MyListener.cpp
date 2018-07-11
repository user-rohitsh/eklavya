/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyListener.cpp
 * Author: rohitsharma
 * 
 * Created on 23 June, 2018, 11:15 PM
 */

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

#include "MyListener.h"

MyListener::MyListener() {
    runningTotal.bid.cov = 0.0;
    runningTotal.bid.max_pov = 0.0;
    runningTotal.bid.min_pov = 0.0;
    runningTotal.bid.negDeltas = 0.0;
    runningTotal.bid.posDeltas = 0.0;

    runningTotal.offer.cov = 0.0;
    runningTotal.offer.max_pov = 0.0;
    runningTotal.offer.min_pov = 0.0;
    runningTotal.offer.negDeltas = 0.0;
    runningTotal.offer.posDeltas = 0.0;
    runningTotal.nfq = 0;

}

MyListener::MyListener(const MyListener& orig) {
}

MyListener::~MyListener() {
}

void MyListener::OnInsertOrderRequest(
        int id,
        char side,
        double price,
        int quantity) {

    orders.insert(std::make_pair<int, Order>(id, Order(id,
            -1,
            side,
            price,
            quantity,
            'N')));

    switch (side) {
        case 'O':
            runningTotal.offer.posDeltas += price * quantity;
            runningTotal.offer.min_pov = runningTotal.offer.cov + runningTotal.offer.negDeltas;
            runningTotal.offer.max_pov = runningTotal.offer.cov + runningTotal.offer.posDeltas;
            break;
        case 'B':
            runningTotal.bid.posDeltas += price * quantity;
            runningTotal.bid.min_pov = runningTotal.bid.cov + runningTotal.bid.negDeltas;
            runningTotal.bid.max_pov = runningTotal.bid.cov + runningTotal.bid.posDeltas;
            break;
    }
}

// Indicates the client has sent a request to change the quantity of an order.
// Exactly one callback will follow:

// * 'OnRequestAcknowledged', in which case the order quantity was modified and the
// order is now tracked by ID 'newId'; or
// * 'OnRequestRejected', in which case the order was not modified and remains
// tracked by ID 'oldId'.

void MyListener::OnReplaceOrderRequest(
        int oldId, // The existing order to modify
        int newId, // The new order ID to use if the modification succeeds
        int deltaQuantity) {

    Orders::const_iterator found_old = orders.find(oldId);
    if (found_old == orders.end()) return;

    Order old_order = orders.at(oldId);

    orders.insert(std::make_pair(newId, Order(newId,
            old_order.id,
            old_order.side,
            old_order.price,
            deltaQuantity,
            'N')));

    double deltaValue = old_order.price * deltaQuantity;
   
    char side = old_order.side;
    switch (side) {
        case 'O':
            if (deltaValue >= 0) {
                runningTotal.offer.posDeltas += deltaValue;
            } else {
                runningTotal.offer.negDeltas += deltaValue;
            }
            runningTotal.offer.min_pov = runningTotal.offer.cov + runningTotal.offer.negDeltas;
            runningTotal.offer.max_pov = runningTotal.offer.cov + runningTotal.offer.posDeltas;
            break;
        case 'B':
            if (deltaValue >= 0) {
                runningTotal.bid.posDeltas += deltaValue;
            } else {
                runningTotal.bid.negDeltas += deltaValue;
            }
            runningTotal.bid.min_pov = runningTotal.bid.cov + runningTotal.bid.negDeltas;
            runningTotal.bid.max_pov = runningTotal.bid.cov + runningTotal.bid.posDeltas;
            break;
    }

}

// These three callbacks represent market confirmations.
// Indicates the insert or modify request was accepted.

void MyListener::OnRequestAcknowledged(
        int id) {
    Orders::const_iterator found = orders.find(id);
    if (found == orders.end()) return;
    Order& order = orders.at(id);

    Orders::const_iterator found_old = orders.find(order.oldId);
    if (order.oldId != -1 && found_old == orders.end()) return;

    double deltaValue = order.price * order.qty;
    char side = order.side;
    switch (side) {
        case 'O':
            if (deltaValue >= 0) {
                runningTotal.offer.cov += deltaValue;
                runningTotal.offer.posDeltas -= deltaValue;
            } else {
                runningTotal.offer.cov += deltaValue;
                runningTotal.offer.negDeltas -= deltaValue;
            }
            runningTotal.offer.min_pov = runningTotal.offer.cov + runningTotal.offer.negDeltas;
            runningTotal.offer.max_pov = runningTotal.offer.cov + runningTotal.offer.posDeltas;
            break;
        case 'B':
            if (deltaValue >= 0) {
                runningTotal.bid.cov += deltaValue;
                runningTotal.bid.posDeltas -= deltaValue;
            } else {
                runningTotal.bid.cov += deltaValue;
                runningTotal.bid.negDeltas -= deltaValue;
            }
            runningTotal.bid.min_pov = runningTotal.bid.cov + runningTotal.bid.negDeltas;
            runningTotal.bid.max_pov = runningTotal.bid.cov + runningTotal.bid.posDeltas;
            break;
    }


    if (found_old != orders.end()) {
        //replacement order
        Order& oldOrder = orders.at(order.oldId);
        order.qty += oldOrder.qty;
        oldOrder.qty = 0;
        oldOrder.status='R';
    }
    order.status='A';
}


// Indicates the insert or modify request was rejected.

void MyListener::OnRequestRejected(
        int id) {
    Orders::const_iterator found = orders.find(id);
    if (found == orders.end()) return;

    Order& order = orders.at(id);
    double deltaValue = order.price * order.qty;
    char side = order.side;
    switch (side) {
        case 'O':
            if (deltaValue >= 0) {
                runningTotal.offer.posDeltas -= deltaValue;
            } else {
                runningTotal.offer.negDeltas -= deltaValue;
            }
            runningTotal.offer.min_pov = runningTotal.offer.cov + runningTotal.offer.negDeltas;
            runningTotal.offer.max_pov = runningTotal.offer.cov + runningTotal.offer.posDeltas;
            break;
        case 'B':
            if (deltaValue >= 0) {
                runningTotal.bid.posDeltas -= deltaValue;
            } else {
                runningTotal.bid.negDeltas -= deltaValue;
            }
            runningTotal.bid.min_pov = runningTotal.bid.cov + runningTotal.bid.negDeltas;
            runningTotal.bid.max_pov = runningTotal.bid.cov + runningTotal.bid.posDeltas;
            break;
    }
    order.qty = 0;
    order.status='R';
}

// Indicates that the order quantity was reduced (and filled) by 'quantityFilled'.

void MyListener::OnOrderFilled(
        int id,
        int quantityFilled) {

    Orders::const_iterator found = orders.find(id);
    if (found == orders.end()) return;

    Order& order = orders.at(id);
    double deltaValue = order.price * quantityFilled;
    char side = order.side;
    switch (side) {
        case 'O':
            runningTotal.offer.cov -= deltaValue;
            runningTotal.nfq -= quantityFilled;
            runningTotal.offer.min_pov = runningTotal.offer.cov + runningTotal.offer.negDeltas;
            runningTotal.offer.max_pov = runningTotal.offer.cov + runningTotal.offer.posDeltas;
            break;
        case 'B':
            runningTotal.bid.cov -= deltaValue;
            runningTotal.nfq += quantityFilled;
            runningTotal.bid.min_pov = runningTotal.bid.cov + runningTotal.bid.negDeltas;
            runningTotal.bid.max_pov = runningTotal.bid.cov + runningTotal.bid.posDeltas;
            break;
    }

    order.qty -= quantityFilled;
}

