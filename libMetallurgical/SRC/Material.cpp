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
#include "SSGrain.hpp"
#include "Grain.hpp"

Material::Material(Temperature& temp, 
		   ChemicalElement& mainChemicalElem,
		   ChemicalComposition& InitialCompo)
  : temperature_(temp), 
    vacancyList_(), 
    grainList_(),
    mainChemicalElement_(mainChemicalElem),
    InitialChemicalComposition_(InitialCompo),
    soluteList_(),
    ssgrain_(0),
    precipitatesList_(),
    YoungModulus_(mainChemicalElem.GetYoungModulus()),
    PoissonCoeff_(mainChemicalElem.GetPoissonCoefficient()),
    latticeParameter_(mainChemicalElem.GetLatticeParameter())
{
  
  
  std::cout<<"Constructor of Material"<<std::endl;
  
  
  
  /*
  std::cout<<"		>>Building a C++ object Material"<<std::endl;
  
  
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
  assert( (this->GetSSGrain()==0) && ("Cannot InitializeGrains(): The material has already solid solution grains! Material ssgrain has already been initialize") );

  // make the SSGrain which has the same properties as the material: ChemicalCompo and mechanical properties
  SSGrain *ssgrain= new SSGrain(*this);
  this->SetSSGrain(*ssgrain);
  
}

//RELATIONS
void
Material::AddVacancy(const Vacancy& vacancy)
{
  
}

void
Material::AddGrain(Grain& grain)
{//TODO : check if grain type is ssgrain. If true, then check if material has already ssgrain or not before pushing in List
  grainList_.push_back(&grain);
}


void 
Material::AddSolute(const ChemicalElement& Solute)
{
  soluteList_.push_back(&Solute);
}

void //set ssgrain_ and add ssgrain to grainList_
Material::SetSSGrain(SSGrain& ssgrain)
{
  
  assert( (this->GetSSGrain()==0) && ("Cannot SetSSGrain(), The material has already solid solution grains! Material ssgrain has already been initialize") );

  this->AddGrain(ssgrain);
  ssgrain_=&ssgrain;
  
  // later... 
}

void
Material::AddPrecipitate(Precipitate& P)
{ //TODO 
  /*Any precipitate which belongs to the material's SSgrain belongs to the
  * material. But all precipitates which belongs to the material does not belong to the material's SSGrain*/
  
  precipitatesList_.push_back(&P);
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
  std::cout <<  "                               temperature: -------------------- BEGIN temperature ---------------------------------"<< std::endl;
  temperature_.Info(); 
  std::cout <<"                        -------------------------------------------- END temperature ---------------------------------"<<std::endl;

  //std::cout <<  "                               vacancyList: " << vacancyList_.Info();
  std::cout <<  "                                   ssgrain: " ;
  if  (ssgrain_!=0) 
  {
    ssgrain_->Info();
  }
  else
  {
    std::cout<<"The material does not have any solid solution grain"<<std::endl ;
    
  };
  
 
  std::cout <<  "                       mainChemicalElement: -------------------- BEGIN mainChemicalElement ---------------------------------"<<std::endl;
  mainChemicalElement_.Info();
  std::cout <<"                      ---------------------------------------------- END mainChemicalElement ---------------------------------"<<std::endl;
  //std::cout <<  "                InitialChemicalComposition: " << InitialChemicalComposition_ << " SI unit" << std::endl;
  //std::cout <<  "                          precipitatesList: " << precipitatesList_ << " SI unit" << std::endl;
  std::cout <<  "                              YoungModulus: " << YoungModulus_ << " SI unit" << std::endl;
  std::cout <<  "                              PoissonCoeff: " << PoissonCoeff_ << " SI unit" << std::endl;
  std::cout <<  "                          LatticeParameter: " << latticeParameter_ << " SI unit" << std::endl;
  std::cout <<  "============================= END Material::Info =================================" 	<< std::endl;
  std::cout <<  std::endl;
}
