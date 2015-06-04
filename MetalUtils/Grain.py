from Mendeleiev import PyChemicalComposition
from ElementsData import Dict as ElementsDataDict

PrecipitateNatureList=['GuinierPreston','Sprime']

class PyPrecipitate:
    def __init__(self,dataDict,mainElement):
        #mandatory keys
        self.__nature=dataDict["nature"]
        self.__chemicalComposition=dataDict["chemicalComposition"]#the chemichal Formulation
        self.__preExpTermForSolvus=dataDict["preExpTermForSolvus"]
        self.__solvusActivationEnergy=dataDict["solvusActivationEnergy"]
        self.__surfaceEnergyPolynomialModel=dataDict["surfaceEnergyPolynomialModel"]
        self.__shapeFactor=dataDict["shapeFactor"]
        self.__deltaCell=dataDict["deltaCell"]
        
       

        #Optional keys
        self.__initialNucleationSitesNb=dataDict.get("initialNucleationSitesNb",None) #example : for GP
        self.__wettingAngle=dataDict.get("wettingAngle",None) #example: for Sprime
        self.__molarVolume=dataDict.get("molarVolume",None) #Can be also computed. Will be done subsequently

        #computed attributes
        self.__distorsionEnergy=None
        #self.__pyChemicalComposition=PyChemicalComposition(chemicalComposition)
        self.__sumOfStoichCoeff=PyChemicalComposition.sumOfStoichCoeff
        #self.__molarVolume=ComputeMolarVolume(ElementsDataDict[mainElement],sumOfStoichCoeff)

    @property
    def chemicalComposition(self): return self.__chemicalComposition
    @property
    def initialNucleationSitesNb(self): return self.__initialNucleationSitesNb
    @property
    def preExpTermForSolvus(self): return self.__preExpTermForSolvus
    @property
    def solvusActivationEnergy(self): return self.__solvusActivationEnergy
    @property
    def surfaceEnergyPolynomialModel(self): return self.__surfaceEnergyPolynomialModel
    @property
    def shapeFactor(self): return self.__shapeFactor
    @property
    def deltaCell(self): return self.__deltaCell
    @property
    def nature(self): return self.__nature
    @property
    def wettingAngle(self): return self.__wettingAngle
    @property
    def distorsionEnergy(self): return self.__distorsionEnergy
    @property
    def pyChemicalComposition(self): return self.__pyChemicalComposition
    @property
    def sumOfStoichCoeff(self): return self.__sumOfStoichCoeff
    @property
    def molarVolume(self): return self.__molarVolume
        


#GP["distorsionEnergy"] = ((GP["deltaCell"][0]**2)*(Al["youngModulus"][0]*10**6)/(1-Al["poissonCoef"][0]),"J.m^-3") #elastic distorsion energy due to the difference of cell size.(was deltageP) 
#GPcompo=PyChemicalComposition(GP["chemicalComposition"])
#GP["molarVolume"] = ComputeMolarVolume(Al,GPcompo.sumOfStoichCoeff) # volume of one mole of precipitates, Find attribute in class SSGrain
