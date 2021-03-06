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

#ifndef __GuinierPreston__hpp__
#define __GuinierPreston__hpp__

#include <cassert>
#include <vector>

#include "Precipitate.hpp"


class GuinierPreston: public Precipitate
{

public:
    GuinierPreston(Material& mat,ChemicalComposition &CC, RadiusDistribution &initialRD);//argument-->Material& because of inheritance with Precipitate
    ~GuinierPreston();
    
    const std::string GetPrecipitateType() { return precipitateType_ ;} ;
    
    void Info() const; 
    
    double ReturnCriticalTotalEnergy();
    void ComputeCriticalTotalEnergy();
    double ReturnVolumicFraction();
    
    //Compute the number of nucleation sites using RadiusDistribution::ComputeTotNbOfItems
    void ComputeNucleationSiteNb();
    
    //Compute the volumic fraction
    void ComputeVolumicFraction();
    
    void ReadDataValue(std::string fileName);

    void ComputeWettingFunction() { assert(!"ComputeWettingFunction has no sense for GP!!! Tchao"); };
    double ReturnWettingFunction() {
      assert(!"ReturnWettingFunction has no sense for GP!!! Tchao");
    };
    
    


protected:

private:
  const std::string precipitateType_;

};

#endif
