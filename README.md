# Introduction

This project implements a causal broadcast algorithm for distributed systems in C++ on the OMNeT++ simulator. 
Causal order is defined by the *happened-before* relationship introduced by Lamport in 1978 [[1]](https://amturing.acm.org/p558-lamport.pdf). 
The *happened-before* relationship orders events in distributed systems following three rules [[1]](https://amturing.acm.org/p558-lamport.pdf):

>**Happened-before relation:**
>	Considering two events *e1* and *e2*, *e1* causally precedes *e2* (*e1 -> e2*) iff:
>	1. *e1* and *e2* occur on the same process and *e1* precedes *e2* or
>	2. for a message m *e1*=send(m) and *e2*=deliver(m) or
>	3. there exists an event *e3* such that *e1->e3* and *e3->e2*.

Causal order ensures that any two causally related messages are delivered to applications respecting that order. 

>**Causal order:** 
>	Processes deliver messages while respecting the causal relation between them.
>	For any message $m$ and $m'$, if $m$ causally precedes $m'$, denoted $m\rightarrow m'$, then all processes deliver $m$ before $m'$:\
>	<p align = "center">send(m) -> send(m') => deliver(m) -> deliver(m'). </p>

**The main features implemented by the project are:**
1. Causal broadcast using **Probabilstic clocks** to causally order messages. 
2. **Error detectors** whose purpose is to detect messages wrongly considered by Probabilistic clocks as causally ordered.
3. **A procedure to recover** the causal dependencies of messages in order to ensure their delivery in causal order. 

Those features are combined to provide 7 simulation settings described in the section which describes how to run the simulation. 

## Error detectors

This section briefly describes the error detectors implemented in this project. 

The idea behind error detectors for Probabilistic clocks comes from the fact that such clocks might wrongly indicate that a message can be delivered in causal order. In other words, they might indicate that all of a message's causal dependencies have been delivered at a node while the node did actually not deliver some of them yet. 

Error detectors for Probabilistic clocks aim to detect such messages.
A node executes an error detector on a message *m* before delivering *m*. 
An error detector can yield *false positives* as well as *false negatives*. 
A *false positive* occurs when the error detector decides that a node did not deliver all of a message's causal dependencies while it actually did, i.e. the node can deliver the message in causal order. 
Conversely, a *false negative* occurs when the error detector decides that a node delivered all of a message's causal dependencies while it actually did not, i.e. the node cannot yet deliver the message in causal order. 

The two implemented error detectors are:
1. Error detector proposed by Mostéfaoui and Weiss whose description can be found in [this paper](https://hal.science/hal-02056349/document)[3]. 
2. Hash-based error detector proposed by Wilhelm and al. whose description can be found in [this paper](https://hal.science/hal-03984499)[4]. 
The hash-based error detector sorts messages either based on reception times or the clock difference between messages. 

The following figure describes the project's error detector class diagram:
![Error Detector inheritance graph.](documentation/mainpage/errorDetectorInheritance.png?raw=true)

## Recovery of message dependencies

The project implements a procedure to request the causal dependencies of messages. It can notably be used by a node *p* to get the causal dependencies of a message *m* that its error detector considers as not causally ordered. After getting the dependencies of *m*, *p* can postpone the delivery of *m* till it delivered all of *m*'s causal dependencies, thus ensuring that it delivers *m* in causal order. 

A detailled description of the recovery of message dependencies can be found in [this paper](https://hal.science/hal-03984499)[4].


## Nodes 

The features of this project are implemented inside Node classes. This section briefly describes these classes. 
The following figure describes the project's Node class diagram: 
![Error Detector inheritance graph.](documentation/mainpage/nodeInheritance.png?raw=true)

A brief description of the Node classes:
1. **NodeBase:** Base class to implement nodes. 
2. **NodeNoControl:** Delivers a message upon reception without any control. 
3. **NodeWithControl:** Base class of nodes that causally orders messages with Probabilistic clocks.
4. **NodePC:** Causally orders messages with Probabilistic clocks and without using error detectors or message recovery. 
5. **NodeDetector:** Base class of nodes that use error detectors to analyze messages before delivering them. 
6. **NodeWithoutRecovery:** Uses error detectors to analyze messages before delivering them without recovering causal dependencies. 
7. **NodeWithRecovery:** Uses erorr detectors to analyze messages before delivering them, recovers the causal dependencies of message tagged by the error detector as not causally ordered, and ensures their causal order delivery.
8. **NodeWithRecoveryTest:** Uses the hash-based error detectors using clock differences. Additionnaly to **6**, before requering the causal dependencies of a message *m* tries to find the hash of *m* with messages that the node did not deliver yet. 

# OMNeT++

The project was implemented on the version 5.6.1 of the OMNeT++ simulator [[5]](https://omnetpp.org/). 
This section gives a brief description of OMNeT++, which is an event based simulator primarly used for building network simulators. 
It is organized into modules that are combined in order to form a network. 

OMNeT++ uses the following specific files:
1. **Initialization file:** Contains the simulation parameters. Has the extension *.ini*.
2. **Network files:** Define modules as well as the simulation network which is composed of modules. Have the extension *.ned*.
3. **Message files:** Define the messages used by nodes. Have the extension *.msg*.


# User guide 

This section firt explains how to install the required ressources to execute the project. Then it shows how to import the project in the OMNeT++ built-in IDE based on Eclipse. Finally, it describes how to execute the simulation on the terminal.

## OMNeT++ installation 

The installation of the version 5.6.1 of OMNeT++ is explained [here](https://doc.omnetpp.org/omnetpp5/InstallGuide.pdf).
Note that OMNeT++ is not (even though mostly) retrocompatible. 
Hence, some modifications might be required when using another version of OMNeT++. 
Thus you should carefully install the version 5.6.1 of OMNeT++.

## Import project in OMNeT++ IDE

OMNeT++ comes with a built-in IDE based on Eclipse. This section explains how to import the project in that IDE and compile it. 

First, launch the OMNeT++ IDE with the command:
> omnetpp

Second, see [here](http://wnss.sv.cmu.edu/teaching/14814/s15/files/hw2addendum.pdf) a guide written by Brian Ricks on how to import existing code within the Ecplise IDE.

Third, the compilation of .msg files requires to add: in Project Explorer (left bar):
> left clock on the project -> Properties -> OMNeT++ -> Makemake -> Options -> Custom -> add MSGC:=$(MSGC) --msg6 -> OK -> Apply and close.

## How to run the simulation 

The simulation can be launched from the IDE as well as from the terminal.

To launch it from the IDE, click on the *Debug* or *Run* buttons in the bar under the *Menu* bar. 

To launch the simulation from the terminal:
1. Open a terminal in the project directory
2. Configure the files *omnetpp.ini* and *network.ned* following the desired simulation settings (number of nodes, control mechanism,...) 
3. Execute the command: 
>../out/gcc-release/src/ErrorDetectors -f omnetpp.ini -u Cmdenv 

The python script 'execution.py' is also provided. It builds the *.ini* and *.ned* files, launches the simulation, copies the simulation statistics in folders, and builds several graphs with those statistics.
Configure the simulation parameters by modifying 'execution.py'. 

The simulation parameter 'deliveryOption' determines the control mechanism used to deliver messages. The available control mechanisms are: 
1. **No control** Nodes deliver messages upon reception without any control. 
2. **Probabilistic clocks:** Nodes use Probabilistic clocks to causally order messages.
3. **Probabilistic clock and the error detector of Mostéfaoui and Weiss:** Nodes use Probabilistic clocks to deliver messages, and execute the error detector proposed by Mostéfaoui and Weiss  [[3]](https://hal.science/hal-02056349/document) on messages before delivering them.
4. **Probabilistic clock and an hash-based error detector using an average propagation delay hypothesis:** 
Nodes use Probabilistic clocks to causally order messages, and execute an hash-based error detector proposed by Wilhelm and al.[[4]](https://hal.science/hal-03984499) on on messages before delivering them. The hash-based error detector uses the average propagation delay of messages to determine their causal relationship. 
5. **Probabilistic clock and an hash-based error detector using the clock difference between messages:** Nodes use Probabilistic clocks to causally order messages, and execute an hash-based error detector proposed by Wilhelm and al.[[4]](https://hal.science/hal-03984499) on on messages before delivering them. The hash-based error detector uses the clock difference between messages to determine their causal relationship. 
6. **Probabilistic clock, an hash-based error detector using the clock difference between messages, and the recovery of messages tagged as not causally ordered:**
Nodes use Probabilistic clocks to causally order messages, and execute an hash-based error detector proposed by Wilhelm and al.[[4]](https://hal.science/hal-03984499) on on messages before delivering them. The hash-based error detector uses the average propagation delay of messages to determine their causal relationship. Moreover, nodes 
recover the causal dependencies of the messages *m* for that the error detector tags as not causally ordered, by requesting the causal dependencies of *m* to the node that broadcasted *m*. Such a message is then delivered once the node delivered all of the message's causal dependencies.
7. **Probabilistic clock, an hash-based error detector using the clock difference between messages, the recovery of messages tagged as not causally ordered, and a recovery test to avoid requesting the causal dependencies of messages:** Additionnally to **6**, before requesting the causal dependencies of a message *m*, a node first tries to find the dependency set of *m* with messages that it did not deliver yet. 

## Data analysis

Statistics about the simulation are collected and written in the *simulations/data* folder. 

The folder *Graphs* contains python scripts to build graphs from those statistics. They are mainly written to be used from the 'execution.py' script, but can also be used outside of it. 

## Documentation

Detailled documentation can be found in the documentation folder, which is providing documentation under the PDF and html format. 
Documentation has been written with [Doxygen](https://www.doxygen.nl/).

# License

This project is distributed under the license GNU 3.0. 

# References

<a id="HappenedBefore">[1]</a> Time, Clocks, and the Ordering of Events in a Distributed System. Lamport, Leslie. Communications of the ACM 1978.

<a id="PC1">[2]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. PaCT 2017.

<a id="PC2">[3]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. Technical report 2017.

<a id="PC">[4]</a> A probabilistic Dynamic Clock Set to capture message causality. Wilhelm, Daniel and Arantes, Luciana and Sens, Pierre. Technical report 2017.

<a id="OMNeT++">[5]</a> The OMNET++ Discrete Event Simulation System. Varga, Andras. ESM 2001.
