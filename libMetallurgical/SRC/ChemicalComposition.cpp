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

//#include <boost/python.hpp>
#include <iostream>
#include <cassert>

#include "ChemicalComposition.hpp"
#include "SSGrain.hpp"
#include "Material.hpp"


//Build a simple classe CC. Does not need any other argument than formula 
//CC built belongs to no material_ and no grain_(grain_ can be Precipitate or SSGrain)
ChemicalComposition::ChemicalComposition(std::string formula)
  :formula_(formula),
   material_(0),
   grain_(0),
   //grain_( *((*material_).GetSSGrainList()[0]) ),
   chemicalElementList_(),
   concentrationList_()
{
  std::cout <<  "Building a simple object <ChemicalComposition> which is not linked to any component yet  " << std::endl;
  //TODO Don't forget to set ChemicalElements because a chemical composition has at least one ChemicalElement. A CC must know the CE it is made of.
       //Maybe constructor can be modified to receive a list of ChemicalElement to initialize chemecalElementList_???
}



//GENERAL CONSTRUCTOR FOR ANY DERIVED CLASS OF VIRTUAL CLASS GRAIN
//class CC Built only belongs to a Grain(can be -> Precipitate or SSGrain or ... ), and NOT to a material
//material_=0, means ChemicalComposition is only for Grain and not necessary for material.ssgrain_.GetComposition() 
//Grain can be a lot of things: SSGrain, Precipitate, ... or somethings else in the future 
//Precipitate can be a lot of things GP, Sprime, ...)
//belongs to a grain_(Precipitate or SSGrain) but no material
ChemicalComposition::ChemicalComposition(std::string formula, Grain &grain) //A CC can know the grain its formula is for. grain can be SS or Precipitate
  :formula_(formula),
   material_(0),
   grain_(&grain),
   chemicalElementList_(),
   concentrationList_()
{	
  std::cout <<  "Building object <ChemicalComposition> which belongs exclusively to a Grain " << std::endl;
 
  //TODO If grain.type==SSGrain then: material_=grain.GetMaterial() 
                  // grain.type==Precipitate then: material=0 
                               
}


/*//SPECIALIZED CONSTRUCTOR FOR Precipitate which is derived from virtual class Grain
//Precipitate can be a lot of things GP, Sprime, ...)
ChemicalComposition::ChemicalComposition(std::string formula, Precipitate &P) //A CC can know the grain its formula is for. grain can be SS or Precipitate
  :formula_(formula),
   material_(0),
   grain_(&P),
   chemicalElementList_(),
   concentrationList_()
{	
  std::cout <<  "Building object <ChemicalComposition> which belongs to a precipitate " << std::endl;
 
}*/


//CC built belongs only to a material, and not to a Grain, thus, not belong to the material's ssgrain
ChemicalComposition::ChemicalComposition(std::string formula,  Material& mat) 
  :formula_(formula),
   material_(&mat),
   grain_(0),
   chemicalElementList_(),
   concentrationList_()
{
  
  //TODO Don't forget to set ChemicalElements because a chemical composition has at least one ChemicalElement. A CC must know the CE it is made of.
       //Maybe constructor can be modified to receive a list of ChemicalElement to initialize chemecalElementList_???
}


//CONSTRUCTOR
//Belongs to a material and to ssgrain. Material and its SSGrain has the same Chemical Composition
ChemicalComposition::ChemicalComposition(std::string formula,  Material& mat, SSGrain& materialSSgrain) 
  :formula_(formula),
   material_(&mat),
   grain_(&materialSSgrain),
   chemicalElementList_(),
   concentrationList_()
{
  assert( (&materialSSgrain==mat.GetSSGrain()) && "Error in contructor: Cannot Build object Chemical Composotion because the SSGrain given in argument does not belong to the material given in argument ");
  
  
  //TODO Don't forget to set ChemicalElements because a chemical composition has at least one ChemicalElement. A CC must know the CE it is made of.
       //Maybe constructor can be modified to receive a list of ChemicalElement to initialize chemecalElementList_???
}





ChemicalComposition::~ChemicalComposition()
{
}

void
ChemicalComposition::UpdateVolumicValues()
{
}

void
ChemicalComposition::Info() const
{
  std::cout <<  "################################# ChemicalComposition::Info #################################" 	<< std::endl;
  std::cout <<  "                                              formula: " << formula_  << " SI unit" << std::endl;
  //std::cout <<  "                                           material: " << material_ << " SI unit" << std::endl;
  //std::cout <<  "                                              grain: " << grain_ << " SI unit" << std::endl;
  //std::cout <<  "                                chemicalElementList: " << chemicalElementList_ << " SI unit" << std::endl;
  //std::cout <<  "                                  concentrationList: " << concentrationList_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}