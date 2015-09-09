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
#include <cassert>
#include <cmath>

#include "InterfacialConcentration.hpp"
#include "RadiusDistribution.hpp"
#include "ChemicalElement.hpp"
#include "ThermoDynamicsConstant.hpp"
#include "Util.hpp"
#include "Diffusion.hpp"
#include "Material.hpp"
#include "Precipitate.hpp"
#include "Concentration.hpp"
#include "ChemicalComposition.hpp"

InterfacialConcentration::InterfacialConcentration(RadiusDistribution& RD,ChemicalElement& CE)
  :interfacialConcentrationValuesList_(),
   interfacialVelocityList_(),
   criticalInterfacialConcentration_(-1),
   radiusDistribution_(RD),
   chemicalElement_(CE)
{
  std::cout<<"building object <InterfacialConcentration>"<<std::endl;
  
  
  //Initializing the table of interface radius values
  unsigned int n = radiusDistribution_.GetItemsValues().size();
  
  assert((n>0)&&"RadiusDistribution values have not been initialize");
  for (unsigned int i=1; i <= n+1; ++i)
  {
    interfacialConcentrationValuesList_.push_back(0);
  }
  
  assert ( (interfacialConcentrationValuesList_.size()== n+1)&&"InterfacialConcentrationValuesList_ do not have a compatible size with RadiusDistribution size" ); 
}

InterfacialConcentration::~InterfacialConcentration()
{
}



const double 
InterfacialConcentration::GetLeftInterfacialVelocityForClass(unsigned int classId)  //classId varies from 1 to n
{
  unsigned int n;
  n= radiusDistribution_.GetItemsValues().size();
  
  assert(  (interfacialVelocityList_.size()== (n+1) )&&"In GetLeftInterfacialVelocityForClass(int  classId) : interfacialVelocityList_.size() \
  and radiusDistribution_.GetItemsValues().size() are incompatible!!! interfacialVelocityList_.size() must be = radiusDistribution_.GetItemsValues().size() +1 " );
  assert ( (classId>=1)&&"InterfacialConcentration::GetLeftInterfacialVelocityForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"InterfacialConcentration::GetLeftInterfacialVelocityForClass():   ClassId must be in the range 1 to n");
  
  
  return interfacialVelocityList_[classId-1];
  
}
  
const double
InterfacialConcentration::GetRightInterfacialVelocityForClass(unsigned int classId)  //classId varies from 1 to n
{
  unsigned int n;
  n= radiusDistribution_.GetItemsValues().size();
  
  assert(  (interfacialVelocityList_.size()== (n+1) )&&"In GetRightInterfacialVelocityForClass(int  classId) : interfacialVelocityList_.size() \
  and radiusDistribution_.GetItemsValues().size() are incompatible!!! interfacialVelocityList_.size() must be = radiusDistribution_.GetItemsValues().size() +1 " );
  assert ( (classId>=1)&&"InterfacialConcentration::GetRightInterfacialVelocityForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"InterfacialConcentration::GetRightInterfacialVelocityForClass():   ClassId must be in the range 1 to n");
  
  return interfacialVelocityList_[classId];
}



const double
InterfacialConcentration::GetLeftInterfacialConcValueForClass(unsigned int classId) //classId varies from 1 to n
{

  unsigned int n;
  
  n= radiusDistribution_.GetItemsValues().size();
  
  assert(  (interfacialConcentrationValuesList_.size()== (n+1) )&&"In GetLeftInterfacialConcValueForClass(int  classId) : interfacialConcentrationValuesList_.size() \
  and radiusDistribution_.GetItemsValues().size() are incompatible!!! interfacialConcentrationValuesList_.size() must be = radiusDistribution_.GetItemsValues().size() +1 " );
  assert ( (classId>=1)&&"InterfacialConcentration::GetLeftInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"InterfacialConcentration::GetLeftInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
 
  assert( interfacialConcentrationValuesList_[classId-1]>=0 );

  return interfacialConcentrationValuesList_[classId-1] ;
}

double
InterfacialConcentration::GetRightInterfacialConcValueForClass(unsigned int classId)  //classId varies from 1 to n
{

  unsigned int n;
  
  n= radiusDistribution_.GetItemsValues().size();
  
  assert(  (interfacialConcentrationValuesList_.size()== n+1)&&"In GetRightInterfacialConcValueForClass(int  classId) : interfacialConcentrationValuesList_.size() \
  and radiusDistribution_.GetItemsValues().size() are incompatible!!! interfacialConcentrationValuesList_.size() must be = radiusDistribution_.GetItemsValues().size() +1 " );
  assert ( (classId>=1)&&"InterfacialConcentration::GetRightInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"InterfacialConcentration::GetRightInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
  
  assert( interfacialConcentrationValuesList_[classId]>=0 );
  

  return interfacialConcentrationValuesList_[classId] ;
}

void 
InterfacialConcentration::SetLeftInterfacialConcValueForClass(double value, unsigned int classId)
{
  assert ((value>=0)&&"SetLeftInterfacialConcValueForClass(value, classId) : given value must be positive");
  
  unsigned int n;
  
  n= radiusDistribution_.GetItemsValues().size();
  
  assert(  (interfacialConcentrationValuesList_.size()== n+1)&&"In SetLeftInterfacialConcValueForClass(int  classId) : interfacialConcentrationValuesList_.size() \
  and radiusDistribution_.GetItemsValues().size() are incompatible!!! interfacialConcentrationValuesList_.size() must be = radiusDistribution_.GetItemsValues().size() +1 " );
  
  assert ( (classId>=1)&&"InterfacialConcentration::GetLeftInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"InterfacialConcentration::GetLeftInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
  
  
  interfacialConcentrationValuesList_[classId-1]=value;
}

void 
InterfacialConcentration::SetRightInterfacialConcValueForClass(double value,unsigned int classId)
{
  assert ((value>=0)&&"SetLeftInterfacialConcValueForClass(value, classId) : given value must be positive");
  
  unsigned int n;
  
  n= radiusDistribution_.GetItemsValues().size();
  
  
  assert(  (interfacialConcentrationValuesList_.size()== n+1)&&"In SetRightInterfacialConcValueForClass(int  classId) : interfacialConcentrationValuesList_.size() \
  and radiusDistribution_.GetItemsValues().size() are incompatible!!! interfacialConcentrationValuesList_.size() must be = radiusDistribution_.GetItemsValues().size() +1 " );
  
  assert ( (classId>=1)&&"InterfacialConcentration::GetLeftInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"InterfacialConcentration::GetLeftInterfacialConcValueForClass():   ClassId must be in the range 1 to n");
  
  interfacialConcentrationValuesList_[classId]=value;
}



//GETTERS
//return a value of interfacial concentration for a given item
const double
InterfacialConcentration::GetItemValueInInterfConcList(int positionOfInterface)
{//TODO
  assert(0);
}
  
//SETTERS
//set a value of interfacial concentration for a given item
void
InterfacialConcentration::SetItemValueInInterfConcList(const double &computedValueOfItemInInterfConcList)
{//TODO
  assert(0);
}

std::vector<double>
InterfacialConcentration::ReturnInterfacialVelocityList()
{
    //////
  std::string elementSymbol = chemicalElement_.GetElementName();
  std::map<std::string, Concentration*> materialCurrentConcMap= radiusDistribution_.GetPrecipitate().GetMaterial().GetCurrentChemicalComposition().GetConcentrationMap();
  std::map<std::string, Concentration*> precipitateConcMap =radiusDistribution_.GetPrecipitate().GetChemicalComposition().GetConcentrationMap();
  double DiffusionCoef = chemicalElement_.GetDiffusion().GetAtomicDiffusionCoef();
  double XvSS= materialCurrentConcMap[elementSymbol]->GetVolumicValue();
  double XvP=  precipitateConcMap[elementSymbol]->GetVolumicValue();
  
  
  std::vector<double> interfacialVelocityList;
  double R,XvInt,Vint,R0,XvInt0,Vint0;
  
  assert (interfacialConcentrationValuesList_.size()>0);
  
  //First value. the value at left of class 1
  R0= radiusDistribution_.GetLeftRadiusForClass(1);
  XvInt0= interfacialConcentrationValuesList_[0];
  assert(R0>0);
  assert((XvP-XvInt0)!=0);
  Vint0=DiffusionCoef/R0*(XvSS-XvInt0)/(XvP-XvInt0);  
  interfacialVelocityList.push_back(Vint0);
  
  //Other values (Value to the right)
  for (unsigned int i=1; i < interfacialConcentrationValuesList_.size(); ++i)
  {
    R= radiusDistribution_.GetRightRadiusForClass(i);
    XvInt= interfacialConcentrationValuesList_[i];
    
    assert(R>0);
    assert((XvP-XvInt)!=0);
    
    Vint=DiffusionCoef/R*(XvSS-XvInt)/(XvP-XvInt);
    
    interfacialVelocityList.push_back(Vint);
  }
  
  assert( interfacialVelocityList.size()==interfacialConcentrationValuesList_.size() );
  
  return interfacialVelocityList; 
  
}

void 
InterfacialConcentration::ComputeInterfacialVelocityList()
{
  //this method must be use after Precipitate::ComputeAllInterfacialConcentrations()
 
  interfacialVelocityList_= this->ReturnInterfacialVelocityList();  
}



const double 
InterfacialConcentration::ReturnCriticalInterfacialVelocity()
{
  std::string elementSymbol = chemicalElement_.GetElementName();
  std::map<std::string, Concentration*> materialCurrentConcMap= radiusDistribution_.GetPrecipitate().GetMaterial().GetCurrentChemicalComposition().GetConcentrationMap();
  std::map<std::string, Concentration*> precipitateConcMap =radiusDistribution_.GetPrecipitate().GetChemicalComposition().GetConcentrationMap();
  double DiffusionCoef = chemicalElement_.GetDiffusion().GetAtomicDiffusionCoef();
  double XvSS= materialCurrentConcMap[elementSymbol]->GetVolumicValue();
  double XvP=  precipitateConcMap[elementSymbol]->GetVolumicValue();
  double criticalIntR,criticalXvInt,criticalVint,rEtoile;
  
  rEtoile = radiusDistribution_.GetPrecipitate().GetCriticalRadius();
  if (rEtoile!=-111.111)
  {
  criticalXvInt=criticalInterfacialConcentration_;
  assert (criticalXvInt>0);

  
  criticalIntR=radiusDistribution_.GetPrecipitate().ReturnCriticalInterfacialConcentrationRadius();
  assert (criticalIntR>0);
  
  criticalVint=DiffusionCoef/criticalIntR*(XvSS-criticalXvInt)/(XvP-criticalXvInt);
  }
  else
  { assert(rEtoile==-111.111);
    // Si  retoile ne s'applique pas, on utilise la première vitesse d'interface comme vitesse critique.
    criticalVint= interfacialVelocityList_[0];
    assert (criticalVint!=0);
  };

  return criticalVint;
}




