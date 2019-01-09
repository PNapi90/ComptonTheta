#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <iterator>

class Processor
{

private:
    
    std::vector<std::vector<double> > Photon;

    std::ofstream OUT;

    int offset, fileAmount;

    void LOAD(int iii);
    void Process(int iter,double Esum);

    std::string GetName(int iii);


public:

    Processor(int offset, int fileAmount);
    ~Processor();

    std::thread threading();
};




#endif