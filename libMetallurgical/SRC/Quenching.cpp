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
