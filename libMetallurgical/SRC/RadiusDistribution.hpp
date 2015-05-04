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
 
 
 /*comment by MG: i think, we should have another constructor, to instantiate radiusDistribution read from a dataFile. This constructor will not take as argument a initialClassNumber,
  *but will use dataFile to compute and initialize initialClassNumber_, etc    */
 

 //The destructor
 ~RadiusDistribution();
 
 void Info() const;

 //Computes the total number(value) of items. Use to find nucleation site number
 void ComputeTotNbOfItems();

 //Return the item value for a  given class i 
 int getItemValueForClass(int i);

 //An object of this class will be devoted to GP and will read the initial GP Distribution needed for Thermal Loading Computing only.
 void ReadInitialDistribution(std::string);

 //Set the item value for a given class i. 
 void SetItemValueForClass(int i, int itemValue);

 //Set the number of distibution classes. /* comment by MG: I think, this method should only increment the number of class, NOT decrease the number of class */
 void SetClassNumber(int );
 
 //Set values for all items. Values can be the same for all classes or a list of values. /*comment by MG: SetAllItemsNumbers can also be called SetItemsValues*/
 void SetAllItemsValues(double *); 
 
 
 double GetMinRadius() const { return minRadius_; };
 int GetInitialClassNumber() const { return initialClassNumber_; }; 
 double GetSpatialStep() const { return spatialStep_; };
 double * GetItemsValues() { return itemsValues_; };//Gives values for all items. Values can be the same for all classes or a list of values
 
 
 //RELATIONS
 //getter
 std::vector<ChemicalElement *> GetChemicalElementList() const {return chemicalElementList_;};
 std::vector<InterfacialConcentration *> GetInterfConcentrationObjectList() const {return interfConcentrationObjectList_;};
 //Setter
 //IMPORTANT: for  chemicalElementList_ AND interfConcentrationObjectList_ , setters are not needed.  The push_back's are made in other class
 
 
 
protected:

private:

    //Values of the class distribution (was NP1).Initialise to 0 at construction. (This attribute was previously named classValue)  Unit: Number of precipitates /m^3
    double * itemsValues_; //or Values of all items:   itemsValues or classItemsValues ====>|value or item of class1|value or item of class2|  ...  |value or item of classN| <====
    
    //number of class at the beginning of hardening or thermal loading. Unit: No unit.Number of classes
    int const initialClassNumber_; // comment by MG:  When a file radiusDistribution is read, we create a new object radiusDistribution, and the number of class of the read file radiusDistribution becomes
                                   //                 the initialClassNumber of the new created object radiusDistribution.
     
    // int classNb_;  /* comment by MG: maybe this attribute is unuseful */
    
    //Value of the first class in the histogramme. Unit: m
    double const minRadius_;
     
    //Spatial discretisation step. unit: m
    double const spatialStep_;
    
    
    Precipitate * precipitate_;// A radius distribution belongs to 0 or one precipitate
    
    std::vector<ChemicalElement *> chemicalElementList_;
    std::vector<InterfacialConcentration *> interfConcentrationObjectList_;
    

private:
    void InitializeInterfConc(); //Process Precipitates chemical compo and data, and create interfacialConcentrationObjects associated to each chemicalElement
    
    

};


inline void
RadiusDistribution::SetItemValueForClass(int i, int itemValue)
{
}

inline void
RadiusDistribution::SetClassNumber(int n)  //Set the number of class in the distribution using itemsValues_  /* comment by MG: I think, this method should only increment the number of class, NOT decrease the number of class */
{
}

//Setter
inline void
RadiusDistribution::SetAllItemsValues(double * NP1)//TO DO   /*comment by MG: SetAllItemsNumbers can also be called SetItemsValues*/
{
  // itemsValues_=...
}

#endif
