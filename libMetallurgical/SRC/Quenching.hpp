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

#include <cassert>

class Quenching
{
public:
  Quenching(double Tsol, double Tfinal, double coolingRate, double deltaT = 1.);
  ~Quenching();

  void Info() const;

  double GetCoolingRate() const        { return coolingRate_; }
  double GetTotIterationNumber() const { return totIterationNumber_; }
  double GetFinalTemp() const          { return finalTemp_; }

  void SetCoolingRate(double x) { assert(x > 0); coolingRate_ = x; }
  void SetTotIterationNumber(double);

protected:

private:
  double solutionizingTemp_;
  double finalTemp_;
  double coolingRate_;
  const double duration_;
  double timeStep_;
  double totIterationNumber_;
  double deltaT_;
};

inline void
Quenching::SetTotIterationNumber(double tin)
{
  // Preconditions:
  assert(tin > 0);	// TODO : affiner 

  totIterationNumber_ = tin;
  deltaT_ = (solutionizingTemp_ - finalTemp_)/totIterationNumber_;
  timeStep_ = duration_ / totIterationNumber_;

  // Postcondition
  assert(deltaT_ <= 1.);	// tin should give a value of deltaT_ better than 1 degree !
}

