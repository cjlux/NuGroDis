#include "Computation.hpp"
#include "Hardening.hpp"
#include "ThermalLoading.hpp"
#include "Quenching.hpp"

#include <iostream>
#include <cassert>

Computation::Computation()
  : radiusDistribution_(0),
    quenching_(0),
    hardening_(0),
    thermalLoading_(0),
    maxComputationTime_(0),
    type_("")
{
  std::cout << "Lancement du constructeur <Computation>" << std::endl;

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
