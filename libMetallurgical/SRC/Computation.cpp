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

#include "Computation.hpp"
#include "Hardening.hpp"
#include "ThermalLoading.hpp"
#include "Quenching.hpp"

Computation::Computation()
  : radiusDistribution_(0),
    quenching_(0),
    hardening_(0),
    thermalLoading_(0),
    maxComputationTime_(0),
    type_("")
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
  delete hardening_;
  delete thermalLoading_;
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




