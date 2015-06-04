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


#ifndef __Polynomial__hpp__
#define __Polynomial__hpp__
#include <boost/python.hpp>

#include <vector>


class Polynomial
{

public:

    //the constructor takes 2 arguments: degree and coefficients to build the polynomial
    Polynomial(int degree=-1);
    
    ~Polynomial();
    
    void Info() ;
    
    
    
    //Gives P(x)
    void ComputeValue(double x);
    
    //Getters
    const std::vector<double> GetCoefsList() const {return coefsList_;};
    const int GetDegree() const {return degree_;};
    
    //Setters
    void SetDegree(int deg);
    void SetCoefs(std::vector<double> const& coefsVector);
    static void PrintVectorDouble(std::vector<double>& vec);
    
    //Debug
    static void PrintVectorPolynomial(std::vector<Polynomial*>& vec);
    
    //Add coefs from a pythonList
    void AddPyCoefs(boost::python::list& pythonCoefList);

protected:

private:
    int degree_;
    std::vector<double> coefsList_;

};

inline
void 
Polynomial::SetDegree(int deg)
{
  assert (degree_==-1);
  degree_=deg;
  
   for (int i=0;i<=degree_;i++)
  {
    coefsList_.push_back(0);
  }
}
#endif
