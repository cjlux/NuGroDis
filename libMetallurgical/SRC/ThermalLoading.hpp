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

class Temperature;

class ThermalLoading
{
public:
    
    ThermalLoading();

    ~ThermalLoading();
    
    void Info() const;
    
    //Read the File giving the radius distribution of precipitates (NP1, NP2, ...
    void ReadDistributionFile(std::string);
    
    //Read the values for the temperature loading 
    //(equivalent to read TK1 and set time and temperature attributes)
    void ReadTemperatureLoading(std::string);
    
    //Read Vacancies concentration after hardening (maturation or tempering)
    void ReadVCAH(std::string);
    
    //Getter
    double GetDuration() const {return duration_;};
    
    //Setter
    void SetDuration(const double &duration);


protected:

private:

    //Thermal loading duration (was tmax - tmat). Unit: s
    double duration_;
    
    //Time vector of temperature loading. Unit: s
    double * time_;
    
    //Temperature vector of ThermalLoading. Unit: K
    double * temperature_;

};

inline void
ThermalLoading::SetDuration(const double &duration)
{
  duration_=duration;
}

#endif
