//
// Created by abc on 12.11.2017.
//

#ifndef PROJECT2_BURAK_YILDIZ_2015400069_CUSTOMER_H
#define PROJECT2_BURAK_YILDIZ_2015400069_CUSTOMER_H

#include "Employee.h"

class Customer {
public:
    double arrTime;
    double orderTime;
    double brewTime;
    double price;
    int action=1;
    double t;
    int e=0;
    int rank=0;
    Customer();
    Customer(double arrTime,double orderTime,double brewTime, double price,int rank);
    bool operator <(const Customer& other) const;
};


#endif //PROJECT2_BURAK_YILDIZ_2015400069_CUSTOMER_H
