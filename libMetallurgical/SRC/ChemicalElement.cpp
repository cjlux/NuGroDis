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
#include <cassert>

#include "ChemicalElement.hpp"
#include "Concentration.hpp"
#include "ChemicalComposition.hpp"


//CONSTRUCTOR
//build ChemicalElement which has no diffusion parameters, but only mechanical, physical and crystallographic param 
ChemicalElement::ChemicalElement(double density, 
				 double molarMass, 
				 double YModulus, 
				 double PoissonCoef, 
				 std::string elementName,
				 double latticeParameter)
  :density_(density),
   molarMass_(molarMass),
   YoungModulus_(YModulus),
   PoissonCoefficient_(PoissonCoef),
   latticeParameter_(latticeParameter),
   elementName_(elementName),
   chemicalCompositionList_(),
   diffusion_(0)
{
  std::cout <<  "building an ChemicalElement Object <" << elementName_<<"> which has no diffusion parameters" << std::endl;
  std::cout <<  std::endl;
}

ChemicalElement::~ChemicalElement()
{
}

void ChemicalElement::EnterInChemicalComposition(ChemicalComposition& compo)
{
  //Enter a CE to chemicalCompositionList
  Concentration* concentration  = new Concentration(*this, compo);
  compo.AddChemicalElements(*this, *concentration);
  chemicalCompositionList_.push_back(& compo);
}


void
ChemicalElement::ConvertMassicToVolumicConcentration(ChemicalComposition& CC)
{
  //TODO assert if CC is in chemicalCompositionList_. If not, abandon 
}

void
ChemicalElement::ConvertVolumicToMassicConcentration(ChemicalComposition& CC)
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
ChemicalElement::ConvertAtomicToVolumicConcentration(ChemicalComposition& CC)
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
ChemicalElement::ConvertVolumicToAtomicConcentration(ChemicalComposition& CC)
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
ChemicalElement::ConvertStoechiometricCoefficient2VolumicConcentration(ChemicalComposition& CC)
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
ChemicalElement::Info() const
{
  std::cout <<  "#########################   ChemicalElement::Info ###########################"<< std::endl;
  std::cout <<  "                        chemicalElementName: " << elementName_  << std::endl;
  std::cout <<  "                                    density: " << density_  << " SI unit" << std::endl;
  std::cout <<  "                                  molarMass: " << molarMass_ << " SI unit" << std::endl;
  std::cout <<  "                               youngModulus: " << YoungModulus_ << " SI unit" << std::endl;
  std::cout <<  "                         poissonCoefficient: " << PoissonCoefficient_ << " SI unit" << std::endl;
  //  std::cout <<  "                    compositionChimiqueList: " << chemicalCompositionList_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
  
}

