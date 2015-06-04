#include <cassert>
#include <iostream>

#include "Grain.hpp"
#include "Material.hpp"
#include "ChemicalComposition.hpp"





//BASIC CONSTRUCTOR
//if Grain==Precipitate or Grain==SSGrain
//A grain MUST Know its material and its chemicalComposition.
//Grain.chemicalComposition can be different from material.initialChemicalComposition
Grain::Grain(Material& mat,ChemicalComposition &CC) 
  :meanDiameter_(-1),
   molarVolume_(-1),
   materialPointer_(&mat),
   chemicalComposition_(CC)
{
   std::cout<<" >Enter in Grain's constructor"<<std::endl;
   
   assert (  (CC.GetGrainPointer()==0)&& "build Grain: ChemicalComposition given in argument already belongs to a Grain");
   CC.SetGrain(*this);
   
   //TODO search and assert *this in (mat.Grain_) . if *this not in (mat.grainList_) then Do:  mat.AddGrain(*this)
   mat.AddGrain(*this);
   
   
   /*debug*/ std::cout<<"ChemicalComposition given in argument now belongs to the Grain at adress:"<<&(CC.GetGrain())<<std::endl;
   std::cout<<" >Exit Grain's constructor"<<std::endl;
}


//TODO Do not forget the other Constructors (special contructors). With arguments ...


//TEST!!! fake code!!
/*
//Only for SSGrain which belongs to a material,
//and has the same properties as the material: ChemicalCompo and mechanical properties
Grain::Grain(Material& mat) 
  :meanDiameter_(-1),
   molarVolume_(-1),
   materialPointer_(&mat)
{
   std::cout<<" >Enter in Grain's constructor"<<std::endl;
   
  //ChemicalComposition* cloneCC =new ChemicalComposition(mat.GetInitialChemicalComposition().GetFormula());
   ChemicalComposition* cloneCC =new ChemicalComposition("Al8Cu8");
   
   
   //TODO search and assert *this in (mat.Grain_) . if *this not in (mat.grainList_) then Do:  mat.AddGrain(*this)
   mat.AddGrain(*this);
   
   //chemicalComposition_=*cloneCC;
   
   
   std::cout<<" >Exit Grain's constructor"<<std::endl;
}*/








Grain::~Grain()
{
}

void
Grain::Info() const
{
  std::cout <<  "#################################  Grain::Info #################################" 	<< std::endl;
  std::cout <<  "                             molarVolume: " << molarVolume_  << " SI unit" << std::endl;
  std::cout <<  "                            meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;
  //std::cout <<  "                              materialPointer_: " << materialPointer_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
  
}