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


#ifndef __Quenching__hpp__
#define __Quenching__hpp__

#include <cassert>

class Computation; 

class Quenching
{
public:
  Quenching(Computation& computation, double Tsol, double Tfinal, double coolingRate, double deltaT = 1.);
  ~Quenching();

  void Info() const;

  //Getters
  double GetCoolingRate() const        { return coolingRate_; }
  double GetTotIterationNumber() const { return totIterationNumber_; }
  double GetFinalTemp() const          { return finalTemp_; }
  double GetDuration() const           { return duration_;}
  double GetTimeStep() const           { return timeStep_;}
  double GetDeltaT() const             { return deltaT_;}
  double GetSolutionizingTemp() const  { return solutionizingTemp_;}

  //Setters
  void SetTotIterationNumber(double tin);//Set totIterationNumber_  and compute timeStep_ and deltaT_ 
  
  //Relations
  Computation& GetComputation() const {return computation_;};

protected:

private:
    //Relations
  Computation& computation_;
  
  //attributes
  const double solutionizingTemp_;
  const double finalTemp_;
  const double coolingRate_;
  const double duration_;
  double timeStep_;
  double totIterationNumber_;
  double deltaT_;

};


//Set totIterationNumber and use it to compute corrct values of deltaT, and timeStep_. Maybe, this part has TO BE REFINED
inline void
Quenching::SetTotIterationNumber(double tin)
{
  // Preconditions:
  assert(tin > 0);	// TODO : refine

  totIterationNumber_ = tin;
  deltaT_ = (solutionizingTemp_ - finalTemp_)/totIterationNumber_;
  timeStep_ = duration_ / totIterationNumber_;

  // Postcondition
  assert(deltaT_ <= 1.);	// tin should give a value of deltaT_ better than 1 degree !
}

#endif
