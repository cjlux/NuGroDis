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

#include "Material.hpp"
#include "Temperature.hpp"
#include "ChemicalElement.hpp"
#include "ChemicalComposition.hpp"
#include "SSGrain.hpp"
#include "Grain.hpp"
#include "Concentration.hpp"
#include "Precipitate.hpp"
#include "Vacancy.hpp"



//FAKE METHOD
void 
Material::test()
{
  std::cout<<"Size = "<<precipitateList_.size()<<std::endl;
  std::cout<<"Adresse of precipitate vector is "<<&precipitateList_<<std::endl;
  std::cout <<"Adresse of precipitate object are: ";
  for( std::vector<Precipitate*>::const_iterator i = precipitateList_.begin(); i != precipitateList_.end(); ++i)
  {std::cout << *i << '|';}
  std::cout <<""<<std::endl;
}


Material::Material(Temperature& temp, 
		   ChemicalElement& mainChemicalElem,
		   ChemicalComposition& InitialCompo)
  : temperature_(temp), 
    vacancy_(0), 
    grainList_(),
    mainChemicalElement_(mainChemicalElem),
    InitialChemicalComposition_(InitialCompo),
    currentChemicalComposition_(* (InitialChemicalComposition_.Clone()) ),
    /*TODO*///currentChemicalComposition_(),
    soluteList_(),
    ssgrainPointer_(0),
    precipitateList_(),
    YoungModulus_(mainChemicalElem.GetYoungModulus()),
    PoissonCoeff_(mainChemicalElem.GetPoissonCoefficient()),
    latticeParameter_(mainChemicalElem.GetLatticeParameter())
{
  
  
  
  
  
  std::cout<<" >Building a C++ object Material"<<std::endl;
  InitialCompo.SetMaterial(*this);
  
  /*
  //debug
  std::cout<<"		material Young modulus "<<YoungModulus_<<std::endl;
  std::cout<<"		material Poisson coef "<<PoissonCoeff_<<std::endl;
  std::cout<<"		material lattice param "<<latticeParameter_<<std::endl;
  

  
  // make the SSGrain:
  SSGrain *ssgrain= new SSGrain(*this);
  std::cout<<"Adress of ssgrain  Object ="<<ssgrain<<std::endl;
  std::cout<<"		SSgrain Young modulus (from YoungModulus_) = "<<ssgrain->GetYoungModulus()<<std::endl;
  
  //debug
  ssgrain->Info();
  
  //debug
  std::cout<<"		SSgrain Young modulus (from YoungModulus_) = "<<ssgrain->GetYoungModulus()<<std::endl;
  
  //debug
  std::cout<<"		material Young modulus "<<YoungModulus_<<std::endl;
  std::cout<<"		material Poisson coef "<<PoissonCoeff_<<std::endl;
  std::cout<<"		material lattice param "<<latticeParameter_<<std::endl;
  std::cout<<"		ssgrain->Info() ";
  //debug
  ssgrain->Info();
  std::cout<<"		SSgrain Young modulus (from YoungModulus_) = "<<ssgrain->GetYoungModulus()<<std::endl;

  
  //Debug
  std::cout<<" 		>>Poisson coeff of material's SSGrain ="<<ssgrain->GetPoissonCoeff()<<std::endl;
  
  ssgrainList_.push_back(ssgrain);
  
  //debug
  std::cout<<" 		>>Poisson coeff of material's this->GetSSGrainList()[0] ="<<this->GetSSGrainList()[0]->GetPoissonCoeff()<<std::endl;
  std::cout<<" 		>>Poisson coeff of material's SSGrainList_[0] ="<<ssgrainList_[0]->GetPoissonCoeff()<<std::endl;
  
  */
  std::cout<<"Exit constructor of Material"<<std::endl;

  
  
} 

Material::~Material()
{
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
    //SET INITIAL ATOMIC VALUE!!!
    assert ( (it->second->GetInitialAtomicValueHasBeenSet()==false)&&"Material Cannot Convert Volumic To AtomicConcentration\
    because InitialAtomicValue has already been set." );
    it->second->SetInitialAtomicValue(atomicConc);
    std::cout<<"===================== VALUE COMPUTED FOR INITIAL ATOMIC CONC OF ELEMENT <"<<it->first<<"> MATERIAL GRAIN IS ============= "<<it->second->GetInitialAtomicValue()<<std::endl;
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
  
  
    double volumicConc=0;
    double elementRho=0;
    double elementMolarMass=0;
    double sum=0;
    for (it2=currentConcMap.begin(); it2!=currentConcMap.end(); ++it2)
    {
      volumicConc=it2->second->GetVolumicValue();
      elementRho=it2->second->GetChemicalElement().GetDensity();
      elementMolarMass=it2->second->GetChemicalElement().GetMolarMass();
      sum+=volumicConc*elementRho*1000/elementMolarMass;
      
    }
    
    volumicConc=it->second->GetVolumicValue();
    elementRho=it->second->GetChemicalElement().GetDensity();
    elementMolarMass=it->second->GetChemicalElement().GetMolarMass();
    
    
    
    
    
    double computedAtomicConc= (volumicConc*elementRho*1000/elementMolarMass)/sum;
   
    std::cout<<"===================== VALUE RETURNED FOR ATOMIC CONC OF ELEMENT <"<<it->first<<"> IN MATERIAL IS ============= "<<it->second->GetInitialAtomicValue()<<std::endl;
  
  
  //Post conditions
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
    double volumicConc=0;
    double elementRho=0;
    double sum=0;
    for (it2=concMap.begin(); it2!=concMap.end(); ++it2)
    {
      volumicConc=it2->second->GetVolumicValue();
      elementRho=it2->second->GetChemicalElement().GetDensity();
      sum+=volumicConc*elementRho;
      
    }
    
    volumicConc=it->second->GetVolumicValue();
    elementRho=it->second->GetChemicalElement().GetDensity();
    
    
    
    
    
    double massicConc= (volumicConc*elementRho)/sum;
    //SET INITIAL MASSIC VALUE!!!
    assert ( (it->second->GetInitialMassicValueHasBeenSet()==false)&&"Material Cannot Convert Volumic To MassicConcentration\
    because InitialMassicValue has already been set." );
    it->second->SetInitialMassicValue(massicConc);
    std::cout<<"===================== VALUE COMPUTED FOR MASSIC CONC OF ELEMENT <"<<it->first<<"> SOLID SOLUTION GRAIN IS ============= "<<it->second->GetInitialMassicValue()<<std::endl;
  }
  
}

//Actualize the volumic value taking into account the volumic fraction of precipitates
void 
Material::UpdateVolumicValues()
{
  
  std::cout<<"Updating material current volumic concentration values\
  taking into account the volumic fraction of precipitates"<<std::endl;
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
	std::map<std::string, Concentration*>::iterator iter;
	double precipitateFracVol=(*i)->GetVolumicFraction();
	double elementVolumicConcInPrecipitate=(*i)->GetChemicalComposition().GetConcentrationForElement(it->first).GetVolumicValue();
	product+= precipitateFracVol*elementVolumicConcInPrecipitate;
	sumOfFracVol+=precipitateFracVol;
      }
      
    double elementInitialVolumicConcInMaterial=it->second->GetVolumicValue();
    double currentVolumicConc= (elementInitialVolumicConcInMaterial-product)/(1-sumOfFracVol);
    
    
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
Material::SetTemperature(const Temperature &temperatureClass)
{
  //Unusefull? later
  assert(0);
}


void
Material::Info() const
{
  std::cout <<  "################################# Material::Info #################################" 	<< std::endl;
  std::cout <<  "                               temperature: -------------------- BEGIN Material's temperature Info ---------------------------------"<< std::endl;
  temperature_.Info(); 
  std::cout <<"                        -------------------------------------------- END Material's temperature Info ---------------------------------"<<std::endl;

  if (vacancy_!=0)
  {
  std::cout <<  "                                   vacancy: -------------------- BEGIN Material's Vacancy Info --------------------------------"<< std::endl;
  vacancy_->Info();
  std::cout <<"                        -------------------------------------------- END material's Vacancy Info ---------------------------------"<<std::endl;
  }
  else
  {
    std::cout<<"The material does not have any Vacancy yet"<<std::endl ;
  };
  
  std::cout <<  "                                   ssgrain: " ;
  if  (ssgrainPointer_!=0) 
  {std::cout<<"Adress of the ssgrain is "<<ssgrainPointer_<<std::endl;
    ssgrainPointer_->Info();
  }
  else
  {
    std::cout<<"The material does not have any solid solution grain"<<std::endl ;
    
  };
  
 
  std::cout <<  "                       mainChemicalElement: -------------------- BEGIN Material's mainChemicalElement Info ---------------------------------"<<std::endl;
  mainChemicalElement_.Info();
  std::cout <<"                      ---------------------------------------------- END Material's mainChemicalElement Info ---------------------------------"<<std::endl;
  //std::cout <<  "                InitialChemicalComposition: " << InitialChemicalComposition_ << " SI unit" << std::endl;
   // currentChemicalComposition_
  //std::cout <<  "                          precipitateList: " << precipitateList_ << " SI unit" << std::endl;
  std::cout <<  "                              YoungModulus: " << YoungModulus_ << " SI unit" << std::endl;
  std::cout <<  "                              PoissonCoeff: " << PoissonCoeff_ << " SI unit" << std::endl;
  std::cout <<  "                          LatticeParameter: " << latticeParameter_ << " SI unit" << std::endl;
  std::cout <<  "============================= END Material::Info =================================" 	<< std::endl;
  std::cout <<  std::endl;
}
