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

#include "Diffusion.hpp"
#include "ChemicalElement.hpp" 
#include "Material.hpp"
#include "Vacancy.hpp"



//CONSTRUCTOR 
//vacancies diffusion and atomic diffusion
//can also be used in case material did not have a ssgrain! Chemical element diffuse in material or material's ssgrain(which can be =0), 
Diffusion::Diffusion(ChemicalElement& CE, Material& mat,Vacancy& vacancy,double preExpDiffCoef,double actEn, double Evac)
 :preExpDiffusionCoef_(preExpDiffCoef),
  activationEnergy_(actEn),
  interactionEnergyWithVacancy_(Evac),
  chemicalElement_(CE),
  material_(&mat),
  ssgrain_(mat.GetSSGrain()),
  vacancy_(&vacancy)
{
  mat.AddSolute(CE);
  vacancy.AddInteractingSolute(CE);
  CE.SetDiffusion(*this);
}


//CONSTRUCTOR 
//ONLY ATOMIC DIFFUSION. (not vacancies diffusion)
//can also be used in case material did not have a ssgrain! Chemical element diffuse in material or material's ssgrain(which can be =0), 
Diffusion::Diffusion(ChemicalElement& CE, Material& mat,double preExpDiffCoef,double actEn)
 :preExpDiffusionCoef_(preExpDiffCoef),
  activationEnergy_(actEn),
  interactionEnergyWithVacancy_(-1),
  chemicalElement_(CE),
  material_(&mat),
  ssgrain_(mat.GetSSGrain()),
  vacancy_(0)
{
  mat.AddSolute(CE);
  CE.SetDiffusion(*this);
}

//CONSTRUCTOR
//ONLY VACANCIES DIFFUSION. (not atomic diffusion)
Diffusion::Diffusion(ChemicalElement& CE, Vacancy& vacancy,double Evac)
 :preExpDiffusionCoef_(-1),
  activationEnergy_(-1),
  interactionEnergyWithVacancy_(Evac),
  chemicalElement_(CE),
  material_(0),
  ssgrain_(0),
  vacancy_(&vacancy)
{
  vacancy.AddInteractingSolute(CE);
  CE.SetDiffusion(*this);
}





Diffusion::~Diffusion()
{
}



void
Diffusion::ComputeCoefValue()
{
}


//RELATIONS
//getter
const Material& 
Diffusion::GetMaterial() const
{
  assert( (material_!=0 ) && "Cannot GetMaterial() for a Diffusion object which does not have Atomic diffusion!!! " );
  return *material_;
}

const Vacancy& 
Diffusion::GetVacancy() const
{
  assert( (vacancy_!=0 ) && "Cannot GetVacancy() for a Diffusion object which does not have Vacancies diffusion !!! " );
  return *vacancy_;
}



void
Diffusion::Info() const
{
  std::cout <<  "################################# Diffusion::Info #################################" 	<< std::endl;
  std::cout <<  "                        preExpDiffusionCoef: " << preExpDiffusionCoef_  << " SI unit" << std::endl;
  std::cout <<  "                           activationEnergy: " << activationEnergy_ << " SI unit" << std::endl;
  std::cout <<  "               interactionEnergyWithVacancy: " << interactionEnergyWithVacancy_ << " SI unit" << std::endl;
  std::cout <<  "                            chemicalElement: " << chemicalElement_.GetElementName() << " SI unit" << std::endl;
  if (material_->GetSSGrain()==0) 
  {
  std::cout <<  "            Diffusion parameter of <  "<< chemicalElement_.GetElementName() <<"> in : material"<< std::endl;
  }
  else
  {
  std::cout <<  "            Diffusion parameter of <  "<< chemicalElement_.GetElementName() <<"> in : solid solution grains of the material"<< std::endl;
  };
  std::cout <<  std::endl;
}