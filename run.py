# -*- coding: utf-8 -*-
from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")

#Bindings
from Metallurgical import Quenching,\
                                             Computation,\
                                             ThermoDynamicsConstant , \
                                             Vacancy, \
                                             RadiusDistribution, \
                                             Hardening, \
                                             ChemicalElement
                          
                              

# execute file Nugrodis.py:
import nugrodis
from MetalUtils.Mendeleiev import PyChemicalElement, PyChemicalComposition #import python Class:  PyChemicalElement, PyChemicalComposition from module MetalUtils
from MetalUtils.Grain import PyPrecipitate #import python Class: PyPrecipitate module MetalUtils
from MetalUtils.PhysicalConstants import Dict as PhysicalConstantsDict #import Physical constants data dictionary from module MetalUtils

print (" ____________________________________________________________________ ")
print(" |                                                            \    /_ | _ _ ._ _  _  _|_ _                        |") 
print(" |                                                             \/\/(/_|(_(_)| | |(/_  |_(_)                      |")
print(" |                                                                         __              __                          |") 
print(" |                                                               |\ |     |  __  __   __  |  \      _              |")
print(" |                                                               | \| |_| |__|  |    |__| |_ /  |  /_\           |")
print ("|___________________________________________________________________   |")


print ("|####################################################################################################|")
print ("|######################################  WELCOME TO NUGRODIS #########################################|")
print ("|####################################################################################################|")



# Create a C++ object of type Computation:
c = Computation()
c.type = nugrodis.ComputationParam["Type"]

material = nugrodis.ComputationParam["Material"]
print("  > importing module                 :",material)
exec "import "+material #importing module material given by user
exec 'L = dir('+material+')' #import  L=directory of module material

print("  > material read is                 :",material)

#Reading initial alloy compo
exec "AlloyInitialComposition="+material+".AlloyInitialComposition" 
print("  > Alloy composition read is        :", AlloyInitialComposition)

#Find main element of Alloy
maxConcentrationValue=max(AlloyInitialComposition.values());
for key,value in AlloyInitialComposition.items():
    if value==maxConcentrationValue:
        mainEl=key #mainEl is the main Chemical element of alloy
        print("  > Alloy main chemical element is   : "+mainEl)

PD={} #a dictionary of precipitates Names as keys with their precipitates python Objects associated. {"precipitate Name": precipitatePyObject}
#Read Precipitate dictionaries in Module material 
for e in L: 
    if e[0] is not '_':
        exec 'E = '+material+'.'+e
        if type(E)==dict :
            nature = E.get('nature', None)
            if nature is 'Precipitate':
                print("  > Found Precipitate dico <{:s}> : {:s}".format(e,E))
                #Build python objects PyPrecipitates according to read dictionnaries
                PD[e]=PyPrecipitate(E,mainEl) #{"precipitate Name": precipitatePyObject}
          

#type of the computation: Q 
print("  > Computation.type                 :", c.type)

# Create a C++ object of type ThermoDynamicsConstant:
thermoDynConst = ThermoDynamicsConstant(PhysicalConstantsDict['R'][0],
                                        PhysicalConstantsDict['kB'][0],
                                        PhysicalConstantsDict['Na'][0])
thermoDynConst.Info();

#Build the Mendeleiv table
PyChemicalElement.initFromElementsData()
ED = PyChemicalElement.GetElementsDict() #a dictionary of chemichal Elements Names as keys with their chemicalElement python Objects associated. {"chemichal Elements Name": chemicalElementPyObject}

#A list of chemical elements involded in the computation using chemical Elements read in AlloyInitialComposition dictionary from Module material
#AlloyInitialComposition.keys() gives the list of chemical elements involded in the computation
ConcernedPyChemicalElements=[] #a list which will contains python objects of type chemicalElements according to chemical elements involved in the computation
for symbol in AlloyInitialComposition.keys():
    ConcernedPyChemicalElements.append(ED[symbol])

#Create C++ objects of type ChemicalElement using Mendeleiv.py
CppElementsDict={}#a dictionary of chemichal Elements Names as keys with their C++ Objects associated. {"chemichal Element Name": chemicalElementC++Object}
for pyElement in ConcernedPyChemicalElements:
    CppElement=pyElement.symbol #Name of C++ chemical Element will be the symbol of the element. Just to be more understandable
    #Building a C++ objects of type chemicalElement
    exec (CppElement+"=ChemicalElement( pyElement.density,\
                                        pyElement.molarMass,\
                                        pyElement.YoungModulus,\
                                        pyElement.PoissonCoeff,\
                                        pyElement.symbol)")
    exec 'CppElementsDict[pyElement.symbol]='+CppElement
    exec CppElement+".Info()"
print("  > C++ Elements Dictionary = ",CppElementsDict)


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
