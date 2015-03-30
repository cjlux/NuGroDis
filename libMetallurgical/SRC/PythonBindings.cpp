#include <boost/python.hpp>

#include "ThermoDynamicsConstant.hpp"
#include "Computation.hpp"
#include "Quenching.hpp"
#include "Vacancy.hpp"

// Python Wrapper
BOOST_PYTHON_MODULE(Metallurgical)
{
  boost::python::class_<Computation>("Computation", boost::python::init<>() )
    .add_property("type", &Computation::GetType, &Computation::SetType)
    .def("Run", &Computation::Run) 
    ;

  boost::python::class_<ThermoDynamicsConstant>("ThermoDynamicsConstant", boost::python::init<double, double, double>() )
    .add_property("R",  &ThermoDynamicsConstant::GetR)
    .add_property("kB", &ThermoDynamicsConstant::Get_kB)
    .add_property("Na", &ThermoDynamicsConstant::GetNa)
    .def("Info", &ThermoDynamicsConstant::Info)
    ;

  boost::python::class_<Quenching>("Quenching", boost::python::init<double, double, double, boost::python::optional<double> >() )
    .add_property("CoolingRate", &Quenching::GetCoolingRate, &Quenching::SetCoolingRate)
    .add_property("TotIterationNumber", &Quenching::GetTotIterationNumber, &Quenching::SetTotIterationNumber)
    .add_property("FinalTemp", &Quenching::GetFinalTemp)
    .add_property("CoolingRate", &Quenching::GetCoolingRate)
    .add_property("CoolingRate", &Quenching::GetCoolingRate)
    .def("Info", &Quenching::Info)
    ;
    
     boost::python::class_<Vacancy>("Vacancy", boost::python::init<double, double, double, double, double, double, double, double, double >() )
    .add_property("vacCreationEnthalpy", &Vacancy::GetVacCreationEnthalpy, &Vacancy::SetVacCreationEnthalpy)
    .add_property("vacCreationEntropy", &Vacancy::GetVacCreationEntropy, &Vacancy::SetVacCreationEntropy)
    .add_property("migrationEnthalpy", &Vacancy::GetMigrationEnthalpy, &Vacancy::SetMigrationEnthalpy)
    .add_property("halfSinkDistance", &Vacancy::GetHalfSinkDistance, &Vacancy::SetHalfSinkDistance)
    .add_property("jumpFrequency", &Vacancy::GetJumpFrequency, &Vacancy::SetJumpFrequency)
    .add_property("preExpDiffusionValue", &Vacancy::GetPreExpDiffusionValue, &Vacancy::SetPreExpDiffusionValue)
    .add_property("magnesiumVacInteractionEnergy", &Vacancy::GetMagnesiumVacInteractionEnergy, &Vacancy::SetMagnesiumVacInteractionEnergy)
    .add_property("solutionisingTemp", &Vacancy::GetSolutionisingTemp, &Vacancy::SetSolutionisingTemp)
    .add_property("copperVacInteractionEnergy", &Vacancy::GetCopperVacInteractionEnergy, &Vacancy::SetCopperVacInteractionEnergy)
    .def("Info", &Vacancy::Info)
    ;
    
}
