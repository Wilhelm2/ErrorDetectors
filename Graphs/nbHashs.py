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
	x = []
	y = []
#	x = [10,25,50,75,100,125,150]
#	y = [1,1,1.14,2.2,6.58,14.8,33.3]

	
	for n in nodes:
		x.append(n/delaySend)
		f=open("data/clocksize"+ str(c) +"Nodes" + str(n) + "/nbHashsFile.dat",'r') 
		lines = f.readlines()[-1]
		y.append(float(lines.split(" ")[0]))
	
	print(y)
	# ~ x= [elt/5. for elt in x]
	plt.plot(x,y, marker='s', color='b')
	
		
#	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
	plt.tick_params(axis='both', which='major', labelsize=14)
	plt.tick_params(axis='both', which='minor', labelsize=14)

#	plt.gca().set_ylim([0,0.0012])
	plt.gca().yaxis.offsetText.set_fontsize(16)
	plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.0f'))


#	plt.xlabel('Number of nodes', fontsize=18)
	plt.xlabel('Message load (messages/second)', fontsize=18)
	plt.ylabel('Computed hashs per delivery', fontsize=18)
	
#	plt.xticks([10,25,50,75,100,125,150])
	
	# ~ plt.title('Average size of BufferReceive of MH in time ')
	plt.tight_layout()
	
	plt.savefig("ComputedHashsPerDelivery"+str(c)+".eps", format='eps',bbox_inches="tight")

	plt.show()
	plt.close("all")

