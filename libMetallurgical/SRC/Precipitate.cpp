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

#include <vector>
#include <map>
#include <cassert>

#include "Material.hpp"
#include "Precipitate.hpp"
#include "RadiusDistribution.hpp"
#include "Grain.hpp"
#include "Polynomial.hpp"
#include "Concentration.hpp"
#include "ChemicalComposition.hpp"
#include "ChemicalElement.hpp"

/*Precipitate::Precipitate()
  :deltaCell_(-1),
   solvusActivationEnergy_(-1),
   distorsionEnergy_(-1),
   nucleationSitesNumber_(-1),
   preExpTermForSolvus_(-1),
   surfaceEnergyPolynomialModel_(),
   shapeFactor_(-1),
   volumicFraction_(-1)
{
  
}*/

Precipitate::Precipitate(Material& mat,ChemicalComposition &CC, RadiusDistribution &initialRD)
  :Grain(mat,CC),
   deltaCell_(-1),
   solvusActivationEnergy_(-1),
   distorsionEnergy_(-1),
   nucleationSitesNumber_(-1),
   preExpTermForSolvus_(-1),
   surfaceEnergyPolynomial_(),
   shapeFactor_(-1),
   volumicFraction_(0),
   radiusDistributionList_(),
   currentRadiusDistribution_(0),
   initialRadiusDistribution_(initialRD),
   sumOfStoicCoefs_(0),
   SumOfStoiCoefsHasBeenComputed_(false)
{
  std::cout<<"Build a C++ Precipitate object"<<std::endl;
  
  currentRadiusDistribution_=&initialRD;
  radiusDistributionList_.push_back(currentRadiusDistribution_);
  
  //TODO search and assert *this in (mat.precipitateList_) . if *this not in (mat.precipitateList_) then Do:  mat.AddPrecipitate(*this)
   mat.AddPrecipitate(*this);
  
  std::cout<<"Exit contructor of Precipitate Object"<<std::endl;  
  
}

Precipitate::~Precipitate()
{
}


//Compute and Set distorsion Energy
void 
Precipitate::ComputeDistorsionEnergy()
{
// WARNING youngModulus_ unit must be MPa
  assert((deltaCell_!=-1)&&"Cannot compute Distorsion energy: Precipitate's deltaCell has not been defined");
  
  distorsionEnergy_=1000000*(materialPointer_->GetYoungModulus())/(1-materialPointer_->GetPoissonCoeff())*(deltaCell_)*(deltaCell_);
}


void 
Precipitate::SetSEPolynomialDegree(const int deg)
{
  surfaceEnergyPolynomial_.SetDegree(deg);
}


void 
Precipitate::AddSEPolynomialPyCoefs(boost::python::list& pythonCoefList)
{
  surfaceEnergyPolynomial_.AddPyCoefs(pythonCoefList);
}

double
Precipitate::ComputeSurfaceEnergy()
{
  return 0;
}


void
Precipitate::ReadDataValue(std::string fileName)
{
}

void
Precipitate::AddRadiusDistribution(const RadiusDistribution& RD)
{

}

void
Precipitate::SetCurrentRadiusDistribution(const RadiusDistribution & RD)
{

}

//TODO ,Does it really need? may be manual set for all attribute would be preferable
/*void 
Precipitate::InitializeParameters(double deltaCell,
				  double solvusActivationEnergy,
				  double nucleationSitesNumber,
				  double preExpTermForSolvus,
				  Polynomial& surfaceEnergyPolynomialModel,
				  double shapeFactor,
				  double volumicFraction)
{
  
  SetDeltaCell(deltaCell);
  SetSolvusActivationEnergy(solvusActivationEnergy);
  SetNucleationSitesNumber(nucleationSitesNumber);
  SetPreExpTermForSolvus(preExpTermForSolvus);
  SetSurfaceEnergyPolynomialModel(surfaceEnergyPolynomialModel);
  SetShapeFactor(shapeFactor);
  SetVolumicFraction(volumicFraction);
}*/


//TODO DO AN INITIALIZITAION THAT CONVERT ALL THAT IS NEED IN THE RIGHT ORDER



//Compute and Set sumOfStoicCoefs_
void
Precipitate::ComputeSumOfStoicCoefs()
{ 
  
  if (!SumOfStoiCoefsHasBeenComputed_)
  {
      std::map<std::string , Concentration*>::iterator it;
      
      std::map<std::string, int> coefsMap;//Use to find max value of Stoichiometric coefs
      std::map<std::string, int>::iterator iter;
      std::map<std::string, int>::iterator maxIter;
      std::map<std::string, Concentration*> concMap=chemicalComposition_.GetConcentrationMap();
      int max=0;
      
      sumOfStoicCoefs_=0;
      
      //Compute and Set sumOfStoicCoefs_
      for (it=concMap.begin(); it!=concMap.end(); ++it)
      {
      assert( (it->second->GetStoichiometricCoef()>0)&&"Cannot Convert Stoichiometric\
      to Atomic concentration because a Stoichiometric coef is not an integer strictly positive" );
      sumOfStoicCoefs_+= it->second->GetStoichiometricCoef();
      coefsMap[it->first] = it->second->GetStoichiometricCoef();
      }
      
      assert ( (sumOfStoicCoefs_>0)&&"Cannot SetSumOfStoicCoefs because computed\
      argument is not an integer stricly positive!" );
      
      for ( iter=coefsMap.begin(); iter!=coefsMap.end(); ++iter )
      {
	if ( iter->second > max ) 
	{
	  max=iter->second;
	  maxIter=iter; 
	};
      }
      assert( (maxIter->second ==max)&&"Cannot find max value" );
      chemicalComposition_.SetMainElementName(maxIter->first);
      std::cout<<"*********************************  MAX element is  ****************************"<<chemicalComposition_.GetMainElementName()<<std::endl;
      
      SumOfStoiCoefsHasBeenComputed_=true;
  }
  
}





void
Precipitate::ConvertMassicToVolumicConcentration()
{
  
  //TODO assert if CC is in chemicalCompositionList_. If not, abandon 
   
}

void
Precipitate::ConvertVolumicToMassicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
Precipitate::ConvertAtomicToVolumicConcentration()
{
  
  std::cout<<"Starting to convert Atomic to Volumic concentration for precipitate "<<std::endl;
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
  
  //Compute And Set SumOfStoichiometric if it has not been done before
  this->ComputeSumOfStoicCoefs();


  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string, Concentration*> concMap=chemicalComposition_.GetConcentrationMap();
  
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {
    double atomicConc=it->second->GetInitialAtomicValue();
    double elementRho=it->second->GetChemicalElement().GetDensity();
    double elementMolarMass=it->second->GetChemicalElement().GetMolarMass();
    
    
    double volumicConc=atomicConc*(elementMolarMass/1000/elementRho)/(molarVolume_/this->GetSumOfStoicCoefs());
    it->second->SetVolumicValue(volumicConc);
    std::cout<<"===================== VALUE COMPUTED FOR VOLUMIC CONC OF ELEMENT <"<<it->first<<"> IN PRECIPITATE IS ============= "<<it->second->GetVolumicValue()<<std::endl;
  }
  
  
  double sumOfOtherVolumicValues=0;
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {
    if ( it->first==chemicalComposition_.GetMainElementName() )
    {
      std::map<std::string , Concentration*>::iterator it2;
      for (it2=concMap.begin(); it2!=concMap.end(); ++it2)
      {
	if (it2!=it)
	{sumOfOtherVolumicValues+=it2->second->GetVolumicValue();};
      }
      
      it->second->SetVolumicValue(1-sumOfOtherVolumicValues);
      std::cout<<"===================== VALUE COMPUTED FOR VOLUMIC CONC OF ELEMENT <"<<it->first<<"> IN PRECIPITATE IS ============= "<<it->second->GetVolumicValue()<<std::endl;
    };
  }
  
  
  
  
  std::cout<<"End of Atomic to volumic Conversion for precipitate"<<std::endl;
}



void
Precipitate::ConvertVolumicToAtomicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
Precipitate::ConvertStoichiometricCoefficientToVolumicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
   
  
}




void
Precipitate::ConvertStoichiometricCoefficientToAtomicConcentration()
{
  std::cout<<"C++. In convert method stoic to atomic"<<std::endl;
  std::cout<<"Precipitate's adress is:"<<this<<std::endl;
  std::cout<<"Precipitate's ChemicalCompo adress is:"<<&chemicalComposition_<<std::endl;
  
  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string, Concentration*> concMap=chemicalComposition_.GetConcentrationMap();
  
  
  //Compute and Set sum Of stoiCoef in Class Concentration if it has not been done before
  this->ComputeSumOfStoicCoefs();
 
  assert ( (sumOfStoicCoefs_!=0)&&"Sum of stoic coefs must be different from zero" ); 
  
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {   
    double conc=double(it->second->GetStoichiometricCoef())/(this->GetSumOfStoicCoefs());// use double cast not to avoid integer division
    std::cout<<"===================== VALUE COMPUTED FOR ATOMIC CONC IS ============= "<<conc<<std::endl;
    it->second->SetInitialAtomicValue(conc);
  }  
  
}







void
Precipitate::Info() const
{
  std::cout <<  "################################# Precipitate::Info #################################" 	<< std::endl;
  std::cout <<  "                                    deltaCell: " << deltaCell_ << " SI unit" << std::endl;
  std::cout <<  "                       solvusActivationEnergy: " << solvusActivationEnergy_ << " SI unit" << std::endl;
  if (distorsionEnergy_==-1)
  {std::cout <<  "                             distorsionEnergy: Not defined" << std::endl;}
  else
  {std::cout <<  "                             distorsionEnergy: " << distorsionEnergy_ << " SI unit" << std::endl;};
  
  std::cout <<  "                        nucleationSitesNumber: " << nucleationSitesNumber_ << " SI unit" << std::endl;
  std::cout <<  "                          preExpTermForSolvus: " << preExpTermForSolvus_ << " SI unit" << std::endl;
 // std::cout <<  "               surfaceEnergyPolynomial: " << surfaceEnergyPolynomial_ << " SI unit" << std::endl;
  std::cout <<  "                                  shapeFactor: " << shapeFactor_ << " SI unit" << std::endl;
  std::cout <<  "                              volumicFraction: " << volumicFraction_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}

