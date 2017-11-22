#include <iostream>
#include <queue>
#include "Employee.h"
#include "Cashier.h"
#include "Barista.h"
#include <fstream>
#include "Customer.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>

using namespace std;
double utilization(Employee &e,double t){
    return e.totalWork/t;
}
template<class Container>
void split1(const string& str, Container& cont){
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),back_inserter(cont));
}
template<typename type>//ordering Customers acc to their arrival order
struct myCompRank {
    bool operator()(const type & first, const type & second) const  {
        return first.rank > second.rank;
    }
};
template<typename type>//ordering Customers according to their critical times
struct myCompTime {
    bool operator()(const type & first, const type & second) const  {
        return first.t > second.t;
    }
};
template<typename type>//ordering Customers according to their orders' price
struct myCompPrice {
    bool operator()(const type & first, const type & second) const  {
        return first.price < second.price;
    }
};
static void model1(priority_queue<Customer, vector<Customer>,myCompTime<Customer>> &times, int cash,char* argv[]);
static void model2(priority_queue<Customer, vector<Customer>,myCompTime<Customer>> &times, int cash,char* argv[]);
int main(int argc, char* argv[]) {
    if(argc!=3){
        cout<<"Run the code with following command: ./projext1 [input_file] [output_file]"<<endl;
        return 1;
    }
    ifstream infile(argv[1]);
    string line;
    getline(infile, line);
    const int cash = stoi(line);

    getline(infile, line);
    const int cus = stoi(line);
    priority_queue<Customer, vector<Customer>,myCompTime<Customer>> times;
    priority_queue<Customer, vector<Customer>,myCompTime<Customer>> times2;
    for (int i = 0; i < cus; i++) {
        getline(infile, line);
        vector<string> words;
        split1(line, words);
        times.push(Customer(stod(words[0]), stod(words[1]), stod(words[2]), stod(words[3]),i));
        times2.push(Customer(stod(words[0]), stod(words[1]), stod(words[2]), stod(words[3]),i));
    }
      model1(times,cash,argv);
      cout<<endl;
      model2(times2,cash,argv);
    return 0;
}
static void model1(priority_queue<Customer, vector<Customer>,myCompTime<Customer>> &times,int cash,char* argv[]){
    Cashier cashArr[cash];
    Barista barArr[cash / 3];
    queue<Customer> cashQ;
    priority_queue<Customer, vector<Customer>, myCompPrice<Customer> > barQ;//Barista priority queue according to price of order
    priority_queue<Customer, vector<Customer>,myCompRank<Customer>> last;
    int maxLC = 0;//Holds maximum length of Cashier queue
    int maxLB=0;//Holds maximum length of Barista queue
    double t=0;//total work time
    while(!times.empty()){
        Customer c=times.top();
        times.pop();
        t=c.t;
        if(c.action==1){
            bool anyAvailable=false;//holds for is there any avaliable cashier
            for (int i = 0; i < cash; i++) {//iterating till find a free cashier
                if (cashArr[i].c== nullptr) {
                    cashArr[i].c=&c;
                    cashArr[i].totalWork+=c.orderTime;
                    c.action++;
                    c.e=i;
                    c.t=t+c.orderTime;
                    times.push(c);
                    anyAvailable=true;
                    break;
                }
            }
            if(!anyAvailable){//not found available cashier, then push the customer to the queue
                cashQ.push(c);
                if(cashQ.size()>maxLC)
                    maxLC=cashQ.size();
            }
        }
        else if(c.action==2){
            cashArr[c.e].c=nullptr;
            if(!cashQ.empty()){//cashierQ is not empty, then move the first customer to newly freed cashier.
                Customer a=cashQ.front();
                cashQ.pop();
                cashArr[c.e].c=&a;
                cashArr[c.e].totalWork+=a.orderTime;
                a.action++;
                a.e=c.e;
                a.t=t+a.orderTime;
                times.push(a);
            }
            bool anyAvailable=false;
            for(int i=0;i<cash/3;i++){//iterating till find a free barista
                if(barArr[i].c== nullptr){
                    barArr[i].c=&c;
                    barArr[i].totalWork+=c.brewTime;
                    c.action++;
                    c.e=i;
                    c.t=t+c.brewTime;
                    times.push(c);
                    anyAvailable=true;
                    break;
                }
            }
            if(!anyAvailable){
                barQ.push(c);
                if(barQ.size()>maxLB)
                    maxLB=barQ.size();
            }
        }
        else if(c.action==3){
            barArr[c.e].c=nullptr;
            last.push(c);
            if(!barQ.empty()){//BaristaQ not empty, then move the Customer to the newly freed Barista
                Customer a=barQ.top();
                barQ.pop();
                barArr[c.e].c=&a;
                barArr[c.e].totalWork+=a.brewTime;
                a.action++;
                a.e=c.e;
                a.t=t+a.brewTime;
                times.push(a);
            }
        }
    }
    ofstream myfile;
    myfile.open(argv[2],ios::trunc);
    myfile<<fixed<<setprecision(2)<<t<<endl;
    myfile<<maxLC<<endl;
    myfile<<maxLB<<endl;
    for(int i=0;i<cash;i++){
        myfile<<utilization(cashArr[i],t)<<endl;
    }
    for(int i=0;i<cash/3;i++){
        myfile<<utilization(barArr[i],t)<<endl;
    }
    while(!last.empty()){//prints customer's total time of order
        myfile<<(last.top()).t-(last.top().arrTime)<<endl;
        last.pop();
    }
    myfile<<endl;
    myfile.close();

}
static void model2(priority_queue<Customer, vector<Customer>,myCompTime<Customer>> &times, int cash,char* argv[]){
    Cashier cashArr[cash];
    Barista barArr[cash / 3];
    queue<Customer> cashQ;
    priority_queue<Customer, vector<Customer>, myCompPrice<Customer> > barQs[cash/3];
    priority_queue<Customer, vector<Customer>,myCompRank<Customer>> last;
    int maxLC = 0;
    int maxLB[cash/3];
    for(int i=0;i<cash/3;i++){
        maxLB[i]=0;
    }
    double t=0;
    while(!times.empty()){
        Customer c=times.top();
        times.pop();
        t=c.t;
        if(c.action==1){
            bool anyAvailable=false;
            for (int i = 0; i < cash; i++) {
                if (cashArr[i].c== nullptr) {
                    cashArr[i].c=&c;
                    cashArr[i].totalWork+=c.orderTime;
                    c.action++;
                    c.e=i;
                    c.t=t+c.orderTime;
                    times.push(c);
                    anyAvailable=true;
                    break;
                }
            }
            if(!anyAvailable){
                cashQ.push(c);
                if(cashQ.size()>maxLC)
                    maxLC=cashQ.size();
            }
        }
        else if(c.action==2){
            cashArr[c.e].c=nullptr;
            if(!cashQ.empty()){
                Customer a=cashQ.front();
                cashQ.pop();
                cashArr[c.e].c=&a;
                cashArr[c.e].totalWork+=a.orderTime;
                a.action++;
                a.e=c.e;
                a.t=t+a.orderTime;
                times.push(a);
            }
                if(barArr[c.e/3].c== nullptr){
                    barArr[c.e/3].c=&c;
                    barArr[c.e/3].totalWork+=c.brewTime;
                    c.action++;
                    c.e=c.e/3;
                    c.t=t+c.brewTime;
                    times.push(c);
                }else{
                    barQs[c.e/3].push(c);
                    if(barQs[c.e/3].size()>maxLB[c.e/3]){
                        maxLB[c.e/3]=barQs[c.e/3].size();
                    }
                }
        }
        else if(c.action==3){
            barArr[c.e].c=nullptr;
            last.push(c);
            if(!barQs[c.e].empty()){
                Customer a=barQs[c.e].top();
                barQs[c.e].pop();
                barArr[c.e].c=&a;
                barArr[c.e].totalWork+=a.brewTime;
                a.action++;
                a.e=c.e;
                a.t=t+a.brewTime;
                times.push(a);
            }
        }
    }
    ofstream myfile;
    myfile.open(argv[2], ios::app);
    myfile<<fixed<<setprecision(2)<<t<<endl;
    myfile<<maxLC;
    for(int i=0;i<cash/3;i++) {
        myfile << endl << maxLB[i];
    }
    for(int i=0;i<cash;i++){
        myfile<<endl<<utilization(cashArr[i],t);
    }
    for(int i=0;i<cash/3;i++){
        myfile<<endl<<utilization(barArr[i],t);
    }
    while(!last.empty()){
        myfile<<endl<<(last.top()).t-(last.top().arrTime);
        last.pop();
    }
    myfile.close();
}