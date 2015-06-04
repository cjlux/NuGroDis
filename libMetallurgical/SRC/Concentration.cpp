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
   initialMassicValue_(-1),
   stoichiometricCoef_(0),
   initialAtomicValueHasBeenSet_(false),
   initialMassicValueHasBeenSet_(false)
{
}

Concentration::~Concentration()
{
}


//TODO could be inline
const int 
Concentration::GetStoichiometricCoef() const
{ 
  assert( (stoichiometricCoef_>0)&&"Cannot Get StoichiometricCoef because it is not strictly positive " );
  return stoichiometricCoef_;
}




void
Concentration::SetStoichiometricCoef(const int stoiCoef)
{
  std::cout<<"In SetStoichoef. Old value is: "<<stoichiometricCoef_<<std::endl;
  assert( (stoiCoef>0)&&"Cannot set value. Stoichiometric coef must be strictly positive " );
  
  stoichiometricCoef_=stoiCoef;
  std::cout<<"In SetStoichoef.After assert. New value is: "<<stoichiometricCoef_<<std::endl;
  std::cout<<"In SetStoichoef.After assert. Addresse of C++ object conentration is "<<this<<std::endl;
  std::cout<<"In SetStoichoef.After assert. Adress of chemical composition is: "<<&chemicalComposition_<<std::endl;
  std::cout<<"In SetStoichoef.After assert. Adress of chemicalCompo's Grain is: "<<&(chemicalComposition_.GetGrain())<<std::endl;
  
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