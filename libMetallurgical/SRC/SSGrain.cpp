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

#include "SSGrain.hpp"
#include "Material.hpp"
#include "ChemicalElement.hpp"




//BASIC CONSTRUCTOR, gives all Grain param unless volum molar_ and meanDiameter 
//a grain has differents mechanical properties and ChemicalCompo from the material
SSGrain::SSGrain(Material& mat,
		 ChemicalComposition &CC,
		 double YM,
		 double PoissonCoeff,
		 double latticeParam,
		 int volNbGP,
		 int volNbSprime)
  :Grain(mat,CC),
   YoungModulus_(YM),
   PoissonCoeff_(PoissonCoeff),
   latticeParameter_(latticeParam),
   volNbGP_(volNbGP),
   volNbSprime_(volNbSprime),
   volNbPrecipitates_(volNbGP+volNbSprime),
   precipitateList_()
   
{
  assert( (mat.GetSSGrain()==0) && ("WARNING! Cannot build a SSGrain object: The material has already solid solution grains! Material ssgrain has already been initialize") );

  
}



//SPECIAL CONSTRUCTOR
//make a grain which belongs to a material
//A grain has the same properties as the material: ChemicalCompo and mechanical properties
SSGrain::SSGrain(Material& mat, int volNbGP,int volNbSprime)
  :   Grain(mat, mat.GetInitialChemicalComposition() ),
      YoungModulus_(mat.GetYoungModulus()),
      PoissonCoeff_(mat.GetPoissonCoeff()), 
      latticeParameter_(mat.GetLatticeParameter()),
      volNbGP_(volNbGP),
      volNbSprime_(volNbSprime),
      volNbPrecipitates_(volNbGP+volNbSprime),
      precipitateList_()
{
  assert( (mat.GetSSGrain()==0) && ("WARNING! Cannot build a SSGrain object:Material attribute ssgrain has already been initialize! The material has already solid solution grains!  ") );
  
  std::cout<<"Enter in SSGRain's constructor"<<std::endl;
  std::cout<<"		Material Young modulus = "<<mat.GetYoungModulus()<<std::endl;
  std::cout<<"		SSgrain Young modulus (from YoungModulus_) = "<<YoungModulus_<<std::endl;
  this->Info();
  std::cout<<"Adress of ssgrain  Object ="<<this<<std::endl;
  std::cout<<"Exit SSGrain constructor"<<std::endl;
}





//TODO CONSTRUCTOR, material and ssgrain have: same mechanical properties but different chemical properties


//TODO CONSTRUCTOR, material and ssgrain have: same chemical properties but different mechanical properties


SSGrain::~SSGrain()
{
}

void
SSGrain::ComputeMolarVolume() 
{
}

void
SSGrain::ReadDataValue(std::string fileName)
{
}

void
SSGrain::Info() const
//if (this!=0)
{
  std::cout <<  "################################# SSGrain::Info #################################" 	<< std::endl;
  //std::cout <<  "                        material_: " << material_  << " SI unit" << std::endl;
  std::cout <<  "                                  volNbGP: " << volNbGP_ << " SI unit" << std::endl;
  std::cout <<  "                              volNbSprime: " << volNbSprime_ << " SI unit" << std::endl;
  std::cout <<  "                        volNbPrecipitates: " << volNbPrecipitates_ << " SI unit" << std::endl;
  std::cout <<  "                             meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;
  std::cout <<  "                              molarVolume: " << molarVolume_ << " SI unit" << std::endl;
  std::cout <<  "                             YoungModulus: " << YoungModulus_ << " SI unit" << std::endl;
  std::cout <<  "                             PoissonCoeff: " << PoissonCoeff_ << " SI unit" << std::endl;
  std::cout <<  "                         latticeParameter: " << latticeParameter_ << " SI unit" << std::endl;
  //std::cout <<  "                         precipitateList_: " << precipitateList_ << " SI unit" << std::endl;
  //std::cout <<  "                         chemicalComposition_: " << chemicalComposition_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}
