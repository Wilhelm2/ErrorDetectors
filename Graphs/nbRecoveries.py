import matplotlib.pyplot as plt
import sys
import ast
import subprocess
from matplotlib.ticker import FormatStrFormatter


nodes = ast.literal_eval(sys.argv[1])
clockSizes = ast.literal_eval(sys.argv[2]) #[20,10,7,5,4,3,2.5]
delaySend = ast.literal_eval(sys.argv[3])

for c in clockSizes:
	plt.figure()	
	x =   []
	yR =  []
	yAR = []
	
	for n in nodes:
		x.append(n/delaySend)
		f=open("data/clocksize"+ str(c) +"Nodes" + str(n) + "D:" + str(deliveryOption) + "/nbRecoveriesFile.dat",'r') 
		lines = f.readlines()[-1]
		yR.append(float(lines.split(" ")[0]))

		f=open("data/clocksize"+ str(c) +"Nodes" + str(n) + "D:" + str(deliveryOption) + "/nbAvoidedRecoveriesFile.dat",'r') 
		lines = f.readlines()[-1]
		lines = lines.split(" ")
		yAR.append(float(lines[1]))
		
	
	print(yR)
	print(yAR)
	# ~ x= [elt/5. for elt in x]
	plt.plot(x,yR, label="Recovery", marker='s', color='b')
	plt.plot(x,yAR, label="Avoided recovery", marker='^', color='r')
	
		
#	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
	plt.tick_params(axis='both', which='major', labelsize=14)
	plt.tick_params(axis='both', which='minor', labelsize=14)

#	plt.gca().set_ylim([0,0.0012])
	plt.gca().yaxis.offsetText.set_fontsize(16)
	plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.5f'))


#	plt.xlabel('Number of nodes', fontsize=18)
	plt.xlabel('Message load (messages/second)', fontsize=18)
	plt.ylabel('Recovery rate ', fontsize=18)
	
	
	
	# ~ plt.title('Average size of BufferReceive of MH in time ')
	plt.legend(fontsize=14)
	plt.tight_layout()
	
	plt.savefig("RecoveryRate"+str(c)+".eps", format='eps',bbox_inches="tight")

	plt.show()
	plt.close("all")

