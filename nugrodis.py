#for x in ChemicalElements.keys():
#    print(x)
#    print(ChemicalElements[x]["density"][0])

NuGroDisVersion="1.0"  

ComputationParam={"Type": "Quenching>Hardening", #Quenching,Hardening,ThermalLoading or mix(example: Quenching>Hardening)
                                                 #Hardening=maturation or tempering
                  "Material": "M2024" } 
                                                               
################################################################################################################################# 
##################################################      PHYSICAL DATA       #####################################################
#################################################################################################################################  

PhysicalConstants={
    "R": (8.314472,"J.mol^-1.K^-1"),#Gas constant
    "kB": (1.381E-23,"J.K^-1"),#Boltzman constant
    "Na": (6.02214E23,"mol^-1")#Avogadro Number
}

##################################################      CHEMICAL ELEMENTS       #####################################################
ChemicalElementsData={            
    #Aluminium
    "Al":{  "density" : (2698,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
            "molarMass": (26.98,"g.mol^-1"),#Molar mass, [g/mol]
            "youngModulus": (70000,"MPa"),#Young modulus, [MPa]
            "poissonCoef": (0.3,""),#Poisson coefficient
            "cellSize": (4.0412E-10,"m"), #Cell size, [m]
            "atomsPerCell":(4,"")  #number of atoms per cell 
         },
    
                
    
    #Copper
    "Cu":{  "density" : (8960,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
            "molarMass": (63.54,"g.mol^-1"), #Molar mass, [g/mol]
            "poissonCoef": (0.33,""), #Poisson coefficient
            "youngModulus": (124000,"MPa")  #Young modulus, [MPa]
         },
    
    #Magnesium
    "Mg":{  "density" : (1738,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
            "molarMass": (24.31,"g.mol^-1"),#Molar Mass, [g/mol]
            "poissonCoef": (0.35,""), #Poisson coefficient
            "youngModulus": (45000,"MPa")#Young modulus, [MPa]
         }
    
    #Oxygen
    #...
} 

ChemicalElementsData["Al"]["molarVolume"]= (PhysicalConstants["Na"][0]*ChemicalElementsData["Al"]["cellSize"][0]**3/ChemicalElementsData["Al"]["atomsPerCell"][0],"m^3.mol^-1"),  #molar volume of Al, is supposed to be the Solid solution volume, the unit will be checked

#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#     CHEMICAL ELEMENTS      *~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*


Cu={
"density" : (8960,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
"molarMass": (63.54,"g.mol^-1") #Molar mass, [g/mol]
}

Mg={
"density" : (1738,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
"molarMass": (24.31,"g.mol^-1")#Molar Mass, [g/mol]
}

Al={
"density" : (2698,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
"molarMass": (26.98,"g.mol^-1"),#Molar mass, [g/mol]
"youngModulus": (70000,"MPa"),#Young modulus, [MPa]
"poissonCoef": (0.3,""),#Poisson coefficient
"cellSize": (4.0412E-10,"m"), #Cell size, [m]
"atomsPerCell":(4,""),  #number of atoms per cell
}
Al["molarVolume"] = (PhysicalConstants["Na"][0]*Al["cellSize"][0]**3/Al["atomsPerCell"][0],"m^3.mol^-1") #molar volume of Al, is supposed to be the Solid solution volume, the unit will be checked

#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~


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
"EVacCu": (19264,"J.mol^-1"), #copper-Vacancy interaction energy #CHECK
"EVacMg": (19264,"J.mol^-1"), #magnesium-Vacancy interaction energy, #CHECK
"deltaSF": (17,"J.mol^-1.K^-1"), #Entropy of vacancy formation
"fE": (10**13,"s^-1"), #Frequency of vacancy's jumps
"halfSinkD": (30E-6,"m") #Semi-distance between vacancy sinks
}
VacanciesParam["Dlac0"]= (VacanciesParam["fE"][0]*Al["cellSize"][0]**2, Al["cellSize"][1]+"."+VacanciesParam["fE"][1])# Preexponential term value of vacancy diffusion expression.  
                                                                                                                      # Once upon a time... (^_^)  : "Dlac0": (Al["cellSize"]**2,"m^2.s^-1")
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~
#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~ 
                                                                                                                     
                                                                            
 

