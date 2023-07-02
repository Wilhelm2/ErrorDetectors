@mainpage
@author Daniel Wilhelm

# Introduction

Implementation of error detectors when using probabilistic clocks [[1]](https://link.springer.com/chapter/10.1007/978-3-319-62932-2_31) to causally order broadcast messages. 

This project provides implementations of causal broadcast using the following control mechanisms:

1. **No control:** A node delivers a message upon its reception without any control. 
2. **Probabilistic clocks:** Nodes use Probabilistic clocks to causally order messages. A node delivers a received message *m* once the delivery conditions of *m*'s attached Probabilistic clock are satisfies. 
3. **Probabilistic clock and the error detector of Mostéfaoui and Weiss:** Additionally to **2**, the node also executes the error detector of Mostéfaoui and Weiss  [[2]](https://hal.science/hal-02056349/document) on the Probabilistic clock of *m* before delivering *m*. 
4. **Probabilistic clock and an hash-based error detector using an average propagation delay hypothesis:** Additionally to **2**, the node also executes an hash-based error detector proposed by Wilhelm and al.[[3]](https://hal.science/hal-03984499) on the Probabilistic clock of *m* before delivering *m*. The hash-based error detector uses the average propagation delay of messages to determine the causal dependencies of messages. 
5. **Probabilistic clock and an hash-based error detector using the clock difference between messages:** Additionally to **2**, the node also executes an hash-based error detector proposed by Wilhelm and al.[[3]](https://hal.science/hal-03984499) on the Probabilistic clock of *m* before delivering *m*. The hash-based error detector uses the average propagation delay of messages to determine the causal dependencies of messages. 
6. **Probabilistic clock, an hash-based error detector using the clock difference between messages, and the recovery of messages tagged as not causally ordered:** Additionnally to **5**, a process recovers the causal dependencies of the messages *m* which its error detector tags as not causally ordered, by requesting the causal dependencies of *m* to the node that broadcasted *m*. The node then delivers *m* once it delivered all of *m*'s causal dependencies. 
7. **Probabilistic clock, an hash-based error detector using the clock difference between messages, the recovery of messages tagged as not causally ordered, and a recovery test to avoid requesting the causal dependencies of messages:** Additionnally to **6**, before requesting the causal dependencies of a message *m*, a node first tries to find the dependency set of *m* with messages that the node did not deliver yet. 

## OMNeT++

The implementation was done in C++ on the version 5.6.1 of the OMNeT++ simulator[[4]](https://omnetpp.org/). 
OMNeT++ is an event based simulator primarly used for building network simulators. 

See [here](https://doc.omnetpp.org/omnetpp5/InstallGuide.pdf) how to install the version 5.6.1 of OMNeT++.

# How to launch the simulation 

To launch the simulation:
1. Compile the 
2. Execute the python script 'execution.py'


## Node class hierarchy 

This section describes the node class hierarchy. 

![Error Detector inheritance graph.](documentation/mainpage/NodeInheritance.png?raw=true)


## Error detectors class hierarchy

![Error Detector inheritance graph.](documentation/mainpage/errorDetectorInheritance.png?raw=true)


## References

<a id="PC">[1]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. PaCT 2017.

<a id="PC">[2]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. Technical report 2017.

<a id="PC">[3]</a> A probabilistic Dynamic Clock Set to capture message causality. Wilhelm, Daniel and Arantes, Luciana and Sens, Pierre. Technical report 2017.

<a id="PC">[4]</a> The OMNET++ Discrete Event Simulation System. Varga, Andras. ESM 2001.
