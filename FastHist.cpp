#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>


//-----------------------------------------------------------------------------------------------------------------------

bool FillHistogram(std::vector<double> &Values, std::vector<std::vector<double> > &Hist)
{
    for(int i = 0;i < 350;++i)
    {
        if(Values[0] >= i*2 && Values[0] < (i+1)*2)
        {
            for(int j = 0;j < 180;++j)
            {
                if(Values[1] >= j && Values[1] < j+1)
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
    std::vector<std::vector<double>> Hist(350, std::vector<double>(180, 0));

    bool tmpBool = true;

    for (int i = 0; i < 10; ++i)
    {
        name = "ProcessedData/Processed" + std::to_string(i * 100) + ".dat";
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

            tmpBool = FillHistogram(Values, Hist);
            if (!tmpBool)
                return 1;
        }
        DATA.close();
        DATA.clear();
        std::cout << "File " << i*100 << " done" << std::endl;
    }


    std::cout << "Normalizing histograms ...";
    std::cout.flush();

    std::vector<double> Norms(350,0);
    for(int i = 0;i < 350;++i)
    {
        for (auto Val : Hist[i]) Norms[i] += Val;
    }

    std::cout << " done" << std::endl;

    std::cout << "Saving histogram ... ";
    std::cout.flush();

    std::ofstream OUT("HistsOut.dat");
    for (int i = 0; i < 350; ++i)
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