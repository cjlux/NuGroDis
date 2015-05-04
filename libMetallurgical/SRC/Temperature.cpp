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

#include <cassert>
#include <iostream>

#include "Temperature.hpp"

Temperature::Temperature(double T) //T=293.15 K,   Standard conditions for temperature and pressure (STP)
  : currentTemp_(T),
    temperatureList_()
{
  assert(currentTemp_ > 0);
  temperatureList_.push_back(currentTemp_);
}

Temperature::~Temperature()
{
}

void
Temperature::SetCurrentTemp(const double temp)
{
  assert(temp >= 0) ; // unit is Kelvin, 
  currentTemp_ = temp;
  temperatureList_.push_back(currentTemp_);
}

void
Temperature::Info() const
{

  std::cout <<  "#########################   Temperature::Info ###########################"<< std::endl;
  std::cout <<  "                            currentTemp: " << currentTemp_ <<"SI unit" << std::endl;
   std::cout <<  std::endl;
}