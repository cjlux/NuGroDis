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

#ifndef __Computation__hpp__
#define __Computation__hpp__

#include <fstream>
#include <string>
#include <cassert>


// Forward declarations, avoid including...
class Hardening;
class ThermalLoading;
class RadiusDistribution;
class Quenching;
class Material;

class Computation
{

public:
  // The constructor.  resultsDirectory_ , manualMaximumAllowedTimeStep_, are OPTIONAL  !!!!
  // by default, resultsDirectory_="" ; defineManuallyMaximumAllowedTimeStep_=false ; manualMaximumAllowedTimeStep_=-1  !!!!
  Computation(double initialTimeStep, std::string ResultsDirectory="", bool defineManuallyMaximumAllowedTimeStep=false ,double manualMaximumAllowedTimeStep=-1);

  // The destructor
  ~Computation();
  

  
  void ProcessMaxComputationTime();

  // Setters:
  void SetType(const std::string &);
  
  std::string GetType() const; 
  
  double GetMaxComputationTime() const;
  
  double GetMaxTimeStep() const { assert(maxTimeStep_>0) ;  return maxTimeStep_;};
  
  double GetDefaultTimeStep() const { assert(defaulTimeStep_>0) ; return defaulTimeStep_;};
  
  void UpdateCurrentTime();
  
  //Compute and save maxComputation time : Hardening.duration + ThermalLoading.duration
  void ComputeDuration();

  void ProcessThermalLoadingData();

  //Reads the data file for the simulation and make all the other objects read their data.
  void ReadDataFile(std::string fileName);

  void Run();
  
  void CreateResultsDirectory();
  void CreateDirectory(std::string relativePathOfTheWantedFolder);
  
  void Info() const;
  
  void ComputeMaxTimeStep();
  
  double GetCurrentTime() const { assert (currentTime_>=0);  return currentTime_;}
  
  int GetIncrement() const { return increment_;};
  std::string GetResultsDirectory() const {return resultsDirectory_;};
  
  
  bool CheckIfMaximumAllowedTimeStepHasBeenDefinedManually();
  double GetManualMaximumAllowedTimeStep();
  
  //Relations
  //setters
  void SetHardening(Hardening& hardening);
  void SetQuenching(Quenching& quenching);
  void SetThermalLoading(ThermalLoading& thermalLoading);
  void SetRadiusDistribution(RadiusDistribution *);
  void SetMaterial(Material& mat);
  
  //getters
  RadiusDistribution& GetRadiusDistribution() const;
  
  Material& GetMaterial() const;
  
  Hardening& GetHardening() const ;
  
  Quenching& GetQuenching() const ;
  
  ThermalLoading& GetThermalLoading() const;
  
  
  

private:

  RadiusDistribution * radiusDistribution_;
  Quenching * quenching_;
  Hardening * hardening_;
  ThermalLoading * thermalLoading_;
  Material * material_;
  

  //The total computation duration in second [s] = Maturation.duration + ThermalLoading.duration (was tmax)
  //Unit: s
  double maxComputationTime_;
  std::string type_;
  double currentTime_; // unit: s
  
  //Also can be used as initial value for time step. (was deltati). Unit: s
  double defaulTimeStep_;
  
  //The maximum value allowed for any computed Time step. is deltat  Unit: s
  double maxTimeStep_;
  
  int increment_;
  
  std::string resultsDirectory_;
  
  
  double const manualMaximumAllowedTimeStep_;
  bool const defineManuallyMaximumAllowedTimeStep_;
  
};


inline bool
Computation::CheckIfMaximumAllowedTimeStepHasBeenDefinedManually()
{
  return defineManuallyMaximumAllowedTimeStep_;
}

inline double
Computation::GetManualMaximumAllowedTimeStep()
{
  
  assert( ( this->CheckIfMaximumAllowedTimeStepHasBeenDefinedManually() == true )&& "Cannot get manual maximum allowed time step because it has not been defined during initialization!");
  
  
  assert ((manualMaximumAllowedTimeStep_>0)&&"Cannot get manual time step because it is not strictly positive. There is an error somewhere!!!");
  
  
  return manualMaximumAllowedTimeStep_;
  
}

inline void
Computation::SetType(const std::string & t)
{
  type_ = t;
}

inline std::string 
Computation::GetType() const
{
  return type_;
}



inline double
Computation::GetMaxComputationTime() const
{
  return maxComputationTime_;
}

inline void
Computation::SetHardening(Hardening& hardening)
{
  
  assert ((hardening_==0)&&"Computation is already linked to a Hardening Object");
  hardening_=&hardening;
}

inline void
Computation::SetQuenching(Quenching& quenching)
{
  assert ((quenching_==0)&&"Computation is already linked to a Quenching Object");
  quenching_=&quenching;
}

inline void
Computation::SetThermalLoading(ThermalLoading& thermalLoading)
{
  assert ((thermalLoading_==0)&&"Computation is already linked to a ThermalLoading Object");
  thermalLoading_=&thermalLoading;
}

inline RadiusDistribution&
Computation::GetRadiusDistribution() const
{
  assert ((radiusDistribution_!=0)&&"Computation object is not linked to any RadiusDistribution object yet ");
  
  return *radiusDistribution_;
}


inline void
Computation::SetMaterial(Material& mat)
{
  assert (material_==0);
  material_=&mat; 
}

inline Hardening& 
Computation::GetHardening() const 
  {
    assert ((hardening_!=0)&&"Computation object is not linked to any Hardening Object yet");
    return *hardening_;
  };
  
inline Quenching& 
Computation::GetQuenching() const 
{
  assert ((quenching_!=0)&&"Computation object is not linked to any Quenching Object yet");
  return *quenching_;
};
  
inline ThermalLoading& 
Computation::GetThermalLoading() const
{ 
  assert ((thermalLoading_!=0)&&"Computation object is not linked to any ThermalLoading Object yet");
  return *thermalLoading_ ;
};

inline  Material& 
Computation::GetMaterial() const
{
  assert ((material_!=0)&&"Computation Object is not linked to any material yet");
  
  return *material_;
}



#endif
