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

class ThermoDynamicsConstant
{

public:
  ThermoDynamicsConstant(double R, double kB, double Na);
  ~ThermoDynamicsConstant();
  
  void Info() const;

  double GetR()   const { return R_;  };
  double Get_kB() const { return kB_; };
  double GetNa()  const { return Na_; };
  
protected:

private:

  //Perfect gas constant. Unit: J/mol.K 
  const double R_;
    
  //Boltzman constant. Unit: J/K
  const double kB_;
    
  //Avogadro's number. Unit: at/mol
  const double Na_;

};
