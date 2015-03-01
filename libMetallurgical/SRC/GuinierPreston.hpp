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

class GuinierPreston
{

public:
    GuinierPreston();
    ~GuinierPreston();
    
    //Compute the number of nucleation sites using RadiusDistribution::ComputeTotNbOfItems
    void ComputeNucleationSiteNb();
    
    //Compute the volumic fraction
    void ComputeVolumicFraction();
    
    void ReadDataValue(std::string fileName);


protected:

private:

};