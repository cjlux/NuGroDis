// This file is part of NuGrodis, a workbench for micro-structural modelling.
//
// Author(s)    : - Jean-luc CHARLES, I2M/ENSAM Talence France
//                  <jean-luc.charles@ensam.eu>
//                - Dominique COUPARD, I2M/ENSAM Talence France
//                  <dominique.coupardensam.eu>
//                - Moubarak GADO, I2M/ENSAM Talence France
//                  <gadomoubarak@yahoo.fr>
//                - Ivan IORDANOFF, I2M/ENSAM Talence France
//                  <ivan.iordanoff@ensam.eu>
//
// Copyright (C) 20014-2015 Jean-luc CHARLES, Dominique COUPARD, Moubarak GADO, Ivan IORDANOFF.
//

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
    
    void Info() const;
    
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