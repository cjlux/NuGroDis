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

#include <vector>

class Polynomial
{

public:

    //the constructor takes 2 arguments: degree and coefficients to build the polynomial
    Polynomial(int degree);
    
    ~Polynomial();
    
    void Info() const;
    
    //Gives P(x)
    void ComputeValue(double x);
    
    //Getters
    const std::vector<double> GetCoefsList() const {return coefsList_;};
    const int GetDegree() const {return degree_;};
    
    //Setters
    void SetCoefs(std::vector<double> const& coefsVector);


protected:

private:
    const int degree_;
    std::vector<double> coefsList_;

};

#endif
