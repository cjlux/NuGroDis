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
#include <cassert>
#include <map>
#include <cmath>
 
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

RadiusDistribution::RadiusDistribution(double deltar, double r1, double initialClassNb, Precipitate& P)
  :itemsValues_(),
   initialClassNumber_(initialClassNb),
   minRadius_(r1),
   spatialStep_(deltar),
   precipitate_(&P),
   chemicalElementList_(),
   interfConcentrationObjectMap_()
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
   interfConcentrationObjectMap_()
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
    chemicalElementList_.push_back(elementList[i]);
    
      
    if ( elementList[i]->GetElementName()!= mainElementName )
    { interfConcentrationObjectMap_[elementList[i]->GetElementName()]= new InterfacialConcentration(*this,*elementList[i]) ; };
    
  }
  assert ( elementList.size()==chemicalElementList_.size() );
  assert ( interfConcentrationObjectMap_.size()== elementList.size()-1 );
  
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
  constantC= DjOverDi-1;
  constantD= -XvSSi-DjOverDi*XvPi;
  
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
  
    
    assert( X != -constantD/constantC );// because denominator of Y must be different from 0
    Y = (constantA*X +constantB)/(constantC*X +constantD);  
    assert ( (Y!=-1 )&&"Solution Y may have not been computed" );
    assert( Y>0);
    assert( Y < XvPj);
    
      //DEBUG    
    std::cout<<"value solution X is: "<<X<<std::endl;
    std::cout<<"value solution Y is: "<<Y<<std::endl;
    
    return 1; //There is a solution
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
  
  this->InitializeInterfConc();
  
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
      
	// BEGIN: Compute gamma, DeltaGv, DeltaGe and CHECK IF they have been updated before//
	
	/*DEBUG*/ std::cout<<"In RadiusDis, adress of precipitate P is "<<P<<std::endl;
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
	double rightRadius, leftRadius;
	double f;
	
	
	
	
	assert ((itemsValues_.size()>0)&&"Radius Distribution class number is null or negative");
	
	std::cout<<" @@@@@@@@@@@@@@ Computing All Interfacial Concentration for precipitate type <"<<typeid(*precipitate_).name()<< " which adress is <"<< precipitate_<<"> @@@@@@@@@@@@@@ \n"<<std::endl;
	
	
	
	//Computes All interfacial concentration to the right : Xint_1 to Xint_n .  NOT Xint_0 will be compute separately
	for ( unsigned int classId=1; classId<=itemsValues_.size(); ++classId )
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
	  /*Debug*/ std::cout<<"f: "<<f<<" XvSSi: "<<XvSSi<<" XvPi: "<<XvPi<<" DjOverDi: "<<DjOverDi<<" XvPj: "<<XvPj<<" XvSSj: "<<XvSSj<<" solX"<<solX<<" solY"<<solY<<"\n"<<std::endl;
	  
	  if (solutionCase==1)
	  {
	     //Remember: XvintP for soluteList[0] is X or i and XvintP for soluteList[1] is Y or j
	    this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetRightInterfacialConcValueForClass(solX, classId);
	    this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetRightInterfacialConcValueForClass(solY, classId);
	  }
	  else 
	  {
	    assert (solutionCase==-1);
	    double X = this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).GetRightInterfacialConcValueForClass(classId);
	    double Y = this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).GetRightInterfacialConcValueForClass(classId);
	    this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetLeftInterfacialConcValueForClass(X, classId);
	    this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetLeftInterfacialConcValueForClass(Y, classId);
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
	    double X = this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).GetLeftInterfacialConcValueForClass(1);
	    double Y = this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).GetLeftInterfacialConcValueForClass(1);
	    
	    /**/std::cout<<"DEBUG: X= "<<X<<" Y= "<<Y<<std::endl;
	    this->GetInterfConcentrationObjectForElement(soluteList[0]->GetElementName()).SetCriticalInterfacialConcentration(X);
	    this->GetInterfConcentrationObjectForElement(soluteList[1]->GetElementName()).SetCriticalInterfacialConcentration(Y);
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
  
   
  std::string elementName;
  
  elementName=chemicalElementList_[0]->GetElementName(); //Normally,  interfacialVelocities must be the same for a precipitate, not depending from the choosen alloying element 
							// So, chemicalElementList_[0] or chemicalElementList_[1] (if it exists) must lead to the same result
  
  interfConcentrationObjectMap_[elementName]->ComputeInterfacialVelocityList();
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
    //The critical velocities must be the same whatever the element choose!!!
    assert ( elementsCriticalVelocityList[i]==elementsCriticalVelocityList[i+1] );
  }
  
  assert (elementsCriticalVelocityList.size()>0);
  return elementsCriticalVelocityList[0]; // elementsCriticalVelocityList[0] must be equal to  elementsCriticalVelocityList[1] = ...
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

