#include <vector>
#include <memory>
#include <string>

#include "Processor.hpp"

int main(int argc,char** argv)
{

    int offset = 0,Energy = 0;
    bool oflag = false,Eflag = false;
    
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
        if(s == "-E")
        {
            Eflag = true;
            continue;
        }
        if(Eflag)
        {
            Energy = std::stoi(s);
            Eflag = false;
            continue;
        }
    }

    int fileAmount = 70;
    int nthr = 10;

    std::vector<std::shared_ptr<Processor> > P;
    P.reserve(nthr);

    int NEnergies = 30;

    std::vector<int> Energies(NEnergies, 0);

    for (int i = 0; i < NEnergies; ++i)
        Energies[i] = i*25;

    int iter_Thr = 0;

    while (iter_Thr < NEnergies/10)
    {
        for(int i = 0;i < nthr;++i)
            P.push_back(std::make_shared<Processor>(i,fileAmount,0,Energies[i]));
        
        std::thread t[nthr];
        
        for(int i = 0;i < nthr;++i)
            t[i] = P[i]->threading();
        
        for(int i = 0;i < nthr;++i)
            t[i].join();

        for(int i = 0;i < nthr;++i) 
            P.pop_back();

        std::cout << "Thread iteration " << iter_Thr << " done" << std::endl;

        ++iter_Thr;
    }

    

    

    return 0;
}