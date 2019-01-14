#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <cmath>


//-----------------------------------------------------------------------------------------------------------------------

bool FillHistogram(std::vector<double> &Values, 
                   std::vector<std::vector<double> > &Hist,
                   std::vector<double> &binsArray)
{
    double binningX = 700./Hist.size();
    double binningY = 180./Hist[0].size();
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

//-----------------------------------------------------------------------------------------------------------------------

int main()
{
    std::string line,name;
    std::ifstream DATA;

    std::vector<double> Values(2, 0);
    std::vector<std::vector<double>> Hist(350/2, std::vector<double>(101, 0));

    std::vector<double> binsArray(101,0);
    for(int i = 0;i < 101;++i)
    {
        binsArray[i] = (double) (-1. + 2.*i/100.);
    }

    bool tmpBool = true;

    for (int i = 0; i < 10; ++i)
    {
        name = "ProcessedData/Processed" + std::to_string(i * 170);
        DATA.open(name);

        if(DATA.fail())
        {
            std::cerr << "Could not find " << name << std::endl;
            return 1;
        }

        while (std::getline(DATA, line))
        {
            std::istringstream Buffer(line);
            std::vector<std::string> Words(std::istream_iterator<std::string>{Buffer}, std::istream_iterator<std::string>());

            for (int i = 0; i < 2; ++i)
            {
                Values[i] = std::stod(Words[i]);
            }

            if (Values[1] != Values[1])
                continue;

            Values[1] = cos(Values[1]*M_PI/180.);

            tmpBool = FillHistogram(Values, Hist,binsArray);
            if (!tmpBool)
                return 1;
        }
        DATA.close();
        DATA.clear();
        std::cout << "File " << i*100 << " done" << std::endl;
    }


    std::cout << "Normalizing histograms ...";
    std::cout.flush();

    std::vector<double> Norms(Hist.size(),0);
    for (int i = 0; i < Hist.size(); ++i)
    {
        for (auto Val : Hist[i]) Norms[i] += Val;
    }

    std::cout << " done" << std::endl;

    std::cout << "Saving histogram ... ";
    std::cout.flush();

    std::ofstream OUT("HistsOut");
    for (int i = 0; i < Hist.size(); ++i)
    {

        Norms[i] = Norms[i] > 0 ? Norms[i] : 1.;

        for (auto Val : Hist[i]) OUT << Val/Norms[i] << " ";
        OUT << std::endl;        
    }

    std::cout << "done" << std::endl;

    OUT.close();


    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------