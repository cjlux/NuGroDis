#include <boost/python.hpp>

#include <string>
#include "Boundary.hpp"
#include "ChemicalComposition.hpp"
#include "ChemicalElement.hpp"
#include "Concentration.hpp"
#include "Computation.hpp"
#include "Diffusion.hpp"
#include "Grain.hpp"
#include "GuinierPreston.hpp"
#include "Hardening.hpp"
#include "Material.hpp"
#include "Polynomial.hpp"
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
  
  boost::python::class_<Boundary>("Boundary", boost::python::init< >() )
    //.def("vectorToPyList", &Boundary::vectorToPyList, return_value_policy<manage_new_object>() )
    //.staticmethod("vectorToPyList")
    ;
  

  //TODO some stuff 
  boost::python::class_<ChemicalComposition>("ChemicalComposition", boost::python::init<std::string, Grain& >() )
    .def(boost::python::init<std::string >())
    .def(boost::python::init<std::string, Material& >())
    .def(boost::python::init<std::string, Material&, SSGrain& >())
    .add_property("formula", &ChemicalComposition::GetFormula)
    .add_property("mainElementName", &ChemicalComposition::GetMainElementName, &ChemicalComposition::SetMainElementName )
    .def("GetMaterial", &ChemicalComposition::GetMaterial,boost::python::return_internal_reference<>() )
    .def("GetGrain", &ChemicalComposition::GetGrain,boost::python::return_internal_reference<>() )
    .def("GetMaterialPointer", &ChemicalComposition::GetMaterialPointer,boost::python::return_internal_reference<>() )
    .def("GetGrainPointer", &ChemicalComposition::GetGrainPointer,boost::python::return_internal_reference<>() )
    .def("GetChemicalElementsPyList", &ChemicalComposition::GetChemicalElementsPyList )
    .def("GetConcentrationPyDict",&ChemicalComposition::GetConcentrationPyDict, boost::python::return_internal_reference<>() )
    .def("GetConcentrationForElement",&ChemicalComposition::GetConcentrationForElement,boost::python::return_internal_reference<>() )
    //.def("GetConcentrationMap",&ChemicalComposition::GetConcentrationMap, boost::python::return_internal_reference<>() )
     //.def("GetChemicalElementsList", &ChemicalComposition::GetChemicalElementsList, boost::python::return_internal_reference<>() )
    //.def("GetConcentrationPyList", &ChemicalComposition::GetChemicalElementsPyList )
    .def("Clone", &ChemicalComposition::Clone, boost::python::return_value_policy<boost::python::manage_new_object>() )
    .def("test", &ChemicalComposition::test)
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
    .def("chemicalCompositionList", &ChemicalElement::GetChemicalComposition )
    .def("EnterInChemicalComposition", &ChemicalElement::EnterInChemicalComposition)
    .def("GetDiffusion",&ChemicalElement::GetDiffusion, boost::python::return_internal_reference<>() /*, boost::python::return_value_policy<boost::python::manage_new_object>()*/ )
    .def("Info", &ChemicalElement::Info)
    ;

   
    
    //TODO some stuff
  boost::python::class_<Concentration>("Concentration", boost::python::init<ChemicalElement&, ChemicalComposition& >() )
    
    //.add_property("ChemicalComposition", &Concentration::GetChemicalComposition)
    .add_property("volumicValue", &Concentration::GetVolumicValue,&Concentration::SetVolumicValue)
    .add_property("initialAtomicValue", &Concentration::GetInitialAtomicValue,&Concentration::SetInitialAtomicValue)
    .add_property("initialMassicValue", &Concentration::GetInitialMassicValue,&Concentration::SetInitialMassicValue)
    .add_property("stoichiometricCoef", &Concentration::GetStoichiometricCoef,&Concentration::SetStoichiometricCoef)
    .def("GetChemicalElement", &Concentration::GetChemicalElement,boost::python::return_internal_reference<>())
    .def("Info", &Concentration::Info)
    ;
  
    
  boost::python::class_<Computation>("Computation", boost::python::init<double, boost::python::optional<std::string> >() )
    .add_property("type", &Computation::GetType, &Computation::SetType)
    .add_property("maxComputationTime", &Computation::GetMaxComputationTime, &Computation::SetMaxComputationTime)
    .def("Run", &Computation::Run) 
    .def("Info", &Computation::Info)
    ;
    
    
    //TODO some stuff
  boost::python::class_<Diffusion>("Diffusion", boost::python::init<ChemicalElement&, Material&, double,double, double >() )
    .def(boost::python::init<ChemicalElement&, Material&, double, double >())
    //.def(boost::python::init<ChemicalElement&, Vacancy&, double >())
    .add_property("preExpDiffusionCoef", &Diffusion::GetPreExpDiffusionCoef)
    .add_property("activationEnergy", &Diffusion::GetActivationEnergy)
    .add_property("interactionEnergyWithVacancy", &Diffusion::GetInteractionEnergyWithVacancy)
    .add_property("toto", &Diffusion::GetToto,&Diffusion::SetToto)
    .def("ComputeAtomicDiffusionCoefValue", &Diffusion::ComputeAtomicDiffusionCoefValue)
    .def("Info", &Diffusion::Info)
    ;  
    
    
    
  /* struct GrainWrap : Grain , boost::python::wrapper<Grain>
   {
    void ConvertMassicToVolumicConcentration()
    {
        this->get_override("ConvertMassicToVolumicConcentration")();
    }
    
    void ConvertVolumicToMassicConcentration()
    {
      this->get_override("ConvertVolumicToMassicConcentration")();
    }
    
    void ConvertAtomicToVolumicConcentration()
    {
      this->get_override("ConvertAtomicToVolumicConcentration")();
    }
    
    void ConvertVolumicToAtomicConcentration()
    {
      this->get_override("ConvertVolumicToAtomicConcentration")();
    }
    
    void ConvertStoichiometricCoefficientToVolumicConcentration()
    {
      this->get_override("ConvertStoichiometricCoefficientToVolumicConcentration")();
    }
    
    void ConvertStoichiometricCoefficientToAtomicConcentration()
    {
      this->get_override("ConvertStoichiometricCoefficientToAtomicConcentration")();
    }
    
   int GetVolNbPrecipitates()
    {
      return this->get_override("GetVolNbPrecipitates")();
    }
    
   };
    
    

    
  boost::python::class_<GrainWrap, boost::noncopyable>("Grain")
    .def("ConvertMassicToVolumicConcentration", boost::python::pure_virtual(&Grain::ConvertMassicToVolumicConcentration))
    .def("ConvertVolumicToMassicConcentration", boost::python::pure_virtual(&Grain::ConvertVolumicToMassicConcentration))
    .def("ConvertAtomicToVolumicConcentration", boost::python::pure_virtual(&Grain::ConvertAtomicToVolumicConcentration))
    .def("ConvertVolumicToAtomicConcentration", boost::python::pure_virtual(&Grain::ConvertVolumicToAtomicConcentration))
    .def("ConvertStoichiometricCoefficientToVolumicConcentration", boost::python::pure_virtual(&Grain::ConvertStoichiometricCoefficientToVolumicConcentration))
    .def("ConvertStoichiometricCoefficientToAtomicConcentration", boost::python::pure_virtual(&Grain::ConvertStoichiometricCoefficientToAtomicConcentration))
    
    .def("GetVolNbPrecipitates", boost::python::pure_virtual(&Grain::GetVolNbPrecipitates))
    //.add_property("molarVolume", &Grain::GetMolarVolume,&Grain::SetMolarVolume)
    //.add_property("meanDiameter", &Grain::GetMeanDiameter,&Grain::SetMeanDiameter)
    //.def("GetChemicalComposition", &Grain::GetChemicalComposition , boost::python::return_internal_reference<>() )
    //.def("Info", &Grain::Info)
    ;*/
    

    
//TODO !!! check no_init correct ?
  boost::python::class_<Grain,boost::noncopyable >("Grain", boost::python::no_init /*<Material&, ChemicalComposition&>()*/ )
  //boost::python::class_<Grain,boost::noncopyable >("Grain", boost::python::init<Material&, ChemicalComposition&>() )
    .add_property("molarVolume", &Grain::GetMolarVolume,&Grain::SetMolarVolume)
    .add_property("meanDiameter", &Grain::GetMeanDiameter,&Grain::SetMeanDiameter)
    .def("GetChemicalComposition", &Grain::GetChemicalComposition  , boost::python::return_internal_reference<>() )
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
    .def("ConvertVolumicToAtomicConcentration", &SSGrain::ConvertVolumicToAtomicConcentration)
   // .def("GetPrecipitateList", &SSGrain::GetPrecipitateList)
    .def("test", &SSGrain::test)
    .def("Info", &SSGrain::Info)
    ;
    
        //TODO !!! check no_init? inherithance of Grain correct? property initialRadiusDistribution?
  boost::python::class_<Precipitate, boost::python::bases<Grain>, boost::noncopyable >("Precipitate", boost::python::no_init /*<Material&, ChemicalComposition&, RadiusDistribution& >()*/ )
    .add_property("deltaCell", &Precipitate::GetDeltaCell,&Precipitate::SetDeltaCell)
    .add_property("solvusActivationEnergy", &Precipitate::GetSolvusActivationEnergy, &Precipitate::SetSolvusActivationEnergy)
    .add_property("distorsionEnergy", &Precipitate::GetDistorsionEnergy)
    .add_property("initialNucleationSitesNumber", &Precipitate::GetInitialNucleationSitesNumber,&Precipitate::SetInitialNucleationSitesNumber)
    .add_property("preExpTermForSolvus", &Precipitate::GetPreExpTermForSolvus,&Precipitate::SetPreExpTermForSolvus)
    .add_property("shapeFactor", &Precipitate::GetShapeFactor,&Precipitate::SetShapeFactor)
    .add_property("volumicFraction", &Precipitate::GetVolumicFraction,&Precipitate::SetVolumicFraction)
    .add_property("surfaceEnergyCurrentValue", &Precipitate::GetSurfaceEnergyCurrentValue)
    .add_property("phaseChangeVolumiqueEnergy", &Precipitate::GetPhaseChangeVolumicEnergy)
    .def("ComputeNucleationSiteNb", &Precipitate::ComputeNucleationSiteNb)
    //.add_property("surfaceEnergyPolynomial", &Precipitate::GetSurfaceEnergyPolynomial)
    //.add_property("initialRadiusDistribution", &Precipitate::GetInitialRadiusDistribution)
    .def("ComputeDistorsionEnergy", &Precipitate::ComputeDistorsionEnergy)
    .def("ComputeSurfaceEnergy", &Precipitate::ComputeSurfaceEnergy)
    .def("SetSEPolynomialDegree", &Precipitate::SetSEPolynomialDegree)
    .def("AddSEPolynomialPyCoefs", &Precipitate::AddSEPolynomialPyCoefs)
    .def("ConvertStoichiometricCoefficientToAtomicConcentration", &Precipitate::ConvertStoichiometricCoefficientToAtomicConcentration)
    .def("ConvertAtomicToVolumicConcentration", &Precipitate::ConvertAtomicToVolumicConcentration)
    .def("InitializeEquilibriumConcentrationMap", &Precipitate::InitializeEquilibriumConcentrationMap)
    .def("GetEquilibriumConcentrationForElement", &Precipitate::GetEquilibriumConcentrationForElement)
    .def("ComputePhaseChangeVolumicEnergy", &Precipitate::ComputePhaseChangeVolumicEnergy)
    .def("SolveEquilibriumConcentrationsEquations", &Precipitate::SolveEquilibriumConcentrationsEquations)
    .def("ComputeEquilibriumConcentrations", &Precipitate::ComputeEquilibriumConcentrations)
    .def("ProcessNucleationRate", &Precipitate::ProcessNucleationRate)
    .def("ComputeAllInterfacialConcentrations", &Precipitate::ComputeAllInterfacialConcentrations)
    .def("Info", &Precipitate::Info)
    ;
    
  //TODO !!!check if inherithance wrapping is correct --> boost::python::bases<Grain> ? 
  boost::python::class_<GuinierPreston,boost::python::bases<Precipitate> >("GuinierPreston", boost::python::init<Material&, ChemicalComposition&, RadiusDistribution& >() )
    .def("Info", &GuinierPreston::Info)
    ;
    
  boost::python::class_<Sprime,boost::python::bases<Precipitate> >("Sprime", boost::python::init<Material&, ChemicalComposition&, RadiusDistribution&, double>()  )
    .add_property("wettingAngle", &Sprime::GetWettingAngle)
    .add_property("Stheta", &Sprime::GetStheta)
    .def("ComputeWettingFunction", &Sprime::ComputeWettingFunction)
    .def("Info", &Sprime::Info)
    ;
    
  
  boost::python::class_<Hardening>("Hardening", boost::python::init<double, Computation& >() )
    .add_property("maxTimeStep", &Hardening::GetMaxTimeStep, &Hardening::SetMaxTimeStep)
    .add_property("duration", &Hardening::GetDuration)
    .def("Info", &Hardening::Info)
    ;
    
    
    //TODO class InterfacialConcentration binding
    
    
  //TODO some stuff
  boost::python::class_<Material>("Material", boost::python::init<Temperature&, ChemicalElement&, ChemicalComposition& , Computation& >() )
    //.add_property("temperature", &Material::GetTemperature,&Material::SetTemperature)
    //.add_property("vacancyList", &Material::GetVacancyList)
    .def("GetSSGrain", &Material::GetSSGrain,boost::python::return_internal_reference<>() ) //Gives the C++ object ssgrain
    .def("GetSSGrainPointer", &Material::GetSSGrainPointer/*,boost::python::return_value_policy<boost::python::manage_new_object>()*/ , boost::python::return_internal_reference<>() ) //Gives the C++ object ssgrain
    .def("GetMainChemicalElement", &Material::GetMainChemicalElement,boost::python::return_internal_reference<>() )
    .def("GetInitialChemicalComposition", &Material::GetInitialChemicalComposition,boost::python::return_internal_reference<>() )
    .def("GetCurrentChemicalComposition", &Material::GetCurrentChemicalComposition,boost::python::return_internal_reference<>() )
    //.add_property("precipitateList", &Material::GetPrecipitateList)
    .add_property("YoungModulus", &Material::GetYoungModulus)
    .add_property("PoissonCoeff", &Material::GetPoissonCoeff)
    .add_property("latticeParameter", &Material::GetLatticeParameter)
    .def("SetVacancy", &Material::SetVacancy)
    .def("InitializeGrains", &Material::InitializeGrains)
    .def("ConvertVolumicToInitialAtomicConcentration", &Material::ConvertVolumicToInitialAtomicConcentration)
    .def("ConvertVolumicToInitialMassicConcentration", &Material::ConvertVolumicToInitialMassicConcentration)
    .def("ReturnAtomicConcFromVolumicForElement", &Material::ReturnAtomicConcFromVolumicForElement)
    .def("UpdateVolumicValues", &Material::UpdateVolumicValues)
    .def("RunProcess", &Material::RunProcess)
    .def("test", &Material::test)
    .def("Info", &Material::Info)
    ;
    

    
    //TODO Info()
  boost::python::class_<Polynomial>("Polynomial", boost::python::init<int >() )
    .add_property("degree", &Polynomial::GetDegree)
    .add_property("coefsList", &Polynomial::GetCoefsList,&Polynomial::SetCoefs)
    .def("AddPyCoefs", &Polynomial::AddPyCoefs)
    .def("ReturnValue", &Polynomial::ReturnValue)
    .def("Info", &Polynomial::Info)
    
    
    ;
    
  boost::python::class_<Quenching>("Quenching", boost::python::init<Computation&, double, double, double, boost::python::optional<double> >() )
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
    .def("GetTemperaturePyList", &Temperature::GetTemperaturePyList )
    .def("Info", &Temperature::Info)
    ;
    
    //TODO some stuff
  boost::python::class_<ThermalLoading>("ThermalLoading", boost::python::init< Computation& >() )
    .add_property("duration", &ThermalLoading::GetDuration, &ThermalLoading::SetDuration)
    //.add_property("time_", &ThermalLoading::GetElementName)
    //.add_property("temperature_", &ThermalLoading::GetElementName)
    .def("Info", &ThermalLoading::Info)
    ;
      
    
  boost::python::class_<ThermoDynamicsConstant>("ThermoDynamicsConstant", boost::python::init<double, double, double>() )
    .add_property("R",  &ThermoDynamicsConstant::Get_R)
    .add_property("kB", &ThermoDynamicsConstant::Get_kB)
    .add_property("Na", &ThermoDynamicsConstant::Get_Na)
    .def("Info", &ThermoDynamicsConstant::Info)
    ;    

  boost::python::class_<Vacancy>("Vacancy", boost::python::init<double, double, double, double, double, double, double, Material&, boost::python::optional<double> >() )
    .add_property("vacCreationEnthalpy", &Vacancy::GetVacCreationEnthalpy, &Vacancy::SetVacCreationEnthalpy)
    .add_property("vacCreationEntropy", &Vacancy::GetVacCreationEntropy, &Vacancy::SetVacCreationEntropy)
    .add_property("migrationEnthalpy", &Vacancy::GetMigrationEnthalpy, &Vacancy::SetMigrationEnthalpy)
    .add_property("halfSinkDistance", &Vacancy::GetHalfSinkDistance, &Vacancy::SetHalfSinkDistance)
    .add_property("jumpFrequency", &Vacancy::GetJumpFrequency, &Vacancy::SetJumpFrequency)
    .add_property("preExpDiffusionValue", &Vacancy::GetPreExpDiffusionValue, &Vacancy::SetPreExpDiffusionValue)
    .add_property("solutionisingTemp", &Vacancy::GetSolutionisingTemp, &Vacancy::SetSolutionisingTemp)
    .add_property("vacancyDiffusionCoef", &Vacancy::GetVacancyDiffusionCoef)
    .add_property("equilibriumConc", &Vacancy::GetEquilibriumConcentration)
    .def("ComputeDiffusionCoefValue", &Vacancy::ComputeDiffusionCoefValue)
    .def("ComputeEquilibriumConcentration", &Vacancy::ComputeEquilibriumConcentration)
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
