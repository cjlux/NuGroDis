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
  Hardening(double hardeningDuration , double initialTimeStep);
  ~Hardening();

  std::string GetInfo();
  
  void Info() const;
  
  //compute and set the maximum value allowed for any computed Time step. This value is computed Unit: s
  double ComputeTimeStep();
  
  double GetMaxTimeStep() const { return maxTimeStep_; };
  double GetDuration()    const { return duration_;    }; 
  
  void SetMaxTimeStep(const double &maxTimeStep);

protected:

private:

    //Hardening duration in second[s]. (was tmat). Unit: s    
    double const duration_;
    
    //The maximum value allowed for any computed Time step. Also can be used as initial value for time step. (was deltati). Unit: s
    double maxTimeStep_;
};

inline void
Hardening::SetMaxTimeStep(const double &maxTimeStep)
{
  maxTimeStep_=maxTimeStep;
}

