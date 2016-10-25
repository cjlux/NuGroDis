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
// Copyright (C) 2014-2015 Jean-luc CHARLES, Dominique COUPARD, Moubarak GADO, Ivan IORDANOFF.
//

#include <iostream>

#include <vector>
#include <map>
#include <cassert>
#include <cmath>
#include <iomanip> //std::setw
#include <fstream>
#include <iterator>
#include <algorithm>

#include "Material.hpp"
#include "Precipitate.hpp"
#include "Computation.hpp"
#include "RadiusDistribution.hpp"
#include "Grain.hpp"
#include "Polynomial.hpp"
#include "Concentration.hpp"
#include "ChemicalComposition.hpp"
#include "ChemicalElement.hpp"
#include "ThermoDynamicsConstant.hpp"
#include "Temperature.hpp"
#include "Diffusion.hpp"
#include "Util.hpp"
#include "InterfacialConcentration.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/bindings/lapack/gesv.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>



#include <limits>


namespace ublas  = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;



Precipitate::Precipitate(Material& mat,ChemicalComposition &CC, RadiusDistribution &initialRD)
  :Grain(mat,CC),
   deltaCell_(-1),
   solvusActivationEnergy_(-1),
   distorsionEnergy_(-1),
   phaseChangeVolumiqueEnergy_(1),
   nucleationSitesNumber_(-1),
   initialNucleationSitesNumber_(-1),
   preExpTermForSolvus_(-1),
   surfaceEnergyPolynomial_(),
   surfaceEnergyCurrentValue_(0),
   shapeFactor_(-1),
   volumicFraction_(0),
   criticalRadius_(-1),
   deltaCriticalRadius_(-1),
   criticalTotalEnergy_(-1),
   ZeldovichFactor_(-1),
   criticalBeta_(-1),
   nucleationRate_(-1),
   radiusDistributionList_(),
   currentRadiusDistribution_(0),
   initialRadiusDistribution_(initialRD),
   sumOfStoicCoefs_(0),
   SumOfStoiCoefsHasBeenComputed_(false),
   solubilityProduct_(-1),
   equilibriumConcMap_(),
   criticalInterfacialConcentrationRadiusFactor_(0.1) 
   /*debug*/,FirstNotEmptyClassIdAfterNucleation(-1)
{
  std::cout<<"Build a C++ Precipitate object"<<std::endl;
  
  currentRadiusDistribution_=&initialRD;
  currentRadiusDistribution_->SetPrecipitate(*this);
  radiusDistributionList_.push_back(currentRadiusDistribution_);
  
  //TODO :search and assert *this in (mat.precipitateList_) . if *this not in (mat.precipitateList_) then Do:  mat.AddPrecipitate(*this)
  mat.AddPrecipitate(*this);
 
  
  std::cout<<"Exit contructor of Precipitate Object"<<std::endl;  
  
}

//TODO
Precipitate::~Precipitate()
{

}



void 
Precipitate::InitializeEquilibriumConcentrationMap()
{
  std::cout<<"Initializing a precipitate equilibrium concentration map"<<std::endl;
  
  
  assert( (equilibriumConcMap_.size()==0)&&"In InitializeEquilibriumConcentrationMap() for precipitate,\
  equilibriumConcMap_ has already been initialized") ;
  
  std::map<std::string , Concentration*>::iterator it;
  
  for (it=chemicalComposition_.GetConcentrationMap().begin(); it!=chemicalComposition_.GetConcentrationMap().end(); ++it )
  {
    if ( (it->first)!= (materialPointer_->GetMainChemicalElement().GetElementName()) )
    {
      equilibriumConcMap_[it->first]=-1; //initialize precipitate equilibrium concentratations
    };  
  }

  
}


//Return firstNotEmptyClassIdAfterNucleation
unsigned int
Precipitate::ReturnRDFirstNotEmptyClassIdAfterNucleation()
{
  unsigned int firstNotEmptyClassIdAfterNucleation;
  
  assert ((currentRadiusDistribution_!=0)&&"Precipitate do not have any Current RadiusDistribution");
  
  unsigned int n= currentRadiusDistribution_->GetItemsValues().size();
  
  double nucleusRadius=criticalRadius_+deltaCriticalRadius_;
  
  double RDminRadius= currentRadiusDistribution_->GetMinRadius();
  
  double RDmaxRadius= currentRadiusDistribution_->GetRightRadiusForClass(n);
  
  std::vector<unsigned int> ListOfUnemptyClassId;
  assert(ListOfUnemptyClassId.size()==0);

  
  for (unsigned int i=1; i<=n; ++i )
  {
    double Ni= currentRadiusDistribution_->GetItemValueForClass(i);
    if (Ni>0)
    {
      ListOfUnemptyClassId.push_back(i);
    } 
  }
  
  if ( (nucleusRadius>RDminRadius)&&(nucleusRadius<RDmaxRadius) ) 
  {
    unsigned int nucleusClassId = currentRadiusDistribution_->FindClassForRadius(nucleusRadius);
    
    ListOfUnemptyClassId.push_back(nucleusClassId);
  }
  
  
  if (ListOfUnemptyClassId.size()==0)
  {
    //means all class are empty and nucleus radius is not in the radius distribution
    
    std::cout<<"In Precipitate::ReturnRDFirstNotEmptyClassIdAfterNucleation(): all class are empty and nucleus radius is not in the radius distribution therefore, firstNotEmptyClassIdAfterNucleation will be 1 (the first class of the distribution) \n";
    firstNotEmptyClassIdAfterNucleation=0; // All class are empty even after nucleation, then there is no classId  ==> classId=0.
  }
  else
  {
    //Means there is at least one unempty class
    
    std::vector<unsigned int>::const_iterator it_MinValue;
    it_MinValue = std::min_element(ListOfUnemptyClassId.begin(), ListOfUnemptyClassId.end());
    
    firstNotEmptyClassIdAfterNucleation=*it_MinValue;
    
    assert (firstNotEmptyClassIdAfterNucleation >=1);
    
  };

  
  
 return firstNotEmptyClassIdAfterNucleation;
}



void
Precipitate::SavePrecipitateAttributes()
{

  std::string precipitateType= this->GetPrecipitateType();
  
  std::string relativeSaveFolderPath="PrecipitatesAttributes/"+precipitateType;
  materialPointer_->GetComputation().CreateDirectory(relativeSaveFolderPath);
  
  
  std::string ResultsDirectoryPath= materialPointer_->GetComputation().GetResultsDirectory();
  
  
  std::string fileName= precipitateType+"_Attributes_"+".txt";
  std::string path=ResultsDirectoryPath+"/"+relativeSaveFolderPath+"/"+fileName;
  std::ofstream output_file;
  output_file.open(path.c_str(), std::ios_base::app);
  
  
  
  
  //Check if file is empty
  
  std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
  
  
  std::stringstream line;
  double CurrentTime = materialPointer_->GetComputation().GetCurrentTime();
  
  double CurrentTemperature= materialPointer_->GetTemperature().GetCurrentTemp();
  
  std::vector<std::string> lineStringVector;
  
  assert (lineStringVector.size()==0);
  
  std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
  
  if ( in.tellg() == 0 /*check if it is empty*/      )
  {
    // file is empty
    
    line<<"time"<<"\t"<<"Vf"<<"\t"<<"DeltaGv"<<"\t"<<"DeltaGe"<<"\t"<<"NuclSiteDensity"<<"\t"<<"gamma"<<"\t"<<"rEtoile"<<"\t"<<"delta_rEtoile"<<"\t"<<"Real_rEtoile"<<"\t"<<"Delta_GEtoile"<<"\t"<<"Z"<<"\t"<<"BetaEtoile"<<"\t"<<"JEtoile"; 
    
    std::map<std::string, double>::iterator it;
    for (it=equilibriumConcMap_.begin(); it!=equilibriumConcMap_.end(); ++it )
    {
      if ( it->first != mainElementSymbol  )
      {
	line<<"\t"<<"Xv"+it->first+"EqSS"; 
      };
      
    }
    
    line<<"\t"<<"FistNotEmptyClassAfterNucleation";
    
    /////////////////////////////////////////////
    /////// Add new attribute label here ////////
    /////////////////////////////////////////////
    
    // by doing  ===>   line<<"\t"<<"NewAttributeLabelName";
  
    line<<"\t"<<"AverageRadius";
    
    line<<"\t"<<"Temperature";
    
    /////////////////////////////////////////////
    /////////////////////////////////////////////

    
    line<<"\n";
    
    lineStringVector.push_back(line.str());
    assert (lineStringVector.size()==1);
  }
  
  std::stringstream lineStream;
    
  lineStream<<CurrentTime<<"\t"<<volumicFraction_<<"\t"<<phaseChangeVolumiqueEnergy_<<"\t"<<distorsionEnergy_<<"\t"<<nucleationSitesNumber_<<"\t"<<surfaceEnergyCurrentValue_<<"\t"<<criticalRadius_<<"\t"<<deltaCriticalRadius_<<"\t"<<deltaCriticalRadius_+criticalRadius_<<"\t"<<criticalTotalEnergy_<<"\t"<<ZeldovichFactor_<<"\t"<<criticalBeta_<<"\t"<<nucleationRate_;
  
  std::map<std::string, double>::iterator it;
  for (it=equilibriumConcMap_.begin(); it!=equilibriumConcMap_.end(); ++it )
  {
    if ( it->first != mainElementSymbol  )
    {
     lineStream<<"\t"<<it->second; 
    };
  }
  
  lineStream<<"\t"<<FirstNotEmptyClassIdAfterNucleation;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////// ADD NEW ATTRIBUTES HERE ////// do not forget to add the label before (see few line above)///////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // by doing  ===>  lineStream<<"\t"<<NewAttributeVariable;
  
  lineStream<<"\t"<<currentRadiusDistribution_->ReturnAverageRadius();
  
  lineStream<<"\t"<<CurrentTemperature;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////  
  
  
  
  lineStream<<"\n";
    
  lineStringVector.push_back(lineStream.str());
  
  

  
  
  std::ostream_iterator<std::string> output_iterator(output_file, "\n");
  std::copy(lineStringVector.begin(), lineStringVector.end(), output_iterator);
  
  
  
  
}




void
Precipitate::SavePrecipitateInterfacialConcentrations()
{

  std::string precipitateType= this->GetPrecipitateType();
  
  std::string relativeSaveFolderPath="PrecipitatesInterfacialProperties/"+precipitateType;
  materialPointer_->GetComputation().CreateDirectory(relativeSaveFolderPath);

  
  std::string ResultsDirectoryPath= materialPointer_->GetComputation().GetResultsDirectory();
  
  
  std::string fileName= precipitateType+"_InterfacialConcentrations_"+".txt";
  std::string path=ResultsDirectoryPath+"/"+relativeSaveFolderPath+"/"+fileName;
  std::ofstream output_file;
  output_file.open(path.c_str(), std::ios_base::app);
  
 
  
  
   
  assert ((currentRadiusDistribution_!=0)&&"Precipitate is not linked to any current Radius Dstribution");
  
  InterfacialConcentration * IntConcObjUsed=0;
   
  IntConcObjUsed = & ( currentRadiusDistribution_->GetInterfacialConcentrationObjectUsed() );
  assert (IntConcObjUsed!=0);
   
  std::string chemicalElementUsedName= IntConcObjUsed->GetChemicalElement().GetElementName();
  
  
  //Check if file is empty
  std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
  std::stringstream line;
  std::vector<std::string> lineStringVector;
  assert (lineStringVector.size()==0);
  if ( in.tellg() == 0 /*check if it is empty*/      )
  {
    // file is empty
    line<<"Chemical Element used is : "<<chemicalElementUsedName<<"\n";
    line<<"time"<<" >>>>>>>\t \t"<<"List Of XvINT"<<"\n"; 
    
    lineStringVector.push_back(line.str());
    assert (lineStringVector.size()==1);
  }
   
  
  double CurrentTime = materialPointer_->GetComputation().GetCurrentTime();
   
  std::stringstream lineStream;
  unsigned int NumberOfClass = currentRadiusDistribution_->GetItemsValues().size();
  
  assert ( NumberOfClass+1==IntConcObjUsed->GetInterfacialConcentrationValuesList().size() );
   
  lineStream<<CurrentTime<<" >>>>>>>\t \t";
  
  
  
  
  for (unsigned int i=0; i<IntConcObjUsed->GetInterfacialConcentrationValuesList().size(); ++i )
  {
    if (i==0)
    {
      //Add The left interface of the first class (class1) 
      lineStream<< IntConcObjUsed->GetLeftInterfacialConcValueForClass(i+1);
    }
    else
    {
      //Add right interface value
      assert(i!=0);
      lineStream<<"\t"<<IntConcObjUsed->GetRightInterfacialConcValueForClass(i);
    };
    
  }
  
  lineStream<<"\n\n";  
  lineStringVector.push_back(lineStream.str());
   
  
  
  
  
  std::ostream_iterator<std::string> output_iterator(output_file, "\n");
  std::copy(lineStringVector.begin(), lineStringVector.end(), output_iterator);
   
  
}

void
Precipitate::SavePrecipitateInterfacialVelocities()
{
  
  if (volumicFraction_>0)
  {
    std::string precipitateType= this->GetPrecipitateType();
    
    std::string relativeSaveFolderPath="PrecipitatesInterfacialProperties/"+precipitateType;
    materialPointer_->GetComputation().CreateDirectory(relativeSaveFolderPath);
    
    std::string ResultsDirectoryPath= materialPointer_->GetComputation().GetResultsDirectory();
    
    
    std::string fileName= precipitateType+"_InterfacialVelocities_"+".txt";
    std::string path=ResultsDirectoryPath+"/"+relativeSaveFolderPath+"/"+fileName;
    std::ofstream output_file;
    output_file.open(path.c_str(), std::ios_base::app);
    
  
    
    
    
    assert ((currentRadiusDistribution_!=0)&&"Precipitate is not linked to any current Radius Dstribution");
    
    InterfacialConcentration * IntConcObjUsed=0;
    
    IntConcObjUsed = & ( currentRadiusDistribution_->GetInterfacialConcentrationObjectUsed() );
    assert (IntConcObjUsed!=0);
    
    std::string chemicalElementUsedName= IntConcObjUsed->GetChemicalElement().GetElementName();
    
    
    //Check if file is empty
    std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
    std::stringstream line;
    std::vector<std::string> lineStringVector;
    assert (lineStringVector.size()==0);
    if ( in.tellg() == 0 /*check if it is empty*/      )
    {
      // file is empty
      line<<"Chemical Element used is : "<<chemicalElementUsedName<<"\n";
      line<<"time"<<" >>>>>>>\t \t"<<"List Of VINT"<<"\n"; 
      
      lineStringVector.push_back(line.str());
      assert (lineStringVector.size()==1);
    };
    
    double CurrentTime = materialPointer_->GetComputation().GetCurrentTime();
    
    std::stringstream lineStream;
    unsigned int NumberOfClass = currentRadiusDistribution_->GetItemsValues().size();
    
    //TODO debug std::cout<<"NumberOfClass toto"<<NumberOfClass<<" IntConcObjUsed size " <<IntConcObjUsed->GetInterfacialVelocityList().size();
    unsigned int toto= IntConcObjUsed->GetInterfacialVelocityList().size();
    
    std::cout<<"Type : "<<this->GetPrecipitateType()<<"\n";
    std::cout<< "toto tototo"<<toto<<"\n";
    std::cout<< "NumberOfClass tototo"<<NumberOfClass<<"\n";
    
    assert ( (NumberOfClass + 1) ==IntConcObjUsed->GetInterfacialVelocityList().size() );
    
    lineStream<<CurrentTime<<" >>>>>>>\t \t";
    
    
    
    
    for (unsigned int i=0; i<IntConcObjUsed->GetInterfacialVelocityList().size(); ++i )
    {
      if (i==0)
      {
	//Add The left interface of the first class (class1) 
	lineStream<< IntConcObjUsed->GetLeftInterfacialVelocityForClass(i+1);
      }
      else
      {
	//Add right interface value
	assert(i!=0);
	lineStream<<"\t"<<IntConcObjUsed->GetRightInterfacialVelocityForClass(i);
      };
      
    }
    
    lineStream<<"\n\n";  
    lineStringVector.push_back(lineStream.str());
    
    
    
    
    
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(lineStringVector.begin(), lineStringVector.end(), output_iterator);
    
  }//ENd If volumicfraction_>0
  
  
  
  
  
}








double 
Precipitate::GetEquilibriumConcentrationForElement(std::string elementSymbol)
{
  assert ( (elementSymbol!=materialPointer_->GetMainChemicalElement().GetElementName() )&&"In precipitate,GetEquilibriumConcentrationForElement. Cannot get the equilibrium \
  concentration of the main chemical element!!!. main element does not have equilib. conc.!!! " );
  
  
  
  std::map<std::string , double>::iterator it;
  it=equilibriumConcMap_.find(elementSymbol);
  assert( (it!=equilibriumConcMap_.end())&&"Cannot find given symbol in equilibriumConcMap_ " );
  
  assert ( (it->second!=-1)&&"In precipitate, Cannot get equilibrium concentration because it has not been computed yet");
     

  
  return it->second;
}


void
Precipitate::AddEmptyClassForCurrentRadiusDistributionWithCondition()
{
  assert ((currentRadiusDistribution_!=0)&&"Precipitate is not linked to any current radius distribution");
  unsigned int n=currentRadiusDistribution_->GetItemsValues().size();
  double lastValue = currentRadiusDistribution_->GetItemValueForClass(n);
  
  
  if ( lastValue>0 )
  {
    std::cout<<"volumic fraction is"<<volumicFraction_<<"\n";
    std::cout<<"precipitate typme is "<<this->GetPrecipitateType()<<"\n";
    currentRadiusDistribution_->AddEmptyClass();
  }
  
}

//  Solve system of equations like:
// (XvSSi-X)/(XvPi-X)= DjOverDi *(XvSSj-Y)/(XvPj-Y) AND   X*Y= f 
// with unknown variables X>0 ,Y>0   
//X value will be return in variable X and Y value in Y.
void
Precipitate::SolveEquilibriumConcentrationsEquations(double f,
					double XvSSi,
					double XvPi,
					double DjOverDi, 
					double XvSSj,
					double XvPj,
					double & X,
					double & Y)
{
  
  std::cout<<"Solving EquilibriumConcentrations Equations "<<std::endl;
  
  
  double constantA, constantB, constantC, constantD;
  std::complex<double> solution1,solution2;
  X=-1,Y=-1; // -1 means variable is not computed yet
  int nbOfSol;//Nber of solutions
  
  //System of equation is: 
  //                       (XvSSi-X)/(XvPi-X)= DjOverDi * (XvSSj-Y)/(XvPj-Y);
  //                 AND   X*Y= K*Exp(-DeltaH/R*T);
  //DjOverDi= Dj/Di
  // system to solve:  1)  j=(A*i+B)/(C*i+D);  2)  i*j=f
  constantA= DjOverDi*XvSSj-XvPj;
  constantB= XvSSi*XvPj-DjOverDi*XvPi*XvSSj;
  constantC= DjOverDi-1.;
  constantD= XvSSi-DjOverDi*XvPi;
  
  //j= fction(i)
  //with i the unknown variable, 2nd order Equation to solve is then: A*i^2 +(B-fC)*i -fD=0
  
  nbOfSol=Util::Util::SolveSecondDegreeEquation(constantA, constantB-f*constantC, -f*constantD,
						solution1,
						solution2,
						false); // 6th argument is false because we dont want complex solutions!!!!
  
  if (nbOfSol==2)
  {
    
    std::cout<<"DEBUG solution1= "<<solution1<<" ; solution2 ="<<solution2<<std::endl;
    if ( (solution1.real())==0 && (solution2.real())==0 )
      assert("Error: No solutions , X=Y=0 ");
    if ( solution1.real()*solution2.real()<0 )
    {
      if (solution1.real()>0)
	X=solution1.real();
      
      if (solution2.real()>0)
	X=solution2.real();
      
    }
    else if (solution1.real()*solution2.real()==0)
    {
      if ( (solution1.real())==0 )
	X=solution2.real();
      if ( (solution2.real())==0 )
	    X=solution1.real();
      
    }
    else // solution1.real()*solution2.real()>0
    {
      //Do not forget to delete this assertion later!  
      //if 2 solution of X, maybe one of them will give a negative or forbiden value for Y. Then we have to discuss. 
      //TODO
      
      if( (solution1.real()>0) && (solution1.real()<=XvSSi) && (solution2.real()>0) && (solution2.real()<=XvSSi) )
      {
	assert (!("TO DO. This case has not been implemented yet"));
      }
      else if ( (solution1.real()>0) && (solution1.real()<=XvSSi)  )
      {
	X=solution1.real();
      }
      else if ( (solution2.real()>0) && (solution2.real()<=XvSSi)  )
      {
	X=solution2.real();
      }
      else
      {
	assert (!("TO DO. This case has not been implemented yet"));
      }
      
    };
    
  };
  if (nbOfSol==1)
  {
    X=solution1.real();
  };
  
  assert ( (X!=-1 )&&"Solution X may have not been computed" );
  assert( X > 0);
  assert( X != -constantD/constantC );
  assert( X != XvPi); //Denominator not null
  Y = (constantA*X +constantB)/(constantC*X +constantD);  
  assert ( (Y!=-1 )&&"Solution Y may have not been computed" );
  assert( Y>0);
  assert( Y!= XvPj);//Denominator not null
  
  
  //DEBUG    
  std::cout<<"Xveq value solution X is: "<<X<<std::endl;
  std::cout<<"value solution Y is: "<<Y<<std::endl;
  
  std::cout<<" Ax2 +bx +c =0 ? "<< constantA*X*X + (constantB-f*constantC)*X + -f*constantD<<std::endl;
  std::cout<<"value solution Y is: "<<Y<<std::endl;
    std::cout<<"X*Y = : "<<X*Y<<std::endl;
    std::cout<<"f = : "<<f<<std::endl;
    std::cout<<" (XvSSi-X)/(XvPi-X) =  "<<(XvSSi-X)/(XvPi-X)<<std::endl;
    std::cout<<"DjOverDi *(XvSSj-Y)/(XvPj-Y) = "<<DjOverDi *(XvSSj-Y)/(XvPj-Y)<<std::endl;
    
    
     ////////
     //post assertions
     /////////
    
    //Check precision of solutions X,Y found
    double epsilon=1e-10;
    double Eq1precision=std::abs(X*Y-f);
    double Eq2precision=std::abs( (XvSSi-X)/(XvPi-X) - DjOverDi*(XvSSj-Y)/(XvPj-Y) );
    std::cout<<"Precision for equation 1: X*Y=f is "<<Eq1precision<<"\n";
    std::cout<<"Precision for equation 2: (XvSSi-X)/(XvPi-X) = DjOverDi *(XvSSj-Y)/(XvPj-Y) is "<<Eq2precision<<"\n";
	  
    assert ( (Eq1precision<= epsilon) && "Precision for equation X*Y=f is not satisfied" );
    assert ( (Eq2precision<= epsilon) && "Precision for equation (XvSSi-X)/(XvPi-X) = DjOverDi *(XvSSj-Y)/(XvPj-Y)  is not satisfied " );
    
    
    
    
    
}




//TEST delete if function SolveEquilibriumConcentrationsEquations(...) works well
// //Compute equilibrium concentrations XvSSeq_i
// void 
// Precipitate::ComputeEquilibriumConcentrations()
// {
//   //if Chemical composition has 3 elements( like Al_a__Cu_x__Mg_y)
//   if (equilibriumConcMap_.size()==3)
//   {
//     std::map<std::string, Concentration*> precipitateConcMap=chemicalComposition_.GetConcentrationMap();
//     std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
//     std::map<std::string, Concentration*>::iterator it;
//     std::map<std::string, int> stoiCoefMap;
//     std::vector<int> AlloyingElementsStoiCoefsList;
//     
//     
//     
//   
//     for (it=precipitateConcMap.begin(); it!=precipitateConcMap.end(); ++it )
//     {
//       if ( it->first != mainElementSymbol  )
//       {
// 	AlloyingElementsStoiCoefsList.push_back( it->second->GetStoichiometricCoef() );
//       };
//     }
//     assert ( (AlloyingElementsStoiCoefsList.size()==2)&&"AlloyingElementsStoiCoefsList size is not equal to 2");
//     
//     
//     
//     assert( (AlloyingElementsStoiCoefsList[0]==1) && (AlloyingElementsStoiCoefsList[1]==1) && "Alloy is not like A_a__B_x__C_y with x=1 AND y=1!!!  Example: Al8CuMg, Al2CuMg, ... "  );
//     //if x=1 and y=1. Exple: Al8CuMg, Al2CuMg,... 
//     if ( (AlloyingElementsStoiCoefsList[0]==1)&&(AlloyingElementsStoiCoefsList[1]==1) )
//     {
//       double K=preExpTermForSolvus_;
//       double DeltaH=solvusActivationEnergy_;
//       double R=ThermoDynamicsConstant::GetR();
//       double T=materialPointer_->GetTemperature().GetCurrentTemp();
//       std::vector<const ChemicalElement*> soluteList =materialPointer_->GetSoluteList();
//       
//       assert ((soluteList.size()==2)&&"Solute list must have 2 chemical element");
//       assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"Division by 0");//soluteList[0] is X or i and soluteList[1] is Y or j
//       assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"AtomicDiffusion coef is 0: No sense");
//       double f=K*std::exp(-DeltaH/(R*T));
//       
//       //soluteList[0] is X or i and soluteList[1] is Y or j
//       double DjOverDi= soluteList[1]->GetDiffusion().GetAtomicDiffusionCoef()/soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef();// example: D_Mg_SS/D_Cu_SS
//       
//       double XvPj=precipitateConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
//       double XvPi=precipitateConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
//       std::map<std::string, Concentration*> materialCurrentConcMap= materialPointer_->GetCurrentChemicalComposition().GetConcentrationMap();
//       double XvSSj =materialCurrentConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
//       double XvSSi =materialCurrentConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
//       
//       
//       
//       
//       
//       double constantA, constantB, constantC, constantD;
//       
//       //System of equation is: 
//       //                       (XvSSi-X)/(XvPi-X)= DjOverDi * (XvSSj-Y)/(XvPj-Y);
//       //                 AND   X*Y= K*Exp(-DeltaH/R*T);
//       //DjOverDi= Dj/Di
//       // system to solve:  1)  j=(A*i+B)/(C*i+D);  2)  i*j=f
//       constantA= DjOverDi*XvSSj-XvPj;
//       constantB= XvSSi*XvPj-DjOverDi*XvPi*XvSSj;
//       constantC= DjOverDi-1;
//       constantD= XvSSi-DjOverDi*XvPi;
//       
//      
//       //j= fction(i)
//       //with i the unknown variable, 2nd order Equation to solve is then: A*i^2 +(B-fC)*i -fD=0
//       std::complex<double> solution1,solution2;
//       double X=-1,Y=-1; // -1 means variable is not computed yet
//       int nbOfSol;//Nber of solutions
//       nbOfSol=Util::Util::SolveSecondDegreeEquation(constantA, constantB-f*constantC, -f*constantD,
// 					    solution1,
// 					    solution2,
// 					    false); // 6th argument is false because we dont want complex solutions!!!!
//       
//       if (nbOfSol==2)
//       {
// 	if ( (solution1.real())==0 && (solution2.real())==0 )
// 	  assert("Error: No solutions for equilibrium concentrations");
// 	
// 	if ( solution1.real()*solution2.real()<0 )
// 	{
// 	  if (solution1.real()>0)
// 	    X=solution1.real();
// 	  
// 	  if (solution2.real()>0)
// 	    X=solution2.real();
// 	}
// 	else if (solution1.real()*solution2.real()==0)
// 	{
// 	  if ( (solution1.real())==0 )
// 	    X=solution2.real();
// 	  
// 	  if ( (solution2.real())==0 )
// 	    X=solution1.real();
// 	}
// 	else if (solution1.real()*solution2.real()>0)
// 	{
// 	  assert ("TO DO. This case has not been implemented yet");//Do not foget to delete this assertion later!
// 	  
// 	  //if 2 solution of X, maybe one of them will give a negative or forbiden value for Y. Then we have to discuss. 
// 	  //TODO
// 	};
//       };
//       
//       if (nbOfSol==1)
//       {
// 	X=solution1.real();
//       };
//       
//       assert ( (X!=-1 )&&"Equilibrium concentration may have not been computed" );
//       assert( X > 0);
//       assert( X != -constantD/constantC );
//       assert( X != XvPi);
//       Y = (constantA*X +constantB)/(constantC*X +constantD);  
//       assert ( (Y!=-1 )&&"Equilibrium concentration may have not been computed" );
//       assert( Y>0);
//       assert( Y!= XvPi);
//       
//       //Remember: soluteList[0] is X or i and soluteList[1] is Y or j
//       
//     };
//     
//     
//     
//   };
//   
//   
//   assert(  (equilibriumConcMap_.size()==3)&&"Resolution for alloy other than ternary alloy has not been implemented yet. "  );//TODO later....,
//   
// 
//   
// }



//Compute equilibrium concentrations XvSSeq_i
void 
Precipitate::ComputeEquilibriumConcentrations()
{
  //if Chemical composition has 3 elements  A_a__B_x__C_y ( like Al_a__Cu_x__Mg_y)
  if (equilibriumConcMap_.size()==2) 
  {
    
    std::map<std::string, Concentration*> precipitateConcMap=chemicalComposition_.GetConcentrationMap();
    std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
    std::map<std::string, Concentration*>::iterator it;
    std::map<std::string, int> stoiCoefMap;
    std::vector<int> AlloyingElementsStoiCoefsList;
    
    
    
  
    for (it=precipitateConcMap.begin(); it!=precipitateConcMap.end(); ++it )
    {
      if ( it->first != mainElementSymbol  )
      {
	AlloyingElementsStoiCoefsList.push_back( it->second->GetStoichiometricCoef() );
      };
    }
    assert ( (AlloyingElementsStoiCoefsList.size()==2)&&"AlloyingElementsStoiCoefsList size is not equal to 2");
    
    
    
    assert( (AlloyingElementsStoiCoefsList[0]==1) && (AlloyingElementsStoiCoefsList[1]==1) && "Alloy is not like\
    A_a__B_x__C_y with x=1 AND y=1!!!  Example: Al8CuMg, Al2CuMg, ... "  );
    //if x=1 and y=1. Exple: Al8CuMg, Al2CuMg,... 
    if ( (AlloyingElementsStoiCoefsList[0]==1)&&(AlloyingElementsStoiCoefsList[1]==1) )
    {
      double K=preExpTermForSolvus_;
      double DeltaH=solvusActivationEnergy_;
      double R=ThermoDynamicsConstant::GetR();
      double T=materialPointer_->GetTemperature().GetCurrentTemp();
      std::vector<const ChemicalElement*> soluteList =materialPointer_->GetSoluteList();
      
      assert ((soluteList.size()==2)&&"Solute list must have 2 chemical element");
      assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"Division by 0");//soluteList[0] is X or i and soluteList[1] is Y or j
      assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"AtomicDiffusion coef is 0: No sense");
      
      double f=K*std::exp(-DeltaH/(R*T));
      
      
      double DjOverDi= soluteList[1]->GetDiffusion().GetAtomicDiffusionCoef()/soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef();// example: D_Mg_SS/D_Cu_SS
      //Because of ratio DjOverDi , solution for soluteList[0] will be X or i and solution for soluteList[1] will be Y or j
      double solX=-1, solY=-1;
      double XvPj=precipitateConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
      double XvPi=precipitateConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
      std::map<std::string, Concentration*> materialCurrentConcMap= materialPointer_->GetCurrentChemicalComposition().GetConcentrationMap();
      double XvSSj =materialCurrentConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
      double XvSSi =materialCurrentConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
      
      
      //solving system of equation:  (XvSSi-solX)/(XvPi-solX)= DjOverDi *(XvSSj-solY)/(XvPj-solY) AND   X*Y= f 
      //with unknown variables solX>0 , solY>0   
      SolveEquilibriumConcentrationsEquations(f,
					      XvSSi,
					      XvPi,
					      DjOverDi,
					      XvSSj,
					      XvPj,
					      solX,
					      solY);
      
      /*Debug*/ std::cout<<"f: "<<f<<" XvSSi: "<<XvSSi<<" XvPi: "<<XvPi<<" DjOverDi: "<<DjOverDi<<" XvPj: "<<XvPj<<" XvSSj: "<<XvSSj<<" solX"<<solX<<" solY"<<solY<<std::endl; 
      /*Debug*/ std::cout<<"get diffusion coef "<<soluteList[1]->GetDiffusion().GetAtomicDiffusionCoef()<<std::endl;
      
      
      //Remember: XvSSeq for soluteList[0] is X or i and XvSSeq for soluteList[1] is Y or j
      equilibriumConcMap_[soluteList[0]->GetElementName()]=solX;
      equilibriumConcMap_[soluteList[1]->GetElementName()]=solY;      
      
    }
    else
    {
      //TODO later
      assert(!("Resolution for ternary alloy with alloying elements stoichiometry other than 1 has not been implemented yet!!!\
       example: AlxCu1Mg1 is GOOD, but,  AlxCu2Mg2, AlxCu1Mg3, AlxCu2Mg3 are not WORKING "));
    }
    
    
    
  };
  
  
  assert(  (equilibriumConcMap_.size()==2)&&"Resolution for alloy other than ternary alloy has not been implemented yet. "  );//TODO later....,
  

  
}


bool 
Precipitate::CheckIfPrecipitationIsPossible() const
{
  double R=ThermoDynamicsConstant::GetR();
  double T=materialPointer_->GetTemperature().GetCurrentTemp();
  std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
  
  double DeltaH= solvusActivationEnergy_;
  double K_infini=preExpTermForSolvus_;
  double K_eq= K_infini*std::exp(-DeltaH/(R*T));
  
  double K_SS, product=1.;
  
  std::map<std::string, Concentration*>::iterator it;
  std::map<std::string, Concentration*> precipitateConcMap=chemicalComposition_.GetConcentrationMap();
  std::map<std::string, Concentration*> materialCurrentConcMap= materialPointer_->GetCurrentChemicalComposition().GetConcentrationMap();
  
  
  for (it=precipitateConcMap.begin(); it!=precipitateConcMap.end(); ++it )
  {
    if ( it->first != mainElementSymbol  )
    {
      int stoiCoef= it->second->GetStoichiometricCoef();
      std::cout<<"TPOTOTOTOTOTOTOTTO1\n";
      double XvSS=  materialCurrentConcMap[it->first]->GetVolumicValue();
      std::cout<<"TPOTOTOTOTOTOTOTTO2\n";
      assert ( (XvSS>0)&&"Current volumic concentration in material must be superior to 0");
      
      product *= std::pow(XvSS,stoiCoef); 
    }
  }
  
  K_SS=product;
  
  std::cout<<"Solid solution current solubility product is"<<K_SS<<std::endl;
  
  bool IsPrecipitationPossible = (K_eq <= K_SS);
  
  //If IsPrecipitationPossible== true then precipitation is possible
  
  /*DEbug*/ std::cout<<"IsPrecipitationPossible "<<IsPrecipitationPossible<<"\n";
  
  return IsPrecipitationPossible;
}

//Compute and Set distorsion Energy
void 
Precipitate::ComputeDistorsionEnergy()
{
// WARNING youngModulus_ unit must be MPa
  assert((deltaCell_!=-1)&&"Cannot compute Distorsion energy: Precipitate's deltaCell has not been defined");
  
  distorsionEnergy_=1000000*(materialPointer_->GetYoungModulus())/(1-materialPointer_->GetPoissonCoeff())*(deltaCell_)*(deltaCell_);
  
  /*DEBUG */std::cout<<"Deistorsion energy is "<<distorsionEnergy_<<"For precipitate type <"<<typeid(*this).name()<<"> "<<std::endl;
}


//TODO
//Compute and set Gibbs Free Energy for phase change (was DeltaGv)
void 
Precipitate::ComputePhaseChangeVolumicEnergy()
{ 
  
  std::cout<<"/////////////////////COMPUTING PHASE CHANGE VOLUMIC ENERGY////////////////////////////"<<std::endl;
  
  bool IsPrecipitationPossible= this->CheckIfPrecipitationIsPossible();
  
  if (IsPrecipitationPossible)
  {
	double R=ThermoDynamicsConstant::GetR();
	double T=materialPointer_->GetTemperature().GetCurrentTemp();
	std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
	std::map<std::string, Concentration*> materialCurrentConcMap= materialPointer_->GetCurrentChemicalComposition().GetConcentrationMap();
	std::map<std::string, Concentration*> precipitateConcMap=chemicalComposition_.GetConcentrationMap();
	std::map<std::string, Concentration*>::iterator it;
	double product=1.;
	
	for (it=precipitateConcMap.begin(); it!=precipitateConcMap.end(); ++it )
	{
	  
	  if ( it->first != mainElementSymbol  )
	  {
	    assert( ( equilibriumConcMap_[it->first]!=-1)&&"in ComputePhaseChangeVolumicEnergy(): equilibrium concentration for precipitate has not been computed yet " );
	    int stoiCoef= it->second->GetStoichiometricCoef();
	    double XvEq=equilibriumConcMap_[it->first];
	    double XvSS=  materialCurrentConcMap[it->first]->GetVolumicValue();
	    
	    std::cout<<"XvSS = "<<XvSS<<std::endl;
	    std::cout<<"stoiCoef = "<<stoiCoef<<std::endl;
	    std::cout<<"XvEq = "<<XvEq<<std::endl;
	    /*DEBUG*/ std::cout<<"XvEq/XvSS "<<XvEq/XvSS<<"\n"; 
	    
	    assert ( (XvSS>0)&&"Current volumic concentration in material must be superior to 0");
	    product*=std::pow( XvEq/XvSS,stoiCoef);  
	  };
	}
	
	std::cout<<"product is"<<product<<std::endl;
	
	assert( (molarVolume_>0)&&"In ComputePhaseChangeVolumicEnergy(): molarVolume has not been defined" );
	
	phaseChangeVolumiqueEnergy_=R*T/molarVolume_*std::log(product);
	
	/*DEBUG*/std::cout<<"DELTA Gv is "<<phaseChangeVolumiqueEnergy_<<"For precipitate type <"<<typeid(*this).name()<<"> "<<std::endl;
	
	
	//post conditions
	assert( (phaseChangeVolumiqueEnergy_ !=1)&&"In Precipitate.ComputePhaseChangeVolumicEnergy(): phaseChangeVolumiqueEnergy_ may have not been computed!!!" );
	
	//assert if computed value is negative. phaseChangeVolumiqueEnergy_ must always be negative
	/*DEBUG*/ std::cout<<"product "<<product<<" \n";
	assert( (phaseChangeVolumiqueEnergy_ <= 0)&&"In Precipitate.ComputePhaseChangeVolumicEnergy(): value computed for phaseChangeVolumiqueEnergy_ is not negative!!!" );
  }
  else //  IsPrecipitationPossible==false, precipitation is not possible
  {
    phaseChangeVolumiqueEnergy_=111.111; // 111.111 means "ne s'applique pas"
  };
  
  

  
}

// 
double
Precipitate::ReturnCriticalRadius()
{
 
  //Assert DeltaGv and DeltaGe have been computed before, and correct (current) values are used
  double oldDeltaGv=phaseChangeVolumiqueEnergy_;
  double oldDeltaGe=distorsionEnergy_;
  double oldGamma=surfaceEnergyCurrentValue_;
  this->ComputePhaseChangeVolumicEnergy();//compute another value of phaseChangeVolumiqueEnergy_
  this->ComputeDistorsionEnergy(); //compute another value of distorsionEnergy_
  this->ComputeSurfaceEnergy();
  assert ( (phaseChangeVolumiqueEnergy_==oldDeltaGv)&&"may be phaseChangeVolumiqueEnergy_ has not been computed before\
  running method ReturnCriticalRadius()");
  assert ( (distorsionEnergy_==oldDeltaGe)&&"may be distorsionEnergy_ has not been computed before\
  running method ReturnCriticalRadius()");
  assert ( (surfaceEnergyCurrentValue_==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before\
  running method ReturnCriticalRadius()");
  
  
  
  double criticalRadius= -4./3.*surfaceEnergyCurrentValue_/(distorsionEnergy_+phaseChangeVolumiqueEnergy_)*(shapeFactor_+2.)/(shapeFactor_+4./3.);
  
  assert ((criticalRadius>0)&&"In ReturnCriticalRadius: computed value of criticalRadius is not strictly positive");
 
  return criticalRadius; 
}
//   
double //TODO
Precipitate::ReturnDeltaCriticalRadius()
{
  double a,b,c,d; //equation a*x^3+ b*x^2+ c*x + d=0
  std::complex<double> x1, x2, x3;
  double solution;
  
  double T=materialPointer_->GetTemperature().GetCurrentTemp();
  //Assert DeltaGv and DeltaGe have been computed before, and correct (current) values are used
  double oldDeltaGv=phaseChangeVolumiqueEnergy_;
  double oldDeltaGe=distorsionEnergy_;
  double oldGamma=surfaceEnergyCurrentValue_;
  this->ComputePhaseChangeVolumicEnergy();//compute (and set!) another value of phaseChangeVolumiqueEnergy_
  this->ComputeDistorsionEnergy(); //compute (and set) another value of distorsionEnergy_
  this->ComputeSurfaceEnergy();//compute (and set) another value of current sureface energy
  assert ( (phaseChangeVolumiqueEnergy_==oldDeltaGv)&&"may be phaseChangeVolumiqueEnergy_ has not been computed before.\
  Error when running method Precipitate::ReturnCriticalRadius()");
  assert ( (distorsionEnergy_==oldDeltaGe)&&"may be distorsionEnergy_ has not been computed before.\
  Error when running method Precipitate::ReturnCriticalRadius()");
  assert ( (surfaceEnergyCurrentValue_==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before.\
  Error when running method Precipitate::ReturnCriticalRadius()");
  
  
  a=M_PI*(shapeFactor_ + 4./3.)*(distorsionEnergy_ + phaseChangeVolumiqueEnergy_);
  //assert that a!=0  ( equation a*x^3+ b*x^2+ c*x + d)
  assert((a!=0)&&"Precipitate::ReturnDeltaCriticalRadius() : coefficient of third order monom (x^3) is 0 ");
  
  b= (-2.)*(shapeFactor_ + 2.)*surfaceEnergyCurrentValue_*M_PI;
  c= 0.;
  d= T*ThermoDynamicsConstant::GetkB();

  Util::Util::SolveThirdDegreeEquation(a, b, c, d,
						 x1,
						 x2,
						 x3); 
  //Remember; equation a*x^3+ b*x^2+ c*x + d=0 has at least one real root
  
  std::vector<double> ListOfRealRoots;
  std::vector<double> ListOfPotentialSolutions;
  assert ( (ListOfRealRoots.size()==0)&&"Something is wrong");
  
  if (x1.imag()==0)
    ListOfRealRoots.push_back(x1.real());
  if (x2.imag()==0)
    ListOfRealRoots.push_back(x2.real());
  if (x3.imag()==0)
    ListOfRealRoots.push_back(x3.real());
  
  assert ( (ListOfRealRoots.size()>=1) && (ListOfRealRoots.size()<=3)&&"Size of vector ListOfRealRoots is not correct:\
  there must be at least one real root, and at most 3 real roots"  );
  
  for (std::vector<double>::const_iterator i = ListOfRealRoots.begin(); i != ListOfRealRoots.end(); ++i)
  {
    //potential solutions of the equation must be STRICTLY POSITIVES
    if ( (*i)>0)
      ListOfPotentialSolutions.push_back(*i);
  }
  
  //the solution of the equation must be UNIQUE!!! therefore, size of vector ListOfPotentialSolutions must be 1
  assert( (ListOfPotentialSolutions.size()==1)&&"Impossible to find solution because there is not only one real solution");
  
  solution=ListOfPotentialSolutions[0];
  
  ////////	  
  //post assertions
  /////////
  //Check precision of solution X found
  double epsilon=1e-16;
  double Eqprecision= a*std::pow(solution,3) +  b*std::pow(solution,2) + c*solution + d ; // compare ax^3 +bx^2 +cx +d  to 0.
  
  std::cout<<"Precision for equation : Pi*(alphaP1 + 4/3)*(deltageP1 + deltagvP1)*x^3 - 2*(alphaP1 + 2)*gammaP1*Pi*x^2 + kB*T =0 is "<<Eqprecision<<"\n";	 
  assert ( (Eqprecision<= epsilon) && "Precision for equation Pi*(alphaP1 + 4/3)*(deltageP1 + deltagvP1)*x^3 - 2*(alphaP1 + 2)*gammaP1*Pi*x^2 + kB*T =0 is NOT SATISFIED" );
	  
  
  
  assert (solution>0);
  return solution;
}


//return Z
double 
Precipitate::ReturnZeldovichFactor()
{
  double T=materialPointer_->GetTemperature().GetCurrentTemp();
  double Na=ThermoDynamicsConstant::GetNa();
  double R=ThermoDynamicsConstant::GetR();
  
  
  assert( (molarVolume_!=-1)&&"molar Volume of precipitate has not been initialized");
  assert ( (criticalRadius_==this->ReturnCriticalRadius())&&"In ReturnZeldovichFactor(): Critical radius may have not been computed");
  assert ( (deltaCriticalRadius_==this->ReturnDeltaCriticalRadius())&&"In ReturnZeldovichFactor(): deltaCriticalRadius_ may have not been computed!!! " );//TODO maybe all this
						                                           //asserts with ReturnXXX are not good for efficiency and rapidity of the computation
  double oldGamma=surfaceEnergyCurrentValue_;
  this->ComputeSurfaceEnergy();
  assert ( (surfaceEnergyCurrentValue_==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before\
  running method ReturnZeldovichFactor()");
  
  assert( (T!=0)&&(Na!=0)&&(R!=0) );
  assert ( (criticalRadius_ + deltaCriticalRadius_)!=0 );

  
  
  double Z=molarVolume_/(2.*M_PI*std::pow(criticalRadius_ + deltaCriticalRadius_, 2))*std::pow(surfaceEnergyCurrentValue_/(Na*R*T), 0.5 );
 
  //TODO post assertions!!!
  assert( (Z>0)&&"Computed Zeldovich factor is not strictly positive" );
  
  return Z;
}


//Return betaetoile
double //TODO
Precipitate::ReturnCriticalBeta()
{
  double latticeParam=materialPointer_->GetLatticeParameter();
  std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
  std::map<std::string, Concentration*> precipitateConcMap=chemicalComposition_.GetConcentrationMap();
  std::map<std::string, Concentration*>::iterator it;
  //std::map<std::string, Concentration*>::iterator it2;
  std::vector<const ChemicalElement*> soluteList =materialPointer_->GetSoluteList();
  
  double sum=0.; // sum of 1/(D_i_SS*X_at_i)
  
  for (it=precipitateConcMap.begin(); it!=precipitateConcMap.end(); ++it )
  {
    if ( it->first != mainElementSymbol  )
    {
      double currentAtomicConc=materialPointer_->ReturnAtomicConcFromVolumicForElement(it->first);  
      
      //TODO some stuff
      for( std::vector<const ChemicalElement*>::const_iterator i = soluteList.begin(); i != soluteList.end(); ++i)
      {
	if ( (*i)->GetElementName() == it->first)
	{
	  double Dcoef = (*i)->GetDiffusion().GetAtomicDiffusionCoef();
	  assert(Dcoef>0);
	  assert (currentAtomicConc>0);
	  sum += 1./(Dcoef*currentAtomicConc);
	  
	  /*DEBUG*/std::cout<<"Elementname "<<it->first<<std::endl;
	  /*DEBUG*/std::cout<<"DCoef "<<Dcoef<<std::endl;
	  /*DEBUG*/std::cout<<"currentAtomicConc "<<currentAtomicConc<<std::endl;
	  /*DEBUG */std::cout<<"@@@@@@@@@@@@@@@@@@@@@ sum = "<<sum<<"\n\n\n\n\n\n\n\n"<<std::endl;
	  /*DEBUG */std::cout<<"@@@@@@@@@@@@@@@@@@@@@ 1./(currentAtomicConc*Dcoef) = "<<1./(currentAtomicConc*Dcoef)<<"\n\n\n\n\n\n\n\n"<<std::endl;
	  /*DEBUG */std::cout<<"@@@@@@@@@@@@@@@@@@@@@ currentAtomicConc = "<<currentAtomicConc<<"\n"<<std::endl;
	  /*DEBUG */std::cout<<"@@@@@@@@@@@@@@@@@@@@@ Dcoef = "<<Dcoef<<"\n"<<std::endl;
	};
      }
      
    };
  
    
  }
  
  /*DEBUG */std::cout<<"@@@@@@@@@@@@@@@@@@@@@ sum = "<<sum<<"\n\n\n\n\n\n\n\n"<<std::endl;
  assert( (sum!=0)&&(latticeParam!=0) );
  double beta=4.*M_PI*std::pow((criticalRadius_+deltaCriticalRadius_), 2.)/std::pow(latticeParam,4.)/sum;
  
  std::cout<<"latticeParam                "<<latticeParam<<" sum "<<sum<<std::endl;
  /*DEBUG*/std::cout<<"DEBUG beta , std::pow((criticalRadius_+deltaCriticalRadius_), 2.)/std::pow(latticeParam,4.)"<<std::pow((criticalRadius_+deltaCriticalRadius_), 2.)/std::pow(latticeParam,4.)<<"\n\n\n"<<std::endl;
  
  return beta;
}

double //TODO
Precipitate::ReturnNucleationRate()
{
  //TODO!!!! Using ZeldovichFactor_, nucleationSitesNumber_ , criticalBeta_ , criticalTotalEnergy_ .
  //So these values has to be computed before Using this method ReturnNucleationRate()
  assert( (ZeldovichFactor_!=-1)&&"Zeldovich factor has not been computed yet!");
  
  assert( (nucleationSitesNumber_!=-1&&"ReturnNucleationRate(): Nucleation site number(nucleationSitesNumber_) has not been computed yet") );
  
  double currentNucleationSiteNb=nucleationSitesNumber_;/*TODO return current nucleation site number, assert it has been computed before*/
  double Z=this->ReturnZeldovichFactor();
  double beta=this->ReturnCriticalBeta();
  double DeltaG=this->ReturnCriticalTotalEnergy(); //ReturnCriticalTotalEnergy() method is now in inherited class
  double kB=ThermoDynamicsConstant::GetkB();
  double T=materialPointer_->GetTemperature().GetCurrentTemp();
  
  assert( (Z==ZeldovichFactor_)&&"Incompatibility: May be Zeldovich  has not computed before" );
  assert( (beta==criticalBeta_)&&"Incompatibility: May be criticalBeta_  has not computed before" );
  assert( (DeltaG==criticalTotalEnergy_)&&"Incompatibility: May be criticalTotalEnergy_  has not computed before" );
  
  
  double J=Z*currentNucleationSiteNb*beta*std::exp(-DeltaG/(kB*T));
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ Precipitatate type @@@@@@@@@@@@@@@@@ <"<<typeid(*this).name()<<"> \n\n\n"<<std::endl;
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ J @@@@@@@@@@@@@@@@@"<<J<<"\n\n\n"<<std::endl;
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ currentNucleationSiteNb @@@@@@@@@@@@@@@@@"<<currentNucleationSiteNb<<std::endl;
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ Z @@@@@@@@@@@@@@@@@"<<Z<<std::endl;
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ beta @@@@@@@@@@@@@@@@@"<<beta<<std::endl;
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ DeltaG @@@@@@@@@@@@@@@@@"<<DeltaG<<std::endl;
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ kB @@@@@@@@@@@@@@@@@"<<kB<<std::endl;
  /*DEBUG*/std::cout<<"@@@@@@@@@@@@@@@@@ T @@@@@@@@@@@@@@@@@"<<T<<std::endl;
  
  //post assertions?
  
  /*
  // Begin: in TEST 
  if (J<1)
  {
    std::cout<<"Nucleatiion rate (= ["<<J<<"] )is inferior to 1, thus it will be setted to O  (do J=0) \n";
    J=0;
  }
  //ENd: in TEST
  */
  
  
  return J;
}

void 
Precipitate::ComputeCriticalRadius()
{
  criticalRadius_=this->ReturnCriticalRadius();
}

void
Precipitate::ComputeDeltaCriticalRadius()
{
  deltaCriticalRadius_=this->ReturnDeltaCriticalRadius();
}

void 
Precipitate::ComputeZeldovichFactor()
{
  ZeldovichFactor_=this->ReturnZeldovichFactor();
}

void 
Precipitate::ComputeCriticalBeta()
{
  criticalBeta_=this->ReturnCriticalBeta();
  
  //asserions???
}

void 
Precipitate::ComputeNucleationRate()
{
  nucleationRate_=this->ReturnNucleationRate();
}



void 
Precipitate::SetSEPolynomialDegree(const int deg)
{
  surfaceEnergyPolynomial_.SetDegree(deg);
}


void 
Precipitate::AddSEPolynomialPyCoefs(boost::python::list& pythonCoefList)
{
  surfaceEnergyPolynomial_.AddPyCoefs(pythonCoefList);
}

void
Precipitate::ComputeSurfaceEnergy()
{
  std::cout<<"\n";
  std::cout<<  "======================================================================================="<< std::endl;
  std::cout<<  " Computing surface energy of Precipitate type <" << this->GetPrecipitateType() << ">"<<std::endl;
  std::cout<<  "======================================================================================="<< std::endl;
  
  assert ( (materialPointer_!=0)&&"In ComputeSurfaceEnergy() of Precipitate: Precipitate does not belongs to any material!!!"); 
  double T= materialPointer_->GetTemperature().GetCurrentTemp();
  
  surfaceEnergyCurrentValue_ = surfaceEnergyPolynomial_.ReturnValue(T);
  
  std::cout<<  "======================================================================================="<< std::endl;
  std::cout<<  " Computing surface energy of Precipitate type <" << this->GetPrecipitateType() << ">"<<std::endl;
  std::cout<<  "======================================================================================="<< std::endl;
  std::cout<<"\n";
  
}


void //TODO
Precipitate::ReadDataValue(std::string fileName)
{
}

void //TODO
Precipitate::AddRadiusDistribution(const RadiusDistribution& RD)
{

}

void //TODO
Precipitate::SetCurrentRadiusDistribution(const RadiusDistribution & RD)
{

}

//TODO ,Does it really need? may be manual set for all attribute would be preferable
/*void 
Precipitate::InitializeParameters(double deltaCell,
				  double solvusActivationEnergy,
				  double nucleationSitesNumber,
				  double preExpTermForSolvus,
				  Polynomial& surfaceEnergyPolynomialModel,
				  double shapeFactor,
				  double volumicFraction)
{
  
  SetDeltaCell(deltaCell);
  SetSolvusActivationEnergy(solvusActivationEnergy);
  SetNucleationSitesNumber(nucleationSitesNumber);
  SetPreExpTermForSolvus(preExpTermForSolvus);
  SetSurfaceEnergyPolynomialModel(surfaceEnergyPolynomialModel);
  SetShapeFactor(shapeFactor);
  SetVolumicFraction(volumicFraction);
}*/


//TODO DO AN INITIALIZITAION THAT CONVERT ALL THAT IS NEED IN THE RIGHT ORDER



//Compute and Set sumOfStoicCoefs_
void
Precipitate::ComputeSumOfStoicCoefs()
{ 
  
  if (!SumOfStoiCoefsHasBeenComputed_)
  {
      std::map<std::string , Concentration*>::iterator it;
      
      std::map<std::string, int> coefsMap;//Use to find max value of Stoichiometric coefs
      std::map<std::string, int>::iterator iter;
      std::map<std::string, int>::iterator maxIter;
      std::map<std::string, Concentration*> concMap=chemicalComposition_.GetConcentrationMap();
      int max=0;
      
      sumOfStoicCoefs_=0;
      
      //Compute and Set sumOfStoicCoefs_
      for (it=concMap.begin(); it!=concMap.end(); ++it)
      {
      assert( (it->second->GetStoichiometricCoef()>0)&&"Cannot Convert Stoichiometric\
      to Atomic concentration because a Stoichiometric coef is not an integer strictly positive" );
      sumOfStoicCoefs_+= it->second->GetStoichiometricCoef();
      coefsMap[it->first] = it->second->GetStoichiometricCoef();
      }
      
      assert ( (sumOfStoicCoefs_>0)&&"Cannot SetSumOfStoicCoefs because computed\
      argument is not an integer stricly positive!" );
      
      for ( iter=coefsMap.begin(); iter!=coefsMap.end(); ++iter )
      {
	if ( iter->second > max ) 
	{
	  max=iter->second;
	  maxIter=iter; 
	};
      }
      assert( (maxIter->second ==max)&&"Cannot find max value" );
      chemicalComposition_.SetMainElementName(maxIter->first);
      std::cout<<"*********************************  MAX element is  ****************************"<<chemicalComposition_.GetMainElementName()<<std::endl;
      
      SumOfStoiCoefsHasBeenComputed_=true;
  }
  
}





void
Precipitate::ConvertMassicToVolumicConcentration()
{
  
  //TODO assert if CC is in chemicalCompositionList_. If not, abandon 
  
  assert(!"Not Needed yet! Not implemented yet ");
   
}

void
Precipitate::ConvertVolumicToMassicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
  assert(!"Not Needed yet! Not implemented yet ");
}

void
Precipitate::ConvertAtomicToVolumicConcentration()
{
  
  std::cout<<"Starting to convert Atomic to Volumic concentration for precipitate "<<std::endl;
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
  
  //Compute And Set SumOfStoichiometric if it has not been done before
  this->ComputeSumOfStoicCoefs();


  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string, Concentration*> concMap=chemicalComposition_.GetConcentrationMap();
  
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {
    double atomicConc=it->second->GetInitialAtomicValue();
    double elementRho=it->second->GetChemicalElement().GetDensity();
    double elementMolarMass=it->second->GetChemicalElement().GetMolarMass();
    
    
    double volumicConc=atomicConc*(elementMolarMass/1000/elementRho)/(molarVolume_/this->GetSumOfStoicCoefs());
    
    
    
    assert((volumicConc>0)&&"In ConvertAtomicToVolumicConcentration(): computed value of volumic conc is negative or null");
    it->second->SetVolumicValue(volumicConc);
    std::cout<<"===================== VALUE COMPUTED FOR VOLUMIC CONC OF ELEMENT <"<<it->first<<"> IN PRECIPITATE IS ============= "<<it->second->GetVolumicValue()<<std::endl;
  }
  
  
  double sumOfOtherVolumicValues=0;
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {
    if ( it->first==chemicalComposition_.GetMainElementName() )
    {
      std::map<std::string , Concentration*>::iterator it2;
      for (it2=concMap.begin(); it2!=concMap.end(); ++it2)
      {
	if (it2!=it)
	{sumOfOtherVolumicValues+=it2->second->GetVolumicValue();};
      }
      
      it->second->SetVolumicValue(1-sumOfOtherVolumicValues);
      
      //TODO !!! assert((volumicConc>0)&&"In ConvertAtomicToVolumicConcentration(): corrected rounding value of volumic conc is negative or null");
      
      std::cout<<"===================== VALUE COMPUTED FOR VOLUMIC CONC OF ELEMENT <"<<it->first<<"> IN PRECIPITATE IS ============= "<<it->second->GetVolumicValue()<<std::endl;
    };
  }
  
  
  
  
  std::cout<<"End of Atomic to volumic Conversion for precipitate"<<std::endl;
}



void
Precipitate::ConvertVolumicToAtomicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
}

void
Precipitate::ConvertStoichiometricCoefficientToVolumicConcentration()
{
  // TODO assert if CC is in chemicalCompositionList_. If not, abandon
   
  
}




void
Precipitate::ConvertStoichiometricCoefficientToAtomicConcentration()
{
  std::cout<<"C++. In convert method stoic to atomic"<<std::endl;
  std::cout<<"Precipitate's adress is:"<<this<<std::endl;
  std::cout<<"Precipitate's ChemicalCompo adress is:"<<&chemicalComposition_<<std::endl;
  
  std::map<std::string , Concentration*>::iterator it;
  std::map<std::string, Concentration*> concMap=chemicalComposition_.GetConcentrationMap();
  
  
  //Compute and Set sum Of stoiCoef in Class Concentration if it has not been done before
  this->ComputeSumOfStoicCoefs();
 
  assert ( (sumOfStoicCoefs_!=0)&&"Sum of stoic coefs must be different from zero" ); 
  
  for (it=concMap.begin(); it!=concMap.end(); ++it)
  {   
    double conc=double(it->second->GetStoichiometricCoef())/(this->GetSumOfStoicCoefs());// use double cast not to avoid integer division
    std::cout<<"===================== VALUE COMPUTED FOR ATOMIC CONC IS ============= "<<conc<<std::endl;
    it->second->SetInitialAtomicValue(conc);
  }  
  
}


//this method is in TEST
void
Precipitate::ProcessNucleationRate()
{
  std::cout<<std::endl;
  std::cout<<"-+-+-+-+-+ Run precipitate::ProcessNucleateRate() for Precipitate <"<<typeid(*this).name()<<"> at adress <"<<this<<"> +-+-+-+-+-+-"<<std::endl;
  std::cout<<std::endl;
  this->ComputeEquilibriumConcentrations();
  this->ComputeSurfaceEnergy();
  this->ComputePhaseChangeVolumicEnergy();
  /*DEBUG*/std::cout<<"Phase change Volumic Energy"<<phaseChangeVolumiqueEnergy_<<std::endl;
  /*DEBUG*/std::cout<<"distorsionEnergy_"<<distorsionEnergy_<<std::endl;
  
  
  // Preconditions!!!
  
  //isSolutesCurrentVolumicConcentrationNegativeOrNull . exple:  XvCuSS<=0 Or XvMgSS<=0 Or ....
  std::map<std::string, Concentration*> materialCurrentConcMap= materialPointer_->GetCurrentChemicalComposition().GetConcentrationMap();
  std::string mainElementSymbol=materialPointer_->GetMainChemicalElement().GetElementName();
  std::map<std::string, Concentration*>::iterator it;
  bool isSolutesCurrentVolumicConcentrationNegativeOrNull=false;
  for (it=materialCurrentConcMap.begin(); it!=materialCurrentConcMap.end(); ++it )
  {
    if ( it->first != mainElementSymbol  )
    {
       if ( (it->second)->GetVolumicValue() <=0 )
	 isSolutesCurrentVolumicConcentrationNegativeOrNull=true;
    };
  }
  
  /*DEBUG*/ std::cout<<" isSolutesCurrentVolumicConcentrationNegativeOrNull  "<<isSolutesCurrentVolumicConcentrationNegativeOrNull<<" nucleationSitesNumber_ "<<nucleationSitesNumber_<<"distorsionEnergy_ + phaseChangeVolumiqueEnergy_   "<<distorsionEnergy_ + phaseChangeVolumiqueEnergy_<<std::endl;
  if ( (isSolutesCurrentVolumicConcentrationNegativeOrNull==true) || (nucleationSitesNumber_<=0) || ((distorsionEnergy_ + phaseChangeVolumiqueEnergy_) >= 0) ||  (phaseChangeVolumiqueEnergy_==111.111)   )
  {
    
    //TODO improve this part
    //  (-111.111) means : "ne s'applique pas"
    criticalRadius_=-111.111;
    deltaCriticalRadius_=-111.111;
    criticalTotalEnergy_=-111.111;
    ZeldovichFactor_=-111.111;
    criticalBeta_=-111.111;
    nucleationRate_=-111.111;
  }
  else
  {
    this->ComputeCriticalRadius();
    /*DEBUG*/std::cout<<"criticalRadius_"<<criticalRadius_<<std::endl;
    /*DEBUG*/this->ComputeDeltaCriticalRadius();
    /*DEBUG*/std::cout<<"deltaCriticalRadius_"<<deltaCriticalRadius_<<std::endl;
    this->ComputeCriticalTotalEnergy();
    /*DEBUG*/std::cout<<"criticalTotalEnergy_"<<criticalTotalEnergy_<<std::endl;
    this->ComputeZeldovichFactor();
    this->ComputeCriticalBeta();
    this->ComputeNucleationRate();
    
    ///////////////////////////////////////////////////////////
    ///////  add classes if  Rmax < R*  ///////////////////////
    
    int n=currentRadiusDistribution_->GetItemsValues().size();
    double maxRadius=currentRadiusDistribution_->GetRadiusForClass(n);
    double nucleationRadius= criticalRadius_+deltaCriticalRadius_;
    
    if (nucleationRadius>maxRadius)
    {
      std::cout<< "============================================================================================================"<<std::endl;
      std::cout<< "WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! "<<std::endl;
      std::cout<< " Retoile > Rmax : Computed nucleation radius (r* + dr*) is superior to the maximum radius of distribution   "<<std::endl;
      std::cout<< " Empty classes will be added "<<std::endl;
      while ( nucleationRadius >= currentRadiusDistribution_->GetCurrentMaxRadius() )
      {
	currentRadiusDistribution_ ->AddEmptyClass();
	std::cout<< "   current max radius is now  ===> "<< currentRadiusDistribution_->GetCurrentMaxRadius()<<std::endl;
      }
      currentRadiusDistribution_ ->AddEmptyClass(); // add another empty class after the class where the nucleated radius will appear
    }
    ///////  add classes if  Rmax < R*  ///////////////////////
    ///////////////////////////////////////////////////////////////
    
    
  };
  
  
  

  
  
  std::cout<<std::endl;
  std::cout<<"-+-+-+-+-+ ENDING the processing of nucleation rate +-+-+-+-+-+- "<<std::endl;
  std::cout<<std::endl;
}

//this method is in TEST
void 
Precipitate::ComputeAllInterfacialConcentrations()
{
  assert ((currentRadiusDistribution_!=0)&&"Precipitate is not linked to any current Radius distribution");
  
  /*DEBUG*/ std::cout<<"precipitate adress is : "<<this<<std::endl;
  
  std::cout<<"+-+-+-+- BEGIN: Running precipitate::ComputeAllInterfacialConcentrations() +-+-+-+-"<<std::endl;
  currentRadiusDistribution_->ComputeAllInterfacialConcentrations();
  std::cout<<"+-+-+-+-   END: Running precipitate::ComputeAllInterfacialConcentrations() +-+-+-+-"<<std::endl;
}


const double 
Precipitate::ReturnCriticalTimeStep()
{
  double criticaltimeStepP;// critical time step for Precipitate
  
  assert ( (currentRadiusDistribution_!=0)&&"Precipitate is not linked to any current Radius Distribution");
  
  
  
  unsigned int firstNotEmptyClassId =this->ReturnRDFirstNotEmptyClassIdAfterNucleation();
  
  /*debug*/ FirstNotEmptyClassIdAfterNucleation=firstNotEmptyClassId;
  
  if(firstNotEmptyClassId==0 )
  {
    assert (volumicFraction_==0);
    std::cout<<"In Precipitate::ReturnCriticalTimeStep():  In this  case there is no nucleus radius in the radius distribution and the volumic fraction of precipitate is 0 \n";
    std::cout<<"Therefore, returned value of precipitate time step will be the computation default time step \n";
    assert ((materialPointer_!=0)&&"Errot:Precipitate does not belong to any precipitate:");
    double defaultTimeStep= materialPointer_->GetComputation().GetDefaultTimeStep();
    criticaltimeStepP=defaultTimeStep;
  }
  else
  {
    
    // The smallest unempty class (first unempty class) after nucleation exist,
    double criticalV, deltar;
    
    
    assert(firstNotEmptyClassId>=1);
    
    
    std::vector<double> relevantAbsoluteValueOfVelocityList;
    
    unsigned int n=currentRadiusDistribution_->GetItemsValues().size();
    
    assert (firstNotEmptyClassId<=n);
    
    // All left interfaces velocities
    for (unsigned int i=firstNotEmptyClassId; i<=n; ++i )
    {
      // left interfaces
      double leftV = currentRadiusDistribution_->GetInterfacialConcentrationObjectUsed().GetLeftInterfacialVelocityForClass(i);
      relevantAbsoluteValueOfVelocityList.push_back(std::abs(leftV));
    }
    
    //right interface of last class
    double rightV_OflastClass=currentRadiusDistribution_->GetInterfacialConcentrationObjectUsed().GetRightInterfacialVelocityForClass(n);
    relevantAbsoluteValueOfVelocityList.push_back(std::abs(rightV_OflastClass));
    
    assert (relevantAbsoluteValueOfVelocityList.size()== (n-firstNotEmptyClassId+2));
    
    
    std::vector<double>::const_iterator it;
    it = std::max_element(relevantAbsoluteValueOfVelocityList.begin(), relevantAbsoluteValueOfVelocityList.end());
    
    criticalV= *it;
    
    deltar=currentRadiusDistribution_->GetSpatialStep();
  
  
    criticaltimeStepP= deltar/2/criticalV;
    
    /*debug*/std::cout<<"criticalV toto "<<criticalV<<"  "<<typeid(*this).name()<<"\n";
    
  };
  

  
    /*TODO erase*/ std::cout<<"criticaltimeStepP toto "<<criticaltimeStepP<< "  "<<typeid(*this).name()<<"\n";

  
  assert  (criticaltimeStepP>0);
  
  return criticaltimeStepP;
}


void 
Precipitate::AddNucleatedPrecipitates()
{
  
  std::cout<<"Adding Nucleus For precipitate type <<"<<typeid(*this).name()<<">> at adress <"<<this<<"> "<<std::endl;
  assert (currentRadiusDistribution_!=0);
  
  int n=currentRadiusDistribution_->GetItemsValues().size();
  assert(n>0);
  double minRadius=currentRadiusDistribution_->GetRadiusForClass(1);
  double maxRadius=currentRadiusDistribution_->GetRadiusForClass(n);
  double nucleationRadius= criticalRadius_+deltaCriticalRadius_;
  
  /*DEBUG*/std::cout<<"DEBUG  r* "<<criticalRadius_<<" delta r* "<<deltaCriticalRadius_<<"\n";
  
  if ( (criticalRadius_==-111.111)&&(deltaCriticalRadius_==-111.111) ) // -111.111 means "ne s'applique pas"
  { 
    std::cout<<"There is no new nucleus for Precipitate type <<"<<typeid(*this).name()<<">> at adress <"<<this<<"> "<<std::endl;
  }
  else if (  (nucleationRadius>0)&&(nucleationRadius<=minRadius)  )
  {
    std::cout<<"Computed nucleation radius (r* + dr*) is not in the range between 0 and Rmin of the RadisDistribution"<<std::endl;
    /*DEBUG*/std::cout<<"nucleationRadius "<<nucleationRadius<<"minRadius "<<minRadius<<" maxRadius"<<maxRadius<<std::endl;
    std::cout<<"Therefore, no new nucleus for Precipitate type <<"<<typeid(*this).name()<<">> at adress <"<<this<<"> will be considered "<<std::endl;
  }
  else
  {
     /*DEBUG*/std::cout<<"nucleationRadius "<<nucleationRadius<<"minRadius "<<minRadius<<" maxRadius"<<maxRadius<<std::endl;
     
     if (nucleationSitesNumber_==0)
     { 
       
       std::cout<<"Precipitate type"<<typeid(*this).name()<<">> at adress <"<<this<<"> "<<" does not have any available nucleationSiteNumber (Ns=0), thus AddNucleatedPrecipitates() will do nothing"<<std::endl;
     }
     else //nucleationSitesNumber_!=0
     {
       //nucleationSitesNumber_!=0
       
       /*DEBUG*/std::cout<<"Precipitate type"<<typeid(*this).name()<<">> at adress <"<<this<<"> \n";
       /*DEBUG*/ std::cout<<"DEBUG: nucleationSitesNumber_ "<<nucleationSitesNumber_<<"\n";
       /*DEBUG*/ std::cout<<"DEBUG: computation currentTime is  "<<materialPointer_->GetComputation().GetCurrentTime() <<"\n";
       
       
       /*DEBUG*/ // The if is just for debbugging, normally it is an assert not an if satatement
       
       //now it is an if statement, warning and press to continue  . Not an assert
       assert ( (nucleationRadius<=maxRadius)&&"Computed nucleation radius (r* + dr*) is superior to max radius distribution. Not in the range of the current RadiusDistribution");
       assert ( (nucleationRadius>=minRadius)&&"Computed nucleation radius (r* + dr*) is inferior to the minimum radius of distribution. Not in the range of the current RadiusDistribution");
       
       
       /*
       ///////  change the assert with the if  ///////////////////////
       if (nucleationRadius < minRadius)
       {
	 std::cout<< "============================================================================================================"<<std::endl;
	 std::cout<< "WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! "<<std::endl;
	 std::cout<< " Retoile < Rmin : Computed nucleation radius (r* + dr*) is inferior to the minimum radius of distribution   "<<std::endl;
	 /// press any key to continue
	 std::cout <<"Press ENTER to continue... " ;
	 std::cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
	 //////////////
       };
       
       
       if (nucleationRadius>maxRadius)
       {
	 std::cout<< "============================================================================================================"<<std::endl;
	 std::cout<< "WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! "<<std::endl;
	 std::cout<< " Retoile > Rmax : Computed nucleation radius (r* + dr*) is superior to the maximum radius of distribution   "<<std::endl;
	 std::cout<< " Empty classes will be added "<<std::endl;
	 while ( nucleationRadius >= currentRadiusDistribution_->GetCurrentMaxRadius() )
	 {
	   currentRadiusDistribution_ ->AddEmptyClass();
	   std::cout<< "   current max radius is now  ===> "<< currentRadiusDistribution_->GetCurrentMaxRadius()<<std::endl;
	 }
	 currentRadiusDistribution_ ->AddEmptyClass(); // add another empty class after the class where the nucleated radius will appear
       }
       ///////  change the assert with the if  ///////////////////////
       ///////////////////////////////////////////////////////////////
       
       */
       
       
       
       
       double deltat= materialPointer_->GetComputation().GetMaxTimeStep();
       unsigned int nucleatedClassId=currentRadiusDistribution_->FindClassForRadius(nucleationRadius);
       double oldN= currentRadiusDistribution_->GetItemValueForClass(nucleatedClassId);
	
       assert ((nucleationRate_>=0)&&"Nucleation can't be negative");
	
       std::cout<<"Adding new Nucleus of radius : "<<nucleationRadius<< " in Radius Distribution class [ "<<nucleatedClassId<<" ] for Precipitate type <<"<<typeid(*this).name()<<">> at adress <"<<this<<"> "<<std::endl;
       currentRadiusDistribution_->SetItemValueForClass(nucleatedClassId, oldN + nucleationRate_*deltat  );
       /*DEBUG*/std::cout<<"oldN "<<oldN<<"nucleationRate_"<<nucleationRate_<<"nucleationRate_*deltat"<<nucleationRate_*deltat<<"oldN + nucleationRate_*deltat "<<oldN + nucleationRate_*deltat<<std::endl;
	 
       
       

     };
     
     
     
     
    
  };
  

  
  
}


void 
Precipitate::SolveCineticLinearSytem()
{
  //TODO
  
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@ Solving cinetic linear system for precipitate type <"<<this->GetPrecipitateType()<<"> which adress is <"<<this<<"> @@@@@@@@@@@@@@@@@@@@@@@@@@@\n"<<std::endl;
  
  assert ((currentRadiusDistribution_!=0)&&"Precipitate is not linked to any current RadiusDistribution");
  
  const int n= currentRadiusDistribution_->GetItemsValues().size(); //system size will be n*n
  const int nrhs = 1;	// number of RHS vectors
  
  ublas::matrix<double, ublas::column_major> A(n, n), Acopy(n,n);
  A.clear();
  Acopy.clear();
  ublas::matrix<double, ublas::column_major> B(n, nrhs), Bcopy(n, nrhs);
  ublas::vector<double> V(n);
  ublas::matrix_column< ublas::matrix<double, ublas::column_major> > col(B, 0) ;
  
  std::vector<double> VintList;
  std::string elementName;
  double deltar, deltat;
  
  InterfacialConcentration* IConcObjPtr =  & (currentRadiusDistribution_->GetInterfacialConcentrationObjectUsed() );
  

  elementName= IConcObjPtr->GetChemicalElement().GetElementName();
  
  std::cout<<"Element Name is "<<elementName<<"\n";
  
  VintList = IConcObjPtr->GetInterfacialVelocityList();
  
  deltar = currentRadiusDistribution_->GetSpatialStep();
  deltat = materialPointer_->GetComputation().GetMaxTimeStep();
  
  /////////////////////////////////////////////////// Begin:  MATRIX FEEDING  ///////////////////////////////////////////////////////////
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////FEED A_1,1 . The first class ///////////////////////////////////////
  double Vleft1,Vright1;
  Vleft1=IConcObjPtr->GetLeftInterfacialVelocityForClass(1);
  Vright1=IConcObjPtr->GetRightInterfacialVelocityForClass(1);
  
  //debug:begin
  std::cout<<"CLass Id is <<"<<1<<">>\n";
	
  std::cout<<"Vleft :"<<Vleft1<<" ; Vright :"<<Vright1<<"\n";	
  std::cout<<"Class 1 left Interfacial Concentration is :" << IConcObjPtr->GetLeftInterfacialConcValueForClass(1)<<"\n";
  std::cout<<"Class 1 Right Interfacial Concentration is :" << IConcObjPtr->GetRightInterfacialConcValueForClass(1)<<"\n";

  std::cout<<"Class 1 Radius is :" << currentRadiusDistribution_->GetRadiusForClass(1)<<"\n";
  std::cout<<"Class 1 Left Radius R-  is :" << currentRadiusDistribution_->GetLeftRadiusForClass(1)<<"\n";
  std::cout<<"Class 1 Right Radius R+  is :" << currentRadiusDistribution_->GetRightRadiusForClass(1)<<"\n";
  //debug:end
  
  
  if ( Vright1>0 && Vleft1>0 )
  {
    A(0,1)=0 ;
    A(0,0)= deltar/deltat + Vright1;
  }
  
  if ( Vright1>0 && Vleft1<0 )
  {
    A(0,1)=0;
    A(0,0)= deltar/deltat + Vright1 - Vleft1;
  }
  
  if ( Vright1<0 && Vleft1<0 )
  {
    A(0,1)=Vright1;
    A(0,0)=deltar/deltat - Vleft1;
  }
  
  if ( Vright1<0 && Vleft1>0 )
  {
    //Normally this is impossible case ???
    assert(!"An IMPOSSIBLE case has been found for the first class. leftInterfacialVelocity>0 AND rightInterfacialVelocity<0.   " ); 
    A(0,1)= Vright1;
    A(0,0)= deltar/deltat;
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////// FEED A_n,n . The last class ///////////////////////////////////////
  double Vleftn,Vrightn;
  Vleftn=IConcObjPtr->GetLeftInterfacialVelocityForClass(n);
  Vrightn=IConcObjPtr->GetRightInterfacialVelocityForClass(n);
  
  //debug:begin
  std::cout<<"CLass Id is <<"<<n<<">>\n";
	
  std::cout<<"Vleft :"<<Vleftn<<" ; Vright :"<<Vrightn<<"\n";	
  std::cout<<"Class n left Interfacial Concentration is :" << IConcObjPtr->GetLeftInterfacialConcValueForClass(n)<<"\n";
  std::cout<<"Class n Right Interfacial Concentration is :" << IConcObjPtr->GetRightInterfacialConcValueForClass(n)<<"\n";

  std::cout<<"Class n Radius is :" << currentRadiusDistribution_->GetRadiusForClass(n)<<"\n";
  std::cout<<"Class n Left Radius R-  is :" << currentRadiusDistribution_->GetLeftRadiusForClass(n)<<"\n";
  std::cout<<"Class n Right Radius R+  is :" << currentRadiusDistribution_->GetRightRadiusForClass(n)<<"\n";
  //debug:end
  
  
  if ( Vrightn>0 && Vleftn>0  )
  {
    A(n-1,n-1)= deltar/deltat +Vrightn;
    A(n-1,n-2)= -Vleftn;
  }
  
  if ( Vrightn>0 && Vleftn<0  )
  {
    A(n-1,n-1)= deltar/deltat -Vleftn + Vrightn;
    A(n-1,n-2)=0;
  }
  
  if ( Vrightn<0 && Vleftn<0  )
  {
    A(n-1,n-1)= deltar/deltat - Vleftn;
    A(n-1,n-2)=0;
  }
  
  if ( Vrightn<0 && Vleftn>0  )
  {
    //Normally this is impossible case ???
    assert(!"An IMPOSSIBLE case has been found for the last class. leftInterfacialVelocity>0 AND rightInterfacialVelocity<0.   " );
    A(n-1,n-1)= deltar/deltat;
    A(n-1,n-2)= -Vleftn;
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////feeding A_2,2 to A_n-1,n-1 /////////////////////////////////////////
  double Vleft,Vright;
  
  for(int i=1; i<n-1; ++i)
    {
      int classId= i+1;
      Vleft = IConcObjPtr->GetLeftInterfacialVelocityForClass(classId);
      Vright = IConcObjPtr->GetRightInterfacialVelocityForClass(classId);
      
      if ( Vright>0 && Vleft>0 )
      {
	std::cout<<"case 1"<<std::endl;
	A(i,i)=deltar/deltat +Vright;
	A(i,i+1)=0;
	A(i,i-1)= -Vleft;
	
	//debug:begin
	std::cout<<"CLass Id is <<"<<classId<<">>\n";
	
	std::cout<<"Vleft :"<<Vleft<<" ; Vright :"<<Vright<<"\n";
	
	
	std::cout<<"Class left Interfacial Concentration is :" << IConcObjPtr->GetLeftInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Right Interfacial Concentration is :" << IConcObjPtr->GetRightInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Radius is :" << currentRadiusDistribution_->GetRadiusForClass(classId)<<"\n";
	std::cout<<"Class Left Radius R-  is :" << currentRadiusDistribution_->GetLeftRadiusForClass(classId)<<"\n";
	std::cout<<"Class Right Radius R+  is :" << currentRadiusDistribution_->GetRightRadiusForClass(classId)<<"\n";
	//debug:end
      }
      else if( Vright>0 && Vleft<0 )
      {
	std::cout<<"case 2"<<std::endl;
	A(i,i)= deltar/deltat + Vright - Vleft;
	A(i,i+1)=0;
	A(i,i-1)=0;
	
	//debug:begin
	std::cout<<"CLass Id is <<"<<classId<<">>\n";
	
	std::cout<<"Vleft :"<<Vleft<<" ; Vright :"<<Vright<<"\n";
	
	
	std::cout<<"Class left Interfacial Concentration is :" << IConcObjPtr->GetLeftInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Right Interfacial Concentration is :" << IConcObjPtr->GetRightInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Radius is :" << currentRadiusDistribution_->GetRadiusForClass(classId)<<"\n";
	std::cout<<"Class Left Radius R-  is :" << currentRadiusDistribution_->GetLeftRadiusForClass(classId)<<"\n";
	std::cout<<"Class Right Radius R+  is :" << currentRadiusDistribution_->GetRightRadiusForClass(classId)<<"\n";
	//debug:end
	
      }
      else if ( Vright<0 && Vleft<0 )
      {	
	std::cout<<"case 3"<<std::endl;
	A(i,i)= deltar/deltat -Vleft;
	A(i,i+1)= Vright;
	A(i,i-1)=0;
	
	//debug:begin
	std::cout<<"CLass Id is <<"<<classId<<">>\n";
	
	std::cout<<"Vleft :"<<Vleft<<" ; Vright :"<<Vright<<"\n";
	
	
	std::cout<<"Class left Interfacial Concentration is :" << IConcObjPtr->GetLeftInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Right Interfacial Concentration is :" << IConcObjPtr->GetRightInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Radius is :" << currentRadiusDistribution_->GetRadiusForClass(classId)<<"\n";
	std::cout<<"Class Left Radius R-  is :" << currentRadiusDistribution_->GetLeftRadiusForClass(classId)<<"\n";
	std::cout<<"Class Right Radius R+  is :" << currentRadiusDistribution_->GetRightRadiusForClass(classId)<<"\n";
	//debug:end
      }
      else // ( Vright<0 && Vleft>0 ) //impossible case ??? 
      {
	//Normally this is impossible case ???
	std::cout<<"case 4"<<std::endl;
	
	std::cout<<"CLass Id is <<"<<classId<<">>\n";
	
	std::cout<<"Vleft :"<<Vleft<<" ; Vright :"<<Vright<<"\n";
	
	
	std::cout<<"Class left Interfacial Concentration is :" << IConcObjPtr->GetLeftInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Right Interfacial Concentration is :" << IConcObjPtr->GetRightInterfacialConcValueForClass(classId)<<"\n";
	std::cout<<"Class Radius is :" << currentRadiusDistribution_->GetRadiusForClass(classId)<<"\n";
	std::cout<<"Class Left Radius R-  is :" << currentRadiusDistribution_->GetLeftRadiusForClass(classId)<<"\n";
	std::cout<<"Class Right Radius R+  is :" << currentRadiusDistribution_->GetRightRadiusForClass(classId)<<"\n";
	
	
	
	/*TODO check if not needed or not*/  assert(!"An IMPOSSIBLE case has been found . leftInterfacialVelocity>0 AND rightInterfacialVelocity<0.   " );
	
	A(i,i)= deltar/deltat;
	A(i,i+1)= Vright;
	A(i,i-1)= -Vleft;
      }  
      std::cout<<"deltar  "<<deltar<<std::endl;
      std::cout<<"deltat "<<deltat<<std::endl;
      std::cout<<"Vright "<<Vright<<std::endl;
      std::cout<<"Vleft "<<Vleft<<std::endl;
      std::cout<<"A(i,i) "<<A(i,i)<<std::endl;
      std::cout<<"A(i,i+1) "<<A(i,i+1)<<std::endl;
      std::cout<<"A(i,i-1) "<<A(i,i-1)<<"\n"<<std::endl;
    }
/////////////////////////////////////////////////// End: MATRIX FEEDING  ///////////////////////////////////////////////////////////
  Acopy=A; /*Just for verification*/
  
  if (n<=100)
  {
    std::cout << "Matrix Aij :" << std::endl;
    std::cout << A << std::endl;
  } 
  
  for(int i=0; i<n; ++i)
  {
    int clId= i+1;
    double Ni= currentRadiusDistribution_->GetItemValueForClass(clId); 
    V(i)= deltar/deltat*Ni;
  }
  col.assign(V);
  
  Bcopy=B;/*Just for verification*/
  
  if (n<=100)
    std::cout << "Matrix of second member(s) B :" << B << std::endl;
  
  std::vector<int> ipiv (n);  // pivot vector
  // 
  // this is the resolution of AX=b, calling LAPACK "gesv" routine
  //
  int ierr = lapack::gesv(A, ipiv, B);
  //new values of B will be solutions X values
  //X=B;
  
  if (ierr != 0) 
    {
      assert(!"matrix is singular"); 
    }
  else
    {
      //Set solutions for currentRadiusDistribution values
      for(int i=0; i<n; ++i)
      {
	/*DEBUG*/ std::cout<<"B(i,j) is "<<B(i,0)<<std::endl;
	int clId= i+1;
	assert( (B(i,0)>=0)&&"A Solution found when solving linear system is negative. (N<0) " );
	currentRadiusDistribution_->SetItemValueForClass(clId, B(i,0) );
      }
      
      if (n<=100)
	{
	  std::cout << "Solution X (Remember that B is now equal to Solution) :"    << B << std::endl;//X=B
          std::cout << "Check : A*x :"    << ublas::prod(Acopy,B) << std::endl;//X=B
	  std::cout << "Check : Second member, initial value. (Bcopy) :"    << Bcopy << std::endl; //Bcopy: second member, initial values of B
	}
    }
  
}


void 
Precipitate::ResetCurrRadDisItemsIfValueIsLowerThan(double value)
{
  assert((value>0)&&"Given argument value must strictly be positive");
  
  
  int n=currentRadiusDistribution_->GetItemsValues().size();
  
  for ( int i=1; i<=n ; ++i)
  {
    if ( currentRadiusDistribution_->GetItemValueForClass(i) < value)
    {
      currentRadiusDistribution_->SetItemValueForClass( i, 0); //Reset to 0
    }
  }
  
}



void
Precipitate::Info() const
{
  std::cout <<  "################################# Precipitate::Info #################################" 	<< std::endl;
  std::cout <<  "                                    deltaCell: " << deltaCell_ << " SI unit" << std::endl;
  std::cout <<  "                       solvusActivationEnergy: " << solvusActivationEnergy_ << " SI unit" << std::endl;
  if (distorsionEnergy_==-1)
  {std::cout <<  "                             distorsionEnergy: Not defined" << std::endl;}
  else
  {std::cout <<  "                             distorsionEnergy: " << distorsionEnergy_ << " SI unit" << std::endl;};
  
  std::cout <<  "                        nucleationSitesNumber: " << nucleationSitesNumber_ << " SI unit" << std::endl;
  std::cout <<  "                          preExpTermForSolvus: " << preExpTermForSolvus_ << " SI unit" << std::endl;
 // std::cout <<  "               surfaceEnergyPolynomial: " << surfaceEnergyPolynomial_ << " SI unit" << std::endl;
  std::cout <<  "                                  shapeFactor: " << shapeFactor_ << " SI unit" << std::endl;
  std::cout <<  "                              volumicFraction: " << volumicFraction_ << " SI unit" << std::endl;
  std::cout <<  std::endl;
}

