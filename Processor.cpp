#include "Processor.hpp"

//-------------------------------------------------------

Processor::Processor(int offset, int fileAmount)
{
    Photon = std::vector<std::vector<double> >(100,std::vector<double>(4,0));

    OUT.open("ProcessedData/Processed"+std::to_string(offset));
    if(OUT.fail())
    {
        std::cerr << "File ProcessedData/Processed" + std::to_string(offset) << " not opened!" << std::endl;
        exit(1);
    }

    this->offset = offset;
    this->fileAmount = fileAmount;
}

//-------------------------------------------------------

Processor::~Processor()
{
    OUT.close();
}

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

        std::istringstream Buffer(line);
        std::vector<std::string> Words(std::istream_iterator<std::string>{Buffer},
                                       std::istream_iterator<std::string>() );

        if(Words.size() != PhotonData.size())
        {
            std::cerr << Words.size() << " <- Words size != 6" << std::endl;
            exit(1);
        }

        for(int i = 0;i < Words.size();++i)
        {
            PhotonData[i] = std::stod(Words[i]);
        }

        //new photon starting
        if(PhotonData[0] == -1)
        {
            PhotonStart = true;
            if(iter > 0) Process(iter,Esum);

            Esum = 0.;
            iter = 0;
            ++iterator;
            if (iterator % 100000 == 0 && offset == 0)
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
}

//-------------------------------------------------------

void Processor::Process(int iter,double Esum)
{
    if(iter == 2 && Esum == 661.7)
    {
        double angleX = 0;
        double norm1 = 0,norm2 = 0,scalar = 0;

        for(int i = 0;i < 3;++i)
        {
            norm1 += pow(Photon[0][i+1],2);
            norm2 += pow(Photon[0][i+1] - Photon[1][i+1], 2);
            scalar += (Photon[1][i+1] - Photon[0][i+1])*(Photon[0][i+1]);
        }

        angleX = acos(scalar/sqrt(norm1*norm2))*180./M_PI;
        OUT << Photon[0][0] << " " << angleX << std::endl;
    }
}

//-------------------------------------------------------

std::string Processor::GetName(int iii)
{
    if(iii > 999) return "Gammas/GammaEvents." + std::to_string(iii);
    if(iii > 99) return "Gammas/GammaEvents.0" + std::to_string(iii);
    if(iii > 9) return "Gammas/GammaEvents.00" + std::to_string(iii);
    return "Gammas/GammaEvents.000" + std::to_string(iii);
}

//-------------------------------------------------------

std::thread Processor::threading()
{
    return std::thread(
        [=]
        {
            for(int i = offset;i < offset+fileAmount;++i)
            {
                LOAD(i);
            }
        }
    );
}

//-------------------------------------------------------