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


#ifndef __Temperature__hpp__
#define __Temperature__hpp__

#include <vector>
#include <boost/python.hpp>

//Class Temperature, defining the current temperature for computing, 
//Can be an array of Value versus time
//Unit: K

class Temperature
{

public:
    Temperature(double T=293.15); //T=293.15 K,   Standard conditions for temperature and pressure
    ~Temperature();
    
    void Info() const;

    //Getters
    double GetCurrentTemp() const {return currentTemp_; };

    std::vector<double> & GetTemperatureList() {return temperatureList_; };
    const std::vector<double> & GetTemperatureList() const {return temperatureList_; };
    boost::python::list GetTemperaturePyList();
    
    //Setters
    void SetCurrentTemp(double temp);
    
    


   
protected:

private:
  double currentTemp_;
  std::vector<double> temperatureList_;
};


#endif
