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
#include <map>
#include <iostream>
#include <boost/python.hpp>

#include "SSGrain.hpp" //class SSGrain;

class ChemicalElement;
class Concentration;
class Material;

class ChemicalComposition
{

public:
  
  //Fake code, just a TEST!!!
  void test()
  {
 
      //TEST begin
  
  std::map<std::string , Concentration*>::iterator it;
  //std::cout<<"In GetConcentrationForElement, Adress of Concentration is "<< it->second)<<std::endl;
  std::cout<<" =====TEST IN CHEMICAL COMPOSITION BEGINS===== "<<std::endl;
  std::cout<<" Adress of CHEMICAL COMPOSITION object "<<this<<std::endl;
  std::cout<<" Adress of Concentration Map "<<& concentrationMap_<<std::endl;
  std::cout<<"Size = "<<concentrationMap_.size()<<std::endl;
  std::cout<<"Adress of Concentration List "<<& concentrationList_<<std::endl;
  std::cout<<"Size = "<<concentrationList_.size()<<std::endl;
 std::cout<<"Adress of Chemical Element List "<<& chemicalElementList_<<std::endl;
 std::cout<<"Size = "<<chemicalElementList_.size()<<std::endl;
  



  for (it=concentrationMap_.begin(); it!=concentrationMap_.end(); ++it)
  {
    std::cout<<"Adress of Concentration Object in Map is "<< it->second <<std::endl;
  }
  
  for( std::vector<Concentration*>::const_iterator i = concentrationList_.begin(); i != concentrationList_.end(); ++i)
  {
    std::cout<<"Adress of Concentration object in vector is "<<*i<<std::endl; 
  }
  
  for( std::vector<ChemicalElement*>::const_iterator j = chemicalElementList_.begin(); j != chemicalElementList_.end(); ++j)
  {
    std::cout<<"Adress of CE object in vector CE is "<<*j<<std::endl; 
  }
  
  std::cout <<"====END OF TEST===="<<std::endl;
  //TEST end
      
    
  }
  
    // ChemicalComposition(std::string formula="UNDEFINABLE");//contructor: only For initial composition of solide solution
                                                           //materialPointer_ and grainPointer_ are NULL
    
    //CONSTRUCTOR:   Build a simple classe CC. Does not need any other argument than formula. materialPointer_ and grainPointer_ = 0 
    ChemicalComposition(std::string formula);
  
    
    //CONSTRUCTOR: class CC Built only belongs to a Grain(can be -> Precipitate or SSGrain or ... ), and NOT to a material
    ChemicalComposition(std::string formula, Grain &grain);
    
    
    //CONSTRUCTOR: CC built belongs only to a material, and not to a Grain, thus, not belong to the material's ssgrain
    ChemicalComposition(std::string formula, Material &mat);
    
    //CONSTRUCTOR: Belongs to a material and to ssgrain. Material and its SSGrain has the same Chemical Composition
    ChemicalComposition(std::string formula,  Material& mat, SSGrain& materialSSgrain);
    
    
    //SPECIALIZED CONSTRUCTOR FOR Precipitate which is derived from virtual class Grain
    ChemicalComposition(std::string formula, Precipitate &P);
    
    
    
    //Copy constructor
    //TODO ? 
    //ChemicalComposition(ChemicalComposition const& CCtoCopy); 
    
   ///*TEST*/ChemicalComposition& operator=(ChemicalComposition const& CCtoCopy);
   ChemicalComposition* Clone() const {return new ChemicalComposition(*this) ;};

    
    
    ~ChemicalComposition();
    
    void Info() const;
    void UpdateVolumicValues();
    bool CheckValidity() const;
    
    
    //getter
    std::string GetFormula() const {return formula_;};
    const std::string GetMainElementName() const ;
    
    //Setters
    void SetMainElementName(const std::string& mainElName) {mainElementName_=mainElName;};
    
   
  
    
    //RELATIONS
    //Getter
    Material& GetMaterial() const ;
    Material* GetMaterialPointer() const 
    {
//       /*test*/ /*debug*/std::cout<<"In GetMaterialPointer of CC, material pointer is "<<materialPointer_<<std::endl;
      return materialPointer_;
    };
    
    Grain& GetGrain() const;
    Grain* GetGrainPointer() //TODO reput const==>  Grain* GetGrainPointer() const
    {
//       /*test*/ /*debug*/std::cout<<"In GetGrainPointer of CC, grain pointer is "<<grainPointer_<<std::endl;
        
      /*
      //TEST begin
  
  std::map<std::string , Concentration*>::iterator it;
  //std::cout<<"In GetConcentrationForElement, Adress of Concentration is "<< it->second)<<std::endl;
  std::cout<<"In GetConcentrationForElement, Adress of Concentration Map "<<& concentrationMap_<<std::endl;
  std::cout<<"In GetConcentrationForElement, Adress of Concentration List "<<& concentrationList_<<std::endl;
 std::cout<<"In GetConcentrationForElement, Adress of Chemical Element List "<<& chemicalElementList_<<std::endl;
  



  for (it=concentrationMap_.begin(); it!=concentrationMap_.end(); ++it)
  {
    std::cout<<"Adress of Concentration in Map is "<< it->second <<std::endl;
  }
  
  for( std::vector<Concentration*>::const_iterator i = concentrationList_.begin(); i != concentrationList_.end(); ++i)
  {
    std::cout<<"Adress of Concentration in vector is "<<*i<<std::endl; 
  }
  
  for( std::vector<ChemicalElement*>::const_iterator j = chemicalElementList_.begin(); j != chemicalElementList_.end(); ++j)
  {
    std::cout<<"Adress of CE in vector CE is "<<*j<<std::endl; 
  }
  //TEST end
      */
      return grainPointer_;
    };
    const std::vector<ChemicalElement *> & GetChemicalElementsList() const {return chemicalElementList_;};
    boost::python::list GetChemicalElementsPyList();
    const std::vector<Concentration *> & GetConcentrationList() const {return concentrationList_;};
    boost::python::list GetConcentrationPyList();
    const std::map<std::string, Concentration*>& GetConcentrationMap() const {return concentrationMap_;} ;
    std::map<std::string, Concentration*>& GetConcentrationMap();
    
    //debugg 
    //std::map<std::string, Concentration*>& GetConcentrationMap(); 
    
    boost::python::dict&  GetConcentrationPyDict();
    /*TODO after*/ //Concentration& GetConcentrationForElement(const ChemicalElement& CE) const ;
    Concentration& GetConcentrationForElement(const std::string elementSymbol) ; /*TODO const */
    
    
    //Setter
    void SetMaterial(Material &mat);
    void SetGrain(Grain& grain); //  Remember ==> Grain can be SSGrain or precipitate
    void AddChemicalElements(ChemicalElement& CE, Concentration& C); //Add a ChemicalElement AND its Concentration !!!

    
    
    
    // Converts a C++ map to a python dict
    template <class K, class V>
    static void MapToPythonDict(std::map<K, V> &map, boost::python::dict &);
    
    
    

protected:

private:
    const std::string formula_;
    //RELATIONS
    Material* materialPointer_;
    Grain* grainPointer_;//A grain can be SSgrain or Precipitate, or ...
    std::vector<ChemicalElement *> chemicalElementList_;
    std::vector<Concentration *> concentrationList_;//TODO: check if Clone() clone the non-reference attributes or copy instead
    std::map<std::string , Concentration*> concentrationMap_;
    boost::python::dict concentrationPyDict_;
    bool mapModified_;
    std::string mainElementName_;
    

};

inline const std::string 
ChemicalComposition::GetMainElementName() const
{
  assert ( (mainElementName_!="Undefined")&&"Cannot GetmainElement because it is undefined" );
  return mainElementName_;
}


//TODO CONST argrument
inline void
ChemicalComposition::SetMaterial(Material &mat)
{
  assert( (materialPointer_==0)&&"Cannnot SetMaterial in ChemicalComposition because Chemical composition already \
  belongs to a Material" ); 
  materialPointer_=&mat;
}




//TODO CONST argrument?
inline void
ChemicalComposition::SetGrain(Grain& grain)
{
  assert( (grainPointer_==0)&&"Cannnot SetGrain in ChemicalComposition because Chemical composition already \
  belongs to a grain" ); 
  grainPointer_=&grain;
}

inline Grain& 
ChemicalComposition::GetGrain() const
{
  
  assert( (grainPointer_!=0)&&"In ChemicalComposition: Cannnot Get Grain object  because chemical composition does\
  not belongs to any Grain (pointer is 0) " ); 
  
//   /*test*/ /*debug*/ std::cout<<"In GetGrain of CC, grain pointer is "<<grainPointer_<<std::endl;
  
  
    return *grainPointer_;   
}

inline Material& 
ChemicalComposition::GetMaterial() const 
{
  assert( (materialPointer_!=0)&&"In ChemicalComposition: Cannnot Get Material object  because chemical composition does\
  not belongs to any material (pointer is 0) " ); 
  
//   /*test*/ /*debug*/std::cout<<"In GetMaterial of CC, material object adress is "<<&(*materialPointer_)<<std::endl;
    return *materialPointer_; 

}




#endif
