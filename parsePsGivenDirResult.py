# -*- coding: utf-8 -*-


import script as impScript
import argparse



parser = argparse.ArgumentParser()

parser.add_argument('-p', '--path', type=str, help="file to plot, or path to results directory")
parser.add_argument("-pbs", "--plotBySequence", nargs='?', type=bool, dest="plotBySequence",help="plot by sequence 'True' or 'False' ", const=True, default=False)
parser.add_argument("-tlts", "--thermalLoadingTimeStep", nargs='?', type=int, dest="thermalLoadingTimeStep",help="thermal loading time step", const=10, default=10)
parser.add_argument("-hts",  "--hardeningTimeStep", nargs='?', type=int, dest="hardeningTimeStep",help="hardening time step", const=3600, default=3600)
parser.add_argument("-qts", "--quenchingTimeStep" , nargs='?', type=int, dest="quenchingTimeStep",help="quenchi,ng time step", const=1, default=1)


args = parser.parse_args()


path=args.path

plBSeq= args.plotBySequence
thermalLoadingTimeStp= args.thermalLoadingTimeStep
hardeningTimeStp= args.hardeningTimeStep
hardeningTimeStp= args.hardeningTimeStep


print(plBSeq)



impScript.PostProcessGivenResultDirectory(resultDirectory=path,plotBySequence=plBSeq,\
                                            thermalLoadingTimeStep=thermalLoadingTimeStp,\
                                            hardeningTimeStep=hardeningTimeStp,\
                                            quenchingTimeStep=hardeningTimeStp)



