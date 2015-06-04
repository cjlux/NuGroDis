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

#ifndef __Boundary__hpp__
#define __Boundary__hpp__

#include <vector>

#include <boost/python.hpp>

class Boundary
{

public:
    Boundary();
    ~Boundary();
    
    void Info() const;
    
//template<class T>
static boost::python::list vectorToPyList(const std::vector<double>& v);

protected:

private:
    double volNbGP_;
    double volNbSprime_;
    int volNbPrecipitates_;
  

};

#endif
