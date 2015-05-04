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

class RadiusDistribution;
class ChemicalElement;

class InterfacialConcentration
{

public:
  InterfacialConcentration(RadiusDistribution& RD,ChemicalElement& CE);
  ~InterfacialConcentration();

  std::vector<double*> ComputeInterfacialConcentrationValuesList(double diameter, double temperature);

  //GETTERS
  //return list (all values) of interfacial concentration
  const std::vector<double*>& GetInterfacialConcentrationValuesList() const {return interfacialConcentrationValuesList_;};
  //return a value of interfacial concentration for a given item
  const double GetItemValueInInterfConcList(int positionOfInterface);
  
  //SETTERS
  //set a value of interfacial concentration for a given item
  void SetItemValueInInterfConcList(const double &computedValueOfItemInInterfConcList);
  
  
  //RELATIONS
  //getter
  const RadiusDistribution& GetRadiusDistribution() const {return radiusDistribution_;};
  const ChemicalElement& GetChemicalElement() const {return chemicalElement_;};
protected:

private:
  std::vector<double*> interfacialConcentrationValuesList_;
  
  //RELATION
  const RadiusDistribution& radiusDistribution_;
  const ChemicalElement& chemicalElement_;
  

};

#endif
