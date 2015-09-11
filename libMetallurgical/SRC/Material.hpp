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

class Computation;
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
  //FAKE METHOD
   void test();
  
  Material(Temperature& , ChemicalElement&, ChemicalComposition& InitialCompo, Computation& computation);
  
  //Material(Temperature& , ChemicalElement&, ChemicalComposition& InitialCompo, PrecipitateList&, Computation& computation); //TODO
  
  ~Material();
  
  
  void RunProcess();

  void ProcessPrecipitatesNucleationRate();
  
  void ComputePrecipitatesAllInterfacialConcentrations();
  
  void ComputePrecipitatesInterfacialVelocityList();
  
  void ComputeCriticalInterfacialConcentration();
  
  void ProcessComputationMaxTimeStep();
  
  void AddNucleatedPrecipitates();
  
  void SolveCineticLinearSytem();
  
  void ComputePrecipitatesVolumicFraction();
  
  void ComputePrecipitatesNucleationSiteNb();
  
  void UpdateTimeStep(); //En cours

  void Info() const;
  void ReadData(std::string dataFileName);
  void InitializeGrains(); //Initialize material SSgrains AND/OR precipitates
  void UpdateVolumicValues();//Actualize the volumic value taking into account the volumic fraction of precipitates
  void ConvertVolumicToInitialAtomicConcentration();
  void ConvertVolumicToInitialMassicConcentration();
  double ReturnAtomicConcFromVolumicForElement(std::string elementName) const;
  
  //Getter
  Temperature & GetTemperature() const {return temperature_; } 
  const double GetYoungModulus() const { return YoungModulus_; }
  const double GetPoissonCoeff() const { return PoissonCoeff_; }
  const double GetLatticeParameter() const { assert ((latticeParameter_>0)&&"Cannot get Material lattice parameter. The value is not strictly positive!!! Material Lattice parameter has not been setted correctly ") ;return latticeParameter_; }

  //Setter

   
  //RELATION
  //getter
  Computation& GetComputation() const {return computation_;}; 
  std::vector<Grain *> GetGrainList() const {return grainList_; } ;//TO DO should be SSGraibLIst+PrecipitateList
  const Vacancy*  GetVacancyPointer() const {return vacancy_; } ;
  Vacancy*  GetVacancyPointer() {return vacancy_; } ;
  const bool AssertVacancyPointer( Vacancy* pointedAdress) const  {return vacancy_== pointedAdress  ;};
  
  Vacancy& GetVacancy() const 
  {
    assert  ( (vacancy_!=0)&&"Cannot GetVacancy() because material does not have any vacancy yet (vacancy pointer is 0) " );
    return *vacancy_;
  };
  const ChemicalElement& GetMainChemicalElement() const {return mainChemicalElement_; };
  const ChemicalComposition & GetInitialChemicalComposition() const 
  {
    std::cout<<"in GetInitialChemCompo. Adress of Material is"<<this<<std::endl;
    std::cout<<"in GetInitialChemCompo. Adress of initialChemicalComposition"<<&InitialChemicalComposition_<<std::endl;
    return InitialChemicalComposition_; 
    
  }; //Not const because==> SSGrain.cpp:30: error: invalid initialization of reference of type ‘ChemicalComposition&’ from
  //expression of type ‘const ChemicalComposition’
  
  ChemicalComposition & GetCurrentChemicalComposition() const {return currentChemicalComposition_;};
  const std::vector<Precipitate *> & GetPrecipitateList() const {return precipitateList_; };
  std::vector<Precipitate *> & GetPrecipitateList() {return precipitateList_; };
  SSGrain * GetSSGrainPointer() const {return ssgrainPointer_; } ; 
  SSGrain& GetSSGrain() const 
  {
    assert ( (ssgrainPointer_!=0)&&"Cannot gGetSSGrain because pointer is 0" );  
    return *ssgrainPointer_;
    
  };
  std::vector<const ChemicalElement*> GetSoluteList() const {return soluteList_;};
  
  //setter
  void AddGrain(Grain& grain) ;//TODO grain_ can be a lot of things:  SSGrain, Precipitate, or maybe in the future something else . Precipitate can be a lot of things: GuinierPreson,Sprime, ... 
  void SetVacancy(Vacancy& vacancy) ;
  void SetSSGrain(SSGrain& ssgrain) ;
  //void SetChemicalElement(const ChemicalElement& chemElm); //unuseful?
  void AddPrecipitate(Precipitate& P); /* All precipitates which belongs to the material does not belong to the material's SSGrain.
  *But any precipitate which belongs to the material's SSgrain belongs to the material. */
  void AddSolute(const ChemicalElement& Solute);

protected:

private:
   
  //RELATIONS
  Temperature& temperature_;
  Vacancy * vacancy_;
  std::vector<Grain *> grainList_; //TODO should be automatically SSGrainList+PrecipitateList
  const ChemicalElement& mainChemicalElement_;
  const ChemicalComposition& InitialChemicalComposition_;
  ChemicalComposition& currentChemicalComposition_; //Also the chemicalComposition (same object, same adress) of the SSGrain if material has one
  std::vector<const ChemicalElement*> soluteList_; /*list of solute in the material/ssgrain. 
                                                   can also be used to find diffusion param of
                                                   solutes in the material/ssgrain*/
                                                   
  Computation& computation_;
 
  //TODO Get ssGrain, Precipitates( =GP OR Sprime OR ...) from grainList_. Retrieve the type of specialisation class
  
  SSGrain * ssgrainPointer_; //material has 0 or 1 ssgrainPointer_.  Remember that ssgrain is the family of SSGrains
  std::vector<Precipitate *> precipitateList_;
  // Mechanical parameters are those of the main chemical component..:
  const double YoungModulus_;
  const double PoissonCoeff_;
  const double latticeParameter_;
 
  //IMPORTANT: Material current ChemicalComposition is known thanks to ssgrainPointer_ and its attribute chemicalComposition_ 
};

#endif
