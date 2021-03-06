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
#include <cmath>


#include "GuinierPreston.hpp"
#include "Polynomial.hpp"
#include "Material.hpp"
#include "RadiusDistribution.hpp"
#include "ChemicalElement.hpp"
#include "Util.hpp"




GuinierPreston::GuinierPreston(Material& mat, ChemicalComposition &CC, RadiusDistribution &initialRD)
  :Precipitate(mat,CC,initialRD),
   precipitateType_("GuinierPreston")
{
}

GuinierPreston::~GuinierPreston()
{
}


double //TODO
GuinierPreston::ReturnCriticalTotalEnergy()
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
  
  assert((phaseChangeVolumiqueEnergy_+distorsionEnergy_)!=0);
  
  double criticalDeltaG= 32./27.*M_PI*std::pow(surfaceEnergyCurrentValue_,3.)/std::pow( (phaseChangeVolumiqueEnergy_+distorsionEnergy_),2.)*std::pow((shapeFactor_+2.),3.)/std::pow((shapeFactor_+4./3.),2.); 
  
  assert (criticalDeltaG>0);
  
  std::cout<<"ReturnCriticalTotalEnergy   "<<criticalDeltaG<<std::endl;
  
  return criticalDeltaG;
}


void
GuinierPreston::ComputeCriticalTotalEnergy()
{
  criticalTotalEnergy_=this->ReturnCriticalTotalEnergy();
}

void
GuinierPreston::ComputeNucleationSiteNb()
{
  
  std::cout<<"Computing Nucleation Site Number For GuinierPreston Precipitate"<<std::endl;
  
  std::vector<Precipitate *> precipitateList = materialPointer_->GetPrecipitateList();
  //TODO  
  std::vector<Precipitate *> GPList;//List of GuinierPreston
  
  
  
  for ( unsigned int i=0; i<precipitateList.size(); ++i  )
  {
    std::string type= precipitateList[i]->GetPrecipitateType();
    if (type=="GuinierPreston")
    {
      GPList.push_back(precipitateList[i]) ;
    } 
  }
  
  
  double SumOfFracOfAllGP=0;
  double am = materialPointer_->GetMainChemicalElement().GetLatticeParameter();
  
  for (unsigned int i=0; i<GPList.size(); ++i )
  {
    
    double fracVol= GPList[i]->ReturnVolumicFraction();
    
    double NOccupiedSiteGP_i= (4./std::pow(am,3.))*fracVol;
    
    assert (NOccupiedSiteGP_i>=0);
    
    SumOfFracOfAllGP += NOccupiedSiteGP_i;
    
  }
  
  
  nucleationSitesNumber_= initialNucleationSitesNumber_ - SumOfFracOfAllGP ;
  
  
  if( nucleationSitesNumber_<0)
  {
    std::cout<<"Nuclueation site Number for precipitate type <GuinierPreston> at adress <"<<this<<"> is negative => IT WILL BE SETTED TO ZERO";
    nucleationSitesNumber_=0;
  }
  

  
  assert (nucleationSitesNumber_>=0);
  
  /*DEBUG*/ std::cout<<"Computed Nucleation sites number for Guinier Preston is : "<<nucleationSitesNumber_<<std::endl;
  
}

double 
GuinierPreston::ReturnVolumicFraction()
{
  unsigned int n=currentRadiusDistribution_->GetItemsValues().size();
  
  assert (n>0);
  
  
  std::vector<double> volumicFractionOfClassVector;
  double sumWithMethod=0;
  
  /////////////////////////////////
  ///////// Normal sum //////////
  double Sum=0;
  for (unsigned int i=1; i<=n; ++i)
  {
    double sum_i;
    double Ri=currentRadiusDistribution_->GetRadiusForClass(i);
    double Ni=currentRadiusDistribution_->GetItemValueForClass(i);
    sum_i= (4./3. + shapeFactor_)*M_PI*Ni*std::pow(Ri,3);
    assert (sum_i >= 0);
    volumicFractionOfClassVector.push_back(sum_i);
    Sum +=sum_i;
  }
  sumWithMethod=Sum;
  //////////////////////////
  
  
  //Method 3, sort, and making my own sum
  /*
  std::sort(volumicFractionOfClassVector.begin(),volumicFractionOfClassVector.end());
  double vectorSum=0;
  for (unsigned int i=0; i<volumicFractionOfClassVector.size(); ++i )
  {
    vectorSum += volumicFractionOfClassVector[i];
  }
  sumWithMethod=vectorSum;
  */
  
  //Method 4, using a special algotithm for the sum : modified_deflation
  /*
  //sumWithMethod = Util::Util::modified_deflation(volumicFractionOfClassVector);
  */
  
  //Method 2, sort, and using accumalate for the sum
  /*
  std::sort(volumicFractionOfClassVector.begin(),volumicFractionOfClassVector.end());
  sumWithMethod = std::accumulate(volumicFractionOfClassVector.begin(),volumicFractionOfClassVector.end(), 0.0);
  */
  
  assert (sumWithMethod>=0);
  return sumWithMethod;
}

void
GuinierPreston::ComputeVolumicFraction()
{ 
  volumicFraction_=this->ReturnVolumicFraction(); 
}




void
GuinierPreston::ReadDataValue(std::string fileName)
{
}

void
GuinierPreston::Info() const
{
  std::cout <<  "=====================================================================================" 	<< std::endl;
  std::cout <<  "#############################  GuinierPreston::Info #################################" 	<< std::endl;
  std::cout <<  "=====================================================================================" 	<< std::endl;
  std::cout <<  "                                    deltaCell: " << deltaCell_ << " SI unit" << std::endl;
  std::cout <<  "                       solvusActivationEnergy: " << solvusActivationEnergy_ << " SI unit" << std::endl;
  if (distorsionEnergy_==-1)
  {std::cout <<  "                             distorsionEnergy: Not defined" << std::endl;}
  else
  {std::cout <<  "                             distorsionEnergy: " << distorsionEnergy_ << " SI unit" << std::endl;};
  
  std::cout <<  "                        nucleationSitesNumber: " << nucleationSitesNumber_ << " SI unit" << std::endl;
  std::cout <<  "                          preExpTermForSolvus: " << preExpTermForSolvus_ << " SI unit" << std::endl;
  std::cout <<  "                                  shapeFactor: " << shapeFactor_ << " SI unit" << std::endl;
  std::cout <<  "                              volumicFraction: " << volumicFraction_ << " SI unit" << std::endl;
  std::cout <<  "                                  molarVolume: " << molarVolume_  << " SI unit" << std::endl;
  if (meanDiameter_==-1)
  {std::cout <<  "                                 meanDiameter: Not defined" << std::endl;}
  else
  {std::cout <<  "                                 meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;};
  
  
  //std::cout <<  "                                  material_: " << material_ << " SI unit" << std::endl;
  if (surfaceEnergyPolynomial_.GetDegree()==-1)
  {
  std::cout <<  "                      surfaceEnergyPolynomial: Not defined" << std::endl;
  }
  else
  {
  std::cout <<  "                      surfaceEnergyPolynomial: " ;
  std::cout <<"<"<<surfaceEnergyPolynomial_.GetCoefsList()[0]<<">";
  for (int i=1; i<=surfaceEnergyPolynomial_.GetDegree();i++)
  {
    std::cout <<" + <"<<surfaceEnergyPolynomial_.GetCoefsList()[i]<<">*"<<"T^"<<i;
  }
  std::cout<< " SI unit" << std::endl;
    
  };
  
  std::cout  << "                         precipitate adress: " <<this<<std::endl;
  
  std::cout <<  "                  currentRadiusDistribution: ==>"<< std::endl;
  currentRadiusDistribution_->Info();
  std::cout <<  "=====================================================================================" 	<< std::endl;
  std::cout <<  "#############################  GuinierPreston::Info #################################" 	<< std::endl;
  std::cout <<  "=====================================================================================" 	<< std::endl;
  

  
  std::cout <<  std::endl;
  
}