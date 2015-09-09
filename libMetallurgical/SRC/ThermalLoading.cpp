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

#include "ThermalLoading.hpp"
#include "Computation.hpp"

ThermalLoading::ThermalLoading(Computation& computation)
  :duration_(0),
   time_(0),
   temperature_(0),
   maxTimeStep_(1),
   computation_(computation)
{
  computation_.SetThermalLoading(*this);
}

ThermalLoading::~ThermalLoading()
{
}

void
ThermalLoading::ReadDistributionFile(std::string fileName)
{
}

void
ThermalLoading::ReadTemperatureLoading(std::string fileName)
{
}

void
ThermalLoading::ReadVCAH(std::string fileName)
{
}

void
ThermalLoading::Info() const
{
  std::cout <<  "############################ ThermalLoading::Info ###################################" << std::endl;
  
  if (duration_==0)
 {std::cout <<  "                                   duration: Is not defined yet"<<std::endl;}
  else
 {std::cout <<  "                                   duration: " << duration_ << " SI unit" << std::endl;};

  if (time_==0)
 {std::cout <<  "                                       time: Is not defined yet"<< std::endl;}
  else
 {std::cout <<  "                                       time: " << *time_ << " SI unit" << std::endl;};
 
  if (temperature_==0)
 {std::cout <<  "                                temperature: Is not defined yet"<< std::endl;}
  else
 {std::cout <<  "                             temperature: " << *temperature_ << " SI unit" << std::endl;};
  
 std::cout << std::endl;
}