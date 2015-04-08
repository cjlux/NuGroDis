# -*- coding: utf-8 -*-
from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")

from Metallurgical import Computation, \
                          Quenching, \
                          ThermoDynamicsConstant , \
                          Vacancy, \
                          RadiusDistribution, \
                          Hardening, \
                          ChemicalElement
                          
                              

# We execute le file Nugrodis.py:
import nugrodis, Mendeleiev


# Create on object of type Computation:
c = Computation()
c.type = nugrodis.ComputationParam["Type"]

material = nugrodis.ComputationParam["Material"]
print("  > importing module",material)
exec "import "+material

print("  > Computation.type : ", c.type)

# Create an object of type ThermoDynamicsConstant:
thermoDynConst = ThermoDynamicsConstant(nugrodis.PhysicalConstants['R'][0],
                                        nugrodis.PhysicalConstants['kB'][0],
                                        nugrodis.PhysicalConstants['Na'][0])

thermoDynConst.Info();

#Build the Mendeleiv table
Mendeleiev.PyChemicalElement.init()

#Read Chemical Composition of precipitates which will be involved in the computation
print("Read Chemical Composition of precipitates which will be involved during the computation")
    #Read GBP chemical composition
GP_chemicalC=Mendeleiev.PyChemicalComposition(M2024.GP['chemicalComposition'][0])
print(GP_chemicalC)
    #Read Sprime chemical composition
Sprime_chemicalC=Mendeleiev.PyChemicalComposition(M2024.Sprime['chemicalComposition'][0])
print(Sprime_chemicalC)


#Create C++ objects of type chemicalElement: new Method using Mendeleiv.py (using JLC's Mendeleiev.py )
ConcernedChemicalElements=list(set(GP_chemicalC.composition.keys()) | set(Sprime_chemicalC.composition.keys())) #a list of chemical elements involded in the computation  = {GP} U {Sprime} - { {GP}  inter {Sprime}}
for chemicalElementName in ConcernedChemicalElements:
    chemicalElementObjectName=chemicalElementName #Just to be more understandable
    exec (chemicalElementObjectName+"=ChemicalElement(nugrodis.ChemicalElementsData[chemicalElementName]['density'][0],\
                                        nugrodis.ChemicalElementsData[chemicalElementName]['molarMass'][0],\
                                        nugrodis.ChemicalElementsData[chemicalElementName]['youngModulus'][0],\
                                        nugrodis.ChemicalElementsData[chemicalElementName]['poissonCoef'][0],\
                                        chemicalElementName)")
    exec(chemicalElementObjectName+".Info()")

###Create C++ objects of type chemicalElement: old Method (by MG)
##NamesOfObjectChemEl=[]
##for key in nugrodis.ChemicalElementsData.keys():
##    chemicalElementName=key 
##    chemicalElementObjectName=chemicalElementName #Just to be more understandable
##    exec (chemicalElementObjectName+"=ChemicalElement(nugrodis.ChemicalElementsData[chemicalElementName]['density'][0],\
##                                        nugrodis.ChemicalElementsData[chemicalElementName]['molarMass'][0],\
##                                        nugrodis.ChemicalElementsData[chemicalElementName]['youngModulus'][0],\
##                                        nugrodis.ChemicalElementsData[chemicalElementName]['poissonCoef'][0],\
##                                        chemicalElementName)")
##    exec(chemicalElementObjectName+".Info()")


                                        
                                        
    
    

# Create an object type Vacancy:
Vacancy= Vacancy(nugrodis.VacanciesParam['deltaHF'][0],
             nugrodis.VacanciesParam['deltaSF'][0],
             nugrodis.VacanciesParam['deltaHM'][0],
             nugrodis.VacanciesParam['fE'][0],
             nugrodis.VacanciesParam['Dlac0'][0],
             nugrodis.VacanciesParam['halfSinkD'][0], 
             M2024.VacanciesParam['Tsol'][0],
             nugrodis.VacanciesParam['EVacCu'][0],
             nugrodis.VacanciesParam['EVacMg'][0])
Vacancy.Info();

#Create an object of type RadiusDistribution
RadDis=RadiusDistribution(nugrodis.CellParam['spatialStep'][0],
                          nugrodis.CellParam['minimumRadius'][0],
                          nugrodis.CellParam['initialClassNumber'][0])
RadDis.Info();
                          
ComputationList = c.type.split(">")
for computation in ComputationList:
    print("  > Processing <"+computation+">")

    if computation == "Quenching":
        # Create an object of type Quenching:
        Quenching = Quenching(M2024.VacanciesParam["Tsol"][0],
                              nugrodis.QuenchingParam["Tfinal"][0],
                              nugrodis.QuenchingParam["v"][0])
        Quenching.Info();
    if computation == "Hardening":
        # Create an object of type Hardening:
        Hardening=Hardening(nugrodis.HardeningParam["duration"][0],
                            nugrodis.HardeningParam["initialTimeStep"][0])
        Hardening.Info()
        #Create an object of type RadiusDistribution
        HardeningRadDis=RadiusDistribution(nugrodis.CellParam['spatialStep'][0],
                          nugrodis.CellParam['minimumRadius'][0],
                          nugrodis.HardeningParam['initialClassNumber'][0])
        HardeningRadDis.Info();
     ##if computation == "ThermalLoading":
        ### Create an object of type ThermalLoading:
        ##ThermalLoading=thermalLoading()


# some stuff.... for Moubarak !

c.Run()

# éééé
