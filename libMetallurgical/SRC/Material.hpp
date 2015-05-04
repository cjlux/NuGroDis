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

#ifndef __Material__hpp__
#define __Material__hpp__

#include <cassert>
#include <string>
#include <vector>

class Temperature;
class ChemicalElement;
class Vacancy;
class ChemicalComposition;
class SSGrain;
class Grain;
class Precipitate;

class Material
{

public:
  Material();
  Material(Temperature& , ChemicalElement&, ChemicalComposition& InitialCompo);
  
  //Material(Temperature& , ChemicalElement&, ChemicalComposition& InitialCompo, PrecipitateList&); //TODO
  
  ~Material();

  void Info() const;
  void ReadData(std::string dataFileName);
  void InitializeGrains(); //Initialize material SSgrains AND/OR precipitates
  
  //Getter
  Temperature & GetTemperature() const {return temperature_; } 
  const double GetYoungModulus() const { return YoungModulus_; }
  const double GetPoissonCoeff() const { return PoissonCoeff_; }
  const double GetLatticeParameter() const { return latticeParameter_; }

  //Setter
  void SetTemperature(const Temperature &temperatureClass);

   
  //RELATION
  //getter
  std::vector<Grain *> GetGrainList() const {return grainList_; } ;//TO DO should be SSGraibLIst+PrecipitateList
  std::vector<Vacancy *> GetVacancyList() const {return vacancyList_; } ;
  const ChemicalElement& GetMainChemicalElement() const {return mainChemicalElement_; };
  ChemicalComposition & GetInitialChemicalComposition() const {return InitialChemicalComposition_; }; //Not const because==>
											//SSGrain.cpp:30: error: invalid initialization  
                                                                                        //of reference of type ‘ChemicalComposition&’ from
                                                                                        //expression of type ‘const ChemicalComposition’
  const std::vector<Precipitate *> & GetPrecipitateList() const {return precipitatesList_; };
  SSGrain * GetSSGrain() const {return ssgrain_; } ; 
  std::vector<const ChemicalElement*> GetSoluteList() const {return soluteList_;};
  
  //setter
  void AddGrain(Grain& grain) ;//TODO grain_ can be alot of things:  SSGrain, Precipitate, or maybe in the future something else . Precipitate can be a lot of things: GuinierPreson,Sprime, ... 
  void AddVacancy(const Vacancy& vacancy) ;
  void SetSSGrain(SSGrain& ssgrain) ;
  //void SetChemicalElement(const ChemicalElement& chemElm); //unuseful?
  void AddPrecipitate(Precipitate& P); /* All precipitates which belongs to the material does not belong to the material's SSGrain.
  *But any precipitate which belongs to the material's SSgrain belongs to the material. */
  void AddSolute(const ChemicalElement& Solute);

protected:

private:
   
  //RELATIONS
  Temperature& temperature_;
  std::vector<Vacancy *> vacancyList_;
  std::vector<Grain *> grainList_; //TODO should be automatically SSGrainList+PrecipitateList
  const ChemicalElement& mainChemicalElement_;
  ChemicalComposition& InitialChemicalComposition_;
  std::vector<const ChemicalElement*> soluteList_; /*list of solute in the material/ssgrain. 
                                                   can also be used to find diffusion param of
                                                   solutes in the material/ssgrain*/
 
  //TODO Get ssGrain, Precipitates( =GP OR Sprime OR ...) from grainList_. Retrieve the type of specialisation class
  
  SSGrain * ssgrain_; //material has 0 or 1 ssgrain_.  Remember that ssgrain is the family of SSGrains
  std::vector<Precipitate *> precipitatesList_;
  // Mechanical parameters are those of the main chemical component..:
  const double YoungModulus_;
  const double PoissonCoeff_;
  const double latticeParameter_;
 
  //IMPORTANT: Material current ChemicalComposition is known thanks to ssgrain_ and its attribute chemicalComposition_ 
};

#endif
