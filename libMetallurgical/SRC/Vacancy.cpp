// This file is part of NuGrodis, a workbench for micro-structural modelling.
//
// Author(s)    : - Jean-luc CHARLES, I2M/ENSAM Talence France
//                  <jean-luc.charles@ensam.eu>
//                - Dominique COUPARD, I2M/ENSAM Talence France
//                  <dominique.coupard@ensam.eu>
//                - Moubarak GADO, I2M/ENSAM Talence France
//                  <gadomoubarak@yahoo.fr>
//                - Ivan IORDANOFF, I2M/ENSAM Talence France
//                  <ivan.iordanoff@ensam.eu>
//
// Copyright (C) 20014-2015 Jean-luc CHARLES, Dominique COUPARD, Moubarak GADO, Ivan IORDANOFF.
//
#include <iostream>
#include <cmath>
#include <map>
#include "Vacancy.hpp"
#include "ChemicalElement.hpp"
#include "Diffusion.hpp"
#include "ThermoDynamicsConstant.hpp"
#include "Material.hpp"
#include "Temperature.hpp"
#include "Concentration.hpp"
#include "ChemicalComposition.hpp"

Vacancy::Vacancy(double deltaHF,double deltaSF,double deltaHM,double fE,double Dlac0,double halfSinkD,double Tf, Material &mat,double coordNb)
  : vacCreationEnthalpy_(deltaHF), 
    vacCreationEntropy_(deltaSF), 
    concentration_(0), 
    concentrationBeforeQuenching_(0), 
    halfSinkDistance_(halfSinkD), 
    jumpFrequency_(fE), 
    preExpDiffusionValue_(Dlac0),  
    migrationEnthalpy_(deltaHM), 
    solutionisingTemp_(Tf), 
    coordinationNumber_(coordNb),
    vacancyDiffusionCoef_(0),
    equilibriumConc_(0),
    material_(mat),
    soluteInteractingWithVacList_()
    
{
  std::cout <<  "Enter in contructor of object <Vacancy> " << std::endl;
  std::cout <<  "Building object <Vacancy> " << std::endl;
  
  /*Do not work, thus, mat.AssertVacancyPointer() is used instead */  //assert ( (mat.GetVacancyPointer==0)&&"Material already has a vacancy!!!"  );
  
  const bool isMaterialVacancyPtrZero = mat.AssertVacancyPointer(0);
  
  if (isMaterialVacancyPtrZero==true)
  {
  mat.SetVacancy(*this);
  
  assert( (coordinationNumber_>=0)&&"In vacancy constructor: Given argument of coordination number must be positive" );
  
  std::cout <<  "Exit contructor of object <Vacancy> " << std::endl;
  }
  else
  {
    assert(!"In Vacancy constructor:  mat.GetvacancyPointer() is not equal to 0. Material already has a vacancy!!!");
  };
  
 

}

Vacancy::~Vacancy()
{
}


void
Vacancy::ComputeDiffusionCoefValue()
{
  std::cout<<"Vacancy::ComputeDiffusionCoefValue() ==> Computing Diffusion Coef Value"<<std::endl;
  double R=ThermoDynamicsConstant::GetR();
  double T=material_.GetTemperature().GetCurrentTemp();
  vacancyDiffusionCoef_=preExpDiffusionValue_*std::exp(-migrationEnthalpy_/(R*T) );
}



//Compute and set the current equilibrium (Xlaceq) concentration for the current temperature 
void  //TODO
Vacancy::ComputeEquilibriumConcentration()
{
  /*  formula  */
   
      std::cout<<"Vacancy::ComputeEquilibriumConcentration() ==> Computing Equilibrium concentration"<<std::endl;
  equilibriumConc_=this->ReturnEquilibriumConcentration();
  
  /*DEBUG */std::cout<<"Equilibrium concentration for vacancy is"<<equilibriumConc_<<"\n";
  std::cout<<"##########  END Vacancy::ComputeEquilibriumConcentration() ==> Computing Equilibrium concentration"<<std::endl; 
  

}



//TODO DELETE AFTER 
//Compute and set the current equilibrium (Xlaceq) concentration for the current temperature 
// void  
// Vacancy::ComputeEquilibriumConcentration()
// {
//   /*  formula  */
//   
//   std::cout<<"Vacancy::ComputeEquilibriumConcentration() ==> Computing Equilibrium concentration"<<std::endl;
//   
//   double R=ThermoDynamicsConstant::GetR();
//   double T=material_.GetTemperature().GetCurrentTemp();
//   
//  double sum_alpha_i=0;
//  for( std::vector<const ChemicalElement*>::const_iterator i = soluteInteractingWithVacList_.begin(); i != soluteInteractingWithVacList_.end(); ++i)
//  {
//    std::string elementName = (*i)->GetElementName();
//    double atomicConc=material_.ReturnAtomicConcFromVolumicForElement(elementName);
//    const bool usingEvac= (*i)->GetDiffusion().AssertInteractionEnergyWithVacancyValue(-2);//Evac is interaction with vacancy. Also remember that -2 means unused
//    double Evac=0;
//    
//    if (usingEvac==true)
//    {
//     Evac= (*i)->GetDiffusion().GetInteractionEnergyWithVacancy();
//    }
//    else
//    {
//     Evac=1;
//    };
//    sum_alpha_i += coordinationNumber_*atomicConc*( -1 + std::exp(Evac/(R*T)) );
//  }
//   
//    alpha
//   double alpha= 1 + sum_alpha_i;
//   equilibriumConc_=alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T));
//   assert ( (equilibriumConc_>0)&&"In ComputeEquilibriumConcentration: Computed value is not positive");
// 
//   
// }



//return alpha  where alpha is used to compute equilibrium conc. equilibriumConcValue=alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T));
const double 
Vacancy::ReturnAlpha(double Temperature) const
{
  double R=ThermoDynamicsConstant::GetR();
  double sum_alpha_i=0.;
  for( std::vector<const ChemicalElement*>::const_iterator i = soluteInteractingWithVacList_.begin(); i != soluteInteractingWithVacList_.end(); ++i)
  {
    std::string elementName = (*i)->GetElementName();
    double atomicConc=material_.ReturnAtomicConcFromVolumicForElement(elementName);
    const bool usingEvac= ! ((*i)->GetDiffusion().AssertInteractionEnergyWithVacancyValue(-2) );//Evac is interaction with vacancy. Also remember that -2 means unused. Do not change -2!!!
    double Evac=0.;
    
    /*DEBUG*/ std::cout<<" usingEvac "<<usingEvac<<std::endl;
    
    if (usingEvac==true)
    {
      Evac= (*i)->GetDiffusion().GetInteractionEnergyWithVacancy();
    }
    else
    {
      Evac=0. ;
      //TODO check what is correct between 1. and 0. .Isnt it 0. ? 0 to make sum_alpha_i=0 and then alpha=1
    };
    
    sum_alpha_i += coordinationNumber_*atomicConc*( -1. + std::exp(Evac/(R*Temperature)) );  // sum of i=1 to number of solute,  z_i*Cb_i*(-1 +exp(Q_i/(R*T))
      /*DEBUG*/ std::cout<<" TOPO TOPO atomicConc "<<atomicConc<<std::endl;
      /*DEBUG*/ std::cout<<" TOPO TOPO Evac "<<Evac<<std::endl;
      /*DEBUG*/ std::cout<<" TOPO TOPO coordinationNumber_ "<<coordinationNumber_<<std::endl;
  }
  /*DEBUG*/ std::cout<<" TOPA sum_alpha_i "<<sum_alpha_i<<std::endl;
  
    
    //TODO alpha
    double alpha= 1. + sum_alpha_i;
    
    return alpha;
  
}





const double  //TODO
Vacancy::ReturnEquilibriumConcentration() const
{  
  

  


/* old: 8/Sep/2016  here, here we assume that if the temperature is constant, the equlibrium concentration must be constant, 
 * thus equal to the previous one coming from the previous time step */   
//   int tempCurrentSize = material_.GetTemperature().GetTemperatureList().size();
//   if( (tempCurrentSize ==1 ) ||
//     (  (tempCurrentSize!=1)&&(material_.GetTemperature().GetTemperatureList()[tempCurrentSize-1] !=  material_.GetTemperature().GetCurrentTemp()) ) 
//   )
//   {
//   
//   
//   std::cout<<"################## Vacancy::ReturnEquilibriumConcentration() "<<std::endl;
//   double R=ThermoDynamicsConstant::GetR();
//   double T=material_.GetTemperature().GetCurrentTemp();
//   
//   const double alpha= this->ReturnAlpha(T);
//   
//   double equilibriumConcValue=alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T));
//   assert ( (equilibriumConcValue>0)&&"In ReturnEquilibriumConcentration: Returned value is not positive");
//   
//   /*DEBUG*/ std::cout<<" TOPO TOPO std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T)) "<<std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T))<<std::endl;
//   /*DEBUG*/ std::cout<<" TOPO TOPO vacCreationEntropy_   "<<vacCreationEntropy_<<"R "<<R<<"T "<<T<<" vacCreationEnthalpy_ "<<vacCreationEnthalpy_<<  std::endl;
//   std::cout<<"############ equilibriumConcValue = "<<equilibriumConcValue<<std::endl;
//   std::cout<<"################## END  Vacancy::ReturnEquilibriumConcentration() "<<std::endl;
// 
//   return equilibriumConcValue;
//   
//   }
//   else
//   {
//     std::cout<<"debug 200"<<" material_.GetTemperature().GetTemperatureList().size()  "<<material_.GetTemperature().GetTemperatureList().size()<<std::endl;
//     return equilibriumConc_;
//   }
    
    
    
    
    

  
  
  std::cout<<"################## Vacancy::ReturnEquilibriumConcentration() "<<std::endl;
  double R=ThermoDynamicsConstant::GetR();
  double T=material_.GetTemperature().GetCurrentTemp();
  
  const double alpha= this->ReturnAlpha(T);
  
  double equilibriumConcValue=alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T));
  assert ( (equilibriumConcValue>0)&&"In ReturnEquilibriumConcentration: Returned value is not positive");
  
  /*DEBUG*/ std::cout<<" TOPO TOPO std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T)) "<<std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T))<<std::endl;
  /*DEBUG*/ std::cout<<" TOPO TOPO vacCreationEntropy_   "<<vacCreationEntropy_<<"R "<<R<<"T "<<T<<" vacCreationEnthalpy_ "<<vacCreationEnthalpy_<<  std::endl;
  std::cout<<"############ equilibriumConcValue = "<<equilibriumConcValue<<std::endl;
  std::cout<<"################## END  Vacancy::ReturnEquilibriumConcentration() "<<std::endl;

  return equilibriumConcValue;  

}




double 
Vacancy::ReturnConcentrationBeforeQuenching()
{
  
  double R=ThermoDynamicsConstant::GetR();
  double Xlacavtrempe;
  
  /////////////////////////////////////////////////////////////////
  //assert if material current volumic conc is initial volumic conc
  /////////////////////////////////////////////////////////////////
  std::map<std::string , Concentration*>::iterator it;
  
  std::map<std::string, Concentration*> initialConcMap=material_.GetInitialChemicalComposition().GetConcentrationMap();
  
  std::map<std::string, Concentration*> currentConcMap=material_.GetCurrentChemicalComposition().GetConcentrationMap();
  
  for (it=initialConcMap.begin(); it!=initialConcMap.end(); ++it)
  {
    double initialVolumicConc=initialConcMap[it->first]->GetInitialVolumicValue();
    
    double currentVolumicValue= currentConcMap[it->first]->GetVolumicValue();
    
    assert (  (initialVolumicConc==currentVolumicValue)&&"Cannot return \
    concentration before quenching before at this moment, initial volumic concentration is not the current volumic concentration" );
  }
  // End assert if material current volumic conc is initial volumic conc
  //////////////////////////////////////////////////////////////////////
  
  
  
  const double alpha= this->ReturnAlpha(solutionisingTemp_);
  
  
  Xlacavtrempe = alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*solutionisingTemp_));
  
  assert(Xlacavtrempe>0);
  
  std::cout<<"Vacancy::ReturnConcentrationBeforeQuenching() vacCreationEntropy_ is"<<vacCreationEntropy_<<std::endl;
  std::cout<<"Vacancy::ReturnConcentrationBeforeQuenching() R is"<<R<<std::endl;
  std::cout<<"Vacancy::ReturnConcentrationBeforeQuenching() vacCreationEnthalpy_ is"<<vacCreationEnthalpy_<<std::endl;
  std::cout<<"Vacancy::ReturnConcentrationBeforeQuenching() solutionisingTemp_ is"<<solutionisingTemp_<<std::endl;
  std::cout<<"Vacancy::ReturnConcentrationBeforeQuenching() Xlacavtrempe is"<<Xlacavtrempe<<std::endl;
  
  return Xlacavtrempe;
}


  //Compute and save equilibrium concentration of vacancies after solutionising. Was Xlacavtremp.  To do: check validity of the equation
void 
Vacancy::ComputeConcentrationBeforeQuenching()
{
  std::cout<<"############# Vacancy::ComputeConcentrationBeforeQuenching()"<<std::endl;
  concentrationBeforeQuenching_=this->ReturnConcentrationBeforeQuenching();
}

double
Vacancy::ReturnCurrentConcentrationFromAnalyticalSolution(double duration, double initialEquilibriumConc) const
{
  assert( (duration >=0)&&"In ReturnCurrentConcentrationFromAnalyticalSolution(): Given duration in argument \
  is not strictly positive!!!");
  assert( (initialEquilibriumConc >=0)&&"In ReturnCurrentConcentrationFromAnalyticalSolution(): Given initial Equilibrium Concentration in argument \
  is not strictly positive!!!");
  
  assert((vacancyDiffusionCoef_!=0)&&"vacancyDiffusionCoef_ must be different from zero !!! Can not \
  compute time factor (Tau) in ReturnCurrentConcentrationFromAnalyticalSolution() .");
  
  double Tau=std::pow( halfSinkDistance_/2/M_PI ,2)/vacancyDiffusionCoef_;
  
  /*DEBUG*/ std::cout<<"Tau "<<Tau << std::endl;
  /*DEBUG*/ std::cout<<"halfSinkDistance_ "<<halfSinkDistance_ << std::endl;
  /*DEBUG*/ std::cout<<"vacancyDiffusionCoef_ "<<vacancyDiffusionCoef_ << std::endl;
  
  assert ( (equilibriumConc_!=0)&&"In ReturnCurrentConcentrationFromAnalyticalSolution: equilibriumConc_ must\
  be different from 0 . It has not been computed yet!");
  
  double Xlac=(initialEquilibriumConc-equilibriumConc_)*std::exp(-duration/Tau) + equilibriumConc_;
  
  /*DEBUG*/ std::cout<<"initialEquilibriumConc is "<<initialEquilibriumConc << std::endl;
  /*DEBUG*/ std::cout<<"equilibriumConc_ is "<<equilibriumConc_ << std::endl;
  /*DEBUG*/ std::cout<<"duration is "<<duration << std::endl;
  /*DEBUG*/ std::cout<<"Tau is "<<Tau << std::endl;
  /*DEBUG*/ std::cout<<"Xlac is "<<Xlac << std::endl;
  
  return Xlac;
}

void
Vacancy::ComputeCurrentConcentrationFromAnalyticalSolution(double duration, double initialEquilibriumConc)
{
  std::cout<<"Vacancy::ComputeCurrentConcentrationFromAnalyticalSolution() ==> Computing current concentration from analytical solution"<<std::endl;
  
  concentration_= this->ReturnCurrentConcentrationFromAnalyticalSolution(duration,initialEquilibriumConc);
  std::cout<<"END Vacancy::ComputeCurrentConcentrationFromAnalyticalSolution() ==> Computing current concentration from analytical solution"<<std::endl;
}


double 
Vacancy::GetConcentrationBeforeQuenching() const
{
  return concentrationBeforeQuenching_;
}


void
Vacancy::SaveQuenchingFinalConcentration(std::string fileName)
{
}

double
Vacancy::GetQuenchingFinalConcentration(std::string fileName)
{
  return 0;
}

double
Vacancy::GetHardeningFinalConcentration(std::string fileName)
{
  return 0;
}

double
Vacancy::SolveConcentrationEvolutionEquation()
{
  return 0;
}

void
Vacancy::Info() const
{
  std::cout <<  "################################### Vacancy::Info ###################################" 	<< std::endl;
  std::cout <<  "                        vacCreationEnthalpy: " << vacCreationEnthalpy_ << " SI unit" << std::endl;
  std::cout <<  "                         vacCreationEntropy: " << vacCreationEntropy_<< " SI unit" << std::endl;
  std::cout <<  "                              concentration: " << concentration_ << " SI unit" << std::endl;
  std::cout <<  "               concentrationBeforeQuenching: " << concentrationBeforeQuenching_ << " SI unit" << std::endl;
  std::cout <<  "                           halfSinkDistance: " << halfSinkDistance_ << " SI unit" << std::endl;
  std::cout <<  "                              jumpFrequency: " << jumpFrequency_ << " SI unit" << std::endl;
  //Info on Diffusion std::cout <<  "              magnesiumVacInteractionEnergy: " << magnesiumVacInteractionEnergy_ << " SI unit" << std::endl;
  std::cout <<  "                          migrationEnthalpy: " << migrationEnthalpy_ << " SI unit" << std::endl;
  std::cout <<  "                          solutionisingTemp: " << solutionisingTemp_ << " SI unit" << std::endl;
  //std::cout <<  "                 copperVacInteractionEnergy: " << copperVacInteractionEnergy_ << " SI unit" << std::endl;
  std::cout << std::endl;
}
