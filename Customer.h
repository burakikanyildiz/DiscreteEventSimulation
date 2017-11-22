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
    int action=1; // in which phase of ordering customer currently is
    double t;     // critical times of customer
    int e=0;      //order of employee customer currently dealing with
    int rank=0;   //order of customer's arrival
    Customer();
    Customer(double arrTime,double orderTime,double brewTime, double price,int rank);
    bool operator <(const Customer& other) const;
};


#endif //PROJECT2_BURAK_YILDIZ_2015400069_CUSTOMER_H
