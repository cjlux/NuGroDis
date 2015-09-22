# -*- coding: utf-8 -*-

################################################################################################################################# 
##################################################       ALLOY 2024 DATA       ##################################################
#################################################################################################################################  


#for e in dir(M2024):
#	exec "if type(M2024."+e+")==dict:print(M2024."+e+",e)";

from __future__ import division, print_function
from MetalUtils.ElementsData import Dict as ElementsDataDict
from MetalUtils.ElementsData import ComputeMolarVolume
from MetalUtils.Mendeleiev import PyChemicalComposition
import Utils
import math
#ALL CONCENTRATIONS MUST BE THOSE AFTER SOLUTIONIZING

AlloyInitialComposition={"Al":(96.7/100,"m^3.m^-3"),# Aluminium initial concentration, in range [0,1]
                  "Cu":(1.22/100,"m^3.m^-3"), # Copper initial concentration, in range [0,1]
                  "Mg":(2.08/100,"m^3.m^-3")# Magnesium initial concentration, in range [0,1]
    }#Unit can be m^3.m^-3 (volumic) or atom.atom^-1 (atomic) or kg.kg^-1 (massic).



#INITIAL CONCENTRATION!!!!!!!!!!!!!!!!!!===>    #Question: where to find it in UMl diagram      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                           #Answer: |-->Find In C++ Class Concentration, attribute volumicValue 


################################################################################################################################# 
##################################################      VACANCIES PARAMETERS       ###############################################
#################################################################################################################################
 
VacanciesParam={
"deltaHF": (72240,"J.mol^-1"), # Enthalpy of vacancy formation 
"deltaHM":(62400,"J.mol^-1"), #Enthalpy of vacancy migration 
"deltaSF": (17,"J.mol^-1.K^-1"), #Entropy of vacancy formation
"fE": (10**13,"s^-1"), #Frequency of vacancy's jumps
"halfSinkD": (30E-6,"m"), #Semi-distance between vacancy sinks
"Tsol": (495+273.15,"K"), #solutionazingTemp=InitialQuenching Temperature, [°K]
#"Tsol": (495,"Celsius degree"), #solutionazingTemp=InitialQuenching Temperature, [°C]
"coordinationNumber": (12,"")#Coordination Number, is the number of near neighbours. For Aluminium structure FCC, it is 12. 
#If coordinationNumber is not given, interaction with ChemicalElements between chemicalElement and vacancies will not be taken into account.
#means alpha in Xlaceq will be equal to 1!!!
}
Al=ElementsDataDict["Al"]
VacanciesParam["Dlac0"]= (VacanciesParam["fE"][0]*Al["cellSize"][0]**2, Al["cellSize"][1]+"."+VacanciesParam["fE"][1])# Preexponential term value of vacancy diffusion expression. 
                   # Once upon a time... (^_^)  : "Dlac0": (Al["cellSize"]**2,"m^2.s^-1")
                                                                                                                      
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~





##################################################   Diffusion DATA     ##########################################################
Cu={
             "nature":"DiffusionParam", #Important                  
"preExpDiffusionCoef": (6.5E-5,"m^2.s^-1"), #Atomic diffusion: initial diffusion value for copper, pre-exponential diffusion coeffcient. Mandatory!               
                  "Q": (135000,"J.mol^-1"), #Atomic diffusion: Activation energy of diffusion for copper.  Mandatory!
               "EVac": (19264,"J.mol^-1")   #Vacancy diffusion: Copper-Vacancy interaction energy.CHECK!. Optional: If this parameter is not needed, user must write :==> (None,None)   

#WARNING: all these keys are mandatory. In case of the key "Evac", If  user don't want to use it, he must give value :==> (None,None)
#WARNING: User must give at least AtomicDiffusion(Q & preExpDiffusionCoef) .
  
}

Mg={        
             "nature":"DiffusionParam", #Important  
"preExpDiffusionCoef": (1.2E-4,"m^2.s^-1"), #initial diffusion value for magnesium, pre-exponential diffusion coeffcient                  
                  "Q": (131000,"J.mol^-1"), #Activation energy of diffusion for magnesium
               "EVac": (19264,"J.mol^-1") #magnesium-Vacancy interaction energy, #CHECK. If this parameter is not needed, user must write :==> (None,None)

#WARNING: User must give at least AtomicDiffusion(Q & preExpDiffusionCoef) . He can also give both
}




################################################## Guinier-Preston DATA ##########################################################
GP={
"nature":"GuinierPreston", #Important nature can be:  "GuinierPreston" or "Sprime". key words can be found in MetalUtils/Grain.py PrecipitateNatureList
"chemicalComposition": ("Al8Cu1Mg1","")  , #Chemical composition, will be used to find stoichiometric coefs and then compute volumic concentration of elements
"initialNucleationSitesNb":(6.02214E28,"at.m^-3")  , # initial Number of nucleation sites at beginning of hardening, was Ns0P1. Is assumed to be the initial number of aluminium atoms/volum unit
"preExpTermForSolvus":(0.992,"m^6.m^-6"),  # was KP1
"molarVolume":(1E-4 ,"m^3.mol^-1"),#volume of one mole of precipitates,
"solvusActivationEnergy":(36000,"J.mol^-1"), # DeltaHP1
"surfaceEnergyPolynomialModel":([0.044],"J.m^-2"), #  for example, if degree of polynom is 3:   ([a0,a1,a2,a3],"J.m^-2")
"shapeFactor":(0,"m.m^-1"),   #ratio between lenght to radius. If nul, shape is spherical. was alphaP
"deltaCell":(1/100,"m.m^-1") , # difference between precipitate and solid solution cell size , was deltamailleP ( for example, = |a_ss-a_precipitate|/a_precipitate)
#REMEMBER: At the moment, molar volume of GuinierPreston precipitates must be define by the user. But, subsequently, it can be computed automatically taking into account the structure of the precipitate.  
}


##################################################    Sprime DATA     ##########################################################
Sprime={
"nature":"Sprime", #important nature can be:  "GuinierPreston" or "Sprime" . Key words can be found in MetalUtils/Grain.py PrecipitateNatureList
"chemicalComposition": ("Al2CuMg","")  , #Chemical composition, will be used to find stoichiometric coefs and then compute volumic concentration of elements
"preExpTermForSolvus":(49.746,"m^6.m^-6"),  # was KP2
"solvusActivationEnergy":(76000,"J.mol^-1"), #DeltaHP2
"molarVolume":(4E-5 ,"m^3.mol^-1"), # volume of one mole of precipitates, In UML diagram find attribute corresponding to "molarVolume" in class SSGrain
                                    #As commented by many authors, Al2CuMg has an orthorhombic structure with 16 atoms ==> (4*Al2CuMg) per unit cell (a=0,4nm, b=0,923 nm, and c=0,714 nm)
                                    #V_moleculeAl2CuMg=V_unitCell/4 , then V_mAl2CuMg which is the volume of 1 mole of molecules Al2CuMg is expressed as follow:  
                                    #V_mAl2CuMg= Na*V_moleculeAl2CuMg  =  3.9686*10^(-5) m^3/moleOfMolecule_Al2CuMg, sensibly equal to 4E-5
"surfaceEnergyPolynomialModel":([-0.10985+0.1,8.31E-4,-8.71E-7],"J.m^-2"), #  for example, if degree of polynom is 3 then we write:   ([a0,a1,a2,a3],"J.m^-2")
#"surfaceEnergyPolynomialModel":([0,0,0,1],"J.m^-2"),
"shapeFactor":(20,"m.m^-1"),   #ratio between lenght to radius. If nul, shape is spherical. was alphaP2
"deltaCell":(1/100,"m.m^-1") , #difference between precipitate and solid solution cell size , was deltamailleP2
"wettingAngle":(30*math.pi/180,"rad") # wetting angle of Sprime precipitates on GP precipitates, was thetaP2
} 

