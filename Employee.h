//
// Created by abc on 12.11.2017.
//

#ifndef PROJECT2_BURAK_YILDIZ_2015400069_EMPLOYEE_H
#define PROJECT2_BURAK_YILDIZ_2015400069_EMPLOYEE_H


#include "Customer.h"
class Customer;
class Employee {
public:
    double totalWork= 0.00;
    Customer *c=nullptr;

    virtual int getArea();
    Employee();
    ~Employee();
};


#endif //PROJECT2_BURAK_YILDIZ_2015400069_EMPLOYEE_H
