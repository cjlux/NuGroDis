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

#include <string>

class Hardening
{
public:
  Hardening();
  ~Hardening();

  std::string GetInfo();
  
  //The maximum value allowed for any computed Time step. Also can be used as initial value for time step. (was deltati). Unit: s
  double GetMinTimeStep();  

protected:

private:

    //Maturation duration in second[s]. (was tmat). Unit: s    
    double duration_;
    
    //The maximum value allowed for any computed Time step. Also can be used as initial value for time step. (was deltati). Unit: s
    double maxTimeStep_;
};
