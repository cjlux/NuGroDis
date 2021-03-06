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


#include "Boundary.hpp"
#include <vector>


Boundary::Boundary()
{ 
}

Boundary::~Boundary()
{
}


//template<class T>
boost::python::list vectorToPyList(const std::vector<double>& v)
{
    boost::python::object get_iter = boost::python::iterator<std::vector<double> >();
    boost::python::object iter = get_iter(v);
    boost::python::list l(iter);
    return l;
}