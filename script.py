# -*- coding: utf-8 -*-
import os, sys
import matplotlib.pyplot as plt
from numpy import array, arange, sqrt, loadtxt
from matplotlib import rc
import M2024



SpecificValues=["-222.222","-111.111","111.111","222.222"] #DO NOT DELETE THIS LINE!!!



import numpy as np
import matplotlib.pyplot as plt
import time

#x, y = np.random.random((2, 10))
#
#fig, ax = plt.subplots()
#scat = ax.scatter(x, y, s=150)
#
## Show the figure, then remove one point every second.
#fig.show()
#
#for _ in range(10):
#    time.sleep(1)
#    xy = np.delete(scat.get_offsets(), 0, axis=0)
#    scat.set_offsets(xy)
#    plt.draw()
#
##fig, ax = plt.subplots(subplot_kw={'xlim': [0,1],'ylim': [0,1]})
#
#
#fig, ax= plt.subplots()
#ax.plot([1,4,8,9,52], [0.1,2,4,25,5])
#plt.show()
#
#ax.lines.pop(4)
#
##plt.show()
#
#p1, = ax.plot(0.5, 0.5, 'bo') # creates a blue dot
#p2, = ax.plot(0.5, 0.5, 'ro')
#
#p2.remove()



def plotWithSpecificValues(dataY, dataX, specificValueFound, savename, xlabel, ylabel, figTitle, trimcurveinfo, attributeToPlotName=''  ):
    ##################### new; begin 
         #input RealRetoileList or attributeToPlotData or dataY, 
              # specificvalue or '-222.222'
              # dataX=timeData ,
            #is know before function:
         #Xlabel = 'time [s]', ylabel, figTitle, trimCurveInfo=(plotBySequence,seqInfoIndex)
    
    from operator import itemgetter
    from itertools import groupby
    
    BoolCheckIfASpecificValueIsInData, specificValueFound = CheckIfASpecificValueIsInData(dataY)
    assert(BoolCheckIfASpecificValueIsInData==True), 'specific value given is not in given dataY'
    
    allOccurenceWhereDataisNotDedined= [i for i, x in enumerate(dataY) if x == specificValueFound]
 
    
    ## range in list, by concecutive index
    consecutiveOccurenceList=[]
    for k, g in groupby(enumerate(allOccurenceWhereDataisNotDedined), lambda (i,x):i-x):
        consecutiveOccurenceList.append(map(itemgetter(1), g))
    
    fig, axReturned = PlotCurve(X=returnListAfterExcludeIndex(dataX, allOccurenceWhereDataisNotDedined),\
                        Y=returnListAfterExcludeIndex(dataY,allOccurenceWhereDataisNotDedined),\
                        saveName=savename, Xlabel=xlabel,Ylabel=ylabel,figTitle=figTitle,\
                        returnFig=True, trimCurveInfo=trimcurveinfo)
                                     # ==========checking if one ax or if returned axList is a list of ax
                        
    if len(axReturned)==1:
        ax=axReturned
        for occurence in consecutiveOccurenceList:
            if len(occurence)==1:
                singleOccurenceindex=occurence[0]
                ax.annotate(attributeToPlotName+' does not exist', xy=(dataX[singleOccurenceindex], 0),  xycoords='data',
                                xytext=(0.01, 0.99), textcoords='axes fraction',
                                arrowprops=dict(facecolor='black', shrink=0.02),
                                horizontalalignment='right', verticalalignment='top')
            else:
                # consecutive occurence
                beginIndex=occurence[0]
                endIndex=occurence[-1]
                
                xmin,xmax= dataX[beginIndex], dataX[endIndex]
                ax.axvspan(xmin,xmax,hatch='\\\\\\', label=attributeToPlotName +" does not exist",\
                           linestyle='dashed', fill=False,edgecolor="grey")
                ax.legend([attributeToPlotName, attributeToPlotName+' does not exist'],\
                      fancybox=True, shadow=True,loc='upper center')        
                # pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,                                    
    
    else:
        for ax in axReturned:
            for occurence in consecutiveOccurenceList:
                if len(occurence)==1:
                    singleOccurenceindex=occurence[0]
                    ax.annotate(attributeToPlotName+' does not exist', xy=(dataX[singleOccurenceindex], 0),  xycoords='data',
                                xytext=(0.01, 0.99), textcoords='axes fraction',
                                arrowprops=dict(facecolor='black', shrink=0.02),
                                horizontalalignment='right', verticalalignment='top')
                else:
                    # consecutive occurence
                    beginIndex=occurence[0]
                    endIndex=occurence[-1]
                    
                    xmin,xmax= dataX[beginIndex], dataX[endIndex]
                    ax.axvspan(xmin,xmax,hatch='\\\\\\', label=attributeToPlotName + " does not exist",\
                               linestyle='dashed', fill=False,edgecolor="grey")
                    ax.legend([attributeToPlotName, attributeToPlotName+' does not exist'],\
                          fancybox=True, shadow=True,loc='upper center')    
                    # pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,        
     # ==========checking if one ax or if returned axList is a list of ax
            
    ##################### new: end
    return fig,axReturned
    

def returnListAfterExcludeIndex(List, index_toExludeList):
    newList=[element for i, element in enumerate(List) if i not in index_toExludeList]
    return newList


def toto():
    import numpy as np
    import matplotlib.pyplot as plt

    fig = plt.figure()
    ax = fig.add_subplot(111)

    t = np.arange(0.0, 5.0, 0.01)
    s = np.cos(2*np.pi*t)
    line, = ax.plot(t, s, lw=2)

    ax.annotate('local max', xy=(1, 1), xytext=(1, 1.5),
            arrowprops=dict(facecolor='black', shrink=0.01),
            )
    ax.annotate('local max 2', xy=(2, 1),  xycoords='data',
            xytext=(0.5, 0.99), textcoords='axes fraction',
            arrowprops=dict(facecolor='black', shrink=0.05),
            horizontalalignment='right', verticalalignment='top',
            )
    ax.annotate('local max 2', xy=(3, 1),  xycoords='data',
            xytext=(0.5, 0.99), textcoords='axes fraction',
            arrowprops=dict(facecolor='black', shrink=0.02),
            horizontalalignment='right', verticalalignment='top',
            )

    ax.set_ylim(-2,2)
    plt.show()


def howToDOToPlot ():
    
    seqInfo=ReadAndReturnComputationSequenceInfo(path)
    
    seqInfoIndex= ReturnComputationSequenceIndex(seqInfo[1:],time)
    
    OutputMaterialChemicalComposition(path,trimCurveInfo=(True,seqInfoIndex) )
    
    return

def ConvertDataInNumpyArrayFloat(Data):
    """ Convert a given Data into a numpy array """
    
    dataArray=[]
    for row in Data :
        try:
            dataArray.append(map(float,row))
        except:
            print(u"Error when converting to data to float:",row)  
            
    dataArray=array(dataArray)
    
    return dataArray
    


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



def Hardness(path):
    """Comparaison courbe de dureté obtenu par simulation numerique et celle obtenue expértimentalement"""
    

    print("******* POSTPROCESSING HARDNESS CURVES *******")    
    ###########
    #SAVE PATH#
    ###########
    ResultDir=path+"Results/Material/Hardness"
    if not os.path.exists(ResultDir):
        os.makedirs(ResultDir)
    ###########
    #SAVE PATH#
    ###########

    materialCurrentCompoFile=path+"/MaterialCurrentCompo/MaterialChemicalCompo.txt"
    materialCurrentCompoData=OpenFileAndRead(materialCurrentCompoFile)[1:]

    
    
    GuinierPrestonAttributeFile=path+"PrecipitatesAttributes/GuinierPreston/GuinierPreston_Attributes_.txt" 
    GuinierPrestonAttributeData=OpenFileAndRead(GuinierPrestonAttributeFile)[1:]
    
    
    initialGamma=GuinierPrestonAttributeData[0][5]
    initialHalfSinkD=GetInitialHalfSinkD(path)
    
    
    K=40.437    
    XvCu=array(map(float,materialCurrentCompoData[:,2]))*100
    XvMg=array(map(float,materialCurrentCompoData[:,3]))*100
    XatCu=XvCu*1.406
    XatMg=XvMg*0.714
    FvGP=map(float,GuinierPrestonAttributeData[:,1])
    Css=XatCu+XatMg
    SigmaGrain=54 #composante dûe au durcissement par taille de grain en MPa    
    SigmaSS=K*Css**(2/3) #composante dûe au durcissement par solution solide, Sigma= KCss^(2/3), avec Css concentration atomique des solutés
    SigmaGP=791.453*sqrt(FvGP) #composante dûe au durcissement par précipatation des GP
    SigmaTotal=SigmaGrain+SigmaSS+SigmaGP
    a=SigmaGrain+SigmaSS
    H=(SigmaTotal+183)/3.67 # Dureté par la loi H=3.67*H-183, cf. thèse Genevois, relation calculée dans un domaine de dureté compris entre 110 et 160 HV
    
    #print(H)
    

    Hexperimentale=[89,91,92,92.5,95,107,114,118,125,126,128,131,134,134,134]
    texperimentale=[0.05,0.17,0.33,0.5,1,2,3,4,6,8,24,48,72,80,90] #en heure
    t=array(map(float,materialCurrentCompoData[:,0]))/3600    # temps en heure
    #print(t)
    import matplotlib.pyplot as plt
    fig1,ax = plt.subplots()
    ax.plot(t,H,'r-o')
    ax.plot(texperimentale,Hexperimentale,'b-o')    
    ax.set_title(u'Numerical simulation and Experimental Hardness Curve\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD)) 
    ax.grid(True)    
    ax.set_xlabel(u"Time [h] ")
    ax.set_ylabel(u"Hardness [HV]",color='green')
    # abaisser la hauteur de l'axe de 10% on the bottom
    box = ax.get_position()
    ax.set_position([box.x0, box.y0 + box.height * 0.1,box.width, box.height * 0.9])
    ax.legend(['Numerical simulation', 'Experimental Curve'],loc='upper center', bbox_to_anchor=(0.5, -0.1),fancybox=True, shadow=True, ncol=5)
    plt.savefig(ResultDir+'/Hardness.pdf')
   
    #courbe sur echelle semi logarithmique
    fig2,ax = plt.subplots()
    ax.semilogx(t,H,'r-o')
    ax.plot(texperimentale,Hexperimentale,'b-o')    
    ax.set_title(u'Numerical simulation and Experimental Hardness Curve, H=f(fvGP)\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD)) 
    ax.grid(True)    
    ax.set_xlabel(u"Time [h] ")
    ax.set_ylabel(u"Hardness [HV]",color='green')
    # abaisser la hauteur de l'axe de 10% on the bottom
    box = ax.get_position()
    ax.set_position([box.x0, box.y0 + box.height * 0.1,box.width, box.height * 0.9])
    ax.legend(['Numerical simulation', 'Experimental Curve'],loc='upper center', bbox_to_anchor=(0.5, -0.1),fancybox=True, shadow=True, ncol=5)
    plt.savefig(ResultDir+'/Hardness_log.pdf')
    
    plt.close("all")
      
    
    return 






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
    
    try:
        #convert into float
        valeur=map(float,valeur)
    except:
        print(u"error while converting into float ", valeur)
    
    try:
        #convert into float
        ColonneData=map(float,ColonneData)
    except:
        print(u"error while converting into float ", ColonneData)
        
    valeur=list(valeur)
    ColonneData=list(ColonneData)
    INDEX=[]
    VPROCHE=[]
    
    for t in valeur:
        if (t in ColonneData):
            valeurProcheIndex= ColonneData.index(t)
            valeurproche=ColonneData[valeurProcheIndex]
            VPROCHE.append(valeurproche)
        else:
            #print(t)
            #print(ColonneData)
            #print(type(ColonneData[0]))
            valeurproche=min(ColonneData, key=lambda x:abs(x-t))
            VPROCHE.append(valeurproche)
    
    #tri necessaire?
    #VPROCHE=list(set(VPROCHE))
    #VPROCHE=sorted(VPROCHE)
           
    
    for x in VPROCHE:
        indexvaleurproche=ColonneData.index(x)
        INDEX.append(indexvaleurproche)
    
    return VPROCHE,INDEX


def ReadAndReturnComputationSequenceInfo(resultDirectory):
    
    if (resultDirectory[-1]=="/"):
        path=resultDirectory
    else:
        path=resultDirectory+"/"
        
    
    path=resultDirectory+"ComputationInfo/ComputationSequenceInfo.txt"
    
    ComputationSequenceInfo=OpenFileAndRead(path) 
    
    return ComputationSequenceInfo
    


def ReturnComputationSequenceIndex(computationInfoData, timeData):
    
    computationSequenceIndex=[]
    
    
    for seq in computationInfoData:
        
        line=[]
        assert(len(seq)==3)
        
        # append name in line
        line.append(seq[0])
        
        # append starting index in line
        valeur=[]
        valeur.append(float(seq[1]))
        Vproche,Index=ValeurlaPlusProche(valeur,timeData)
        assert(len(Vproche)==1)    
        #print(Vproche[0],valeur[0])
        assert(Vproche[0]==valeur[0])
        line.append(Index[0])
        
        # append ending index in line
        valeur=[]
        valeur.append(float(seq[2]))
        Vproche,Index=ValeurlaPlusProche(valeur,timeData)
        assert(len(Vproche)==1)        
        assert(Vproche[0]==valeur[0])
        line.append(Index[0])
        
        
        # append in Data computationSequenceIndex, line=  'name' | 'startingIndex' | 'endingEndex'
        computationSequenceIndex.append(line)
        
    
    return computationSequenceIndex



def PostProcessing(plotBySequence=True,\
                    thermalLoadingTimeStep=10,\
                    hardeningTimeStep=3600,\
                    quenchingTimeStep=1):
    # timeStep is the default time Step if thermal
    f=open("lastResultDirectoryPath.txt","r")
    ComputationResultsDirectory=f.readlines(1)[0] #the first and only line of file "lastResultDirectoryPath.txt"
    f.close()
    
    PostProcessGivenResultDirectory(resultDirectory=ComputationResultsDirectory,\
                                            plotBySequence=plotBySequence,\
                                            thermalLoadingTimeStep=thermalLoadingTimeStep,\
                                            hardeningTimeStep=hardeningTimeStep,\
                                            quenchingTimeStep=quenchingTimeStep)    
    
    
def PostProcessGivenResultDirectory(resultDirectory,plotBySequence=False,\
                                            thermalLoadingTimeStep=2,\
                                            hardeningTimeStep=3600,\
                                            quenchingTimeStep=1):
    
    if (resultDirectory[-1]=="/"):
        path=resultDirectory
    else:
        path=resultDirectory+"/"
    
    
    # 'thermalLoadingTimeStep' or 'hardeningTimeStep' or 'quenchingTimeStep'
    # are only usefull if plotBySequence=True
    
    
    timeData=loadtxt(path+"MaterialCurrentCompo/MaterialChemicalCompo.txt", skiprows=1)[:,0]
    

       
    
    if (plotBySequence==False):
        
        #plot normally
        OutputAttributes(path)
        OutputMaterialChemicalComposition(path)
        OutputMaterialVacancyProperties(path)
        OutputDistributionCurves(path, timeStep=3600, timeToPlot=[]) # by default timeStep is 3600 and timeToPlot=[] (see function)  
        OutputInterfacialDistributionCurves(path, timeStep=3600, timeToPlot=[]) # by default timeStep is 3600 and timeToPlot=[] (see function)  
         
    
        
        
    else:
        seqInfo=ReadAndReturnComputationSequenceInfo(path) [1:]
        seqInfoIndex= ReturnComputationSequenceIndex(seqInfo,timeData)
        
        #OutputMaterialChemicalComposition(path,trimCurveInfo=(True,seqInfoIndex) )
        #OutputMaterialVacancyProperties(path,trimCurveInfo=(True,seqInfoIndex) )        
        OutputAttributes(path,plotBySequence=plotBySequence )

        
        for seq in seqInfo:
            
            listOfTimeToPlot=[]
            
            sequenceName= seq[0]
            startingTime= float(seq[1])
            endingtime  = float(seq[2]) 
            
            
            if (sequenceName=='ThermalLoading'):
                sequenceTimeStep= thermalLoadingTimeStep       
            elif (sequenceName=='Hardening'):
                sequenceTimeStep= hardeningTimeStep           
            elif (sequenceName=='Quenching'):
                sequenceTimeStep= quenchingTimeStep         
            else:
                assert(0), "sequenceName should be either 'Quenching' or 'Hardening' or 'ThermalLoading' "
            
            
            listOfTimeToPlot=list(arange(startingTime,endingtime,sequenceTimeStep))    # Temps par steps de 'timeStep' secondes
            if endingtime not in listOfTimeToPlot:listOfTimeToPlot.append(endingtime)
            
            
            #################################
            #CREATE SEQUENCE SAVE DIRECTORY #
            #################################
            sequenceResultDir=path+sequenceName
            if not os.path.exists(sequenceResultDir):
                os.makedirs(sequenceResultDir)
            #################################
            #CREATE SEQUENCE SAVE DIRECTORY #
            #################################
            
            
            #print('listOfTimeToPlot',listOfTimeToPlot)
            #OutputDistributionCurves(path, timeToPlot=listOfTimeToPlot ) # by default timeStep is 3600 and timeToPlot=[] (see function)  
            #OutputInterfacialDistributionCurves(path, timeToPlot = listOfTimeToPlot) 
            
        ## TODO: sort Distribution curbe by sequence Type ? i.e create folder 'sequenceTypename'
            #coresponding distribution curve
    
    
    Hardness(path)
    
    return


def CheckIfASpecificValueIsInData(data):
        
    ResultBool=False
    valueInData=None
    specificValueFoundList=[]
    for specific in SpecificValues:
        if specific in data:
            ResultBool=True
            valueInData=specific
            specificValueFoundList.append(valueInData)
            assert(len(specificValueFoundList)==1), 'Error: Differents specific values have been found in data'
            
    
    return ResultBool, valueInData
        



def OutputDistributionCurves(path,Retoile=True ,timeToPlot=[], timeStep=3600):
    """Save distribution Curves"""
    
    # option 'timeStep' is only usefull if timeToPlot=[]
    
    

    print("******* POSTPROCESSING DISTRIBUTION CURVES *******")

    if (path[-1]=="/"):
        path=path
    else:
        path=path+"/"
        
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
        
            PrecipitateAttributeData=loadtxt(fname=AttributesFileDir+attributeFileName,skiprows=1)

            
            timeList=list(PrecipitateAttributeData[:,0])
            
            
            initialGamma=PrecipitateAttributeData[0][5]
            initialHalfSinkD=GetInitialHalfSinkD(path)
            
            
            # define Time To plot                    
            if timeToPlot==[]:
                max_Time=timeList[-1] #[end_time_of_Computation]
                timeToPlot=list(arange(timeStep,max_Time,timeStep))    # Temps par steps de 'timeStep' secondes
                if max_Time not in timeToPlot:timeToPlot.append(max_Time)
            # define Time To plot   
            
            assert(timeToPlot!=[]) # here timeToPlot should not be = []
            assert (type(timeToPlot)==list) , " 'timeToPlot' should be a list "

            closestTimeList,closestTimeindexList=  ValeurlaPlusProche(timeToPlot,timeList) 
                
                
                
                
            
#==============================================================================
#             closeTimeList=list( arange(timeStep,timeList[-1],timeStep)  )  # Time per steps of 'timeStep' secondes 
#             
#             if timeList[-1] not in closeTimeList:closeTimeList.append(timeList[-1])
#                 
#                 
#             if (thermalLoadingTimeInterval != [None,None] ) :
#                 assert ( (thermalLoadingTimeInterval[0] != None) and (thermalLoadingTimeInterval[1] != None) )
#                 
#                 thermalLoadingTimeInterval=map(float,thermalLoadingTimeInterval)
#                 assert ( thermalLoadingTimeInterval[0]<thermalLoadingTimeInterval[1] )
#                 
#                 sequenceKeyTime=[]
#                 sequenceKeyTime.append( timeList[0] )
#                 sequenceKeyTime.append( thermalLoadingTimeInterval[0] )
#                 sequenceKeyTime.append( thermalLoadingTimeInterval[1] )
#                 sequenceKeyTime.append( timeList[-1] )
#                 
#                 # sort by increasing order
#                 sequenceKeyTime.sort()
#                 
#                 
#                 
#                 closeTimeThermalLoadingList=list( arange(thermalLoadingTimeInterval[0],\
#                                                             thermalLoadingTimeInterval[1],\
#                                                             thermalLoadingTimeStep)  )  
#                                                             
#                 closeTimeList=closeTimeList+closeTimeThermalLoadingList
#                 
#                 closeTimeList= list(set(closeTimeList))
#                 closeTimeList.sort()
#             closestTimeList,closestTimeindexList=  ValeurlaPlusProche(closeTimeList,timeList)
#==============================================================================
            
            
            if Retoile==True:
                ##From file Precipitate atrtribute, get information on real critical radius (r* + dr* )>>> >>> >>> 

                realCriticalRadiusList=[]
                realCriticalRadiusList=list(PrecipitateAttributeData[:,8])
                ##
            
            
            

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
                        ' with gamma='+str(initialGamma)+'; L='+str(initialHalfSinkD)+'\n')
                        plt.savefig(saveName)
                        plt.close()


def PlotCurve(X,Y,saveName,Xlabel="",Ylabel="",figTitle="",marker="r-o",Xcolor="green",Ycolor="red", returnFig=False, labelCurve=' ', trimCurveInfo=(False,[]), saveSequenceCurveSeparately=True ):
    "Plot a curve and save it to the given path,  sequenceNameStartEndTriplet=(SequenceName,StartingIndex,EndingEndex)="
    #rc('text', usetex=True)
    rc('font', family='serif')
    
    dataWillbePlottedBySequence=trimCurveInfo[0]
    sequencesInfo=trimCurveInfo[1]
    
    if (dataWillbePlottedBySequence==False):
        assert (sequencesInfo==[]) , "sequencesInfo which is trimCurveInfo[1] is not needed"
    else:
        for u in sequencesInfo:
            assert(len(u)==3), " sequenceInfo should be '|sequenceName|StartingIndex|EndingEndex| '  "
        

    
    # marker can be 
            # un-filled markers  ',' , '.', '1' , '2' , '4' , '_' , 'x' , '|' , '0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '+'
            # filled markers 'o', 'v', '^', '<', '>', '8', 's', 'p', '*', 'h', 'H', 'D', 'd'
    
    axToReturn=None # initialisation
    if (dataWillbePlottedBySequence==False):
        # data will be plotted normally
        fig, ax = plt.subplots()
        axToReturn=ax
        ax.plot(X,Y,marker, label=labelCurve)        
        ax.grid(True)
        plt.title(u""+figTitle)
        ax.set_xlabel(u""+Xlabel,color=Xcolor)
        ax.set_ylabel(u""+Ylabel,color=Ycolor)
        ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0., shadow=True, fancybox=True)
    else:
        # data will be plotted by sequence
        
        numberOfSequence= len(sequencesInfo)
      
        
        fig,axList=plt.subplots(numberOfSequence)
        axToReturn=axList
        
        
        plt.subplots_adjust(hspace=0.3)    
        
        
        fig.suptitle(u""+figTitle)   
        
        axList=list(axList)  
 
        for (axSub,sequence) in zip (axList,sequencesInfo):
            
            sequenceName  = sequence[0]
            StartingIndex = int(sequence[1])
            EndingIndex   = int(sequence[2])
            
            if (EndingIndex == len(X)-1 ):
                realEndingIndex=None
            else:
                realEndingIndex=EndingIndex+1
            
            axSub.plot(X[StartingIndex:realEndingIndex],Y[StartingIndex:realEndingIndex],marker, label=sequenceName)        
            axSub.grid(True)
            axSub.set_xlabel(u""+Xlabel,color=Xcolor)
            axSub.set_ylabel(u""+Ylabel,color=Ycolor)
            axSub.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0., shadow=True, fancybox=True)
            
            if (saveSequenceCurveSeparately==True):
                #Remember: sequence subplots will be saved separetely only if saveSequenceCurveSeparately ==True!!!
                ## save each sequence on separate plot
                figSequence, axSequence = plt.subplots()
                axSequence.plot(X[StartingIndex:realEndingIndex],Y[StartingIndex:realEndingIndex],marker, label=labelCurve)        
                axSequence.grid(True)
                plt.title(u""+figTitle+' '+sequenceName)
                axSequence.set_xlabel(u""+Xlabel,color=Xcolor)
                axSequence.set_ylabel(u""+Ylabel,color=Ycolor)
                axSequence.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0., shadow=True, fancybox=True)
                saveNameSequence=saveName[:-4]+'_'+sequenceName+saveName[-4:]
                figSequence.savefig(saveNameSequence, bbox_inches='tight')

    
    if returnFig==False:
        fig.savefig(saveName, bbox_inches='tight') #saveName or Absolute complete path to saveFile
        plt.close("all")
    if returnFig==True:
        assert (axToReturn!=None)
        return fig,axToReturn
    
    
    
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
    


def PlotFromReadData(ReadFile,saveDirPath,Yindex,saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="", marker="r-o", trimCurveInfo=(False,[])  ):
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
        figTitle="Temporal evolution of volumic fraction"

    if attributeToPlot=="DeltaGv":
        Ylabel="Phase change volumic Energy [J/m^3]"
        figTitle="Temporal evolution of Phase change volumique Energy"

    if attributeToPlot=="DeltaGe":
        Ylabel="Elastique distorsion volumique Energy [J/m^3]"
        figTitle="Temporal evolution of distorsion volumique Energy"

    if attributeToPlot=="NuclSiteDensity":
        Ylabel="Nucleation site density [Number of Site per volumic unit]"
        figTitle="Temporal evolution of Nucleation site density"

    if attributeToPlot=="gamma":
        Ylabel="Surface energy [J/m^2]"
        figTitle="Temporal evolution of Surface energy"

    if attributeToPlot=="rEtoile":
        Ylabel="Critical radius [m]"
        figtitle="Temporal evolution of critical radius (r*) "

    if attributeToPlot=="delta_rEtoile":
        Ylabel="Delta critical radius [m]"
        figTitle="Temporal evolution of Delta critical radius (dr*_kT)"

    if attributeToPlot=="Real_rEtoile":
        Ylabel="Real critical radius [m]"
        figTitle="temporal evolution of the critical radius (r* + dr*)"

    if attributeToPlot=="Delta_GEtoile":
        Ylabel="Total critical germination volumic Energy [J/m^3]"
        figTitle="Temporal evolution of critical germination total"

    if attributeToPlot=="Z":
        Ylabel="Zeldovich factor"
        figTitle="Temporal evolution of Zeldovich factor"
        
    if attributeToPlot=="BetaEtoile":
        Ylabel=" Beta* (Beta for critical radius) "
        figTitle="Temporal evolution of Beta* "

    if attributeToPlot=="JEtoile":
        Ylabel="Nucleation Rate [NbPrecipitates/m^3/s]"
        figTitle="Temporal evolution of nucleation rate"

    if attributeToPlot=="XvCuEqSS":
        Ylabel="XvCuEqSS"
        figTitle="Copper equilibrium conc. in Solid Solution(with infinite radius prec.)"
        
    if attributeToPlot=="XvMgEqSS":
        Ylabel="XvMgEqSS"
        figTitle="Magnesium equilibrium conc. in Solid Solution(with infinite radius prec.)"
    ##########   Specific values of Ylabel and figTitle   ###############
        
    savePath=saveDirPath+"/"+saveFigName+".pdf"
    
        
    PlotCurve(xData,attributeToPlotData,savePath,Xlabel=Xlabel,Ylabel=Ylabel,figTitle=figTitle,marker=marker,trimCurveInfo=trimCurveInfo)
    

            

                  


def ReadFileAndPlot(fileName,fileDirPath,saveDirPath,Yindex,Xindex=0, Xlabel=None,Ylabel=None,figTitle="", trimCurveInfo=(False,[]) ):
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
                     saveFigName=saveFigName,Xindex=Xindex, Xlabel=Xlabel,Ylabel=Ylabel,figTitle=figTitle,trimCurveInfo=trimCurveInfo)





def OutputMaterialChemicalComposition(path,trimCurveInfo=(False,[]) ):
    
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

    
    if (path[-1]=="/"):
        path=path
    else:
        path=path+"/"
    
    fileDir=path+"MaterialCurrentCompo/"

    filename="MaterialChemicalCompo.txt"  
    
    
    RF=OpenFileAndRead(fileDir+filename)
    for dataToPlotIndex in range(1, len(RF[0]) ):  
        PlotFromReadData(ReadFile=RF,saveDirPath=ResultDir,Xindex=0, Yindex=dataToPlotIndex,trimCurveInfo=trimCurveInfo)# saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="" 
        PlotFromReadData(ReadFile=RF,saveDirPath=ResultDir,Xindex=4, Yindex=dataToPlotIndex,trimCurveInfo=trimCurveInfo) # Xindex=4, temperature

def OutputMaterialVacancyProperties(path, trimCurveInfo=(False,[]) ):
        
    print("******* POSTPROCESSING MATERIAL VACANCIES PROPERTIES CURVES *******")  
    
    if (trimCurveInfo[0]==False):
        assert (trimCurveInfo[0]==[])
    
    ###########
    #SAVE PATH#
    ###########
    ResultDir=path+"Results/Material/MaterialVacancyProperties"
    if not os.path.exists(ResultDir):
        os.makedirs(ResultDir)
    ###########
    #SAVE PATH#
    ###########

    if (path[-1]=="/"):
        path=path
    else:
        path=path+"/"

    fileDir=path+"MaterialCurrentCompo/"

    
    filename="MaterialVacancyProperties.txt"

    
    RF=OpenFileAndRead(fileDir+filename)
    for dataToPlotIndex in range(1, len(RF[0]) ):  
        PlotFromReadData(ReadFile=RF,saveDirPath=ResultDir,Yindex=dataToPlotIndex,trimCurveInfo=trimCurveInfo)#,saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="" )

    


def OutputMaterialSaveFile(path, trimCurveInfo=(False,[]) ):
    
    ###########
    #SAVE PATH#
    ###########
    ResultDir=path+"Results/Material/"
    if not os.path.exists(ResultDir):
        os.makedirs(ResultDir)
    ###########
    #SAVE PATH#
    ###########

    if (path[-1]=="/"):
        path=path
    else:
        path=path+"/"

    fileDir=path+"MaterialCurrentCompo/"


    
    for filename in os.listdir(fileDir):
        if filename.endswith(".txt"):
            RF=OpenFileAndRead(fileDir+filename)

            
            for dataToPlotIndex in range(1, len(RF[0]) ):  
                PlotFromReadData(ReadFile=RF,saveDirPath=ResultDir,Yindex=dataToPlotIndex,trimCurveInfo=trimCurveInfo)#,saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="" )
        
    

    
            
def OutputAttributes(path,plotBySequence=False):
    """Save Precipitates attributes Curves"""
    
    print("******* POSTPROCESSING PRECIPITATE ATTRIBUTES CURVES *******")


    if (path[-1]=="/"):
        path=path
    else:
        path=path+"/"

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
                    
#==================================WHY?? A MISTAKE============================================
#                     if float(Data[0][0])==0: #Data[0][0] should be the first value of the time column
#                         #skip line time=0. Dont use data where time = 0
#                         Data=ReadFile[2:]
#==============================================================================
                    
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
                    
                    temperatureData=AttributesDataDict["Temperature"]
                    
                    
                    
                    
                                       
                    
                    #########################
                    seqInfoIndex=[]
                    if (plotBySequence==False):
                        seqInfoIndex=[]
                    else:
                        computationInfoData=ReadAndReturnComputationSequenceInfo(path)
                        seqInfoIndex= ReturnComputationSequenceIndex(computationInfoData[1:],timeData)
                    
                    ###############################
                    

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
                                    figtitle="Temporal evolution of volumic fraction"
                                    figtitleVsTemperature="Thermal evolution of volumic fraction"

                                if attributeToPlot=="DeltaGv":
                                    ylabel="$\Delta G_v$ [$J.m^{3}$]"
                                    figtitle="Temporal evolution of Phase change volumique Energy"
                                    figtitleVsTemperature="Thermal evolution of Phase change volumique Energy"

                                if attributeToPlot=="DeltaGe":
                                    ylabel="$\Delta G_{el}$ [$J.m^{-3}$]"
                                    figtitle="Temporal evolution of distorsion volumique Energy"
                                    figtitleVsTemperature="Thermal evolution of distorsion volumique Energy"

                                if attributeToPlot=="NuclSiteDensity":
                                    ylabel="Nsites [Number per volumic unit]"
                                    figtitle="Temporal evolution of Nucleation site density"
                                    figtitleVsTemperature="Thermal evolution of Nucleation site density"

                                elif attributeToPlot=="gamma":
                                    ylabel="Surface energy [$J.m^{-2}$]"
                                    figtitle="Temporal evolution of Surface energy"
                                    figtitleVsTemperature="Thermal evolution of Surface energy"

                                if attributeToPlot=="rEtoile":
                                    ylabel="Critical radius [m]"
                                    figtitle="Temporal evolution of critical radius (r*) "
                                    figtitleVsTemperature="Thermal evolution of critical radius (r*) "

                                elif attributeToPlot=="delta_rEtoile":
                                    ylabel=" $\delta r{*}_{kT}$ [$m$]"
                                    figtitle="Temporal evolution of Delta critical radius (dr*_kT)"
                                    figtitleVsTemperature="Thermal evolution of Delta critical radius (dr*_kT)"

                                if attributeToPlot=="Real_rEtoile":
                                    ylabel="Real critical radius [m]"
                                    figtitle="temporal evolution of the critical radius (r* + dr*)"
                                    figtitleVsTemperature="Thermal evolution of the critical radius (r* + dr*)"

                                if attributeToPlot=="Delta_GEtoile":
                                    ylabel="$\Delta G^{*}$ [$J.m^{3}$]"
                                    figtitle="Temporal evolution of critical germination total"
                                    figtitleVsTemperature="Thermal evolution of critical germination total"

                                if attributeToPlot=="Z":
                                    ylabel="Zeldovich factor"
                                    figtitle="Temporal evolution of Zeldovich factor"
                                    figtitleVsTemperature="Thermal evolution of Zeldovich factor"
                                    
                                if attributeToPlot=="BetaEtoile":
                                    ylabel=" Beta* (Beta for critical radius) "
                                    figtitle="Temporal evolution of Beta* "
                                    figtitleVsTemperature="Thermal evolution of Beta*"

                                if attributeToPlot=="JEtoile":
                                    ylabel=r"Nucleation Rate [NbPrecipitates.m^{3}.s^{-1}]"
                                    figtitle="Temporal evolution of nucleation rate"
                                    figtitleVsTemperature="Thermal evolution of nucleation rate"

                                if attributeToPlot=="XvCuEqSS":
                                    ylabel="XvCuEqSS"
                                    figtitle="Copper equilibrium concentration in Solid Solution\n(with infinite radius prec.)"
                                    figtitleVsTemperature="Thermal evolution of in Solid Solution\n(with infinite radius prec.)"

                                if attributeToPlot=="XvMgEqSS":
                                    ylabel="XvMgEqSS"
                                    figtitle="Magnesium equilibrium concentration in Solid Solution\n(with infinite radius prec.)"
                                    figtitleVsTemperature="Thermal evolution of in Solid Solution\n(with infinite radius prec.)"
                                
                            else:
                                ylabel=attributeToPlot
                                figtitle="Temporal evolution of "+ylabel
                                figtitleVsTemperature="Thermal evolution of "+ylabel

                            #### SAVE NAME  ####
                            saveName=ResultsDir+filename[:-15]+attributeToPlot+".pdf"
                            saveNameTemperature=ResultsDir+filename[:-15]+attributeToPlot+"_VsTemperature.pdf"
                            ####################
                            
                            if ( (attributeToPlot=="Real_rEtoile")&("-222.222" in attributeToPlotData) ):
                                RealRetoileList=list(attributeToPlotData)
                                firstOccurenceWhereRealRetoileDoesntExist=RealRetoileList.index("-222.222")

                                
                                
#                                ##################### new; begin 
#                                     #input RealRetoileList or attributeToPlotData or dataY, 
#                                          # specificvalue or '-222.222'
#                                          # dataX=timeData ,
#                                        #is know before function:
#                                     #Xlabel = 'time [s]', ylabel, figTitle, trimCurveInfo=(plotBySequence,seqInfoIndex)
#                                allOccurenceWhereRealRetoileDoesntExist= [i for i, x in enumerate(RealRetoileList) if x == "-222.222"]
#             
#                                
#                                ## range in list, by concecutive index
#                                consecutiveOccurenceList=[]
#                                for k, g in groupby(enumerate(allOccurenceWhereRealRetoileDoesntExist), lambda (i,x):i-x):
#                                    consecutiveOccurenceList.append(map(itemgetter(1), g))
#                                
#                                fig, axReturned = PlotCurve(X=returnListAfterExcludeIndex(timeData, allOccurenceWhereRealRetoileDoesntExist),\
#                                                    Y=returnListAfterExcludeIndex(attributeToPlotData,allOccurenceWhereRealRetoileDoesntExist),\
#                                                    saveName, Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle,\
#                                                    returnFig=True, trimCurveInfo=(plotBySequence,seqInfoIndex))
#                                                                 # ==========checking if one ax or if returned axList is a list of ax
#                                if len(axReturned)==1:
#                                    ax=axReturned
#                                    for occurence in consecutiveOccurenceList:
#                                        if len(occurence)==1:
#                                                singleOccurenceindex=occurence[0]
#                                                ax.annotate('does not exist', xy=(timeData[singleOccurenceindex], 0),  xycoords='data',
#                                                            xytext=(0.01, 0.99), textcoords='axes fraction',
#                                                            arrowprops=dict(facecolor='black', shrink=0.02),
#                                                            horizontalalignment='right', verticalalignment='top')
#                                        else:
#                                            # consecutive occurence
#                                            beginIndex=occurence[0]
#                                            endIndex=occurence[-1]
#                                            
#                                            xmin,xmax= timeData[beginIndex], timeData[endIndex]
#                                            ax.axvspan(timeWhereRealCriticalRadiusStop,xmax,hatch='\\\\\\', label="Retoile does not exist",\
#                                                       linestyle='dashed', fill=False,edgecolor="grey")
#                                            ax.legend(["Real Critical radius", 'Critical radius does not exist'],\
#                                                  fancybox=True, shadow=True,loc='upper center')        
#                                            # pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,                                    
#                                
#                                else:
#                                    for ax in axReturned:
#                                        for occurence in consecutiveOccurenceList:
#                                        if len(occurence)==1:
#                                                singleOccurenceindex=occurence[0]
#                                                ax.annotate('does not exist', xy=(timeData[singleOccurenceindex], 0),  xycoords='data',
#                                                            xytext=(0.01, 0.99), textcoords='axes fraction',
#                                                            arrowprops=dict(facecolor='black', shrink=0.02),
#                                                            horizontalalignment='right', verticalalignment='top')
#                                        else:
#                                            # consecutive occurence
#                                            beginIndex=occurence[0]
#                                            endIndex=occurence[-1]
#                                            
#                                            xmin,xmax= timeData[beginIndex], timeData[endIndex]
#                                            ax.axvspan(timeWhereRealCriticalRadiusStop,xmax,hatch='\\\\\\', label="Retoile does not exist",\
#                                                       linestyle='dashed', fill=False,edgecolor="grey")
#                                            ax.legend(["Real Critical radius", 'Critical radius does not exist'],\
#                                                  fancybox=True, shadow=True,loc='upper center')    
#                                            # pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,        
#                                 # ==========checking if one ax or if returned axList is a list of ax
#                                
#                                ##################### new: end
                                
                            
#                                ############## old way good
#                                # Vs time
#                                timeWhereRealCriticalRadiusStop= timeData[firstOccurenceWhereRealRetoileDoesntExist]
#
#                                
#                                fig, axReturned = PlotCurve(timeData[:firstOccurenceWhereRealRetoileDoesntExist],\
#                                                    attributeToPlotData[:firstOccurenceWhereRealRetoileDoesntExist],saveName,\
#                                                    Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle, returnFig=True, trimCurveInfo=(plotBySequence,seqInfoIndex))
#                                #ax.axvline(x=timeWhereRealCriticalRadiusStop,linestyle='--', linewidth=1, color='blue')
#                                
#                                 # ==========checking if one ax or if returned axList is a list of ax
#                                if len(axReturned)==1:
#                                    ax=axReturned
#                                    xmin,xmax= ax.get_xlim()
#                                    ax.axvspan(timeWhereRealCriticalRadiusStop,xmax,hatch='\\\\\\', label="Retoile does not exist",\
#                                               linestyle='dashed', fill=False,edgecolor="grey")
#                                    ax.legend(["Real Critical radius", 'Critical radius does not exist'],\
#                                          fancybox=True, shadow=True,loc='upper center')
#                                    # pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,                                    
#                                else:
#                                    for ax in axReturned:                    
#                                        xmin,xmax= ax.get_xlim()
#                                        ax.axvspan(timeWhereRealCriticalRadiusStop,xmax,hatch='\\\\\\', label="Retoile does not exist",\
#                                                       linestyle='dashed', fill=False,edgecolor="grey")
#                                        ax.legend(["Real Critical radius", 'Critical radius does not exist'],\
#                                                  fancybox=True, shadow=True,loc='upper center')
#                                            # pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,        
#                                 # ==========checking if one ax or if returned axList is a list of ax
#                                                  
#                                #== old way vs time
#
#                                fig.suptitle(figtitle+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
#                                fig.savefig(saveName,bbox_inches='tight')
#                                plt.close()
#                                
#                                ############### Vs temperature
#                                tempWhereRealCriticalRadiusStop= temperatureData[firstOccurenceWhereRealRetoileDoesntExist]
#                                figT, axT_returned = PlotCurve(temperatureData[:firstOccurenceWhereRealRetoileDoesntExist],\
#                                                    attributeToPlotData[:firstOccurenceWhereRealRetoileDoesntExist],saveName,\
#                                                    Xlabel='Temperature [K]',Ylabel=ylabel,figTitle=figtitleVsTemperature, returnFig=True, trimCurveInfo=(plotBySequence,seqInfoIndex))
#                                
#                                # ==========checking if one ax or if returned axList is a list of ax
#                                if len(axT_returned)==1:
#                                    # only one ax
#                                    axT=axT_returned
#                                    xmin,xmax= axT.get_xlim()
#                                    axT.axvspan(tempWhereRealCriticalRadiusStop,xmax,hatch='\\\\\\', label="Retoile does not exist",\
#                                               linestyle='dashed', fill=False,edgecolor="grey")        
#                                    axT.legend(["Real Critical radius", 'Critical radius does not exist'],\
#                                          fancybox=True, shadow=True,loc='upper center')                                    
#                                else:
#                                    # several ax
#                                    for axT in axT_returned:
#                                        xmin,xmax= axT.get_xlim()
#                                        axT.axvspan(tempWhereRealCriticalRadiusStop,xmax,hatch='\\\\\\', label="Retoile does not exist",\
#                                                   linestyle='dashed', fill=False,edgecolor="grey")        
#                                        axT.legend(["Real Critical radius", 'Critical radius does not exist'],\
#                                              fancybox=True, shadow=True,loc='upper center')                                           
#                                 # ==========checking if one ax or if returned axList is a list of ax
#                                
#                                figT.suptitle(figtitleVsTemperature+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
#                                figT.savefig(saveNameTemperature,bbox_inches='tight')
#                                plt.close()                                         

                            BoolCheckIfASpecificValueIsInData, specificValueFound = CheckIfASpecificValueIsInData(attributeToPlotData)
                            if ( BoolCheckIfASpecificValueIsInData==True ):
                                #print('attribute to plat name', attributeToPlot, 'specific value found ',specificValueFound,'type',type(specificValueFound))
                                AttributeToplotDataList=list(attributeToPlotData)
                                firstOccurenceWhereSpecificValueFound=AttributeToplotDataList.index(specificValueFound)
                                 
                                 

                                #time
                                fig, axReturned=plotWithSpecificValues(dataY=AttributeToplotDataList,\
                                                        dataX=timeData, \
                                                        specificValueFound=specificValueFound,\
                                                        savename=saveName,\
                                                        xlabel='Time [s]',\
                                                        ylabel=ylabel,\
                                                        figTitle=figtitle, \
                                                        trimcurveinfo=(plotBySequence,seqInfoIndex), \
                                                        attributeToPlotName=attributeToPlot  )
                                
                                fig.suptitle(figtitle+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
                                fig.savefig(saveName,bbox_inches='tight')
                                plt.close()
                                 
                                 
                                 
                                # temperature
                                figT, axTReturned=plotWithSpecificValues(dataY=AttributeToplotDataList,\
                                                        dataX=temperatureData, \
                                                        specificValueFound=specificValueFound,\
                                                        savename=saveName,\
                                                        xlabel='Temperature [K]',\
                                                        ylabel=ylabel,\
                                                        figTitle=figtitleVsTemperature, \
                                                        trimcurveinfo=(plotBySequence,seqInfoIndex), \
                                                        attributeToPlotName=attributeToPlot  )
                                figT.suptitle(figtitleVsTemperature+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
                                figT.savefig(saveNameTemperature,bbox_inches='tight')
                                plt.close()
                                 
#                                 ## old working way
#                                 #Vs time
#                                 timeWhereSpecificValueFound=timeData[firstOccurenceWhereSpecificValueFound]
#                                 
#
#                                 fig, axList = PlotCurve(timeData[:firstOccurenceWhereSpecificValueFound],\
#                                                    attributeToPlotData[:firstOccurenceWhereSpecificValueFound],saveName,\
#                                                    Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle, returnFig=True, trimCurveInfo=(plotBySequence,seqInfoIndex) )
#                                 
#                                 
#  
#                                 
#                                 # ==========checking if one ax or if returned axList is a list of ax
#                                 if len(axList)==1:
#                                     # just one and only one ax
#                                     ax=axList                                     
#                                     xmin,xmax= ax.get_xlim()
#                                     ax.axvspan(timeWhereSpecificValueFound,xmax,hatch='\\\\\\', label=attributeToPlot+" does not exist",\
#                                               linestyle='dashed', fill=False,edgecolor="grey")
#                                     ax.legend([attributeToPlot, attributeToPlot+" does not exist"],\
#                                          fancybox=True, shadow=True,loc='upper center')
#                                 else:
#                                     # several ax
#                                     for ax in axList:
#                                         xmin,xmax= ax.get_xlim()
#                                         ax.axvspan(timeWhereSpecificValueFound,xmax,hatch='\\\\\\', label=attributeToPlot+" does not exist",\
#                                                   linestyle='dashed', fill=False,edgecolor="grey")
#                                         ax.legend([attributeToPlot, attributeToPlot+" does not exist"],\
#                                              fancybox=True, shadow=True,loc='upper center')
#                                              #pour avoir legend à droite: bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.,
#                                  # ==========checking if one ax or if returned axList is a list of ax
#                                  
#                                     
#
#                                 fig.suptitle(figtitle+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
#                                 suptitle.savefig(saveName,bbox_inches='tight')
#                                 plt.close()
#                                 
#                                 
#                                 # Vs temperature
#                                 temperatureWhereSpecificValueFound= temperatureData[firstOccurenceWhereSpecificValueFound]
#                                 figT, axTList= PlotCurve(temperatureData[:firstOccurenceWhereSpecificValueFound],\
#                                                    attributeToPlotData[:firstOccurenceWhereSpecificValueFound],saveName,\
#                                                    Xlabel='Temperature [K]',Ylabel=ylabel,figTitle=figtitleVsTemperature, returnFig=True, trimCurveInfo=(plotBySequence,seqInfoIndex) )
#                                 
#                                 
#                                 # ==========checking if one ax or if returned axList is a list of ax
#                                 if len(axTList)==1:
#                                     axT=axTList
#                                     xmin,xmax= axT.get_xlim()
#                                     axT.axvspan(temperatureWhereSpecificValueFound,xmax,hatch='\\\\\\', label=attributeToPlot+" does not exist",\
#                                               linestyle='dashed', fill=False,edgecolor="grey")
#                                     axT.legend([attributeToPlot, attributeToPlot+" does not exist"],\
#                                          fancybox=True, shadow=True,loc='upper center') 
#                                 else:
#                                     for axT in axTList:
#                                         xmin,xmax= axT.get_xlim()
#                                         axT.axvspan(temperatureWhereSpecificValueFound,xmax,hatch='\\\\\\', label=attributeToPlot+" does not exist",\
#                                                   linestyle='dashed', fill=False,edgecolor="grey")
#                                         axT.legend([attributeToPlot, attributeToPlot+" does not exist"],\
#                                              fancybox=True, shadow=True,loc='upper center')      
#                                # ==========checking if one ax or if returned axList is a list of ax
#                                 
#                                 figT.suptitle(figtitleVsTemperature+'\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD))
#                                 figT.savefig(saveNameTemperature,bbox_inches='tight')
#                                 plt.close()      
#                                 ## old working way
                                                                
                            else:
                                PlotCurve(timeData,attributeToPlotData,saveName,Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle+\
                                '\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD), trimCurveInfo=(plotBySequence,seqInfoIndex))
                                
                                ### thermal evolution : attribute Vs temperature
                                PlotCurve(temperatureData,attributeToPlotData,saveNameTemperature,Xlabel='Temperature [K]',Ylabel=ylabel,figTitle=figtitleVsTemperature+\
                                '\ngamma='+str(initialGamma)+'; L='+str(initialHalfSinkD), trimCurveInfo=(plotBySequence,seqInfoIndex))
                                
                                
    return

def ProcessUnfinishedComputation(path):
    #OutputDistributionCurves(path)
    OutputAttributes(path)
    OutputMaterialChemicalComposition(path)
    OutputMaterialVacancyProperties(path)





def OutputInterfacialDistributionCurves(path, timeToPlot=[], timeStep=3600):
    """Save Precipitates Interfacial Distribution Curves"""
    
    
    # option 'timeStep' is only usefull if timeToPlot=[]
    
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
            
            
            
            
            
                          
            
            
            
#================================= plot oly on thermal loading=============================================
# 
#             
#             if (thermalLoadingTimeInterval != [None,None] ) :
#                 timeData=loadtxt(path+"MaterialCurrentCompo/MaterialChemicalCompo.txt", skiprows=1)[:,0]
#                 timeList=list(timeData)
#                 
#                 closeTimeList=list( arange(timeStep,timeList[-1],timeStep)  )  # Time per steps of 'timeStep' secondes 
#                 if timeList[-1] not in closeTimeList:closeTimeList.append(timeList[-1])
# 
#                 assert ( (thermalLoadingTimeInterval[0] != None) and (thermalLoadingTimeInterval[1] != None) )
#                 
#                 thermalLoadingTimeInterval=map(float,thermalLoadingTimeInterval)
#                 assert ( thermalLoadingTimeInterval[0]<thermalLoadingTimeInterval[1] )
#                 
#                 sequenceKeyTime=[]
#                 sequenceKeyTime.append( timeList[0] )
#                 sequenceKeyTime.append( thermalLoadingTimeInterval[0] )
#                 sequenceKeyTime.append( thermalLoadingTimeInterval[1] )
#                 sequenceKeyTime.append( timeList[-1] )
#                 
#                 # sort by increasing order
#                 sequenceKeyTime.sort()
#                 
#                 
#                 
#                 closeTimeThermalLoadingList=list( arange(thermalLoadingTimeInterval[0],\
#                                                             thermalLoadingTimeInterval[1],\
#                                                             thermalLoadingTimeStep)  )  
#                                                             
#                 closeTimeList=closeTimeList+closeTimeThermalLoadingList
#                 
#                 closeTimeList= list(set(closeTimeList))
#                 closeTimeList.sort()
#             
#             closestTimeList,closestTimeindexList=  ValeurlaPlusProche(closeTimeList,timeList)
#==============================================================================
            
            
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
                    
                    # define Time To plot                    
                    if timeToPlot==[]:
                        max_Time=timeList[-1] #[end_time_of_Computation]
                        timeToPlot=list(arange(timeStep,max_Time,timeStep))    # Temps par steps de 'timeStep' secondes
                        if max_Time not in timeToPlot:timeToPlot.append(max_Time)
                    # define Time To plot
                    
                    assert(timeToPlot!=[]) 
                    assert (type(timeToPlot)==list) , " 'timeToPlot' should be a list "
                    
                    closestTimeList,closestTimeindexList=  ValeurlaPlusProche(timeToPlot,timeList) 
                    
                    
                    
                    
                   
                    
                    
                    
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
                    
                    # define Time To plot
                    if timeToPlot==[]:
                        max_Time=timeList[-1] #[end_time_of_Computation]
                        timeToPlot=list(arange(timeStep,max_Time,timeStep))    # Temps par steps de 'timeStep' secondes
                        if max_Time not in timeToPlot:timeToPlot.append(max_Time)
                    # define Time To plot
                    assert(timeToPlot!=[]) 
                    assert (type(timeToPlot)==list) , " 'timeToPlot' should be a list "
                    
                    
                    closestTimeList,closestTimeindexList=  ValeurlaPlusProche(timeToPlot,timeList)  
                    
                    
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
    return

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



                    
#D=OpenAndReadFilePrecInterf("/home/mgado/work/Git/NuGroDis/gamma_0.0415_L_1.2e-05_t_150000.0__Wed_Dec_02_16hr_50min_37s_2015/PrecipitatesInterfacialProperties/GuinierPreston/GuinierPreston_InterfacialConcentrations_.txt")
#/home/mgado/work/Git/NuGroDis/gamma_0.044_L_3e-05_t_1000.0__Tue_Dec_01_17hr_01min_14s_2015/RadDisFiles/GuinierPreston/RadDis_time_1_.txt               
    
