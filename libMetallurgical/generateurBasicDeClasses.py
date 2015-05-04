# -*- coding: utf-8 -*-
import os


globalSaveDirectory="/home/users/mgado/phd/biblio/Git/NuGroDis/libMetallurgical/";
#/home/mgado/diskBasile/phd/PJE9-2014/NuGroDis/libMetallurgical/


#Read classNames text file
listeDesClasses=open(globalSaveDirectory+'listeDesClasses.txt');

# list of classNames
ClassNames=listeDesClasses.read().split();


#create .hpp
for Class in ClassNames:
    if not os.path.exists(globalSaveDirectory+'SRC/'+Class+'.hpp'):
        file = open(globalSaveDirectory+'SRC/'+Class+'.hpp', 'w+')
        file.write('class '+Class+'\n')
        file.write('{\n\n')
        file.write('public:\n')
        file.write(Class+'();\n')
        file.write('~'+Class+'();\n\n')
        file.write('protected:\n\n')
        file.write('private:\n\n')
        file.write('};')

#create .cpp
for Class in ClassNames:
    if not os.path.exists(globalSaveDirectory+'SRC/'+Class+'.cpp'):
        file = open(globalSaveDirectory+'SRC/'+Class+'.cpp', 'w+')
        file.write('')
        file.write('#include "'+Class+'.hpp"\n\n')
        file.write(Class+'::'+Class+'()\n')
        file.write('{\n')
        file.write('}\n\n')
        file.write(Class+'::~'+Class+'()\n')
        file.write('{\n')
        file.write('}\n')
    