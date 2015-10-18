# -*- Coding: utf-8 -*-
from __future__ import division, print_function

import sys, os

# get the users's home directory:
usersNugrodisRoot = {'/home/jlc'        : "/work/Projets/NuGroDis/",
                     '/home/users/jlc'  : "/work/Projets/NuGroDis/",                    
                     '/home/gado'       : "/phd/biblio/Git/NuGroDis/",
                     '/home/users/gado' : "/phd/biblio/Git/NuGroDis/"}
userHome = os.path.expanduser("~")

if usersNugrodisRoot.get(userHome, None) is None:
    print("Unknown user environment : please update the dictionnary 'usersNugrodisRoot' ...")
    sys.exit()

nugrodisRoot = userHome + usersNugrodisRoot[userHome]

    
libMetallurgical_dir    = nugrodisRoot + "libMetallurgical/"
libMetallurgicalSRC_dir = libMetallurgical_dir + "SRC/"
#/home/mgado/diskBasile/phd/PJE9-2014/NuGroDis/libMetallurgical/

#Read classNames text file
fileName = libMetallurgical_dir + 'listeDesClasses.txt'
with open(fileName, "r") as classesList:
    # list of classNames
    classNames = classesList.read().split()

print(classNames)

#create .hpp
for className in classNames:
    class_hpp = libMetallurgicalSRC_dir + className + '.hpp'
    class_cpp = libMetallurgicalSRC_dir + className + '.cpp'
    
    if not os.path.exists(class_hpp):
        print("\tCreating file <" + class_hpp + ">")
        file = open(class_hpp, 'w')
        file.write('class ' + className + '\n')
        file.write('{\n')
        file.write('public:\n')
        file.write('  ' + className + '();\n')
        file.write('  ' + '~' + className + '();\n\n')
        file.write('protected:\n\n')
        file.write('private:\n\n')
        file.write('};')
        file.close()

    #create .cpp
    if not os.path.exists(class_cpp):
        print("\tCreating file <" + class_cpp + ">")
        file = open(class_cpp, 'w')
        file.write('#include "' + className + '.hpp"\n\n')
        file.write(className + '::' + className + '()\n')
        file.write('{\n}\n\n')
        file.write(className + '::~' + className + '()\n')
        file.write('{\n}\n')
        file.close()
        
