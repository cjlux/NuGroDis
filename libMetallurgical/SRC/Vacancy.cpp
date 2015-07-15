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
#include "Vacancy.hpp"
#include "ChemicalElement.hpp"
#include "Diffusion.hpp"
#include "ThermoDynamicsConstant.hpp"
#include "Material.hpp"
#include "Temperature.hpp"

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
  double R=ThermoDynamicsConstant::GetR();
  double T=material_.GetTemperature().GetCurrentTemp();
  vacancyDiffusionCoef_=preExpDiffusionValue_*std::exp(-migrationEnthalpy_/(R*T) );
}



//Compute and set the current equilibrium (Xlaceq) concentration for the current temperature 
void  //TODO
Vacancy::ComputeEquilibriumConcentration()
{
  /*  formula  */
  
  
  double R=ThermoDynamicsConstant::GetR();
  double T=material_.GetTemperature().GetCurrentTemp();
  
 double sum_alpha_i=0;
 for( std::vector<const ChemicalElement*>::const_iterator i = soluteInteractingWithVacList_.begin(); i != soluteInteractingWithVacList_.end(); ++i)
 {
   std::string elementName = (*i)->GetElementName();
   double atomicConc=material_.ReturnAtomicConcFromVolumicForElement(elementName);
   
   const bool usingEvac= (*i)->GetDiffusion().AssertInteractionEnergyWithVacancyValue(-2);//Evac is interaction with vacancy. Also remember that -2 means unused
   double Evac=0;
   
   if (usingEvac==true)
   {
    Evac= (*i)->GetDiffusion().GetInteractionEnergyWithVacancy();
   }
   else
   {
    Evac=1;
   };
   sum_alpha_i += coordinationNumber_*atomicConc*( -1 + std::exp(Evac/(R*T)) );
 }
  
  //TODO alpha
  double alpha= 1 + sum_alpha_i;
  
  
  equilibriumConc_=alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T));
  assert ( (equilibriumConc_>0)&&"In ComputeEquilibriumConcentration: Computed value is not positive");

  
}

const double  //TODO
Vacancy::ReturnEquilibriumConcentration() const
{  
  
  
  double R=ThermoDynamicsConstant::GetR();
  double T=material_.GetTemperature().GetCurrentTemp();
  
 double sum_alpha_i=0;
 for( std::vector<const ChemicalElement*>::const_iterator i = soluteInteractingWithVacList_.begin(); i != soluteInteractingWithVacList_.end(); ++i)
 {
   std::string elementName = (*i)->GetElementName();
   double atomicConc=material_.ReturnAtomicConcFromVolumicForElement(elementName);
   double Evac= (*i)->GetDiffusion().GetInteractionEnergyWithVacancy();
   sum_alpha_i += coordinationNumber_*atomicConc*( -1 + std::exp(Evac/(R*T)) );
 }
  
  //TODO alpha
  double alpha= 1 + sum_alpha_i;
  
  
  double equilibriumConcValue=alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T));
  assert ( (equilibriumConcValue>0)&&"In ReturnEquilibriumConcentration: Returned value is not positive");

  return equilibriumConcValue;
  
}





  //Compute and save equilibrium concentration of vacancies after solutionising. Was Xlacavtremp.  To do: check validity of the equation
void 
Vacancy::ComputeConcentrationBeforeQuenching()
{
  
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
  
  assert ( (equilibriumConc_!=0)&&"In ReturnCurrentConcentrationFromAnalyticalSolution: equilibriumConc_ must\
  be different from 0 . It has not been computed yet!");
  
  double Xlac=(initialEquilibriumConc-equilibriumConc_)*std::exp(-duration/Tau) + equilibriumConc_;
  
  return Xlac;
}

void
Vacancy::ComputeCurrentConcentrationFromAnalyticalSolution(double duration, double initialEquilibriumConc)
{
  concentration_= this->ReturnCurrentConcentrationFromAnalyticalSolution(duration,initialEquilibriumConc);
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
