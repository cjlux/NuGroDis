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
#include <string>
#include "ChemicalElement.hpp"

ChemicalElement::ChemicalElement(double density, double molarMass, double yModulus, double poissonCoef, std::string elementName)
  :density_(density),
   molarMass_(molarMass),
   youngModulus_(yModulus),
   poissonCoefficient_(poissonCoef),
   elementName_(elementName)
{
  std::cout <<  "building an ChemicalElement Object <" << elementName_<<">" << std::endl;
  std::cout <<  std::endl;
}

ChemicalElement::~ChemicalElement()
{
}

double 
ChemicalElement::ComputeInterFacialConcentration(double diameter, double temperature)
{
    // Preconsitions
    assert(diameter >0 && temperature > 0);
    
    //....
    
    
    // Check:
    //assert( quelquechose dont je suis certain)
    
    // Postconditions
    assert(interFacialConcentration_ > 0);
    
    return interFacialConcentration_;
}

void
ChemicalElement::ConvertMassicToVolumicConcentration()
{
}

void
ChemicalElement::ConvertVolumicToMassicConcentration()
{
}

void
ChemicalElement::ConvertAtomicToVolumicConcentration()
{
}

void
ChemicalElement::ConvertVolumicToAtomicConcentration()
{
}

void
ChemicalElement::ConvertStoechiometricCoefficient2VolumicConcentration()
{
}

void
ChemicalElement::Info() const
{
  std::cout <<  "#########################   ChemicalElement::Info ###########################"<< std::endl;
  std::cout <<  "                        chemicalElementName: " << elementName_  << std::endl;
  std::cout <<  "                                    density: " << density_  << " SI unit" << std::endl;
  std::cout <<  "                                  molarMass: " << molarMass_ << " SI unit" << std::endl;
  std::cout <<  "                               youngModulus: " << youngModulus_ << " SI unit" << std::endl;
  std::cout <<  "                         poissonCoefficient: " << poissonCoefficient_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}

