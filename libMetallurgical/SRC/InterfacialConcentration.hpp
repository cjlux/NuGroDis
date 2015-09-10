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

#ifndef __InterfacialConcentration__hpp__
#define __InterfacialConcentration__hpp__

#include <vector>
#include <cassert>

class RadiusDistribution;
class ChemicalElement;

class InterfacialConcentration
{

public:
  InterfacialConcentration(RadiusDistribution& RD,ChemicalElement& CE); 
  ~InterfacialConcentration();
  
  const double GetLeftInterfacialVelocityForClass(unsigned int classId)  ; //classId varies from 1 to n
  
  const double GetRightInterfacialVelocityForClass(unsigned int classId) ; //classId varies from 1 to n
  
  const double GetLeftInterfacialConcValueForClass(unsigned int classId)  ; //classId varies from 1 to n
  
  double GetRightInterfacialConcValueForClass(unsigned int classId) ; //classId varies from 1 to n
  
  void SetLeftInterfacialConcValueForClass(double value,unsigned int classId);
  
  void SetRightInterfacialConcValueForClass(double value,unsigned int classId);
  
  std::vector<double> ReturnInterfacialVelocityList();// list of VINT 
  
  void ComputeInterfacialVelocityList(); //compute list of VINT
  
  const std::vector<double> GetInterfacialVelocityList(); // get list of VINT
  
  const double ReturnCriticalInterfacialVelocity();// VINTcritique_El_P . example VINTcritique_Cu_P

  //GETTERS
  //return list (all values) of interfacial concentration
  const std::vector<double>& GetInterfacialConcentrationValuesList() const {return interfacialConcentrationValuesList_;};
  //return a value of interfacial concentration for a given item. position started from 0 ton n
  const double GetItemValueInInterfConcList(int positionOfInterface);
  
  double GetCriticalInterfacialConcentration() const;  
  
  
  //SETTERS
  //set a value of interfacial concentration for a given item
  void SetItemValueInInterfConcList(const double &computedValueOfItemInInterfConcList);
  void SetCriticalInterfacialConcentration(const double & computedValueOfCriticalInterfacialConcentration);
  
  
  
  //RELATIONS
  //getter
  const RadiusDistribution& GetRadiusDistribution() const {return radiusDistribution_;};
  const ChemicalElement& GetChemicalElement() const {return chemicalElement_;};
protected:

private:
  std::vector<double> interfacialConcentrationValuesList_;//vector of XvintP
  
  std::vector<double> interfacialVelocityList_;
  
  double criticalInterfacialConcentration_;//XvintcritiqueP_i. example: XvintcritiqueP_Cu, XvintcritiqueP_Mg
  
  
  //RELATION
  const RadiusDistribution& radiusDistribution_;
  const ChemicalElement& chemicalElement_;

};

inline  double 
InterfacialConcentration::GetCriticalInterfacialConcentration() const
{
  assert ( (criticalInterfacialConcentration_ !=-1)&&"criticalInterfacialConcentration_ has not been computed yet");
  
  assert( (criticalInterfacialConcentration_ >=0)&&"Cannot get value because criticalInterfacialConcentration_ is not positive" );
  
  assert( (criticalInterfacialConcentration_ !=222.222)&&"Cannot GetCriticalInterfacialConcentrationCritical() because: interfacial concentration Cannot be found at this time. No solution from Equation system." );
  
  return criticalInterfacialConcentration_;
}

inline void
InterfacialConcentration::SetCriticalInterfacialConcentration(const double & computedValueOfCriticalInterfacialConcentration)
{
  assert ((computedValueOfCriticalInterfacialConcentration>=0)&&"Cannot SetCriticalInterfacialConcentration(): given value is not  positive!!");
  
  criticalInterfacialConcentration_=computedValueOfCriticalInterfacialConcentration;
}

inline const std::vector<double>
InterfacialConcentration::GetInterfacialVelocityList()
{
  //TODO find a way to Check if interfacialVelocityList_ has been computed before!
  
  return interfacialVelocityList_;
}



#endif
