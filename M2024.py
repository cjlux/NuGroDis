# -*- coding: utf-8 -*-

################################################################################################################################# 
##################################################       ALLOY 2024 DATA       ##################################################
#################################################################################################################################  


from nugrodis import Al
import Utils
#ALL CONCENTRATIONS MUST BE THOSE AFTER SOLUTIONIZING


##################################################    Copper DATA       ##########################################################

Cu={
"initialConcentration": (1.22/100,"m^3.m^-3"), #Unit can be m^3.m^-3 (volumic) or atom.atom^-1 (atomic) or kg.kg^-1 (massic). Copper initial concentration, in range [0,1]
"Q": (135000,"J.mol^-1"), #Activation energy of diffusion for copper
"preExpDiffusionCoef": (6.5E-5,"m^2.s^-1") #initial diffusion value for copper, pre-exponential diffusion coeffcient
}
##################################################   Magnesium DATA     ##########################################################

Mg={
"initialConcentration": (2.08/100,"m^3.m^-3"),#Unit can be m^3.m^-3 (volumic) or atom.atom^-1 (atomic) or kg.kg^-1 (massic) . Magnesium initial concentration, in range [0,1]
"Q": (131000,"J.mol^-1"), #Activation energy of diffusion for magnesium
"preExpDiffusionCoef": (1.2E-4,"m^2.s^-1") #initial diffusion value for magnesium, pre-exponential diffusion coeffcient
}


################################################## Guinier-Preston DATA ##########################################################
GP={
"chemicalComposition": ("Al8CuMg","")  , #Chemical composition, will be used to find stoichiometric coefs and then compute volumic concentration of elements
"initialNucleationSitesNb":(6.02214E28,"at.m^-3")  , # initial Number of nucleation sites at beginning of hardening, was Ns0P1
"preExpTermForSolvus":(0.992,"m^6.m^-6"),  # was KP1
"solvusActivationEnergy":(36000,"J.mol^-1"), # DeltaHP1
"surfaceEnergyPolynomialModel":([0.044],"J.m^-2"), #  for example, if degree of polynom is 3:   ([a0,a1,a2,a3],"J.m^-2")
"shapeFactor":(0,"m.m^-1"),   #ratio between lenght to radius. If nul, shape is spherical. was alphaP
"deltaCell":(1/100,"m.m^-1") , # difference between precipitate and solid solution cell size , was deltamailleP ( for example, = |a_ss-a_precipitate|/a_precipitate)
}
GP["distorsionEnergy"] = ((GP["deltaCell"][0]**2)*(Al["youngModulus"][0]*10**6)/(1-Al["poissonCoef"][0]),"J.m^-3") #elastic distorsion energy due to the difference of cell size.(was deltageP) 
GP["molarVolume"] = (Al["molarVolume"][0]*Utils.SumOfCoefficients(GP["chemicalComposition"][0]),"m^3.mol^-1") # volume of one mole of precipitates, Find attribute in class SSGrain

##################################################    Sprime DATA     ##########################################################
Sprime={
"chemicalComposition": ("Al2CuMg","")  , #Chemical composition, will be used to find stoichiometric coefs and then compute volumic concentration of elements
"preExpTermForSolvus":(49.746,"m^6.m^-6"),  # was KP2
"solvusActivationEnergy":(76000,"J.mol^-1"), #DeltaHP2
"molarVolume":(4E-5 ,"m^3.mol^-1"), # volume of one mole of precipitates, In UML diagram find attribute corresponding to "molarVolume" in class SSGrain
                                    #As commented by many authors, Al2CuMg has an orthorhombic structure with 16 atoms ==> (4*Al2CuMg) per unit cell (a=0,4nm, b=0,923 nm, and c=0,714 nm)
                                    #V_moleculeAl2CuMg=V_unitCell/4 , then V_mAl2CuMg which is the volume of 1 mole of molecules Al2CuMg is expressed as follow:  
                                    #V_mAl2CuMg= Na*V_moleculeAl2CuMg  =  3.9686*10^(-5) m^3/moleOfMolecule_Al2CuMg, sensibly equal to 4E-5
"surfaceEnergyPolynomialModel":([-0.10985+0.1,8.31E-4,-8.71E-7],"J.m^-2"), #  for example, if degree of polynom is 3 then we write:   ([a0,a1,a2,a3],"J.m^-2")
"shapeFactor":(20,"m.m^-1"),   #ratio between lenght to radius. If nul, shape is spherical. was alphaP2
"deltaCell":(1/100,"m.m^-1") , #difference between precipitate and solid solution cell size , was deltamailleP2
"wettingAngle":(30,"degree") # wetting angle of Sprime precipitates on GP precipitates, was thetaP2
}
Sprime["distorsionEnergy"] = ((Sprime["deltaCell"][0]**2)*(Al["youngModulus"][0]*10**6)/(1-Al["poissonCoef"][0]),"J.m^-3" ), #elastic distorsion energy due to the difference of cell size.(was deltageP2)


################################################################################################################################# 
####################################      solutionazing Temperature, VACANCIES PARAMETERS       #################################
#################################################################################################################################

VacanciesParam={
"Tsol": (495,"°C") #solutionazingTemp=InitialQuenching Temperature, [°C]
}

#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~ 