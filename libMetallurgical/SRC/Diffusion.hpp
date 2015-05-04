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

class Material;
class ChemicalElement;
class SSGrain;
class Vacancy;


class Diffusion
{

public:

  Diffusion(ChemicalElement& CE, Material& mat,Vacancy& vacancy,double preExpDiffCoef,double actEn, double Evac);
  Diffusion(ChemicalElement& CE, Material& mat,double preExpDiffCoef,double actEn);
  Diffusion(ChemicalElement& CE,Vacancy& vacancy, double Evac);
    ~Diffusion();
    
    void Info() const;
    
    //Computes the diffusion coefficient for the given value of the temperature . Unit: K
    void ComputeCoefValue();
    
    //getter
    const double GetPreExpDiffusionCoef() {return preExpDiffusionCoef_;};
    const double GetActivationEnergy() {return activationEnergy_;};
    const double GetInteractionEnergyWithVacancy() {return interactionEnergyWithVacancy_;};
    
    //RELATION
    //getter
    const Material& GetMaterial() const ;//TODO check if 0 or not
    const Vacancy& GetVacancy() const;
    const ChemicalElement& GetChemicalElement() const {return chemicalElement_;};
    SSGrain* GetSSGrain() const ;
    
protected:

private:
    
    //Initial diffusion value. unit: m^2/s
    const double preExpDiffusionCoef_;
    
    //Activation energy of diffusion for a pair of chemical elements. Unit: J/mol
    const double activationEnergy_;
    
    //Interaction energy between Chemical Element and vacancies. Unit: J
    const double interactionEnergyWithVacancy_;
    
    //Association relation
    const ChemicalElement& chemicalElement_;
    const Material* const material_;
    SSGrain* ssgrain_;
    const Vacancy* const vacancy_;
    

};
inline SSGrain*
Diffusion::GetSSGrain() const
{
  return ssgrain_;
}

#endif
