#ifndef __Grain__hpp__
#define __Grain__hpp__

#include <string>
#include <vector>

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
  
  
  
  ~Grain();
  
  void Info() const;
  
  //Compute the molar volume of (precipitates, solid solution,...).The result is written in molar volume[m^3/mol] (was VmP1,VmP2). Unit: m^3/mol
  void ComputeMolarVolume();
  
 
  virtual int GetVolNbPrecipitates() const =0 ;
  
  
  //Getters
  double GetMeanDiameter()      const {return meanDiameter_;     };
  double GetMolarVolume()       const {return molarVolume_ ;     };  
  //Setters
  void SetMeanDiameter(const double &meanDiameter);
  void SetMolarVolume(const double &);
  


  //RELATIONS
  //getters
  const Material& GetMaterial() const {return *material_;};
  ChemicalComposition& GetChemicalComposition() const {return chemicalComposition_;};
  //setters
  void SetMaterial(const Material &mat);
 
  
  //Must be in SSGrain AND Precipitate ==>//ChemicalComposition& GetChemicalComposition() const {return chemicalComposition_;};
  //Must be in SSGrain AND Precipitate ==>//const ChemicalComposition& GetChemicalComposition() const {return chemicalComposition_;};
   
protected:
  
  //The mean diameter of the grain, or of the bounding sphere of the grain. Unit : m
  double meanDiameter_;
  // Volume of one mol of precipitate or  one mol af atoms of SSGrain. Unit : m^3/mol ==>    m^3/atomMol (for SSGrain) or m^3/precipitatesMol (for Precipitates)
  double molarVolume_;
  
  //RELATIONS
  Material *material_;//A Grain belongs to 1 and only 1 material
  ChemicalComposition& chemicalComposition_;  //A Grain has 1(one and only one) chemical composition.FOR precipitate it is constant and for SSGrain it can varies with time


private:
  
  

};

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

#endif