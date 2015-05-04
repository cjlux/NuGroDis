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

#include "Concentration.hpp"

#include "ChemicalElement.hpp"
#include "ChemicalComposition.hpp"

Concentration::Concentration(const ChemicalElement& elem,const ChemicalComposition& CC)
  :chemicalElement_(elem),
   chemicalComposition_(CC),
   volumicValue_(-1),
   initialAtomicValue_(-1),
   initialMassicValue_(-1)
{
}

Concentration::~Concentration()
{
}


void
Concentration::ComputeAtomicValue()
{
}

void
Concentration::ComputeMassicValue()
{
}

void
Concentration::Info() const
{
  std::cout <<  "################################# Concentration::Info #################################" 	<< std::endl;
  std::cout <<  "                                   volumicValue: " << volumicValue_  << " SI unit" << std::endl;
  std::cout <<  "                             initialAtomicValue: " << initialAtomicValue_ << " SI unit" << std::endl;
  std::cout <<  "                             initialMassicValue: " << initialMassicValue_ << " SI unit" << std::endl;
  //std::cout <<  "                            chemicalComposition: " << chemicalComposition_ << " SI unit" << std::endl;
  //std::cout <<  "                                chemicalElement: " << chemicalElement_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}