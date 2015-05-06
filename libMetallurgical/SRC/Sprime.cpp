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

#include <iostream>

#include "Sprime.hpp"

Sprime::Sprime(Material& mat,ChemicalComposition &CC, RadiusDistribution &initialRD,double wetA)
  :Precipitate(mat,CC,initialRD),
   wettingAngle_(wetA),
   Stheta_(-1)
{  
}

Sprime::~Sprime()
{
}

void
Sprime::ComputeNucleationSiteNb()
{
}

void
Sprime::ComputeVolumicFraction()
{
}

void
Sprime::ReadDataValue(std::string dataFile)
{
}

void
Sprime::Info() const
{
  std::cout <<  "###################################  Sprime::Info #################################" 	<< std::endl;
  std::cout <<  "                                molarVolume: " << molarVolume_  << " SI unit" << std::endl;
  std::cout <<  "                               meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;
  std::cout <<  "                               wettingAngle: " << wettingAngle_ << " SI unit" << std::endl;
  std::cout <<  "                                     Stheta: " << Stheta_ << " SI unit" << std::endl;  
  //std::cout <<  "                                material_: " << material_ << " SI unit" << std::endl;
  std::cout <<  "                                  deltaCell: " << deltaCell_ << " SI unit" << std::endl;
  std::cout <<  "                     solvusActivationEnergy: " << solvusActivationEnergy_ << " SI unit" << std::endl;
  std::cout <<  "                           distorsionEnergy: " << distorsionEnergy_ << " SI unit" << std::endl;
  std::cout <<  "                      nucleationSitesNumber: " << nucleationSitesNumber_ << " SI unit" << std::endl;
  std::cout <<  "                        preExpTermForSolvus: " << preExpTermForSolvus_ << " SI unit" << std::endl;
 // std::cout <<  "               surfaceEnergyPolynomialModel: " << surfaceEnergyPolynomialModel_ << " SI unit" << std::endl;
  std::cout <<  "                                shapeFactor: " << shapeFactor_ << " SI unit" << std::endl;
  std::cout <<  "                            volumicFraction: " << volumicFraction_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
  
}

