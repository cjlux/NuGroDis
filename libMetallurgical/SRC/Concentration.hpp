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

#ifndef __Concentration__hpp__
#define __Concentration__hpp__

class ChemicalElement;
class ChemicalComposition;

class Concentration
{

public:

    //The conctructor, taking as optional argument the initial  volumic concentration value
    Concentration(const ChemicalElement&,const ChemicalComposition& );
     
    ~Concentration();
    
    
    void Info() const;
    
    //Compute the atomic concentration from  current volumic value. 
    //Because current atomic concentration is not stored , it is computed using chemicalElement::ConvertVolumicToAtomicConcentration
    void ComputeAtomicValue();
    
    // Compute massic from the current volumic value  Because current amassic concentration is not 
    // stored , it is computed using chemicalElement::ConvertVolumicToAtMassicncentration
    void ComputeMassicValue();
    
    //Set Initial atomic value
    void SetInitialAtomicValue();
    
    //Set Initial massic value
    void SetInitialMassicValue();
    
    //Set value of volumic concentration
    void SetVolumicValue();
    
    //This method return the value of the volumic concentration
    double GetVolumicValue() const;
    
    double GetInitialAtomicValue() const;
    
    double GetInitialMassicValue() const;
    
    //RELATIONS
    //getter
    const ChemicalElement& GetChemicalElement() {return chemicalElement_; };
    const ChemicalComposition& GetChemicalComposition() {return chemicalComposition_;};


protected:

private:
  
     //Relations
    const ChemicalElement & chemicalElement_;
    const ChemicalComposition & chemicalComposition_;
    
    
     //value of the current volumic concentration of the chemical element. Unit : ratio in the range[ 0, 1]. May be computed from massic concentration....
    double volumicValue_;

    //The initial atomic concentration of the chemical element. Unit: ratio in the range [0,1]
    double initialAtomicValue_; 
    
    //The initial massic concentration of the chemical element. Unit: ratio in the range [0,1]
    double initialMassicValue_;
    
    //TODO new double[NbElements][NbElement] stoechiometricCoef_;
    
    //TODO double equilibriumConcentration_;  //Equilibrium concentration between the solid solution and a planar precipitate.(volumic %)
				              //Depends on temperature
					      //Symbol : X_eq_Cu_SS, X_eq_Mg_SS...
					      //Unit : volumic percentage
    
 

};

inline void
Concentration::SetInitialMassicValue()
{
}

inline void
Concentration::SetInitialAtomicValue()
{
}

inline void
Concentration::SetVolumicValue()
{
}

inline double
Concentration::GetInitialAtomicValue() const
{
  return initialAtomicValue_;
}

inline double
Concentration::GetInitialMassicValue() const
{
  return initialMassicValue_;
}

inline double
Concentration::GetVolumicValue() const
{
  return 0;
}

#endif
