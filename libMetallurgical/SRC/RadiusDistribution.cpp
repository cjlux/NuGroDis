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
 
#include "RadiusDistribution.hpp"
#include "Precipitate.hpp"
#include "InterfacialConcentration.hpp"
#include "ChemicalComposition.hpp"
#include "ChemicalElement.hpp"


RadiusDistribution::RadiusDistribution(double deltar, double r1, double initialClassNb, Precipitate& P)
  :itemsValues_(0),
   initialClassNumber_(initialClassNb),
   minRadius_(r1),
   spatialStep_(deltar),
   precipitate_(&P)
{
  InitializeInterfConc();
  std::cout <<  "Building object <RadiusDistribution> " << std::endl;
}

//CONSTRUCTOR of RD which does not have any precipitate. Can be used for initial Radius Distribution
RadiusDistribution::RadiusDistribution(double deltar, double r1, double initialClassNb)
  :itemsValues_(0),
   initialClassNumber_(initialClassNb),
   minRadius_(r1),
   spatialStep_(deltar),
   precipitate_(0),
   chemicalElementList_(),
   interfConcentrationObjectList_()
{
  std::cout <<  "Building object <RadiusDistribution> which is not linked to any Precipitate" << std::endl;
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
  return 0; // TODO
}

void
RadiusDistribution::ReadInitialDistribution(std::string dataFile)
{
}

//Process Precipitates chemical compo and data, and create interfacialConcentrationObjects associated to each chemicalElement
void
RadiusDistribution::InitializeInterfConc()
{
  //step0: assert if RD is initialRD or not (means if precipitate_=0 or not)
  assert( (precipitate_!=0)&&"Cannot InitializeInterfConc() if RadiusDiustribution precipitate_=0 " );
  
  //Step1: scan precipitates ChemicalCompo
  const ChemicalComposition& compo=precipitate_->GetChemicalComposition();
  //BUG//const std::vector<ChemicalElement *> & elementList = compo.GetChemicalElementsList();
  
  //BUG
  /*//Step2: Add ChemicalElements to chemicalElementList and create interfacialConcentrationObjects associated to each chemicalElement 
  for (unsigned int i=0; i<elementList.size(); ++i)
  {
    chemicalElementList_.push_back(elementList[i]);
    InterfacialConcentration interfConcObject(*this,*elementList[i]);
    interfConcentrationObjectList_.push_back(&interfConcObject);
  }*/
  
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

