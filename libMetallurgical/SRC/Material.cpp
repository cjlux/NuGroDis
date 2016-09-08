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

#include <cassert>
#include <iostream>

#include <Python.h>
#include "Material.hpp"
#include "Temperature.hpp"
#include "ChemicalElement.hpp"
#include "ChemicalComposition.hpp"
#include "SSGrain.hpp"
#include "Grain.hpp"
#include "Concentration.hpp"
#include "Precipitate.hpp"
#include "Vacancy.hpp"
#include "Diffusion.hpp"
#include "Computation.hpp"
#include "RadiusDistribution.hpp"


//FAKE METHOD
void 
Material::test()
{
  std::cout << "Size = " << precipitateList_.size() << std::endl;
  std::cout << "Adresse of precipitate vector is " << &precipitateList_ << std::endl;
  std::cout << "Adresse of precipitate object are: ";
  for( std::vector<Precipitate*>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {std::cout << *i << '|';}
  std::cout << "" << std::endl;
}


Material::Material(Temperature& temp, 
		   ChemicalElement& mainChemicalElem,
		   ChemicalComposition& InitialCompo, Computation& computation)
  : temperature_(temp), 
    vacancy_(0), 
    grainList_(),
    mainChemicalElement_(mainChemicalElem),
    InitialChemicalComposition_(InitialCompo),
    currentChemicalComposition_(* (InitialChemicalComposition_.Clone()) ),
    /*TODO*///currentChemicalComposition_(),
    soluteList_(),
    computation_(computation),
    ssgrainPointer_(0),
    precipitateList_(),
    YoungModulus_(mainChemicalElem.GetYoungModulus()),
    PoissonCoeff_(mainChemicalElem.GetPoissonCoefficient()),
    latticeParameter_(mainChemicalElem.GetLatticeParameter())
{
  temperature_.SetMaterial(*this);
  computation_.SetMaterial(*this);
  
  std::cout << " >Building a C++ object Material" << std::endl;
  InitialCompo.SetMaterial(*this);
  
  /*
  //debug
  std::cout << "		material Young modulus " << YoungModulus_ << std::endl;
  std::cout << "		material Poisson coef " << PoissonCoeff_ << std::endl;
  std::cout << "		material lattice param " << latticeParameter_ << std::endl;
  

    
  // make the SSGrain:
  SSGrain *ssgrain= new SSGrain(*this);
  std::cout << "Adress of ssgrain  Object =" << ssgrain << std::endl;
  std::cout << "		SSgrain Young modulus (from YoungModulus_) = " << ssgrain->GetYoungModulus() << std::endl;
  
  //debug
  ssgrain->Info();
  
  //debug
  std::cout << "		SSgrain Young modulus (from YoungModulus_) = " << ssgrain->GetYoungModulus() << std::endl;
  
  //debug
  std::cout << "		material Young modulus " << YoungModulus_ << std::endl;
  std::cout << "		material Poisson coef " << PoissonCoeff_ << std::endl;
  std::cout << "		material lattice param " << latticeParameter_ << std::endl;
  std::cout << "		ssgrain->Info() ";
  //debug
  ssgrain->Info();
  std::cout << "		SSgrain Young modulus (from YoungModulus_) = " << ssgrain->GetYoungModulus() << std::endl;

  
  //Debug
  std::cout << " 		>>Poisson coeff of material's SSGrain =" << ssgrain->GetPoissonCoeff() << std::endl;
  
  ssgrainList_.push_back(ssgrain);
  
  //debug
  std::cout << " 		>>Poisson coeff of material's this->GetSSGrainList()[0] =" << this->GetSSGrainList()[0]->GetPoissonCoeff() << std::endl;
  std::cout << " 		>>Poisson coeff of material's SSGrainList_[0] =" << ssgrainList_[0]->GetPoissonCoeff() << std::endl;
  
  */
  std::cout << "Exit constructor of Material" << std::endl;
} 

Material::~Material()
{
}

bool 
Material::CheckIfTheVolumicConcentrationsArePositive()
{
  
  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string , Concentration*> currentConcMap=currentChemicalComposition_.GetConcentrationMap();
  bool isAVolumicConcPositive=true;
    
    for (it=currentConcMap.begin(); it!=currentConcMap.end(); ++it)
    {
      double volumicConc=it->second->GetVolumicValue();
      
      if (volumicConc<=0)
      {
	isAVolumicConcPositive=false;
      }
      
    }
  return isAVolumicConcPositive;
}

void // in TEST run beginning of "boucle temporelle"
Material::RunProcess()
{
#ifdef DEBUG
  std::string rep;
  std::cin >> rep;
#endif
  std::cout << "Material::RunProcess() ===>  Running Process " << std::endl;
  double currentTime, computationDuration;
  
  currentTime=computation_.GetCurrentTime();
  computationDuration=computation_.GetMaxComputationTime();
  std::cout << "Computation Duration is " << computationDuration << "\n" << std::endl;
  
  this->UpdateVolumicValues();//in this case, UpdateVolumicValues() Process the initial volumic concentration
  
  
  
  while ( (currentTime<computationDuration)&&(this->CheckIfTheVolumicConcentrationsArePositive()==true) )
  {//Begin While
    std::cout << "Current time is " << currentTime << "\n\n\n\n\n" << std::endl;
    //assert (computation_->GetCurrentTime==0)
    
    this->UpdateVolumicValues();
    
    
    //Update SurfeceEnergy (gamma) (at new temperature)
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->ComputeSurfaceEnergy();// Update gamma
    }
    
    
    this->UpdateAtomicDiffusionCoef();// Important! : Update of AtomicDiffusion coef
    
    this->SaveMaterialVacancyProperties();
    
    this->ProcessPrecipitatesNucleationRate(); 
    ///*DEBUG*/assert (!("Tpooet"));
    

    this->ComputePrecipitatesAllInterfacialConcentrations();
    
    this->ComputePrecipitatesInterfacialVelocityList();
      
    this->ComputeCriticalInterfacialConcentration();
      
    this->ProcessComputationMaxTimeStep();
    
    
    this->AddNucleatedPrecipitates();
    
    this->SolveCineticLinearSytem();
    
    
    //Cut of negligle value of RadiusHistogram(<1)
    double cutOffvalue;
    cutOffvalue=1.;
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->ResetCurrRadDisItemsIfValueIsLowerThan(cutOffvalue);
    }
    
    //Cout RadiusDis
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->GetCurrentRadiusDistribution().CoutRadiusDistribution();
    }
    
    /*
    //plot histogram
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->GetCurrentRadiusDistribution().PlotPythonHistogram();
    }*/
    
    
    this->ComputePrecipitatesVolumicFraction();
    
    
    this->UpdateVolumicValues(); //Important to use method UpdateVolumicValues() here.
    
    this->ComputePrecipitatesNucleationSiteNb();
    
    
    
    this->UpdateComputationCurrentTime();
    currentTime=computation_.GetCurrentTime();
    if (currentTime>= computationDuration)
    {
      std::cout << "Current time is equal or superior to Computation Duration. Current time just after update is " << currentTime << "\n\n\n\n\n" << std::endl;
    }
    
    //Update all properties that are dependent to temperature
    
    //TODO Update current Temperature
    /*Just for debug*/this->GetTemperature().SetCurrentTemp(293.15);
    
    
    
    
    
    //ajout d'une nouvelle classe dans la radDis si la derniere classe est vide
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->AddEmptyClassForCurrentRadiusDistributionWithCondition();
    }
    
    
    /*
    this->UpdateVolumicValues();//Not needed?
    vacancy_->ComputeDiffusionCoefValue();
    vacancy_->ComputeEquilibriumConcentration();
    vacancy_->ComputeCurrentConcentrationFromAnalyticalSolution(computation_.GetCurrentTime(), vacancy_->GetConcentrationBeforeQuenching()); 
    
    for( std::vector<const ChemicalElement*>::const_iterator i = soluteList_.begin(); i != soluteList_.end(); ++i)
    {
      (*i)->ComputeAtomicDiffusionCoefValue();
    }
    
    this->ProcessPrecipitatesNucleationRate();
    
    this->AddNucleatedPrecipitates();
    
    this->SolveCineticLinearSytem();
    
    //Cut of negligle value of RadiusHistogram(<1)
    double cutOffvalue;
    cutOffvalue=1.;
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->ResetCurrRadDisItemsIfValueIsLowerThan(cutOffvalue);
    }
    
    //Cout RadiusDis
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->GetCurrentRadiusDistribution().CoutRadiusDistribution();
    }
    
    this->ComputePrecipitatesVolumicFraction();
    
    this->UpdateVolumicValues();
    
    this->ComputePrecipitatesNucleationSiteNb();
    
    this->UpdateComputationCurrentTime();*/
  
    
    
    
    
    //Save RadiusDistribution of all precipitates
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->GetCurrentRadiusDistribution().SaveDistribution();
    }
    
    //Save Attributes values of all precipitates
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->SavePrecipitateAttributes();
      (*i)->SavePrecipitateInterfacialConcentrations();
      (*i)->SavePrecipitateInterfacialVelocities();
    }
    
    this->SaveMaterialCurrentChemicalCompo();
    
    
    
  //End WHILE
  }
  
  
    /*TODO uncomment if you want to plot the last distribution
    //plot histogram
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
    {
      (*i)->GetCurrentRadiusDistribution().PlotPythonHistogram();
    }
    */
    

   
   
  std::cout << "\n\n\n\n" << std::endl;
  std::cout << "++--++--++--++ ################# END OF Material::RunProcess()" << std::endl;
  std::cout << std::endl;
}




void Material::UpdateAtomicDiffusionCoef()
{
  vacancy_->ComputeDiffusionCoefValue();
  vacancy_->ComputeEquilibriumConcentration();
  if (computation_.GetCurrentTime()==0)
  {
    std::cout << "Computing Concentration before quenching\n" << std::endl;
    vacancy_->ComputeConcentrationBeforeQuenching();
  }
  
  std::cout << "MaterialGetConcentrationBeforeQuenching()  " << vacancy_->GetConcentrationBeforeQuenching() << std::endl;
  vacancy_->ComputeCurrentConcentrationFromAnalyticalSolution(computation_.GetCurrentTime(), vacancy_->GetConcentrationBeforeQuenching());
  
  for( std::vector<const ChemicalElement*>::const_iterator i = soluteList_.begin(); i != soluteList_.end(); ++i)
  {
    (*i)->ComputeAtomicDiffusionCoefValue();
  }
}

//in TEST 
void
Material::ProcessPrecipitatesNucleationRate()
{
  std::cout << "Material::ProcessPrecipitatesNucleationRate() : process Nucleation rate from C++, and for material" << std::endl;
  for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->ProcessNucleationRate();
  }
}

void
Material::ComputePrecipitatesAllInterfacialConcentrations()
{
  std::cout << "Material::ComputePrecipitatesAllInterfacialConcentrations() : Compute precipitates All interfacial concentration from C++, and for material" << std::endl;
  for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->ComputeAllInterfacialConcentrations();
  }
}

void
Material::ComputePrecipitatesInterfacialVelocityList()
{
  std::cout << "Material::ComputePrecipitatesInterfacialVelocityList() : Compute precipitates All interfacial velocities ( interfacialVelocityList) " << std::endl;
  
  for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->GetCurrentRadiusDistribution().ComputeInterfacialVelocityList();
  }
  
  std::cout << "+-+-+-+-   END: Running Material::ComputePrecipitatesInterfacialVelocityList() +-+-+-+-" << std::endl;
}


void
Material::ComputeCriticalInterfacialConcentration()
{
  std::cout << "Material::ComputeCriticalInterfacialConcentration() : Compute precipitates critical interfacial concentration  (XvIntcritique for each solute \
  element of the precipitate) " << std::endl;
  
  for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->GetCurrentRadiusDistribution().ComputeCriticalInterfacialConcentration();
  }
  
  std::cout << "+-+-+-+-   END: Running Material::ComputeCriticalInterfacialConcentration() +-+-+-+-" << std::endl;
  
}


void
Material::ProcessComputationMaxTimeStep()
{
  std::cout << "Material::ProcessComputationMaxTimeStep() : compute the maximum time step allowed for the Computation" << std::endl; 
  computation_.ComputeMaxTimeStep();
  std::cout << "+-+-+-+-   END: Running Material::ProcessComputationMaxTimeStep() +-+-+-+-" << std::endl;
}


void
Material::AddNucleatedPrecipitates()
{
  std::cout << "Material::AddNucleatedPrecipitates() : Adding nucleated Precipites For all precipiates in the material " << std::endl; 
  
  
  for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->AddNucleatedPrecipitates();
  }
  
  std::cout << "+-+-+-+-   END: Running Material::AddNucleatedPrecipitates() +-+-+-+-" << std::endl;
  
}


void
Material::SolveCineticLinearSytem()
{
  std::cout << "Material::SolveCineticLinearSytem() : Solving cinetic liear system For all precipiates in the material " << std::endl; 
  
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->SolveCineticLinearSytem();
  }
  
  std::cout << "+-+-+-+-   END: Running Material::SolveCineticLinearSytem() +-+-+-+-" << std::endl;
  
}

void
Material::ComputePrecipitatesVolumicFraction()
{
  std::cout << "Material::ComputePrecipitatesVolumicFraction() : Computing volumic fraction for all precipitates in the material " << std::endl; 
  
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->ComputeVolumicFraction();
  }
  
  std::cout << "+-+-+-+-   END: Running Material::ComputePrecipitatesVolumicFraction() +-+-+-+-" << std::endl;
  
}

void
Material::ComputePrecipitatesNucleationSiteNb()
{
  std::cout << "Material::ComputePrecipitatesNucleationSiteNb() : Computing nucleation sites number for all precipitates in the material " << std::endl; 
  
    for (std::vector<Precipitate *>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {
    (*i)->ComputeNucleationSiteNb();
  }
  
  std::cout << "+-+-+-+-   END: Running Material::ComputePrecipitatesNucleationSiteNb() +-+-+-+-" << std::endl;
  
}

void
Material::UpdateComputationCurrentTime()
{
  std::cout << "Material::UpdateTimeStep() : Updating the current time step of the Computation" << std::endl; 
  computation_.UpdateCurrentTime();
  std::cout << "+-+-+-+-   END: Running Material::UpdateTimeStep() +-+-+-+-" << std::endl;
}

void
Material::ReadData(std::string dataFileName)
{
}

void  
Material::InitializeGrains()  //Initialize material SSgrains AND/OR precipitates
{
  assert( (ssgrainPointer_==0) && ("Cannot InitializeGrains(): The material has already solid solution grains! Material ssgrain has already been initialize") );

  // make the SSGrain which has the same properties as the material: ChemicalCompo and mechanical properties
  new SSGrain(*this);//Remember, creating an SSgrain by constructor SSgrain(materialObject) also set material's ssgrainPointer
  
}


void
Material::ConvertVolumicToInitialAtomicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
  
  
  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string , Concentration*>::iterator it2;
  std::map<std::string, Concentration*> concMap=InitialChemicalComposition_.GetConcentrationMap();
  
    
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {
    double initialVolumicConc=0;
    double elementRho=0;
    double elementMolarMass=0;
    double sum=0;
    for (it2=concMap.begin(); it2!=concMap.end(); ++it2)
    {
      initialVolumicConc=it2->second->GetInitialVolumicValue();
      elementRho=it2->second->GetChemicalElement().GetDensity();
      elementMolarMass=it2->second->GetChemicalElement().GetMolarMass();
      sum+=initialVolumicConc*elementRho*1000/elementMolarMass;
      
      /*debug begin*/
      std::cout << "Rho" << it2->first << "  = " << elementRho << "\n";
      std::cout << "Mm" << it2->first << "  = " << elementMolarMass << "\n";
      std::cout << "Xv0" << it2->first << "SS  = " << initialVolumicConc << "\n";
      std::cout << "monome (Xv0SS*Rho*1000/Mm) when element i is " << it2->first <<  initialVolumicConc*elementRho*1000/elementMolarMass  << "\n";
      std::cout << " Sum cumulé proressive " << sum << "\n";
      /*debug end*/
      
    }
    
    initialVolumicConc=it->second->GetInitialVolumicValue();
    elementRho=it->second->GetChemicalElement().GetDensity();
    elementMolarMass=it->second->GetChemicalElement().GetMolarMass();
    
    
    
    /*debug begin*/
    std::cout << "debug: numerateur = " << elementRho*1000/elementMolarMass << "\n";
    std::cout << "debug: coef for converting volumicSS to atomicSS for element  <" << it->first << "> is --->" << elementRho*1000/elementMolarMass/sum << "\n";
    /*debug end*/
    
    double atomicConc= (initialVolumicConc*elementRho*1000/elementMolarMass)/sum;
    //SET INITIAL ATOMIC VALUE!!!
    assert ( (it->second->GetInitialAtomicValueHasBeenSet()==false)&&"Material Cannot Convert Volumic To AtomicConcentration\
    because InitialAtomicValue has already been set." );
    it->second->SetInitialAtomicValue(atomicConc);
    std::cout << "===================== VALUE COMPUTED FOR INITIAL ATOMIC CONC OF ELEMENT <" << it->first << "> MATERIAL GRAIN IS ============= " << it->second->GetInitialAtomicValue() << std::endl;
  }
  
}

//Return current atomic concentration from volumic concentration
double 
Material::ReturnAtomicConcFromVolumicForElement(std::string elementName) const
{
  
   std::map<std::string , Concentration*>::iterator it;
  std::map<std::string , Concentration*>::iterator it2;
  std::map<std::string, Concentration*> currentConcMap=currentChemicalComposition_.GetConcentrationMap();
  
  it=currentConcMap.find(elementName);
  assert( (it!=currentConcMap.end())&&"Cannot find given symbol in concentrationMap of Material's current Chemical composition " );
  
  
    double volumicConc=0.;
    double elementRho=0.;
    double elementMolarMass=0.;
    double sum=0.;
    for (it2=currentConcMap.begin(); it2!=currentConcMap.end(); ++it2)
    {
      volumicConc=it2->second->GetVolumicValue();
      elementRho=it2->second->GetChemicalElement().GetDensity();
      elementMolarMass=it2->second->GetChemicalElement().GetMolarMass();
      sum+=volumicConc*elementRho*1000./elementMolarMass;
    }
    
    volumicConc=it->second->GetVolumicValue();
    elementRho=it->second->GetChemicalElement().GetDensity();
    elementMolarMass=it->second->GetChemicalElement().GetMolarMass();
    
    
    
    
    
    double computedAtomicConc= (volumicConc*elementRho*1000./elementMolarMass)/sum;
    
    
    
    
    
   
    std::cout << "===================== VALUE RETURNED FOR ATOMIC CONC OF ELEMENT <" << it->first << "> IN MATERIAL IS ============= " << it->second->GetInitialAtomicValue() << std::endl;
  
  
  //Post conditions
  /*DEBUG*/ std::cout << " computedAtomicConc is " << computedAtomicConc << "volumic conc is" << volumicConc << "Element name is " << elementName <<  "\n";
  assert( (computedAtomicConc>0)&&"In ReturnAtomicConcentrationFromVolumic: result of conversion must be positive" );
  assert( (computedAtomicConc<=1)&&"In ReturnAtomicConcentrationFromVolumic: result of conversion must inferior to 1" );
  
  return computedAtomicConc;
}



void
Material::ConvertVolumicToInitialMassicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
  
  
  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string , Concentration*>::iterator it2;
  std::map<std::string, Concentration*> concMap=InitialChemicalComposition_.GetConcentrationMap();
  
    
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {
    double initialVolumicConc=0;
    double elementRho=0;
    double sum=0;
    for (it2=concMap.begin(); it2!=concMap.end(); ++it2)
    {
      initialVolumicConc=it2->second->GetInitialVolumicValue();
      elementRho=it2->second->GetChemicalElement().GetDensity();
      sum+=initialVolumicConc*elementRho;
      
    }
    
    initialVolumicConc=it->second->GetInitialVolumicValue();
    elementRho=it->second->GetChemicalElement().GetDensity();
    
    
    
    
    
    double massicConc= (initialVolumicConc*elementRho)/sum;
    //SET INITIAL MASSIC VALUE!!!
    assert ( (it->second->GetInitialMassicValueHasBeenSet()==false)&&"Material Cannot Convert Volumic To MassicConcentration\
    because InitialMassicValue has already been set." );
    it->second->SetInitialMassicValue(massicConc);
    std::cout << "===================== VALUE COMPUTED FOR INITIAL MASSIC CONC OF ELEMENT <" << it->first << "> SOLID SOLUTION GRAIN IS ============= " << it->second->GetInitialMassicValue() << std::endl;
  }
  
}

//Actualize the volumic value taking into account the volumic fraction of precipitates
void 
Material::UpdateVolumicValues()
{
  // Remember : checkIfUpdatedValuesArePositive default value is true !!!
  
  std::cout << "Updating material current volumic concentration values\
  taking into account the volumic fraction of precipitates" << std::endl;
  // XvCuSS
  //TODO 
  std::map<std::string, Concentration*> currentConcMap=currentChemicalComposition_.GetConcentrationMap();
  std::map<std::string, Concentration*> initialConcMap=InitialChemicalComposition_.GetConcentrationMap();
  
  std::map<std::string, Concentration*>::iterator it;
  

    
    for (it=initialConcMap.begin(); it!=initialConcMap.end(); ++it)
    {

      
      double product=0;
      double sumOfFracVol=0;
      
      for( std::vector<Precipitate*>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
      {
	double oldVolumicFraction, VolumicFraction;
	oldVolumicFraction=(*i)->GetVolumicFraction();
	VolumicFraction=(*i)->ReturnVolumicFraction();
	
	assert ( (VolumicFraction==oldVolumicFraction)&&"Volumic fraction of precipitates must be computed before run method Material::UpdateVolumicValues()");
	
	double precipitateFracVol=(*i)->GetVolumicFraction(); //fracVolP
	double elementVolumicConcInPrecipitate=(*i)->GetChemicalComposition().GetConcentrationForElement(it->first).GetVolumicValue(); //Xv_i_P
	product += (precipitateFracVol*elementVolumicConcInPrecipitate);// Xv_i_P1 * fracVolP1 + Xv_i_P2 * fracVolP2 + ... + Xv_i_Pn * fracVolPn 
	sumOfFracVol += precipitateFracVol;  // fracVolP1 + fracVolP2 + ... + fracVolPn 
	
	//Debug
	std::cout << "TARATATA: Precipitate type is " << (*i)->GetPrecipitateType() << "\n";
	std::cout << "TARATATA1: Precipitate frac vol is " << precipitateFracVol << "\n" << " Xv" << it->first << "P : " << elementVolumicConcInPrecipitate << "\n" << " total Xv_i*Vf_i Product is" << product << "\n Sum of frac vol is :" << precipitateFracVol << "\n";
      }
      
    double elementInitialVolumicConcInMaterial=it->second->GetInitialVolumicValue(); //Xv0_i_SS
    
    //debug
    std::cout << "TARATATA2 Initial COncentration OIbd adress is " << it->second << "\n";
    std::cout << " Xv0" << it->first << "SS is:" << elementInitialVolumicConcInMaterial << "\n";
    
    assert(sumOfFracVol!= 1.);
    assert( ( sumOfFracVol < 1 ) && " Precipitates total volumic fraction must be less than 1" );
    double currentVolumicConc= (elementInitialVolumicConcInMaterial-product)/(1.-sumOfFracVol);
    
    //debug
    std::cout << "TARATATA3 Current volumic con is then: " << currentVolumicConc << "\n";
    
    
    currentConcMap[it->first]->SetVolumicValue(currentVolumicConc);
    
    }
    
 
}





//RELATIONS

void 
Material::SetVacancy(Vacancy& vacancy)
{
  assert ( (vacancy_==0)&&"In SetVacancy(): Cannot set another vacancy because material already has a Vacancy object!!! ");
  vacancy_=&vacancy;
  assert ( (vacancy_!=0)&&"In SetVacancy(): pointer can not be equal to 0 after setting vacancy object!!! ");
}


void
Material::AddGrain(Grain& grain)
{
  //TODO : check if grain type is ssgrain. If true, then check if material has already ssgrain or not before pushing in List
  grainList_.push_back(&grain);
}


void 
Material::AddSolute(const ChemicalElement& Solute)
{
  soluteList_.push_back(&Solute);
}

void //set ssgrainPointer_ and add ssgrain to grainList_
Material::SetSSGrain(SSGrain& ssgrain)
{
  
  assert( (ssgrainPointer_==0) && ("Cannot SetSSGrain(), The material has already solid solution grains! Material ssgrain has already been initialize") );

  this->AddGrain(ssgrain);
  ssgrainPointer_=&ssgrain;
  
  // later... 
}

void
Material::AddPrecipitate(Precipitate& P)
{ //TODO 
  /*Any precipitate which belongs to the material's SSgrain belongs to the
  * material. But all precipitates which belongs to the material does not belong to the material's SSGrain*/
  
  precipitateList_.push_back(&P);
 // later...
  
}

void
Material::SaveMaterialVacancyProperties()
{
  std::string ResultsDirectoryPath= computation_.GetResultsDirectory();
  
  std::string fileName= "MaterialVacancyProperties.txt";
  std::string path=ResultsDirectoryPath+"/MaterialCurrentCompo/"+fileName;
  std::ofstream output_file;
  output_file.open(path.c_str(), std::ios_base::app);
  
  //Check if file is empty
  std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
  
  std::stringstream line;
  double CurrentTime = computation_.GetCurrentTime();
  std::vector<std::string> lineStringVector;
  assert (lineStringVector.size()==0);
    
  
  if ( in.tellg() == 0 /*check if it is empty*/      )
  {
    // file is empty
    line << "time" << "\t" << "Dlac" << "\t" << "Xlac" << "\t" << "XlacEq" << "\t" << "LambdaBoostFactor" << "\t" << "halSinkDistance" << "\t";
    
    for( std::vector<const ChemicalElement*>::const_iterator i = soluteList_.begin(); i != soluteList_.end(); ++i)
    {
      line << "D_"+(*i)->GetElementName()+"SS" << "\t";
    }
    
    lineStringVector.push_back(line.str());
    assert (lineStringVector.size()==1);
  }
  
  double Dlac,Xlac, XlacEq, lambda, halfSinkD;
  
  halfSinkD= this->GetVacancy().GetHalfSinkDistance();
  XlacEq=this->GetVacancy().GetEquilibriumConcentration();
  Xlac=this->GetVacancy().GetConcentration();
  lambda= this->GetVacancy().ReturnBoostFactor();
  Dlac= this->GetVacancy().GetVacancyDiffusionCoef();
  
  std::stringstream lineStream;
  lineStream << CurrentTime << "\t";
  lineStream << Dlac << "\t" << Xlac << "\t" << XlacEq << "\t" << lambda << "\t" << halfSinkD << "\t";
  
  
  //Getting the atomic diffusion coef (D_i_SS)
  for( std::vector<const ChemicalElement*>::const_iterator i = soluteList_.begin(); i != soluteList_.end(); ++i)
  {
    lineStream <<  (*i)->GetDiffusion().GetAtomicDiffusionCoef() << "\t";
  }
  
  
  
  lineStringVector.push_back(lineStream.str());
  
  std::ostream_iterator<std::string> output_iterator(output_file, "\n");
  std::copy(lineStringVector.begin(), lineStringVector.end(), output_iterator);
  
}


void 
Material::SaveMaterialCurrentChemicalCompo()
{

  std::string ResultsDirectoryPath= computation_.GetResultsDirectory();
  
  std::string fileName= "MaterialChemicalCompo.txt";
  std::string path=ResultsDirectoryPath+"/MaterialCurrentCompo/"+fileName;
  std::ofstream output_file;
  output_file.open(path.c_str(), std::ios_base::app);
  
  //Check if file is empty
  std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
  
  std::stringstream line;
  double CurrentTime = computation_.GetCurrentTime();
  std::vector<std::string> lineStringVector;
  
  assert (lineStringVector.size()==0);
  
  std::map<std::string, Concentration*>::iterator it;
  
  std::vector<std::string> ElementsName;
  std::vector<double> ElementVolumicConc;
  std::map<std::string, Concentration*> concMap= currentChemicalComposition_.GetConcentrationMap();
  
  
  assert (ElementsName.size()==0);
  for (it=concMap.begin(); it!=concMap.end(); ++it)
    {
      ElementsName.push_back(it->first);
      ElementVolumicConc.push_back(it->second->GetVolumicValue());      
    }
  
  assert (ElementsName.size()==ElementVolumicConc.size());
  
  if ( in.tellg() == 0 /*check if it is empty*/      )
  {
    // file is empty
    line << "time" << "\t";
    for (unsigned int i=0; i<ElementsName.size(); ++i)
    {
      line << "Xv"+ElementsName[i] << "\t";
    } 
    
    lineStringVector.push_back(line.str());
    assert (lineStringVector.size()==1);
  }
    
  std::stringstream lineStream;
  lineStream << CurrentTime << "\t";
  for (unsigned int i=0; i<ElementsName.size(); ++i)
  {
    lineStream << ElementVolumicConc[i] << "\t";
  } 
  
  lineStringVector.push_back(lineStream.str());
  
  std::ostream_iterator<std::string> output_iterator(output_file, "\n");
  std::copy(lineStringVector.begin(), lineStringVector.end(), output_iterator);
  
}




void
Material::Info() const
{
  std::cout <<   "################################# Material::Info #################################" 	 <<  std::endl;
  std::cout <<   "                               temperature: -------------------- BEGIN Material's temperature Info ---------------------------------" <<  std::endl;
  temperature_.Info(); 
  std::cout << "                        -------------------------------------------- END Material's temperature Info ---------------------------------" << std::endl;

  if (vacancy_!=0)
  {
  std::cout <<   "                                   vacancy: -------------------- BEGIN Material's Vacancy Info --------------------------------" <<  std::endl;
  vacancy_->Info();
  std::cout << "                        -------------------------------------------- END material's Vacancy Info ---------------------------------" << std::endl;
  }
  else
  {
    std::cout << "The material does not have any Vacancy yet" << std::endl ;
  };
  
  std::cout <<   "                                   ssgrain: " ;
  if  (ssgrainPointer_!=0) 
  {std::cout << "Adress of the ssgrain is " << ssgrainPointer_ << std::endl;
    ssgrainPointer_->Info();
  }
  else
  {
    std::cout << "The material does not have any solid solution grain" << std::endl ;
    
  };
  
 
  std::cout <<   "                       mainChemicalElement: -------------------- BEGIN Material's mainChemicalElement Info ---------------------------------" << std::endl;
  mainChemicalElement_.Info();
  std::cout << "                      ---------------------------------------------- END Material's mainChemicalElement Info ---------------------------------" << std::endl;
  //std::cout <<   "                InitialChemicalComposition: " <<  InitialChemicalComposition_ <<  " SI unit" <<  std::endl;
   // currentChemicalComposition_
  //std::cout <<   "                          precipitateList: " <<  precipitateList_ << " SI unit" << std::endl;
  std::cout <<  "                              YoungModulus: " << YoungModulus_ << " SI unit" << std::endl;
  std::cout <<  "                              PoissonCoeff: " << PoissonCoeff_ << " SI unit" << std::endl;
  std::cout <<  "                          LatticeParameter: " << latticeParameter_ << " SI unit" << std::endl;
  std::cout <<  "============================= END Material::Info =================================" 	<< std::endl;
  std::cout <<  std::endl;
}
