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

#ifndef __Diffusion__hpp__
#define __Diffusion__hpp__

#include <cassert>

class Material;
class ChemicalElement;
class SSGrain;
class Vacancy;


class Diffusion
{

public:

  Diffusion(ChemicalElement& CE, Material& mat,double preExpDiffCoef,double actEn, double Evac);
  Diffusion(ChemicalElement& CE, Material& mat,double preExpDiffCoef,double actEn);
  /*TODO ERASE THIS CONSTRUCTOR Diffusion(ChemicalElement& CE,Vacancy& vacancy, double Evac);*/
    ~Diffusion();
    
    void Info() const;
    
    //Computes the atomic diffusion coefficient for the current value of the temperature . Unit: K
    void ComputeAtomicDiffusionCoefValue();
    
    //getter
    const double GetPreExpDiffusionCoef() {return preExpDiffusionCoef_;};
    const double GetActivationEnergy() {return activationEnergy_;};
    const double GetInteractionEnergyWithVacancy() const;
    const bool AssertInteractionEnergyWithVacancyValue(double valueAsserted) const;
    
    double GetAtomicDiffusionCoef() const ;
    
    //fake attribute: debug
    const double GetToto() {return toto;};
    void SetToto(const double pouet) {toto=pouet;};
    
    
    //RELATION
    //getter
    const Material& GetMaterial() const ;//TODO check if 0 or not
    const Vacancy& GetVacancy() const;
    const ChemicalElement& GetChemicalElement() const {return chemicalElement_;};
    SSGrain* GetSSGrainPointer() const ;
    SSGrain& GetSSGrain() const;
    
protected:

private:
    
    //Initial diffusion value or diffusion prexponential term for an atom. (was D0_i_SS) unit: m^2/s
    const double preExpDiffusionCoef_;
    
    //Activation energy of diffusion for a pair of chemical elements.(was Q) Unit: J/mol
    const double activationEnergy_;
    
    //Interaction energy between Chemical Element and vacancies.(was Evac) Unit: J
    const double interactionEnergyWithVacancy_;
    
    //was(D_i_SS)
    double atomicDiffusionCoef_;
    
    //Association relation
    const ChemicalElement& chemicalElement_;
    const Material* material_;
    SSGrain* ssgrainPointer_;
    Vacancy* vacancy_;
    

    
    //fake attribute:debug
    double toto;
   
};


inline const double 
Diffusion::GetInteractionEnergyWithVacancy() const
{
  assert( (interactionEnergyWithVacancy_!=-2)&&"In GetInteractionEnergyWithVacancy(): Cannot get interaction energy\
  because Atomic Diffusion Object has been built with no vacancy diffusion and only atomic diffusion "  );
  return interactionEnergyWithVacancy_;
}

inline const bool 
Diffusion::AssertInteractionEnergyWithVacancyValue(double valueAsserted) const
{
  /*DEBUG*/std::cout<<"value asserted is "<<valueAsserted<<" interactionEnergyWithVacancy_ is "<<interactionEnergyWithVacancy_<<std::endl;
  return interactionEnergyWithVacancy_==valueAsserted;
}


inline double 
Diffusion::GetAtomicDiffusionCoef() const 
{
  assert (  (atomicDiffusionCoef_!=-2)&&"Cannot GetAtomicDiffusionCoef because\
  Diffusion Object do not have Atomic diffusion (Use another constructor) ");
  
  assert (  (atomicDiffusionCoef_!=0)&&"Atomic diffusion coefficient is 0!!! No sense Value.\
  Cannot GetAtomicDiffusionCoef()");
  
  assert (  (atomicDiffusionCoef_!=-1)&&"In Diffusion::GetAtomicDiffusionCoef(): Atomic diffusion coefficient has not been computed yet!!! ");
  
  
  
  return atomicDiffusionCoef_;
}




inline SSGrain*
Diffusion::GetSSGrainPointer() const
{
  return ssgrainPointer_;
}

inline SSGrain& 
Diffusion::GetSSGrain() const
{
  if (ssgrainPointer_!=0)
  {
  return (*ssgrainPointer_);
  }
  else
  {
   return *ssgrainPointer_;
  };
}

#endif
