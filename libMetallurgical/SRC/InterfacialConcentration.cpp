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
// Copyright (C) 2014-2015 Jean-luc CHARLES, Dominique COUPARD, Moubarak GADO, Ivan IORDANOFF.
//

#include <iostream>
#include <cassert>

#include "InterfacialConcentration.hpp"
#include "RadiusDistribution.hpp"
#include "ChemicalElement.hpp"

InterfacialConcentration::InterfacialConcentration(RadiusDistribution& RD,ChemicalElement& CE)
  :interfacialConcentrationValuesList_(),
   radiusDistribution_(RD),
   chemicalElement_(CE)
{
  std::cout<<"building object <InterfacialConcentration>"<<std::endl;
}

InterfacialConcentration::~InterfacialConcentration()
{
}


















std::vector<double*>
InterfacialConcentration::ComputeInterfacialConcentrationValuesList(double diameter, double temperature)
{
  //TODO
    // Preconsitions
    assert(diameter >0 && temperature > 0);
    
    //....
    
    
    // Check:
    //assert( quelquechose dont je suis certain)
    
    // Postconditions
    //assert interfacial concentration values >0
    //assert(interfacialConcentrationValuesList_> 0);
    
    return interfacialConcentrationValuesList_;
}




//GETTERS
//return a value of interfacial concentration for a given item
const double
InterfacialConcentration::GetItemValueInInterfConcList(int positionOfInterface)
{//TODO
  assert(0);
}
  
//SETTERS
//set a value of interfacial concentration for a given item
void
InterfacialConcentration::SetItemValueInInterfConcList(const double &computedValueOfItemInInterfConcList)
{//TODO
  assert(0);
}