//
// Created by abc on 12.11.2017.
//

#include "Customer.h"

Customer::Customer(double arrTime, double orderTime, double brewTime, double price,int rank) {
    this->arrTime=arrTime;
    this->brewTime=brewTime;
    this->orderTime=orderTime;
    this->price=price;
    action=1;
    t=arrTime;
    this->rank=rank;
}
Customer::Customer() {}
bool Customer::operator<(const Customer &other) const {
    return this->price < other.price;
}
