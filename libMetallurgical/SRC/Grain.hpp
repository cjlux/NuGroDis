#ifndef __Grain__hpp__
#define __Grain__hpp__

#include <string>
#include <vector>
#include <boost/python.hpp>

class Precipitate;
class ChemicalComposition;
class Material;

class Grain
{

public:
  //TODO We should have several constructors
  
  
  //CONSTRUCTOR
  //A grain MUST Know its material and its chemicalComposition.
  //Grain.chemicalComposition can be different from material.chemicalComposition
  Grain(Material& mat, ChemicalComposition &CC ); 
  
  
  //Only for SSGrain which belongs to a material,
//and has the same properties as the material: ChemicalCompo and mechanical properties
  //Grain(Material& mat); //TEST fake code!!!
  
    
  
  ~Grain();
  
  void Info() const;
  
  //Compute the molar volume of (precipitates, solid solution,...).The result is written in molar volume[m^3/mol] (was VmP1,VmP2). Unit: m^3/mol
  void ComputeMolarVolume();
  
  
  virtual void ConvertMassicToVolumicConcentration()=0;
  virtual void ConvertVolumicToMassicConcentration()=0;
  virtual void ConvertAtomicToVolumicConcentration()=0;
  virtual void ConvertVolumicToAtomicConcentration()=0;
  virtual void ConvertStoichiometricCoefficientToVolumicConcentration()=0;
  virtual void ConvertStoichiometricCoefficientToAtomicConcentration()=0; 
  
  //virtual void SetSSGrainChemicalComposition(const ChemicalComposition &CC)=0;
  
 
  virtual int GetVolNbPrecipitates() const =0 ;
  
  
  //Getters
  double GetMeanDiameter()      const {return meanDiameter_;     };
  double GetMolarVolume()       const {return molarVolume_ ;     };  
  //Setters
  void SetMeanDiameter(const double &meanDiameter);
  void SetMolarVolume(const double &);
  


  //RELATIONS
  //getters
  Material& GetMaterial() const;
  Material* GetMaterialPointer() const {return materialPointer_;};
  ChemicalComposition& GetChemicalComposition() const 
  {
    std::cout<<"in GetChemCompo. Adress of Grain is"<<this<<std::endl;
    std::cout<<"in GetChemCompo. Adress of ChemicalComposition"<<&chemicalComposition_<<std::endl;
    return chemicalComposition_;
  };

  //setters
  void SetMaterial(Material &mat);
 
  
  //Must be in SSGrain AND Precipitate ==>//ChemicalComposition& GetChemicalComposition() const {return chemicalComposition_;};
  //Must be in SSGrain AND Precipitate ==>//const ChemicalComposition& GetChemicalComposition() const {return chemicalComposition_;};
   
protected:
  
  //The mean diameter of the grain, or of the bounding sphere of the grain. Unit : m
  double meanDiameter_;
  // Volume of one mol of precipitate or  one mol af atoms of SSGrain. Unit : m^3/mol ==>    m^3/atomMol (for SSGrain) or m^3/precipitatesMol (for Precipitates)
  double molarVolume_;
  
  //RELATIONS
  Material *materialPointer_;//A Grain belongs to 1 and only 1 material
  ChemicalComposition& chemicalComposition_;  //A Grain has 1(one and only one) chemical composition.FOR precipitate it is constant and for SSGrain it can varies with time


private:
  
  

};

inline Material& 
Grain::GetMaterial() const 
{
  assert (  (materialPointer_!=0)&&"In GetMaterial of Grain object: Cannot Get material object because Grain does not have material yet" );
  return *materialPointer_;
}

inline void
Grain::SetMeanDiameter(const double &meanD)
{
  meanDiameter_=meanD;
}

inline void
Grain::SetMolarVolume(const double &molV)
{
  molarVolume_=molV;
}

inline void 
Grain::SetMaterial(Material &mat)
{
  //TODO find if Grain is in material.grainList_. If yes, abandon"Cannot Set material because it is already belongs to the material"
  
  //if not then do:
  materialPointer_=&mat;
}


#endif