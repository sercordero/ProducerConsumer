//
// Created by cordese on 19/3/2019.
//

#ifndef PRODUCER_CONSUMER_HPP
#define PRODUCER_CONSUMER_HPP

#endif //PRODUCER_CONSUMER_HPP

#include <stdio.h>  // for printf
#include <string>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <vector>
#include<unistd.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

using std::string;
using namespace std;

class ProducerConsumer
{
public:
	ProducerConsumer(const char* line, int id, char *name);
	ProducerConsumer(int id, char * name);
    ~ProducerConsumer();
    void run();
    void initParameters();
    void initMembers();
    void setVectorSubscriber(string vSubscriber);
    void orderVector(char ordered);
    void printVector();

    vector<string> vStrid;
    vector<string> vStrimsi;
    vector<string> vString;
    char          fileOut[100];
    std::ofstream   outfile;
    int order;

};
