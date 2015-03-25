from __future__ import division, print_function

import sys
sys.path.append("libMetallurgical/PythonB")


from Metallurgical import Computation
c = Computation()
print(c.type)
c.type = "Quenching"
print(c.type)
