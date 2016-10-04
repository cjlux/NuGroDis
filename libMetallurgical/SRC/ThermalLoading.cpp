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
#include <iostream>
#include <cassert>
//#include <algorithm>

#include "ThermalLoading.hpp"
#include "Computation.hpp"
#include "Util.hpp"

#include <boost/python.hpp>

ThermalLoading::ThermalLoading(Computation& computation, double maximumTimeStepAllowedDuringThermalLoading) // by default maximumTimeStepAllowedDuringThermalLoading =1
  :duration_(-1),
   time_(),
   temperature_(0),
   maxTimeStep_(maximumTimeStepAllowedDuringThermalLoading),
   computation_(computation)
{
  std::cout <<  "Building object <ThermalLoading> " << std::endl;
  assert ((maxTimeStep_>0)&&"Given value for maximum Time Step Allowed During ThermalLoading must be > 0 ") ;
  computation_.SetThermalLoading(*this);
}

ThermalLoading::~ThermalLoading()
{
}

void
ThermalLoading::ReadDistributionFile(std::string fileName)
{
}

void
ThermalLoading::ReadTemperatureLoading(std::string fileName)
{
}

void
ThermalLoading::ReadVCAH(std::string fileName)
{
}



//set time, temperature , and duration attributes
//Read From python lists the values for temperature loading : the time and temperature
//(equivalent to read TK1)
void 
ThermalLoading::ReadTemperatureLoadingFromPythonList(boost::python::list& timePyList, boost::python::list& temperaturePyList)
{
  
  assert ( (time_.size()==0) && (temperature_.size()==0) &&"temperature Loading has already been define");
  
  
  std::vector<double> timeVector= Util::Util::ConvertPythonListToVector(timePyList);
  std::vector<double> temperatureVector= Util::Util::ConvertPythonListToVector(temperaturePyList);
  
  
  assert ( (timeVector[0]==0)&&"Temperature loading did not start at time t=0 ");
  
  //////////////////////////////////////
  // Check if timeVector is monotonous and increasing, as it should be
  unsigned int j=0;
  while ( (j<timeVector.size()) && (j+1<timeVector.size())  )
  {
    assert ( ( timeVector[j]< timeVector[j+1] ) && "Time in ThermalLoading temperature file is not monotonous and increasing" );
    j=j+1;
  }
  // Check if timeVector is monotonous and increasing, as it should be
  //////////////////////////////////////
  
  for (unsigned int i=0; i<timeVector.size(); ++i)
  {
    time_.push_back(timeVector[i]);
  }
  assert (time_.size()==timeVector.size());
  
  for (unsigned int i=0; i<temperatureVector.size(); ++i)
  {
    temperature_.push_back(temperatureVector[i]);
  }
  assert (temperature_.size()==temperatureVector.size());
  
  
  // set duration
  std::vector<double>::const_iterator max_time_it, min_time_it ;
  
  max_time_it = std::max_element(time_.begin(), time_.end());
  min_time_it = std::min_element(time_.begin(), time_.end());
  
  double maxTimeThermalLoading, minTimeThermalLoading ;
  
  maxTimeThermalLoading=  *max_time_it;
  minTimeThermalLoading=  *min_time_it;
  
  assert ( (time_[0]== minTimeThermalLoading ) && (time_[time_.size()-1]== maxTimeThermalLoading) && "time in thermalLoading read file is not monotonous" );
  
  
  assert  ( (duration_==-1)&&"Thermal loading duration has already been defined " );
  duration_=maxTimeThermalLoading;
  
}


double 
ThermalLoading::ReturnTemperatureAtTime(double time)
{
  assert ( (time_.size()!=0) && (temperature_.size()!=0) && (time_.size()==temperature_.size()) &&"Temperature loading has not been defined yet!"); 
  
  std::vector<double>::const_iterator max_time_it, min_time_it ;
  max_time_it = std::max_element(time_.begin(), time_.end());
  min_time_it = std::min_element(time_.begin(), time_.end());
  
  assert (  (time>=  *min_time_it )&& (time<= *max_time_it ) && "Given time is not in the range [time_min; time_max]" );
  
  
  
  //////////////////////////////////////////////////////////
  //// find the closest value and its index ////////////////
  double maxDistance=  std::abs(*max_time_it - *min_time_it);
  int indexClosestValue=-1;
  double closestValue=-1; // initialization, the maximum value
  double distance=-1;
  for (unsigned int i=0; i<time_.size(); ++i)
  {
    distance= std::abs (time_[i] - time);
    if (distance<=maxDistance)
    {
      maxDistance=distance;
      closestValue=time_[i];
      indexClosestValue=i;
    };
  }
  assert ( (indexClosestValue!=-1)&&(closestValue!=-1)&&(distance!=-1)  );
  //// find the closest value and its index ////////////////
  //////////////////////////////////////////////////////////
  
  
  
  ///////////////////////////////////////////////////////////////
  ///////// Return the corresponding value of temperature ///////
  double temperatureValueToReturn;
  
  if (time_[indexClosestValue]==time)
  {
    // the given value of time is exactly defined
    temperatureValueToReturn=temperature_[indexClosestValue];
  }
  else
  {
    
    // Linear interpolation to find the value because the given time is not exactly defined
    
    std::cout<<"No exact data for thermal Loading after a duration of : "<< time  <<" [s]. Therefore, a linear interpolation will be made to find the corresponding value of Temperature"<<std::endl;
    
    assert (closestValue != time);
    
    int indexIntervalMin=-1; // -1 is just fake value for initialization, will be used to check after
    int indexIntervalMax=-1; // -1 is just fake value for initialization, will be used to check after
      
    if (closestValue < time)
    {
      indexIntervalMin= indexClosestValue;
      indexIntervalMax= indexClosestValue +1;
    }
    else
    {
      // in this case, time < closestValue
      indexIntervalMin= indexClosestValue-1;
      indexIntervalMax= indexClosestValue;     
    };
     
    assert ( (indexIntervalMin!=-1)&&(indexIntervalMax!=-1)  );
    
    
    temperatureValueToReturn=  temperature_[indexIntervalMin] + (time-time_[indexIntervalMin])*(temperature_[indexIntervalMax]-temperature_[indexIntervalMin])/(time_[indexIntervalMax]-time_[indexIntervalMin]);
  
  };
  ///////// Return the corresponding value of temperature ///////
  ///////////////////////////////////////////////////////////////
  
  
  std::cout<< "Temperature at :"<< time << " [s] after the begining of thermal loading is : "<< temperatureValueToReturn<< " [K]."<<std::endl;
  
  return temperatureValueToReturn;
  
}




void
ThermalLoading::Info() const
{
  std::cout <<  "############################ ThermalLoading::Info ###################################" << std::endl;
  
  if (duration_==0)
 {std::cout <<  "                                   duration: Is not defined yet"<<std::endl;}
  else
 {std::cout <<  "                                   duration: " << duration_ << " SI unit" << std::endl;};

 //TODO time_ and temperature_
  
 std::cout << std::endl;
}