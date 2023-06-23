
import subprocess
from shutil import copyfile
import sys
import ast

subprocess.check_call(["bash","-c","make MODE=debug"])				
subprocess.check_call(["bash","-c","make "])				

nbNodes 	= [290] #[200,300,400,500]# [200,300,400]#,500,1000]#[50,125,250,375,500,750] #[100,150,200,250,300,350,400] 
clockSize	= [125]#25,50,75]

deliveryOption = 2 # 0: no control ; 1: Probabilistic clock comparison; 2: Probabilistic clock with mostefaoui error detector, 3: Probabilistic clock with hash detector; 4: Probablistic clock + hash + recovery

delaySend = 2 # seconds
#PEAKSPERDELAY = nbNodes[0]

subprocess.check_call(["bash","-c","mkdir -p simulations/data"])

for c in clockSize:
	for n in nbNodes:
		print ("Number of nodes " + str(n))

		# MODIFICATIONS POUR CHARGE CONSTANTE
		# ~ load = 110 # nombre de messages a diffuser par seconde
		# delaySend = nbNodes / load
		# ~ delaySend = n*1000. / load
		PEAKSPERSECOND = float(n)

		subprocess.check_call(["bash","-c","python completeInitFiles.py " + str(n) + " " + str(c)+ " " + str(delaySend)+ " " + str(PEAKSPERSECOND) + " " + str(deliveryOption)]) # upadtes files .ini and .ned
		 
		subprocess.check_call(["bash","-c","../out/gcc-release/src/ErrorDetectors -f omnetpp.ini -u Cmdenv > res.txt" ], cwd='simulations')	
		#subprocess.check_call(["bash","-c","valgrind --tool=massif ../out/gcc-debug/src/DetecteursErreursWithRecovery_dbg -f omnetpp.ini -u Cmdenv" ], cwd='simulations')	
		#subprocess.check_call(["bash","-c","valgrind --tool=callgrind --trace-children=yes ../out/gcc-debug/src/DetecteursErreursWithRecovery_dbg -f omnetpp.ini -u Cmdenv" ], cwd='simulations')	
 		#subprocess.check_call(["bash","-c","gdb --args ../out/gcc-debug/src/DetecteursErreursWithRecovery_dbg -f omnetpp.ini -u Cmdenv" ], cwd='simulations')	
 

		subprocess.check_call(["bash","-c","mkdir -p ../codeGraphes/data/clocksize"+str(c)+ "Nodes"+ str(n) ])			
		subprocess.check_call(["bash","-c","cp -r simulations/data/* ../codeGraphes/data/clocksize"+str(c)+ "Nodes"+ str(n)  ])


# subprocess.check_call(["bash","-c","python ../codeGraphes/nbRecoveries.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "' '" + str(delaySend)+ "'"], cwd='../codeGraphes')
# subprocess.check_call(["bash","-c","python nbHashs.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "' '" + str(delaySend) + "'" ], cwd='../codeGraphes')
# subprocess.check_call(["bash","-c","python FalseTrueNegatives.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "' '" + str(delaySend) + "'" ], cwd='../codeGraphes')
# subprocess.check_call(["bash","-c","python errorPercentage.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "' '" + str(delaySend) + "'" ], cwd='../codeGraphes')
#subprocess.check_call(["bash","-c","python ../codeGraphes/hashDistribution.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "' '" + str(delaySend)+ "'"], cwd='../codeGraphes')

subprocess.check_call(["bash","-c","python Dependencies.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "' '" + str(delaySend) + "'" ], cwd='Graphs')


# ~ subprocess.check_call(["bash","-c","python falseDependenciesEnsemble.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "'" ]) # un graphe qui represente pour differentes tailles d'horloges
#subprocess.check_call(["bash","-c","python nbRecoveriesEnsemble.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "' " + str(delaySend/1000) ])
		
