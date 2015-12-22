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

#include <cassert>

class ChemicalElement;
class ChemicalComposition;

class Concentration
{

public:

    //The conctructor, taking as optional argument the initial  volumic concentration value
    Concentration(const ChemicalElement&,const ChemicalComposition& );
     
    ~Concentration();
    
    
    void Info() const;
    
    //Compute and NOT SET the atomic concentration from  current volumic value. 
    //Because current atomic concentration is not stored , it is computed using chemicalElement::ConvertVolumicToAtomicConcentration
    void ReturnCurrentAtomicValue();
    
    // Compute NOT SET massic from the current volumic value  Because current amassic concentration is not 
    // stored , it is computed using chemicalElement::ConvertVolumicToAtMassicncentration
    void ReturnCurrentMassicValue();
    
    
    
    
    
    //setters
    //Set Initial atomic value
    void SetInitialAtomicValue(const double initialAtomicValue);
    //Set Initial massic value
    void SetInitialMassicValue(const double initialMassicValue);
    //Set Initial volumic value: especially for initial Chemical Compo
    void SetInitialVolumicValue(const double initialMassicValue);
    //Set value of volumic concentration
    void SetVolumicValue(const double volumicvalue);
    void SetVolumicValueAndcheckIfSettedValueIsPositive(const double volumicvalue, bool checkIfSettedValueIsPositive = true);
    void SetStoichiometricCoef(const int stoiCoef);
    
    
    //getters
    //This method return the value of the volumic concentration
    double GetVolumicValue() const;
    double GetInitialAtomicValue() const;
    double GetInitialMassicValue() const;
    double GetInitialVolumicValue() const;
    const int GetStoichiometricCoef() const;
    bool GetInitialAtomicValueHasBeenSet() const {return initialAtomicValueHasBeenSet_;};
    bool GetInitialMassicValueHasBeenSet() const {return initialMassicValueHasBeenSet_;};
    bool GetInitialVolumicValueHasBeenSet() const {return initialVolumicValueHasBeenSet_;};
    
    
    
    
    //RELATIONS
    //getter
    const ChemicalElement& GetChemicalElement() const {return chemicalElement_; };
    const ChemicalComposition& GetChemicalComposition() const {return chemicalComposition_;};


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
    
    
    
    int stoichiometricCoef_;
    
    bool initialAtomicValueHasBeenSet_;
    bool initialMassicValueHasBeenSet_;
    
    double initialVolumicValue_;
    bool initialVolumicValueHasBeenSet_;
    
    
    
    //TODO double equilibriumConcentration_;  //Equilibrium concentration between the solid solution and a planar precipitate.(volumic %)
				              //Depends on temperature
					      //Symbol : X_eq_Cu_SS, X_eq_Mg_SS...
					      //Unit : volumic percentage
    
 

};



//SETTERS
inline void
Concentration::SetInitialMassicValue(const double initialMassicValue)
{
  assert ( (initialMassicValueHasBeenSet_==false)&&"Cannot Set Initial Massic Value because it has already been set " );
  initialMassicValue_=initialMassicValue;
  initialMassicValueHasBeenSet_=true;
}

inline void
Concentration::SetInitialAtomicValue(const double initialAtomicValue)
{
  assert ( (initialAtomicValueHasBeenSet_==false)&&"Cannot Set Initial Atomic Value because it has already been set " );
  initialAtomicValue_=initialAtomicValue;
  initialAtomicValueHasBeenSet_=true;
}


inline void
Concentration::SetInitialVolumicValue(const double initialVolumicValue)
{
  assert ( (initialVolumicValueHasBeenSet_==false)&&"Cannot Set Initial Volumic Value because it has already been set " );
  initialVolumicValue_=initialVolumicValue;
  initialVolumicValueHasBeenSet_=true;
 
  assert ( (initialVolumicValue_>=0)&&"Value of Initial Volumic Concentration is negative. Cannot set initial volumic value!" );
}


inline void
Concentration::SetVolumicValue(const double volumicValue)
{
  if (initialVolumicValueHasBeenSet_==false)
  {
    this->SetInitialVolumicValue(volumicValue);
  }
  
  // /*DEBUG*/std::cout<<"DEBUG: element name is "<<chemicalElement_.GetElementName()<<"\n";

    std::cout<<" NOTIFICATION:  A Positive value of Setted volumic concentration has been asserted.";
    assert ( (volumicValue>=0)&&"Value of Volumic Concentration is negative. Cannot set volumic value!" );
  
  volumicValue_=volumicValue;
}








inline void
Concentration::SetVolumicValueAndcheckIfSettedValueIsPositive(const double volumicValue, bool checkIfSettedValueIsPositive)
{
  
  // Remember : checkIfSettedValueIsPositive default value is true !!!
  
  
  // /*DEBUG*/std::cout<<"DEBUG: element name is "<<chemicalElement_.GetElementName()<<"\n";
  
  if (checkIfSettedValueIsPositive==true )
  {
    std::cout<<" NOTIFICATION:  A Positive value of Setted volumic concentration is being asserted.";
    assert ( (volumicValue>=0)&&"Value of Volumic Concentration is negative. Cannot set volumic value!" );
  }
  else
  {
    std::cout<<" NOTIFICATION:  Positive value of Setted volumic concentration has not been asserted because checkIfSettedValueIsPositive=false";
  }
  
  volumicValue_=volumicValue;
}






//GETTERS
inline double
Concentration::GetInitialAtomicValue() const
{
  assert(  (initialAtomicValue_!=-1)&&"Cannot Get initialAtomicValue because it is equal to -1 \
  and probably has not been setted!!!"  );
  return initialAtomicValue_;
}

inline double
Concentration::GetInitialMassicValue() const
{
  assert(  (initialMassicValue_!=-1)&&"Cannot Get InitialAtomic. Value because it is equal to -1 \
  and probably has not been setted!!!"  );
  return initialMassicValue_;
}

inline double
Concentration::GetInitialVolumicValue() const
{
  assert(  (initialVolumicValue_!=-1)&&"Cannot Get Initial Volumic Value because it is equal to -1 \
  and probably has not been setted!!!"  );
  return initialVolumicValue_;
}



inline double
Concentration::GetVolumicValue() const
{
  return volumicValue_;
}







#endif
