# -*- coding: utf-8 -*-

#recurrent bugs Section:
#extension class wrapper for base class Precipitate has not been created yet: swap the order of the class_ statements, base class before derived class



from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")

#Bindings
from Metallurgical import ChemicalComposition, \
                                             ChemicalElement, \
                                             Concentration, \
                                             Computation, \
                                             Diffusion, \
                                             GuinierPreston, \
                                             Hardening, \
                                             Material, \
                                             Quenching,\
                                             RadiusDistribution, \
                                             Sprime, \
                                             SSGrain, \
                                             Temperature, \
                                             ThermalLoading, \
                                             ThermoDynamicsConstant , \
                                             Vacancy
                                             
                                             
                                             
                          
                              

# execute file Nugrodis.py:
import nugrodis
from MetalUtils.Mendeleiev import PyChemicalElement, PyChemicalComposition #import python Class:  PyChemicalElement, PyChemicalComposition from module MetalUtils
from MetalUtils.Grain import PyPrecipitate, PrecipitateNatureList #import:  python Class PyPrecipitate module MetalUtils, Precipitate nature list
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


# Create a C++ object of type ThermoDynamicsConstant:
thermoDynConst = ThermoDynamicsConstant(PhysicalConstantsDict['R'][0],
                                        PhysicalConstantsDict['kB'][0],
                                        PhysicalConstantsDict['Na'][0])
thermoDynConst.Info();


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
          

#type of the computation: Q 
print("  > Computation.type                 :", c.type)



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

#Create a C++ object ChemicalComposition: Initial Chemical Composition of material
InitialCCCpp=ChemicalComposition("solide solution");


#reading Temperature parameters and Create a C++ object of type Temperature: Initial Temperature
TempParam=nugrodis.TemperatureParam
print("  > Found TemperatureParam           : ", TempParam)
InitialTempCpp=Temperature(TempParam['T0'][0]);


#Create a C++ object of type Material
CppMaterial=Material(InitialTempCpp,
                     CppElementsDict[mainEl],
                     InitialCCCpp);


#Reading Vacancies Parameters
exec "VacParam="+material+".VacanciesParam"
print("  > Found dico VacancyParam           : ", VacParam)
exec "CppVacancy= Vacancy("+material+".VacanciesParam['deltaHF'][0],\
                 "+material+".VacanciesParam['deltaSF'][0],\
                 "+material+".VacanciesParam['deltaHM'][0],\
                 "+material+".VacanciesParam['fE'][0],\
                 "+material+".VacanciesParam['Dlac0'][0],\
                 "+material+".VacanciesParam['halfSinkD'][0],\
                 "+material+".VacanciesParam['Tsol'][0],\
                 CppMaterial)"
CppVacancy.Info();


#Search for DiffusionParam for solute chemicalElements ( any element different from the main chemical Element)
#Read dictionaries which have a nature of  DiffusionParam
CppDiffusionDict={} #a dictionary of Chemical Elements names as keys with their C++ diffusion Objects associated. {"ChemicalElementSymbol": diffusionCppObject}
for e in L: 
    if e[0] is not '_':
        exec 'E = '+material+'.'+e
        if type(E)==dict :
            nature = E.get('nature', None)
            if nature is 'DiffusionParam':
                assert ( (E.get('preExpDiffusionCoef', None)==None and E.get('Q', None)==None) or (E.get('preExpDiffusionCoef', None)!=None and E.get('Q', None)!=None) ),\
                       "Missing attribute  'preExpDiffusionCoef' or 'Q'  in diffusion parameter dico  <{:s}> :  {:s} ".format(e,E) #       assertion failed:  001, 010, 101,110
                if (E.get('EVac', None)==None and E.get('preExpDiffusionCoef', None)==None and E.get('Q', None)==None): #000.
                    #No Vacancy diffusion(i.e. interaction ) and no atomic diffusion. Build C++ objects Diffusion according to read dictionnaries :
                    print("  > Chemical Elements <{:s}> has no diffusion parameter (neither atomic diffusion nor Vacancy  interaction)".format(e))
                elif ( E.get('EVac', None)!=None and E.get('preExpDiffusionCoef', None)!=None and E.get('Q', None)!=None ): #111
                    #Atomic Diffusion and Vacancy diffusion. Therefore, appropriate C++ constructor for Diffusion must be used 
                    print("  > Found diffusion parameters dico <{:s}> with atomic diffusion and  vacancies interaction energy : {:s}".format(e,E))
                    print("         > building C++ Object <Diffusion> for Chemical Element <{:s}>".format(e))
                    CppDiffusionDict[e]=Diffusion(CppElementsDict[e],
                                                  CppMaterial,
                                                  CppVacancy,
                                                  E["preExpDiffusionCoef"][0],
                                                  E["Q"][0],
                                                  E["EVac"][0])
                elif ( E.get('EVac', None)!=None and E.get('preExpDiffusionCoef', None)==None and E.get('Q', None)==None ): #100
                    #Only vacancies diffusion or interaction , i.e. no Atomic diffusion but only vacancies interaction. Therefore, appropriate C++ constructor for Diffusion must be used
                    print("  > Found diffusion parameters dico <{:s}> with only vacancies diffusion(interaction energy)  and no atomic diffusion : {:s}".format(e,E))
                    print("         > building C++ Object <Diffusion> for Chemical Element <{:s}>".format(e))
                    CppDiffusionDict[e]=Diffusion(CppElementsDict[e],
                                                  CppVacancy,
                                                  E["EVac"][0])
                elif ( E.get('EVac', None)==None and E.get('preExpDiffusionCoef', None)!=None and E.get('Q', None)!=None ):   #011
                    #ONLY ATOMIC DIFFUSION, i.e. no vacancy interaction but Atomic diffusion. Therefore, appropriate C++ constructor for Diffusion must be used
                    print("  > Found diffusion parameters dico <{:s}> with only atomic diffusion and no vacancies interaction : {:s}".format(e,E))
                    print("         > building C++ Object <Diffusion> for Chemical Element <{:s}>".format(e))
                    CppDiffusionDict[e]=Diffusion(CppElementsDict[e],
                                                  CppMaterial,
                                                  E["preExpDiffusionCoef"][0],
                                                  E["Q"][0])
                CppDiffusionDict[e].Info()
print("  > C++ Diffusion Objects Dictionary = ",CppDiffusionDict)

pyPD={} #a dictionary of precipitates Names as keys with their precipitates python Objects associated. {"precipitate Name": precipitatePyObject}
#Read Precipitate dictionaries in Module material 
for e in L: 
    if e[0] is not '_':
        exec 'E = '+material+'.'+e
        if type(E)==dict :
            nature = E.get('nature', None)
            if nature in PrecipitateNatureList:
                print("  > Found <"+nature+"> Precipitate dico named <{:s}> : {:s}".format(e,E))
                #Build python objects PyPrecipitates according to read dictionnaries
                pyPD[e]=PyPrecipitate(E,mainEl) #{"precipitate Name": precipitatePyObject}
print("  > PyPrecipitates python Objects  dictionary = ",pyPD)                    
                          
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
        # Create a C++ object of type Hardening:
        CppHardening=Hardening(nugrodis.HardeningParam["duration"][0],
                            nugrodis.HardeningParam["initialTimeStep"][0])
        CppHardening.Info()
        #Create a C++ object of type RadiusDistribution
        CppHardeningRadDis=RadiusDistribution(nugrodis.CellParam['spatialStep'][0],
                          nugrodis.CellParam['minimumRadius'][0],
                          nugrodis.HardeningParam['initialClassNumber'][0])
        CppHardeningRadDis.Info();
        #Create Precipitates C++ object  
        CppPrecipitateDict={} #a dictionary of precipitates Names as keys with their C++ Precipitate Objects associated. {"precipitate Name": CppPrecipitateObject}
        
        for x in pyPD:
            pyPrecipitateObj=pyPD[x]
            #Step1: Build A C++ object ChemicalComposition. necessary for building Precipitate CppObject
            exec "CppChemicalComposition"+x+"=ChemicalComposition(pyPrecipitateObj.chemicalComposition[0])"  # e.g.  CppChemicalCompositionGP, CppChemicalCompositionSprime, ...
            exec "CppCompo=CppChemicalComposition"+x #just to be more understandable
            #Step2: Create Precipitates: GuinierPreston , Sprime, and ....
            if pyPrecipitateObj.nature=="GuinierPreston":
                #Create a GuinierPreston C++ object
                print("  > Building a C++ GuinierPreston object  : <"+x+">")
               #exec "Cpp"+x+"=GuinierPreston(CppMaterial,CppCompo,CppHardeningRadDis)"
                CppPrecipitateDict[x]=GuinierPreston(CppMaterial,CppCompo,CppHardeningRadDis)
                #step3: Set CppPrecipitates other properties manually or  with Precipitate.InitializeParameters(...)?
            if pyPrecipitateObj.nature=="Sprime":
                #Create a Sprime C++ object
                print("  > Building a C++ Sprime object               : <"+x+">")
               #exec "Cpp"+x+"=Sprime(CppMaterial,CppCompo,CppHardeningRadDis,pyPrecipitateObj.wettingAngle[0])"
                CppPrecipitateDict[x]=Sprime(CppMaterial,CppCompo,CppHardeningRadDis,pyPrecipitateObj.wettingAngle[0])
                #step3: Set CppPrecipitates other properties manually or  with Precipitate.InitializeParameters(...)?
##    if computation == "ThermalLoading":
##        #Create an object of type ThermalLoading:
##        ThermalLoading=thermalLoading()

#toto=PyChemicalComposition("Al2Cu8")
#print(toto)
#print(toto.composition)

# some stuff.... for Moubarak !

c.Run()

# éééé
