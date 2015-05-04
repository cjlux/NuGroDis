#for x in ChemicalElements.keys():
#    print(x)
#    print(ChemicalElements[x]["density"][0])

from __future__ import division, print_function
from MetalUtils.PhysicalConstants import Dict as PhysicalConstantsDict
from MetalUtils.ElementsData import Dict as ElementsDataDict

NuGroDisVersion="1.0"  

ComputationParam={"Type": "Quenching>Hardening", #Quenching,Hardening,ThermalLoading or mix(example: Quenching>Hardening)
                                                 #Hardening=maturation or tempering
                  "Material": "M2024" } 

TemperatureParam ={ 
"T0" : (20,"Celsius degree")#Initial temperature, [Celsius degree] 
}

QuenchingParam={
"v": (50,"Celsius degree.s^-1"), # v is the cooling rate
"totIterationNumber":(None,""), # user can replace None by a value which must be superior to a computed value ... , unitless
"Tfinal":(20,"Celsius degree") # Final temperature
#Tsol is used but is given by vacanciesParam dictionary in material dataFile
}

HardeningParam={
"duration": (150000,"s"), # hardening duration
"initialClassNumber" : (20, ""), # Initial number for hardening radiusDistribution Class , unitless. 
"initialTimeStep" : (1,"s") #Time step for first iteration
}

ThermalLoadingParam={
"loadProfile": ("TK1.xls","xls"), # unit can be xls,txt,csv means read File. else, it received data from socket and user must write --> (None,None) 
"loadGPDistribution": ("NP1.xls","xls"),# unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
"loadSprimeDistribution": ("NP2.xls","xls") # unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
#Do not forget to compute lenght,duration, ... after reading file NP1,NP2, TK1
}


TimeParam ={

}

InitialGlobalData={
}


################################################################################################################################# 
########################################      RADIUS DISTRIBUTION PARAM       ###################################################
#################################################################################################################################
CellParam ={
"spatialStep"  : (10E-10,"m") , #Spatial discretisation step
"initialClassNumber" : (20, ""), # Initial number for radiusDistribution Class, unitless. This is the default value use when an radiusDistribution object is instanciate
}  #avant : (4* CellParam["spatialStep"],"m")
CellParam["minimumRadius"] = (4*CellParam["spatialStep"][0],CellParam["spatialStep"][1]) #not useful in cellParam ???? argument for object RadiusDiustribution contructor or not????
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~ 


################################################################################################################################# 
##################################################      VACANCIES PARAMETERS       ###############################################
#################################################################################################################################
 
VacanciesParam={
"deltaHF": (72400,"J.mol^-1"), # Enthalpy of vacancy formation 
"deltaHM":(62400,"J.mol^-1"), #Enthalpy of vacancy migration 
"deltaSF": (17,"J.mol^-1.K^-1"), #Entropy of vacancy formation
"fE": (10**13,"s^-1"), #Frequency of vacancy's jumps
"halfSinkD": (30E-6,"m") #Semi-distance between vacancy sinks
}
Al=ElementsDataDict["Al"]
VacanciesParam["Dlac0"]= (VacanciesParam["fE"][0]*Al["cellSize"][0]**2, Al["cellSize"][1]+"."+VacanciesParam["fE"][1])# Preexponential term value of vacancy diffusion expression. 
                   # Once upon a time... (^_^)  : "Dlac0": (Al["cellSize"]**2,"m^2.s^-1")

#FIND attribute Tsol(solutionizing temp or initial quenching temp) in module Material.py (example M2024.py) in dictionary VacanciesParam
                                                                                                                      
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~ 
                                                                                                                     
                                                                            
 

