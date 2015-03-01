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

#include "ChemicalElement.hpp"

ChemicalElement::ChemicalElement()
{
}

ChemicalElement::~ChemicalElement()
{
}

double 
ChemicalElement::ComputeInterFacialConcentration(double diameter, double temperature)
{
    // Preconsitions
    assert(diameter >0 && temperature > 0);
    
    //....
    
    
    // Check:
    //assert( quelquechose dont je suis certain)
    
    // Postconditions
    assert(interFacialConcentration_ > 0);
    
    return interFacialConcentration_;
}

void
ChemicalElement::ConvertMassicToVolumicConcentration()
{
}

void
ChemicalElement::ConvertVolumicToMassicConcentration()
{
}

void
ChemicalElement::ConvertAtomicToVolumicConcentration()
{
}

void
ChemicalElement::ConvertVolumicToAtomicConcentration()
{
}

void
ChemicalElement::ConvertStoechiometricCoefficient2VolumicConcentration()
{
}

