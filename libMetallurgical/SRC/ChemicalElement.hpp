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
#include <string>

class ChemicalElement
{

public:
    ChemicalElement(double density, double molarMass, double yModulus, double poissonCoef, std::string elementName);
    ~ChemicalElement();
    
    void Info() const;
    void ConvertMassicToVolumicConcentration();
    void ConvertVolumicToMassicConcentration();
    void ConvertAtomicToVolumicConcentration();
    void ConvertVolumicToAtomicConcentration();
    void ConvertStoechiometricCoefficient2VolumicConcentration();
    double ComputeInterFacialConcentration(double diameter, double temperature);
    double GetMolarMass()          const;
    double GetDensity()            const {return density_; };
    double GetPoissonCoefficient() const {return poissonCoefficient_; };
    double GetYoungModulus()       const {return youngModulus_; };
    std::string GetElementName()   const {return elementName_; };
    double GetEquilibriumConc()    const {return equilibriumConcentration_; };
    double GetInterfacialConc()    const {return equilibriumConcentration_; };
    void SetEquilibriumConc();
    void SetInterfacialConc();


protected:

private:
    const double      density_;
    const double      molarMass_;
    const double      youngModulus_;
    const double      poissonCoefficient_;
    const std::string elementName_;
    double            equilibriumConcentration_;
    double            interFacialConcentration_;
    
};

inline double
ChemicalElement::GetMolarMass() const
{
    assert(molarMass_ > 0);
    return molarMass_;
}

//Setters
inline void
ChemicalElement::SetEquilibriumConc()
{
  
}

inline void
ChemicalElement::SetInterfacialConc()
{
  
}
