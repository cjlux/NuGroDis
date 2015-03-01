#include <string>

//Precitate of type Sprime
//There will be 2 instantiations of Sprime, one in boundary and other  in SSGrain


class Sprime
{

public:

    //the conctructor
    Sprime();
    
    //The destructor
    ~Sprime();
    
    //Compute the number of nucleation sites using  the Guinier-Preston RadiusDistribution::ComputeTotNbOfIte
    void ComputeNucleationSiteNb();
    
    //compute the volumic fraction of Sprime precipitates
    void ComputeVolumicFraction();
    
    //Read data concerning SPrime
    void ReadDataValue(std::string);
    
    //compute value of Stheta
    void WettingFunction();


protected:

private:

    //(was thetaP2) .Unit: degree
    double wettingAngle_;
    
    //(was SthetaP2)
    double Stheta


};