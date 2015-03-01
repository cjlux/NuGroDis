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

#include <string>

class Vacancy
{

public:

    //The constructor
    Vacancy(double deltaHF,double deltaSF,double deltaHM,double fE,double Dlac0,double halfSinkD,double Tf,double EVacCu,double EVacMg);
    
    ~Vacancy();
    
    //Compute and save equilibrium concentration of vacancies after solutionising. Was Xlacavtremp.  To do: check validity of the equation
    void ComputeConcentrationBeforeQuenching();
    
    //get equilibrium concentration of vacancies after solutionising.  was Xlacavtremp
    double GetConcentrationBeforeQuenching();
    
    //save the concentration (atomic fraction or molar fraction)  of vacancies at the end of the quenching. (was at the end of quenching, was Xlacfinmat). Unit: Vacancy nb/atoms nb
    void SaveQuenchingFinalConcentration(std::string);
    
    //get the concentration (atomic fraction) of vacancies at the end of the quenching. (was at the end of quenching)
    double GetQuenchingFinalConcentration(std::string);
    
    //get the concentration (atomic fraction) of vacancies at the end of the hardening. 
    double GetHardeningFinalConcentration(std::string);
    
    // Compute and save the evolution of vacancies based on analytical solution of the differential equation 
    double SolveConcentrationEvolutionEquation();
    
    void SetConcentrationBeforeQuenching(double);
    
    //Set the vacancy concentration  value after hardening  (maturation or tempering). Set XlacFinmat
    void SetConcentrationAfterHardening(double);

protected:

private:

    //Free Energy(Was DeltaHF). Enthalpy of vacancy formation. Unit: J/mol
    const double vacCreationEnthalpy_;
    
    //Free Energy(Was DeltaSF). Entropy of vacancy formation. Unit: J/mol/K
    const double vacCreationEntropy_;
    
    //save current concentration value. Last Current value can be  Concentratrion  After hardening. 
    double concentration_;
    
    //The value of vacancies concentration before quenching (was Xlacavtrempe) .Unit: number of vacancies/m^3
    const double concentrationBeforeQuenching_;
    
    //Semi Distance between vacancy sinks (was l) .Unit: m
    const double halfSinkDistance_;
    
    //Frequency of vacancy's jumps (was fE). Unit: s^(-1)
    const double jumpFrequency_;
    
    
    //Preexponential term of vacancy diffusion expression(was Dlac0). Unit: m^2/s
    const double preExpDiffusionValue_;
    
    //Interaction energy between magnesium and vacancies. Unit: J
    const double magnesiumVacInteractionEnergy_;
    
    //Enthalpy of vacancy migration (Was DeltaHM). Unit: J/mol
    const double migrationEnthalpy_;
    
    //Solutionising temperature: temperature at which the metal is heated before quenching (was Tf). Unit: K
    const double solutionisingTemp_;
    
    //Interaction energy between copper and vacancies. Unit: J
    const double copperVacInteractionEnergy_;

};

inline void
Vacancy::SetConcentrationBeforeQuenching(double C)
{
}

inline void
Vacancy::SetConcentrationAfterHardening(double C)
{
}