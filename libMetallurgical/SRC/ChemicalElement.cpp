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

