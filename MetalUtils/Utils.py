def SumOfCoefficients(chemicalComposition):
    # some stuff...
    return 0.


def CopyAllDicoInNewPyFile(material,outputName=None, OutPutFolderPath=None):
    """Copy all dictionary of a file name 'material'.py  . Name \
    material is given in argument  """    

    if OutPutFolderPath is None:
        OutPutFolderPath=""
    else:
        OutPutFolderPath=OutPutFolderPath+"/"
    
    if outputName is None:
        outputName=material+'_DicoCopy'
        

    exec "import "+material
    newF=open(OutPutFolderPath+outputName+".py",'w')
    exec 'L = dir('+material+')'
    for e in L:
        if e[0] is not '_':
            exec 'E ='+material+'.'+e
            if type(E)==dict :
                #print(E)
                if len(E.items())!=0:
                    newF.write(str(e)+"={")
                    newF.write("\n")
                    newF.write( '"'+ E.items()[0][0] + '"' )
                    newF.write(":")
                    #if type(E.items()[0][1]) is str:
                        #newF.write( '"'+ str(E.items()[0][1]) + '"' )
                    #elif type(E.items()[0][1]) is not str:
                        #newF.write( str(E.items()[0][1]) )
                    for items in E.items():
                        if items==E.items()[0]: #First key
                            if type(items[1]) is str:
                                newF.write( '"'+ str(items[1]) + '"' )
                            elif type(items[1]) is not str:
                                newF.write( str(items[1]) )
                        else: # Not first key
                            newF.write(",")
                            newF.write("\n")
                            newF.write( '"'+ str(items[0]) + '"' )
                            newF.write(":")
                            if type(items[1]) is str:
                                newF.write( '"'+ str(items[1]) + '"' )
                            elif type(items[1]) is not str:    
                                newF.write( str(items[1]) )
                    newF.write("\n")
                    newF.write("}")
                    newF.write("\n\n")
                else: # len(E.items())==0
                    newF.write(str(e)+"={}")
                    newF.write("\n\n")
    newF.close()
    
    #Check if the copy is identic to the original
    CheckIfAllDicoInTwoPyFilesAreTheSame(material, outputName)



def CheckIfAllDicoInTwoPyFilesAreTheSame(File1,File2):
    """File1 and File2 must be string  'File1' 'File2'  """
    import sys    
    
    exec "import "+File1 + " as File1"
    exec "import "+File2 + " as File2"    
    print ("Checking if the file ",File1,"have the same dictionary of file", File2 )
    
    L1 = dir(File1)
    L2 = dir(File2)
        
    
    DictionariesNamesOfFile1=[]
    DictionariesNamesOfFile2=[]
    
    
    for e1 in L1:
        if e1[0] is not '_':
            exec "E1 =File1."+e1
            if type(E1)==dict :
                DictionariesNamesOfFile1.append(e1)
    for e2 in L2:
        if e2[0] is not '_':
            exec "E2=File2."+e2
            if type(E2)==dict :
                DictionariesNamesOfFile2.append(e2)
    
    assert( len(DictionariesNamesOfFile1) == len ( DictionariesNamesOfFile2) ),"\
    The 2 files File1 and File2 are differents, number of dictionaries is not\
    the same!!!!"
    
    print("Number of Dictionaries in both files is : ", len(DictionariesNamesOfFile1) )
    for dicoName1, dicoName2 in zip(DictionariesNamesOfFile1,DictionariesNamesOfFile2):
        exec "assert (File1."+dicoName1 + " == File2."+dicoName1+"),'The copy\
        of a dictionary is not the same as the original ' "
        exec "assert (File1."+dicoName2 + " == File2."+dicoName2+"),'The copy\
        of a dictionary is not the same as the original ' "



def PrintAllDico(material):
    """Print all dictionaries of a file name 'material'.py    """    

    exec "import "+material+" as module"  
    L = dir(module)
    print ("Filename is : ",material)
    for e in L:
        if e[0] is not '_':
            exec "E =module."+e
            if type(E)==dict :
                print ("Dictionary Name <"+str(e)+">; Dico ==>",E)
