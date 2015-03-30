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
#include "Vacancy.hpp"

Vacancy::Vacancy(double deltaHF,double deltaSF,double deltaHM,double fE,double Dlac0,double halfSinkD,double Tf,double EVacCu,double EVacMg)
  : vacCreationEnthalpy_(deltaHF), 
    vacCreationEntropy_(deltaSF), 
    concentration_(0), 
    concentrationBeforeQuenching_(0), 
    halfSinkDistance_(halfSinkD), 
    jumpFrequency_(fE), 
    preExpDiffusionValue_(Dlac0), 
    magnesiumVacInteractionEnergy_(EVacMg), 
    migrationEnthalpy_(deltaHM), 
    solutionisingTemp_(Tf), 
    copperVacInteractionEnergy_(EVacCu)
{
  
}

Vacancy::~Vacancy()
{
}

void
Vacancy::ComputeConcentrationBeforeQuenching()
{
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
}

double
Vacancy::GetHardeningFinalConcentration(std::string fileName)
{
}

double
Vacancy::SolveConcentrationEvolutionEquation()
{
}

void
Vacancy::Info() const
{
  std::cout <<  "############################ Vacancy::Info ############################" 	<< std::endl;
  std::cout <<  "                vacCreationEnthalpy : " << vacCreationEnthalpy_ << " SI unit" << std::endl;
  std::cout <<  "      		  vacCreationEntropy : " << vacCreationEntropy_<< " SI unit" << std::endl;
  std::cout <<  "                      concentration : " << concentration_ << " SI unit" << std::endl;
  std::cout <<  "       concentrationBeforeQuenching : " << concentrationBeforeQuenching_ << " SI unit" << std::endl;
  std::cout <<  "                   halfSinkDistance : " << halfSinkDistance_ << " SI unit" << std::endl;
  std::cout <<  "                      jumpFrequency : " << jumpFrequency_ << " SI unit" << std::endl;
  std::cout <<  "      magnesiumVacInteractionEnergy : " << magnesiumVacInteractionEnergy_ << " SI unit" << std::endl;
  std::cout <<  "                  migrationEnthalpy : " << migrationEnthalpy_ << " SI unit" << std::endl;
  std::cout <<  "                  solutionisingTemp : " << solutionisingTemp_ << " SI unit" << std::endl;
  std::cout <<  "         copperVacInteractionEnergy : " << copperVacInteractionEnergy_ << " SI unit" << std::endl;
}