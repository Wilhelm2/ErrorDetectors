import matplotlib.pyplot as plt
import sys
import ast
import subprocess
from matplotlib.ticker import FormatStrFormatter
from matplotlib.ticker import PercentFormatter
import numpy as np


nodes = ast.literal_eval(sys.argv[1])
clockSizes = ast.literal_eval(sys.argv[2]) #[20,10,7,5,4,3,2.5]
delaySend = ast.literal_eval(sys.argv[3])

for c in clockSizes:
    for n in nodes:
        x 			   = []
        falseNegatives = []
        trueNegatives  = []

        f=open("data/clocksize"+ str(c) +"Nodes" + str(n) + "/doneCombinationsToFindHashFile.dat",'r') 
        line = f.readlines()[-1].split(" ")[0:-1]
        line = [int(tmp) for tmp in line]
        for i in range(0,len(line)):
            for y in range(0,line[i]):
                x.append(i)
        print(x)
        plt.yscale('log')
#        plt.xticks(range(0,x[-1],x[-1]/10+1))
        plt.xticks(range(0,201,20))
        plt.hist(x, weights=np.ones(len(x)) / len(x) , bins = range(0,201),align="left")	
#        plt.hist(x, weights=np.ones(len(x)) / len(x) , bins=range(1,x[-1]), label="Computed Hashes",align="left")	
        plt.xlabel('Number of computed hashes', fontsize=18)

        
        plt.gca().yaxis.set_major_formatter(PercentFormatter(1,decimals=2))
        plt.yticks([0.0001,0.001,0.01,0.1,0.5,1])

  #      plt.tight_layout()
        
        plt.savefig("hashDistribution"+str(c)+".eps", format='eps',bbox_inches="tight")

        plt.show()
        plt.close("all")

