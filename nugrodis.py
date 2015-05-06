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
#Tsol (solutionizing temp or initial quenching temp) is used but is given by vacanciesParam dictionary in material dataFile. 
#FIND attribute Tsol in module Material.py (example M2024.py) in dictionary VacanciesParam
}

HardeningParam={
"duration": (150000,"s"), # hardening duration
"initialClassNumber" : (20, ""), # Initial number for hardening radiusDistribution Class , unitless. 
"initialTimeStep" : (1,"s") #Time step for first iteration
}

#initialRadiusDistribution={
#"nature":"Hardening",
#"initialClassNumber" : (20, ""), # Initial number for hardening radiusDistribution Class , unitless. 
#"initialTimeStep" : (1,"s"), #Time step for first iteration
#"initialRadiusDistribution":("True","")# .If true, means user Hardening is processed before  unitless
#}


ThermalLoadingParam={
"loadProfile": ("TK1.xls","xls"), # unit can be xls,txt,csv means read File. else, it received data from socket and user must write --> (None,None) 
"loadGPDistribution": ("NP1.xls","xls"),# unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
"loadSprimeDistribution": ("NP2.xls","xls") # unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
#Do not forget to compute lenght,duration, ... after reading file NP1,NP2, TK1
}

#initialRadiusDistribution={
#"nature":"ThermalLoadingParam",
#"loadGPDistribution": ("NP1.xls","xls"),# unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
#"loadSprimeDistribution": ("NP2.xls","xls") # unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
##Do not forget to compute lenght,duration, ... after reading file NP1,NP2, TK1
#}


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
 
                                                                                                                     
                                                                            
 

