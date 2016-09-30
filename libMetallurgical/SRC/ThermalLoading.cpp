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

ThermalLoading::ThermalLoading(Computation& computation)
  :duration_(),
   time_(),
   temperature_(0),
   maxTimeStep_(1),
   computation_(computation)
{
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

//Read From python lists the values for temperature loading : the time and temperature
//(equivalent to read TK1 and set time, temperature , and duration attributes)
void 
ThermalLoading::ReadTemperatureLoadingFromPythonList(boost::python::list& timePyList, boost::python::list& temperaturePyList)
{
  
  assert ( (time_.size()==0) && (temperature_.size()==0) &&"temperature Loading has already been define");
  
  
  std::vector<double> timeVector= Util::Util::ConvertPythonListToVector(timePyList);
  std::vector<double> temperatureVector= Util::Util::ConvertPythonListToVector(temperaturePyList);
  
  
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
  std::vector<double>::const_iterator max_it, min_it ;
  
  max_it = std::max_element(time_.begin(), time_.end());
  min_it = std::min_element(time_.begin(), time_.end());
  
  double maxTimeThermalLoading, minTimeThermalLoading ;
  
  maxTimeThermalLoading=  *max_it;
  minTimeThermalLoading=  *min_it;
  
  assert ( (time_[0]== minTimeThermalLoading ) && (time_[time_.size()-1]== maxTimeThermalLoading) && "time in thermalLoding read file is not monotonous" );
  
  duration_=maxTimeThermalLoading;
  
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