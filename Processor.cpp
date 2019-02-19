#include "Processor.hpp"

//-------------------------------------------------------

Processor::Processor(int offset,
                     int fileAmount,
                     int _m_offset,
                     int _Energy,
                     bool _Mode,
                     int _thrN) 
    : m_offset(_m_offset),
      Energy(_Energy),
      Mode(_Mode),
      thrN(_thrN)
{   


    nbins = Mode ? Energy/4 + 1 : 175;
    //nbins = 25;

    Photon = std::vector<std::vector<double> >(100,std::vector<double>(4,0));
    Hist = std::vector<std::vector<double> >(nbins, std::vector<double>(101, 0));

    binsArray = std::vector<double>(101,0);
    for(int i = 0;i < 101;++i)
    {
        binsArray[i] = (double) (-1. + 2.*i/100.);
    }

    std::cout << "Created processor for E = " << Energy << " with " << nbins << " bins" << std::endl;
    std::cout << "Processing Events from " << offset << " to " << offset + fileAmount << std::endl;

    thrN = offset;
    this->offset = offset;
    this->fileAmount = fileAmount;

    if(Mode)
    {
        this->offset = 0;
        this->fileAmount = 1;
    }
}

//-------------------------------------------------------

Processor::~Processor()
{}

//-------------------------------------------------------

void Processor::LOAD(int iii)
{

    std::string file = GetName(iii);
    std::ifstream DATA(file);
    if (DATA.fail())
    {
        std::cerr << "Could not find file " << file << std::endl;
        exit(1);
    }

    std::string line;
    bool DataComing = false;

    int iter = 0;
    bool PhotonStart = false;

    int iterator = 0;
    double Esum = 0.;

    std::vector<double> PhotonData(6,0);

    while(std::getline(DATA,line))
    {   
        if(line[0] == '$')
        {
            DataComing = true;
            continue;
        }
        if(!DataComing) continue;

        sscanf(line.c_str(), format, &PhotonData[0],&PhotonData[1],&PhotonData[2],&PhotonData[3],&PhotonData[4],&PhotonData[5]);


        //new photon starting
        if(PhotonData[0] == -1)
        {
            PhotonStart = true;

            if(iter > 0)
                Process(iter,Esum);

            Esum = 0.;
            iter = 0;
            ++iterator;
            if (iterator % 100000 == 0 && thrN == 0)
            {   
                std::cout << "\r";
                std::cout << iii <<" -> iterator at photon # " << iterator << "\t\t\t\t\t\t";
                std::cout.flush();
            }
        }
        
        else
        {
            for(int i = 0;i < 4;++i)
            {
                Photon[iter][i] = PhotonData[i+1];
            }
            Esum += Photon[iter][0];

            ++iter;
        }
        
    }

    SaveHist(iii);
}

//-------------------------------------------------------

void Processor::Process(int iter,double Esum)
{
    if(iter >= 2)
    {
        std::vector<double> Values(2,0);
        double angleX = 0;
        double norm1 = 0,norm2 = 0,scalar = 0;

        for(int i = 0;i < 3;++i)
        {
            norm1 += pow(Photon[0][i+1],2);
            norm2 += pow(Photon[0][i+1] - Photon[1][i+1], 2);
            scalar += (Photon[1][i+1] - Photon[0][i+1])*(Photon[0][i+1]);
        }

        angleX = (scalar/sqrt(norm1*norm2));

        Values[0] = Photon[0][0];
        Values[1] = angleX;
        
        //bypass distance problem (norm2 == 0) of Geant4
        if(norm1 == 0 || norm2 == 0)
            return;

        bool check = FillHistogram(Values);

        if(!check)
            exit(1);
    }
}

//-------------------------------------------------------

std::string Processor::GetName(int iii)
{
    if(!Mode)
    {
        //std::cout << "Opened Gammas/Edeps_661_40.dat" << std::endl;  
        //return "Gammas/Edeps_661_40.dat";
        return "Gammas/GammaEvents.000" + std::to_string(iii);
        if(iii > 999) return "Gammas/Events0/GammaEvents." + std::to_string(iii);
        if(iii > 99) return "Gammas/Events0/GammaEvents.0" + std::to_string(iii);
        if(iii > 9) return "Gammas/Events0/GammaEvents.00" + std::to_string(iii);
        return "Gammas/Events0/GammaEvents.000" + std::to_string(iii);
    }
    
    std::string AllEns = "AllEnergies/Edeps_" + std::to_string(Energy) + ".dat";
    return AllEns;
}

//-------------------------------------------------------

std::thread Processor::threading()
{
    if (Mode) 
        return std::thread([=] {LOAD(0);});
    
    return std::thread(
        [=] {
            for (int i = offset + m_offset; i < offset + fileAmount + m_offset; ++i)
            {
                LOAD(i);
                if (offset == 0)
                    std::cout << std::endl;
            }
        }
    );
}

//-------------------------------------------------------

bool Processor::FillHistogram(std::vector<double> &Values)
{
    double binningX = Mode ? ((double) Energy)/((double) nbins-1) : 4;
    for(int i = 0;i < Hist.size();++i)
    {
        if(Values[0] >= i*binningX && Values[0] < (i+1)*binningX)
        {
            for(int j = 0;j < Hist[i].size();++j)
            {
                if (Values[1] >= binsArray[j] && Values[1] < binsArray[j+1])
                {
                    Hist[i][j] += 1.;
                    return true;
                }
            }
        }
    }

    std::cerr << Values[0] << "," << Values[1] << " didn't find a match!" << std::endl;
    return false;
}

//-------------------------------------------------------

void Processor::SaveHist(int iii)
{

    std::vector<double> Norms(Hist.size(),0);
    for (int i = 0; i < Hist.size(); ++i)
    {
        for (auto Val : Hist[i]) 
            Norms[i] += Val;
    }

    std::string FileName = "Histograms/ComptonHist_" + std::to_string(Energy) + "_new";

    //if (!Mode)
    //    FileName += "_" + std::to_string(thrN) + "_" + std::to_string(iii);
    
    std::ofstream OUT(FileName);

    if(OUT.fail())
    {
        std::cerr << "Could not open output file " << FileName << std::endl;
        exit(1);
    }

    for (int i = 0; i < Hist.size(); ++i)
    {
        Norms[i] = Norms[i] > 0 ? Norms[i] : 1.;

        for (auto Val : Hist[i]) 
            OUT << Val/Norms[i] << " ";
        OUT << std::endl;        
    }

    OUT.close();
}

//-------------------------------------------------------