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

#include <string>

class SSGrain
{

public:
    SSGrain();
    ~SSGrain();
    
    void Info() const;
    
    //Compute the molar volume of (precipitates, solid solution,...).The result is written in molar volume[m^3/mol] (was VmP1,VmP2). Unit: m^3/mol
    void ComputeMolarVolume();
    
    //Read Specific data value for the given Alloy. Computes the atomic concentrtaions for the chemical elements in the solid Solution
    void ReadDataValue(std::string);

protected:

private:

    //The lattice parameter of the main ChemicalElement. Unit: m
    const double latticeParameter_;
    
    //The mean diameter of the grain, or of the bounding sphere of the grain. Unit : m
    double meanDiameter_;
    
    // Volume of one mol of precipitate or  one mol af atoms of SSGrain. Unit : m^3/mol  :    m^3/atomMol (SSGrain) or m^3/precipitatesMol (Precipitates)
    double molarVolume_;
    
    //The Poisson coefficient of the Grain. If not given, can be approximated with the Poisson coefficient of the main chemical element of the grain. Unit: no unit
    const double poissonCoeff_;
    
    //volumic number of GP precipitates. Unit: number/m^3
    int volNbGP_;
    
    //volumic number of Sprime precipitates. Unit: number/m^3
    int volNbSprime_;
    
    //Volumic number of precipitates in the grain. Unit: number/m^3
    int volNbPrecipitates_;
    
    //The Young modulus of the Grain. If not given, can be approximated with the Young modulus of the main chemical element of the grain. Unit: MPa
    const double youngModulus_;

};