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
  void SetType(const std::string &);
  std::string GetType() const;
  void SetRadiusDistribution(RadiusDistribution *);
  RadiusDistribution * GetRadiusDistribution() const;
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
Computation::SetType(const std::string & t)
{
  type_ = t;
}

inline std::string 
Computation::GetType() const
{
  return type_;
}
