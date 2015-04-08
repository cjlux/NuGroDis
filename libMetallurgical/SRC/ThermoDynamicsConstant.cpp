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

#include "ThermoDynamicsConstant.hpp"

ThermoDynamicsConstant::ThermoDynamicsConstant(double r, double k, double n)
  : R_(r),
    kB_(k),
    Na_(n)
{
  std::cout <<  "Building object <ThermoDynamicsConstant> " << std::endl;
}

ThermoDynamicsConstant::~ThermoDynamicsConstant()
{
}

void
ThermoDynamicsConstant::Info() const
{
  std::cout <<  "#################### ThermoDynamicsConstant::Info #############################"<< std::endl;
  std::cout <<  "                                          R: " << R_  << " SI unit" << std::endl;
  std::cout <<  "                                         kB: " << kB_ << " SI unit" << std::endl;
  std::cout <<  "                                         Na: " << Na_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}
