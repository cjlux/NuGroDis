#include "InterfacialConcentration.hpp"

InterfacialConcentration::InterfacialConcentration()
  :InterfacialConcentration_(-1)
{
}

InterfacialConcentration::~InterfacialConcentration()
{
}

double 
InterfacialConcentration::ComputeInterFacialConcentration(double diameter, double temperature)
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
