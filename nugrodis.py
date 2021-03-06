# -*- coding: utf-8 -*-
#for x in ChemicalElements.keys():
#    print(x)
#    print(ChemicalElements[x]["density"][0])

from __future__ import division, print_function
from MetalUtils.PhysicalConstants import Dict as PhysicalConstantsDict
from MetalUtils.ElementsData import Dict as ElementsDataDict

import os 
dir_path = os.path.dirname(os.path.realpath(__file__))

NuGroDisVersion="1.0"


#savePath=None  or savePath=""

#savePath="Tk1_NoHardeningBefore_L_hardening3.5e-6_L_thermalLoading_3.5e-6_gammaGP_0.0315_gammaSprime_Origin_withVacancyConcentrationAfterHardening_new" # if it None or "", it means default save path

#savePath="Hardening200000_TK1_gammaGP3.5e-6_gammaSprime_Origin_new" # if it None or "", it means default save path

# NotInteractionWith_Hardening_200000_gammaGP0.0485_L3.5_NotInteractionWithSolute

# "NotInteractionWith_ThermalLoadingTK1_200000_gammaGP0.048_L4_NotInteractionWithSolute_SprimeGamma_origin"


# NotInteractionWith_Hardening_200000_gammaGP0.044_L2.8_NotInteractionWithSolute


#savePath= "NotInteractionWith_ThermalLoadingTK1_gammaGP0.044_L2.8_NotInteractionWithSolute_SprimeGamma_origin"


savePath="VALIDATION_TESTS/cycle_218_start_at_20/halfsinkD_8nm/"

ComputationParam={            "Type": "ThermalLoading", #Quenching,Hardening,ThermalLoading or mix(example: Quenching>Hardening)
                                                 #Hardening=maturation or tempering
                          "Material": "M2024",
                  "initialTimeStep" : (1,"s") #default Time step for the first iteration 
                  } 

#======== obsolete now. hardening temperature is now define in HardeningParam
#TemperatureParam ={ 
#"T0" : (293.15,"K")
#}
#===========

QuenchingParam={
"v": (50,"Celsius degree.s^-1"), # v is the cooling rate
"totIterationNumber":(None,""), # user can replace None by a value which must be superior to a computed value ... , unitless
"Tfinal":(20,"Celsius degree") # Final temperature
#Tsol (solutionizing temp or initial quenching temp) is used but is given by vacanciesParam dictionary in material dataFile. 
#FIND attribute Tsol in module Material.py (example M2024.py) in dictionary VacanciesParam
}

HardeningParam={
"duration": (200000,"s"), # hardening duration
"temperature": (293.15,"K") #hardening temperature  #296.39 is Tk1_T0, ie , T0_TK1 = 296.39
#"initialClassNumber" : (20, "") # Initial number for hardening radiusDistribution Class , unitless.
#"initialClassNumber" : (117, "") # Initial number for hardening radiusDistribution Class , unitless. 
}

#initialRadiusDistribution={
#"nature":"Hardening",
#"initialClassNumber" : (20, ""), # Initial number for hardening radiusDistribution Class , unitless. 
#"initialTimeStep" : (1,"s"), #Time step for first iteration
#"initialRadiusDistribution":("True","")# .If true, means user Hardening is processed before  unitless
#}


ThermalLoadingParam={
#==============================================================================
# "loadProfile": ("TK1","xls"), # unit can be xls,txt,csv means read File. else, it received data from socket and user must write --> (None,None) 
# "loadGuinierPrestonDistribution": ("NP1","xls"),# unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
# "loadSprimeDistribution": ("NP2","xls") # unit can be xls,txt,csv means read File. else, it received data from a previous hardening processing and user must write --> (None,None)
#==============================================================================
#Do not forget to compute lenght,duration, ... after reading file NP1,NP2, TK1

##### Just For example #########
"loadProfile": (dir_path+"/fichiers_Temperature/cycle_218C_convertInKelvin_startAt20","csv"),
"loadGuinierPrestonDistribution": (dir_path+'/NotInteractionWith_Hardening_200000_gammaGP0.044_L2.8_NotInteractionWithSolute__Tue_Nov_22_17hr_27min_50s_2016/RadDisFiles/GuinierPreston/RadDis_time_200000_','txt'),
"loadSprimeDistribution": (dir_path+'/NotInteractionWith_Hardening_200000_gammaGP0.044_L2.8_NotInteractionWithSolute__Tue_Nov_22_17hr_27min_50s_2016/RadDisFiles/Sprime/RadDis_time_200000_','txt'),
"maximumTimeStep": (1,"s"), #the maximum time step allowed during the thermalLoading
"initialVacancyConcentration": (1.03806E-12,"SI") # to have lambda=1, because XlacEq = 1.03806E-12 , at end of  293.15 K


#"initialVacancyConcentration": (1.43089e-07,"SI") # can be vacancy concentration at end of hardening or quenching
#                                                 # is usefull when thermalLoading is the first sequence


#"initialVacancyConcentration": (1.03806E-12,"SI") # to have lambda=1, because XlacEq = 1.03806E-12 , at end of  293.15 K
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
"spatialStep"  : (1E-10,"m") , #Spatial discretisation step
#"spatialStep"  : (1E-11,"m") , #n=122
"initialClassNumber" : (20, "")# Initial number for radiusDistribution Class, unitless. This is the default value use when an radiusDistribution object is instanciate
}  #avant : (4* CellParam["spatialStep"],"m")
CellParam["minimumRadius"] = (4*CellParam["spatialStep"][0],CellParam["spatialStep"][1]) #not useful in cellParam ???? argument for object RadiusDiustribution contructor or not????
#CellParam["minimumRadius"] = (1E-10,CellParam["spatialStep"][1])
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~ 
 
                                                                                                                     
                                                                            
 

