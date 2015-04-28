from __future__ import print_function, division
from ElementsData import Dict as ElementsDataDict

##    __data = """ H 1,Li 3,Na 11,K 19,Rb 37,Cs 55,Fr 87,Be 4,Mg 12,Ca 20,Sr 38,Ba 56,Ra 88,Sc 21,Y 39,
##    Ti 22,Zr 40,Hf 72,Rf 104,V 23,Nb 41,Ta 73,Db 105,Cr 24,Mo 42,W 74,Sg 106,Mn 25,Tc 43,
##    Re 75,Bh 107,Fe 26,Ru 44,Os 76,Hs 108,Co 27,Rh 45,Ir 77,Mt 109,Ni 28,Pd 46,Pt 78,Ds 110,
##    Cu 29,Ag 47,Au 79,Rg 111,Zn 30,Cd 48,Hg 80,Uub 112,B 5,Al 13,Ga 31,In 49,Tl 81,Uut 113,
##    C 6,Si 14,Ge 32,Sn 50,Pb 82,Uuq 114,N 7,P 15,As 33,Sb 51,Bi 83,Uup 115,O 8,S 16,Se 34,
##    Te 52,Po 84,Uuh 116,F 9,Cl 17,Br 35,I 53,At 85,Uus 117,He 2,Ne 10,Ar 18,Kr 36,Xe 54,Rn 86,
##    Uuo 118,La 57,Ac 89,Ce 58,Th 90,Pr 59,Pa 91,Nd 60,U 92,Pm 61,Np 93,Sm 62,Pu 94,Eu 63,Am 95,
##    Gd 64,Cm 96,Tb 65,Bk 97,Dy 66,Cf 98,Ho 67,Es 99,Er 68,Fm 100,Tm 69,Md 101,Yb 70,No 102,Lu 71,Lr 103"""
    
class PyChemicalElement:
    """The concept of chemical element."""
    
    # static private attribute data holds the coma separated string "Symbol density massMolaire E nhu,AtomicNumber ..." 
    __data = """ Al 2698 26.98 70000 0.3 13,
                 Mg 1738 24.31 45000 0.35 12,
                 Cu 8960 64.54 124000 0.33 29
            """ 
    # the static private attribute Mendeleiv : dictionnary of pairs {symbol: atomic number}
    __MendeleievDic = {}

    @staticmethod
    def GetMendeleievDict() :
        """Returns the Mendeleiev dictionnary."""
        return PyChemicalElement.__MendeleievDic

    # the static private attribute Element : dictionnary of pairs {symbol: ChemicalElement object}
    __ElementsDic   = {}

    @staticmethod
    def GetElementsDict() :
        """Returns the Elements dictionnary."""
        return PyChemicalElement.__ElementsDic

    def __init__(self, symbol, density,  molarMass, YoungModulus, PoissonCoeff,atomicNumber ):
        """The constructor takes the 'symbol' and the 'atomic number' as arguments."""
        self.__symbol = symbol
        self.__density = density
        self.__molarMass = molarMass
        self.__YoungModulus = YoungModulus
        self.__PoissonCoeff = PoissonCoeff
        self.__atomicNumber = int(atomicNumber)

    @property
    def symbol(self): return self.__symbol

    @property
    def atomicNumber(self) : return self.__atomicNumber

    @property
    def density(self): return self.__density
    
    @property
    def molarMass(self): return self.__molarMass

    @property
    def YoungModulus(self): return self.__YoungModulus

    @property
    def PoissonCoeff(self): return self.__PoissonCoeff

    @staticmethod
    def initFromElementsData():
        """This public static method reads elements data from file ElementData.py. Is also used to populate the Mendeleiev and the Elements dictionnaries."""          
        for symbol in ElementsDataDict.keys():
            dataDict = ElementsDataDict[symbol]
            PyChemicalElement.__ElementsDic[symbol] = PyChemicalElement(symbol,  dataDict['density'][0],
                                                                                                                                 dataDict['molarMass'][0],
                                                                                                                                 dataDict['YoungModulus'][0],
                                                                                                                                 dataDict['PoissonCoeff'][0],
                                                                                                                                 dataDict['atomicNumber'][0])

##    @staticmethod
##    def initFromData():
##        """This public static method is used to populate the Mendeleiev and the Elements dictionnaries."""
##
##        PyChemicalElement.__MendeleievDic = { e.split()[0]: (map(float, e.split()[1:])) for e in PyChemicalElement.__data.split(",")}            
##        for symbol in PyChemicalElement.__MendeleievDic:
##            data = PyChemicalElement.__MendeleievDic[symbol]
##            PyChemicalElement.__ElementsDic[symbol] = PyChemicalElement(symbol, *data)

    @staticmethod
    def GetAtomicNumber(symbol):
        """This public static method returns the atomic number of the element corresponding to arg 'symbol'."""
        an = None
        try:
            an = PyChemicalElement.__MendeleievDic[symbol]
        except:
            print("Unknown symbol : <"+symbol+">")
        return an

    @staticmethod
    def GetElement(symbol) :
        """Returns the ChemicalElement object corresponding to arg 'symbol'."""
        return PyChemicalElement.__ElementsDic.get(symbol, None)

    def __str__(self):
        msg = ""
        formatSpec = "Element: {:4s}, {:>15s}{:4d}\n{:>15s}{:10.6e}\n{:>15s}{:10.6e}\n{:>15s}{:10.6e}\n{:>15s}{:10.6e}"
        msg += formatSpec.format(self.symbol,
                                 "atomic number: ", self.atomicNumber,
                                 " "*53+"     density: ", self.density,
                                 " "*53+"   molarMass: ", self.molarMass,
                                 " "*53+"YoungModulus: ", self.YoungModulus,
                                 " "*53+"PoissonCoeff: ", self.PoissonCoeff)
        return msg


class PyChemicalComposition:
    """ChemicalComposition models the concept of chemical composition.
       It has a normalized symbol (like "Al2CuMg"), which is given to the constructor :
       at consctruction time the symbol is parsed to obtain the list of each chemical
       component with its  """

    def __init__(self, symbols):
        self.__symbols = symbols
        self.__composition = {}
        self.__ParseSymbols()
        self.__listElements = []

    @property
    def symbols(self) : return self.__symbols
    
    @property
    def composition(self) : return self.__composition

    @property
    def sumOfStoichCoeff(self):
        sumOfStoichCoeff=0
        for compo in self.__composition.items():
            sumOfStoichCoeff+=compo[1][0]
        return sumOfStoichCoeff
            
    
    def __ParseSymbols(self):
        symbolBegins, symbol, coeff = True, "", 1
        for c in self.__symbols:
            if symbolBegins:
                assert c.isalpha() and c.isupper()
                symbol += c
                symbolBegins = False
            else:
                assert c.isalnum()
                if c.isalpha():
                    if c.islower():
                        symbol += c
                    else:
                        self.__composition[symbol] = (1, PyChemicalElement.GetElement(symbol))
                        symbol, coeff = c, 1
                else:
                    self.__composition[symbol] = (int(c), PyChemicalElement.GetElement(symbol))
                    symbolBegins, symbol, coeff = True, "", 1
                    
        if not self.__symbols[-1].isdigit():
            self.__composition[symbol] = (1, PyChemicalElement.GetElement(symbol))

        self.__listElements = sorted(self.__composition.keys())
        
        # debug: print [ (e, self.__composition[e], PyChemicalElement.GetElement(e)) for e in self.__listElements]

    def __str__(self):
        """This method is used when you try to convert a ChemicalComposition to a string
           (for instance when you type print(ChemicalComposition object))."""
        msg = "Chemical composition: {:15s}\n".format(self.symbols)
        for c in self.__composition.items():
            symbol = c[0]
            multiplicity, chemElem = c[1]
            msg += "\tSymbol:{:3s}, multiplicity:{:2d}, {:s}\n".format(symbol, multiplicity, chemElem)
            print ("Reading ChemicalElement("+symbol+")")
            #print "building C++ object ChemicalElement("+symbol+")"
        return msg
                        
    
if __name__ == "__main__":
##    # Build the Mendeleiev table:
##    PyChemicalElement.initFromData()
##    ED = PyChemicalElement.GetElementsDict()
##    MT = PyChemicalElement.GetMendeleievDict()
##
##    # try some chemical composition
##    a=PyChemicalComposition("Al8CuMg")
##    print(a)
##    print("Sum of Stoichiometric number = ",a.sumOfStoichCoeff )
##
##    a=PyChemicalComposition("NaCl")
##    print(a)
##    print("Sum of Stoichiometric number = ",a.sumOfStoichCoeff )
##
##    a= PyChemicalComposition("H2O")
##    print(a)
##    print("Sum of Stoichiometric number = ",a.sumOfStoichCoeff )

    PyChemicalElement.initFromElementsData()
    ED = PyChemicalElement.GetElementsDict()
    MT = PyChemicalElement.GetMendeleievDict()

    # try some chemical composition
    a=PyChemicalComposition("Al8CuMg")
    print(a)
    print("Sum of Stoichiometric number = ",a.sumOfStoichCoeff )

    a=PyChemicalComposition("NaCl")
    print(a)
    print("Sum of Stoichiometric number = ",a.sumOfStoichCoeff )

    a= PyChemicalComposition("H2O")
    print(a)
    print("Sum of Stoichiometric number = ",a.sumOfStoichCoeff )
    
    
