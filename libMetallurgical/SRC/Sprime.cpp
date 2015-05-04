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


