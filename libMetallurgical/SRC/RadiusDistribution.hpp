#include <string>

class RadiusDistribution
{

public:

 //the constructor. Builds an instance with given parameters deltar and r1 (all the histogram values are initialized to 0)
 RadiusDistribution(double deltar, double r1);

 //The destructor
 ~RadiusDistribution();

 //Computes the total number of  items. Use to find nucleation site number
 void ComputeTotNbOfItems();

 //Return the item number for class i 
 int getItemNumberForClass();

 //An object of this class will be devoted to GP and will read the initial GP Distribution needed for Thermal Loading Computing only.
 void ReadInitialDistribution(std::string);

 //Set the item number for class i. 
 void SetItemNumberForClass(int i, int itemNumber);

 //Gives values for all the item numbers. Values can be the same for all classes or a list of values
 void SetAllItemsNumbers(double *);

 //Set the number of distibution classes
 void SetClassNumber(int );

protected:

private:

    //Values of the class distribution (was NP1).Initialise to 0 at construction. Unit: Number of precipitates /m^3
    double * classValues_;
    
    //number of class at the beginning of maturation or thermal loading. Unit: No unit.Number of classes
    int initialClassNumber_;
    
    //Value of the first class in the histogramme. Unit: m
    double minRadius_;
    
    //Spatial discretisation step. unit: m
    double spatialStep_;

};

inline void
RadiusDistribution::SetItemNumberForClass(int i, int itemNumber)
{
}

inline void
RadiusDistribution::SetAllItemsNumbers(double * NP1)
{
}

inline void
RadiusDistribution::SetClassNumber(int n)
{
}