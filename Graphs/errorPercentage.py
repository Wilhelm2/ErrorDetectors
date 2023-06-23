import matplotlib.pyplot as plt
import sys
import ast
import subprocess
from matplotlib.ticker import FormatStrFormatter


nodes = ast.literal_eval(sys.argv[1])
clockSizes = ast.literal_eval(sys.argv[2])  # [20,10,7,5,4,3,2.5]
delaySend = ast.literal_eval(sys.argv[3])

for c in clockSizes:
    plt.figure()
    x = []
    errorPercentage = []
    falseErrorPercentage = []
    trueErrorPercentage  = []

    print(nodes)
    for n in nodes:
        x.append(n/delaySend)
        f=open("data/clocksize"+ str(c) +"Nodes" + str(n) + "/trueFalseNegativesFile.dat",'r') 
        lines = f.readlines()[-1]
        totalErrors = float(lines.split(" ")[0]) + float(lines.split(" ")[1])
        falseErrors = float(lines.split(" ")[0])
        trueErrors  = float(lines.split(" ")[1])

        f=open("data/clocksize"+ str(c) +"Nodes" + str(n) + "/nbDeliveredMessagesFile.dat",'r') 
        lines = f.readlines()[-1]
        totalDelivered = float(lines.split(" ")[0])
        if totalDelivered > 0:
            errorPercentage.append(100*totalErrors/totalDelivered)
            falseErrorPercentage.append(100*falseErrors/totalDelivered)
            trueErrorPercentage.append(100*trueErrors/totalDelivered)
        else :
            errorPercentage.append(0)
            falseErrorPercentage.append(0)
            trueErrorPercentage.append(0)
        print(errorPercentage)
	
    print(errorPercentage)

	# ~ x= [elt/5. for elt in x]
    plt.plot(x,errorPercentage, label="Errors", marker='s', color='b')
    plt.plot(x,falseErrorPercentage, label="False Positives", marker='s', color='g')
    plt.plot(x,trueErrorPercentage, label="True Positives", marker='s', color='r')
	
		
#	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    plt.tick_params(axis='both', which='major', labelsize=14)
    plt.tick_params(axis='both', which='minor', labelsize=14)

#	plt.gca().set_ylim([0,0.0012])
    plt.gca().yaxis.offsetText.set_fontsize(16)
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.0f'))


#	plt.xlabel('Number of nodes', fontsize=18)
    plt.xlabel('Message load (messages/second)', fontsize=18)
    plt.ylabel('Percentage', fontsize=18)
	
	
	
	# ~ plt.title('Average size of BufferReceive of MH in time ')
    plt.legend(fontsize=14)
    plt.tight_layout()
	
    plt.savefig("errorPercentage"+str(c)+".eps", format='eps',bbox_inches="tight")

    plt.show()
    plt.close("all")

