#include <vector>
#include <memory>
#include <string>

#include "Processor.hpp"

//-------------------------------------------------------

struct FlagsAndVals
{
    int offset = 0;
    int Energy = 661;
    int nthr = 1;
    bool oflag = false;
    bool Eflag = false;
    bool thrflag = false;
    bool Mode = false;
};

//-------------------------------------------------------

void PrintError(int NEnergies,int nthr);

//-------------------------------------------------------

void GetFlags(FlagsAndVals &F,
              int argc,
              char** argv);

//-------------------------------------------------------

int main(int argc,char** argv)
{

    FlagsAndVals F;

    GetFlags(F,argc,argv);    
    
    int nthr = F.nthr;


    int fileAmount = 10;

    std::vector<std::shared_ptr<Processor> > P;
    P.reserve(nthr);

    if(F.Mode)
    {

        int NEnergies = 250/25;

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
                P.push_back(std::make_shared<Processor>(i,fileAmount,0,Energies[E_iter],F.Mode,i));
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
    }
    
    else
    {   for (int i = 0; i < nthr; ++i)
            P.push_back(std::make_shared<Processor>(i * fileAmount, fileAmount, F.offset,661,F.Mode,i));

        std::thread t[nthr];
        for (int i = 0; i < nthr; ++i)
            t[i] = P[i]->threading();
        for (int i = 0; i < nthr; ++i)
            t[i].join();
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


void GetFlags(FlagsAndVals &F,
              int argc,
              char** argv)
{
    std::string s;

    for (int i = 0; i < argc; ++i)
    {
        s = std::string(argv[i]);
        if (s == "-o")
        {
            F.oflag = true;
            continue;
        }
        if (F.oflag)
        {
            F.offset = std::stoi(s);
            F.oflag = false;
            continue;
        }
        if (s == "-t")
        {
            F.thrflag = true;
            continue;
        }
        if (F.thrflag)
        {
            F.nthr = std::stoi(s);
            F.thrflag = false;
            continue;
        }
        if (s == "-E")
        {
            F.Eflag = true;
            continue;
        }
        if (F.Eflag)
        {
            F.Energy = std::stoi(s);
            F.Eflag = false;
            continue;
        }
        if(s == "-M")
        {
            F.Mode = true;
            continue;
        }
    }
}

//-------------------------------------------------------