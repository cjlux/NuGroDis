# -*- coding: utf-8 -*-
from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")

from Metallurgical import Computation, \
                          Quenching, \
                          ThermoDynamicsConstant 
                          
                              

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
                                        nugrodis.PhysicalConstants['Na'][0],)

thermoDynConst.Info();

ComputationList = c.type.split(">")
for computation in ComputationList:
    print("  > Processing <"+computation+">")

    if computation == "Quenching":
        # Create on object of type Quenching:
        Quenching = Quenching(M2024.VacanciesParam["Tsol"][0],
                              nugrodis.QuenchingParam["Tfinal"][0],
                              nugrodis.QuenchingParam["v"][0])
        Quenching.Info();
    if computation is "Hardening":
        pass
    
# some stuff.... for Moubarak !

c.Run()

# éééé
