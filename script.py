import os, sys
import matplotlib.pyplot as plt
from numpy import array


def OutputDistributionCurves(path,Retoile=True):
    """Save distribution Curves"""

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
            if Retoile==True:
                ##Read file Precipitate atrtribute and get information on real critical radius (r* + dr* )
                ReadFilePrecipitateAttribute=OpenFileAndRead(AttributesFileDir+attributeFileName)
                realCriticalRadiusList=[]
                PrecipitateAttributeData=ReadFilePrecipitateAttribute[1:]
                timeList=list(PrecipitateAttributeData[:,0])
                realCriticalRadiusList=list(PrecipitateAttributeData[:,8])
                ##
            
            

            print("RadDisFilesDir", RadDisFilesDir)
            for filename in os.listdir(RadDisFilesDir):
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
                        
                    fig, ax = plt.subplots()
                    ax.plot(RR,NP)
                    ax.grid(True)
                    ax.set_xlabel(u'Rayon [m]',color='green')
                    ax.set_ylabel(u'Nombre de particules N ',color='red')
                    # draw a vertical line x=retoile
                    if Retoile==True:
                        radDisCurrentTime=filename[12:-5]
                        currentTimeIndex=timeList.index(radDisCurrentTime)
                        criticalRadius=realCriticalRadiusList[currentTimeIndex]
                        #print("critical radius is",criticalRadius, "current time", radDisCurrentTime ,"index", currentTimeIndex)
                        ax.axvline(x=criticalRadius,linestyle='--', linewidth=1, color='black',label=u"Position")
                        ax.text(10.1,0,'blah',rotation=90)
                        ax.legend(['Distribution curve', 'Critical radius'] )
                        
                    plt.savefig(saveName)
                    plt.close()


def PlotCurve(X,Y,saveName,Xlabel="",Ylabel="",figTitle="",Xcolor="green",Ycolor="red"):
    "Plot a curve and save it to the given path"
    fig, ax = plt.subplots()
    ax.plot(X,Y,'r-o')
    ax.grid(True)
    plt.title(u""+figTitle)
    ax.set_xlabel(u""+Xlabel,color=Xcolor)
    ax.set_ylabel(u""+Ylabel,color=Ycolor)
    plt.savefig(saveName)
    plt.close()



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
    


def PlotFromReadData(ReadFile,saveDirPath,Yindex,saveFigName=None,Xindex=0,Xlabel=None,Ylabel=None, figTitle="" ):
    """Plot from a data given in argument"""
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
    PlotCurve(xData,attributeToPlotData,savePath,Xlabel=Xlabel,Ylabel=Ylabel,figTitle=figTitle)

 


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





    
            
def OutputAttributes(path):
    """Save Precipitates attributes Curves"""

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

                    #print("toto",len(ReadFile[0]))

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
                                figtitle="Temporal evolution of "+ylabel

                            saveName=ResultsDir+filename[:-15]+attributeToPlot+".pdf"
                            PlotCurve(timeData,attributeToPlotData,saveName,Xlabel='Time [s]',Ylabel=ylabel,figTitle=figtitle)


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
