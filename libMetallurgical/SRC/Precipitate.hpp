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

//A Precipitate "Is a" Grain.
//A Precipitate cannot contain any other Grain.
//A precipitate has a fixed composition (fixed stoechiometrie).
//The class Precipitate will not be instanciated because the number of precipitates is too high ( ~10^9). 
//Instead the volumic number of precipitates is computed in the class Grain thanks to the atribute volNbPrecipitates.

class Precipitate
{

public:

    //The constructor
    Precipitate();
    
    //the destructor
    ~Precipitate();
    
    void Info() const;
    
    //elastic distorsion energy due to the difference of cell size.(was deltageP) . unit: J/m^3
    double ComputeDistorsionEnergy();
    
    //Compute the interfacial energy. Unit: J/m^2. Actually, surface energy is not stored
    double ComputeSurfaceEnergy();
    
    //Read data value specific for a type of precipitate
    void ReadDataValue(std::string fileName);

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
    double preExptermForSolvus_;
    
    //The polynomial model to compute the surface energy. unit J/m^2
    Polynomial * surfaceEnergyPolynomialModel_;
    
    //ratio between lenght to radius.If nul, shape is spherical.
    double shapeFactor_;
    
    //was fractvolPn
    double volumicFraction_;

};