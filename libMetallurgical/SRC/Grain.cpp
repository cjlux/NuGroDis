#include <cassert>
#include <iostream>

#include "Grain.hpp"
#include "Material.hpp"





//BASIC CONSTRUCTOR
//if Grain==Precipitate or Grain==SSGrain
//A grain MUST Know its material and its chemicalComposition.
//Grain.chemicalComposition can be different from material.initialChemicalComposition
Grain::Grain(Material& mat,ChemicalComposition &CC) 
  :meanDiameter_(-1),
   molarVolume_(-1),
   material_(&mat),
   chemicalComposition_(CC)
{
   std::cout<<" >Enter in Grain's constructor"<<std::endl;
   std::cout<<" >Exit Grain's constructor"<<std::endl;
}




//TODO Do not forget the other Constructors (special contructors). With arguments ...

Grain::~Grain()
{
}

void
Grain::Info() const
{
  std::cout <<  "#################################  Grain::Info #################################" 	<< std::endl;
  std::cout <<  "                             molarVolume: " << molarVolume_  << " SI unit" << std::endl;
  std::cout <<  "                            meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;
  //std::cout <<  "                              material_: " << material_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
  
}