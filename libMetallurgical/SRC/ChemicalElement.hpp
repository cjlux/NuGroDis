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

#include <cassert>

class ChemicalElement
{

public:
    ChemicalElement();
    ~ChemicalElement();
    
    void Info() const;
    void ConvertMassicToVolumicConcentration()
    void ConvertVolumicToMassicConcentration()
    void ConvertAtomicToVolumicConcentration()
    void ConvertVolumicToAtomicConcentration()
    void ConvertStoechiometricCoefficient2VolumicConcentration()
    double ComputeInterFacialConcentration(double diameter, double temperature);
    double GetMolarMass() const;


protected:

private:
    const double density_;
    const double poissonCoefficient_;
    const double youngModulus_;
    double equilibriumConcentration_;
    double interFacialConcentration_;
    double molarMass_;
};

inline double
ChemicalElement::GetMolarMass() const
{
    assert(molarMass_ > 0);
    return molarMass_;
}