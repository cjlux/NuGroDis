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

#include "SSGrain.hpp"


//A Precipitate "Is a" Grain.
//A Precipitate cannot contain any other Grain.
//A precipitate has a fixed composition (fixed stoechiometrie).
//The class Precipitate will not be instanciated because the number of precipitates is too high ( ~10^9). 
//Instead the volumic number of precipitates is computed in the class Grain thanks to the atribute volNbPrecipitates.
//Precipitate can be a lot of things: GuinierPreston, Sprime,... or maybe in the future something else

class Polynomial;
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
    double ComputeDistorsionEnergy();
    
    //Compute the interfacial energy. Unit: J/m^2. Actually, surface energy is not stored
    double ComputeSurfaceEnergy();
    
    //Read data value specific for a type of precipitate
    void ReadDataValue(std::string fileName);
    
    
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


  virtual double WettingFunction(double theta) = 0;
  
  
  //Getter
  double GetDeltaCell() const {return deltaCell_;} ;
  double GetSolvusActivationEnergy() const {return solvusActivationEnergy_;};
  double GetDistorsionEnergy()          const {return distorsionEnergy_;};
  double GetNucleationSitesNumber()          const {return nucleationSitesNumber_;};
  double GetPreExpTermForSolvus()          const {return preExpTermForSolvus_;};
  double GetShapeFactor()          const {return shapeFactor_;};
  double GetVolumicFraction()          const {return volumicFraction_;};
  Polynomial* GetSurfaceEnergyPolynomialModel() const {return surfaceEnergyPolynomialModel_;};
  
  
  //Setter
  void SetDeltaCell (const double deltaCell);
  void SetSolvusActivationEnergy(const double );
  void SetDistorsionEnergy(const double );
  void SetNucleationSitesNumber(const double);
  void SetPreExpTermForSolvus(const double);
  void SetSurfaceEnergyPolynomialModel(Polynomial&);
  void SetShapeFactor(const double );
  void SetVolumicFraction(const double );

  
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

private:
  
    //difference between precipitate and solid solution cell size , was deltamailleP ( for example, = |a_ss-a_precipitate|/a_precipitate) 
    double deltaCell_;
    
    //DeltaH
    double solvusActivationEnergy_;
    
    //elastic distorsion energy due to the difference of cell size.(was deltageP). unit: J/m^3
    double distorsionEnergy_;
    
    //urrent nucleation sites number. The first value is initial value (was Ns0P1 or N0P2) .Unit: atomNumber/m^3 (P1) or Precipitates-Dislocations-GrainBoundary Number/m^3  (P2)
    double nucleationSitesNumber_;
    
    //pre-exponential solvus constant. Used in the temperature dependency of X_eq_SS or X_i_SS... . Value depends on the precipitate....
    double preExpTermForSolvus_;
    
    //The polynomial model to compute the surface energy. unit J/m^2
    Polynomial * surfaceEnergyPolynomialModel_;
    
    //ratio between lenght to radius.If nul, shape is spherical.
    double shapeFactor_;
    
    //was fractvolPn
    double volumicFraction_;//Value is setted and update thanks to Method ComputeVolumicFraction in cclass GuinierPreston and Sprime

    //RELATIONS
    std::vector<RadiusDistribution*> radiusDistributionList_; //Save all the RadiusDistribution at each step of the computation
    RadiusDistribution * currentRadiusDistribution_;
    const RadiusDistribution & initialRadiusDistribution_;
    
   
    //ChemicalComposition & chemicalComposition_;  //A Grain has 1(one and only one) chemical composition.FOR precipitate it is constant and for SSGrain it can varies with time
  
    
};

//TODO Inline Setters!!! (AND do not forget assrtions!!!!)

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
Precipitate::SetDistorsionEnergy(const double distEn)
{
  distorsionEnergy_=distEn;
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
Precipitate::SetSurfaceEnergyPolynomialModel(Polynomial& surfEnPoly)
{
  surfaceEnergyPolynomialModel_=&surfEnPoly;
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
