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
#include "Hardening.hpp"

#include "Computation.hpp"

Hardening::Hardening(double hardeningDuration, Computation& computation)
  :duration_(hardeningDuration),
   computation_(computation),
   type_("Hardening")
{
  std::cout <<  "Building object <Hardening> " << std::endl;
  
  computation_.SetHardening(*this);
}

Hardening::~Hardening()
{
}


std::string 
Hardening::GetInfo()
{
  return std::string("Bonjour");
}


void
Hardening::Info() const
{
  std::cout <<  "################################# Hardening::Info ###################################" << std::endl;
  std::cout <<  "                                   duration: " << duration_ << " SI unit" << std::endl;
  std::cout <<  "\n"<<std::endl;
}
