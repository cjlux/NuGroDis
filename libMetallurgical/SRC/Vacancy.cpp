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

Vacancy::Vacancy(double deltaHF,double deltaSF,double deltaHM,double fE,double Dlac0,double halfSinkD,double Tf, Material &mat,double coordNb )
  : vacCreationEnthalpy_(deltaHF), 
    vacCreationEntropy_(deltaSF), 
    concentration_(0.), 
    concentrationBeforeQuenching_(0), 
    halfSinkDistance_(halfSinkD), 
    jumpFrequency_(fE), 
    preExpDiffusionValue_(Dlac0),  
    migrationEnthalpy_(deltaHM), 
    solutionisingTemp_(Tf), 
    coordinationNumber_(coordNb),
    vacancyDiffusionCoef_(0),
    equilibriumConc_(0),
    initialConcentrationHasBeenDefined_(false),
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


//// WARNING if you change Vacancy::ReturnAlpha, remember to also change Vacancy::ReturnConcentrationBeforeQuenching()
//return alpha  where alpha is used to compute equilibrium conc. equilibriumConcValue=alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*T));
const double 
Vacancy::ReturnAlpha(double Temperature) const
{
  //// WARNING if you change Vacancy::ReturnAlpha, remember to also change Vacancy::ReturnConcentrationBeforeQuenching()
  
  double R=ThermoDynamicsConstant::GetR();
  double sum_alpha_i=0.;
  
  bool UsingInteractionWithSolute=false;
  
  
  for( std::vector<const ChemicalElement*>::const_iterator i = soluteInteractingWithVacList_.begin(); i != soluteInteractingWithVacList_.end(); ++i)
  {
    std::string elementName = (*i)->GetElementName();
    double atomicConc=material_.ReturnAtomicConcFromVolumicForElement(elementName);
    const bool usingEvac= ! ((*i)->GetDiffusion().AssertInteractionEnergyWithVacancyValue(-2) );//Evac is interaction with vacancy. Also remember that -2 means unused. Do not change -2!!!
    double Evac=0.;
    
    /*DEBUG*/ std::cout<<" usingEvac "<<usingEvac<<std::endl;
    
    if (usingEvac==true)
    {
      std::cout<<" Using interaction energy with vacancy"<<std::endl;
      Evac= (*i)->GetDiffusion().GetInteractionEnergyWithVacancy();
      UsingInteractionWithSolute=true;
      
      std::cout<<" Chemical Element <"<<elementName<<">"<<std::endl;
      std::cout<<"  Atomic concentration : "<<atomicConc<<std::endl;
      std::cout<<"  Vacancy interaction energy (Evac) : "<<Evac<<std::endl;
      std::cout<<"  Coordination Number : "<<coordinationNumber_<<std::endl;
    }
    else
    {
      std::cout<<" Not using interaction energy with vacancy"<<std::endl;
      Evac=0. ;
      //TODO check what is correct between 1. and 0. .Isnt it 0. ? 0 to make sum_alpha_i=0 and then alpha=1
    };
    
   
    sum_alpha_i += coordinationNumber_*atomicConc*( -1. + std::exp(Evac/(R*Temperature)) );  // sum of i=1 to number of solute,  z_i*Cb_i*(-1 +exp(Q_i/(R*T))

  }
  
  
 
  
  double alpha= 1. + sum_alpha_i;
  
  if (UsingInteractionWithSolute==true)
  {
    /*DEBUG*/ std::cout<<" sum_alpha_i "<<sum_alpha_i<<std::endl;
    /*DEBUG*/ std::cout<<" alpha "<<alpha<<std::endl;
  }
  else
  {
    assert((alpha==1.)&&"There is a problem here because if alpha you are not using interaction with solute!");
    
    std::cout<<"Not using interaction between solutes and vacancy"<<std::endl;   
  };
    
    
    
    
  //// WARNING if you change Vacancy::ReturnAlpha, remember to also change Vacancy::ReturnConcentrationBeforeQuenching()
    
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


//// WARNING if you change Vacancy::ReturnConcentrationBeforeQuenching() remember to also change  Vacancy::ReturnAlpha,
double 
Vacancy::ReturnConcentrationBeforeQuenching()
{
  
   //// WARNING if you change Vacancy::ReturnConcentrationBeforeQuenching() remember to also change  Vacancy::ReturnAlpha,
  
  double R=ThermoDynamicsConstant::GetR();
  double Xlacavtrempe;
  
  
  
  ///// compute alpha, for initial concentration values and T=solutionizing Temp  /////
  std::cout<<"In Vacancy::ReturnConcentrationBeforeQuenching()\n";
  double sum_alpha_i=0.;
  std::map<std::string, Concentration*> initialConcMap=material_.GetInitialChemicalComposition().GetConcentrationMap();
  
  bool UsingInteractionWithSolute=false;
  
  for( std::vector<const ChemicalElement*>::const_iterator i = soluteInteractingWithVacList_.begin(); i != soluteInteractingWithVacList_.end(); ++i)
  {
    std::string elementName = (*i)->GetElementName();
    double initialAtomicConc = initialConcMap[elementName]->GetInitialAtomicValue();
    const bool usingEvac= ! ((*i)->GetDiffusion().AssertInteractionEnergyWithVacancyValue(-2) );//Evac is interaction with vacancy. Also remember that -2 means unused. Do not change -2!!!
    double Evac=0.;
    /*DEBUG*/ std::cout<<" usingEvac "<<usingEvac<<std::endl;
    if (usingEvac==true)
    {
      std::cout<<" Using interaction energy with vacancy"<<std::endl;
      Evac= (*i)->GetDiffusion().GetInteractionEnergyWithVacancy();
      UsingInteractionWithSolute=true;
      
      std::cout<<" Chemical Element <"<<elementName<<">"<<std::endl;
      std::cout<<"  Atomic concentration : "<<initialAtomicConc<<std::endl;
      std::cout<<"  Vacancy interaction energy (Evac) : "<<Evac<<std::endl;
      std::cout<<"  Coordination Number : "<<coordinationNumber_<<std::endl;
    }
    else
    {
      std::cout<<" Not using interaction energy with vacancy"<<std::endl;
      Evac=0. ;
      //TODO check what is correct between 1. and 0. .Isnt it 0. ? 0 to make sum_alpha_i=0 and then alpha=1
    };
    
    sum_alpha_i += coordinationNumber_*initialAtomicConc*( -1. + std::exp(Evac/(R*solutionisingTemp_)) );  // sum of i= 1 to number of solute,  z_i*Cb_i*(-1 +exp(Q_i/(R*T))
    
  }
  
  
  const double alpha= 1. + sum_alpha_i;
  if (UsingInteractionWithSolute==true)
  {
    /*DEBUG*/ std::cout<<" sum_alpha_i "<<sum_alpha_i<<std::endl;
    /*DEBUG*/ std::cout<<" alpha "<<alpha<<std::endl;
  }
  else
  {
    assert((alpha==1.)&&"There is a problem here because if alpha you are not using interaction with solute!");
    
    std::cout<<"Not using interaction between solutes and vacancy"<<std::endl;   
  };
    
  
  
  ///////////////////////
  
  
  Xlacavtrempe = alpha*std::exp(vacCreationEntropy_/R - vacCreationEnthalpy_/(R*solutionisingTemp_));
  
  
  std::cout<<" vacCreationEntropy_ : "<<vacCreationEntropy_<<std::endl;
  std::cout<<" R : "<<R<<std::endl;
  std::cout<<" vacCreationEnthalpy_ : "<<vacCreationEnthalpy_<<std::endl;
  std::cout<<" solutionisingTemp_ : "<<solutionisingTemp_<<std::endl;
  std::cout<<"Value of 'Xlacavtrempe' computed : "<<Xlacavtrempe<<std::endl;
  
  
  assert(Xlacavtrempe>0);
  
  
  //// WARNING if you change Vacancy::ReturnConcentrationBeforeQuenching() remember to also change  Vacancy::ReturnAlpha,
  
  return Xlacavtrempe;
}


  //Compute and save equilibrium concentration of vacancies after solutionising. Was Xlacavtremp.  To do: check validity of the equation
void 
Vacancy::ComputeConcentrationBeforeQuenching()
{

  std::cout<< "==================================================="<<std::endl; 
  std::cout<< " Vacancy :Computing Concentration before quenching ";
  std::cout<< "==================================================="<<std::endl; 
  
  assert ((concentrationBeforeQuenching_==0)&&"Concentration before quenching has already been computed !!!");
  
  concentrationBeforeQuenching_=this->ReturnConcentrationBeforeQuenching();
  
  std::cout<< "==================================================="<<std::endl; 
  std::cout<< " Vacancy :Computing Concentration before quenching "<<std::endl;
  std::cout<< "==================================================="<<std::endl; 
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
  assert ((concentrationBeforeQuenching_>0)&&"Concentration before quenching has not been computed yet");
  return concentrationBeforeQuenching_;
}


void
Vacancy::SaveQuenchingFinalConcentration(std::string fileName)
{
  assert(!"not implemented yet.");
}

double
Vacancy::GetQuenchingFinalConcentration(std::string fileName)
{
  assert(!"not implemented yet.");
  return 0;
}

double
Vacancy::GetHardeningFinalConcentration(std::string fileName)
{
  assert(!"not implemented yet.");
  return 0;
}

double
Vacancy::SolveConcentrationEvolutionEquation()
{
  
  assert(!"Unuseful?  anyway, it is not implemented yet.");
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
