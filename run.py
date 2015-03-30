# -*- coding: utf-8 -*-
from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")

from Metallurgical import Computation, \
                          Quenching, \
                          ThermoDynamicsConstant ,\
                          Vacancy
                          
                              

# We execute le file Nugrodis.py:
import nugrodis

# Create on object of type Computation:
c = Computation()
c.type = nugrodis.ComputationParam["Type"]

material = nugrodis.ComputationParam["Material"]
print("  > importing module",material)
exec "import "+material

print("  > Computation.type : ", c.type)

# Create an object of type ThermoDynamicsConstant:
thermoDynConst = ThermoDynamicsConstant(nugrodis.PhysicalConstants['R'][0],
                                        nugrodis.PhysicalConstants['kB'][0],
                                        nugrodis.PhysicalConstants['Na'][0])

thermoDynConst.Info();

# Create an object of type Vacancy:
Vcy= Vacancy(nugrodis.VacanciesParam['deltaHF'][0],
             nugrodis.VacanciesParam['deltaSF'][0],
             nugrodis.VacanciesParam['deltaHM'][0],
             nugrodis.VacanciesParam['fE'][0],
             nugrodis.VacanciesParam['Dlac0'][0],
             nugrodis.VacanciesParam['halfSinkD'][0], 
             M2024.VacanciesParam['Tsol'][0],
             nugrodis.VacanciesParam['EVacCu'][0],
             nugrodis.VacanciesParam['EVacMg'][0])
Vcy.Info();

ComputationList = c.type.split(">")
for computation in ComputationList:
    print("  > Processing <"+computation+">")

    if computation == "Quenching":
        # Create on object of type Quenching:
        Quenching = Quenching(M2024.VacanciesParam["Tsol"][0],
                              nugrodis.QuenchingParam["Tfinal"][0],
                              nugrodis.QuenchingParam["v"][0])
        Quenching.Info();
    if computation == "Hardening":
        # Create on object of type Hardening:
        pass
    
# some stuff.... for Moubarak !

c.Run()

# éééé
