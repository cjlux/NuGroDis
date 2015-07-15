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
#include <cmath>

#include "Material.hpp"
#include "Precipitate.hpp"
#include "RadiusDistribution.hpp"
#include "Grain.hpp"
#include "Polynomial.hpp"
#include "Concentration.hpp"
#include "ChemicalComposition.hpp"
#include "ChemicalElement.hpp"
#include "ThermoDynamicsConstant.hpp"
#include "Temperature.hpp"
#include "Diffusion.hpp"

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
   phaseChangeVolumiqueEnergy_(-1),
   nucleationSitesNumber_(-1),
   preExpTermForSolvus_(-1),
   surfaceEnergyPolynomial_(),
   surfaceEnergyCurrentValue_(0),
   shapeFactor_(-1),
   volumicFraction_(0),
   criticalRadius_(-1),
   deltaCriticalRadius_(-1),
   criticalTotalEnergy_(-1),
   ZeldovichFactor_(-1),
   criticalBeta_(-1),
   nucleationRate_(-1),
   radiusDistributionList_(),
   currentRadiusDistribution_(0),
   initialRadiusDistribution_(initialRD),
   sumOfStoicCoefs_(0),
   SumOfStoiCoefsHasBeenComputed_(false),
   solubilityProduct_(-1),
   equilibriumConcMap_()
{
  std::cout<<"Build a C++ Precipitate object"<<std::endl;
  
  currentRadiusDistribution_=&initialRD;
  radiusDistributionList_.push_back(currentRadiusDistribution_);
  
  //TODO search and assert *this in (mat.precipitateList_) . if *this not in (mat.precipitateList_) then Do:  mat.AddPrecipitate(*this)
   mat.AddPrecipitate(*this);
  
  std::cout<<"Exit contructor of Precipitate Object"<<std::endl;  
  
}

//TODO
Precipitate::~Precipitate()
{

}


void 
Precipitate::InitializeEquilibriumConcentrationMap()
{
  std::cout<<"Initializing a precipitate equilibrium concentration map"<<std::endl;
  
  
  assert( (equilibriumConcMap_.size()==0)&&"In InitializeEquilibriumConcentrationMap() for precipitate,\
  equilibriumConcMap_ has already been initialized") ;
  
  std::map<std::string , Concentration*>::iterator it;
  
  for (it=chemicalComposition_.GetConcentrationMap().begin(); it!=chemicalComposition_.GetConcentrationMap().end(); ++it )
  {
    if ( (it->first)!= (materialPointer_->GetMainChemicalElement().GetElementName()) )
    {
      equilibriumConcMap_[it->first]=-1; //initialize precipitate equilibrium concentratations
    };  
  }

  
}

double 
Precipitate::GetEquilibriumConcentrationForElement(std::string elementSymbol)
{
  assert ( (elementSymbol!=materialPointer_->GetMainChemicalElement().GetElementName() )&&"In precipitate,GetEquilibriumConcentrationForElement. Cannot get the equilibrium \
  concentration of the main chemical element!!!. main element does not have equilib. conc.!!! " );
  
  
  
  std::map<std::string , double>::iterator it;
  it=equilibriumConcMap_.find(elementSymbol);
  assert( (it!=equilibriumConcMap_.end())&&"Cannot find given symbol in equilibriumConcMap_ " );
  
  assert ( (it->second!=-1)&&"In precipitate, Cannot get equilibrium concentration because it has not been computed yet");
     

  
  return it->second;
}




//Compute and Set distorsion Energy
void 
Precipitate::ComputeDistorsionEnergy()
{
// WARNING youngModulus_ unit must be MPa
  assert((deltaCell_!=-1)&&"Cannot compute Distorsion energy: Precipitate's deltaCell has not been defined");
  
  distorsionEnergy_=1000000*(materialPointer_->GetYoungModulus())/(1-materialPointer_->GetPoissonCoeff())*(deltaCell_)*(deltaCell_);
}


//TODO
//Compute and set Gibbs Free Energy for phase change (was DeltaGv)
void 
Precipitate::ComputePhaseChangeVolumicEnergy()
{ 
  double R=ThermoDynamicsConstant::GetR();
  double T=materialPointer_->GetTemperature().GetCurrentTemp();
  std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
  std::map<std::string, Concentration*> materialCurrentConcMap= materialPointer_->GetCurrentChemicalComposition().GetConcentrationMap();
  std::map<std::string, Concentration*> precipitateConcMap=chemicalComposition_.GetConcentrationMap();
  std::map<std::string, Concentration*>::iterator it;
  double product=1;
  
  for (it=precipitateConcMap.begin(); it!=precipitateConcMap.end(); ++it )
  {
    
    if ( it->first != mainElementSymbol  )
    {
      assert( ( equilibriumConcMap_[it->first]!=-1)&&"in ComputePhaseChangeVolumicEnergy(): equilibrium concentration for precipitate has not been computed yet " );
      int stoiCoef= it->second->GetStoichiometricCoef();
      double XvEq=equilibriumConcMap_[it->first];
      double XvSS=  materialCurrentConcMap[it->first]->GetVolumicValue();
      
      std::cout<<"XvSS is"<<XvSS<<std::endl;
      std::cout<<"stoiCoef is"<<stoiCoef<<std::endl;
      std::cout<<"XvEq is"<<XvEq<<std::endl;
      
      assert ( (XvSS>0)&&"Current vomimic concentration in material must be superior to 0");
      product*=std::pow( XvEq/XvSS,stoiCoef);  
    };
  }
  
  std::cout<<"product is"<<product<<std::endl;
  
  assert( (molarVolume_>0)&&"In ComputePhaseChangeVolumicEnergy(): molarVolume has not been defined" );
  
  phaseChangeVolumiqueEnergy_=R*T/molarVolume_*std::log(product);
  
  
  //post conditions
  //assert if computed value is negative. phaseChangeVolumiqueEnergy_ must always be negative
  assert( (phaseChangeVolumiqueEnergy_ <= 0)&&"In Precipitate.ComputePhaseChangeVolumicEnergy(): value computed for phaseChangeVolumiqueEnergy_ is not negative!!!" );
}

// 
double
Precipitate::ReturnCriticalRadius()
{
 
  //Assert DeltaGv and DeltaGe have been computed before, and correct (current) values are used
  double oldDeltaGv=phaseChangeVolumiqueEnergy_;
  double oldDeltaGe=distorsionEnergy_;
  double oldGamma=surfaceEnergyCurrentValue_;
  this->ComputePhaseChangeVolumicEnergy();//compute another value of phaseChangeVolumiqueEnergy_
  this->ComputeDistorsionEnergy(); //compute another value of distorsionEnergy_
  this->ComputeSurfaceEnergy();
  assert ( (phaseChangeVolumiqueEnergy_==oldDeltaGv)&&"may be phaseChangeVolumiqueEnergy_ has not been computed before\
  running method ReturnCriticalRadius()");
  assert ( (distorsionEnergy_==oldDeltaGe)&&"may be distorsionEnergy_ has not been computed before\
  running method ReturnCriticalRadius()");
  assert ( (surfaceEnergyCurrentValue_==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before\
  running method ReturnCriticalRadius()");
  
  
  
  double criticalRadius= -4/3*surfaceEnergyCurrentValue_/(distorsionEnergy_+phaseChangeVolumiqueEnergy_)*(shapeFactor_+2)/(shapeFactor_+4/3);
  
  assert ((criticalRadius>0)&&"In ReturnCriticalRadius: computed value of criticalRadius is not strictly positive");
 
  return criticalRadius; 
}
//   
double //TODO
Precipitate::ReturnDeltaCriticalRadius()
{
  return 0;
}


//return Z
double 
Precipitate::ReturnZeldovichFactor()
{
  double T=materialPointer_->GetTemperature().GetCurrentTemp();
  double Na=ThermoDynamicsConstant::GetNa();
  double R=ThermoDynamicsConstant::GetR();
  
  
  assert( (molarVolume_!=-1)&&"molar Volume of precipitate has not been initialized");
  assert ( (criticalRadius_==this->ReturnCriticalRadius())&&"In ReturnZeldovichFactor(): Critical radius may have not been computed");
  assert ( (deltaCriticalRadius_==this->ReturnDeltaCriticalRadius())&&"In ReturnZeldovichFactor(): deltaCriticalRadius_ may ave not been computed!!! " );
  double oldGamma=surfaceEnergyCurrentValue_;
  this->ComputeSurfaceEnergy();
  assert ( (surfaceEnergyCurrentValue_==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before\
  running method ReturnZeldovichFactor()");
  
  assert( (T!=0)&&(Na!=0)&&(R!=0) );
  assert ( (criticalRadius_ + deltaCriticalRadius_)!=0 );

  
  
  double Z=molarVolume_/(2*M_PI*std::pow(criticalRadius_ + deltaCriticalRadius_, 2))*std::pow(surfaceEnergyCurrentValue_/(Na*R*T), 0.5 );
 
  //TODO post assertions!!!
  assert( (Z>0)&&"Computed Zeldovich factor is not strictly positive" );
  
  return Z;
}


//Return betaetoile
double //TODO
Precipitate::ReturnCriticalBeta()
{
  double latticeParam=materialPointer_->GetLatticeParameter();
  std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
  std::map<std::string, Concentration*> precipitateConcMap=chemicalComposition_.GetConcentrationMap();
  std::map<std::string, Concentration*>::iterator it;
  std::map<std::string, Concentration*>::iterator it2;
  std::vector<const ChemicalElement*> soluteList =materialPointer_->GetSoluteList();
  
  double sum=0; // sum of 1/(D_i_SS*X_at_i)
  
  for (it2=precipitateConcMap.begin(); it2!=precipitateConcMap.end(); ++it2 )
  {
    if ( it->first != mainElementSymbol  )
    {
      double currentAtomicConc=materialPointer_->ReturnAtomicConcFromVolumicForElement(it->first);  
      
      //TODO some stuff
      for( std::vector<const ChemicalElement*>::const_iterator i = soluteList.begin(); i != soluteList.end(); ++i)
      {
	if ( (*i)->GetElementName() == it->first)
	{
	  double Dcoef = (*i)->GetDiffusion().GetAtomicDiffusionCoef();
	  assert(Dcoef>0);
	  assert (currentAtomicConc>0);
	  sum += 1/(currentAtomicConc*Dcoef);
	};
      }
      
    };
  
    
  }
  
  assert( (sum!=0)&&(latticeParam!=0) );
  double beta=4*M_PI*std::pow((criticalRadius_+deltaCriticalRadius_), 2)/std::pow(latticeParam,4)/sum;
  
  return beta;
}

double //TODO
Precipitate::ReturnNucleationRate()
{
  //TODO!!!! Using ZeldovichFactor_, nucleationSitesNumber_ , criticalBeta_ , criticalTotalEnergy_ .
  //So these values has to be computed before Using this method ReturnNucleationRate()
  assert( (ZeldovichFactor_!=-1)&&"Zeldovich factor has not been computed yet!");
  
  assert( (nucleationSitesNumber_!=-1&&"ReturnNucleationRate(): Nucleation site number(nucleationSitesNumber_) has not been computed yet") );
  
  double currentNucleationSiteNb=nucleationSitesNumber_;/*TODO return current nucleation site number, assert it has been computed before*/
  double Z=this->ReturnZeldovichFactor();
  double beta=this->ReturnCriticalBeta();
  double DeltaG=this->ReturnCriticalTotalEnergy(); //ReturnCriticalTotalEnergy() method is now in inherited class
  double kB=ThermoDynamicsConstant::GetkB();
  double T=materialPointer_->GetTemperature().GetCurrentTemp();
  
  assert( (Z==ZeldovichFactor_)&&"Incompatibility: May be Zeldovich  has not computed before" );
  assert( (beta==criticalBeta_)&&"Incompatibility: May be criticalBeta_  has not computed before" );
  assert( (DeltaG==criticalTotalEnergy_)&&"Incompatibility: May be criticalTotalEnergy_  has not computed before" );
  
  
  double J=Z*currentNucleationSiteNb*beta*std::exp(-DeltaG/(kB*T));
  
  //post assertions?
  
  
  return J;
}

void 
Precipitate::ComputeCriticalRadius()
{
  criticalRadius_=this->ReturnCriticalRadius();
}

void //TODO 
Precipitate::ComputeDeltaCriticalRadius()
{
  
}

void 
Precipitate::ComputeZeldovichFactor()
{
  ZeldovichFactor_=this->ReturnZeldovichFactor();
}

void 
Precipitate::ComputeCriticalBeta()
{
  criticalBeta_=this->ReturnCriticalBeta();
  
  //asserions???
}

void 
Precipitate::ComputeNucleationRate()
{
  nucleationRate_=this->ReturnNucleationRate();
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

void
Precipitate::ComputeSurfaceEnergy()
{
  assert ( (materialPointer_!=0)&&"In ComputeSurfaceEnergy() of Precipitate: Precipitate does not belongs to any material!!!"); 
  double T= materialPointer_->GetTemperature().GetCurrentTemp();
  
  surfaceEnergyCurrentValue_ = surfaceEnergyPolynomial_.ReturnValue(T);
  
}


void //TODO
Precipitate::ReadDataValue(std::string fileName)
{
}

void //TODO
Precipitate::AddRadiusDistribution(const RadiusDistribution& RD)
{

}

void //TODO
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
    
    assert((volumicConc>0)&&"In ConvertAtomicToVolumicConcentration(): computed value of volumic conc is negative or null");
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
      
      //TODO !!! assert((volumicConc>0)&&"In ConvertAtomicToVolumicConcentration(): corrected rounding value of volumic conc is negative or null");
      
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

