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
#include <cassert>

#include "Quenching.hpp"

Quenching::Quenching(double Tsol, double Tfinal, double coolingRate, double deltaT)
  : solutionizingTemp_(Tsol),
    finalTemp_(Tfinal),
    coolingRate_(coolingRate),
    duration_((solutionizingTemp_ - finalTemp_)/coolingRate_),
    timeStep_(0),
    totIterationNumber_(0),
    deltaT_(deltaT)
{
  std::cout <<  "Building object <Quenching> " << std::endl;
  
  // Preconditions:
  assert(finalTemp_ < solutionizingTemp_);
  assert(coolingRate_ > 0);
  assert(deltaT_ > 0);
  

  totIterationNumber_ = (solutionizingTemp_ - finalTemp_)/deltaT_;
  timeStep_ = duration_ / totIterationNumber_;
}

Quenching::~Quenching()
{
}


void
Quenching::Info() const
{
  std::cout <<  "################################# Quenching::Info #################################" 	<< std::endl;
  std::cout <<  "                          solutionizingTemp: " << solutionizingTemp_  << " SI unit" << std::endl;
  std::cout <<  "                                  finalTemp: " << finalTemp_ << " SI unit" << std::endl;
  std::cout <<  "                                coolingRate: " << coolingRate_ << " SI unit" << std::endl;
  std::cout <<  "                                   duration: " << duration_ << " SI unit" << std::endl;
  std::cout <<  "                                   timeStep: " << timeStep_ << " SI unit" << std::endl;
  std::cout <<  "                         totIterationNumber: " << totIterationNumber_ << " SI unit" << std::endl;
  std::cout <<  "                                     deltaT: " << deltaT_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}
