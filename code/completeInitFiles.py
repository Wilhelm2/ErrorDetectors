import sys
from shutil import copyfile
import subprocess


if len(sys.argv) != 6:
	print sys.argv[0] + " nbNodes clockSize sendWindow peaksPerSecond deliveryOption"  
	exit(1)

nbNodes = sys.argv[1]
clockSize = sys.argv[2]
sendWindow = sys.argv[3]
peaksPerSecond = sys.argv[4]
deliveryOption = sys.argv[5]
recovering = "true" if (deliveryOption=='5' or deliveryOption=='6') else "false"

def writeInitFile(nbNodes, clockSize, sendWindow, peaksPerSecond):
	with open("simulations/omnetpp.ini","w") as f:
		f.write("[General]\nnetwork = ErrorDetectorWithRecovery\nsim-time-limit = 1000s\n*.simParams.nbNodes =" + nbNodes + "\n*.simParams.clockLength = " + clockSize+"\n*.simParams.delaySend="+sendWindow+"\n*.simParams.PEAKSPERDELAY=" + peaksPerSecond + "\n*.simParams.deliveryOption=" + deliveryOption + "\n*.simParams.recovering=" + recovering)
		f.write("\n**.vector-recording = false\n**.scalar-recording = false\n**.statistic-recording = false\n")
		f.close()

def writeNedFile(nbNodes):
	copyfile("ErrorDetectorWithRecoveryTemplate.ned", "simulations/ErrorDetectorWithRecovery.ned")
	with open("simulations/ErrorDetectorWithRecovery.ned","a") as f:
		f.write("\t\tsimParams: SimulationParameters;\n")
		if deliveryOption == '0':
			f.write("\t\tNodes["+nbNodes+"]: NodeNoControl;\n")
		elif deliveryOption == '1':
			f.write("\t\tNodes["+nbNodes+"]: NodePC;\n")
		elif deliveryOption == '2' or deliveryOption == '3' or  deliveryOption == '4' : 
			f.write("\t\tNodes["+nbNodes+"]: NodeWithoutRecovery;\n")
		else :
			f.write("\t\tNodes["+nbNodes+"]: NodeWithRecovery;\n")

		f.write("\t\tcontrol: Controller;\n")
		f.write("\t\tstat: Stats;\n")
		f.write("\t\tCommunications: CommunicationDispatcher;\n")
		f.write("\tconnections:\n")
		for i in range(0,int(nbNodes)) :
			f.write("\t\tNodes["+str(i)+"].gate <--> Channel <--> Communications.neighbour++;\n");	
		f.write("}")
		f.close()
		
writeInitFile(nbNodes, clockSize, sendWindow, peaksPerSecond)
writeNedFile(nbNodes)
