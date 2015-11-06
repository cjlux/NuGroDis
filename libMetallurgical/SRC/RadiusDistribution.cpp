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
#include <iostream>
#include <iomanip> //std::setw
#include <cassert>
#include <map>
#include <cmath>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <Python.h>

 
#include "RadiusDistribution.hpp"
#include "Precipitate.hpp"
#include "InterfacialConcentration.hpp"
#include "ChemicalComposition.hpp"
#include "ChemicalElement.hpp"
#include "Material.hpp"
#include "Util.hpp"
#include "Concentration.hpp"
#include "ThermoDynamicsConstant.hpp"
#include "Temperature.hpp"
#include "Diffusion.hpp"
#include "Computation.hpp"




RadiusDistribution::RadiusDistribution(double deltar, double r1, double initialClassNb, Precipitate& P)
  :itemsValues_(),
   initialClassNumber_(initialClassNb),
   minRadius_(r1),
   spatialStep_(deltar),
   precipitate_(&P),
   chemicalElementList_(),
   interfConcentrationObjectMap_(),
   InterfacialConcentrationObjectUsed_(0),
   InterfacialConcentrationObjectUsedHasBeenChosen_(false),
   chemicalElementChosen_(0)
{
  
  std::cout <<  "Building object <RadiusDistribution> " << std::endl;
  
  assert (spatialStep_>0);
  assert (minRadius_>0);
  assert (initialClassNumber_>0);
  assert ( ( (minRadius_-deltar/2)>0)&&"Error with given arguments: minRadius_ must be superior to deltar/2 " );// Because minRadius_-deltar/2 represent which is the first radius value of interfacial concentration   
  
  
  this->Initialize();
}

//CONSTRUCTOR of RD which does not have any precipitate. Can be used for initial Radius Distribution
RadiusDistribution::RadiusDistribution(double deltar, double r1, double initialClassNb)
  :itemsValues_(),
   initialClassNumber_(initialClassNb),
   minRadius_(r1),
   spatialStep_(deltar),
   precipitate_(0),
   chemicalElementList_(),
   interfConcentrationObjectMap_(),
   InterfacialConcentrationObjectUsed_(0),
   InterfacialConcentrationObjectUsedHasBeenChosen_(false),
   chemicalElementChosen_(0)
{
  std::cout <<  "Building object <RadiusDistribution> which is not linked to any Precipitate" << std::endl;
  assert (deltar>0);
  assert (r1>0);
  assert (initialClassNb>0);
  assert ( ( (minRadius_-deltar/2)>0)&&"Error with given arguments: minRadius_ must be superior to deltar/2 " );// Because minRadius_-deltar/2 represent the first radius value of interfacial concentration
  
  
  this->Initialize();
}


RadiusDistribution::~RadiusDistribution()
{
}


void
RadiusDistribution::AddEmptyClass()
{
  assert ((precipitate_!=0)&&"radiusDistribution does not belong to any precipitate");
  
  std::cout<<" Adding an empty class to RadiusDistribution of precipitate type <"<<typeid(precipitate_).name()<<"> ; precipitate adress is <"<<precipitate_<<"> "<<std::endl;
  unsigned int n= itemsValues_.size();
  itemsValues_.push_back(0);
  assert (itemsValues_.size()== n+1. );
  
  
  
  /*
   
  ////// 
  //Method2: IncrementWithEmptyValues() only with InterfacialConcentrationObjectUsed_ instead of //all  InterfObj of interfConcentrationObjectMap_
  /////
  
  assert (InterfacialConcentrationObjectUsed_!=0);
  std::cout<<"Incrementing with empty value the InterfacialConcentration Object for the used Element "<<InterfacialConcentrationObjectUsed_->GetChemicalElement().GetElementName()<<";InterfacialConcentration Object used Adress is :"<<InterfacialConcentrationObjectUsed_<<std::endl;
  InterfacialConcentrationObjectUsed_->IncrementWithEmptyValues();
  */
  
  
  
  
  
  
  ////// 
  //Method1: IncrementWithEmptyValues() with all  InterfObj of interfConcentrationObjectMap_
  /////
  std::cout<<"Incrementing with empty value All InterfacialConcentration Objects : \n ";
  std::map<std::string , InterfacialConcentration*>::iterator it;
  for (it=interfConcentrationObjectMap_.begin(); it!=interfConcentrationObjectMap_.end(); ++it )
  {
    std::cout<<"Incrementing with empty value the InterfacialConcentration Object for Element "<<it->first<<";InterfacialConcentration Object Adress is :"<<it->second<<std::endl;
    it->second->IncrementWithEmptyValues();
  }
  
  
  
}

void
RadiusDistribution::SaveDistribution()
{
  assert((precipitate_!=0)&&"radiusDistribution is not linked to any precipitate");
  std::string precipitateType= precipitate_->GetPrecipitateType();
  
  std::string relativeSaveFolderPath="RadDisFiles/"+precipitateType;
  precipitate_->GetMaterial().GetComputation().CreateDirectory(relativeSaveFolderPath);
  
  std::string ResultsDirectoryPath= precipitate_->GetMaterial().GetComputation().GetResultsDirectory();
  
  double CurrentTime = precipitate_->GetMaterial().GetComputation().GetCurrentTime();
  
  std::stringstream timeStrstream;
  timeStrstream<<CurrentTime;
  
  std::string fileName= "RadDis_time_"+timeStrstream.str()+"_.txt";
  std::string path=ResultsDirectoryPath+"/"+relativeSaveFolderPath+"/"+fileName;
  std::ofstream output_file;
  
  
  
  
  
  std::stringstream line;
  
  line<<"ClassId"<<std::setw(20)<<"R"<<std::setw(20)<<"NP"<<"\n"; 
  
  std::vector<double> RR=this->ReturnRadiusList();
  
  std::vector<std::string> lineStringVector;
  
  assert (lineStringVector.size()==0);
  lineStringVector.push_back(line.str());
  assert (lineStringVector.size()==1);
  
  
  for (unsigned int i=0; i<itemsValues_.size(); ++i )
  {
    std::stringstream lineStream;
    lineStream<<i+1<<std::setw(20)<<RR[i]<<std::setw(20)<<itemsValues_[i]<<"\n";
    lineStringVector.push_back(lineStream.str());
  }
  
  

  
  
  

  output_file.open(path.c_str());
  
  
  
  std::ostream_iterator<std::string> output_iterator(output_file, "\n");
  std::copy(lineStringVector.begin(), lineStringVector.end(), output_iterator);
  
  
}

std::vector<double> 
RadiusDistribution::ReturnRadiusList()
{
  unsigned int n=itemsValues_.size();
  std::vector<double> RR;
  
  assert (RR.size()==0);
  
  for (unsigned int i=1; i<=n; ++i )
  {
    RR.push_back(this->GetRadiusForClass(i));
  }
  assert (RR.size()==n);
  
  return RR;
}

void
RadiusDistribution::PlotPythonHistogram()
{
  

  std::cout<<"\n\n\n Printing distribution of precipitate <"<<typeid(*precipitate_).name()<<"> at adress <"<<precipitate_<<"> "<<std::endl;
  boost::python::list itemsValuesPyList;
  boost::python::list radiusPyList;
  unsigned int n=itemsValues_.size();
  
  for (unsigned int i=0; i<n; ++i )
  {
    itemsValuesPyList.append(itemsValues_[i]);
  }
  assert ( boost::python::len(itemsValuesPyList)== n);
  
  assert ( boost::python::len(radiusPyList)== 0);
  radiusPyList.append( this->GetLeftRadiusForClass(1)  );
  assert ( boost::python::len(radiusPyList)== 1);
  for (unsigned int i=1; i<=n; ++i )
  {
    radiusPyList.append( this->GetRightRadiusForClass(i)  );
  }
  assert ( boost::python::len(radiusPyList)== n+1);
  
  

  std::stringstream nPy, R1, NPy_n;
  nPy<<n;
  
  
  std::string rListString="[";
  R1<< (this->GetLeftRadiusForClass(1));
  rListString += R1.str();
  for (unsigned int i=1; i<=n; ++i )
  {
    std::stringstream RR;
    rListString +=",";
    RR<< (this->GetRightRadiusForClass(i));
    rListString += RR.str();
  }
  rListString +="]";
  
  
  std::string NlistString="[";
  for (unsigned int i=0; i<n-1; ++i )
  {
    std::stringstream NPy;
    NPy<< (itemsValues_[i]);
    NlistString +=NPy.str();
    NlistString +=",";
  }
  NPy_n<<(itemsValues_[n-1]);
  NlistString+=NPy_n.str();
  NlistString +=",0]";
  
  
  
  
  //boost::python::str m=(radiusPyList);
  
  
  
  
  
  
  std::string A = "P.hist(";
  std::string x=rListString;
  std::string B = ",bins=";
  std::string PyNumberOfBins= nPy.str();
  std::string C = ", histtype='bar', weights=";
  std::string PyWeights= NlistString;
  std::string D = ")";
  
  const char* expression= (A+x+B+PyNumberOfBins+C+PyWeights+D).c_str();
  
  
  std::cout<<"A "<<A<<"\nx "<<x<<"\nB "<<B<<"\nPynumberofbins "<<PyNumberOfBins<<"\nC "<<C<<"\npyweights "<<PyWeights<<"\nD "<<D;

  
  Py_Initialize();
  PyRun_SimpleString("import pylab as P");
  
  //PyRun_SimpleString("P.hist([3,4,5], bins=2,histtype='bar', weights=[1,2,0])");
  PyRun_SimpleString(expression);
  PyRun_SimpleString("P.show()");
  //Py_Exit(0);
  
  
  //Plotting the radius distribution
  const char* plottingExpression= ("X,Y="+rListString+","+NlistString).c_str();
  PyRun_SimpleString("import matplotlib.pyplot as plt");
  PyRun_SimpleString(plottingExpression);
  PyRun_SimpleString("fig, ax = plt.subplots()");
  PyRun_SimpleString("ax.plot(X,Y)");
  PyRun_SimpleString("ax.grid(True)");
  PyRun_SimpleString("ax.set_xlabel(u'Rayon [m]',color='green')");
  PyRun_SimpleString("ax.set_ylabel(u'Nombre de particules N ',color='red')");
  PyRun_SimpleString("plt.show()");
  
  
  //Py_Exit(0);
  
   
}


void
RadiusDistribution::PlotRadiusDistribution()
{  
 //TODO 
}
   
   
void
RadiusDistribution::CoutRadiusDistribution()
{
  for (unsigned int i=0; i<itemsValues_.size(); ++i)
  {
    std::cout<<"| Class <"<<i+1<<"> ;  N = << "<<itemsValues_[i]<<" >> |\n";
  }
}

void 
RadiusDistribution::Initialize()
{
  assert (initialClassNumber_>0);
  assert ((itemsValues_.size()==0)&&"In Initialize(): Cannot initialize because ItemsValues vector is not empty");
  for (int i=1; i<=initialClassNumber_; ++i)
  {
    itemsValues_.push_back(0);
  }
   
   //post assertion
   assert ( (int) itemsValues_.size()== initialClassNumber_);
}


const double
RadiusDistribution::ReturnTotNbOfItems()
{
  assert ( (itemsValues_.size()>0)&&"RadiusDistribution have not been initialize");
  
  double Sum=0;
  
  for ( unsigned int i=0; i<itemsValues_.size(); ++i )
  {
    Sum += itemsValues_[i];
  }
  
  assert (Sum>=0);
  
  return Sum;
}

double
RadiusDistribution::GetItemValueForClass(unsigned int classId)
{
  unsigned int n=itemsValues_.size();
  assert (n>0);
  assert ( (classId<=n)&&(classId>=1)&&"Given classId in argument is not in the range 1 to (size_of_RadiusDistribution)");
  
  double N = itemsValues_[classId-1];
  
  assert ( (N>=0)&&"Cannot GetItemValueForClass() because Item value Ni is negative !!!");

  return N; 
}

unsigned int 
RadiusDistribution::FindClassForRadius(double r)
{
  unsigned int classId=0;
  
  int n=itemsValues_.size();
  assert ((n>=1)&&"initial class number must be > 1 ");
  double MaxRadius=this->GetRightRadiusForClass(n);
  assert ((r >=minRadius_)&&(r<=MaxRadius)&&"radius given in argument must be in range rmin rmax");
  
  
  for ( int i=1; i<=n ; ++i )
  {
    double leftLimit=GetLeftRadiusForClass(i);
    double rightLimit=GetRightRadiusForClass(i);
  
    if ( (leftLimit <= r) && (r<= rightLimit) )
    {
      classId=i;  
    }
  }
  
  assert ( (classId!=0)&&"given radius argument is not in the RadiusDistribution");
 
  return classId;
}

void
RadiusDistribution::ReadInitialDistribution(std::string dataFile)
{
}

//Process Precipitates chemical compo and data, and create interfacialConcentrationObjects associated to each chemicalElement
//Initialize chemicalElementList_ and interfConcentrationObjectMap_
void
RadiusDistribution::InitializeInterfConc()
{
  //step0: assert if RD is initialRD or not (means if precipitate_=0 or not)
  assert( (precipitate_!=0)&&"Cannot InitializeInterfConc() : precipitate_=0. Cannot InitializeInterfConc() if the RadiusDistribution is not linked to any Precipitate " );
  
  assert(  (interfConcentrationObjectMap_.size()==0)&&(chemicalElementList_.size()==0)&&"InterfacialConcentration has already been initialized!!!"  );
  
  //Step1: scan precipitates ChemicalCompo
  const ChemicalComposition& compo=precipitate_->GetChemicalComposition();
  const std::vector<ChemicalElement *> & elementList = compo.GetChemicalElementsList();
  std::string mainElementName=precipitate_->GetMaterial().GetMainChemicalElement().GetElementName();
  
  
  //Step2: Add ChemicalElements to chemicalElementList and create interfacialConcentrationObjects associated to each chemicalElement 
  for (unsigned int i=0; i<elementList.size(); ++i)
  {
      
    if ( elementList[i]->GetElementName()!= mainElementName )
    { 
      chemicalElementList_.push_back(elementList[i]);
      interfConcentrationObjectMap_[elementList[i]->GetElementName()]= new InterfacialConcentration(*this,*elementList[i]); 
    };
    
  }
  assert ( elementList.size()==chemicalElementList_.size() + 1 );
  assert ( interfConcentrationObjectMap_.size()== elementList.size() - 1 );
  
}





//  Solve system of equations like:
// (XvSSi-X)/(XvPi-X)= DjOverDi *(XvSSj-Y)/(XvPj-Y) AND   X*Y= f 
// with unknown variables X>0 ,Y>0   
//X value will be return in variable X and Y value in Y.
int
RadiusDistribution::SolveInterfacialConcentrationsEquations(double f,
								   double XvSSi,
								   double XvPi,
								   double DjOverDi, 
								   double XvSSj,
								   double XvPj,
								   double & X,
								   double & Y)
{
  std::cout<<"Solving InterfacialConcentrations Equations "<<std::endl;
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
  const double k=1;
  nbOfSol=Util::Util::SolveSecondDegreeEquation(constantA/(k*k), (constantB-f*constantC)/k, -f*constantD,
						solution1,
						solution2,
						false); // 6th argument is false because we dont want complex solutions!!!!
  solution1.real()/=k;
  solution2.real()/=k;
  
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
     
      //if 2 solution of X, maybe one of them will give a negative or forbiden value for Y. Then we have to discuss. 
      //TODO
      
      if( (solution1.real()>0) && (solution1.real()<=XvPi) && (solution2.real()>0) && (solution2.real()<=XvPi) )
      {
	assert (!("TO DO. case 1 :This case has not been implemented yet"));
      }
      else if ( (solution1.real()>0) && (solution1.real()<=XvPi)  )
      {
	X=solution1.real();
      }
      else if ( (solution2.real()>0) && (solution2.real()<=XvPi)  )
      {
	X=solution2.real();
      }
      else
      {
	/*DEBUG*/std::cout<<" solution1.real() "<<solution1.real()<<" solution2.real() "<<solution2.real()<< "XvPi "<<XvPi<<std::endl;
	std::cout<<" There is no solutions. The 2 roots are not in the range 0 to XvPi ]0;"<< XvPi<<"]"<<std::endl;
	
	//assert (!("TO DO. case 4: This case has not been implemented yet"));
      }
      
    };
    
  };
  if (nbOfSol==1)
  {
    X=solution1.real();
  };
  
  
  if(  (X!=-1)&&(X > 0)&&(X < XvPi)  ) //there is a solution
  {
	assert ( (X!=-1 )&&"Solution X may have not been computed" );
	assert( X > 0);
	assert( X < XvPi);
	
	/*DEBUG:*/std::cout<<"DEBUG: Value Found using discriminant is"<<X<<"\n";
	/*DEBUG*/std::cout<<" P(X) with discriminant= "<< constantA*X*X + (constantB-f*constantC)*X + -f*constantD<<std::endl;
	////////////// Use dichotomous ////////////// 
	/*
	
	double leftLimit, rightLimit;
	
// 	double X2; // inadmissible solution
// 	if (X==solution1.real())
// 	{
// 	  X2= solution2.real();
// 	  
// 	}
// 	else if (X== solution2.real())
// 	{
// 	  X2= solution1.real();
// 	}
// 	else
// 	{
// 	  assert (!"there is something wrong with the 2 solutions ");
// 	};
// 	
// 	if (X>X2)
// 	{
// 	  leftLimit=X2;
// 	  rightLimit=X;
// 	}
// 	else if (X<X2)
// 	{
// 	  leftLimit=X;
// 	  rightLimit=X2;
// 	}
// 	else // X==X2
// 	{
// 	  assert (X==X2);
// 	  leftLimit=0;
// 	  rightLimit=XvPi;
// 	};
	
	  
	leftLimit=0;
 	rightLimit=XvPi;
	double dichoSolution=Util::Util::DichotomousMethodForSecondDegreeEquation(constantA , (constantB-f*constantC), -f*constantD, X, leftLimit, rightLimit ); // epsilon and alpha are optionnal. by default, epsilon=1.e-16 and alpha=0.1;
	X=dichoSolution;
	*/
	//////////// End ofuse dichotomous //////////// 
	
	
	assert( X != -constantD/constantC );// because denominator of Y must be different from 0
	//Y = (constantA*X +constantB)/(constantC*X +constantD);  
	Y=f/X;
	/*DEBUG*/std::cout<<"DEBUG if Y = (AX+B)/(CX+D) then Y= "<< (constantA*X + constantB)/(constantC*X + constantD)<<"\n";
	assert ( (Y!=-1 )&&"Solution Y may have not been computed" );
	
	
	/*Before the part in TEST we have:
	  assert( Y>0);
	  assert( Y < XvPj);
	  std::cout<<"value solution X is: "<<X<<std::endl;
	  std::cout<<" Ax2 +bx +c =0 ? "<< constantA*X*X + (constantB-f*constantC)*X + -f*constantD<<std::endl;
	  std::cout<<"value solution Y is: "<<Y<<std::endl;
	  std::cout<<"X*Y = : "<<X*Y<<std::endl;
	  std::cout<<"f = : "<<f<<std::endl;
	  std::cout<<" (XvSSi-X)/(XvPi-X) =  "<<(XvSSi-X)/(XvPi-X)<<std::endl;
	  std::cout<<"DjOverDi *(XvSSj-Y)/(XvPj-Y) = "<<DjOverDi *(XvSSj-Y)/(XvPj-Y)<<std::endl;
	  return 1; //There is a solution
	  
	*/
	
	
	
	/*This part is in TEST*/
	if( (Y>0)&&(Y<XvPj) )
	{
	  assert( Y>0);
	  assert( Y < XvPj);
	  std::cout<<"value solution X is: "<<X<<std::endl;
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
	  double epsilon=1e-5;
	  double Eq1precision=std::abs(X*Y-f);
	  double Eq2precision=std::abs( (XvSSi-X)/(XvPi-X) - DjOverDi*(XvSSj-Y)/(XvPj-Y) );
	  
	  std::cout<<"Precision for equation 1: X*Y=f is "<<Eq1precision<<"\n";
	  std::cout<<"Precision for equation 2: (XvSSi-X)/(XvPi-X) = DjOverDi *(XvSSj-Y)/(XvPj-Y) is "<<Eq2precision<<"\n";
	  
	  assert ( (Eq1precision<= epsilon) && "Precision for equation X*Y=f is not satisfied" );
	  assert ( (Eq2precision<= epsilon) && "Precision for equation (XvSSi-X)/(XvPi-X) = DjOverDi *(XvSSj-Y)/(XvPj-Y)  is not satisfied " );
	  
	  return 1; //There is a solution
	  
	}
	else //Y solution is not admissible, then, its means there is no admissible couple {X,Y} solution
	{ 
	  std::cout<<" Y= <"<<Y<<"> is not an admissible solution, then, its means there is no admissible couple {X,Y} solution \n";
	  std::cout<<"Y= <"<<Y<<"> is not in range ]0;"<<XvPj<<"[ \n"; 
	  return -1;
	};
	/*This part is in TEST*/
	

	
    

  }
  else //There is no solution
  {

    std::cout<<"There is No solution:  X is: "<<X<<std::endl;
    std::cout<<"There is No solution:  Y is: "<<Y<<std::endl;
    
    return -1;// there is no solution
  };
  
  
}








void 
RadiusDistribution::ComputeAllInterfacialConcentrations()
{
  
  
  Precipitate* P;
  P= precipitate_;
  

  
  
  assert ( (precipitate_ != 0)&&"Radius distribution is not linked to any precipitate!"    ); 
  assert ( ( P->GetMaterialPointer()!=0)&&"In ComputeInterfacialConcentration(): Precipitate is not linked to any material !!!");
  
  
  if ((interfConcentrationObjectMap_.size()==0)&&(chemicalElementList_.size()==0))
  {
    this->InitializeInterfConc();
  }
  
  
  
  assert( (interfConcentrationObjectMap_.size()!=0)&&"Interfacial concentratration have not been initialized for RadiusDisutribution.\
  Run RadiusDistribution::InitializeInterfConc() before Computing Interfacial Concentrations " );
  
  //if Chemical composition has 3 elements  A_a__B_x__C_y ( like Al_a__Cu_x__Mg_y)*
  if (interfConcentrationObjectMap_.size()==2)
  {
    
    std::map<std::string, Concentration*> precipitateConcMap= P->GetChemicalComposition().GetConcentrationMap();
    
    assert ( (P->GetMaterialPointer()!=0)&&"Precipitate is not linked to any material");
    
    std::string mainElementSymbol= P->GetMaterialPointer()->GetMainChemicalElement().GetElementName();
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
     

	
	double K=P->GetPreExpTermForSolvus();
	double DeltaH=P->GetSolvusActivationEnergy();
	double R=ThermoDynamicsConstant::GetR();
	double T=P->GetMaterial().GetTemperature().GetCurrentTemp();
	double alphaP= P->GetShapeFactor();
	double Vm= P->GetMolarVolume();
      
	// BEGIN: Compute gamma, DeltaGe, and CHECK IF they have been updated before//
	
	/*DEBUG*/ std::cout<<"In RadiusDis, adress of precipitate P is "<<P<<std::endl;
	double oldGamma=P->GetSurfaceEnergyCurrentValue();
	double oldDeltaGe=P->GetDistorsionEnergy();
	P->ComputeDistorsionEnergy(); //compute another value of distorsionEnergy_
	P->ComputeSurfaceEnergy();// compute gamma
  
	double gamma= P->GetSurfaceEnergyCurrentValue();
	double DeltaGe= P->GetDistorsionEnergy();
	
	std::map<std::string, Concentration*> materialCurrentConcMap=P->GetMaterial().GetCurrentChemicalComposition().GetConcentrationMap();

	
	
	assert ( (gamma==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before\
	running method ComputeInterfacialConcentrations");

	assert ( (DeltaGe==oldDeltaGe)&&"may be distorsionEnergy_ has not been computed before.\
	Error when running method Precipitate::ReturnCriticalRadius()");
      // END: Compute gamma, DeltaGe, and CHECK IF they have been updated before //
	
	std::vector<const ChemicalElement*> soluteList =P->GetMaterial().GetSoluteList();
	
	assert ((soluteList.size()==2)&&"Solute list must have 2 chemical element");
	assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"Division by 0");//soluteList[0] is X or i and soluteList[1] is Y or j
	assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"AtomicDiffusion coef is 0: No sense");
	
	
// 	//Begin assert if D_i_SS has been computed before and if they have been updated
// 	//TODO
// 	std::string element_O_name= soluteList[0]->GetElementName();
// 	std::string element_1_name= soluteList[1]->GetElementName();
// 	

// 	
// 	double oldXvSS_O= materialCurrentConcMap[element_O_name]->GetVolumicValue();
// 	double oldXvSS_1= materialCurrentConcMap[element_1_name]->GetVolumicValue();
// 	
// 	precipitate_->GetMaterial().UpdateVolumicValues(); 
// 	
// 	double XvSS_O=materialCurrentConcMap[element_O_name]->GetVolumicValue();
// 	double XvSS_1= materialCurrentConcMap[element_1_name]->GetVolumicValue();	
// 	
// 	/*DEBUG*/std::cout<<"oldXvSS_O "<<oldXvSS_O<<" XvSS_O "<<XvSS_O<<"oldXvSS_1"<<oldXvSS_1<<"XvSS_1"<<XvSS_1<<  std::endl;
// 	/*DEBUG */std::cout<< " oldXvSS_O-XvSS_O "<<oldXvSS_O-XvSS_O<<" oldXvSS_1-XvSS_1 "<<oldXvSS_1-XvSS_1<<std::endl; 
	
	//assert ( (oldXvSS_O==XvSS_O)&&(oldXvSS_1==XvSS_1)&&"may be volumic values has not been computed or updated before. Error when running method RadiusDistribution::ComputeAllInterfacialConcentrations()");
// 	
// 	//TODO pas fini encore. Il Reste à checker les valeurs des coefficient des diffusion atomic D_i_SS. N'est cer pas mieux tout simplement de faire l'update avant la methode RadiusDistribution::ComputeAllInterfacialConcentrations() ?
// 	
// 	//End assert if D_i_SS has been computed before and if they have been updated
	
	
	double DjOverDi= soluteList[1]->GetDiffusion().GetAtomicDiffusionCoef()/soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef();// example: D_Mg_SS/D_Cu_SS
	//Because of ratio DjOverDi , solution for soluteList[0] will be X or i and solution for soluteList[1] will be Y or j
	double solX=-1, solY=-1;
	double XvPj=precipitateConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
	double XvPi=precipitateConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
	double XvSSj =materialCurrentConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
	double XvSSi =materialCurrentConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
	double rightRadius, leftRadius;
	double f;
	
	
	
	
	assert ((itemsValues_.size()>0)&&"Radius Distribution class number is null or negative");
	
	std::cout<<" @@@@@@@@@@@@@@ Computing All Interfacial Concentration for precipitate type <"<<typeid(*precipitate_).name()<< " which adress is <"<< precipitate_<<"> @@@@@@@@@@@@@@ \n"<<std::endl;
	
	
	
	//Computes All interfacial concentration to the right : Xint_1 to Xint_n .  NOT Xint_0 will be compute separately
	for ( unsigned int classId=itemsValues_.size(); classId>=1; --classId )
	{
	  
	  
	  rightRadius=this->GetRightRadiusForClass(classId) ;
	
	  f=K*std::exp(-DeltaH/(R*T))*std::exp( 4./3.*(alphaP+2)/(4./3.+alphaP)*Vm*gamma/(R*T*rightRadius) + DeltaGe*Vm/(R*T)) ;
	  

	  std::cout<<"4./3.*(alphaP+2)/(4./3.+alphaP)*Vm*gamma/(R*T*rightRadius)+ DeltaGe*Vm/(R*T) " <<4./3.*(alphaP+2)/(4./3.+alphaP)*Vm*gamma/(R*T*rightRadius)+ DeltaGe*Vm/(R*T)<<std::endl;
	  
	  
	  std::cout<<"DeltaGe*Vm"<<DeltaGe*Vm<<std::endl;
	  std::cout<<" ClassId <"<<classId<<"> right interface"<<std::endl;
	  //solving system of equation:  (XvSSi-solX)/(XvPi-solX)= DjOverDi *(XvSSj-solY)/(XvPj-solY) AND   X*Y= f 
	  //with unknown variables solX>0 , solY>0   
	  int solutionCase=this->SolveInterfacialConcentrationsEquations(f,
						  XvSSi,
						  XvPi,
						  DjOverDi,
						  XvSSj,
						  XvPj,
						  solX,
						  solY);
	  /*Debug*/ std::cout<<"f: "<<f<<" XvSSi: "<<XvSSi<<" XvPi: "<<XvPi<<" DjOverDi: "<<DjOverDi<<" XvPj: "<<XvPj<<" XvSSj: "<<XvSSj<<" solX"<<solX<<" solY"<<solY<<" solution case is "<<solutionCase<<"\n"<<std::endl;
	  
	  if (solutionCase==1)
	  {
	     //Remember: XvintP for soluteList[0] is X or i and XvintP for soluteList[1] is Y or j
	    this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetRightInterfacialConcValueForClass(solX, classId);
	    this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetRightInterfacialConcValueForClass(solY, classId);
	  }
	  else 
	  {
	    
	    assert (solutionCase==-1);
	    
	    //In this case, we take right interfacial concentration of class (i+1)
	    double X = this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).GetRightInterfacialConcValueForClass(classId + 1);
	    double Y = this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).GetRightInterfacialConcValueForClass(classId + 1);
	    /*DEBUG */std::cout<<"X of Class Id + 1 is "<<this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).GetRightInterfacialConcValueForClass(classId + 1)<<"\n"; 
	    
	    /*DEBUG */std::cout<<"X of Class Id + 2 is "<<this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).GetRightInterfacialConcValueForClass(classId + 2)<<"\n"; 
	    
	    /*DEBUG */std::cout<<"Class Id is "<<classId<<"\n";
	    
	    /*DEBUG */std::cout<<"Class Id is "<<classId<<"\n"; 
	    /*DEBUG */std::cout<<"X is "<<X<<" Y is "<<Y<<"\n"; 
	    assert (X!=0);
	    assert(Y!=0);
	    this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetRightInterfacialConcValueForClass(X, classId);
	    this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetRightInterfacialConcValueForClass(Y, classId);
	  };
	  
	  
	  
	}
	
	//REMEMBER!!!: Always compute the first concentration ( left concentration for class 1 ) after computing right concentration for class n to 1 !!!!!!!!
	//because if X[[0]]={} then X[[0]]= X[[1]] i.e right concentration for class 1. riht interface for class 1 must be then computed before.
	//Compute the LEFT INTERFACIAL concentration for the FIRST class (Xint_0)
	std::cout<<" ClassId <"<<1<<"> left interface"<<std::endl;
	leftRadius=this->GetLeftRadiusForClass(1) ;
	f=K*std::exp(-DeltaH/(R*T))*std::exp( 4./3.*(alphaP+2)/(4./3.+alphaP)*Vm*gamma/(R*T*leftRadius) + DeltaGe*Vm/(R*T)) ;
	//solving system of equation:  (XvSSi-solX)/(XvPi-solX)= DjOverDi *(XvSSj-solY)/(XvPj-solY) AND   X*Y= f 
	//with unknown variables solX>0 , solY>0   
	int solutionCase =this->SolveInterfacialConcentrationsEquations(f,
						  XvSSi,
						  XvPi,
						  DjOverDi,
						  XvSSj,
						  XvPj,
						  solX,
						  solY);
	
	
	/*Debug*/ std::cout<<"f: "<<f<<" XvSSi: "<<XvSSi<<" XvPi: "<<XvPi<<" DjOverDi: "<<DjOverDi<<" XvPj: "<<XvPj<<" XvSSj: "<<XvSSj<<" solX"<<solX<<" solY"<<solY<<"\n"<<std::endl;
	
	if (solutionCase==1)
	{
	  //Remember: XvintP for soluteList[0] is X or i and XvintP for soluteList[1] is Y or j
	  this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetLeftInterfacialConcValueForClass(solX, 1);
	  this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetLeftInterfacialConcValueForClass(solY, 1);
	}
	else 
	{
	  assert (solutionCase==-1);
	  double X = this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).GetRightInterfacialConcValueForClass(1);
	  double Y = this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).GetRightInterfacialConcValueForClass(1);
	  this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetLeftInterfacialConcValueForClass(X, 1);
	  this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetLeftInterfacialConcValueForClass(Y, 1);
	  std::cout<<"value given to X is "<<X<<std::endl;
	  std::cout<<"value given to Y is "<<Y<<std::endl;
	};
	

      
	
		
      
      
      
    }
    else
    {
      //TODO later
      assert(!("Resolution for ternary alloy with alloying elements stoichiometry other than 1 has not been implemented yet!!!\
      example: AlxCu1Mg1 is GOOD, but,  AlxCu2Mg2, AlxCu1Mg3, AlxCu2Mg3, ... are not WORKING "));
    }
    
  };
  
  
  assert( (interfConcentrationObjectMap_.size()==2)&&"Interfacial concentratrion Resolution for alloy other than \
  ternary alloy has not been implemented yet." );//TODO later....,
  
}
  

void 
RadiusDistribution::ComputeCriticalInterfacialConcentration()
{
  Precipitate* P;
  P= precipitate_;
  
  std::cout<<"Computing Critical Interfacial Concentration for precipitate type <"<<typeid(*precipitate_).name()<< " which adress is <"<< precipitate_<<"> "<<std::endl;
  
  assert ( (precipitate_ != 0)&&"Radius distribution is not linked to any precipitate!"    ); 
  assert ( ( P->GetMaterialPointer()!=0)&&"In ComputeCriticalInterfacialConcentration(): Precipitate is not linked to any material !!!");
  
  
  assert( (interfConcentrationObjectMap_.size()!=0)&&"Interfacial concentration have not been initialized for RadiusDisutribution.\
  Run RadiusDistribution::InitializeInterfConc() before RadiusDistribution::ComputeCriticalInterfacialConcentration() " );
  
  
  double rEtoile;
  rEtoile= P->GetCriticalRadius();

  
  
  if ( rEtoile!=-111.111  ) // -111.111 means "ne s'applique pas"
  {
   
    
      //if Chemical composition has 3 elements  A_a__B_x__C_y ( like Al_a__Cu_x__Mg_y)*
    if (interfConcentrationObjectMap_.size()==2)
    {
      
      std::map<std::string, Concentration*> precipitateConcMap= P->GetChemicalComposition().GetConcentrationMap();
      
      assert ( (P->GetMaterialPointer()!=0)&&"Precipitate is not linked to any material");
      
      std::string mainElementSymbol= P->GetMaterialPointer()->GetMainChemicalElement().GetElementName();
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
      

	  
	  double K=P->GetPreExpTermForSolvus();
	  double DeltaH=P->GetSolvusActivationEnergy();
	  double R=ThermoDynamicsConstant::GetR();
	  double T=P->GetMaterial().GetTemperature().GetCurrentTemp();
	  double alphaP= P->GetShapeFactor();
	  double Vm= P->GetMolarVolume();
	
	  // BEGIN: Compute gamma, DeltaGv, DeltaGe and CHECK IF they have been updated before//
	  
	  double oldGamma=P->GetSurfaceEnergyCurrentValue();
	  double oldDeltaGv=P->GetPhaseChangeVolumicEnergy();
	  double oldDeltaGe=P->GetDistorsionEnergy();
	  P->ComputePhaseChangeVolumicEnergy();//compute another value of phaseChangeVolumiqueEnergy_
	  P->ComputeDistorsionEnergy(); //compute another value of distorsionEnergy_
	  P->ComputeSurfaceEnergy();
    
	  double gamma= P->GetSurfaceEnergyCurrentValue();
	  double DeltaGv= P->GetPhaseChangeVolumicEnergy();
	  double DeltaGe= P->GetDistorsionEnergy();
	  

	  
	  
	  assert ( (gamma==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before\
	  running method ComputeInterfacialConcentrations");
	  assert ( (DeltaGv==oldDeltaGv)&&"may be phaseChangeVolumiqueEnergy_ has not been computed before.\
	  Error when running method Precipitate::ReturnCriticalRadius()");
	  assert ( (DeltaGe==oldDeltaGe)&&"may be distorsionEnergy_ has not been computed before.\
	  Error when running method Precipitate::ReturnCriticalRadius()");
	// END: Compute gamma, DeltaGv, DeltaGe and CHECK IF they have been updated before //
	  
	  std::vector<const ChemicalElement*> soluteList =P->GetMaterial().GetSoluteList();
	  
	  assert ((soluteList.size()==2)&&"Solute list must have 2 chemical element");
	  assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"Division by 0");//soluteList[0] is X or i and soluteList[1] is Y or j
	  assert (soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef()!=0&&"AtomicDiffusion coef is 0: No sense");
	  
	  
	  double DjOverDi= soluteList[1]->GetDiffusion().GetAtomicDiffusionCoef()/soluteList[0]->GetDiffusion().GetAtomicDiffusionCoef();// example: D_Mg_SS/D_Cu_SS
	  //Because of ratio DjOverDi , solution for soluteList[0] will be X or i and solution for soluteList[1] will be Y or j
	  double solX=-1, solY=-1;
	  double XvPj=precipitateConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
	  double XvPi=precipitateConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
	  std::map<std::string, Concentration*> materialCurrentConcMap= P->GetMaterial().GetCurrentChemicalComposition().GetConcentrationMap();
	  double XvSSj =materialCurrentConcMap[soluteList[1]->GetElementName()]->GetVolumicValue();
	  double XvSSi =materialCurrentConcMap[soluteList[0]->GetElementName()]->GetVolumicValue();
	  double f;
	  
	  // BEGIN: Compute criticalradius and CHECK IF it has been updated before. criticalradius will be used to compute criticalInterfacialConcentrationRadius
	  double oldCriticalRadius= P->GetCriticalRadius();
	  double criticalRadius = P->ReturnCriticalRadius();
	  /*DEBUG*/ std::cout<<"oldCriticalRadius is <<"<<oldCriticalRadius<<" new critical radius computed is "<<criticalRadius<<std::endl;
	  assert ( (oldCriticalRadius==criticalRadius)&&"may be criticalradius_ has not been computed before\
	  running method ComputeCriticalInterfacialConcentration()");
	  // END:  Compute criticalradius and CHECK IF it has been updated before
	  
	  
	  double criticalInterfacialConcentrationRadius= P->ReturnCriticalInterfacialConcentrationRadius();
	  //Compute the critical interfacial concentration for the criticalInterfacialConcentrationRadius. Compute XvINTcritiqueP_i and XvINTcritiqueP_j
	  
	  std::cout<<"Computing Critical Interfacial Concentration for precipitate type <"<<typeid(*precipitate_).name()<< " which adress is <"<< precipitate_<<"> "<<std::endl;
	  
	  f=K*std::exp(-DeltaH/(R*T))*std::exp( 4/3*(alphaP+2)/(4/3+alphaP)*Vm*gamma/(R*T*criticalInterfacialConcentrationRadius) + DeltaGe*Vm/(R*T)) ;
	  //solving system of equation:  (XvSSi-solX)/(XvPi-solX)= DjOverDi *(XvSSj-solY)/(XvPj-solY) AND   X*Y= f 
	  //with unknown variables solX>0 , solY>0   
	  int solutionCase=this->SolveInterfacialConcentrationsEquations(f,
						    XvSSi,
						    XvPi,
						    DjOverDi,
						    XvSSj,
						    XvPj,
						    solX,
						    solY);
	  
	  /*Debug*/ std::cout<<"f: "<<f<<" XvSSi: "<<XvSSi<<" XvPi: "<<XvPi<<" DjOverDi: "<<DjOverDi<<" XvPj: "<<XvPj<<" XvSSj: "<<XvSSj<<" solX"<<solX<<" solY"<<solY<<std::endl;
	  
	  if (solutionCase==1)
	  {
	    //Remember: XvintP for soluteList[0] is X or i and XvintP for soluteList[1] is Y or j
	    this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetCriticalInterfacialConcentration(solX);
	    this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetCriticalInterfacialConcentration(solY);
	  }
	  else 
	  {
	    assert (solutionCase==-1);
	    
	    std::cout<<"Critical interfacial concentration cannot be found at this time because we have no solution. "<<std::endl; //222.222 will be setted
	    this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetCriticalInterfacialConcentration(222.222); //222.222 means "n'existe pas"
	    this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetCriticalInterfacialConcentration(222.222); //222.222 means "n'existe pas"
	  };
	  
	 
	  
      }
      else
      {
	//TODO later
	assert(!("In ComputeCriticalInterfacialConcentration(): Resolution for ternary alloy with alloying elements stoichiometry other than 1 has not been implemented yet!!!\
	example: AlxCu1Mg1 is GOOD, but,  AlxCu2Mg2, AlxCu1Mg3, AlxCu2Mg3, ... are not WORKING "));
      }
      
    };
    
    
    assert( (interfConcentrationObjectMap_.size()==2)&&"In ComputeCriticalInterfacialConcentration(): Interfacial concentratrion Resolution for alloy other than \
    ternary alloy has not been implemented yet." );//TODO later....,
       
  }
  else
  {
    std::cout<<"ComputeCriticalInterfacialConcentration() : CriticalInterfacialConcentration has not been computed because\
    there is no new Nucleated precipitate for <<"<<typeid(*P).name()<<">> which adress is <"<< P<<"> at this point"<<std::endl;
  };
  
  
}


void 
RadiusDistribution::ComputeInterfacialVelocityList()
{
  
  if ( InterfacialConcentrationObjectUsedHasBeenChosen_== false)
  {
    assert ((InterfacialConcentrationObjectUsedHasBeenChosen_==false)&&"InterfacialConcentration Object used for Interfacial Velocity has already been chosen");
    
    std::string chosenElementName;
    
    chemicalElementChosen_= chemicalElementList_[0];
    
    assert (chemicalElementChosen_!=0);
    chosenElementName=chemicalElementChosen_->GetElementName(); //Normally,  interfacialVelocities must be the same for a precipitate, not depending from the choosen alloying element 
							// So, chemicalElementList_[0] or chemicalElementList_[1] (if it exists) must lead to the same result
    std::cout<<"Element chosen is : "<<chosenElementName<<"\n";
  
  
   InterfacialConcentrationObjectUsed_= interfConcentrationObjectMap_[chosenElementName];
   InterfacialConcentrationObjectUsedHasBeenChosen_=true;
   
   InterfacialConcentrationObjectUsed_->ComputeInterfacialVelocityList();
  
  }
  else //  (InterfacialConcentrationObjectUsedHasBeenChosen_== true)
  {
    assert ((InterfacialConcentrationObjectUsedHasBeenChosen_==true)&&"InterfacialConcentration Object used for Interfacial Velocity has not been chosen");
    InterfacialConcentrationObjectUsed_->ComputeInterfacialVelocityList();
  };
   
}


  
const double 
RadiusDistribution::ReturnCriticalInterfacialVelocity()
{
  assert (chemicalElementList_.size()>0);
  assert (interfConcentrationObjectMap_.size()>0);
  
  std::map<std::string, InterfacialConcentration*>::iterator it;
  
  std::vector<double> elementsCriticalVelocityList;
  for (it=interfConcentrationObjectMap_.begin(); it!=interfConcentrationObjectMap_.end(); ++it )
  {
    
    double criticalV;
    
    criticalV= it->second->ReturnCriticalInterfacialVelocity();//it->second is an InterfacialCocentratyion Object!!!
    
    elementsCriticalVelocityList.push_back(criticalV); 
  }
  
  
  
  for (unsigned int i=0; i<elementsCriticalVelocityList.size()-1; ++i)
  {
    
    /*DEBUG*/std::cout<<"elementsCriticalVelocityList[i] = "<<elementsCriticalVelocityList[i]<<"\n elementsCriticalVelocityList[i+1] = "<<elementsCriticalVelocityList[i+1]<<"\n\n"<<std::endl;
    //The critical velocities must be the same whatever the element choose!!!
    //TODO assert ( elementsCriticalVelocityList[i]==elementsCriticalVelocityList[i+1] );
  }
  
  assert (elementsCriticalVelocityList.size()>0);
  return elementsCriticalVelocityList[0]; // elementsCriticalVelocityList[0] must be equal to  elementsCriticalVelocityList[1] = ...
} 
  



double
RadiusDistribution::ReturnInterfacialVelocityListFirstElement()
{
  std::string chosenElementName;
  
  assert ((chemicalElementChosen_!=0)&&"No chemical element has been chosen for interfacial velocity");
  chosenElementName=chemicalElementChosen_->GetElementName(); //Normally,  interfacialVelocities must be the same for a precipitate, not depending from the choosen alloying element 
							// So, chemicalElementList_[0] or chemicalElementList_[1] (if it exists) must lead to the same result
  
  
  
   std::vector<double>ICList= this->GetInterfConcentrationObjectForElement(chosenElementName).GetInterfacialVelocityList();
  
  return ICList[0];

}





void
RadiusDistribution::Info() const
{
  std::cout <<  "######################## RadiusDistribution::Info ######################" 	<< std::endl;
  //std::cout <<  "                                itemsValues: " <<itemsValues_ << " SI unit" << std::endl;
  std::cout <<  "                         initialClassNumber: " << initialClassNumber_<< " SI unit" << std::endl;
  std::cout <<  "                                  minRadius: " << minRadius_<< " SI unit" << std::endl;
  std::cout <<  "                                spatialStep: " << spatialStep_<< " SI unit" << std::endl;
  std::cout <<  "                                                                 " 	<< std::endl;
}

