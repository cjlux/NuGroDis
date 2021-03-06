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


#ifndef __Sprime__hpp__
#define __Sprime__hpp__

#include <string>
#include <cmath>

#include "Precipitate.hpp"

//Precitate of type Sprime
//There will be 2 instantiations of Sprime, one in boundary and other  in SSGrain


class Sprime: public Precipitate
{

public:

    //the CONSTRUCTOR . //argument-->Material& because of inheritance with Precipitate
    Sprime(Material& mat, ChemicalComposition &CC, RadiusDistribution &initialRD,double wettingAngle); //argument-->Material& because of inheritance with Precipitate 
    
    //The destructor
    ~Sprime();
    
    const std::string GetPrecipitateType() { return precipitateType_ ;} ;
    
    void Info() const;
    
    double ReturnCriticalTotalEnergy();
    
    void ComputeCriticalTotalEnergy();
    
    //Compute the number of nucleation sites using  the Guinier-Preston RadiusDistribution::ComputeTotNbOfIte
    void ComputeNucleationSiteNb();
    
    double ReturnVolumicFraction();
    
    //compute the volumic fraction of Sprime precipitates
    void ComputeVolumicFraction();
    
    //Read data concerning SPrime
    void ReadDataValue(std::string);
    
    
    //Setter
    //compute value of Stheta
    void ComputeWettingFunction();
    double ReturnWettingFunction();
    
    //Getter
    double GetWettingAngle() const {return wettingAngle_;};
    double GetStheta() const {return Stheta_;};
    


protected:

private:

  //(was thetaP2) .Unit: SI unit
  const double wettingAngle_;
    
    //(was SthetaP2)
  double Stheta_;
  
  const std::string precipitateType_;


};


void inline //Compute and set value of Stheta
Sprime::ComputeWettingFunction()
{
  assert( (Stheta_==-1) && "WettingFunction: Stheta has already been computed" );
  Stheta_=this->ReturnWettingFunction();
}

double inline
Sprime::ReturnWettingFunction()
{
  double Stheta;
  Stheta=(1.-cos(wettingAngle_))*(1.-cos(wettingAngle_))*(2.+cos(wettingAngle_))/4.;
  return Stheta;
}

#endif
