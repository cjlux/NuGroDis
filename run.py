from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")

# We execute le file Nugrodis.py:
import nugrodis

from Metallurgical import Computation
c = Computation()
c.type = nugrodis.ComputationParam["Type"]

print("Computation.type : ", c.type)

ComputationList = c.type.split(">")
for computation in ComputationList:
    print("Processing <"+computation+">")
          
# some stuff.... for Moubarak !


c.Run()

