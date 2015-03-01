#include <string>

// Forward declarations, avoid including...
class Hardening;
class ThermalLoading;
class RadiusDistribution;
class Quenching;

class Computation
{

public:
  // The constructor :
  Computation();

  // The destructor
  ~Computation();

  // Setters:
  void SetType(std::string);
  void SetRadiusDistribution(RadiusDistribution *);

  //Compute and save maxComputation time : Hardening.duration + ThermalLoading.duration
  void ComputeDuration();

  void ProcessThermalLoadingData();

  //Reads the data file for the simulation and make all the other objects read their data.
  void ReadDataFile(std::string fileName);

  void Run();

private:

  RadiusDistribution * radiusDistribution_;
  Quenching * quenching_;
  Hardening * hardening_;
  ThermalLoading * thermalLoading_;
  

  //The total computation duration in second [s] = Maturation.duration + ThermalLoading.duration (was tmax)
  //Unit: s
  double maxComputationTime_;
  std::string type_;
};

inline void
Computation::SetType(std::string t)
{
  type_ = t;
}
