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

#include "GuinierPreston.hpp"

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

/*void
GuinierPreston::Info() const
{
  std::cout <<  "###########################  GuinierPreston::Info #################################" 	<< std::endl;
  std::cout <<  "                                molarVolume: " << molarVolume_  << " SI unit" << std::endl;
  std::cout <<  "                               meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;
  //std::cout <<  "                                  material_: " << material_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
  
}*/