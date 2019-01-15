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

    const char *format = "%lf %lf %lf %lf %lf %lf ";

    std::vector<std::vector<double>> Photon;

    std::ofstream OUT;

    int offset, fileAmount, m_offset;

    void LOAD(int iii);
    void Process(int iter, double Esum);

    std::string GetName(int iii);


public:

    Processor(int offset, int fileAmount,int _m_offset);
    ~Processor();

    std::thread threading();
};




#endif