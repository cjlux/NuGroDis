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
#include <algorithm>
#include <algorithm>

#include "ChemicalComposition.hpp"
#include "SSGrain.hpp"
#include "Material.hpp"
#include "ChemicalElement.hpp"
#include "Concentration.hpp"


//Build a simple classe CC. Does not need any other argument than formula 
//CC built belongs to no materialPointer_ and no grainPointer_(grainPointer_ can be Precipitate or SSGrain)
ChemicalComposition::ChemicalComposition(std::string formula)
  :formula_(formula),
   materialPointer_(0),
   grainPointer_(0),
   chemicalElementList_(),
   concentrationList_(),
   concentrationMap_(),
   concentrationPyDict_(),
   mapModified_(false),
   mainElementName_("Undefined")
{
  std::cout<<   "Enter in ChemicalComposition constructor"<<std::endl;
  std::cout <<  "Building a simple object <ChemicalComposition> which is not linked to any component yet  " << std::endl;
  //TODO Don't forget to set ChemicalElements because a chemical composition has at least one ChemicalElement. A CC must know the CE it is made of.
       //Maybe constructor can be modified to receive a list of ChemicalElement to initialize chemecalElementList_???

  
//   /*debug*/std::cout<<   "adress of the ChemicalCompo built is"<<this<<std::endl;
  std::cout<<   "Exit ChemicalComposition constructor"<<std::endl;
}



//GENERAL CONSTRUCTOR FOR ANY DERIVED CLASS OF VIRTUAL CLASS GRAIN
//class CC Built only belongs to a Grain(can be -> Precipitate or SSGrain or ... ), and NOT to a material
//materialPointer_=0, means ChemicalComposition is only for Grain and not necessary for material.ssgrainPointer_.GetComposition() 
//Grain can be a lot of things: SSGrain, Precipitate, ... or somethings else in the future 
//Precipitate can be a lot of things GP, Sprime, ...)
//belongs to a grain_(Precipitate or SSGrain) but no material
ChemicalComposition::ChemicalComposition(std::string formula, Grain &grain) //A CC can know the grain its formula is for. grain can be SS or Precipitate
  :formula_(formula),
   materialPointer_(0),
   grainPointer_(&grain),
   chemicalElementList_(),
   concentrationList_(),
   concentrationMap_(),
   concentrationPyDict_(),
   mapModified_(false),
   mainElementName_("Undefined")
{	
  std::cout <<  "Building object <ChemicalComposition> which belongs exclusively to a Grain " << std::endl;
 
  //TODO If grain.type==SSGrain then: materialPointer_=grain.GetMaterial() 
                  // grain.type==Precipitate then: material=0 
                               
}


/*//SPECIALIZED CONSTRUCTOR FOR Precipitate which is derived from virtual class Grain
//Precipitate can be a lot of things GP, Sprime, ...)
ChemicalComposition::ChemicalComposition(std::string formula, Precipitate &P) //A CC can know the grain its formula is for. grain can be SS or Precipitate
  :formula_(formula),
   materialPointer_(0),
   grainPointer_(&P),
   chemicalElementList_(),
   concentrationList_(),
   concentrationMap_(),
   concentrationPyDict_(),
   mapModified_(false),
   mainElementName_("Undefined")
{	
  std::cout <<  "Building object <ChemicalComposition> which belongs to a precipitate " << std::endl;
 
}*/


//CC built belongs only to a material, and not to a Grain, thus, not belong to the material's ssgrain
ChemicalComposition::ChemicalComposition(std::string formula,  Material& mat) 
  :formula_(formula),
   materialPointer_(&mat),
   grainPointer_(0),
   chemicalElementList_(),
   concentrationList_(),
   concentrationMap_(),
   concentrationPyDict_(),
   mapModified_(false),
   mainElementName_("Undefined")
{
  
  //TODO Don't forget to set ChemicalElements because a chemical composition has at least one ChemicalElement. A CC must know the CE it is made of.
       //Maybe constructor can be modified to receive a list of ChemicalElement to initialize chemecalElementList_???
}


//CONSTRUCTOR
//Belongs to a material and to ssgrain. Material and its SSGrain has the same Chemical Composition
ChemicalComposition::ChemicalComposition(std::string formula,  Material& mat, SSGrain& materialSSgrain) 
  :formula_(formula),
   materialPointer_(&mat),
   grainPointer_(&materialSSgrain),
   chemicalElementList_(),
   concentrationList_(),
   concentrationMap_(),
   concentrationPyDict_(),
   mapModified_(false),
   mainElementName_("Undefined")
{
  assert( (&materialSSgrain==mat.GetSSGrainPointer()) && "Error in contructor: Cannot Build object Chemical\
  Composition because the SSGrain given in argument does not belong to the material given in argument ");
  
  //TODO Don't forget to set ChemicalElements because a chemical composition has at least one ChemicalElement. A CC must know the CE it is made of.
       //Maybe constructor can be modified to receive a list of ChemicalElement to initialize chemecalElementList_???
}

/* TODO ?
ChemicalComposition::ChemicalComposition(ChemicalComposition const& CCtoCopy)
  :formula_(CCtoCopy.formula_),
   chemicalElementList_(CCtoCopy.chemicalElementList_),
   concentrationList_(CCtoCopy.concentrationList_),
   concentrationMap_(CCtoCopy.concentrationMap_),
   concentrationPyDict_(CCtoCopy.concentrationPyDict_),
   mapModified_(CCtoCopy.mapModified_),
   mainElementName_(CCtoCopy.mainElementName_)
{
   materialPointer_ = new ChemicalComposition (*(CCtoCopy.materialPointer_)); //eroor
   grainPointer_ = new ChemicalComposition ( *(CCtoCopy.grainPointer_)); //error
}*/


//Copy Contructor
/*
ChemicalComposition::ChemicalComposition(ChemicalComposition const& CCtoCopy)
 :formula_(CCtoCopy.formula_),
  materialPointer_(CCtoCopy.materialPointer_),
  grainPointer_(CCtoCopy.grainPointer_),
  chemicalElementList_(CCtoCopy.chemicalElementList_),
  concentrationPyDict_(CCtoCopy.concentrationPyDict_),
  mapModified_(CCtoCopy.mapModified_),
  mainElementName_(CCtoCopy.mainElementName_)
{
 
 
  concentrationList_=CCtoCopy.concentrationList_;
  concentrationMap_=CCtoCopy.concentrationMap_;
  
}*/


//Operator =
/*
ChemicalComposition& 
ChemicalComposition::operator=(ChemicalComposition const& CCtoCopy)
{
  if(this != &CCtoCopy)
  {
    formula_=CCtoCopy.GetFormula();
    
    materialPointer_=CCtoCopy;
    
    grainPointer_=CCtoCopy;
    chemicalElementList_=CCtoCopy;
    concentrationList_=CCtoCopy;
    concentrationMap_=CCtoCopy;
    concentrationPyDict_=CCtoCopy;
    mapModified_=CCtoCopy;
    mainElementName_=CCtoCopy; 
    
  };
  return *this;

}*/



//TODO could be return in declaration
std::map<std::string, Concentration*>& 
ChemicalComposition::GetConcentrationMap()
{
  //begin debug
//   std::map<std::string , Concentration*>::iterator it;
//   std::cout<<"In Get ConcentartionMap. C++ Condentration adresses in Map are "<<std::endl;
//   for (it=concentrationMap_.begin(); it!=concentrationMap_.end(); ++it )
//   {
//     std::cout<<"Element Name: "<<it->first<<"; Adress is :"<<it->second<<std::endl;
//   }
  //end debug
  
  
  
  
  return concentrationMap_;
  
}









boost::python::list 
ChemicalComposition::GetChemicalElementsPyList()
{
  boost::python::list chemicalElementsPyList;
  
  for( std::vector<ChemicalElement *>::const_iterator i = chemicalElementList_.begin(); i != chemicalElementList_.end(); ++i)
  {
    chemicalElementsPyList.append(*i);
  }
  return chemicalElementsPyList;
}


boost::python::list 
ChemicalComposition::GetConcentrationPyList()
{
  boost::python::list ConcentrationPyList;
  
  for( std::vector<Concentration *>::const_iterator i = concentrationList_.begin(); i != concentrationList_.end(); ++i)
  {
    ConcentrationPyList.append(*i);
  }
  return ConcentrationPyList;
}


//TODO
//Concentration& 
//ChemicalComposition::GetConcentrationForElement(const ChemicalElement& CE) const
//{
//  std::vector<ChemicalElement *>::iterator it;
    
  //it = find(chemicalElementList_.begin(), chemicalElementList_.end(), CE);
  /*if (it != chemicalElementList_.end())
    std::cout << "Chemical Element found in chemicalElementList: " << *it << std::endl;
  else
    std::cout << "Chemical Element not found in chemicalElementList\n";*/

//std::vector<Concentration *>::iterator it2(it);
  
  //return *it;
//}



//TODO const ? referecne?
Concentration& 
ChemicalComposition::GetConcentrationForElement( std::string elementSymbol)
{
  
  std::map<std::string , Concentration*>::iterator it;
  it=concentrationMap_.find(elementSymbol);
  assert( (it!=concentrationMap_.end())&&"Cannot find given symbol in concentrationMap_ " );
 
  
  
  return *(it->second);
  
}





void
ChemicalComposition::AddChemicalElements(ChemicalElement& CE, Concentration& C )
{
  //preconditions
 assert( (chemicalElementList_.size()==concentrationMap_.size())&&"ChemicalElementList and Concentration List must have the same size" );
 
//  /*this line is debug, delete after */std::cout<<"In AddChemicalElement:  Adress of concentration added is: "<<&C<<std::endl;
//  /*this line is debug, delete after */std::cout<<"In AddChemicalElement:  Adress of ChemicalComposition is: "<<this<<std::endl;
 
 
 
 chemicalElementList_.push_back(& CE);
 concentrationMap_[CE.GetElementName()] = & C;
 mapModified_=true;

 
 //postConcditions
 assert( (chemicalElementList_.size()==concentrationMap_.size())&&"ChemicalElementList and Concentration List must have the same size" );


}



// Static method that Convert a C++ map to a python dict
/*DO NOT USE THIS IF SECOND ATTRIBUTE OF MAP IS AN ADRESS!!!
 IF SECOND ATTRIBUTE OF MAP IS AN ADRESS,  IT CAN BE USED, BUT BE AWARE THAT 
 IT CAN ONLY BE USED FOR GETTING INFORMATIONS, NOT SETTING !!! */
template <class K, class V>
void ChemicalComposition::MapToPythonDict(std::map<K, V> & map, boost::python::dict & concentrationPyDict)
{
       typename std::map<K, V>::iterator iter;
       std::cout<<""<<std::endl;
      for ( iter = map.begin(); iter != map.end(); ++iter) 
      {
	std::cout<<"iter-first: "<<iter->first<<std::endl;
	std::cout<<"iter-second: "<<iter->second<<std::endl;
	if ( !concentrationPyDict.has_key(iter->first) )
	{
	concentrationPyDict[iter->first] = iter->second;
	assert( (concentrationPyDict[iter->first]==iter->second)&&"Value is not properly add from C++ Map To python Dictionary!!!" );
	}
	else
	{
	  assert ( (iter->second==concentrationPyDict[iter->first])&&"Python dict already has a value for key, value cannot be modified" );
	};
	
	
      }
  
}



boost::python::dict& 
ChemicalComposition::GetConcentrationPyDict()
{
 if (mapModified_)
  {
  ChemicalComposition::MapToPythonDict(concentrationMap_, concentrationPyDict_);
  mapModified_=false;
  }

  return concentrationPyDict_;  
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
  std::cout <<  "                                      materialPointer: " << materialPointer_ << " SI unit" << std::endl;
  std::cout <<  "                                         grainPointer: " << grainPointer_ << " SI unit" << std::endl;
  std::cout <<  "                                          mainElement: " << mainElementName_ << std::endl;
  //std::cout <<  "                                chemicalElementList: " << chemicalElementList_ << " SI unit" << std::endl;
  //std::cout <<  "                                  concentrationList: " << concentrationList_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}