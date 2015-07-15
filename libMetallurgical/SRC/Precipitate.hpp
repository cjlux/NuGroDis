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


#ifndef __Precipitate__hpp__
#define __Precipitate__hpp__

#include <cassert>
#include <vector>
#include <map>
#include <boost/python.hpp>

#include "SSGrain.hpp"



//A Precipitate "Is a" Grain.
//A Precipitate cannot contain any other Grain.
//A precipitate has a fixed composition (fixed stoechiometrie).
//The class Precipitate will not be instanciated because the number of precipitates is too high ( ~10^9). 
//Instead the volumic number of precipitates is computed in the class Grain thanks to the atribute volNbPrecipitates.
//Precipitate can be a lot of things: GuinierPreston, Sprime,... or maybe in the future something else

#include "Polynomial.hpp"
class RadiusDistribution;
//class Material;

class Precipitate : public Grain
{


public:

    //The constructor
    //Precipitate();
    Precipitate(Material& mat, ChemicalComposition &CC, RadiusDistribution &initialRD); // Relation: SSGrain inheritance 
				//argument-->Material& because of inheritance with Grain
    
    //the destructor
    ~Precipitate();
    
    void Info() const;
    
    //elastic distorsion energy due to the difference of cell size.(was deltageP) . unit: J/m^3
    void ComputeDistorsionEnergy(); //Compute and Set distorsion Energy
    
    //Compute the value of interfacial energy according to current temperature. Unit: J/m^2. Actually, current surface energy is stored in surfaceEnergyCurrentValue_
    void ComputeSurfaceEnergy();
    
    //Compute and set Gibbs Free Energy for phase change (was DeltaGv)
    void ComputePhaseChangeVolumicEnergy();
    
    //Compute and return a value. The value computed is not set!!!
    double ReturnCriticalRadius();
    double ReturnDeltaCriticalRadius();
    virtual double ReturnCriticalTotalEnergy()=0;//this method has to be defined in inherithed class!!!
    double ReturnZeldovichFactor();
    double ReturnCriticalBeta();
    double ReturnNucleationRate();
    
    //compute and set
    void ComputeCriticalRadius();
    void ComputeDeltaCriticalRadius();
    virtual void ComputeCriticalTotalEnergy()=0;
    void ComputeZeldovichFactor();
    void ComputeCriticalBeta();
    void ComputeNucleationRate();
    
    //Read data value specific for a type of precipitate
    void ReadDataValue(std::string fileName);
    
    //
    void InitializeEquilibriumConcentrationMap();
    
    
    //Initialize attributes of class Precipitate.
    //Can also be used with method ReadDataValue(std::string) which will read a string data before give them to InitializeParameters()
    void InitializeParameters(double deltaCell,
			      double solvusActivationEnergy,
			      double nucleationSitesNumber,
			      double preExpTermForSolvus,
			      Polynomial& surfaceEnergyPolynomialModel,
			      double shapeFactor,
			      double volumicFraction); //TODO because Sprime and GP has a lot of different paramerter, maube it will be more useful to make a manual Set 
    
    
    int GetVolNbPrecipitates() const  { assert(!"Method GetVolNbPrecipitates() has no sense for Precipitates!!! "); };
    //void SetSSGrainChemicalComposition(const ChemicalComposition &CC) { assert(!"SetSSGrainChemicalComposition does not exist for pecipitate!!!")  ;};

  virtual double WettingFunction(double theta) = 0;
  
  
  //Concentration conversion functions
  void ComputeSumOfStoicCoefs();//Compute and Set Sum of stoichiometric coefficients
  void ConvertMassicToVolumicConcentration();
  void ConvertVolumicToMassicConcentration();
  void ConvertAtomicToVolumicConcentration();
  void ConvertVolumicToAtomicConcentration();
  void ConvertStoichiometricCoefficientToVolumicConcentration();
  void ConvertStoichiometricCoefficientToAtomicConcentration();
  
  
  
  
  
  //Getter
  double GetDeltaCell()                         const {return deltaCell_;} ;
  double GetSolvusActivationEnergy()            const {return solvusActivationEnergy_;};
  double GetDistorsionEnergy()                  const {return distorsionEnergy_;};
  double GetphaseChangeVolumicEnergy()          const {assert ((phaseChangeVolumiqueEnergy_!=-1)&&"In precipitate GetphaseChangeVolumicEnergy(): phaseChangeVolumiqueEnergy_ has not been computed"); return phaseChangeVolumiqueEnergy_;};
  double GetNucleationSitesNumber()             const {return nucleationSitesNumber_;};
  double GetPreExpTermForSolvus()               const {return preExpTermForSolvus_;};
  double GetShapeFactor()                       const {return shapeFactor_;};
  double GetVolumicFraction()                   const {return volumicFraction_;};
  Polynomial& GetSurfaceEnergyPolynomial()            {return surfaceEnergyPolynomial_;};
  const Polynomial& GetSurfaceEnergyPolynomial()const {return surfaceEnergyPolynomial_;};
  const int GetSumOfStoicCoefs() const;
  double GetSurfaceEnergyCurrentValue() const {return surfaceEnergyCurrentValue_;};
  std::map<std::string, double> GetEquilibriumConcMap() const {return equilibriumConcMap_;};
  double GetEquilibriumConcentrationForElement(std::string elementSymbol);
  double GetCriticalRadius()         const {return  criticalRadius_      ;} ;/*TODO assert*/
  double GetDeltaCriticalRadius()    const {return  deltaCriticalRadius_ ;} ;/*TODO assert*/
  double GetCriticalTotalEnergy()    const {return  criticalTotalEnergy_ ;} ;/*TODO assert*/
  double GetZeldovichFactor()        const {return  ZeldovichFactor_     ;} ;/*TODO assert*/
  double GetCriticalBeta()           const {return  criticalBeta_        ;} ;/*TODO assert*/
  double GetNucleationRate() const 
  {
    assert ((nucleationRate_>=0)&&"Cannot GetNucleationRate() because it is not positive!!!");
    return  nucleationRate_      ;
  } ;

  
  //Setter
  void SetDeltaCell (const double deltaCell);
  void SetSolvusActivationEnergy(const double );
  void SetNucleationSitesNumber(const double);
  void SetPreExpTermForSolvus(const double);
  void SetShapeFactor(const double );
  void SetVolumicFraction(const double );
  void SetSEPolynomialDegree(const int deg);//SE i.e. SurfaceEnergy
  void AddSEPolynomialPyCoefs(boost::python::list& pythonCoefList);

  
  //RELATIONS
  //getter
  const RadiusDistribution& GetInitialRadiusDistribution()     const {return initialRadiusDistribution_;};
  RadiusDistribution* GetCurrentRadiusDistribution()           const {return currentRadiusDistribution_;};
 // std::vector<RadiusDistribution*>& GetRadiusDistributionList() const {return radiusDistributionList_;}
  const std::vector<RadiusDistribution*>& GetRadiusDistributionList() const {return radiusDistributionList_;};

  //setter
  void AddRadiusDistribution(const RadiusDistribution& RD);
  void SetCurrentRadiusDistribution(const RadiusDistribution & RD);
 
  
  //Must be in SSGrain AND Precipitate ==>//ChemicalComposition& GetChemicalComposition() {return chemicalComposition_;};
  //Must be in SSGrain AND Precipitate ==>//const ChemicalComposition& GetChemicalComposition() const {return chemicalComposition_;};
 
protected:
  
  //difference between precipitate and solid solution cell size , was deltamailleP ( for example, = |a_ss-a_precipitate|/a_precipitate) 
    double deltaCell_;
    
    //DeltaH
    double solvusActivationEnergy_;
    
    //elastic distorsion energy due to the difference of cell size.(was deltageP). unit: J/m^3
    double distorsionEnergy_;
    
    // (was DeltaGv)
    double phaseChangeVolumiqueEnergy_;
    
    //current nucleation sites number. The first value is initial value (was Ns0P1 or N0P2) .Unit: atomNumber/m^3 (P1) or Precipitates-Dislocations-GrainBoundary Number/m^3  (P2)
    double nucleationSitesNumber_;
    
    //pre-exponential solvus constant. Used in the temperature dependency of X_eq_SS or X_i_SS... . Value depends on the precipitate....
    double preExpTermForSolvus_;
    
    //The polynomial model to compute the surface energy.
    Polynomial surfaceEnergyPolynomial_;
    
    //Current value of surfaceEnergyPolynomial_ according to temperature. unit J/m^2
    double surfaceEnergyCurrentValue_;
    
    //ratio between lenght to radius.If nul, shape is spherical.
    double shapeFactor_;
    
    //was fractvolP
    double volumicFraction_;//Value is setted and update thanks to Method ComputeVolumicFraction in cclass GuinierPreston and Sprime

    //was retoileP or r*
    double criticalRadius_;
    //was deltaretoileP
    double deltaCriticalRadius_;
    //was deltaGetoile DeltaG*
    double criticalTotalEnergy_;
    //was ZP
    double ZeldovichFactor_;
    //was Beta*
    double criticalBeta_;
    //was J
    double nucleationRate_;
    
    //RELATIONS
    std::vector<RadiusDistribution*> radiusDistributionList_; //Save all the RadiusDistribution at each step of the computation
    RadiusDistribution * currentRadiusDistribution_;
    const RadiusDistribution & initialRadiusDistribution_;
    
    int sumOfStoicCoefs_;
    bool SumOfStoiCoefsHasBeenComputed_;
    double solubilityProduct_;
    std::map<std::string, double> equilibriumConcMap_; //(map of XvSSeq_i)
    
   
    //ChemicalComposition & chemicalComposition_;  //A Grain has 1(one and only one) chemical composition.FOR precipitate it is constant and for SSGrain it can varies with time

private:
  
    
};

//TODO Inline Setters!!! (AND do not forget assrtions!!!!)




inline const int 
Precipitate::GetSumOfStoicCoefs() const
{
  assert ( (sumOfStoicCoefs_>0)&&"Cannot GetSumOfStoicCoefs because Sum of \
  stoichiomoetric coefficients is not an integer stricly positive!!!");
  
  return sumOfStoicCoefs_;
}


inline void
Precipitate::SetDeltaCell(const double deltaCell)
{
  deltaCell_=deltaCell;
}

inline void 
Precipitate::SetSolvusActivationEnergy(const double solvActEn)
{
 solvusActivationEnergy_=solvActEn;
}



inline void
Precipitate::SetNucleationSitesNumber(const double nuclSitNb)
{
  nucleationSitesNumber_=nuclSitNb;
}

inline void
Precipitate::SetPreExpTermForSolvus(const double preExpSolv)
{
 preExpTermForSolvus_=preExpSolv;
}


inline void
Precipitate::SetShapeFactor(const double shapeFactor)
{
  shapeFactor_=shapeFactor;
}

inline void
Precipitate::SetVolumicFraction(const double volFrac)
{
  volumicFraction_=volFrac;
}


#endif
