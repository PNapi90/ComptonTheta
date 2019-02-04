#include <vector>
#include <memory>
#include <string>

#include "Processor.hpp"

//-------------------------------------------------------

void PrintError(int NEnergies,int nthr);

//-------------------------------------------------------

int main(int argc,char** argv)
{

    int offset = 0,Energy = 0,nthr = 1;
    bool oflag = false,Eflag = false,thrflag = false;
    
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
        if (s == "-n")
        {
            thrflag = true;
            continue;
        }
        if (thrflag)
        {
            nthr = std::stoi(s);
            thrflag = false;
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

    std::vector<std::shared_ptr<Processor> > P;
    P.reserve(nthr);

    int NEnergies = 30;

    std::vector<int> Energies(NEnergies, 0);

    for (int i = 0; i < NEnergies; ++i)
        Energies[i] = i*25+25;

    int iter_Thr = 0;

    if(NEnergies % nthr != 0)
    {
        PrintError(NEnergies,nthr);
        return -1;
    } 

    int E_iter = 0;
    std::cout << "---------------------------------------------" << std::endl;
    while (iter_Thr < NEnergies/nthr)
    {
        for(int i = 0;i < nthr;++i)
        {
            P.push_back(std::make_shared<Processor>(i,fileAmount,0,Energies[E_iter]));
            ++E_iter;
        }

        std::cout << "================" << std::endl;

        std::thread t[nthr];
        
        for(int i = 0;i < nthr;++i)
            t[i] = P[i]->threading();
        
        for(int i = 0;i < nthr;++i)
            t[i].join();

        for(int i = 0;i < nthr;++i) 
            P.pop_back();

        std::cout << "\nThread iteration " << iter_Thr << " done" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;

        ++iter_Thr;
    }

    

    

    return 0;
}

//-------------------------------------------------------

void PrintError(int NEnergies,int nthr)
{
    std::cerr << NEnergies << " mod " << nthr << " not 0!" << std::endl;
    std::cerr << "Non equal distribution of data on threads!" << std::endl;
    std::cerr << "Please reset your number of threads according to NEnergies" << std::endl;
}

//-------------------------------------------------------
