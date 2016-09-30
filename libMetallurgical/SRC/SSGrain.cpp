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
#include "ChemicalComposition.hpp"
#include "Concentration.hpp"


//FAKE METHOD
void 
SSGrain::test()
{
  std::cout<<"SSGrain::test()"<<std::endl;
  std::cout<<"Size = "<<precipitateList_.size()<<std::endl;
  std::cout<<"Adresse of precipitate vector is "<<&precipitateList_<<std::endl;
  std::cout <<"Adresse of precipitate object are: ";
  for( std::vector<Precipitate*>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {std::cout << *i << '|';}
  std::cout <<""<<std::endl;
}





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
   precipitateList_( * new std::vector<Precipitate*>)// precipitateList_ is initialize by reference, thus an object must be given in the construction
   
{
  assert( (mat.GetSSGrainPointer()==0) && ("WARNING! Cannot build a SSGrain object: The material has already solid solution grains! Material ssgrain has already been initialize") );

  
}


/* Good code!!! un comment after and delete what is downside
//SPECIAL CONSTRUCTOR
//make a grain which belongs to a material
//A grain has the same properties as the material: ChemicalCompo and mechanical properties
SSGrain::SSGrain(Material& mat, int volNbGP,int volNbSprime)
  :   Grain(mat, mat.GetCurrentChemicalComposition() ),
      YoungModulus_(mat.GetYoungModulus()),
      PoissonCoeff_(mat.GetPoissonCoeff()), 
      latticeParameter_(mat.GetLatticeParameter()),
      volNbGP_(volNbGP),
      volNbSprime_(volNbSprime),
      volNbPrecipitates_(volNbGP+volNbSprime),
      precipitateList_(* new std::vector<Precipitate*>)
{
  assert( (mat.GetSSGrainPointer()==0) && ("WARNING! Cannot build a SSGrain object:Material attribute ssgrain has already been initialize! The material has already solid solution grains!  ") );
  
  std::cout<<"Enter in SSGRain's constructor"<<std::endl;
  
  std::cout<<"Adress of ssgrain  Object ="<<this<<std::endl;
  mat.SetSSGrain(*this);
  std::cout<<"After set. Material ssgraion adress is :"<<mat.GetSSGrainPointer();
  this->Info();
  std::cout<<"Exit SSGrain constructor"<<std::endl;
}

*/



//TODO  Test: if good, erase conctructior that is upside 
//SPECIAL CONSTRUCTOR
//make a grain which belongs to a material
//A grain has the same properties as the material: ChemicalCompo and mechanical properties
SSGrain::SSGrain(Material& mat, int volNbGP,int volNbSprime)
 :    Grain(mat, *(mat.GetCurrentChemicalComposition().Clone()) ),
      YoungModulus_(mat.GetYoungModulus()),
      PoissonCoeff_(mat.GetPoissonCoeff()), 
      latticeParameter_(mat.GetLatticeParameter()),
      volNbGP_(volNbGP),
      volNbSprime_(volNbSprime),
      volNbPrecipitates_(volNbGP+volNbSprime),
      precipitateList_(mat.GetPrecipitateList())

{

  //ChemicalComposition* a= new ChemicalComposition("Al2Cu");
  
  assert( (mat.GetSSGrainPointer()==0) && ("WARNING! Cannot build a SSGrain object:Material attribute ssgrain has already been initialize! The material has already solid solution grains!  ") );
  
  
  
  
  
  std::cout<<"Enter in SSGRain's constructor"<<std::endl;
  
  // make a copy of materalInitialComposition and assign it to chemical composition of SSGrain 
  //because SSGrain CC is the current one. Thus, at the beginning, SSGrain CC is the Initial CC of marterial
  
  
 
 
  

  
  std::cout<<"Adress of ssgrain  Object ="<<this<<std::endl;
  mat.SetSSGrain(*this);
  std::cout<<"After set. Material ssgraion adress is :"<<mat.GetSSGrainPointer();
  this->Info();
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
SSGrain::ConvertMassicToVolumicConcentration()
{
  
  //TODO assert if CC is in chemicalCompositionList_. If not, abandon 
  
}

void
SSGrain::ConvertVolumicToMassicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
SSGrain::ConvertAtomicToVolumicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
SSGrain::ConvertVolumicToAtomicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
  
  
  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string , Concentration*>::iterator it2;
  std::map<std::string, Concentration*> concMap=chemicalComposition_.GetConcentrationMap();
  
    
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {
    double volumicConc=0;
    double elementRho=0;
    double elementMolarMass=0;
    double sum=0;
    for (it2=concMap.begin(); it2!=concMap.end(); ++it2)
    {
      volumicConc=it2->second->GetVolumicValue();
      elementRho=it2->second->GetChemicalElement().GetDensity();
      elementMolarMass=it2->second->GetChemicalElement().GetMolarMass();
      sum+=volumicConc*elementRho*1000/elementMolarMass;
      
    }
    
    volumicConc=it->second->GetVolumicValue();
    elementRho=it->second->GetChemicalElement().GetDensity();
    elementMolarMass=it->second->GetChemicalElement().GetMolarMass();
    
    
    
    
    
    double atomicConc= (volumicConc*elementRho*1000/elementMolarMass)/sum;
    it->second->SetInitialAtomicValue(atomicConc);
    std::cout<<"===================== VALUE COMPUTED FOR INITIAL ATOMIC CONC OF ELEMENT <"<<it->first<<"> IN SOLID SOLUTION GRAIN IS ============= "<<it->second->GetInitialAtomicValue()<<std::endl;
  }
  
}







//TEST
/*void
SSGrain::SetSSGrainChemicalComposition(const ChemicalComposition &CC)
{
  chemicalComposition_=CC;
}*/



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
