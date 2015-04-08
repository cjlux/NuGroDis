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
#include "RadiusDistribution.hpp"

RadiusDistribution::RadiusDistribution(double deltar, double r1, double initialClassNb)
  :itemsValues_(0),
   initialClassNumber_(initialClassNb),
   minRadius_(r1),
   spatialStep_(deltar)
{
  std::cout <<  "Building object <RadiusDistribution> " << std::endl;
}

RadiusDistribution::~RadiusDistribution()
{
}

void
RadiusDistribution::ComputeTotNbOfItems()
{
}

int
RadiusDistribution::getItemValueForClass(int)
{
}

void
RadiusDistribution::ReadInitialDistribution(std::string dataFile)
{
}

void
RadiusDistribution::Info() const
{
  std::cout <<  "######################## RadiusDistribution::Info ######################" 	<< std::endl;
  std::cout <<  "                                itemsValues: " <<itemsValues_ << " SI unit" << std::endl;
  std::cout <<  "                         initialClassNumber: " << initialClassNumber_<< " SI unit" << std::endl;
  std::cout <<  "                                  minRadius: " << minRadius_<< " SI unit" << std::endl;
  std::cout <<  "                                spatialStep: " << spatialStep_<< " SI unit" << std::endl;
  std::cout <<  "                                                                 " 	<< std::endl;
}

