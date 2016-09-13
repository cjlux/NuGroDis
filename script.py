# -*- coding: utf-8 -*-
import os, sys
import matplotlib.pyplot as plt
from numpy import array, arange
from matplotlib import rc
import M2024



SpecificValues=["-222.222","-111.111","111.111","222.222"] #DO NOT DELETE THIS LINE!!!



def GetInitialHalfSinkD(resultPath):
    """ Return initialHalfSinkD """
    ##Read file material vacancy properties and get information
    vacancyPropertiesFile=resultPath+"MaterialCurrentCompo/MaterialVacancyProperties.txt" 
    ReadVacancyPropertiesFile=OpenFileAndRead(vacancyPropertiesFile)
    initialHalfSinkD=float(ReadVacancyPropertiesFile[1][5])  # first save data is at line 2 so index 1. And "half sink distance" or "L" is the column 6, so index 5     
    return initialHalfSinkD

def GetInitialGamma(precipitateAttributesFile):
    """ Return initialGamma """
    
    ##Read file Precipitate attribute and get information
    ReadFilePrecipitateAttribute=OpenFileAndRead(precipitateAttributesFile)
    PrecipitateAttributeData=ReadFilePrecipitateAttribute[1:] 
    initialGamma=float(PrecipitateAttributeData[0][5]) # "surface energy" or "gamma" is the column 6, so index 5
    
    
    return initialGamma




def ValeurlaPlusProche(valeur,ColonneData):
    """Donne la liste de valeurs et la liste de leur index les plus proches possibles d'une liste de valeurs dans une colonne de données """
        
    valeur=list(valeur)
    ColonneData=list(ColonneData)
    INDEX=[]
    VPROCHE=[]
    for t in valeur:
        valeurproche=min(ColonneData, key=lambda x:abs(x-t))
        VPROCHE.append(valeurproche)
    VPROCHE=list(set(VPROCHE))
    VPROCHE=sorted(VPROCHE)
           
    
    for x in VPROCHE:
        indexvaleurproche=ColonneData.index(x)
        INDEX.append(indexvaleurproche)
    
    return VPROCHE,INDEX


def PostProcessing():
    f=open("lastResultDirectoryPath.txt","r")
    ComputationResultsDirectory=f.readlines(1)[0] #the first and only line of file "lastResultDirectoryPath.txt"
    f.close()
    path=ComputationResultsDirectory+"/"

    OutputDistributionCurves(path, timeStep=3600) # by default timeStep is 3600 (see function)
    OutputAttributes(path)
    OutputMaterialChemicalComposition(path)
    OutputMaterialVacancyProperties(path)
    OutputInterfacialDistributionCurves(path, timeStep=3600) # by default timeStep is 3600 (see function)


def CheckIfASpecificValueIsInData(data):
    ResultBool=False
    valueInData=None
    for specific in SpecificValues:
        if specific in data:
            ResultBool=True
            valueInData=specific
            
    
    return ResultBool, valueInData
        



def OutputDistributionCurves(path,Retoile=True, timeStep=3600):
    """Save distribution Curves"""
    
    print("******* POSTPROCESSING DISTRIBUTION CURVES *******")

    RadDisDir=path+"RadDisFiles/"  
    
    

    
##    for root, subdirs, files in os.walk(RadDisDir):
##        # print path to all subdirectories first.
##        for subdirname in subdirs:
##            print(os.path.join(root, subdirname))
##        #print path to all filenames.
##        for filename in files:
##            print(os.path.join(root,filename))
            
                
    

    #print ("os.listdir(RadDisDir)",os.listdir(RadDisDir))
    for subdirName in os.listdir(RadDisDir):
        if subdirName !=".directory":

            ###########
            #SAVE PATH#
            ###########
            ResultsDir=path+"Results/DistributionsCurves/"+subdirName+"/"
            if not os.path.exists(ResultsDir):
                os.makedirs(ResultsDir)
            ###########
            #SAVE PATH#
            ###########

            #print ("subdir Name is ",subdirName )
            RadDisFilesDir=RadDisDir+subdirName+"/"

            attributeFileName=subdirName+"_Attributes_.txt"
            AttributesFileDir=path+"PrecipitatesAttributes/"+subdirName+"/"
            
            ##Read file Precipitate atrtribute and get information
            ReadFilePrecipitateAttribute=OpenFileAndRead(AttributesFileDir+attributeFileName)
            PrecipitateAttributeData=ReadFilePrecipitateAttribute[1:]
            timeList=map(float,list(PrecipitateAttributeData[:,0]))
            
            
            initialGamma=PrecipitateAttributeData[0][5]
            initialHalfSinkD=GetInitialHalfSinkD(path)
            
            
            closeTimeList=list( arange(timeStep,float(timeList[-1]),timeStep)  )  # Time per steps of 'timeStep' secondes 
            
            if float(timeList[-1]) not in closeTimeList:closeTimeList.append(timeList[-1])
            
            if Retoile==True:
                ##From file Precipitate atrtribute, get information on real critical radius (r* + dr* )>>> >>> >>> 

                realCriticalRadiusList=[]
                realCriticalRadiusList=list(PrecipitateAttributeData[:,8])
                ##
            
            closestTimeList,closestTimeindexList=  ValeurlaPlusProche(closeTimeList,timeList)
            

            print("RadDisFilesDir", RadDisFilesDir)
            for filename in os.listdir(RadDisFilesDir):
                radDisCurrentTime=float(filename[12:-5])
                if radDisCurrentTime in closestTimeList:
                    if filename.endswith(".txt"):
                        #print ("PostProcessing file: <",filename, ">")
                        saveName=ResultsDir+filename[:-4]+".pdf"
                        ReadFile=OpenFileAndRead(RadDisFilesDir+filename)
                                
                        Data=ReadFile[1:]
                        RR=[]
                        NP=[]
                        for data in Data:
                            RR.append(data[1])
                            NP.append(data[2])
    
                        rc('font', family='serif')
                        fig, ax = plt.subplots()
                        ax.plot(RR,NP,'r-o')
                        ax.grid(True)
                        ax.set_xlabel(u'Rayon [m]',color='green')
                        ax.set_ylabel(u'Nombre de particules par unité de volume [N/m^3] ',color='red')
                        # draw a vertical line x=retoile
                        if Retoile==True:
                            currentTimeIndex=timeList.index(radDisCurrentTime)
                            criticalRadius=realCriticalRadiusList[currentTimeIndex]
                            #print("critical radius is",criticalRadius, "current time", radDisCurrentTime ,"index", currentTimeIndex)
                            
                            if criticalRadius!="-222.222": # retoile existe
                                ax.axvline(x=criticalRadius,linestyle='--', linewidth=1, color='blue',label=u"Position")
                                ax.legend([subdirName+' distribution', 'Critical radius'],\
                                          fancybox=True, shadow=True )
                            else: #criticalRadius=="-222.222" , retoile n'existe pas
                                ax.legend([subdirName+' distribution;\n(CriticalRadius does not exist)'],\
                                          fancybox=True, shadow=True )
    
                        plt.title(subdirName+u" precipitates distribution at time:\n"+\
                        str(float(radDisCurrentTime)/3600.)+" [h] (or "+str(radDisCurrentTime)+" [s])"+\
                        ' with gamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
                        plt.savefig(saveName)
                        plt.close()


def PlotCurve(X,Y,saveName,Xlabel="",Ylabel="",figTitle="",marker="r-o",Xcolor="green",Ycolor="red", returnFig=False, labelCurve=' '):
    "Plot a curve and save it to the given path"
    #rc('text', usetex=True)
    rc('font', family='serif')

    # marker can be 
            # un-filled markers  ',' , '.', '1' , '2' , '4' , '_' , 'x' , '|' , '0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '+'
            # filled markers 'o', 'v', '^', '<', '>', '8', 's', 'p', '*', 'h', 'H', 'D', 'd'

    fig, ax = plt.subplots()
    ax.plot(X,Y,marker, label=labelCurve)
    ax.grid(True)
    plt.title(u""+figTitle)
    ax.set_xlabel(u""+Xlabel,color=Xcolor)
    ax.set_ylabel(u""+Ylabel,color=Ycolor)
    ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0., shadow=True, fancybox=True)

    if returnFig==False:
        plt.savefig(saveName, bbox_inches='tight') #saveName or Absolute complete path to saveFile
        plt.close()
    if returnFig==True:
        return fig,ax
        
def PlotInterfacialDistributionCurve(X,Y,saveName,Xlabel="",Ylabel="",figTitle="",Xcolor="green",Ycolor="red", returnFig=False):
    "Plot a curve and save it to the given path"
    #rc('text', usetex=True)
    import matplotlib.pyplot as plt
    import numpy as np
    
    rc('font', family='serif')
    
    fig, ax = plt.subplots()
    markerline, stemlines, baseline = ax.stem(X, Y, '-') #markerfmt="  for blank marker"
    #plt.setp(markerline, 'markerfacecolor', 'b')
    plt.setp(stemlines, 'color', 'b')
    plt.setp(baseline, 'color', 'black', 'linewidth', 2)
    
    ax.grid(True)
    plt.title(u""+figTitle)
    ax.set_xlabel(u""+Xlabel,color=Xcolor)
    ax.set_ylabel(u""+Ylabel,color=Ycolor)

    if returnFig==False:
        plt.savefig(saveName, bbox_inches='tight') #saveName or Absolute complete path to saveFile
        plt.close()
    if returnFig==True:
        return fig,ax
        




def OpenFileAndRead(path):
    #print ("Reading file: <",path, ">")
    ReadData=[]
    f=open(path,"r")
    for line in f:
        try:
            if line!="\n":
                    x=line.split() 
                    ReadData.append(x)            
        except:
            print(u"erreur de conversion de la ligne:",line)
    f.close()
    #convert in matrix
    ReadData=array(ReadData)   
    return ReadData
    


def PlotFromReadData(ReadFile,saveDirPath,Yindex,saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="", marker="r-o" ):
    """Plot from a data given in argument (ReadFile)"""
    ###########
    #SAVE PATH#
    ###########
    if not os.path.exists(saveDirPath):
        os.makedirs(saveDirPath)
    ###########
    #SAVE PATH#
    ###########
    
    Data=ReadFile[1:]

    #print("Number of labels is: ",len(ReadFile[0]))

    ListOfAttributeName=[]
    for label in ReadFile[0]:
        ListOfAttributeName.append(label)
    assert (len(ListOfAttributeName)==len(Data[1])),"The attributes labels are more than data"
    lenghtOfEachLines=len(Data[1])

    assert (Yindex in range(len(ListOfAttributeName)) ),"Given Yindex is not in range of potential index"
    assert (Xindex in range(len(ListOfAttributeName)) ),"Given Xindex is not in range of potential index"
    
    for labelData in ListOfAttributeName:
        exec labelData+"=[]"

    for data in Data:
        for labelData,value in zip(ListOfAttributeName,data):
            exec labelData+".append(value)"
            
    AttributesDataDict={}
    for labelData in ListOfAttributeName:
        #Add AttributeData do dictionary
        exec "AttributesDataDict[labelData]="+labelData

    #print("Attibute data dict",AttributesDataDict)

    # ListOfAttributeName[Xindex] should be the label in X coordinate. ex: "time"
    xData=AttributesDataDict[ListOfAttributeName[Xindex]] 
    
    attributeToPlot=ListOfAttributeName[Yindex]
    attributeToPlotData= AttributesDataDict[attributeToPlot]

    ####################################################
    ## DEFAULTS VALUE FOR LABELS, TITLE & figSaveName ##

    if Xlabel==None:
        Xlabel=ListOfAttributeName[Xindex]

    if Ylabel==None:
        Ylabel=attributeToPlot

    if figTitle=="":
        figTitle=figtitle="Evolution of "+attributeToPlot+" Vs "+ListOfAttributeName[Xindex]

    if saveFigName==None:
        saveFigName=attributeToPlot+"_Vs_"+ListOfAttributeName[Xindex]
    ## DEFAULTS VALUE FOR LABELS, TITLE & figSaveName ##
                


    ####################################################################
    ##########   Specific values of Ylabel and figTitle   ###############
    if attributeToPlot=="Vf":
        Ylabel="Volumic Fraction [Nb/m^3] "
        figTitle="Temporal evolution of\n volumic fraction"

    if attributeToPlot=="DeltaGv":
        Ylabel="Phase change volumic Energy [J/m^3]"
        figTitle="Temporal evolution of\n Phase change volumique Energy"

    if attributeToPlot=="DeltaGe":
        Ylabel="Elastique distorsion volumique Energy [J/m^3]"
        figTitle="Temporal evolution of\n distorsion volumique Energy"

    if attributeToPlot=="NuclSiteDensity":
        Ylabel="Nucleation site density [Number of Site per volumic unit]"
        figTitle="Temporal evolution of\n Nucleation site density"

    if attributeToPlot=="gamma":
        Ylabel="Surface energy [J/m^2]"
        figTitle="Temporal evolution of\n Surface energy"

    if attributeToPlot=="rEtoile":
        Ylabel="Critical radius [m]"
        figtitle="Temporal evolution of\n critical radius (r*) "

    if attributeToPlot=="delta_rEtoile":
        Ylabel="Delta critical radius [m]"
        figTitle="Temporal evolution of\n Delta critical radius (dr*_kT)"

    if attributeToPlot=="Real_rEtoile":
        Ylabel="Real critical radius [m]"
        figTitle="temporal evolution of\n the critical radius (r* + dr*)"

    if attributeToPlot=="Delta_GEtoile":
        Ylabel="Total critical germination volumic Energy [J/m^3]"
        figTitle="Temporal evolution of\n critical germination total"

    if attributeToPlot=="Z":
        Ylabel="Zeldovich factor"
        figTitle="Temporal evolution of\n Zeldovich factor"
        
    if attributeToPlot=="BetaEtoile":
        Ylabel=" Beta* (Beta for critical radius) "
        figTitle="Temporal evolution of Beta* "

    if attributeToPlot=="JEtoile":
        Ylabel="Nucleation Rate [NbPrecipitates/m^3/s]"
        figTitle="Temporal evolution of\n nucleation rate"

    if attributeToPlot=="XvCuEqSS":
        Ylabel="XvCuEqSS"
        figTitle="Copper equilibrium conc.\n in Solid Solution\n(with infinite radius prec.)"
        
    if attributeToPlot=="XvMgEqSS":
        Ylabel="XvMgEqSS"
        figTitle="Magnesium equilibrium conc.\n in Solid Solution\n(with infinite radius prec.)"
    ##########   Specific values of Ylabel and figTitle   ###############
        


    savePath=saveDirPath+"/"+saveFigName+".pdf"
    PlotCurve(xData,attributeToPlotData,savePath,Xlabel=Xlabel,Ylabel=Ylabel,figTitle=figTitle,marker=marker)

 


def ReadFileAndPlot(fileName,fileDirPath,saveDirPath,Yindex,Xindex=0, Xlabel=None,Ylabel=None,figTitle=""):
    """save curve """
    ###########
    #SAVE PATH#
    ###########
    if not os.path.exists(saveDirPath):
        os.makedirs(saveDirPath)
    ###########
    #SAVE PATH#
    ###########
    
    #print ("PostProcessing file: <",fileDirPath+"/"+fileName, ">")
    ReadFile=OpenFileAndRead(fileDirPath+"/"+fileName)

    ListOfAttributeName=[]
    for label in ReadFile[0]:
        ListOfAttributeName.append(label)
    
    attributeToPlot=ListOfAttributeName[Yindex]
    saveFigName=fileName[:-4]+"_"+attributeToPlot #fileName[:-4] to avoid ".txt"

    PlotFromReadData(ReadFile=ReadFile,saveDirPath=saveDirPath,Yindex=Yindex,\
                     saveFigName=saveFigName,Xindex=Xindex, Xlabel=Xlabel,Ylabel=Ylabel,figTitle=figTitle)





def OutputMaterialChemicalComposition(path):
    
    print("******* POSTPROCESSING MATERIAL CHEMICAL COMPOSITION CURVES *******")    
    
    ###########
    #SAVE PATH#
    ###########
    ResultDir=path+"Results/Material/ChemicalComposition"
    if not os.path.exists(ResultDir):
        os.makedirs(ResultDir)
    ###########
    #SAVE PATH#
    ###########

    fileDir=path+"/MaterialCurrentCompo/"

    filename="MaterialChemicalCompo.txt"

    
    RF=OpenFileAndRead(fileDir+filename)
    for dataToPlotIndex in range(1, len(RF[0]) ):  
        PlotFromReadData(ReadFile=RF,saveDirPath=ResultDir,Yindex=dataToPlotIndex)# saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="" 


def OutputMaterialVacancyProperties(path):
        
    print("******* POSTPROCESSING MATERIAL VACANCIES PROPERTIES CURVES *******")    
    
    ###########
    #SAVE PATH#
    ###########
    ResultDir=path+"Results/Material/MaterialVacancyProperties"
    if not os.path.exists(ResultDir):
        os.makedirs(ResultDir)
    ###########
    #SAVE PATH#
    ###########

    fileDir=path+"/MaterialCurrentCompo/"

    
    filename="MaterialVacancyProperties.txt"

    
    RF=OpenFileAndRead(fileDir+filename)
    for dataToPlotIndex in range(1, len(RF[0]) ):  
        PlotFromReadData(ReadFile=RF,saveDirPath=ResultDir,Yindex=dataToPlotIndex)#,saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="" )

    


def OutputMaterialSaveFile(path):
    
    ###########
    #SAVE PATH#
    ###########
    ResultDir=path+"Results/Material/"
    if not os.path.exists(ResultDir):
        os.makedirs(ResultDir)
    ###########
    #SAVE PATH#
    ###########

    fileDir=path+"/MaterialCurrentCompo/"


    
    for filename in os.listdir(fileDir):
        if filename.endswith(".txt"):
            RF=OpenFileAndRead(fileDir+filename)

            
            for dataToPlotIndex in range(1, len(RF[0]) ):  
                PlotFromReadData(ReadFile=RF,saveDirPath=ResultDir,Yindex=dataToPlotIndex)#,saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="" )
        
    

    
            
def OutputAttributes(path):
    """Save Precipitates attributes Curves"""
    
    print("******* POSTPROCESSING PRECIPITATE ATTRIBUTES CURVES *******")

    AttributesDir=path+"PrecipitatesAttributes/"

    for subdirName in os.listdir(AttributesDir):
        if subdirName !=".directory":

            ###########
            #SAVE PATH#
            ###########
            ResultsDir=path+"Results/PrecipitatesAttributesCurves/"+subdirName+"/"
            if not os.path.exists(ResultsDir):
                os.makedirs(ResultsDir)
            ###########
            #SAVE PATH#
            ###########
            
            AttibutesFilesDir=AttributesDir+subdirName+"/"
            print("Found directory for precipitate ", subdirName)
                
            for filename in os.listdir(AttibutesFilesDir):
                if filename.endswith(".txt"):
                    #print ("PostProcessing file: <",filename, ">")
                    ReadFile=OpenFileAndRead(AttibutesFilesDir+filename)

                    Data=ReadFile[1:]
                    
                    if float(Data[0][0])==0: #Data[0][0] should be the first value of the time column
                        #skip line time=0. Dont use data where time = 0
                        Data=ReadFile[2:]
                    
                    initialGamma=Data[0][5]
                    initialHalfSinkD=GetInitialHalfSinkD(path)
                    

                    ListOfAttributeName=[]
                    for label in ReadFile[0]:
                        ListOfAttributeName.append(label)
                    assert (len(ListOfAttributeName)==len(Data[1])),"The attributes labels are more than data"
                    lenghtOfEachLines=len(Data[1])
                    
                    for labelData in ListOfAttributeName:
                        exec labelData+"=[]"

                    for data in Data:
                        for labelData,value in zip(ListOfAttributeName,data):
                            exec labelData+".append(value)"
                            
                    AttributesDataDict={}
                    for labelData in ListOfAttributeName:
                        #Add AttributeData do dictionary
                        exec "AttributesDataDict[labelData]="+labelData

                    #print("toto",AttributesDataDict)

                    timeData=AttributesDataDict[ListOfAttributeName[0]] 
                    # ListOfAttributeName[0] should be the time, or the value in X coordinate
                    for attributeToPlot,attributeToPlotData in AttributesDataDict.items():
                        if attributeToPlot != ListOfAttributeName[0]:

                            #label in Precipitate::SavePrecipitateAttributes() in file Precipitate.cpp are:
                            # time	Vf	DeltaGv	DeltaGe	NuclSiteDensity	gamma
                            # rEtoile	delta_rEtoile	Real_rEtoile	Delta_GEtoile	Z
                            # BetaEtoile	JEtoile	XvCuEqSS	XvMgEqSS


                            specificAttribute=["Vf","DeltaGv","DeltaGe","NuclSiteDensity","gamma","rEtoile","delta_rEtoile","Real_rEtoile",\
                                               "Delta_GEtoile","Z","BetaEtoile","JEtoile","XvCuEqSS","XvMgEqSS"]

                            if (attributeToPlot in specificAttribute):
                                if attributeToPlot=="Vf":
                                    ylabel="Volumic Fraction [Nb/m^3] "
                                    figtitle="Temporal evolution of\n volumic fraction"

                                if attributeToPlot=="DeltaGv":
                                    ylabel="Phase change volumic Energy [J/m^3]"
                                    figtitle="Temporal evolution of\n Phase change volumique Energy"

                                if attributeToPlot=="DeltaGe":
                                    ylabel="Elastique distorsion volumique Energy [J/m^3]"
                                    figtitle="Temporal evolution of\n distorsion volumique Energy"

                                if attributeToPlot=="NuclSiteDensity":
                                    ylabel="Nucleation site density [Number of Site per volumic unit]"
                                    figtitle="Temporal evolution of\n Nucleation site density"

                                elif attributeToPlot=="gamma":
                                    ylabel="Surface energy [J/m^2]"
                                    figtitle="Temporal evolution of\n Surface energy"

                                if attributeToPlot=="rEtoile":
                                    ylabel="Critical radius [m]"
                                    figtitle="Temporal evolution of\n critical radius (r*) "

                                elif attributeToPlot=="delta_rEtoile":
                                    ylabel="Delta critical radius [m]"
                                    figtitle="Temporal evolution of\n Delta critical radius (dr*_kT)"

                                if attributeToPlot=="Real_rEtoile":
                                    ylabel="Real critical radius [m]"
                                    figtitle="temporal evolution of\n the critical radius (r* + dr*)"

                                if attributeToPlot=="Delta_GEtoile":
                                    ylabel="Total critical germination volumic Energy [J/m^3]"
                                    figtitle="Temporal evolution of\n critical germination total"

                                if attributeToPlot=="Z":
                                    ylabel="Zeldovich factor"
                                    figtitle="Temporal evolution of\n Zeldovich factor"
                                    
                                if attributeToPlot=="BetaEtoile":
                                    ylabel=" Beta* (Beta for critical radius) "
                                    figtitle="Temporal evolution of Beta* "

                                if attributeToPlot=="JEtoile":
                                    ylabel="Nucleation Rate [NbPrecipitates/m^3/s]"
                                    figtitle="Temporal evolution of\n nucleation rate"

                                if attributeToPlot=="XvCuEqSS":
                                    ylabel="XvCuEqSS"
                                    figtitle="Copper equilibrium concentration\n in Solid Solution\n(with infinite radius prec.)"

                                if attributeToPlot=="XvMgEqSS":
                                    ylabel="XvMgEqSS"
                                    figtitle="Magnesium equilibrium concentration\n in Solid Solution\n(with infinite radius prec.)"
                                
                            else:
                                ylabel=attributeToPlot
                                figtitle="Temporal evolution of "+ylabel

                            #### SAVE NAME  ####
                            saveName=ResultsDir+filename[:-15]+attributeToPlot+".pdf"
                            ####################
                            
                            if ( (attributeToPlot=="Real_rEtoile")&("-222.222" in attributeToPlotData) ):                                                               
                                RealRetoileList=list(attributeToPlotData)
                                firstOccurenceWhereRealRetoileDoesntExist=RealRetoileList.index("-222.222")
                                timeWhereRealCriticalRadiusStop= timeData[firstOccurenceWhereRealRetoileDoesntExist]

                                
                                fig, ax = PlotCurve(timeData[:firstOccurenceWhereRealRetoileDoesntExist],\
                                                    attributeToPlotData[:firstOccurenceWhereRealRetoileDoesntExist],saveName,\
                                                    Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle, returnFig=True)
                                #ax.axvline(x=timeWhereRealCriticalRadiusStop,linestyle='--', linewidth=1, color='blue')
                                
                                xmin,xmax= plt.xlim()
                                ax.axvspan(timeWhereRealCriticalRadiusStop,xmax,hatch='\\\\\\', label="Retoile does not exist",\
                                           linestyle='dashed', fill=False,edgecolor="grey")
                                ax.legend(["Real Critical radius", 'Critical radius does not exist'],\
                                      fancybox=True, shadow=True,loc='upper center')
                                # pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,

                                plt.title(figtitle+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
                                plt.savefig(saveName,bbox_inches='tight')
                                plt.close()

                            BoolCheckIfASpecificValueIsInData, specificValueFound = CheckIfASpecificValueIsInData(attributeToPlotData)
                            if ( BoolCheckIfASpecificValueIsInData==True ):
                                 AttributeToplotDataList=list(attributeToPlotData)
                                 firstOccurenceWhereSpecificValueFound=AttributeToplotDataList.index(specificValueFound)
                                 timeWhereSpecificValueFound=timeData[firstOccurenceWhereSpecificValueFound]

                                 fig, ax = PlotCurve(timeData[:firstOccurenceWhereSpecificValueFound],\
                                                    attributeToPlotData[:firstOccurenceWhereSpecificValueFound],saveName,\
                                                    Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle, returnFig=True )
                                 xmin,xmax= plt.xlim()
                                 ax.axvspan(timeWhereSpecificValueFound,xmax,hatch='\\\\\\', label=attributeToPlot+" does not exist",\
                                           linestyle='dashed', fill=False,edgecolor="grey")
                                 ax.legend([attributeToPlot, attributeToPlot+" does not exist"],\
                                      fancybox=True, shadow=True,loc='upper center')
                                 #pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,

                                 plt.title(figtitle+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
                                 plt.savefig(saveName,bbox_inches='tight')
                                 plt.close()
                                 
                                                                
                            else:
                                PlotCurve(timeData,attributeToPlotData,saveName,Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle+\
                                '\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))


def ReadFileAndPlotOldVersion(fileName,fileDirPath,saveDirPath,Yindex,Xindex=0):

    
    """save curve """
    ###########
    #SAVE PATH#
    ###########
    if not os.path.exists(saveDirPath):
        os.makedirs(saveDirPath)
    ###########
    #SAVE PATH#
    ###########
    
    #print ("PostProcessing file: <",fileDirPath+"/"+fileName, ">")
    ReadFile=OpenFileAndRead(fileDirPath+"/"+fileName)

    Data=ReadFile[1:]

    #print("Number of labels is: ",len(ReadFile[0]))

    ListOfAttributeName=[]
    for label in ReadFile[0]:
        ListOfAttributeName.append(label)
    assert (len(ListOfAttributeName)==len(Data[1])),"The attributes labels are more than data"
    lenghtOfEachLines=len(Data[1])
    
    for labelData in ListOfAttributeName:
        exec labelData+"=[]"

    for data in Data:
        for labelData,value in zip(ListOfAttributeName,data):
            exec labelData+".append(value)"
            
    AttributesDataDict={}
    for labelData in ListOfAttributeName:
        #Add AttributeData do dictionary
        exec "AttributesDataDict[labelData]="+labelData

    #print("Attibute data dict",AttributesDataDict)

    # ListOfAttributeName[Xindex] should be the label in X coordinate. ex: "time"
    xData=AttributesDataDict[ListOfAttributeName[Xindex]] 
    
    attributeToPlot=ListOfAttributeName[Yindex]
    attributeToPlotData= AttributesDataDict[attributeToPlot]

    if attributeToPlot=="Vf":
        ylabel="Volumic Fraction [Nb/m^3] "
        figtitle="Temporal evolution of volumic fraction"

    elif attributeToPlot=="DeltaGv":
        ylabel="Phase change volumic Energy [J/m^3]"
        figtitle="Temporal evolution of Phase change volumique Energy"

    elif attributeToPlot=="DeltaGe":
        ylabel="Elastique distorsion volumique Energy [J/m^3]"
        figtitle="Temporal evolution of distorsion volumique Energy"

    elif attributeToPlot=="NuclSiteDensity":
        ylabel="Nucleation site density [Number of Site per volumic unit]"
        figtitle="Temporal evolution of Nucleation site density"

    elif attributeToPlot=="gamma":
        ylabel="Surface energy [J/m^2]"
        figtitle="Temporal evolution of Surface energy"

    elif attributeToPlot=="rEtoile":
        ylabel="Critical radius [m]"
        figtitle="Temporal evolution of critical radius (r*) "

    elif attributeToPlot=="delta_rEtoile":
        ylabel="Delta critical radius [m]"
        figtitle="Temporal evolution of Delta critical radius (dr*_kT)"

    elif attributeToPlot=="Real_rEtoile":
        ylabel="Real critical radius [m]"
        figtitle="temporal evolution of the critical radius (r* + dr*)"

    elif attributeToPlot=="Delta_GEtoile":
        ylabel="Total critical germination volumic Energy [J/m^3]"
        figtitle="Temporal evolution of critical germination total"

    elif attributeToPlot=="Z":
        ylabel="Zeldovich factor"
        figtitle="Temporal evolution of Zeldovich factor"
        
    elif attributeToPlot=="BetaEtoile":
        ylabel=" Beta* (Beta for critical radius) "
        figtitle="Temporal evolution of Beta* "

    elif attributeToPlot=="JEtoile":
        ylabel="Nucleation Rate [NbPrecipitates/m^3/s]"
        figtitle="Temporal evolution of nucleation rate"

    elif attributeToPlot=="XvCuEqSS":
        ylabel="XvCuEqSS"
        figtitle="Copper equilibrium conc. in Solid Solution(with infinite radius prec.)"
        
    elif attributeToPlot=="XvMgEqSS":
        ylabel="XvMgEqSS"
        figtitle="Magnesium equilibrium conc. in Solid Solution(with infinite radius prec.)"
        
    else:
        ylabel=attributeToPlot
        figtitle="Evolution of "+ylabel+" Vs "+ListOfAttributeName[Xindex] 

    saveFigName=fileName[:-4]+"_"+attributeToPlot+".pdf"
    savePath=saveDirPath+"/"+saveFigName
    PlotCurve(xData,attributeToPlotData,savePath,Xlabel=ListOfAttributeName[Xindex],Ylabel=ylabel,figTitle=figtitle)

def ProcessUnfinishedComputation(path):
    #OutputDistributionCurves(path)
    OutputAttributes(path)
    OutputMaterialChemicalComposition(path)
    OutputMaterialVacancyProperties(path)



def OpenAndReadFilePrecInterf(path):
    #print ("Reading file: <",path, ">")
    ReadData=[]
    f=open(path,"r")
    for line in f:
        try:
            if line!="\n":
                    x=line.split()
                    if ">>>>>>>" in x:
                        x.remove(">>>>>>>")
                    ReadData.append(x)            
        except:
            print(u"erreur de conversion de la ligne:",line)
    f.close()
    #convert in matrix
    ReadData=array(ReadData)   
    return ReadData



def FindMinRadiusAndSpatialStepOfDistribution(pathToFile):
    """Find minimum radius and spatial step of a given radius disttribution file"""
    from numpy import isclose
    
    ReadFile=OpenFileAndRead(pathToFile)
    Data=ReadFile[1:]
    RR=[]
    for data in Data:
        RR.append(data[1])    
    
    RR=map(float,RR)
    
    r1= RR[0] #Value of R1, first value of Radius
    
    r2= RR[1] #value of R2, second value of Radiu
    
    
    ListOfDelta=[]
    for r, rNext in zip(RR[:-1], RR[1:]):
        ListOfDelta.append(rNext-r)
    
    # Check if: r_n - r_(n-1) = ... = r3-r2 = r2-r1
    for delta, deltaNext in zip(ListOfDelta[:-1], ListOfDelta[1:] ):
        assert( isclose(delta,deltaNext) ), "problem when finding spatial step of radius distribution"
    
    
    DeltaR= r2-r1

    
    
    assert (r1>=0), "minimum radius found is negative"
    
    assert (DeltaR>0), "spatial step found is not strictly positive"
    
    return r1,DeltaR


def makeRadiusListForDistribution(r1,deltaR,n):
    """generate and return a radius list for a distribution"""    
    RR=[]
    assert (r1>=0), "given minimum radius for makeRadiusListForDistribution() is negative"    
    assert (deltaR>0), "given spatial step for makeRadiusListForDistribution() is not strictly positive"    
    assert(n>=1) ,"given number of class for makeRadiusListForDistribution() is not superior or equal to 1. Cannot make Radius List! "        
    
    for u in range(1,n,1):
        rr=r1 + (u-1)*deltaR
        RR.append(rr)

    return RR


def makeInterfacialRadiusListForDistribution(r1,deltaR,n):
    """generate and return an interfacial radius list for a distribution"""    
    interfacialRR=[]
    assert (r1>=0), "given minimum radius for makeInterfacialRadiusListForDistribution() is negative"    
    assert (deltaR>0), "given spatial step for makeInterfacialRadiusListForDistribution() is not strictly positive"    
    assert(n>=1) ,"given number of class for makeInterfacialRadiusListForDistribution() is not superior or equal to 1. Cannot make Radius List! "    

    #add interface radius 0 (r0Left)
    r0Left=r1-deltaR/2
    assert( r0Left >0), "The left radius of first class (r0Left) is not strictly positive: given minimum radius and spatialStep are incompatible "   
    
    for u in range(1,n+1,1):
        rr=r0Left + (u-1)*deltaR
        interfacialRR.append(rr)

    return interfacialRR



def OutputInterfacialDistributionCurves(path, timeStep=3600):
    """Save Precipitates Interfacial Distribution Curves"""
    
    print("******* POSTPROCESSING PRECIPITATES INTERFACIAL DISTRIBUTION CURVES *******")
    
    PrecipitatesInterfPropertiesDir=path+"PrecipitatesInterfacialProperties/"
    RadDisDir=path+"RadDisFiles/" 
    
    for subdirName in os.listdir(PrecipitatesInterfPropertiesDir):
        if subdirName !=".directory":

            ###########
            #SAVE PATH#
            ###########
            ResultsDir=path+"Results/PrecipitatesInterfacialPropertiesCurves/"+subdirName+"/"
            if not os.path.exists(ResultsDir):
                os.makedirs(ResultsDir)
            ###########
            #SAVE PATH#
            ###########
            
            FileDir=PrecipitatesInterfPropertiesDir+subdirName+"/"
            print("Found directory for precipitate ", subdirName)
                
            for filename in os.listdir(FileDir):
                if filename.endswith("InterfacialConcentrations_.txt"):
                    print ("PostProcessing Interfacial concentration File: <",filename, ">")
                    
                    ###########
                    #create save Directory For Interfacial concentration files#
                    ###########
                    InterfacialCocentrationResultsDir=ResultsDir+"InterfacialConcentration"
                    if not os.path.exists(InterfacialCocentrationResultsDir):
                        os.makedirs(InterfacialCocentrationResultsDir)
                    ###########
                    #create save Directory For Interfacial concentration files#
                    ###########                    
                    
                    ReadFile=OpenAndReadFilePrecInterf(FileDir+filename)
                    Data=ReadFile[2:]
                    
                    AnyFileRadiusDistribution= os.listdir(RadDisDir+subdirName+"/") [0] #0 or any int, because no matter what the file chosen
                    
                    
                    pathToTheFileRadiusDistribution= RadDisDir+subdirName+"/"+AnyFileRadiusDistribution
                    print (pathToTheFileRadiusDistribution)
                    minRadius, spatialStep= FindMinRadiusAndSpatialStepOfDistribution(pathToTheFileRadiusDistribution)
                    
                    timeList=[]
                    for lineData in Data:
                        timeList.append(lineData[0])
                    timeList=map(float, timeList)
                    
                    closeTimeList=list( arange(timeStep,float(timeList[-1]),timeStep)  )  # Time per steps of 'timeStep' secondes
                    if float(timeList[-1]) not in closeTimeList:closeTimeList.append(timeList[-1])
                    closestTimeList,closestTimeindexList=  ValeurlaPlusProche(closeTimeList,timeList)  
                    
                    for i in closestTimeindexList:
                        data=Data[i]
                        currentTime=data[0]
                        InterfacialValuesList=data[1:]
                        n=len(InterfacialValuesList)
                        RList = makeInterfacialRadiusListForDistribution(minRadius,spatialStep,n )
                        assert( len(RList)==len(InterfacialValuesList) ), "Radius List and Interfacial values list do not have the same dimensions"                    
                        
                        saveFigName="ICDistribution_"+currentTime+".pdf"
                        savePath=InterfacialCocentrationResultsDir+"/"+saveFigName
                        PlotInterfacialDistributionCurve(RList,InterfacialValuesList,
                                  saveName=savePath, 
                                  Xlabel="Radius [m]",\
                                  Ylabel="InterfacialConcentration", 
                                  figTitle="Interfacial concentration distribution at time:\n"+str(float(currentTime)/3600.)+" [h] (or "+currentTime+" [s]")                   
                    
                    
#                    for lineData in Data:
#                        currentTime= lineData[0]
#                        InterfacialValuesList= lineData[1:]
#                        n=len(InterfacialValuesList)
#                        RList = makeInterfacialRadiusListForDistribution(minRadius,spatialStep,n )    
#                        assert( len(RList)==len(InterfacialValuesList) ), "Radius List and Interfacial valist list do not have the same dimensions"
#                        
#                        
#                        
#                        saveFigName="ICDistribution_"+currentTime+".pdf"
#                        savePath=InterfacialCocentrationResultsDir+"/"+saveFigName
#                        PlotInterfacialDistributionCurve(RList,InterfacialValuesList,
#                                  saveName=savePath, 
#                                  Xlabel="Radius [m]",\
#                                  Ylabel="InterfacialConcentration", 
#                                  figTitle="Interfacial concentration distribution at time:\n"+str(float(currentTime)/3600.)+" [h] (or "+currentTime+" [s]")
                    
                if filename.endswith("InterfacialVelocities_.txt"):
                    print ("PostProcessing Interfacial Velocities File: <",filename, ">")
                    
                    ###########
                    #create save Directory For Interfacial concentration files#
                    ###########
                    InterfacialVelocityResultsDir=ResultsDir+"InterfacialVelocity"
                    if not os.path.exists(InterfacialVelocityResultsDir):
                        os.makedirs(InterfacialVelocityResultsDir)
                    ###########
                    #create save Directory For Interfacial Velocities files#
                    ###########                    
                    
                    ReadFile=OpenAndReadFilePrecInterf(FileDir+filename)
                    Data=ReadFile[2:]
                    
                    AnyFileRadiusDistribution= os.listdir(RadDisDir+subdirName+"/") [0] #0 or any int, because no matter what the file chosen
                    
                    
                    pathToTheFileRadiusDistribution= RadDisDir+subdirName+"/"+AnyFileRadiusDistribution
                    print (pathToTheFileRadiusDistribution)
                    minRadius, spatialStep= FindMinRadiusAndSpatialStepOfDistribution(pathToTheFileRadiusDistribution)
                    
                    timeList=[]
                    for lineData in Data:
                        timeList.append(lineData[0])
                    timeList=map(float, timeList)
                    
                    closeTimeList=list( arange(timeStep,float(timeList[-1]),timeStep)  )  # Time per steps of 'timeStep' secondes
                    if float(timeList[-1]) not in closeTimeList:closeTimeList.append(timeList[-1])
                    closestTimeList,closestTimeindexList=  ValeurlaPlusProche(closeTimeList,timeList)  
                    
                    
                    for i in closestTimeindexList:
                        data=Data[i]
                        currentTime=data[0]
                        InterfacialValuesList=data[1:]
                        n=len(InterfacialValuesList)
                        RList = makeInterfacialRadiusListForDistribution(minRadius,spatialStep,n )
                        assert( len(RList)==len(InterfacialValuesList) ), "Radius List and Interfacial valist list do not have the same dimensions"
                        
                        
                        saveFigName="IVDistribution_"+currentTime+".pdf"
                        savePath=InterfacialVelocityResultsDir+"/"+saveFigName
                        PlotInterfacialDistributionCurve(RList,InterfacialValuesList,
                                  saveName=savePath, 
                                  Xlabel="Radius [m]",\
                                  Ylabel="InterfacialConcentration", 
                                  figTitle="Interfacial Velocities distribution at time:\n"+str(float(currentTime)/3600.)+" [h] (or "+currentTime+" [s]")                    
                    
                    
#                    for lineData in Data:
#                        currentTime= lineData[0]
#                        InterfacialValuesList= lineData[1:]
#                        n=len(InterfacialValuesList)
#                        RList = makeInterfacialRadiusListForDistribution(minRadius,spatialStep,n )    
#                        assert( len(RList)==len(InterfacialValuesList) ), "Radius List and Interfacial valist list do not have the same dimensions"
#                        
#                        
#                        
#                        saveFigName="IVDistribution_"+currentTime+".pdf"
#                        savePath=InterfacialVelocityResultsDir+"/"+saveFigName
#                        PlotInterfacialDistributionCurve(RList,InterfacialValuesList,
#                                  saveName=savePath, 
#                                  Xlabel="Radius [m]",\
#                                  Ylabel="InterfacialConcentration", 
#                                  figTitle="Interfacial Velocities distribution at time:\n"+str(float(currentTime)/3600.)+" [h] (or "+currentTime+" [s]")
                    
#D=OpenAndReadFilePrecInterf("/home/mgado/work/Git/NuGroDis/gamma_0.0415_L_1.2e-05_t_150000.0__Wed_Dec_02_16hr_50min_37s_2015/PrecipitatesInterfacialProperties/GuinierPreston/GuinierPreston_InterfacialConcentrations_.txt")
#/home/mgado/work/Git/NuGroDis/gamma_0.044_L_3e-05_t_1000.0__Tue_Dec_01_17hr_01min_14s_2015/RadDisFiles/GuinierPreston/RadDis_time_1_.txt               
    
