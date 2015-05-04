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

#include "Polynomial.hpp"


Polynomial::Polynomial(int degree)
  :degree_(degree),
   coefsList_()
{
  for (int i=0;i<degree_;i++)
  {
    coefsList_.push_back(0);
  }
}

Polynomial::~Polynomial()
{
}

void
Polynomial::ComputeValue(double x)
{
}

void
Polynomial::SetCoefs(std::vector<double> const& coefsVector)
{
  assert( ( coefsList_.size()==coefsVector.size() ) &&"Cannot set Polynome coefficients! Size of given argument Vector does not match with the size of coefsList_" );
  
  coefsList_=coefsVector;
}