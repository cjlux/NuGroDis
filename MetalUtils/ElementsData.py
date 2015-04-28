from PhysicalConstants import Dict as PhysicalConstantsDict

def ComputeMolarVolume(Element,stoechiometrySumCoeff=1):
    AvogadroNumber=PhysicalConstantsDict["Na"]
    cellSize=Element["cellSize"]
    atomsPerCell=Element["atomsPerCell"]
    assert AvogadroNumber[1]=="mol^-1"
    assert cellSize[1]=="m"
    assert atomsPerCell[1]==""
    molarVolume= AvogadroNumber[0]*cellSize[0]**3/atomsPerCell[0]*stoechiometrySumCoeff
    return (molarVolume,"m^3.mol^-1")

##################################################      CHEMICAL ELEMENTS       #####################################################
Dict={            
    #Aluminium
    "Al":{  "density" : (2698,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
            "molarMass": (26.98,"g.mol^-1"),#Molar mass, [g/mol]
            "YoungModulus": (70000,"MPa"),#Young modulus, [MPa]
            "PoissonCoeff": (0.3,""),#Poisson coefficient
            "cellSize": (4.0412E-10,"m"), #Cell size, [m]
            "atomsPerCell":(4,"") , #number of atoms per cell
            "atomicNumber":(13,"")
         },
    
                
    
    #Copper
    "Cu":{  "density" : (8960,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
            "molarMass": (63.54,"g.mol^-1"), #Molar mass, [g/mol]
            "PoissonCoeff": (0.33,""), #Poisson coefficient
            "YoungModulus": (124000,"MPa"),  #Young modulus, [MPa]
            "atomicNumber":(29,"")
         },
    
    #Magnesium
    "Mg":{  "density" : (1738,"Kg.m^-3"),#Volumetric mass density, [Kg/m^3]
            "molarMass": (24.31,"g.mol^-1"),#Molar Mass, [g/mol]
            "PoissonCoeff": (0.35,""), #Poisson coefficient
            "YoungModulus": (45000,"MPa"),#Young modulus, [MPa]
            "atomicNumber":(12,"")
         }
    
    #Oxygen
    #...
} 

Dict["Al"]["molarVolume"]= ComputeMolarVolume(Dict["Al"])  #molar volume of Al, is supposed to be the Solid solution volume, the unit will be checked

#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~# ~#*~#*~#              CHEMICAL ELEMENTS      *~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*~#*


