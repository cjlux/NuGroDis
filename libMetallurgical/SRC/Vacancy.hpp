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


#ifndef __Vacancy__hpp__
#define __Vacancy__hpp__

#include <string>
#include <vector>

class Material; 
class ChemicalElement;

class Vacancy
{

public:

    //The constructor
    Vacancy(double deltaHF,double deltaSF,double deltaHM,double fE,double Dlac0,double halfSinkD,double Tf, Material &mat);
    
    ~Vacancy();
    
    void Info() const;
    
    void SetHalfSinkDistance(const double &);
    void SetJumpFrequency(const double &);
    void SetVacCreationEnthalpy(const double &);
    void SetVacCreationEntropy(const double &);
    void SetConcentration(const double &);
    void SetMigrationEnthalpy(const double &);
    void SetSolutionisingTemp(const double &);
    void SetPreExpDiffusionValue(const double &);
    
    double GetHalfSinkDistance() const;
    double GetJumpFrequency() const;
    double GetVacCreationEnthalpy() const;
    double GetVacCreationEntropy() const;
    double GetConcentration() const;
    double GetMigrationEnthalpy() const;
    double GetSolutionisingTemp() const;
    double GetPreExpDiffusionValue() const;
    
    //Compute and save equilibrium concentration of vacancies after solutionising. Was Xlacavtremp.  To do: check validity of the equation
    void ComputeConcentrationBeforeQuenching();
    
    //get equilibrium concentration of vacancies after solutionising.  was Xlacavtremp
    double GetConcentrationBeforeQuenching() const ;
    
    //save the concentration (atomic fraction or molar fraction)  of vacancies at the end of the quenching. (was at the end of quenching, was Xlacfinmat). Unit: Vacancy nb/atoms nb
    void SaveQuenchingFinalConcentration(std::string);
    
    //get the concentration (atomic fraction) of vacancies at the end of the quenching. (was at the end of quenching)
    double GetQuenchingFinalConcentration(std::string);
    
    //get the concentration (atomic fraction) of vacancies at the end of the hardening. 
    double GetHardeningFinalConcentration(std::string);
    
    // Compute and save the evolution of vacancies based on analytical solution of the differential equation 
    double SolveConcentrationEvolutionEquation();
    
    void SetConcentrationBeforeQuenching(const double &);
    
    //Set the vacancy concentration  value after hardening  (maturation or tempering). Set XlacFinmat. Last Current value of concentration_ can be  Concentratrion  After hardening.
    void SetConcentrationAfterHardening(double &);
    
    
    
    
   //RELATIONS
   //getter
   const Material& GetMaterial() const {return material_;};
   std::vector<const ChemicalElement*> GetSoluteInteractingWithVacList() const {return soluteInteractingWithVacList_;};
   //setter
   void AddInteractingSolute(const ChemicalElement& solute);
   
   
protected:

private:

    //Free Energy(Was DeltaHF). Enthalpy of vacancy formation. Unit: J/mol
    double vacCreationEnthalpy_;
    //Free Energy(Was DeltaSF). Entropy of vacancy formation. Unit: J/mol/K
    double vacCreationEntropy_;
    //save current concentration value. Last Current value can be  Concentratrion  After hardening. 
    double concentration_;
    //The value of vacancies concentration before quenching (was Xlacavtrempe) .Unit: number of vacancies/m^3
    double concentrationBeforeQuenching_;
    //Semi Distance between vacancy sinks (was l) .Unit: m
    double halfSinkDistance_;
    //Frequency of vacancy's jumps (was fE). Unit: s^(-1)
    double jumpFrequency_;
    //Preexponential term of vacancy diffusion expression(was Dlac0). Unit: m^2/s
    double preExpDiffusionValue_;
    //Enthalpy of vacancy migration (Was DeltaHM). Unit: J/mol
    double migrationEnthalpy_;
    //Solutionising temperature: temperature at which the metal is heated before quenching (was Tf). Unit: K
    double solutionisingTemp_;
    
    
    //RELATIONS
    const Material& material_;
    std::vector<const ChemicalElement*> soluteInteractingWithVacList_;

};



inline void
Vacancy::SetConcentrationAfterHardening(double &C)
{
}

//Setters
inline void
Vacancy::SetConcentrationBeforeQuenching(const double &cBQ)
{
  concentrationBeforeQuenching_=cBQ;
}

inline void
Vacancy::SetHalfSinkDistance(const double &hSD)
{
  halfSinkDistance_=hSD;
}

inline void
Vacancy::SetJumpFrequency(const double &jF)
{
  jumpFrequency_=jF;
}

inline void
Vacancy::SetVacCreationEnthalpy(const double &vCEnthalpy)
{
  vacCreationEnthalpy_=vCEnthalpy;
}

inline void
Vacancy::SetVacCreationEntropy(const double &vCEntropy)
{
  vacCreationEntropy_=vCEntropy;
}

inline void
Vacancy::SetConcentration(const double &C)
{
  concentration_=C;
}



inline void
Vacancy::SetMigrationEnthalpy(const double &migrationEnth)
{
  migrationEnthalpy_=migrationEnth;
}

inline void
Vacancy::SetSolutionisingTemp(const double &solT)
{
  solutionisingTemp_=solT;
}

inline void
Vacancy::SetPreExpDiffusionValue(const double &pEDV)
{
 preExpDiffusionValue_ =pEDV;
}



//Getters
inline double 
Vacancy::GetHalfSinkDistance() const
{
  return halfSinkDistance_;
}

inline double 
Vacancy::GetJumpFrequency() const
{
  return jumpFrequency_;
}

inline double 
Vacancy::GetVacCreationEnthalpy() const
{
  return vacCreationEnthalpy_;
}

inline double 
Vacancy::GetVacCreationEntropy() const
{
  return vacCreationEntropy_;
}

inline double 
Vacancy::GetConcentration() const
{
  return concentration_;
}


inline double 
Vacancy::GetSolutionisingTemp() const
{
  return solutionisingTemp_;
}

inline double 
Vacancy::GetMigrationEnthalpy() const
{
  return migrationEnthalpy_;
}

inline double 
Vacancy::GetPreExpDiffusionValue() const
{
  return preExpDiffusionValue_;
}


//RELATIONS
//Setter
inline void 
Vacancy::AddInteractingSolute(const ChemicalElement& solute)
{
  soluteInteractingWithVacList_.push_back(&solute);
}

#endif

