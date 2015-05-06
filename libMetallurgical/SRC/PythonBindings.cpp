#include <boost/python.hpp>

#include <string>
#include "ChemicalComposition.hpp"
#include "ChemicalElement.hpp"
#include "Concentration.hpp"
#include "Computation.hpp"
#include "Diffusion.hpp"
#include "Grain.hpp"
#include "GuinierPreston.hpp"
#include "Hardening.hpp"
#include "Material.hpp"
#include "Precipitate.hpp"
#include "Quenching.hpp"
#include "RadiusDistribution.hpp"
#include "Sprime.hpp"
#include "SSGrain.hpp"
#include "Temperature.hpp"
#include "ThermalLoading.hpp"
#include "ThermoDynamicsConstant.hpp"
#include "Vacancy.hpp"




// Python Wrapper
BOOST_PYTHON_MODULE(Metallurgical)
{
 
  //TODO class Boundary Binding !!!!
  
  

  //TODO some stuff 
  boost::python::class_<ChemicalComposition>("ChemicalComposition", boost::python::init<std::string, Grain& >() )
    .def(boost::python::init<std::string >())
    .def(boost::python::init<std::string, Material& >())
    .def(boost::python::init<std::string, Material&, SSGrain& >())
    .add_property("formula", &ChemicalComposition::GetFormula)
    //.add_property("material", &ChemicalComposition::GetMaterial)
    //.add_property("grain", &ChemicalComposition::GetGrain)
    .def("Info", &ChemicalComposition::Info)
    ;    
    
    //TODO some stuff: function EnterInChemicalComposition: deal with arguments?
  boost::python::class_<ChemicalElement>("ChemicalElement", boost::python::init<double, double, double, double, std::string >() )
    .add_property("density", &ChemicalElement::GetDensity)
    .add_property("molarMass", &ChemicalElement::GetMolarMass)
    .add_property("youngModulus", &ChemicalElement::GetYoungModulus)
    .add_property("poissonCoefficient", &ChemicalElement::GetPoissonCoefficient)
    .add_property("latticeParameter", &ChemicalElement::GetLatticeParameter, &ChemicalElement::SetLatticeParameter)
    .add_property("elementName", &ChemicalElement::GetElementName)
    .add_property("chemicalCompositionList", &ChemicalElement::GetChemicalComposition)
    .def("EnterInChemicalComposition", &ChemicalElement::EnterInChemicalComposition)
    .def("Info", &ChemicalElement::Info)
    ;

   
    
    //TODO some stuff
  boost::python::class_<Concentration>("Concentration", boost::python::init<ChemicalElement&, ChemicalComposition& >() )
    //.add_property("ChemicalElement", &Concentration::GetChemicalElement)
    //.add_property("ChemicalComposition", &Concentration::GetChemicalComposition)
    .add_property("volumicValue", &Concentration::GetVolumicValue,&Concentration::SetVolumicValue)
    .add_property("initialAtomicValue", &Concentration::GetInitialAtomicValue,&Concentration::SetInitialAtomicValue)
    .add_property("initialMassicValue", &Concentration::GetInitialMassicValue,&Concentration::SetInitialMassicValue)
    .def("Info", &Concentration::Info)
    ;
  
    
  boost::python::class_<Computation>("Computation", boost::python::init<>() )
    .add_property("type", &Computation::GetType, &Computation::SetType)
    .def("Run", &Computation::Run) 
    ;
    
    
    //TODO some stuff
  boost::python::class_<Diffusion>("Diffusion", boost::python::init<ChemicalElement&, Material&, Vacancy&, double,double, double >() )
    .def(boost::python::init<ChemicalElement&, Material&, double, double >())
    .def(boost::python::init<ChemicalElement&, Vacancy&, double >())
    .add_property("preExpDiffusionCoef", &Diffusion::GetPreExpDiffusionCoef)
    .add_property("activationEnergy", &Diffusion::GetActivationEnergy)
    .add_property("interactionEnergyWithVacancy", &Diffusion::GetInteractionEnergyWithVacancy)
    .def("Info", &Diffusion::Info)
    ;  
    
   
  //TODO !!! check no_init correct ?
  boost::python::class_<Grain,boost::noncopyable >("Grain", boost::python::no_init /*<Material&, ChemicalComposition&>()*/ )
    .def("Info", &Grain::Info)
    ;
    
    
      boost::python::class_<SSGrain, boost::python::bases<Grain> >("SSGrain", boost::python::init<Material&,boost::python::optional<int, int> >() )
    .def(boost::python::init<Material&, ChemicalComposition&, double, double, double,boost::python::optional<int, int> >())  
    .add_property("YoungModulus", &SSGrain::GetYoungModulus)
    .add_property("PoissonCoeff", &SSGrain::GetPoissonCoeff)
    .add_property("latticeParameter", &SSGrain::GetLatticeParameter)
    .add_property("volNbGP", &SSGrain::GetVolNbGP, &SSGrain::SetVolNbGP)
    .add_property("volNbSprime", &SSGrain::GetVolNbSprime, &SSGrain::SetVolNbSprime)
    .add_property("volNbPrecipitates", &SSGrain::GetVolNbPrecipitates, &SSGrain::SetVolNbPrecipitates)
    .def("Info", &SSGrain::Info)
    ;
    
        //TODO !!! check no_init? inherithance of Grain correct? property initialRadiusDistribution?
  boost::python::class_<Precipitate, boost::python::bases<Grain>, boost::noncopyable >("Precipitate", boost::python::no_init /*<Material&, ChemicalComposition&, RadiusDistribution& >()*/ )
    .add_property("deltaCell", &Precipitate::GetDeltaCell,&Precipitate::SetDeltaCell)
    .add_property("solvusActivationEnergy", &Precipitate::GetSolvusActivationEnergy, &Precipitate::SetSolvusActivationEnergy)
    .add_property("distorsionEnergy", &Precipitate::GetDistorsionEnergy,&Precipitate::SetDistorsionEnergy)
    .add_property("nucleationSitesNumber", &Precipitate::GetNucleationSitesNumber,&Precipitate::SetNucleationSitesNumber)
    .add_property("preExpTermForSolvus", &Precipitate::GetPreExpTermForSolvus,&Precipitate::SetPreExpTermForSolvus)
    .add_property("shapeFactor", &Precipitate::GetShapeFactor,&Precipitate::SetShapeFactor)
    .add_property("volumicFraction", &Precipitate::GetVolumicFraction,&Precipitate::SetVolumicFraction)
    //.add_property("initialRadiusDistribution", &Precipitate::GetInitialRadiusDistribution)
    .def("Info", &Precipitate::Info)
    ;
    
  //TODO !!!check if inherithance wrapping is correct --> boost::python::bases<Grain> ? 
  boost::python::class_<GuinierPreston,boost::python::bases<Precipitate> >("GuinierPreston", boost::python::init<Material&, ChemicalComposition&, RadiusDistribution& >() )
    .def("Info", &GuinierPreston::Info)
    ;
    
  boost::python::class_<Sprime,boost::python::bases<Precipitate> >("Sprime", boost::python::init<Material&, ChemicalComposition&, RadiusDistribution&, double>()  )
    .add_property("wettingAngle", &Sprime::GetWettingAngle)
    .add_property("Stheta", &Sprime::GetStheta)
    .def("Info", &Sprime::Info)
    ;
    
  
  boost::python::class_<Hardening>("Hardening", boost::python::init<double, double >() )
    .add_property("maxTimeStep", &Hardening::GetMaxTimeStep, &Hardening::SetMaxTimeStep)
    .add_property("duration", &Hardening::GetDuration)
    .def("Info", &Hardening::Info)
    ;
    
    
    //TODO class InterfacialConcentration binding
    
    
  //TODO some stuff
  boost::python::class_<Material>("Material", boost::python::init<Temperature&, ChemicalElement&, ChemicalComposition& >() )
    //.add_property("temperature", &Material::GetTemperature,&Material::SetTemperature)
    //.add_property("vacancyList", &Material::GetVacancyList)
    //.add_property("ssgrainList", &Material::GetSSGrain)
    //.add_property("mainChemicalElement", &Material::GetMainChemicalElement)
    //.add_property("InitialChemicalComposition", &Material::GetInitialChemicalComposition)
    //.add_property("precipitatesList", &Material::GetPrecipitateList)
    .add_property("YoungModulus", &Material::GetYoungModulus)
    .add_property("PoissonCoeff", &Material::GetPoissonCoeff)
    .add_property("latticeParameter", &Material::GetLatticeParameter)
    .def("Info", &Material::Info)
    ;
    

    
    //TODO !!! class Polynomial Binding 
    
    
  boost::python::class_<Quenching>("Quenching", boost::python::init<double, double, double, boost::python::optional<double> >() )
    .add_property("CoolingRate", &Quenching::GetCoolingRate)
    .add_property("TotIterationNumber", &Quenching::GetTotIterationNumber,&Quenching::SetTotIterationNumber)
    .add_property("FinalTemp", &Quenching::GetFinalTemp)
    .add_property("duration", &Quenching::GetDuration)
    .add_property("solutionizingTemp", &Quenching::GetSolutionizingTemp)
    .add_property("deltaT", &Quenching::GetDeltaT)
    .add_property("timeStep", &Quenching::GetTimeStep)
    .def("Info", &Quenching::Info)
    ;
    
    
    //TODO some stuff
  boost::python::class_<RadiusDistribution>("RadiusDistribution", boost::python::init<double, double, double >() )
    .add_property("spatialStep", &RadiusDistribution::GetSpatialStep)
    .add_property("minRadius", &RadiusDistribution::GetMinRadius)
    .add_property("initialClassNumber", &RadiusDistribution::GetInitialClassNumber)
    //.add_property("itemsValues", &RadiusDistribution::GetItemsValues, &RadiusDistribution::SetAllItemsValues) // itemsValues is a pointer PROBLEM
    .def("Info", &RadiusDistribution::Info)
    ;
      
    
    
     //TODO some stuff
  boost::python::class_<Temperature>("Temperature", boost::python::init< boost::python::optional<double> >() )
    .add_property("currentTemp", &Temperature::GetCurrentTemp, &Temperature::SetCurrentTemp)
    //.add_property("temperatureList", &Temperature::GetTemperatureList)
    .def("Info", &Temperature::Info)
    ;
    
    //TODO some stuff
  boost::python::class_<ThermalLoading>("ThermalLoading", boost::python::init<>() )
    .add_property("duration", &ThermalLoading::GetDuration, &ThermalLoading::SetDuration)
    //.add_property("time_", &ThermalLoading::GetElementName)
    //.add_property("temperature_", &ThermalLoading::GetElementName)
    .def("Info", &ThermalLoading::Info)
    ;
      
    
  boost::python::class_<ThermoDynamicsConstant>("ThermoDynamicsConstant", boost::python::init<double, double, double>() )
    .add_property("R",  &ThermoDynamicsConstant::GetR)
    .add_property("kB", &ThermoDynamicsConstant::Get_kB)
    .add_property("Na", &ThermoDynamicsConstant::GetNa)
    .def("Info", &ThermoDynamicsConstant::Info)
    ;    

  boost::python::class_<Vacancy>("Vacancy", boost::python::init<double, double, double, double, double, double, double, Material& >() )
    .add_property("vacCreationEnthalpy", &Vacancy::GetVacCreationEnthalpy, &Vacancy::SetVacCreationEnthalpy)
    .add_property("vacCreationEntropy", &Vacancy::GetVacCreationEntropy, &Vacancy::SetVacCreationEntropy)
    .add_property("migrationEnthalpy", &Vacancy::GetMigrationEnthalpy, &Vacancy::SetMigrationEnthalpy)
    .add_property("halfSinkDistance", &Vacancy::GetHalfSinkDistance, &Vacancy::SetHalfSinkDistance)
    .add_property("jumpFrequency", &Vacancy::GetJumpFrequency, &Vacancy::SetJumpFrequency)
    .add_property("preExpDiffusionValue", &Vacancy::GetPreExpDiffusionValue, &Vacancy::SetPreExpDiffusionValue)
    .add_property("solutionisingTemp", &Vacancy::GetSolutionisingTemp, &Vacancy::SetSolutionisingTemp)
    .def("Info", &Vacancy::Info)
    ;

}


                                     //HOW TO SECTION
  //HOw to add Constructor 
  //For class SSGrain as exemple: 
  //.def(boost::python::init<Material&, ChemicalComposition&, double, double, double,boost::python::optional<int>,boost::python::optional<int> >())
  
  //how to make abstract class?
  //class_< Abstract , boost::noncopyable>("Abstract", no_init)
    
  //How to make inheritance
  // class_<Derived, bases<Base> >("Derived")
