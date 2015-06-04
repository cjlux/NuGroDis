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

#include "GuinierPreston.hpp"
#include "Polynomial.hpp"


GuinierPreston::GuinierPreston(Material& mat, ChemicalComposition &CC, RadiusDistribution &initialRD)
  :Precipitate(mat,CC,initialRD)
{
}

GuinierPreston::~GuinierPreston()
{
}

void
GuinierPreston::ComputeNucleationSiteNb()
{
}

void
GuinierPreston::ComputeVolumicFraction()
{
}

void
GuinierPreston::ReadDataValue(std::string fileName)
{
}

void
GuinierPreston::Info() const
{
  std::cout <<  "#############################  GuinierPreston::Info #################################" 	<< std::endl;
  std::cout <<  "                                    deltaCell: " << deltaCell_ << " SI unit" << std::endl;
  std::cout <<  "                       solvusActivationEnergy: " << solvusActivationEnergy_ << " SI unit" << std::endl;
  if (distorsionEnergy_==-1)
  {std::cout <<  "                             distorsionEnergy: Not defined" << std::endl;}
  else
  {std::cout <<  "                             distorsionEnergy: " << distorsionEnergy_ << " SI unit" << std::endl;};
  
  std::cout <<  "                        nucleationSitesNumber: " << nucleationSitesNumber_ << " SI unit" << std::endl;
  std::cout <<  "                          preExpTermForSolvus: " << preExpTermForSolvus_ << " SI unit" << std::endl;
  std::cout <<  "                                  shapeFactor: " << shapeFactor_ << " SI unit" << std::endl;
  std::cout <<  "                              volumicFraction: " << volumicFraction_ << " SI unit" << std::endl;
  std::cout <<  "                                  molarVolume: " << molarVolume_  << " SI unit" << std::endl;
  if (meanDiameter_==-1)
  {std::cout <<  "                                 meanDiameter: Not defined" << std::endl;}
  else
  {std::cout <<  "                                 meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;};
  
  
  //std::cout <<  "                                  material_: " << material_ << " SI unit" << std::endl;
  if (surfaceEnergyPolynomial_.GetDegree()==-1)
  {
  std::cout <<  "                      surfaceEnergyPolynomial: Not defined" << std::endl;
  }
  else
  {
  std::cout <<  "                      surfaceEnergyPolynomial: " ;
  std::cout <<"<"<<surfaceEnergyPolynomial_.GetCoefsList()[0]<<">";
  for (int i=1; i<=surfaceEnergyPolynomial_.GetDegree();i++)
  {
    std::cout <<" + <"<<surfaceEnergyPolynomial_.GetCoefsList()[i]<<">*"<<"T^"<<i;
  }
  std::cout<< " SI unit" << std::endl;
    
  };
  
  std::cout  << "                         precipitate adress: " <<this<<std::endl;
  

  
  std::cout <<  std::endl;
  
}