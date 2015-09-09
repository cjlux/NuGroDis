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


#ifndef __Hardening__hpp__
#define __Hardening__hpp__

#include <string>
class Computation;

class Hardening
{
public:
  Hardening(double hardeningDuration, Computation& computation);
  ~Hardening();

  std::string GetInfo();
  
  void Info() const;
  
  //compute and set the maximum value allowed for any computed Time step. This value is computed Unit: s
  double ComputeTimeStep();
  
  double GetMaxTimeStep() const { return maxTimeStep_; };//TODO ERASE, unusefull ?
  double GetDuration()    const { return duration_;    }; 
  
  void SetMaxTimeStep(const double &maxTimeStep); //TODO ERASE, unusefull ?
						  //TODO unusefull? because we already have ComputeTimeStep(). And it will be good if we hanve only one method which modify the time step
  
  void ComputeMaxTimeStep(); //Compute and set the maxTimeStep //TODO only declaration have been made //TODO ERASE, unusefull ?
  
  Computation& GetComputation() const {return computation_;};

protected:

private:

    //Hardening duration in second[s]. (was tmat). Unit: s    
    double const duration_;
    
    //The maximum value allowed for any computed Time step. Also can be used as initial value for time step. (was deltati). Unit: s
    double maxTimeStep_;//TODO ERASE, unusefull ?
    
    //Relations
    Computation& computation_;
};

inline void
Hardening::SetMaxTimeStep(const double &maxTimeStep)
{
  maxTimeStep_=maxTimeStep;//TODO ERASE, unusefull ?
}

#endif
