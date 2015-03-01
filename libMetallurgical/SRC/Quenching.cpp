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

#include "Quenching.hpp"

#include <cassert>

Quenching::Quenching(double Tsol, double Tfinal, double coolingRate, double deltaT)
  : solutionizingTemp_(Tsol),
    finalTemp_(Tfinal_),
    coolingRate_(coolingRate_),
    duration_(solutionizingTemp_ - finalTemp_)/coolingRate_),
    timeStep_(0),
    totIterationNb_(0),
    deltaT_(delatT)
{
  // Preconditions:
  assert(finalTemp_ < solutionizingTemp_);
  assert(coolingRate_ > 0);
  assert(delatT_ > 0);

  totIterationNumber_ = (solutionizingTemp - finalTemp_)/deltaT_;
  timeStep_ = duration_ / totIterationNumber_;
}

Quenching::~Quenching()
{
}
