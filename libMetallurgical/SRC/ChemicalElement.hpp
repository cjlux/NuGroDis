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


#ifndef __ChemicalElement__hpp__
#define __ChemicalElement__hpp__


#include <cassert>
#include <string>
#include <vector>

class ChemicalComposition;
class Diffusion;

class ChemicalElement
{

public:
    ChemicalElement(double density, 
		    double molarMass, 
		    double YModulus, 
		    double PoissonCoef, 
		    std::string elementName,
		    double latticeParameter = -1);
    ~ChemicalElement();
    
    void Info() const;
    void ComputeAtomicDiffusionCoefValue() const;
    
    void ConvertMassicToVolumicConcentration(ChemicalComposition& CC);
    void ConvertVolumicToMassicConcentration(ChemicalComposition& CC);
    void ConvertAtomicToVolumicConcentration(ChemicalComposition& CC);
    void ConvertVolumicToAtomicConcentration(ChemicalComposition& CC);
    void ConvertStoechiometricCoefficientToVolumicConcentration(ChemicalComposition& CC);
    void ConvertStoechiometricCoefficientToAtomicConcentration(ChemicalComposition& CC);

    //getters
    double GetMolarMass()          const;
    const double GetDensity()            const {return density_; }
    const double GetPoissonCoefficient() const {return PoissonCoefficient_; }
    const double GetYoungModulus()       const {return YoungModulus_; }
    std::string GetElementName()   const {return elementName_; }
    const double  GetLatticeParameter() const { return latticeParameter_; }
        
    //setters
    void SetLatticeParameter(double x) { assert(x > 0); latticeParameter_ = x; }
    
    //RELATION
    //getters
    void EnterInChemicalComposition(ChemicalComposition& compo);//Enter a CE to chemicalCompositionList
    std::vector<ChemicalComposition *> GetChemicalComposition() const {return chemicalCompositionList_;};
    const Diffusion& GetDiffusion() const ; 
    //setters
    void SetDiffusion(Diffusion &diffusionObj);
    
    //debug
    //bool operator==(const ChemicalElement& CE1){return etElementName_==CE1.GetElementName() ;};
    

protected:

private:
    const double      density_;
    const double      molarMass_;
    const double      YoungModulus_;
    const double      PoissonCoefficient_;
    double            latticeParameter_;
    const std::string elementName_; /*WARNING Must be unique!!! 2 Objects ChemicalElement
				     *can not have the same value of attribute elementName_*/
    
    //RELATIONS
    std::vector<ChemicalComposition *> chemicalCompositionList_;
    Diffusion* diffusion_;
    /* Material or ssgrain in which the ChemicalElement diffuse is known thanks to diffusion_->GetMaterial() */
    
   /*WARNING: A ChemicalElement do not need to know the RadiusDistribution or InterfacialConcentration
    * it is linked with. Only RadiusDitribution and InterfacialConcentration must necessary know
    * the  ChemicalElement they are linked with. 
    */
  
};

inline const Diffusion&
ChemicalElement::GetDiffusion() const
{
  assert( (diffusion_!=0)&&"In GetDiffusion: cannot get diffusion because the chemical\
  element does not have any diffusion object yet ( pointer is 0 )" );
  return *diffusion_;
}

    

inline double
ChemicalElement::GetMolarMass() const
{
    assert(molarMass_ > 0);
    return molarMass_;
}

//Setters

//RELATIONS
//setter
inline void
ChemicalElement::SetDiffusion(Diffusion &diffusionObj)
{
  diffusion_=&diffusionObj;
}


#endif
