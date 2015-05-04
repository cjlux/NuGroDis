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


#ifndef __SSGrain__hpp__
#define __SSGrain__hpp__

#include <string>
#include <vector>

#include "Grain.hpp"

class Precipitate;
class ChemicalComposition;
class Material;


class SSGrain: public Grain
{

public:
  
  //Basic constructor, An SSGrain must Know its ChemicalComposition, its material, its YoungM, Its PoissonCoef, its volNbGP and its volNbSprime
  //This constructor means, material.YM, material.PoisCoef, material.latticP, and material.ChemicalCompo could be different from those of SSGrain.
  SSGrain(Material &mat,ChemicalComposition &CC,double YM,double PoissonCoeff, double latticeParam, int volNbGP=0, int volNbSprime=0); 
  
  //Special constructor
  //This constructor means SSGrain.ChemicalCompo, SSGrain.YM, SSGrain.PoisCoef and SSGrain.latticP are the same with those from material.
  SSGrain(Material &mat,int volNbGP=0, int volNbSprime=0);
  
    ~SSGrain();
    
    void Info() const;
    
    //Compute the molar volume of (precipitates, solid solution,...).The result is written in molar volume[m^3/mol] (was VmP1,VmP2). Unit: m^3/mol
    void ComputeMolarVolume();
    
    //Read Specific data value for the given Alloy. Computes the atomic concentrtaions for the chemical elements in the solid Solution
    void ReadDataValue(std::string);
    
    
    //Setters
    void SetVolNbGP(const int &);
    void SetVolNbSprime(const int &);
    void SetVolNbPrecipitates(const int &);
    //Getters
    int    GetVolNbGP()           const {return volNbGP_;          };
    int    GetVolNbSprime()       const {return volNbSprime_;      };
    int    GetVolNbPrecipitates() const {return volNbPrecipitates_;};
    const double GetLatticeParameter()  const {return latticeParameter_; };
    const double GetYoungModulus()      const {return YoungModulus_;     };
    const double GetPoissonCoeff()      const {return PoissonCoeff_;     };
    
    //RELATIONS
    //Setter Aggregations
    
    //Add in precipitateList.
    void AddPrecipitate(Precipitate & prec);/*Any precipitate which belongs to a material's SSgrain belongs to the
     * material. But all precipitates which belongs to the material does not belong to the material's SSGrain*/

    //Getter aggregations
    std::vector<Precipitate*> GetPrecipitateList() {return precipitateList_;};

protected:

private:
   
    //The Young modulus of the Grain. If not given, can be approximated with the Young modulus of the main chemical element of the grain. Unit: MPa
    const double & YoungModulus_;
    //The Poisson coefficient of the Grain. If not given, can be approximated with the Poisson coefficient of the main chemical element of the grain. Unit: no unit
    const double & PoissonCoeff_;       
    //The lattice parameter of the main ChemicalElement. Unit: m
    const double & latticeParameter_;   
    //volumic number of GP precipitates. Unit: number/m^3
    int volNbGP_;
    //volumic number of Sprime precipitates. Unit: number/m^3
    int volNbSprime_;
    //Volumic number of precipitates in the grain. Unit: number/m^3
    int volNbPrecipitates_;
    
    // RELATIONS
    std::vector<Precipitate *> precipitateList_; //A SSGrain contains 0 or many Precipitates
};




inline void
SSGrain::SetVolNbGP(const int &vNGP)
{
  volNbGP_=vNGP;
}

inline void
SSGrain::SetVolNbSprime(const int &vNSP)
{
  volNbSprime_=vNSP;
}

inline void
SSGrain::SetVolNbPrecipitates(const int &vNP)
{
  volNbPrecipitates_=vNP;
}


//Aggregation setters
inline void 
SSGrain::AddPrecipitate(Precipitate & prec)
{
  precipitateList_.push_back(& prec);
}

#endif
