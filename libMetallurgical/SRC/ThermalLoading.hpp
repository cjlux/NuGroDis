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

#ifndef __ThermalLoading__hpp__
#define __ThermalLoading__hpp__

#include <string>
#include <vector>
#include <boost/python.hpp>

class Computation; 
class Temperature;

class ThermalLoading
{
public:
    
    ThermalLoading(Computation& computation);

    ~ThermalLoading();
    
    void Info() const;
    
    //Read the File giving the radius distribution of precipitates (NP1, NP2, ...
    void ReadDistributionFile(std::string);
    
    //Read the values for the temperature loading 
    //(equivalent to read TK1 and set time and temperature attributes)
    void ReadTemperatureLoading(std::string);
    
    //Read From python lists the values for temperature loading : the time and temperature
    //(equivalent to read TK1 and set time, temperature, and duration attributes)
    void ReadTemperatureLoadingFromPythonList(boost::python::list& timePyList, boost::python::list& temperaturePyList);
    
    //Read Vacancies concentration after hardening (maturation or tempering)
    void ReadVCAH(std::string);
    
    //Getter
    double GetDuration() const {return duration_;};
    std::vector<double> GetTime() const {return time_;};
    std::vector<double> Gettemperature() const {return temperature_;};
    
    
    void ComputeMaxTimeStep();//TODO only declaration have been made //TODO ERASE, unusefull ?
    
    //Relations
    Computation& GetComputation() const {return computation_;};


protected:

private:

    //Thermal loading duration (was tmax - tmat). Unit: s
    double duration_; //TODO Set duration when reading File TK1
    
    //Time vector of temperature loading. Unit: s
    std::vector<double>  time_;
    
    //Temperature vector of ThermalLoading. Unit: K
    std::vector<double>  temperature_;
    
    double maxTimeStep_; //TODO ERASE, unusefull ?
    
    //Relations
    Computation& computation_;

};

#endif
