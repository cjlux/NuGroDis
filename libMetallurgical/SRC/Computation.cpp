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

#include <boost/python.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iomanip> //put_time 
#include <ctime>
#include <sys/stat.h> //mkdir


#include "Computation.hpp"
#include "Hardening.hpp"
#include "ThermalLoading.hpp"
#include "Quenching.hpp"
#include "Material.hpp"
#include "Precipitate.hpp"
#include "RadiusDistribution.hpp"

//resultsDirectory_, defineManuallyMaximumAllowedTimeStep_  are OPTIONAL  !!!!
// by default, resultsDirectory_="" ; defineManuallyMaximumAllowedTimeStep_=false ; manualMaximumAllowedTimeStep_=-1  !!!!
Computation::Computation(double initialTimeStep, std::string ResultsDirectory, bool defineManuallyMaximumAllowedTimeStep, double manualMaximumAllowedTimeStep )//
  : radiusDistribution_(0),
    quenching_(0),
    hardening_(0),
    thermalLoading_(0),
    material_(0),
    maxComputationTime_(0),
    type_(""),
    currentTime_(0),
    defaulTimeStep_(initialTimeStep),
    maxTimeStep_(defaulTimeStep_),
    increment_(1),
    manualMaximumAllowedTimeStep_(manualMaximumAllowedTimeStep),
    defineManuallyMaximumAllowedTimeStep_(defineManuallyMaximumAllowedTimeStep)
    
{  
std::cout << "   .-.     .-.     .-.     .-.     .-.     .-.     .-.     .-.     .-.     .-.     .-. " << std::endl;
std::cout << " .'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `." << std::endl;
std::cout << "(    .     .-.     .-.     .-.     .-.     .-.     .-.     .-.     .-.     .-.     .    )" << std::endl;
std::cout << " `.   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   .'" << std::endl;  
std::cout << "   )    )        __        __   _                            _____              (    (" << std::endl;
std::cout << " ,'   ,'         \\ \\      / /__| | ___ ___  _ __ ___   ___  |_   _|__            `.   `." << std::endl;
std::cout << "(    (            \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\   | |/ _ \\              )    )" << std::endl;
std::cout << " `.   `.           \\ V  V /  __/ | (_| (_) | | | | | |  __/   | | (_) |           .'   .' " << std::endl;
std::cout << "   )    )           \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|   |_|\\___/           (    (" << std::endl;
std::cout << " ,'   ,'                                                                          `.   `. " << std::endl;
std::cout << "(    (                _   _        ____           ____  _                          )    )" << std::endl;
std::cout << " `.   `.             | \\ | |_   _ / ___|_ __ ___ |  _ \\(_)___                    .'   .'" << std::endl;
std::cout << "   )    )            |  \\| | | | | |  _| '__/ _ \\| | | | / __|                  (    (" << std::endl;
std::cout << " ,'   ,'             | |\\  | |_| | |_| | | | (_) | |_| | \\__ \\                   `.   `." << std::endl;
std::cout << "(    (               |_| \\_|\\__,_|\\____|_|  \\___/|____/|_|___/                     )    )" << std::endl;
std::cout << " `.   `.                                                                         .'   .' " << std::endl;                                         
std::cout << "   )    )       _       _       _       _       _       _       _       _       (    (" << std::endl;
std::cout << " ,'   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   `." << std::endl;
std::cout << "(    '  _  `-'  _  `-'  _  `-'  _  `-'  _  `-'  _  `-'  _  `-'  _  `-'  _  `-'  _  `    )" << std::endl;
std::cout << " `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .' `.   .'" << std::endl;
std::cout << "   `-'     `-'     `-'     `-'     `-'     `-'     `-'     `-'     `-'     `-'     `-'" << std::endl;
std::cout <<""<<std::endl;
std::cout <<""<<std::endl;
std::cout <<""<<std::endl;


std::cout << "Building Object <Computation>" << std::endl;


  if (ResultsDirectory=="")
  {
    // current date/time based on current system
    std::time_t now = time(0);
    
    char buffer[100];
    // convert now to string form
    //char* dt = std::ctime(&now);
    
    std::strftime(buffer, sizeof(buffer), "%a_%b_%d_%Hhr_%Mmin_%Ss_%Y", std::localtime(&now));
    

    
    resultsDirectory_= buffer;
    
  }
  else
  {
    // current date/time based on current system
    std::time_t now = time(0);
    
    char buffer[100];
    // convert now to string form
    //char* dt = std::ctime(&now);
    
    std::strftime(buffer, sizeof(buffer), "%a_%b_%d_%Hhr_%Mmin_%Ss_%Y", std::localtime(&now));
    
    resultsDirectory_=ResultsDirectory+"__"+buffer;
  }

this->CreateResultsDirectory();


if (defineManuallyMaximumAllowedTimeStep_ ==true)
{
  assert  ((manualMaximumAllowedTimeStep_ > 0)&& "manualMaximumAllowedTimeStep_ has not been defined correctly" );
  
  std::cout<<"Manual definition of time step : A maximum allowed time step has been defined manually and setted to <"<<manualMaximumAllowedTimeStep_<<"> [s] \n "<<std::endl;
}
else
{
  assert (manualMaximumAllowedTimeStep_==-1);
};

}

Computation::~Computation()
{
  
 
  /*DEBUG*/ std::cout<< "resultsDirectory_ "<<resultsDirectory_<<std::endl;
  std::cout << "Destruction d'un objet Computation" << std::endl << std::flush;
  //delete hardening_; //TODO ?
  //delete thermalLoading_; //TODO ?
}

// Définition de la méthdoe ReadDataFile de la classe Computation:
void
Computation::ReadDataFile(std::string fileName)
{
  // ouverture du fichier fileName
  //...
  // lecture...
  //...

  std::cout << "Lancement de la méthode ReadDataFile avec comme fichier : <" << fileName << ">" << std::endl;
}


//The total "Precipitation" computation duration in second [s] = Maturation.duration + ThermalLoading.duration (was tmax)
void   
Computation::ProcessMaxComputationTime() 
{
  double maxCompTime=0;
  
  if (hardening_ != 0)
  {
    maxCompTime += hardening_->GetDuration();
  };
  
  if (thermalLoading_ != 0)
  {
    maxCompTime += thermalLoading_->GetDuration();
  };
  
  maxComputationTime_= maxCompTime;
}




void
Computation::Run()
{
  std::cout << "Youpiiiiiiiiiiii ça commence!!!" << std::endl;

  if (type_ == std::string("Hardening"))
    {
      assert(hardening_ != 0);
      std::string reponse = hardening_->GetInfo();
    }

  if (type_ == std::string("ThermalLoading"))
    {
      assert(thermalLoading_ != 0);
      //
    }
}

void  //TODO à affiner: cas où on est en en fin de process, à la transition entre 2 process. par exemple cas où (tmat-t) < min(deltatP_i)  
Computation::ComputeMaxTimeStep()
{
  assert ( (material_!=0)&&"Computation Object is not linked to any material. You must set the material");
  
  std::vector<Precipitate*> PrecipitateList= material_->GetPrecipitateList();
  
  std::vector<Precipitate*> precipitateReallyFoundInMaterial;
  assert (precipitateReallyFoundInMaterial.size()==0);
  
  
  /////////////////////////////////////////////////////////////
  // TAKING INTO ACCOUNT PRECIPITATE CRITICAL TIME STEP
  for (unsigned int i=0; i<PrecipitateList.size(); ++i )
  { 
   double SumP = PrecipitateList[i]->GetCurrentRadiusDistribution().ReturnTotNbOfItems();
   if (SumP!=0)
   {
     precipitateReallyFoundInMaterial.push_back(PrecipitateList[i]); //push_back adress of precipitate
   };
  }
  
  
  if (precipitateReallyFoundInMaterial.size()==0) //means no precipitate
  {
    maxTimeStep_= defaulTimeStep_;
    std::cout<<"Critical Time step is the default time step. deltat= "<<maxTimeStep_<<std::endl;
  }
  else //means at least one precipitate
  {
    
    std::vector<double> listOfPrecipitatesCriticalTimeStep;
    for (unsigned int i=0; i<precipitateReallyFoundInMaterial.size(); ++i  )
    {
      
      double deltatP= precipitateReallyFoundInMaterial[i]->ReturnCriticalTimeStep();
      listOfPrecipitatesCriticalTimeStep.push_back(deltatP);
    }
    
    std::vector<double>::const_iterator it;
    it = std::min_element(listOfPrecipitatesCriticalTimeStep.begin(), listOfPrecipitatesCriticalTimeStep.end());
    assert ((*it)>0);
    
    maxTimeStep_= *it;
    
    
    std::cout<<"crititical Time step for all precipitates is deltatcritique= <<< "<< maxTimeStep_ <<" >>>"<<std::endl;
    
  };
  // TAKING INTO ACCOUNT PRECIPITATE CRITICAL TIME STEP
  /////////////////////////////////////////////////////////////
  
  
  
  // GESTION OF TIME STEP AT THE END OF COMPUTATION
  if ( (currentTime_+maxTimeStep_) > maxComputationTime_)
  {
    maxTimeStep_=maxComputationTime_-currentTime_;
  };
  
  
  
  
  ////////////////////////////////////////////////////
  // TAKING INTO ACCOUNT THERMALLOADING TIME STEP
  if (thermalLoading_!=0)
  {
    double thermalLoadingMaxTimeStep= thermalLoading_->GetMaximumTimeStep();
    
    if (thermalLoadingMaxTimeStep<maxTimeStep_)
    {
      // thermalLoadingMaxTimeStep is the miniumum, therefore take thermalLoadingMaxTimeStep as the computation time Step
      maxTimeStep_=thermalLoadingMaxTimeStep;
    };
  };
  // TAKING INTO ACCOUNT THERMALLOADING TIME STEP
  ////////////////////////////////////////////////////
  
  
  /*
  ///////////////////////////////////////////////////////////////
  //////// IF MANUAL TIME STEP IS DEFINED, TAKE IT INTO ACCOUNT
  if ( this->CheckIfMaximumAllowedTimeStepHasBeenDefinedManually() == true)
  {
    if (manualMaximumAllowedTimeStep_<maxTimeStep_) 
    { 
      maxTimeStep_=manualMaximumAllowedTimeStep_;
    };
  };
  //////// IF MANUAL TIME STEP IS DEFINED, TAKE IT INTO ACCOUNT
  ///////////////////////////////////////////////////////////////
  */
}


void
Computation::UpdateCurrentTime()
{
  currentTime_+=maxTimeStep_;
  std::cout<<">>>>>>>>>>>>>>>>> Update Current computation time. CurrentTime is now :  "<<currentTime_<<"\n\n"<<std::endl;
}


void
Computation::CreateResultsDirectory()
{  
  
  
  mkdir(resultsDirectory_.c_str(), 0777);
  mkdir((resultsDirectory_+"/RadDisFiles").c_str(),0777); //
  mkdir((resultsDirectory_+"/PrecipitatesAttributes").c_str(),0777);
  mkdir((resultsDirectory_+"/MaterialCurrentCompo").c_str(),0777);
  mkdir((resultsDirectory_+"/PrecipitatesInterfacialProperties").c_str(),0777);
   
   
}


void 
Computation::CreateDirectory(std::string relativePathOfTheWantedFolder)
{
  mkdir((resultsDirectory_+"/"+relativePathOfTheWantedFolder).c_str(),0777); // 
}


void 
Computation::Info() const
{
  std::cout <<  "##############################  Computation::Info #################################" << std::endl;
  std::cout <<  "                                maxTimeStep: " << maxTimeStep_<< " SI unit" << std::endl;
}






