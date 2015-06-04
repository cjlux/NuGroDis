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
  for (int i=0;i<=degree_;i++)
  {
    coefsList_.push_back(0);
  }
}

Polynomial::~Polynomial()
{
}


void 
Polynomial::AddPyCoefs(boost::python::list& pythonCoefList)
{
  //Debug
  /*std::cout<<std::abs(coefsList_.size())<<std::endl;
  std::cout<<boost::python::len(pythonCoefList)<<std::endl;*/
 
  assert( ( std::abs(coefsList_.size())==boost::python::len(pythonCoefList) ) &&"Cannot set Polynome coefficients! Size of given argument python List does not match with degree of the polynome ");
  
  //Step1: erase initialization values. 0 values
   for (int i=0;i<=degree_;i++)
  {
    coefsList_.pop_back();
  }
  
  //step 2: set new values given in argument
  for (int i = 0; i <= len(pythonCoefList)-1; ++i)
  {
    coefsList_.push_back(boost::python::extract<double>(pythonCoefList[i]));
  }
}

void
Polynomial::PrintVectorDouble(std::vector<double>& vec)
{
  for( std::vector<double>::const_iterator i = vec.begin(); i != vec.end(); ++i)
    std::cout << *i << '|';
}

//DEBUG
void
Polynomial::PrintVectorPolynomial(std::vector<Polynomial*>& vec)
{
  for( std::vector<Polynomial*>::const_iterator i = vec.begin(); i != vec.end(); ++i)
    std::cout << *i << '|';
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


void
Polynomial::Info()
{
  std::cout <<  "##########################  Polynomial::Info ############################"<< std::endl;
  std::cout <<  "                                degree: " << degree_ << " SI unit" << std::endl;
  std::cout <<  "                             coefsList: ";
  Polynomial::PrintVectorDouble(coefsList_);
  std::cout << " SI unit" << std::endl;

  
}