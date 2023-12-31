
import subprocess
from shutil import copyfile
import sys
import ast

subprocess.check_call(["bash", "-c", "make MODE=debug"])
subprocess.check_call(["bash", "-c", "make "])

nbNodes = [360] 
clockSize = [50]

# 1: no control ; 2: Probabilistic clock comparison; 3: Probabilistic clock with mostefaoui error detector, 4: Probabilistic clock with hash detector (difference); 5: Probabilistic clock with hash detector (interval); 6: Probablistic clock + hash(difference) + recovery; 7: Probablistic clock + hash(difference) + recovery + recoveryTest
deliveryOption = 4

delaySend = 2  # Each process sends a message every x seconds. Hence, the system's message load corresponds to nbNodes/delaySend
# PEAKSPERDELAY = nbNodes[0]

subprocess.check_call(["bash", "-c", "mkdir -p simulations/data"])

for c in clockSize:
    for n in nbNodes:
        print("Number of nodes " + str(n))

        # MODIFICATIONS POUR CHARGE CONSTANTE
        # ~ load = 110 # nombre de messages a diffuser par seconde
        # delaySend = nbNodes / load
        # ~ delaySend = n*1000. / load
        PEAKSPERSECOND = float(n)

        subprocess.check_call(["bash", "-c", "python completeInitFiles.py " + str(n) + " " + str(c) + " " + str(
            delaySend) + " " + str(PEAKSPERSECOND) + " " + str(deliveryOption)])  # upadtes files .ini and .ned

        subprocess.check_call(
            ["bash", "-c", "../out/gcc-release/src/ErrorDetectors -f omnetpp.ini -u Cmdenv > res.txt"], cwd='simulations')
        # subprocess.check_call(["bash","-c","valgrind --tool=massif ../out/gcc-debug/src/DetecteursErreursWithRecovery_dbg -f omnetpp.ini -u Cmdenv" ], cwd='simulations')
        # subprocess.check_call(["bash","-c","valgrind --tool=callgrind --trace-children=yes ../out/gcc-debug/src/DetecteursErreursWithRecovery_dbg -f omnetpp.ini -u Cmdenv" ], cwd='simulations')
        # subprocess.check_call(["bash","-c","gdb --args ../out/gcc-debug/src/ErrorDetectors_dbg -f omnetpp.ini -u Cmdenv" ], cwd='simulations')

        subprocess.check_call(["bash", "-c", "mkdir -p ../Graphs/data/clocksize" +
                              str(c) + "Nodes" + str(n) + "D:" + str(deliveryOption)])
        subprocess.check_call(["bash", "-c", "cp -r simulations/data/* ../Graphs/data/clocksize" +
                              str(c) + "Nodes" + str(n) + "D:" + str(deliveryOption)])
        subprocess.check_call(["bash", "-c", "rm simulations/data/*"])


subprocess.check_call(["bash", "-c", "python falseDelivered.py '" + str(nbNodes) + "' '" + str(
    clockSize) + "' '" + str(delaySend) + "' '" + str(deliveryOption) + "'"], cwd='../Graphs')

if deliveryOption >= 2:
    subprocess.check_call(["bash", "-c", "python errorPercentage.py '" + str(nbNodes) + "' '" + str(
        clockSize) + "' '" + str(delaySend) + "' '" + str(deliveryOption) + "'"], cwd='../Graphs')
    subprocess.check_call(["bash", "-c", "python FalseTrueNegatives.py '" + str(nbNodes) + "' '" + str(
        clockSize) + "' '" + str(delaySend) + "' '" + str(deliveryOption) + "'"], cwd='../Graphs')

if deliveryOption >= 3:
    subprocess.check_call(["bash", "-c", "python nbHashs.py '" + str(nbNodes) + "' '" + str(
        clockSize) + "' '" + str(delaySend) + "' '" + str(deliveryOption) + "'"], cwd='../Graphs')
    subprocess.check_call(["bash", "-c", "python Dependencies.py '" + str(nbNodes) + "' '" + str(
        clockSize) + "' '" + str(delaySend) + "' '" + str(deliveryOption) + "'"], cwd='../Graphs')
    subprocess.check_call(["bash", "-c", "python hashDistribution.py '" + str(nbNodes) + "' '" + str(
        clockSize) + "' '" + str(delaySend) + "' '" + str(deliveryOption) + "'"], cwd='../Graphs')

if deliveryOption >= 5:
    subprocess.check_call(["bash", "-c", "python nbRecoveries.py '" + str(nbNodes) + "' '" + str(
        clockSize) + "' '" + str(delaySend) + "' '" + str(deliveryOption) + "'"], cwd='../Graphs')
