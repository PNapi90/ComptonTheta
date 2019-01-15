#include <vector>
#include <memory>
#include <string>

#include "Processor.hpp"

int main(int argc,char** argv)
{

    int offset = 0;
    bool oflag = false;
    
    std::string s;

    for(int i = 0;i < argc;++i)
    {
        s = std::string(argv[i]);
        if(s == "-o")
        {
            oflag = true;
            continue;
        }
        if(oflag)
        {
            offset = std::stoi(s);
            oflag = false;
            continue;
        }
    }

    int fileAmount = 170;
    int nthr = 10;

    std::vector<std::shared_ptr<Processor> > P;
    P.reserve(nthr);

    for(int i = 0;i < nthr;++i) P.push_back(std::make_shared<Processor>(i*fileAmount,fileAmount,offset));

    std::thread t[nthr];
    for(int i = 0;i < nthr;++i) t[i] = P[i]->threading();
    for(int i = 0;i < nthr;++i) t[i].join();

    return 0;
}