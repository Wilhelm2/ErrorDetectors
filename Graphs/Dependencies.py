import matplotlib.pyplot as plt
import sys
import ast
import subprocess


nodes = ast.literal_eval(sys.argv[1])
clockSizes = ast.literal_eval(sys.argv[2]) #[20,10,7,5,4,3,2.5]
deliveryOption = int(sys.argv[4])

#subprocess.check_call(["bash","-c","mkdir -p bufferReceiveMH"  ])


for c in clockSizes: # pour chaque taille d'horloge
	plt.figure()	
	x = []
	y = []
	
	for n in nodes:
		x.append(n)

		f=open("data/clocksize"+ str(c) +"Nodes" + str(n)  + "D:" + str(deliveryOption) + "/nbDeliveredMessagesFile.dat",'r') 
		lines = f.readlines()[-1]
		totalDelivered = float(lines.split(" ")[0])
	
		f=open("data/clocksize"+ str(c) +"Nodes" + str(n)  + "D:" + str(deliveryOption) + "/nbSentDependenciesFile.dat",'r') 
		lines = f.readlines()[-1].split(" ")
		print("total delivered " + str(totalDelivered) + " sentdependencies " + lines[0])
		y.append(float(lines[0])*n/totalDelivered)


	# ~ x= [elt/5. for elt in x]
	plt.plot(x,y, marker='s', color='b')
	plt.tick_params(axis='both', which='major', labelsize=14)
	plt.tick_params(axis='both', which='minor', labelsize=14)
	plt.gca().yaxis.offsetText.set_fontsize(16)
	

	plt.xlabel('Message load (messages/second)', fontsize=18)
	plt.ylabel('Dependencies/messages', fontsize=18)
	# ~ plt.title('Average size of BufferReceive of MH in time ')
#	plt.legend()
	plt.tight_layout()

	plt.savefig("Dependencies"+str(c)+".eps", format='eps',bbox_inches="tight")

	plt.show()
	plt.close("all")
