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
// Copyright (C) 2014-2015 Jean-luc CHARLES, Dominique COUPARD, Moubarak GADO, Ivan IORDANOFF.
//

#include <iostream>

#include <vector>

#include "Precipitate.hpp"
#include "RadiusDistribution.hpp"
#include "Grain.hpp"
//#include "SSGrain.hpp"

/*Precipitate::Precipitate()
  :deltaCell_(-1),
   solvusActivationEnergy_(-1),
   distorsionEnergy_(-1),
   nucleationSitesNumber_(-1),
   preExpTermForSolvus_(-1),
   surfaceEnergyPolynomialModel_(),
   shapeFactor_(-1),
   volumicFraction_(-1)
{
  
}*/

Precipitate::Precipitate(Material& mat,ChemicalComposition &CC, RadiusDistribution &initialRD)
  :Grain(mat,CC),
   deltaCell_(-1),
   solvusActivationEnergy_(-1),
   distorsionEnergy_(-1),
   nucleationSitesNumber_(-1),
   preExpTermForSolvus_(-1),
   surfaceEnergyPolynomialModel_(),
   shapeFactor_(-1),
   volumicFraction_(0),
   radiusDistributionList_(),
   currentRadiusDistribution_(0),
   initialRadiusDistribution_(initialRD)
{
  std::cout<<"Build a C++ Precipitate object"<<std::endl;
  currentRadiusDistribution_=&initialRD;
  radiusDistributionList_.push_back(currentRadiusDistribution_);
  std::cout<<"Exit contructor of Precipitate Object"<<std::endl;
  
}

Precipitate::~Precipitate()
{
}

double
Precipitate::ComputeDistorsionEnergy()
{
  return 0;
}

double
Precipitate::ComputeSurfaceEnergy()
{
  return 0;
}

void
Precipitate::ReadDataValue(std::string fileName)
{
}

void
Precipitate::AddRadiusDistribution(const RadiusDistribution& RD)
{

}

void
Precipitate::SetCurrentRadiusDistribution(const RadiusDistribution & RD)
{

}

//TODO ,Does it really need? may be manual set for all attribute would be preferable
/*void 
Precipitate::InitializeParameters(double deltaCell,
				  double solvusActivationEnergy,
				  double nucleationSitesNumber,
				  double preExpTermForSolvus,
				  Polynomial& surfaceEnergyPolynomialModel,
				  double shapeFactor,
				  double volumicFraction)
{
  
  SetDeltaCell(deltaCell);
  SetSolvusActivationEnergy(solvusActivationEnergy);
  SetNucleationSitesNumber(nucleationSitesNumber);
  SetPreExpTermForSolvus(preExpTermForSolvus);
  SetSurfaceEnergyPolynomialModel(surfaceEnergyPolynomialModel);
  SetShapeFactor(shapeFactor);
  SetVolumicFraction(volumicFraction);
}*/


void
Precipitate::Info() const
{
  std::cout <<  "################################# Precipitate::Info #################################" 	<< std::endl;
  std::cout <<  "                                    deltaCell: " << deltaCell_ << " SI unit" << std::endl;
  std::cout <<  "                       solvusActivationEnergy: " << solvusActivationEnergy_ << " SI unit" << std::endl;
  std::cout <<  "                             distorsionEnergy: " << distorsionEnergy_ << " SI unit" << std::endl;
  std::cout <<  "                        nucleationSitesNumber: " << nucleationSitesNumber_ << " SI unit" << std::endl;
  std::cout <<  "                          preExpTermForSolvus: " << preExpTermForSolvus_ << " SI unit" << std::endl;
 // std::cout <<  "               surfaceEnergyPolynomialModel: " << surfaceEnergyPolynomialModel_ << " SI unit" << std::endl;
  std::cout <<  "                                  shapeFactor: " << shapeFactor_ << " SI unit" << std::endl;
  std::cout <<  "                              volumicFraction: " << volumicFraction_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}

