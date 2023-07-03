@mainpage
@author Daniel Wilhelm

# Introduction

This project implements causal broadcast in a distributed system.
Causal order is defined by the *happened-before* relationship introduced by Lamport in 1978 [[2]](https://amturing.acm.org/p558-lamport.pdf). 
The *happened-before* relationship orders events in distributed systems following three rules [[2]](https://amturing.acm.org/p558-lamport.pdf):

>**Happened-before relation:**
>	Considering two events $e_1$ and $e_2$, $e_1$ causally precedes $e_2$ ($e_1 \rightarrow e_2$) iff:
>	1. $e_1$ and $e_2$ occur on the same process and $e_1$ precedes $e_2$ or
>	2. for a message m $e_1$=send(m) and $e_2$=deliver(m) or
>	3. there exists an event $e_3$ such that $e_1\rightarrow e_3$ and $e_3\rightarrow e_2$.

Causal order ensures that any two causally related messages are delivered to applications respecting that order. 

>**Causal order:** 
>	Processes deliver messages while respecting the causal relation between them.
>	For any message $m$ and $m'$, if $m$ causally precedes $m'$, denoted $m\rightarrow m'$, then all processes deliver $m$ before $m'$:

>	<center>*send(m) \rightarrow send(m') \Rightarrow deliver(m) \rightarrow deliver(m').* </center>

**The main features implemented by the project are:**
1. Causal broadcast using **Probabilstic clocks** to causally order messages. 
2. **Error detectors** whose purpose is to detect messages wrongly considered by Probabilistic clocks as causally ordered.
3. **A procedure to recover** the causal dependencies of messages in order to ensure their delivery in causal order. 

Those features are combined to provide the following simulation settings:

1. **No control:** A node delivers a message upon its reception without any control. 
2. **Probabilistic clocks:** Nodes use Probabilistic clocks to causally order messages. A node delivers a received message *m* once the delivery conditions of *m*'s attached Probabilistic clock are satisfies. 
3. **Probabilistic clock and the error detector of Mostéfaoui and Weiss:** Additionally to **2**, the node also executes the error detector of Mostéfaoui and Weiss  [[3]](https://hal.science/hal-02056349/document) on the Probabilistic clock of *m* before delivering *m*. 
4. **Probabilistic clock and an hash-based error detector using an average propagation delay hypothesis:** Additionally to **2**, the node also executes an hash-based error detector proposed by Wilhelm and al.[[4]](https://hal.science/hal-03984499) on the Probabilistic clock of *m* before delivering *m*. The hash-based error detector uses the average propagation delay of messages to determine the causal dependencies of messages. 
5. **Probabilistic clock and an hash-based error detector using the clock difference between messages:** Additionally to **2**, the node also executes an hash-based error detector proposed by Wilhelm and al.[[3]](https://hal.science/hal-03984499) on the Probabilistic clock of *m* before delivering *m*. The hash-based error detector uses the average propagation delay of messages to determine the causal dependencies of messages. 
6. **Probabilistic clock, an hash-based error detector using the clock difference between messages, and the recovery of messages tagged as not causally ordered:** Additionnally to **5**, a process recovers the causal dependencies of the messages *m* which its error detector tags as not causally ordered, by requesting the causal dependencies of *m* to the node that broadcasted *m*. The node then delivers *m* once it delivered all of *m*'s causal dependencies. 
7. **Probabilistic clock, an hash-based error detector using the clock difference between messages, the recovery of messages tagged as not causally ordered, and a recovery test to avoid requesting the causal dependencies of messages:** Additionnally to **6**, before requesting the causal dependencies of a message *m*, a node first tries to find the dependency set of *m* with messages that the node did not deliver yet. 

# Table of contents

# OMNeT++

This section gives a brief description of the simulator OMNeT++ on which the project was implemented. 

The implementation was done in C++ on the version 5.6.1 of the OMNeT++ simulator[[5]](https://omnetpp.org/). 
OMNeT++ is an event based simulator primarly used for building network simulators. 

parler des modules, des fichiers ini et ned, msg 

See [here](https://doc.omnetpp.org/omnetpp5/InstallGuide.pdf) how to install the version 5.6.1 of OMNeT++.

# User guide 

## OMNeT++ installation 
, the simulator on which the project is built

## Import project in OMNeT++ IDE

OMNeT++ comes with a built-in IDE based on Eclipse. This section explains how to import the project in that IDE and compile it. 
 



## How to run the simulation 

To launch the simulation:
1. Compile the 
2. Execute the python script 'execution.py'

## Data analysis 


## Node class hierarchy 

This section describes the node class hierarchy. 

![Error Detector inheritance graph.](documentation/mainpage/nodeInheritance.png?raw=true)


## Error detectors class hierarchy

![Error Detector inheritance graph.](documentation/mainpage/errorDetectorInheritance.png?raw=true)


## Documentation

More detailled documentation can be found in the documentation folder, which is providing documentation under the PDF and html format. 
Documentation has been built with [Doxygen](https://www.doxygen.nl/).

#License

# Badges

## References

<a id="HappenedBefore">[1]</a> Time, Clocks, and the Ordering of Events in a Distributed System. Lamport, Leslie. Communications of the ACM 1978.

<a id="PC1">[2]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. PaCT 2017.

<a id="PC2">[3]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. Technical report 2017.

<a id="PC">[4]</a> A probabilistic Dynamic Clock Set to capture message causality. Wilhelm, Daniel and Arantes, Luciana and Sens, Pierre. Technical report 2017.

<a id="OMNeT++">[5]</a> The OMNET++ Discrete Event Simulation System. Varga, Andras. ESM 2001.
