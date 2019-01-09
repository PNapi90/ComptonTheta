#include <vector>
#include <memory>

#include "Processor.hpp"

int main()
{

    int fileAmount = 25;
    int nthr = 40;

    std::vector<std::shared_ptr<Processor> > P;
    P.reserve(nthr);

    for(int i = 0;i < 40;++i) P.push_back(std::make_shared<Processor>(i*fileAmount,fileAmount));

    std::thread t[nthr];
    for(int i = 0;i < nthr;++i) t[i] = P[i]->threading();
    for(int i = 0;i < nthr;++i) t[i].join();

    return 0;
}