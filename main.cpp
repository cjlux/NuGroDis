// This file is part of NuGrodis, a workbench for micro-structural modelling.
//
// Author(s)    : - Jean-luc CHARLES, I2M/ENSAM Talence France
//                  <jean-luc.charles@ensam.eu>
//                - Dominique COUPARD, I2M/ENSAM Talence France
//                  <dominique.coupardensam.eu>
//                - Moubarak GADO, I2M/ENSAM Talence France
//                  <gadomoubarak@yahoo.fr>
//
// Copyright (C) 20014-2015 Jean-luc CHARLES, Dominique COUPARD, Moubarak GADO.
//

#include "libMetallurgical/SRC/Boundary.hpp"
#include "libMetallurgical/SRC/ChemicalComposition.hpp"
#include "libMetallurgical/SRC/ChemicalElement.hpp"
#include "libMetallurgical/SRC/Concentration.hpp"
#include "libMetallurgical/SRC/Computation.hpp"
#include "libMetallurgical/SRC/Diffusion.hpp"
#include "libMetallurgical/SRC/GuinierPreston.hpp"
#include "libMetallurgical/SRC/Hardening.hpp"
#include "libMetallurgical/SRC/InterfacialConcentration.hpp"
#include "libMetallurgical/SRC/Material.hpp"
#include "libMetallurgical/SRC/Polynomial.hpp"
#include "libMetallurgical/SRC/Precipitate.hpp"
#include "libMetallurgical/SRC/Quenching.hpp"
#include "libMetallurgical/SRC/RadiusDistribution.hpp"
#include "libMetallurgical/SRC/Grain.hpp"
#include "libMetallurgical/SRC/Sprime.hpp"
#include "libMetallurgical/SRC/SSGrain.hpp"
#include "libMetallurgical/SRC/Temperature.hpp"
#include "libMetallurgical/SRC/ThermoDynamicsConstant.hpp"
#include "libMetallurgical/SRC/Vacancy.hpp"
#include <iostream>
#include <typeinfo>


int main()
{
  std::cout << "Hello world" << std::endl;
  //Computation master;
  
  
  Boundary a;
  //Create classe temperature, default temperature is 293.15K=20°C
  Temperature tempClass;
  tempClass.Info();
  
  ChemicalElement mainCE(20,25,30,300000,"Al",25);//density, mMass, Ym,Pc,name,latticeParam
  mainCE.Info();
  
  ChemicalComposition InitialCC("solution solide");
  InitialCC.Info();
  
  Material material(tempClass,mainCE,InitialCC);
  //The material has SSgrains so create an SSGrain and then intialialize the material's ssgrain
  material.InitializeGrains();
  material.Info();
  
  

  //Sprime S(material,InitialCC,RD,30);
  //std::cout<<"meterial ssgrain "<<material.GetSSGrain();
  
  
  
  std::cout<<" >>Temperature courante =  "<<tempClass.GetCurrentTemp()<<" SI Unit"<<std::endl;
  std::cout<<" >>adresse  de objet Boundary a  =  "<<&a<<std::endl;
  std::cout<<" >>Initial formula or attribute 'formula' of C++ Object InitialCC: "<<InitialCC.GetFormula()<<std::endl;
  std::cout<<" >>Material Poisson coeff  = "<<material.GetPoissonCoeff()<<std::endl;
  std::cout<<" >>Material Young Modulus = "<<material.GetYoungModulus()<<std::endl;
  std::cout<<" >>Material Initial chemical composition = :";
  material.GetInitialChemicalComposition().Info();
  
  
  //BEGIn program
  
  //Create Object ThermoDynamicsConstant
  ThermoDynamicsConstant TDC(8.31,6,6.023);
  //Create CElements
  ChemicalElement Cu(27,2,2,0.3,"Cu",0.4),Mg(8,89,24,0.2,"Mg",0.8),Al=mainCE;
  //Create Vacancy
  Vacancy vcy(55.,88.,11.,87.,45.,450.,12.,material);//fake values!
  //Create quenching
  Quenching quenching(450,20,0.2);
  //Cretate Hardening
  Hardening hardening(150000,1);
  //Create a radius Distribution
  RadiusDistribution initialRD(4,1,20);
  //Create Object: Diffusion parameters. Atomic diffusion AND/OR vacancy diffusion
  Diffusion CuDiffusion(Cu,material,vcy,6.5E-5,135000.,19264.),MgDiffusion(Mg,material,vcy,1.2E-4,131000,19264);
  //Create Precipitates: GuinierPreston and Sprime
       //step1  create ChemicalCompositions for precipitates
       ChemicalComposition CCGP("Al8CuMg"), CCSprime("Al2CuMg");
       //Step2: Create Precipitates: GuinierPreston and Sprime
       GuinierPreston GP(material,CCGP,initialRD);
       Sprime Sp(material,CCSprime,initialRD,0.52);
       //step3: Set Precipitates properties manually or  with Precipitate.InitializeParameters(...)
       GP.SetDeltaCell(0.01);
       GP.SetSolvusActivationEnergy(36000);
       GP.SetNucleationSitesNumber(6.02214E28);
       GP.SetPreExpTermForSolvus(0.992);
       GP.SetShapeFactor(0);
       Sp.SetDeltaCell(0.01);
       Sp.SetSolvusActivationEnergy(76000);
       Sp.SetPreExpTermForSolvus(49.746);
       Sp.SetShapeFactor(20);
       Sp.SetMolarVolume(4E-5);
       //TODO: create a polynome and read polynome data, 
       Polynomial GPPoly(0),SpPoly(2);
       std::vector<double> GPCoefs,SpCoefs;
       GPCoefs.push_back(0.044);//GP: a0
       SpCoefs.push_back(-0.10985+0.1);//Sprime: a0
       SpCoefs.push_back(8.31E-4);//Sprime: a1
       SpCoefs.push_back(-8.71E-7);//Sprime: a2
       GPPoly.SetCoefs(GPCoefs);
       SpPoly.SetCoefs(SpCoefs);
       //Step4: Add precipitates material's SSgrain, therefore to the material
       //WARNING Any precipitate which belongs to the material's SSgrain belongs to the material. 
       //But all precipitates which belongs to the material does not belong to the material's SSGrain    
       (material.GetSSGrain())->AddPrecipitate(GP);
       (material.GetSSGrain())->AddPrecipitate(Sp);
       ChemicalComposition totoGP("Al4Cu2",GP);
       std::cout<<"GP class name "<<typeid(Sp).name()<<std::endl;
  
//if(!GPCoefs.empty())
//{
//    /* v is non-empty */
//    if(std::find(GPCoefs.begin(), GPCoefs.end(), 0.044) != GPCoefs.end())
//    {
//    /* v contains x */
//    std::cout<<"Value is contained in std::vector"<<std::endl;
//    } 
//    else 
//    {
//    /* v does not contain x */
//    std::cout<<"Value is not contained in std::vector"<<std::endl;
//    };
//} else 
//{
//  /* v is empty */
//   std::cout<<"cannot find value because std::vector is empty"<<std::endl;  
//};


  
  
  
  
  
  
  
  return 1;
}
