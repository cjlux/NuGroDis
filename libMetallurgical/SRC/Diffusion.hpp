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

class Diffusion
{

public:
    Diffusion();
    ~Diffusion();
    
    void Info() const;
    
    //Computes the diffusion coefficient for the given value of the temperature . Unit: K
    void ComputeCoefValue();

protected:

private:
    
    //Initial diffusion value. unit: m^2/s
    const double preExpDiffusionCoef_;
    
    //Activation energy of diffusion for a pair of chemical elements. Unit: J/mol
    const double activationEnergy_;
    

};