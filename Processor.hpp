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

    const char *format = "%lf %lf %lf %lf %lf %lf";

    std::vector<std::vector<double>> Photon,Hist;
    std::vector<double> binsArray;

    std::ofstream OUT;

    int offset, fileAmount, m_offset,Energy,thrN;

    void LOAD(int iii);
    void Process(int iter, double Esum);
    void SaveHist();

    bool FillHistogram(std::vector<double> &Values);


    std::string GetName(int iii);
    std::string GetName(int iii,bool tmp);



public:

    Processor(int offset,
              int fileAmount,
              int _m_offset,
              int _Energy);
    ~Processor();

    std::thread threading();
};




#endif