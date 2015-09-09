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


#include "Computation.hpp"
#include "Hardening.hpp"
#include "ThermalLoading.hpp"
#include "Quenching.hpp"
#include "Material.hpp"
#include "Precipitate.hpp"
#include "RadiusDistribution.hpp"

Computation::Computation(double initialTimeStep)
  : radiusDistribution_(0),
    quenching_(0),
    hardening_(0),
    thermalLoading_(0),
    material_(0),
    maxComputationTime_(0),
    type_(""),
    currentTime_(0),
    defaulTimeStep_(initialTimeStep),
    maxTimeStep_(defaulTimeStep_)
    
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


}

Computation::~Computation()
{
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
    std::cout<<"crtitical Time step for all precipitates is deltatcritique= <<< "<< maxTimeStep_ <<" >>>"<<std::endl;  
  };
  
}


void 
Computation::Info() const
{
  std::cout <<  "##############################  Computation::Info #################################" << std::endl;
  std::cout <<  "                                maxTimeStep: " << maxTimeStep_<< " SI unit" << std::endl;
}






