# -*- coding: utf-8 -*-

#recurrent bugs Section:
#extension class wrapper for base class Precipitate has not been created yet: swap the order of the class_ statements, base class before derived class



from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")

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




print ("|####################################################################################################|")
print ("|######################################  RUNNING FILE <run.py> ######################################|")
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
ED = PyChemicalElement.GetElementsDict() #Mendeleiv  Table : a dictionary of chemichal Elements Names as keys with their chemicalElement python Objects associated. {"chemichal Elements Name": chemicalElementPyObject}

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
    exec "print ("+CppElement+")"
print("  > C++ Elements Dictionary = ",CppElementsDict)

#Create a C++ object ChemicalComposition: Initial Chemical Composition. This obect will be give to Cpp material 
InitialCCCpp=ChemicalComposition("solide solution");
#Add "ALL" Cpp ChemicalElement in Cpp object Initial ChemicalComposition
for u in AlloyInitialComposition.items():
    symbol=u[0]
    initialVolumicConcentration=u[1][0]
    print("pouet 1")
    #Add Cpp ChemicalElement in Cpp in Cpp object Initial ChemicalComposition (InitialCCCpp)
    CppElementsDict[symbol].EnterInChemicalComposition(InitialCCCpp)
    print("pouet 2")
#SETTING Cpp CHEMICAL COMPOSITION'S CONCENTRATION BY FINDING CONCENTRATION IN C++ Map using name.
    print("pouet 3")
    #Set the volumic concentration of Cpp chemical Element in the Cpp Concentration for Cpp object Initial ChemicalComposition
    InitialCCCpp.GetConcentrationForElement(symbol).volumicValue=initialVolumicConcentration
    InitialCCCpp.mainElementName=mainEl
    print("CppConc Setted volumic volumic concentration ",InitialCCCpp.GetConcentrationForElement(symbol).volumicValue)

#reading Temperature parameters and Create a C++ object of type Temperature: Initial Temperature
TempParam=nugrodis.TemperatureParam
print("  > Found TemperatureParam           : ", TempParam)
InitialTempCpp=Temperature(TempParam['T0'][0]);


#Create a C++ object of type Material
CppMaterial=Material(InitialTempCpp,
                     CppElementsDict[mainEl],
                     InitialCCCpp);
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
CppMaterial.ConvertVolumicToInitialAtomicConcentration()#Convert and set initialAtomicValues
CppMaterial.ConvertVolumicToInitialMassicConcentration()#Convert and set initialMassicValues
    


#Reading Vacancies Parameters and Create a Cpp object Vacancy wich will be automatically setted as the material's vacancy. Remember that creating a vacancy Object  with a given material as argument, also set this vacancy as the material's vacancy
exec "VacParam="+material+".VacanciesParam"
print("  > Found dico VacancyParam           : ", VacParam)
exec "CppVacancy= Vacancy("+material+".VacanciesParam['deltaHF'][0],\
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

#after setting materia
#Search for DiffusionParam for solute chemicalElements ( any element different from the main chemical Element)
#Read dictionaries which have a nature of  DiffusionParam
CppDiffusionDict={} #a dictionary of Chemical Elements names as keys with their C++ diffusion Objects associated. {"ChemicalElementSymbol": diffusionCppObject}
for e in L: 
    if e[0] is not '_':
        exec 'E = '+material+'.'+e
        if type(E)==dict :
            nature = E.get('nature', None)
            if nature is 'DiffusionParam':
                assert (  (E.get('EVac', 'missing')!='missing' and E.get('preExpDiffusionCoef', 'missing')!='missing' and E.get('Q', 'missing')!='missing') ),\
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
    #QUENCHING
    if computation == "Quenching":
        # Create an object of type Quenching:
        exec "Quenching = Quenching("+material+".VacanciesParam['Tsol'][0],\
                              nugrodis.QuenchingParam['Tfinal'][0],\
                              nugrodis.QuenchingParam['v'][0])"
        Quenching.Info();
    #HARDENING
    if computation == "Hardening":
        # Create a C++ object of type Hardening:
        CppHardening=Hardening(nugrodis.HardeningParam["duration"][0],
                            nugrodis.HardeningParam["initialTimeStep"][0])
        CppHardening.Info()

        
        #InitialiszeGrains. Create C++ object SSGrain which belongs to the material,  and has the same properties as the material. TODO: call it InitializeSSGrain ?
        CppMaterial.InitializeGrains()
        #REMEMBER: if concentration in Material SSGrain is change, it affects concentration of Material, and vice versa

        
        

        
        #Create a C++ object of type RadiusDistribution
        CppHardeningRadDis=RadiusDistribution(nugrodis.CellParam['spatialStep'][0],
                          nugrodis.CellParam['minimumRadius'][0],
                          nugrodis.HardeningParam['initialClassNumber'][0])
        CppHardeningRadDis.Info();
        #Create Precipitates C++ object  
        CppPrecipitateDict={} #a dictionary of precipitates Names as keys with their C++ Precipitate Objects associated. {"precipitate Name": CppPrecipitateObject}
##        CppPolynomialDict={} #a dictionary of precipitates Names as keys with their C++ Polynomial Objects asssociated. {"precipitates Name": CppPolynomialObject}
        for x in pyPD:
            pyPrecipitateObj=pyPD[x]
            #Step1: Build A C++ object ChemicalComposition. necessary for building Precipitate CppObject
            print("End of concevrsion######################################################################################")
            exec "CppChemicalComposition"+x+"=ChemicalComposition(pyPrecipitateObj.chemicalComposition[0])"  # e.g.  CppChemicalCompositionGP, CppChemicalCompositionSprime, ...
            print("End of concevrsion######################################################################################")
            exec "CppCompo=CppChemicalComposition"+x #just to be more understandable
            print("End of concevrsion######################################################################################")
            #Step2: Create Precipitates: GuinierPreston , Sprime, and ....
            if pyPrecipitateObj.nature=="GuinierPreston":
                #Create a GuinierPreston C++ object
                print("  > Building a C++ GuinierPreston object named : <"+x+">")
                CppPrecipitate=GuinierPreston(CppMaterial,CppCompo,CppHardeningRadDis)
                CppPrecipitateDict[x]=CppPrecipitate 
                #step3: Set specific GuinierPreston properties manually . (Or  with Precipitate.InitializeParameters() ? 
                CppPrecipitate.nucleationSitesNumber=pyPrecipitateObj.initialNucleationSitesNb[0]
            #Step2: Create Precipitates: GuinierPreston , Sprime, and ....
            elif pyPrecipitateObj.nature=="Sprime":
                #Create a Sprime C++ object
                print("  > Building a C++ Sprime object named         : <"+x+">")
               #exec "Cpp"+x+"=Sprime(CppMaterial,CppCompo,CppHardeningRadDis,pyPrecipitateObj.wettingAngle[0])"
                CppPrecipitate=Sprime(CppMaterial,CppCompo,CppHardeningRadDis,pyPrecipitateObj.wettingAngle[0])
                CppPrecipitateDict[x]=CppPrecipitate
                #step3: Set Sprime specifice properties manually or  with Precipitate.InitializeParameters(...)?
                #Sprime did not have any other specific properties than wetting Angle which have been already setted in constructor
            else:
                print("Precipitate nature should be Guinier Preston or Sprime")
                sys.exit(-1)
            #step4: Set precipiatate ChemicalComposition
            #CppPrecipitate.
            #step5: set common properties of GuinierPrecton, Sprime , ...
            CppPrecipitate.solvusActivationEnergy=pyPrecipitateObj.solvusActivationEnergy[0]
            CppPrecipitate.preExpTermForSolvus=pyPrecipitateObj.preExpTermForSolvus[0]
            CppPrecipitate.shapeFactor=pyPrecipitateObj.shapeFactor[0]
            CppPrecipitate.molarVolume=pyPrecipitateObj.molarVolume[0]
            CppPrecipitate.deltaCell=pyPrecipitateObj.deltaCell[0]
            CppPrecipitate.ComputeDistorsionEnergy()
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

            #Add "ALL" Cpp ChemicalElement inCpp ChemicalComposition of Cpp Precipitate
            for u in PyCC.composition.items():
                symbol=u[0]
                stoichiometry=u[1][0]
                print("pouet 1")
                #Add Cpp ChemicalElement in Cpp ChemicalComposition of Cpp Precipitate
                CppElementsDict[symbol].EnterInChemicalComposition(CppCCP)
                print("pouet 2")

#Method 1: SETTING CONCENTRATION BY USING CONCENTRATION PyDict. Not work well
##            print("PyCC.Composition.items:  ",PyCC.composition.items())
##            for u in PyCC.composition.items():
##                symbol=u[0]
##                stoichiometry=u[1][0]
##                print("pouet 2 bis")
##                CppConcentrationDict=CppCCP.GetConcentrationPyDict() #just to be more understandable. A dictionary of { "A chemical element symbol" : "Cpp object Concentration of the precipitate"}
##                print("pouet 2 bis bis")
##                #Set the stoichiometry of Cpp chemical Element in the Cpp Concentration for the Precipitate's Cpp ChemicalComposition
##                print("pouet 3")
##                CppConcentrationDict[symbol].stoichiometricCoef=stoichiometry
##                print("CppConc Setted stoichiometric coefs ",CppConcentrationDict[symbol].stoichiometricCoef)
##            #print("Python print stoichiomemetric coef. Bynding Get stoi coef",CppConcentrationDict[symbol].stoichiometricCoef)

#Method 2: SETTING CONCENTRATION BY FINDING CONCENTRATION IN C++ Map using name.
            CppConcentrationDict={}#A dictionary of { "A chemical element symbol" : "Cpp object Concentration of the precipitate"}
            print("PyCC.Composition.items:  ",PyCC.composition.items())
            for u in PyCC.composition.items():
                symbol=u[0]
                stoichiometry=u[1][0]
                print("pouet 2")
                #Set the stoichiometry of Cpp chemical Element in the Cpp Concentration for the Precipitate's Cpp ChemicalComposition
                print("pouet 3")
                CppConcentrationDict[symbol]=CppCCP.GetConcentrationForElement(symbol) # add found concentration for element in concentration dictionary
                CppCCP.GetConcentrationForElement(symbol).stoichiometricCoef=stoichiometry
                print("CppConc Setted stoichiometric coefs ",CppCCP.GetConcentrationForElement(symbol).stoichiometricCoef)
            
            

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

        #Initialize material precipitate: make precipitates belong to the material or the SSGrain

            
            
            
            #b=CppCCP.GetGrainPointer()
            #print("Grain to which is linked the CC")
            #print(b)


##a=CppPrecipitate.GetChemicalComposition()
##print(a.formula+" AAAAAAAAAAAAAAAA")
##aCCL=a.GetChemicalElementsPyList()
##print("last CppPrecipitate CC")
##print("aCCL",aCCL)
###CppElementsDict["Cu"].chemicalCompositionList()
##print("CppChemicalCompositionGP.GetChemicalElementsPyList()",CppChemicalCompositionGP.GetChemicalElementsPyList())
##CppElementsDict["Cu"].EnterInChemicalComposition(CppChemicalCompositionGP)
##CppElementsDict["Cu"].EnterInChemicalComposition(CppChemicalCompositionSprime)
##print(CppChemicalCompositionGP.GetChemicalElementsPyList())
##print(CppChemicalCompositionGP.GetChemicalElementsPyList()[0].elementName)
##print(InitialTempCpp.GetTemperaturePyList())
##print("CppPrecipitates CC")
##print("aCCL",aCCL)
##print("CppChemicalCompositionSprime.GetChemicalElementsPyList()",CppChemicalCompositionSprime.GetChemicalElementsPyList())


##    THERMAL LOADING
##    if computation == "ThermalLoading":
##        #Create an object of type ThermalLoading:
##        ThermalLoading=thermalLoading()

#toto=PyChemicalComposition("Al2Cu8")
#print(toto)
#print(toto.composition)

# some stuff.... for Moubarak !



CppMaterial.Info()
CppMaterial.GetSSGrain().volNbGP=4
CppMaterial.Info()
CppMaterial.GetSSGrain().GetChemicalComposition().Info()
CppMaterial.GetInitialChemicalComposition().Info()


print('##############################################################################5674555555555555555555555555555555555555555')
toto=ChemicalComposition("Al8Cu4")
toto2=toto.Clone()
toto3=CppChemicalCompositionSprime.Clone()
toto3.GetGrainPointer()
toto3.GetMaterialPointer()
toto3.GetConcentrationForElement("Al")
CppChemicalCompositionSprime.GetGrainPointer()
CppChemicalCompositionSprime.GetMaterialPointer()
CppChemicalCompositionSprime.GetConcentrationForElement("Al")
print('##############################################################################5674555555555555555555555555555555555555555')



print("Material test")
CppMaterial.test()
print("SSGrain test")
CppMaterial.GetSSGrain().test()

CppMaterial.GetInitialChemicalComposition().test()
CppMaterial.GetSSGrain().GetChemicalComposition().test()
CppMaterial.GetCurrentChemicalComposition().test()#if concentration in Material SSGrain is change, it affects concentration of Material

CppPrecipitateDict["Sprime"].ComputeSurfaceEnergy()
print("Surface energy of <Sprime> value ==============================================",CppPrecipitateDict["Sprime"].surfaceEnergyCurrentValue)

CppPrecipitateDict["GP"].ComputeSurfaceEnergy()
print("Surface energy of <GP> value ==============================================",CppPrecipitateDict["GP"].surfaceEnergyCurrentValue)


#CppPrecipitateDict["GP"].GetEquilibriumConcentrationForElement("Mg");
#CppPrecipitateDict["GP"].GetEquilibriumConcentrationForElement("Cu");
#CppPrecipitateDict["Sprime"].GetEquilibriumConcentrationForElement("Cu");
#CppPrecipitateDict["Sprime"].ComputePhaseChangeVolumicEnergy()
#print("Phase change energy is ",CppPrecipitateDict["Sprime"].phaseChangeVolumiqueEnergy)




c.Run()




# éééé
