#include <cassert>

class ChemicalElement
{

public:
    ChemicalElement();
    ~ChemicalElement();
    
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