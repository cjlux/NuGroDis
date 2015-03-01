#include <cassert>

class Quenching
{
public:
  Quenching(double Tsol, double Tfinal, double coolingRate, double deltaT=1.);
  ~Quenching();

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

