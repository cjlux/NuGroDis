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
    defineManuallyMaximumAllowedTimeStep_(defineManuallyMaximumAllowedTimeStep),
    computationSequence_(),
    sequenceKeyTimeVector_(),
    sequenceTypeRankMap_(),
    sequenceKeyTimeVectorHasBeenProcessed_(false)
    
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
  assert (manualMaximumAllowedTimeStep_==-1); // -1 is the value that we choose to represent a non defined manualMaximumAllowedTimeStep_; by default is is -1. See header file
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
    
  
  // process all sequences starting and ending absolute time
  this->ProcessSequenceKeyTimeVector();  
  
  
  assert(material_!=0);
  material_->RunProcess();
    
  
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
   
  
  
  ////////////////////////////////////////////////////
  // TAKING INTO ACCOUNT THERMALLOADING TIME STEP
  if (thermalLoading_!=0)
  {
    
    std::string type= thermalLoading_->GetType();
    double t_initial=GetAbsoluteInitialTimeForSequenceType(type);
    
    double t_final=GetAbsoluteFinalTimeForSequenceType(type);
    
    if ( (currentTime_>=t_initial) && (currentTime_<=t_final) )
    {
      double thermalLoadingMaxTimeStep= thermalLoading_->GetMaximumTimeStep();
    
      if (thermalLoadingMaxTimeStep<maxTimeStep_)
      {
	// thermalLoadingMaxTimeStep is the miniumum, therefore take thermalLoadingMaxTimeStep as the computation time Step
	maxTimeStep_=thermalLoadingMaxTimeStep;
      };
      
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
  
  
  
  double t_final_currentSequenceType=  this->GetAbsoluteFinalTimeForSequenceType( this->GetCurrentSequenceType() ) ;
  
  // GESTION OF TIME STEP AT THE END OF COMPUTATION
  if ( (currentTime_+maxTimeStep_) > t_final_currentSequenceType)
  {
    maxTimeStep_=t_final_currentSequenceType-currentTime_;
  };
  
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
Computation::PrintComputationSequenceInfo()
{
  
  assert (this->CheckIfSequenceKeyTimeVectorHasBeenProcessed()==true);
  
  // printing info
  std::cout<< "Printing computation sequence informations \n\n";
  
  for (unsigned int i=0; i<computationSequence_.size(); ++i)
  {
    
    std::string sequenceType= computationSequence_[i];
    
    this->AssertIfGivenTypeIsAdmitted(sequenceType);
    
    double t_initial, t_final;
    
    t_initial=this->GetAbsoluteInitialTimeForSequenceType(sequenceType);
    
    t_final= this->GetAbsoluteFinalTimeForSequenceType(sequenceType);
    
    std::cout<< "\n";
    std::cout<< "---------------------------------------------------"<<std::endl;
    std::cout<< "  sequence type : '"<<sequenceType<<"'"<<std::endl;
    std::cout<< "    starting at :  "<<t_initial<<" [s] "<<std::endl;
    std::cout<< "      ending at :  "<<t_final<<" [s] "<<std::endl;
    std::cout<< ""<<std::endl;
    
  }
  
  
}



double 
Computation::GetAbsoluteInitialTimeForSequenceType(std::string givenSequenceType)
{
  this->AssertIfGivenTypeIsAdmitted(givenSequenceType);
  
  assert ((this->CheckIfSequenceKeyTimeVectorHasBeenProcessed()==true)&&"sequence key time vector (sequenceKeyTimeVector_) has not been processed yet !");
 
  unsigned int rank= GetRankForSequenceType(givenSequenceType);
  
  
  assert (( rank>=1 ) && (rank <= 3 ) );
 
  double absoluteInitialTime= sequenceKeyTimeVector_[rank-1];
  
  return absoluteInitialTime;
  
}


double 
Computation::GetAbsoluteFinalTimeForSequenceType(std::string givenSequenceType)
{
  this->AssertIfGivenTypeIsAdmitted(givenSequenceType);
  
  assert ((this->CheckIfSequenceKeyTimeVectorHasBeenProcessed()==true)&&"sequence key time vector (sequenceKeyTimeVector_) has not been processed yet !");
  
  unsigned int rank= GetRankForSequenceType(givenSequenceType);
  
  
  assert (( rank>=1 ) && (rank <= 3 ) );
 
  double absoluteFinalTime= sequenceKeyTimeVector_[rank];
  
  return absoluteFinalTime;
}


void 
Computation::ProcessSequenceKeyTimeVector()
{
  
  std::cout<<"=================================================================================================================="<<std::endl;
  std::cout<<"               Processing the key time of each sequence of the computation (t_initial and t_final)                "<<std::endl;
  std::cout<<"=================================================================================================================="<<std::endl;
  
  assert ((this->CheckIfSequenceKeyTimeVectorHasBeenProcessed()==false)&& (sequenceKeyTimeVector_.size()==0) &&"sequence key time vector has already been processed");

  assert ( (computationSequence_.size() <=3 )&&"Something is wrong! computationSequence_ cannot have more than 3 items.");
  
  assert ( (computationSequence_.size() >0 )&&"Something is wrong! computationSequence_ is empty. No computation type has been defined !!!");
  
  //////////////////
  /// defining sequenceKeyTimeVector_, which is a vector where initial time of computation
  ///  and final time (t_final) of each sequenceType in the computation sequence are saved;
  //////////////////
  std::vector<double> sequenceDurationVector;
 
  for (unsigned int i=0; i<computationSequence_.size(); ++i)
  {
    std::string sequenceType = computationSequence_[i];
    
    if (sequenceType=="Hardening")
    {
      assert(hardening_ != 0);
      
      assert((hardening_->GetDuration()!=0)&&"hardening duration is defined but its duration is zero");
      sequenceDurationVector.push_back(hardening_->GetDuration());
    }
    else if (sequenceType=="ThermalLoading")
    {
      assert(thermalLoading_ != 0);
      
      assert((thermalLoading_->GetDuration()!=0)&&"hardening ThermalLoading is defined but its duration is zero");
      
      sequenceDurationVector.push_back(thermalLoading_->GetDuration());
    }
    else if (sequenceType=="Quenching")
    {
      assert(quenching_ != 0);
      
      assert((quenching_->GetDuration()!=0)&&"Quenching duration is defined but its duration is zero");
      
      sequenceDurationVector.push_back(quenching_->GetDuration());
    }
    else
    {
      assert(!"a given sequence key type for computation is not 'Hardening' or 'ThermalLoading' or 'Quenching' ");
    };
  }
  
  
  assert(currentTime_==0);
  assert (sequenceKeyTimeVector_.size()==0);
  sequenceKeyTimeVector_.push_back(0);
  for (unsigned int i=0; i<sequenceDurationVector.size(); ++i)
  {
    assert (sequenceKeyTimeVector_.size()== i+1 );
    
    double t_final_of_sequence= sequenceKeyTimeVector_[i] + sequenceDurationVector[i];
    
    sequenceKeyTimeVector_.push_back(t_final_of_sequence);
  }
  
  
  sequenceKeyTimeVectorHasBeenProcessed_=true;
  
  //////////////////
  /// defining sequenceKeyTimeVector_, which is a vector where initial time of computation
  ///   and final time (t_final) of each sequenceType in the computation sequence are saved;
  //////////////////
  
  assert ((this->CheckIfSequenceKeyTimeVectorHasBeenProcessed()==true)&&"sequenceKeyTimeVectorHasBeenProcessed_ should be 'true' after or at the end of method ProcessSequenceKeyTimeVector() ");
  
  
  //print sequence info
  this->PrintComputationSequenceInfo();
  
  std::cout<<"=================================================================================================================="<<std::endl;
  std::cout<<"               Processing the key time of each sequence of the computation (t_initial and t_final)                "<<std::endl;
  std::cout<<"=================================================================================================================="<<std::endl;
}



unsigned int 
Computation::GetRankForSequenceType(std::string sequenceType)
{
  //print if you want some info
  //std::cout<<"Getting rank for a given sequence type"<<std::end;
  
  this->AssertIfGivenTypeIsAdmitted(sequenceType);
  
  std::map<std::string , unsigned int >::iterator it;
  it=sequenceTypeRankMap_.find(sequenceType);
  assert( (it!=sequenceTypeRankMap_.end())&&"Cannot find given 'sequenceType' in sequenceTypeRankMap_ " );
  
  unsigned int searchRank= it->second;
  
  assert((searchRank>=1)&&(searchRank<=3)&&"Something is wrong: sequence rank should be 1, 2 or 3");
  
  //print if you want some info
  //std::cout<<"Rank for sequence type '"<<sequenceType<<"' is : ["<< searchRank <<"] "<<std::endl;
  
  return searchRank;
}






void 
Computation::Info() const
{
  std::cout <<  "##############################  Computation::Info #################################" << std::endl;
  std::cout <<  "                                maxTimeStep: " << maxTimeStep_<< " SI unit" << std::endl;
}






