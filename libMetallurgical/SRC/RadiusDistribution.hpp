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


#ifndef __RadiusDistribution__hpp__
#define __RadiusDistribution__hpp__


#include <string>
#include <vector>
#include <map>

class Precipitate;
class ChemicalElement;
class InterfacialConcentration;



class RadiusDistribution
{

public:
  
  
  //     |classItemsValues |value or item of class1|value or item of class2|        ...         |value or item of classN| 
  //     |classNumbers     |        class1         |        class2         |        ...         |   classN=classNumber_ |

 //CONSTRUCTOR. Builds an instance with given parameters spatialStep deltar and minimum radius r1 (all the histogram values are initialized to 0)
 RadiusDistribution(double deltar, double r1, double initialClassNb, Precipitate& );
 
 //CONSTRUCTOR of RD which does not have any precipitate //initial RD
 RadiusDistribution(double deltar, double r1, double initialClassNb); 
 
 
 /*comment by MG TODO : i think, we should have another constructor, to instantiate radiusDistribution read from a dataFile. This constructor will not take as argument a initialClassNumber,
  *but will use dataFile to compute and initialize initialClassNumber_, etc    */
 
 

 //The destructor
 ~RadiusDistribution();
 
 void Info() const;
 
 void SaveDistribution();
 
 
 void Initialize();
 
 void InitializeInterfConc(); //Process Precipitates chemical compo and data, and create interfacialConcentrationObjects associated to each chemicalElement. 
                              //For information, this method was private
 
 int SolveInterfacialConcentrationsEquations(double f,
					       double XvSSi,
					       double XvPi,
					       double DjOverDi, 
					       double XvSSj,
					       double XvPj,
					       double & X,
					       double & Y);
 
 
 void ComputeAllInterfacialConcentrations();
 
 void ComputeInterfacialVelocityList(); // use InterfacialConcentration::ComputeInterfacialVelocityList()
 
 void ComputeCriticalInterfacialConcentration(); 
 
 const double ReturnCriticalInterfacialVelocity();//return VINTcritique
 

 //Computes (but not set) and return the total number(value) of items. Use to find nucleation site number
 const double ReturnTotNbOfItems();

 
 unsigned int FindClassForRadius(double r);
 
 //Return the item value for a  given class i 
 double GetItemValueForClass(unsigned int classId);
 
 double GetRadiusForClass(unsigned int classId);//classId varies from 1 to n
 double GetRadiusForClass(unsigned int classId) const;//classId varies from 1 to n
 
 double GetRightRadiusForClass(unsigned int classId);//classId varies from 1 to n
 double GetRightRadiusForClass(unsigned int classId) const ;//classId varies from 1 to n
 
 double GetLeftRadiusForClass(unsigned int classId);//classId varies from 1 to n
 double GetLeftRadiusForClass(unsigned int classId) const;//classId varies from 1 to n

 //An object of this class will be devoted to GP and will read the initial GP Distribution needed for Thermal Loading Computing only.
 void ReadInitialDistribution(std::string);

 //Set the item value for a given class i. 
 void SetItemValueForClass(unsigned int classId, double itemValue);

 //Set the number of distibution classes. /* comment by MG: I think, this method should only increment the number of class, NOT decrease the number of class */
 void SetClassNumber(int );
 
 //Set values for all items. Values can be the same for all classes or a list of values. /*comment by MG: SetAllItemsNumbers can also be called SetItemsValues*/
 void SetAllItemsValues(std::vector<double> NP1); 
 
 void AddEmptyClass();
 
 void CoutRadiusDistribution();
 void PlotPythonHistogram();
 void PlotRadiusDistribution();
 
 double GetMinRadius() const { return minRadius_; };
 int GetInitialClassNumber() const { return initialClassNumber_; }; 
 double GetSpatialStep() const { return spatialStep_; };
 std::vector<double> GetItemsValues() { return itemsValues_; };//Gives values for all items. Values can be the same for all classes or a list of values
 const std::vector<double> GetItemsValues() const { return itemsValues_; };
 
 std::vector<double> ReturnRadiusList();
 
 
 //RELATIONS
 //getter
 const std::vector<ChemicalElement *>& GetChemicalElementsList() const {return chemicalElementList_;};
 InterfacialConcentration& GetInterfConcentrationObjectForElement(std::string elementSymbol);
 std::map<std::string, InterfacialConcentration *> GetInterfConcentrationObjectMap() {return interfConcentrationObjectMap_;};
 Precipitate& GetPrecipitate();
 Precipitate& GetPrecipitate() const;
 //Setter
 void SetPrecipitate(Precipitate& P);
 //IMPORTANT: for  chemicalElementList_ AND interfConcentrationObjectMap_ , setters are not needed.  The push_back's are made in other class
 
 
 
protected:

private:

    //Values of the class distribution (was NP1).Initialise to 0 at construction. (This attribute was previously named classValue)  Unit: Number of precipitates /m^3
    std::vector<double> itemsValues_; //or Values of all items:   itemsValues or classItemsValues ====>|value or item of class1|value or item of class2|  ...  |value or item of classN| <====
    
    //number of class at the beginning of hardening or thermal loading. Unit: No unit.Number of classes
    int const initialClassNumber_; // comment by MG:  When a file radiusDistribution is read, we create a new object radiusDistribution, and the number of class of the read file radiusDistribution becomes
                                   //                 the initialClassNumber of the new created object radiusDistribution.
     
    // int classNb_;  /* comment by MG: maybe this attribute is unuseful */
    
    //Value of the first class in the histogramme. Unit: m
    double const minRadius_;
     
    //Spatial discretisation step. unit: m
    double const spatialStep_;
    
    
    Precipitate * precipitate_;// A radius distribution belongs to 0 or one precipitate
    
    std::vector<ChemicalElement *>  chemicalElementList_;
    std::map<std::string, InterfacialConcentration *> interfConcentrationObjectMap_;
    

};


inline double 
RadiusDistribution::GetRadiusForClass(unsigned int classId)
{
  unsigned int n;
  n=itemsValues_.size();
  double R;
  assert ( (classId>=1)&&"RadiusDistribution::GetRadiusForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"RadiusDistribution::GetRadiusForClass():   ClassId must be in the range 1 to n");
  
  R=minRadius_+ spatialStep_*(classId-1);
  
  assert(R>0);
  
  return R;
  
}

inline double 
RadiusDistribution::GetRadiusForClass(unsigned int classId) const
{
  unsigned int n;
  n=itemsValues_.size();
  double R;
  assert ( (classId>=1)&&"RadiusDistribution::GetRadiusForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"RadiusDistribution::GetRadiusForClass():   ClassId must be in the range 1 to n");
  
  R=minRadius_+ spatialStep_*(classId-1);
  
  assert((R>0)&&"radius cant be negative");
  assert (R >= minRadius_);
  
  return R;
  
}


inline double
RadiusDistribution::GetRightRadiusForClass(unsigned int classId)
{
  unsigned int n;
  n=itemsValues_.size();
  double Ri, rightRadius;
  assert ( (classId>=1)&&"RadiusDistribution::GetRightRadiusForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"RadiusDistribution::GetRightRadiusForClass():   ClassId must be in the range 1 to n");
  
  Ri=this->GetRadiusForClass(classId);
  
  rightRadius=Ri+ spatialStep_/2;
  
  assert(rightRadius>0);
  
  
  return rightRadius;
  
}

inline double
RadiusDistribution::GetRightRadiusForClass(unsigned int classId) const
{
  unsigned int n;
  n=itemsValues_.size();
  double Ri, rightRadius;
  assert ( (classId>=1)&&"RadiusDistribution::GetRightRadiusForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"RadiusDistribution::GetRightRadiusForClass():   ClassId must be in the range 1 to n");
  
  Ri=this->GetRadiusForClass(classId);
  
  rightRadius=Ri+ spatialStep_/2;
  
  assert(rightRadius>0);
  
  return rightRadius;
  
}

inline double 
RadiusDistribution::GetLeftRadiusForClass(unsigned int classId)//classId varies from 1 to n
{
  unsigned int n;
  n=itemsValues_.size();
  double Ri, leftRadius;
  assert ( (classId>=1)&&"RadiusDistribution::GetLeftRadiusForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"RadiusDistribution::GetLeftRadiusForClass():   ClassId must be in the range 1 to n");
  
  Ri=this->GetRadiusForClass(classId);
  
  leftRadius=Ri - spatialStep_/2;
  
  assert(leftRadius>0);
  
  return leftRadius;
}

inline double 
RadiusDistribution::GetLeftRadiusForClass(unsigned int classId) const //classId varies from 1 to n
{
  unsigned int n;
  n=itemsValues_.size();
  double Ri, leftRadius;
  assert ( (classId>=1)&&"RadiusDistribution::GetLeftRadiusForClass():   ClassId must be in the range 1 to n");
  assert ( (classId<=n)&&"RadiusDistribution::GetLeftRadiusForClass():   ClassId must be in the range 1 to n");
  
  Ri=this->GetRadiusForClass(classId);
  
  leftRadius=Ri - spatialStep_/2;
  
  assert(leftRadius>0);
  
  return leftRadius;
}

inline Precipitate&
RadiusDistribution::GetPrecipitate()
{
  assert ( (precipitate_!=0)&&"In GetPrecipitate(): Cannot get precipitate because precipitate_ pointer is 0.\
  Radius distribution is not linked to any precipitate ");
  
  return *precipitate_;
}

inline Precipitate& 
RadiusDistribution::GetPrecipitate() const
{
  assert ( (precipitate_!=0)&&"In GetPrecipitate(): Cannot get precipitate because precipitate_ pointer is 0.\
  Radius distribution is not linked to any precipitate ");
  
  return *precipitate_;
}

inline void 
RadiusDistribution::SetPrecipitate(Precipitate& P)
{
  assert ( (precipitate_==0)&&"In SetPrecipitate(): Cannot set precipitate because precipitate_ pointer is not 0.\
  Radius distribution is already linked to another precipitate ");
  
  precipitate_=&P;
}


inline InterfacialConcentration&
RadiusDistribution::GetInterfConcentrationObjectForElement(std::string elementSymbol)
{
  std::map<std::string , InterfacialConcentration*>::iterator it;
  it=interfConcentrationObjectMap_.find(elementSymbol);
  assert( (it!=interfConcentrationObjectMap_.end())&&"Cannot find given symbol in concentrationMap_ " );
  
  
  return *(it->second);
}


inline void
RadiusDistribution::SetItemValueForClass(unsigned int classId, double itemValue)
{
  unsigned int n=itemsValues_.size();
  assert (n>0);
  assert ( (classId<=n)&&(classId>=1)&&"Given classId in argument is not in the range 1 to (size_of_RadiusDistribution)");
  
  assert ( (itemValue>=0)&&"Given itemValue argument can't be negative " );
  
  itemsValues_[classId-1]=itemValue;
}

inline void
RadiusDistribution::SetClassNumber(int n)  //Set the number of class in the distribution using itemsValues_  /* comment by MG: I think, this method should only increment the number of class, NOT decrease the number of class */
{
}

//Setter
inline void
RadiusDistribution::SetAllItemsValues(std::vector<double> NP1)//TO DO   /*comment by MG: SetAllItemsNumbers can also be called SetItemsValues*/
{
  // itemsValues_=...
}



#endif
