#!/usr/bin/python
# -*- coding: utf-8 -*-

#recurrent bugs Section:
#extension class wrapper for base class Precipitate has not been created yet: swap the order of the class_ statements, base class before derived class

from __future__ import division, print_function
from math import exp
from collections import OrderedDict
from MetalUtils.Utils import CopyAllDicoInNewPyFile,\
                                                 PrintAllDico, \
                                                 ReadAndProcessRadiusDistributionFile,\
                                                 ReadFileWithExtension

import argparse, sys, os
##################### PARSE ARGUMENTS 
parser = argparse.ArgumentParser(description='Run computation with given param GPsurfaceEnergyPolynomial,\
halfSinkDistance, hardening duration')

parser.add_argument('-g', type=str, help='gammaGP: Surface energy Polynomial Model as a list [a0, a1, ... , aN]  //Unit=[J/m^2]')
parser.add_argument('-l', type=float, help='half Sink distance  //Unit=[µm]')
parser.add_argument('-d', type=float, help='Computation duration = hardening duration //Unit=[second]')
parser.add_argument('-D', action='store_true', help='Run in debug mode for C++')
args = parser.parse_args()

gammaParser=args.g
halfSinkDParser=args.l
hardeningDurationParser=args.d
debug=args.D
################### END PARSE ARGUMENTS

if not debug:
    sys.path.append("libMetallurgical/PythonB/RELEASE")
else:
    sys.path.append("libMetallurgical/PythonB/DEBUG")

#Bindings
from Metallurgical import ChemicalComposition, \
                                             Boundary,\
                                             ChemicalElement, \
                                             Concentration, \
                                             Computation, \
                                             Diffusion, \
                                             GuinierPreston, \
                                             Hardening, \
                                             Material, \
                                             Polynomial,\
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
from MetalUtils.ElementsData import Dict as ElementsDataDict
import script

print ("|####################################################################################################|")
print ("|######################################  RUNNING FILE <run.py> ######################################|")
print ("|####################################################################################################|")

materialFileName = str(nugrodis.ComputationParam["Material"])
material = nugrodis.ComputationParam["Material"]

print("  > importing module                 :", material)
exec "import "+material        # imports the modulewhose name 'material' is was found in file "Nugrodis.py"
exec 'L = dir('+material+')'   # executes dir() of module 'material'
print("  > material read is                 :", material)


###### pre steps_ Copy old file material.py and nugrodis .py  ( Before update)#####
### this copy just all the dictionary of "material".py and nugrodis.py and in new files  ***_DicoCopy.py 
CopyAllDicoInNewPyFile(materialFileName) # creates file "material_DicoCopy.py"
CopyAllDicoInNewPyFile("nugrodis")       #create file "nugrodis_DicoCopy.py"


if ( (gammaParser!=None) & (halfSinkDParser!=None) & (hardeningDurationParser!=None) ):
    ##### Step 0.1: Saving old values of gamma, halfSinkD, and duration. (in order to replace old values by new ones, after)
    exec "oldLtuple="+material+".VacanciesParam['halfSinkD']"
    exec "oldGammaGPtuple="+material+".GP['surfaceEnergyPolynomialModel']"
    oldHardeningDurationtuple=nugrodis.HardeningParam["duration"]

    ##### Step 0.1: Changing Parameters with given parsed arguments:   gamma, halfSinkD, and duration
    ##### exec material+".GP.update({'surfaceEnergyPolynomialModel':(gammaParser,'J.m^-2')})" # unit in [J/m^2]
    exec material+".GP.update({'surfaceEnergyPolynomialModel':("+gammaParser+",'J.m^-2')})" # unit in [J/m^2]
    exec material+".VacanciesParam.update({'halfSinkD': (halfSinkDParser*1E-6,'m')})" # unit in micro meter [µm]
    nugrodis.HardeningParam.update({'duration': (hardeningDurationParser,'s')})# unit in second [s]

    ### Save current values ( values after update)
    exec "CurrentValueOfSurfaceEnergyPolynomialTuple="+material+".GP['surfaceEnergyPolynomialModel']"
    exec "CurrentValueOfHalfSinkDTuple="+material+".VacanciesParam['halfSinkD']"
    CurrentValueOfHardeningDurationTuple=nugrodis.HardeningParam["duration"]


#Name of Directory as parameters(g,l,d) values which will be used for computation
exec "lSave= "+material+".VacanciesParam['halfSinkD'][0]"
lSave=str(lSave)
exec "gammaGPSave= "+material+".GP['surfaceEnergyPolynomialModel'][0]"
gammaGPSave= str(gammaGPSave)[1:-1] #to take of "[" and "]"
durationSave=nugrodis.HardeningParam["duration"][0]
durationSave=str(durationSave)

#######################################
##### SETTING RESULT DIRETORY #########

#SavePath=nugrodis.savePath ##Oldversion, M.G.  thinks that this is the way things should be

##Actual version
SavePath="gamma_"+gammaGPSave+"_L_"+lSave+"_t_"+durationSave

##### SETTING RESULT DIRETORY #########
#######################################


#NAME of directory as parameters values

##########################################
# Create a C++ object of type Computation:
c = Computation(nugrodis.ComputationParam["initialTimeStep"][0],SavePath) # manualMaximumAllowedTimeStep_, manualMaximumAllowedTimeStep_ are optional.
                                                                          # by default, defineManuallyMaximumAllowedTimeStep_=false ; manualMaximumAllowedTimeStep_=-1  !!!!
c.Info()
c.type = nugrodis.ComputationParam["Type"]

# Create a C++ object of type Computation:
##########################################

######## OUTPUT DIRECTORY ###########
ComputationResultsDirectory=c.resultsDirectory
#####################################

###############################################################################################################################################
#####    Save Parameters used for computation:  save Material.py and nugrodis.py used for the computation as                             ######
#####                                           Material_save.py and nugrodis_save.py (Write new files in Back_up)                       ######

##### Step I.1: Create Back_Up folder in OutPut Directory
backUpDir=ComputationResultsDirectory+"/Back_up"
if not os.path.exists(backUpDir):
    os.makedirs(backUpDir)


##### Step II.2: Create empty files material_save.py and nugrodis_save.py in Back_Up folder   
newMaterialPyFile=open(backUpDir+'/'+material+'_save.py','w')
newNugrodisPyfile=open(backUpDir+'/'+'nugrodis_save.py','w')


##### Step II.3 (OLD): Copy the content of Old .py files (material.py and nugrodis.py) in empty new .py files (material_save.py and nugrodis_save.py)
##oldMaterialPyfile=open(material+".py",'r').read()
##oldNugrodisPyfile=open("nugrodis.py",'r').read()
##for line in oldMaterialPyfile:
##   newMaterialPyFile.write(line)
##newMaterialPyFile.close()
##for line in oldNugrodisPyfile:
##    newNugrodisPyfile.write(line)
##newNugrodisPyfile.close()



##### Step II.3 (NEW): Copy the content of Old 'pyfile'_DicoCopy.py files (material.py and nugrodis.py) in empty new .py files (material_save.py and nugrodis_save.py)
oldMaterialPyfile=open(material+"_DicoCopy.py",'r').read()
oldNugrodisPyfile=open("nugrodis_DicoCopy.py",'r').read()
for line in oldMaterialPyfile:
   newMaterialPyFile.write(line)
newMaterialPyFile.close()
for line in oldNugrodisPyfile:
    newNugrodisPyfile.write(line)
newNugrodisPyfile.close()

if ( (gammaParser!=None) & (halfSinkDParser!=None) & (hardeningDurationParser!=None) ):
    # :::::::   USE PARSED ARGUMENTS FOR THE COMPUTATION AND SAVE CORRECT 'material'.py and nugrodis.py used for Computation  ::::::: #
    
    ##### Step III: In Back_Up .py files (material_save.py and nugrodis_save.py) ==>
    ### replacing old values of params: gamma, halfSinkD, and duration, by the real values used for Computation



    ##### Check (Assert) single occurence of string to replace: check if Occurence of string to replace is 1. Because, it is not 1,
    # it means something we DONT WANT to change can be modified.
    materialBackUpPyFile=open(backUpDir+'/'+material+'_save.py','r+')
    nugrodisBackUpPyfile=open(backUpDir+'/'+'nugrodis_save.py','r+')  

    assert (oldMaterialPyfile.count(str(oldGammaGPtuple))==1), "old value of argument 'surfaceEnergyPolynomialModel' in dictionary GP can't be set\
by parsing. It has to be setted manually in file 'material'.py " 
    assert( oldMaterialPyfile.count( str(oldLtuple))==1   ),"The old tuple of argument 'halfSinkD' in dictionary VacanciesParam APPEARS MORE THAN\
ONE TIME so it can't be\
set by parsing. It has to be setted manually in file 'material'.py"
    assert( oldNugrodisPyfile.count( str(oldHardeningDurationtuple))==1   ),"The old tuple of argument duration in dictionary \
HardeningParam APPEARS MORE THAN ONE TIME so it can't be set by parsing. It has to be setted manually in file nugrodis.py"

    clearMaterialBackUpPyFile=open(backUpDir+'/'+material+'_save.py','w')
    clearNugrodisBackUpPyfile=open(backUpDir+'/'+'nugrodis_save.py','w')

    for line in oldMaterialPyfile.replace(str(oldGammaGPtuple), str(CurrentValueOfSurfaceEnergyPolynomialTuple)).replace(str(oldLtuple),str(CurrentValueOfHalfSinkDTuple)):
        clearMaterialBackUpPyFile.write(line)
    clearMaterialBackUpPyFile.close()
    for line in oldNugrodisPyfile.replace(str(oldHardeningDurationtuple),str(CurrentValueOfHardeningDurationTuple)):
       clearNugrodisBackUpPyfile.write(line)
    clearNugrodisBackUpPyfile.close()

##### END:Save Parameters used for computation:  save Material.py and nugrodis.py used for the computation (Write new files in Back_up)  ######
###############################################################################################################################################






#####################################################
########### SETTINGS FOR POSTPROCESSING #############
lastSaveTextFile=open('lastResultDirectoryPath.txt', 'w')
lastSaveTextFile.write(ComputationResultsDirectory)
lastSaveTextFile.close()
########### SETTINGS FOR POSTPROCESSING #############
#####################################################






#===================== Begin: Read and define computation type =====================

acceptedkeysForComputationType=["Quenching","Hardening","ThermalLoading"]

ComputationList = c.type.split(">")

#================================================================
SequenceOfPrecipitationComputing=ComputationList # copy of COmputationList
#modify computationSequenceWithoutQuenching by removing "Quenching"
while "Quenching" in SequenceOfPrecipitationComputing:
    SequenceOfPrecipitationComputing.remove("Quenching")
FirstSequenceOfPrecipitationComputing=SequenceOfPrecipitationComputing[0]
#================================================================


for computation in ComputationList:    
    
    ## check if 'computation' key is acceptable    
    if (computation not in acceptedkeysForComputationType):
        print("Error with given computation key '"+computation+ "' is not acceptable.\
 Computation key must be either : ",acceptedkeysForComputationType)        
    assert (computation in acceptedkeysForComputationType), "Error given computation key is not acceptable"
    
    print("  > Processing <"+computation+">")
    #QUENCHING
    if computation == "Quenching":
        # Create an object of type Quenching:
        exec "Quenching = Quenching(c,"+material+".VacanciesParam['Tsol'][0],\
                              nugrodis.QuenchingParam['Tfinal'][0],\
                              nugrodis.QuenchingParam['v'][0])"
        Quenching.Info();
    
    #COmputation is "Hardening or "ThermalLoading
             
    #HARDENING
    if computation == "Hardening":
        # Create a C++ object of type Hardening:
        CppHardening=Hardening(nugrodis.HardeningParam["duration"][0], c)
        CppHardening.Info()
    
    ##THERMAL LOADING
    if computation == "ThermalLoading":  

        ##Create an object of type ThermalLoading:
        CppThermalLoading=ThermalLoading(c, nugrodis.ThermalLoadingParam["maximumTimeStep"][0])          

        
        ## TODO!!! : initialize thermalLoading temperature   
        temperature_Data=ReadFileWithExtension(path=nugrodis.ThermalLoadingParam["loadProfile"][0],\
                                            extension=nugrodis.ThermalLoadingParam["loadProfile"][1],\
                                            delimiter=";")
        
        temperature_Data=script.ConvertDataInNumpyArrayFloat(temperature_Data)
        
        timeList, temperature_List=  list(temperature_Data[:,0]) , list(temperature_Data[:,1]) 
        
        
        CppThermalLoading.ReadTemperatureLoadingFromPythonList(timeList,temperature_List)
                
        CppThermalLoading.ReturnTemperatureAtTime(0) #debug
        CppThermalLoading.ReturnTemperatureAtTime(25) #debug
        CppThermalLoading.ReturnTemperatureAtTime(192) #debug
        CppThermalLoading.ReturnTemperatureAtTime(219) #debug
        CppThermalLoading.ReturnTemperatureAtTime(315.1666666667) #debug
        CppThermalLoading.ReturnTemperatureAtTime(283) #debug
        CppThermalLoading.ReturnTemperatureAtTime(535.0523809524) #debug
        
        CppThermalLoading.Info() 
        
        


#===================== End: Read and define computation type =====================





# Create a C++ object of type ThermoDynamicsConstant:
thermoDynConst = ThermoDynamicsConstant(PhysicalConstantsDict['R'][0],
                                        PhysicalConstantsDict['kB'][0],
                                        PhysicalConstantsDict['Na'][0])
thermoDynConst.Info();

#Reading initial alloy compo
exec "AlloyInitialComposition="+material+".AlloyInitialComposition"
#Ordered Composition in ascending order. The first value will be the minimum.
AlloyInitialComposition=OrderedDict(sorted(AlloyInitialComposition.items(), key=lambda t: t[1][0])) 
print("  > Alloy composition read is        :", AlloyInitialComposition)

#Find main element of Alloy
maxConcentrationValue=max(AlloyInitialComposition.values());
for key,value in AlloyInitialComposition.items():
    if value==maxConcentrationValue:
        mainEl=key #mainEl is the main Chemical element of alloy
        print("  > Alloy main chemical element is   : "+mainEl)
          

#type of the computation: Q 
print("  > Computation.type                 :", c.type)


mainElementLaticeParameter=ElementsDataDict[mainEl]["cellSize"][0]
#Build the Mendeleiv table
PyChemicalElement.initFromElementsData()
ED = PyChemicalElement.GetElementsDict() #Mendeleiv  Table : a dictionary of chemichal Elements Names as keys with their chemicalElement python Objects associated. {"chemichal Elements Name": chemicalElementPyObject}

#A list of chemical elements involded in the computation using chemical Elements read in AlloyInitialComposition dictionary from Module material
#AlloyInitialComposition.keys() gives the list of chemical elements involded in the computation
ConcernedPyChemicalElements=[] #a list which will contains python objects of type chemicalElements according to chemical elements involved in the computation
for symbol in AlloyInitialComposition.keys():
    ConcernedPyChemicalElements.append(ED[symbol])
print("AlloyInitialComposition.keys()",AlloyInitialComposition.keys())
print("AlloyInitialComposition",AlloyInitialComposition)
print("Coneerned python chemicalElements obj",ConcernedPyChemicalElements )

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
    if CppElement==mainEl:
        exec CppElement+".latticeParameter=mainElementLaticeParameter"
    exec "print ("+CppElement+")"
print("  > C++ Elements Dictionary = ",CppElementsDict)

#Create a C++ object ChemicalComposition: Initial Chemical Composition. This obect will be give to Cpp material 
InitialCCCpp=ChemicalComposition("solide solution");
#Add "ALL" Cpp ChemicalElement in Cpp object Initial ChemicalComposition
for u in AlloyInitialComposition.items():
    symbol = u[0]
    initialVolumicConcentration = u[1][0]
    #Add Cpp ChemicalElement in Cpp in Cpp object Initial ChemicalComposition (InitialCCCpp)
    CppElementsDict[symbol].EnterInChemicalComposition(InitialCCCpp)
    #SETTING Cpp CHEMICAL COMPOSITION'S CONCENTRATION BY FINDING CONCENTRATION IN C++ Map using name.
    #Set the volumic concentration of Cpp chemical Element in the Cpp Concentration for Cpp object Initial ChemicalComposition
    InitialCCCpp.GetConcentrationForElement(symbol).volumicValue = initialVolumicConcentration
    InitialCCCpp.mainElementName = mainEl
    print("CppConc Setted volumic volumic concentration ",InitialCCCpp.GetConcentrationForElement(symbol).volumicValue)

#reading Temperature parameters and Create a C++ object of type Temperature: Initial Temperature
TempParam=nugrodis.TemperatureParam
print("  > Found TemperatureParam           : ", TempParam)
InitialTempCpp=Temperature(TempParam['T0'][0]);

#Create a C++ object of type Material
CppMaterial=Material(InitialTempCpp,
                     CppElementsDict[mainEl],
                     InitialCCCpp,c);
CppMaterial.Info()

###Add "ALL" Cpp ChemicalElement in Cpp ChemicalComposition of Material
##CppCCMaterial=CppMaterial.GetInitialChemicalComposition()
##for u in AlloyInitialComposition.items():
##    symbol=u[0]
##    initialVolumicConcentration=u[1][0]
##    print("pouet 1")
##    #Add Cpp ChemicalElement in Cpp ChemicalComposition of Cpp Material
##    CppElementsDict[symbol].EnterInChemicalComposition(CppCCMaterial)
##    print("pouet 2")
###SETTING MATERIAL CONCENTRATION BY FINDING CONCENTRATION IN C++ Map using name.
##    CppMaterialConcentrationDict={}
##    print("pouet 3")
##    
##    CppMaterialConcentrationDict[symbol]=CppCCMaterial.GetConcentrationForElement(symbol) # add found concentration for element in concentration dictionary
##    print("pouet 4")
##    #Set the volumic concentration of Cpp chemical Element in the Cpp Concentration for the material's Cpp ChemicalComposition
##    CppCCMaterial.GetConcentrationForElement(symbol).volumicValue=initialVolumicConcentration
##    CppCCMaterial.mainElementName=mainEl
##    print("CppConc Setted volumic volumic concentration ",CppCCMaterial.GetConcentrationForElement(symbol).volumicValue)

CppMaterial.ConvertVolumicToInitialAtomicConcentration()	# Convert and set initialAtomicValues
CppMaterial.ConvertVolumicToInitialMassicConcentration()	# Convert and set initialMassicValues
    

#Reading Vacancies Parameters and Create a Cpp object Vacancy wich will be automatically setted as the material's vacancy. Remember that creating a vacancy Object  with a given material as argument, also set this vacancy as the material's vacancy
exec "VacParam="+material+".VacanciesParam"
print("  > Found dico VacancyParam           : ", VacParam)
exec "CppVacancy = Vacancy("+material+".VacanciesParam['deltaHF'][0],\
                 "+material+".VacanciesParam['deltaSF'][0],\
                 "+material+".VacanciesParam['deltaHM'][0],\
                 "+material+".VacanciesParam['fE'][0],\
                 "+material+".VacanciesParam['Dlac0'][0],\
                 "+material+".VacanciesParam['halfSinkD'][0],\
                 "+material+".VacanciesParam['Tsol'][0],\
                 CppMaterial,\
                 "+material+".VacanciesParam.get('coordinationNumber', [0,""])[0] )"
CppVacancy.Info();

print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& ONE')

#after setting material
#Search for DiffusionParam for solute chemicalElements ( any element different from the main chemical Element)
#Read dictionaries which have a nature of  DiffusionParam
CppDiffusionDict={} # a dictionary of Chemical Elements names as keys with their C++ diffusion Objects associated. {"ChemicalElementSymbol": diffusionCppObject}
for e in L: 
    if e[0] is not '_':
        exec 'E = '+material+'.'+e
        if type(E) == dict :
            nature = E.get('nature', None)
            if nature is 'DiffusionParam':
                assert ( (E.get('EVac', 'missing')!='missing' and E.get('preExpDiffusionCoef', 'missing')!='missing' and E.get('Q', 'missing')!='missing') ),\
                       "Chemical Elements <{:s}> has no diffusion parameter (neither atomic diffusion nor Vacancy  interaction). Missing keys :  'preExpDiffusionCoef',  'Q'  and 'EVac' ".format(e)
                assert ( (E.get('preExpDiffusionCoef', 'missing')!='missing' and E.get('Q', 'missing')!='missing')  ),\
                       "Missing attribute  'preExpDiffusionCoef' or 'Q' or both in diffusion parameter dico  <{:s}> :  {:s} ".format(e,E)
                assert ( (E.get('EVac', 'missing')!='missing' ) ),\
                       "Missing attribute  'EVac' in diffusion parameter dico  <{:s}> :  {:s} ".format(e,E)
                         
                #assert ( (E.get('preExpDiffusionCoef', None)==None and E.get('Q', None)==None) or (E.get('preExpDiffusionCoef', None)!=None and E.get('Q', None)!=None) ),\
                   #    "Missing attribute  'preExpDiffusionCoef' or 'Q'  in diffusion parameter dico  <{:s}> :  {:s} ".format(e,E)#       assertion failed:  001, 010, 101,110
                if ( E["EVac"][0]==None):
                    #ONLY ATOMIC DIFFUSION, i.e. no vacancy interaction but Atomic diffusion. Therefore, appropriate C++ constructor for Diffusion must be used
                    print("  > Found diffusion parameters dico <{:s}> with ONLY atomic diffusion and NO vacancies interaction : {:s}".format(e,E))
                    print("         > building C++ Object <Diffusion> for Chemical Element <{:s}>".format(e))
                    CppDiffusionDict[e]=Diffusion(CppElementsDict[e],
                                                  CppMaterial,
                                                  E["preExpDiffusionCoef"][0],
                                                  E["Q"][0])
                else:
                    try:
                        i = float(E["EVac"][0])
                    except (ValueError, TypeError):
                        print(" Value ==> "+E["EVac"][0]+" <== for key 'EVac' is not a numeric value , in diffusion parameters dico <{:s}>  : {:s}".format(e,E)  )
                        sys.exit(0)
                    #Found Atomic Diffusion param and Vacancy diffusion param. Therefore, appropriate C++ constructor for Diffusion must be used 
                    print("  > Found diffusion parameters dico <{:s}> with atomic diffusion and  vacancies interaction energy : {:s}".format(e,E))
                    print("         > building C++ Object <Diffusion> for Chemical Element <{:s}>".format(e))
                    CppDiffusionDict[e]=Diffusion(CppElementsDict[e],
                                                  CppMaterial,
                                                  E["preExpDiffusionCoef"][0],
                                                  E["Q"][0],
                                                  E["EVac"][0])
                CppDiffusionDict[e].Info()
print("  > C++ Diffusion Objects Dictionary = ",CppDiffusionDict)

print("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& TWO") 

pyPD={} # a dictionary of precipitates Names as keys with their precipitates python Objects associated. {"precipitate Name": precipitatePyObject}

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



#InitialiszeGrains. Create C++ object SSGrain which belongs to the material,  and has the same properties as the material. TODO: call it InitializeSSGrain ?
CppMaterial.InitializeGrains()
#REMEMBER: if concentration in Material SSGrain is change, it affects concentration of Material, and vice versa    



#### old place where computation type is read and define      
        


            
##############################################        
################ TEST ; WORK IN PROGRESS ########################


#Create Precipitates C++ object  
CppPrecipitateDict={} # Dictionary of precipitates Names as keys with their C++ Precipitate Objects associated. {"precipitate Name": CppPrecipitateObject}    



CppInitialRadDisDict = {} # Dictionary of precipitates Names as keys with their initial C++ RadiusDistribution Objects associated. {"precipitate Name": CppRadiusDIstributionObject}
precipitateNatureList=["GuinierPreston", "Sprime"]    
for x in pyPD:    
    pyPrecipitateObj=pyPD[x]
    

    if pyPrecipitateObj.nature not in precipitateNatureList:
        print("Precipitate nature should be in the following list : ", precipitateNatureList)
        sys.exit(-1)
        
        
    #============================================================    
    # BEGIN: initializing radius distribution
    #============================================================
        
    #============================================================ 
    # step0: Create a C++ object of type RadiusDistribution (Required 
    #                                  for building Precipitate CppObject)
    #============================================================ 
    if computation == "Hardening":   
        
        #============================================================ 
        #  Create a C++ object of type RadiusDistribution in case of hardening
        #============================================================             
        CppInitialRadDisDict[x]=RadiusDistribution(nugrodis.CellParam['spatialStep'][0],\
                                                        nugrodis.CellParam['minimumRadius'][0],\
                                                        nugrodis.CellParam['initialClassNumber'][0])
        CppInitialRadDisDict[x].Info()

    if computation == "ThermalLoading":
        #============================================================ 
        #   Load GuinierPreston AND/OR Sprime Radius initial distribution for thermalLoading
        #                               & 
        #   therefore Create a C++ object of type RadiusDistribution
        #============================================================            
        if pyPrecipitateObj.nature == "GuinierPreston":
            ## load GuinierPreston initial Radius distribution
            radStepGP,minRGP,numberOfClassGP,radDisDataGP=ReadAndProcessRadiusDistributionFile(path=nugrodis.ThermalLoadingParam["loadGuinierPrestonDistribution"][0],\
            extension=nugrodis.ThermalLoadingParam["loadGuinierPrestonDistribution"][1],\
            delimiter=";" ) # by default, delimiter is already ";"
            CppInitialRadDisDict[x]=RadiusDistribution(radStepGP,\
                                                        minRGP,\
                                                        numberOfClassGP)
            
            ## set all item value for all class
            classIdList=map(int,radDisDataGP[:,0])
            ItemValuesList=radDisDataGP[:,2]
            for classId, ItemValue in zip (classIdList,ItemValuesList): 
                CppInitialRadDisDict[x].SetItemValueForClass(classId,ItemValue)
   
            ## Info
            CppInitialRadDisDict[x].Info()
            
        if pyPrecipitateObj.nature == "Sprime":
            ## load Sprime initial Radius distribution
            radStepSprime,minSprime,numberOfClassSprime,radDisDataSprime=\
            ReadAndProcessRadiusDistributionFile(\
            path=nugrodis.ThermalLoadingParam["loadSprimeDistribution"][0],\
            extension=nugrodis.ThermalLoadingParam["loadSprimeDistribution"][1],\
            delimiter=";" ) # by default, delimiter is already ";"
            CppInitialRadDisDict[x]=RadiusDistribution(radStepSprime,\
                                                            minSprime,\
                                                            numberOfClassSprime)
 
            ### set all item value for all class
            classIdList=map(int,radDisDataSprime[:,0])
            ItemValuesList=radDisDataSprime[:,2]
            for classId, ItemValue in zip (classIdList,ItemValuesList): 
                CppInitialRadDisDict[x].SetItemValueForClass(classId,ItemValue)                                                        
            
            ## info                             
            CppInitialRadDisDict[x].Info()
    
#============================================================    
# END: initializing radius distribution
#============================================================        
    
        
    #============================================================    
    #Step1: Build A C++ object ChemicalComposition. necessary for building Precipitate CppObject
    #============================================================ 
    print("End of coversion######################################################################################")
    exec "CppChemicalComposition"+x+"=ChemicalComposition(pyPrecipitateObj.chemicalComposition[0])"  # e.g.  CppChemicalCompositionGP, CppChemicalCompositionSprime, ...
    print("End of concevrsion######################################################################################")
    exec "CppCompo=CppChemicalComposition"+x #just to be more understandable
    print("End of concevrsion######################################################################################")
    
    #============================================================ 
    #Step2 & Step3: Create Precipitates and Set specific properties: GuinierPreston , Sprime, and ....
    #============================================================ 
    if pyPrecipitateObj.nature == "GuinierPreston":
        #Create a GuinierPreston C++ object
        print("  > Building a C++ GuinierPreston object named : <"+x+">")
        CppPrecipitate = GuinierPreston(CppMaterial, CppCompo, CppInitialRadDisDict[x])
        CppPrecipitateDict[x] = CppPrecipitate 
        #============================================================ 
        #step3: Set specific GuinierPreston properties manually . (Or  with Precipitate.InitializeParameters() ? 
        #============================================================             
        CppPrecipitate.initialNucleationSitesNumber=pyPrecipitateObj.initialNucleationSitesNb[0]
    
    if pyPrecipitateObj.nature == "Sprime":
        #Create a Sprime C++ object
        print("  > Building a C++ Sprime object named         : <"+x+">")
        #exec "Cpp"+x+"=Sprime(CppMaterial,CppCompo,CppInitialRadDisDict[x],pyPrecipitateObj.wettingAngle[0])"
        CppPrecipitate = Sprime(CppMaterial,CppCompo,CppInitialRadDisDict[x],pyPrecipitateObj.wettingAngle[0])
        CppPrecipitateDict[x] = CppPrecipitate
        #============================================================ 
        #step3: Set Sprime specific properties manually or  with Precipitate.InitializeParameters(...)?
        #============================================================             
        CppPrecipitate.ComputeWettingFunction()
        #Sprime did not have any other specific properties than wetting Angle which have been already setted in constructor
        
    #============================================================     
    #step4: set common properties of GuinierPrecton, Sprime , ...
    #============================================================ 
    CppPrecipitate.solvusActivationEnergy = pyPrecipitateObj.solvusActivationEnergy[0]
    CppPrecipitate.preExpTermForSolvus = pyPrecipitateObj.preExpTermForSolvus[0]
    CppPrecipitate.shapeFactor = pyPrecipitateObj.shapeFactor[0]
    CppPrecipitate.molarVolume = pyPrecipitateObj.molarVolume[0]
    CppPrecipitate.deltaCell   = pyPrecipitateObj.deltaCell[0]
    CppPrecipitate.ComputeDistorsionEnergy()
    CppPrecipitate.ComputeNucleationSiteNb()
    #Set Precipitate Polynomial object
    polyDeg=len(pyPrecipitateObj.surfaceEnergyPolynomialModel[0])-1
    CppPrecipitate.SetSEPolynomialDegree(polyDeg)
    CppPrecipitate.AddSEPolynomialPyCoefs(pyPrecipitateObj.surfaceEnergyPolynomialModel[0])
    CppPrecipitate.Info()
    #Set precipitate chemical Composition
    CppCompoOfCppPrecipitate=CppPrecipitate.GetChemicalComposition() #getting ChemicalComposition C++ object of the C++ Precipitate
    CppCCP=CppCompoOfCppPrecipitate#just to be more accessible
    print("CC Cpp object of precipitate is: ",CppCCP)
    print("Precipitate Chemical Formulation: ",CppCCP.formula)
    PyCC=PyChemicalComposition(pyPrecipitateObj.chemicalComposition[0])
    print("pyPrecipitateObj.chemicalComposition[0]",pyPrecipitateObj.chemicalComposition[0])
        
    #Add "ALL" Cpp ChemicalElement inCpp ChemicalComposition of Cpp Precipitate
    #Ordered by alphabetic order. The First key which is not the main Element will be used when computing interfacial Conc
    for u in sorted(PyCC.composition.items(), key=lambda t: t[0]): # Previously for u in PyCC.composition.items(): 
        symbol=u[0]
        stoichiometry=u[1][0]
        print("Symbol", symbol)
        print("PyCC.composition.items()", PyCC.composition.items())
        #Add Cpp ChemicalElement in Cpp ChemicalComposition of Cpp Precipitate
        CppElementsDict[symbol].EnterInChemicalComposition(CppCCP)
    
    
    #=================================================   
    #   SETTING CONCENTRATION
    #================================================= 
        
#==============================================================================
#         #Method 1: Not working well, SETTING CONCENTRATION BY USING CONCENTRATION PyDict. 
#         print("PyCC.Composition.items:  ",PyCC.composition.items())
#         for u in PyCC.composition.items():
#             symbol=u[0]
#             stoichiometry=u[1][0]
#             print("pouet 2 bis")
#             CppConcentrationDict=CppCCP.GetConcentrationPyDict() #just to be more understandable. A dictionary of { "A chemical element symbol" : "Cpp object Concentration of the precipitate"}
#             print("pouet 2 bis bis")
#             #Set the stoichiometry of Cpp chemical Element in the Cpp Concentration for the Precipitate's Cpp ChemicalComposition
#             print("pouet 3")
#             CppConcentrationDict[symbol].stoichiometricCoef=stoichiometry
#             print("CppConc Setted stoichiometric coefs ",CppConcentrationDict[symbol].stoichiometricCoef)
#         #print("Python print stoichiomemetric coef. Bynding Get stoi coef",CppConcentrationDict[symbol].stoichiometricCoef)
#==============================================================================
        
    #Method 2: SETTING CONCENTRATION BY FINDING CONCENTRATION IN C++ Map using name.
    CppConcentrationDict={}#A dictionary of { "A chemical element symbol" : "Cpp object Concentration of the precipitate"}
    print("PyCC.Composition.items:  ",PyCC.composition.items())
    for u in PyCC.composition.items():
        symbol=u[0]
        stoichiometry=u[1][0]
        #Set the stoichiometry of Cpp chemical Element in the Cpp Concentration for the Precipitate's Cpp ChemicalComposition
        CppConcentrationDict[symbol]=CppCCP.GetConcentrationForElement(symbol) # add found concentration for element in concentration dictionary
        CppCCP.GetConcentrationForElement(symbol).stoichiometricCoef=stoichiometry
        print("CppConc Setted stoichiometric coefs ",CppCCP.GetConcentrationForElement(symbol).stoichiometricCoef)
    
    #=================================================   
    #   END OF SETTING CONCENTRATION
    #=================================================
    

    #CppCCP.GetChemicalElementsPyList() returns a python List of C++ ChemicalElement Objects from Precipitate ChemicalComposition C++ object
    print("Chemical Elements C++ Objects of <"+x+">  Precipitate Chemical Composition  are:",CppCCP.GetChemicalElementsPyList())
    print("Dictionary of Concentration C++ objects for precipitate <"+x+">: ",CppConcentrationDict)
    print("######################################################################################")

    print("Beginning of Conversion stoic into Atomic ")
    CppPrecipitate.ConvertStoichiometricCoefficientToAtomicConcentration()
    print("End of concevrsion######################################################################################")
    CppPrecipitate.ConvertAtomicToVolumicConcentration()

    #initialize precipitate equilibrium concentratations
    CppPrecipitate.InitializeEquilibriumConcentrationMap();
    
#################### TEST ; WORK IN PROGRESS  ########################
##################################################        
        


   ###debut Commenntaire: DECOMMENTEZ APRES     


# some stuff.... for Moubarak !

CppMaterial.Info()
CppMaterial.Info()
CppMaterial.GetSSGrain().GetChemicalComposition().Info()
CppMaterial.GetInitialChemicalComposition().Info()

print("Material test")
CppMaterial.test()
print("SSGrain test")
CppMaterial.GetSSGrain().test()

CppMaterial.GetInitialChemicalComposition().test()
CppMaterial.GetSSGrain().GetChemicalComposition().test()
CppMaterial.GetCurrentChemicalComposition().test()#if concentration in Material SSGrain is modified, it affects concentration of Material

CppPrecipitateDict["Sprime"].ComputeSurfaceEnergy()
print("Surface energy of <Sprime> value ==============================================",CppPrecipitateDict["Sprime"].surfaceEnergyCurrentValue)

CppPrecipitateDict["GP"].ComputeSurfaceEnergy()
print("Surface energy of <GP> value ==============================================",CppPrecipitateDict["GP"].surfaceEnergyCurrentValue)


#=================================================================================================
# Begin: compute and set the max computation time =  #Hardening.duration + ThermalLoading.duration
#=================================================================================================
c.ProcessMaxComputationTime()
#=================================================================================================
# End: compute and set the max computation time =  #Hardening.duration + ThermalLoading.duration
#=================================================================================================


##############  Begin: BOUCLE TEMPORELLE ########################
CppMaterial.RunProcess()#RunProcess run the main loop.
##############  End: BOUCLE TEMPORELLE ########################
c.Run()



##CppMaterial.UpdateVolumicValues()
##print("XvCuSS ",CppMaterial.GetCurrentChemicalComposition().GetConcentrationForElement("Cu").volumicValue)
##print("XvMgSS ",CppMaterial.GetCurrentChemicalComposition().GetConcentrationForElement("Mg").volumicValue)
##CppVacancy.ComputeDiffusionCoefValue()
##print("Dlac ",CppVacancy.vacancyDiffusionCoef)
##CppVacancy.ComputeEquilibriumConcentration()
##print("Xlaceq ",CppVacancy.equilibriumConc)



#test du solveur des concentrations d'equilibre####
##K=CppPrecipitateDict["GP"].preExpTermForSolvus
##DH=CppPrecipitateDict["GP"].solvusActivationEnergy
##R=PhysicalConstantsDict["R"][0]
##T=293.15
##
##f=K*exp(-DH/(R*T))
##print("f",f,K,DH,R,T)
##
##DMgOverDCu=(1.2*10**(-4))/(6.5*10**(-5))
##print("DMg/DCu",DMgOverDCu)
##XvSSCu=1.22/100
##XvPCu=7.02/100
##XvSSMg=2.08/100
##XvPMg=13.84/100
##XCueq=1.
##XMgeq=1.
##
##
##CppPrecipitateDict["GP"].SolveEquilibriumConcentrationsEquations(f,\
##                                                                 XvSSCu,\
##                                                                 XvPCu,\
##                                                                 DMgOverDCu,\
##                                                                 XvSSMg,\
##                                                                 XvPMg,\
##                                                                 XCueq,\
##                                                               XMgeq)
##print("XeqCu from python: ",XCueq)
##
##CppPrecipitateDict["GP"].ComputeEquilibriumConcentrations()
##print(CppPrecipitateDict["GP"].GetEquilibriumConcentrationForElement("Cu"))




#CppPrecipitateDict["GP"].GetEquilibriumConcentrationForElement("Mg");
#CppPrecipitateDict["GP"].GetEquilibriumConcentrationForElement("Cu");
#CppPrecipitateDict["Sprime"].GetEquilibriumConcentrationForElement("Cu");
#CppPrecipitateDict["Sprime"].ComputePhaseChangeVolumicEnergy()
#print("Phase change energy is ",CppPrecipitateDict["Sprime"].phaseChangeVolumiqueEnergy)





######## POSTPROCESSING, old method     #####
## script.OutputDistributionCurves(ComputationResultsDirectory+"/")
## script.OutputAttributes(ComputationResultsDirectory+"/")
## script.OutputMaterialChemicalComposition(ComputationResultsDirectory+"/")
## script.OutputMaterialVacancyProperties(ComputationResultsDirectory+"/")
#########################################################





# ###fin Commenntaire: DECOMMENTEZ APRES 










##========================================================================
##===BEGINING OF COPY=====================================================
##========================================================================
####  
#    
#    #HARDENING
#    if computation == "Hardening":
#        # Create a C++ object of type Hardening:
#        CppHardening=Hardening(nugrodis.HardeningParam["duration"][0], c)
#        CppHardening.Info()
#        
#        #InitialiszeGrains. Create C++ object SSGrain which belongs to the material,  and has the same properties as the material. TODO: call it InitializeSSGrain ?
#        CppMaterial.InitializeGrains()
#        #REMEMBER: if concentration in Material SSGrain is change, it affects concentration of Material, and vice versa
#
#        
#        #Create Precipitates C++ object  
#        CppPrecipitateDict     = {} # Dictionary of precipitates Names as keys with their C++ Precipitate Objects associated. {"precipitate Name": CppPrecipitateObject}
#        CppHardeningRadDisDict = {} # Dictionary of precipitates Names as keys with their C++ RadiusDistribution Objects associated. {"precipitate Name": CppRadiusDIstributionObject}
###        CppPolynomialDict={} #a dictionary of precipitates Names as keys with their C++ Polynomial Objects asssociated. {"precipitates Name": CppPolynomialObject}
#        for x in pyPD:
#            pyPrecipitateObj=pyPD[x]
#
#            # step0: Create a C++ object of type RadiusDistribution. Required for building Precipitate CppObject
#            CppHardeningRadDisDict[x]=RadiusDistribution(nugrodis.CellParam['spatialStep'][0],
#                                                         nugrodis.CellParam['minimumRadius'][0],
#                                                         nugrodis.CellParam['initialClassNumber'][0])
#            CppHardeningRadDisDict[x].Info()
#            
#            #Step1: Build A C++ object ChemicalComposition. necessary for building Precipitate CppObject
#            print("End of coversion######################################################################################")
#            exec "CppChemicalComposition"+x+"=ChemicalComposition(pyPrecipitateObj.chemicalComposition[0])"  # e.g.  CppChemicalCompositionGP, CppChemicalCompositionSprime, ...
#            print("End of concevrsion######################################################################################")
#            exec "CppCompo=CppChemicalComposition"+x #just to be more understandable
#            print("End of concevrsion######################################################################################")
#            
#            #Step2: Create Precipitates: GuinierPreston , Sprime, and ....
#            if pyPrecipitateObj.nature == "GuinierPreston":
#                #Create a GuinierPreston C++ object
#                print("  > Building a C++ GuinierPreston object named : <"+x+">")
#                CppPrecipitate = GuinierPreston(CppMaterial, CppCompo, CppHardeningRadDisDict[x])
#                CppPrecipitateDict[x] = CppPrecipitate 
#                #step3: Set specific GuinierPreston properties manually . (Or  with Precipitate.InitializeParameters() ? 
#                CppPrecipitate.initialNucleationSitesNumber=pyPrecipitateObj.initialNucleationSitesNb[0]
#            elif pyPrecipitateObj.nature == "Sprime":
#                #Create a Sprime C++ object
#                print("  > Building a C++ Sprime object named         : <"+x+">")
#                #exec "Cpp"+x+"=Sprime(CppMaterial,CppCompo,CppHardeningRadDisDict[x],pyPrecipitateObj.wettingAngle[0])"
#                CppPrecipitate = Sprime(CppMaterial,CppCompo,CppHardeningRadDisDict[x],pyPrecipitateObj.wettingAngle[0])
#                CppPrecipitateDict[x] = CppPrecipitate
#                #step3: Set Sprime specifice properties manually or  with Precipitate.InitializeParameters(...)?
#                CppPrecipitate.ComputeWettingFunction()
#                #Sprime did not have any other specific properties than wetting Angle which have been already setted in constructor
#            else:
#                print("Precipitate nature should be Guinier Preston or Sprime")
#                sys.exit(-1)
#
#            #step4: Set precipiatate ChemicalComposition
#            #CppPrecipitate.
#
#            #step5: set common properties of GuinierPrecton, Sprime , ...
#            CppPrecipitate.solvusActivationEnergy = pyPrecipitateObj.solvusActivationEnergy[0]
#            CppPrecipitate.preExpTermForSolvus = pyPrecipitateObj.preExpTermForSolvus[0]
#            CppPrecipitate.shapeFactor = pyPrecipitateObj.shapeFactor[0]
#            CppPrecipitate.molarVolume = pyPrecipitateObj.molarVolume[0]
#            CppPrecipitate.deltaCell   = pyPrecipitateObj.deltaCell[0]
#            CppPrecipitate.ComputeDistorsionEnergy()
#            CppPrecipitate.ComputeNucleationSiteNb()
#            #Set Precipitate Polynomial object
#            polyDeg=len(pyPrecipitateObj.surfaceEnergyPolynomialModel[0])-1
#            CppPrecipitate.SetSEPolynomialDegree(polyDeg)
#            CppPrecipitate.AddSEPolynomialPyCoefs(pyPrecipitateObj.surfaceEnergyPolynomialModel[0])
#            CppPrecipitate.Info()
#            #Set precipitate chemical Composition
#            CppCompoOfCppPrecipitate=CppPrecipitate.GetChemicalComposition() #getting ChemicalComposition C++ object of the C++ Precipitate
#            CppCCP=CppCompoOfCppPrecipitate#just to be more accessible
#            print("CC Cpp object of precipitate is: ",CppCCP)
#            print("Precipitate Chemical Formulation: ",CppCCP.formula)
#            PyCC=PyChemicalComposition(pyPrecipitateObj.chemicalComposition[0])
#            print("pyPrecipitateObj.chemicalComposition[0]",pyPrecipitateObj.chemicalComposition[0])
#
#            #Add "ALL" Cpp ChemicalElement inCpp ChemicalComposition of Cpp Precipitate
#            #Ordered by alphabetic order. The First key which is not the main Element will be used when computing interfacial Conc
#            for u in sorted(PyCC.composition.items(), key=lambda t: t[0]): # Previously for u in PyCC.composition.items(): 
#                symbol=u[0]
#                stoichiometry=u[1][0]
#                print("pouet 1")
#                print("Symbol", symbol)
#                print("PyCC.composition.items()", PyCC.composition.items())
#                #Add Cpp ChemicalElement in Cpp ChemicalComposition of Cpp Precipitate
#                CppElementsDict[symbol].EnterInChemicalComposition(CppCCP)
#                print("pouet 2")
#
##Method 1: SETTING CONCENTRATION BY USING CONCENTRATION PyDict. Not work well
###            print("PyCC.Composition.items:  ",PyCC.composition.items())
###            for u in PyCC.composition.items():
###                symbol=u[0]
###                stoichiometry=u[1][0]
###                print("pouet 2 bis")
###                CppConcentrationDict=CppCCP.GetConcentrationPyDict() #just to be more understandable. A dictionary of { "A chemical element symbol" : "Cpp object Concentration of the precipitate"}
###                print("pouet 2 bis bis")
###                #Set the stoichiometry of Cpp chemical Element in the Cpp Concentration for the Precipitate's Cpp ChemicalComposition
###                print("pouet 3")
###                CppConcentrationDict[symbol].stoichiometricCoef=stoichiometry
###                print("CppConc Setted stoichiometric coefs ",CppConcentrationDict[symbol].stoichiometricCoef)
###            #print("Python print stoichiomemetric coef. Bynding Get stoi coef",CppConcentrationDict[symbol].stoichiometricCoef)
#
##Method 2: SETTING CONCENTRATION BY FINDING CONCENTRATION IN C++ Map using name.
#            CppConcentrationDict={}#A dictionary of { "A chemical element symbol" : "Cpp object Concentration of the precipitate"}
#            print("PyCC.Composition.items:  ",PyCC.composition.items())
#            for u in PyCC.composition.items():
#                symbol=u[0]
#                stoichiometry=u[1][0]
#                print("pouet 2")
#                #Set the stoichiometry of Cpp chemical Element in the Cpp Concentration for the Precipitate's Cpp ChemicalComposition
#                print("pouet 3")
#                CppConcentrationDict[symbol]=CppCCP.GetConcentrationForElement(symbol) # add found concentration for element in concentration dictionary
#                CppCCP.GetConcentrationForElement(symbol).stoichiometricCoef=stoichiometry
#                print("CppConc Setted stoichiometric coefs ",CppCCP.GetConcentrationForElement(symbol).stoichiometricCoef)
#            
#            
#
#            #CppCCP.GetChemicalElementsPyList() returns a python List of C++ ChemicalElement Objects from Precipitate ChemicalComposition C++ object
#            print("Chemical Elements C++ Objects of <"+x+">  Precipitate Chemical Composition  are:",CppCCP.GetChemicalElementsPyList())
#            print("Dictionary of Concentration C++ objects for precipitate <"+x+">: ",CppConcentrationDict)
#            print("######################################################################################")
#
#            print("Beginning of Conversion stoic into Atomic ")
#            CppPrecipitate.ConvertStoichiometricCoefficientToAtomicConcentration()
#            print("End of concevrsion######################################################################################")
#            CppPrecipitate.ConvertAtomicToVolumicConcentration()
#
#            #initialize precipitate equilibrium concentratations
#            CppPrecipitate.InitializeEquilibriumConcentrationMap();
#
#            #Initialize material precipitate: make precipitates belong to the material or the SSGrain
#            
#            #b=CppCCP.GetGrainPointer()
#            #print("Grain to which is linked the CC")
#            #print(b)
#
###a=CppPrecipitate.GetChemicalComposition()
###print(a.formula+" AAAAAAAAAAAAAAAA")
###aCCL=a.GetChemicalElementsPyList()
###print("last CppPrecipitate CC")
###print("aCCL",aCCL)
####CppElementsDict["Cu"].chemicalCompositionList()
###print("CppChemicalCompositionGP.GetChemicalElementsPyList()",CppChemicalCompositionGP.GetChemicalElementsPyList())
###CppElementsDict["Cu"].EnterInChemicalComposition(CppChemicalCompositionGP)
###CppElementsDict["Cu"].EnterInChemicalComposition(CppChemicalCompositionSprime)
###print(CppChemicalCompositionGP.GetChemicalElementsPyList())
###print(CppChemicalCompositionGP.GetChemicalElementsPyList()[0].elementName)
###print(InitialTempCpp.GetTemperaturePyList())
###print("CppPrecipitates CC")
###print("aCCL",aCCL)
###print("CppChemicalCompositionSprime.GetChemicalElementsPyList()",CppChemicalCompositionSprime.GetChemicalElementsPyList())
#
#
###    THERMAL LOADING
#    if computation == "ThermalLoading":
#        ## initialize thermalLoading temperature        
#        
#        
#        ##Create an object of type ThermalLoading:
#        CppThermalLoading=ThermalLoading(c)
#        CppThermalLoading.Info()
#        
#        #InitialiszeGrains. Create C++ object SSGrain which belongs to the material,  and has the same properties as the material. TODO: call it InitializeSSGrain ?
#        CppMaterial.InitializeGrains()
#        #REMEMBER: if concentration in Material SSGrain is change, it affects concentration of Material, and vice versa        
#
#        #Create Precipitates C++ object  
#        CppPrecipitateDict     = {} # Dictionary of precipitates Names as keys with their C++ Precipitate Objects associated. {"precipitate Name": CppPrecipitateObject}
#        CppThermalLoadingRadDisDict = {} # Dictionary of precipitates Names as keys with their C++ RadiusDistribution Objects associated. {"precipitate Name": CppRadiusDIstributionObject}
#
#
##toto=PyChemicalComposition("Al2Cu8")
##print(toto)
##print(toto.composition)
#
## some stuff.... for Moubarak !


##========================================================================
##===END OF COPY==========================================================
##========================================================================



#==================================== old version ===================================================
#c.maxComputationTime=CppHardening.duration #CppHardening.duration + CppThermalLoading.duration
###############  TEST DE LA BOUCLE TEMPORELLE ########################
#CppMaterial.RunProcess()#RunProcess run the main loop.
###############  FIN DE TEST DE LA BOUCLE TEMPORELLE ########################
#c.Run()
#==================================== old version ===================================================
