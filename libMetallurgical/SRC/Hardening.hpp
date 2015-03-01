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
