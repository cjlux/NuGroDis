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
#ifndef __ChemicalComposition__hpp__
#define __ChemicalComposition__hpp__

#include <vector>
#include <iostream>

#include "SSGrain.hpp" //class SSGrain;

class ChemicalElement;
class Concentration;
class Material;

class ChemicalComposition
{

public:
    // ChemicalComposition(std::string formula="UNDEFINABLE");//contructor: only For initial composition of solide solution
                                                           //material_ and grain_ are NULL
    
    //CONSTRUCTOR:   Build a simple classe CC. Does not need any other argument than formula. material_ and grain_ = 0 
    ChemicalComposition(std::string formula);
  
    
    //CONSTRUCTOR: class CC Built only belongs to a Grain(can be -> Precipitate or SSGrain or ... ), and NOT to a material
    ChemicalComposition(std::string formula, Grain &grain);
    
    
    //CONSTRUCTOR: CC built belongs only to a material, and not to a Grain, thus, not belong to the material's ssgrain
    ChemicalComposition(std::string formula, Material &mat);
    
    //CONSTRUCTOR: Belongs to a material and to ssgrain. Material and its SSGrain has the same Chemical Composition
    ChemicalComposition(std::string formula,  Material& mat, SSGrain& materialSSgrain);
    
    
    //SPECIALIZED CONSTRUCTOR FOR Precipitate which is derived from virtual class Grain
    //ChemicalComposition(std::string formula, Precipitate &P);
    
    
    ~ChemicalComposition();
    
    void Info() const;
    void UpdateVolumicValues();
    bool CheckValidity() const;
    
    //getter
    std::string GetFormula() const {return formula_;};
   
  
    
    //RELATIONS
    //Getter
     //const SSGrain& GetGrain() const {return grain_;};
    const Material& GetMaterial() const {return *material_;};
    const Grain& GetGrain() const {return *grain_;};
    const std::vector<ChemicalElement *> & GetChemicalElementsList() const;
    const std::vector<Concentration *> & GetConcentrationList() const;
    Concentration& GetConcentrationForElement(const ChemicalElement&) const;
    Concentration& GetConcentrationForElement(const std::string&) const;
    
    //Setter
    void SetMaterial(const Material &mat);
    void SetGrain(const Grain& grain); //  Remember ==> Grain can be SSGrain or precipitate
    void AddChemicalElements(ChemicalElement&, Concentration& ); //Add a ChemicalElement AND its Concentration !!!


protected:

private:
    const std::string formula_;
    //RELATIONS
    Material* material_;
    //const SSGrain& grain_;
    Grain* grain_;
    std::vector<ChemicalElement *> chemicalElementList_;
    std::vector<Concentration *> concentrationList_;
    
    
    

};

inline void
ChemicalComposition::AddChemicalElements(ChemicalElement&, Concentration& )
{
 
}

#endif
