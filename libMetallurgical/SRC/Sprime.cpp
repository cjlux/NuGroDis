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

#include "Sprime.hpp"
#include "RadiusDistribution.hpp"
#include "Material.hpp"
#include "ChemicalElement.hpp"

Sprime::Sprime(Material& mat,ChemicalComposition &CC, RadiusDistribution &initialRD,double wetA)
  :Precipitate(mat,CC,initialRD),
   wettingAngle_(wetA),
   Stheta_(-1),
   precipitateType_("Sprime")
{  
}

Sprime::~Sprime()
{
}


double //TODO
Sprime::ReturnCriticalTotalEnergy()
{
    //Assert DeltaGv and DeltaGe have been computed before, and correct (current) values are used
  
  double oldDeltaGv=phaseChangeVolumiqueEnergy_;
  double oldDeltaGe=distorsionEnergy_;
  double oldGamma=surfaceEnergyCurrentValue_;
  double oldStheta=Stheta_;
  this->ComputePhaseChangeVolumicEnergy();//compute another value of phaseChangeVolumiqueEnergy_
  this->ComputeDistorsionEnergy(); //compute another value of distorsionEnergy_
  this->ComputeSurfaceEnergy();
  this->WettingFunction(wettingAngle_);
  assert ( (phaseChangeVolumiqueEnergy_==oldDeltaGv)&&"may be phaseChangeVolumiqueEnergy_ has not been computed before\
  running method ReturnCriticalRadius()");
  assert ( (distorsionEnergy_==oldDeltaGe)&&"may be distorsionEnergy_ has not been computed before\
  running method ReturnCriticalRadius()");
  assert ( (surfaceEnergyCurrentValue_==oldGamma)&&"may be surfaceEnergyCurrentValue_ has not been computed before\
  running method ReturnCriticalRadius()");
  assert ( (Stheta_==oldStheta)&&"may be Stheta_ has not been computed before\
  running method ReturnCriticalRadius()");
  
  
  assert((phaseChangeVolumiqueEnergy_+distorsionEnergy_)!=0);
  
  double criticalDeltaG= Stheta_*32./27.*M_PI*std::pow(surfaceEnergyCurrentValue_,3.)/std::pow( (phaseChangeVolumiqueEnergy_+distorsionEnergy_),2.)*std::pow((shapeFactor_+2.),3.)/std::pow((shapeFactor_+4./3.),2.); 
  
  assert (criticalDeltaG>0);
  
  return criticalDeltaG;
}


void
Sprime::ComputeCriticalTotalEnergy()
{
  criticalTotalEnergy_=this->ReturnCriticalTotalEnergy();
}


void
Sprime::ComputeNucleationSiteNb()
{
  std::cout<<"Computing Nucleation Site Number For Sprime Precipitate"<<std::endl;
  
  std::vector<Precipitate *> precipitateList = materialPointer_->GetPrecipitateList();
  //TODO  
  std::vector<Precipitate *> SprimeList;//List of Sprime
  std::vector<Precipitate *> GPList;//List of GuinierPreston
  
  for ( unsigned int i=0; i<precipitateList.size(); ++i  )
  {
    std::string type= precipitateList[i]->GetPrecipitateType();
    if (type=="GuinierPreston")
    {
      GPList.push_back(precipitateList[i]) ;
    } 
    if (type=="Sprime")
    {
      SprimeList.push_back(precipitateList[i]);
    }
  }
  
  double SumOfFracOfAllGP=0;
  double SumOfFracOfAllSprime=0;
  double am = materialPointer_->GetMainChemicalElement().GetLatticeParameter();
  
  for (unsigned int i=0; i<GPList.size(); ++i )
  {
    
    double fracVol= GPList[i]->ReturnVolumicFraction();
    
    double NsGP_i=  (4/std::pow(am,3))*fracVol;
    
    assert (NsGP_i>=0);
    
    SumOfFracOfAllGP += NsGP_i;

  }
  
  for (unsigned int i=0; i<SprimeList.size(); ++i )
  {
    
    double fracVol= SprimeList[i]->ReturnVolumicFraction();
    
    double NsSprime_i=  (4/std::pow(am,3))*fracVol;
    
    assert (NsSprime_i>=0);
    
    SumOfFracOfAllSprime += NsSprime_i;
  }
  
  nucleationSitesNumber_=SumOfFracOfAllGP-SumOfFracOfAllSprime;
  
  if( nucleationSitesNumber_<0)
  {
    std::cout<<"Nuclueation site Number for precipitate type <Sprime> at adress <"<<this<<"> is negative => IT WILL BE SETTED TO ZERO";
    nucleationSitesNumber_=0;
  }
  
  assert(nucleationSitesNumber_>=0);
  /*DEBUG*/ std::cout<<"ççççççççççççççççççççççççççççççççççççççççççççç Nucleation sites number: "<<nucleationSitesNumber_<<std::endl;

}


double 
Sprime::ReturnVolumicFraction()
{
  double n=currentRadiusDistribution_->GetItemsValues().size();
  
  assert (n>0);
  
  double Sum=0;
  for (unsigned int i=1; i<=n; ++i)
  {
    double sum_i;
    double Ri=currentRadiusDistribution_->GetRadiusForClass(i);
    double Ni=currentRadiusDistribution_->GetItemValueForClass(i);
    sum_i= (4/3 + shapeFactor_)*M_PI*Ni*std::pow(Ri,3);
    assert (sum_i >= 0);
    Sum +=sum_i;
  }
  
  assert(Sum>=0);
  
  return Sum;
}


void
Sprime::ComputeVolumicFraction()
{  
  volumicFraction_=this->ReturnVolumicFraction(); 
}

void
Sprime::ReadDataValue(std::string dataFile)
{
}

void
Sprime::Info() const
{
  std::cout <<  "###################################  Sprime::Info #################################" 	<< std::endl;
  std::cout <<  "                                molarVolume: " << molarVolume_  << " SI unit" << std::endl;
  
  
  std::cout <<  "                               wettingAngle: " << wettingAngle_ << " SI unit" << std::endl;  
  //std::cout <<  "                                material_: " << material_ << " SI unit" << std::endl;
  std::cout <<  "                                  deltaCell: " << deltaCell_ << " SI unit" << std::endl;
  std::cout <<  "                     solvusActivationEnergy: " << solvusActivationEnergy_ << " SI unit" << std::endl;
  
  //DistorsionEnergy
  if (distorsionEnergy_==-1)
  {std::cout << "                           distorsionEnergy: Not defined" << std::endl;}
  else
  {std::cout << "                           distorsionEnergy: " << distorsionEnergy_ << " SI unit" << std::endl;};  
   
  std::cout <<  "                        preExpTermForSolvus: " << preExpTermForSolvus_ << " SI unit" << std::endl;
  
  std::cout <<  "                                shapeFactor: " << shapeFactor_ << " SI unit" << std::endl;
  std::cout <<  "                            volumicFraction: " << volumicFraction_ << " SI unit" << std::endl;
  
  //MeanDiameter
  if (meanDiameter_==-1)
  {std::cout << "                               meanDiameter: Not defined" << std::endl;}
  else
  {std::cout << "                               meanDiameter: " << meanDiameter_ << " SI unit" << std::endl;};
  
  //SurfaceEnergyPolynomial
  if (surfaceEnergyPolynomial_.GetDegree()==-1)
  {
  std::cout <<  "                    surfaceEnergyPolynomial: Not defined" << std::endl;
  }
  else
  {
  std::cout <<  "                    surfaceEnergyPolynomial: " ;
  std::cout <<"<"<<surfaceEnergyPolynomial_.GetCoefsList()[0]<<">";
  for (int i=1; i<=surfaceEnergyPolynomial_.GetDegree();i++)
  {
    std::cout <<" + <"<<surfaceEnergyPolynomial_.GetCoefsList()[i]<<">*"<<"T^"<<i;
  }
  std::cout<< " SI unit" << std::endl;
  };
  
  //Stheta
  if (Stheta_==-1)
  {std::cout << "                                     Stheta: Not computed yet" << std::endl;}
  else
  {std::cout << "                                     Stheta: " << Stheta_ << " SI unit" << std::endl;};
  
  //Nucleation site Nb
  if (nucleationSitesNumber_==-1)
  {std::cout << "                      nucleationSitesNumber: Not defined"<<std::endl;}
  else
  {std::cout << "                      nucleationSitesNumber: " << nucleationSitesNumber_ << " SI unit" << std::endl;};
  
  std::cout  << "                         precipitate adress: " <<this<<std::endl;
  std::cout <<  std::endl;
  
}

