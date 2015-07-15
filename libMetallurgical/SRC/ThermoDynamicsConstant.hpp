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


#ifndef __ThermoDynamicsConstant__hpp__
#define __ThermoDynamicsConstant__hpp__

#include <cmath>

class ThermoDynamicsConstant
{

public:
  ThermoDynamicsConstant(double R, double kB, double Na);
  ~ThermoDynamicsConstant();
  
  void Info() const;

  double Get_R()   { return R_;  };
  double Get_kB()  { return kB_; };
  double Get_Na()  { return Na_; };
  
  static double GetR()  {return R;};
  static double GetNa() {return 6.02214179*std::pow(10,23);};
  static double GetkB() {return 1.3806488*std::pow(10,-23);};
  
protected:

private:

  //Perfect gas constant. Unit: J/mol.K 
  const double R_;
    
  //Boltzman constant. Unit: J/K
  const double kB_;
    
  //Avogadro's number. Unit: at/mol
  const double Na_;
  
  static const double R=8.314472; // Unit "J.mol^-1.K^-1"
  //static const double Na; //unit mol^-1
};



#endif
