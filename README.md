@mainpage
@author Daniel Wilhelm

Implementation of error detectors when using probabilistic clocks [[1]](https://link.springer.com/chapter/10.1007/978-3-319-62932-2_31) to implement causal broadcast. 

This project provides implementations of causal broadcast using the following control mechanisms:

1. **No control:** Messages are delivered upon reception without any control. 
2. **Probabilistic clocks:** Messages are delivered upon reception once their appended Probabilistic clock satisfies the delivery conditions. 
3. **Probabilistic clocks:** Messages are delivered upon reception once their appended Probabilistic clock satisfies the delivery conditions. Furthermore, delivered messages are analyzed by the error detector proposed by Mostéfaoui and Weiss [[2]](https://hal.science/hal-02056349/document). 
3. **Probabilistic clocks:** Messages are delivered upon reception once their appended Probabilistic clock satisfies the delivery conditions. Furthermore, delivered messages are analyzed by the hash-based error detector proposed by Wilhelm and al.[[3]](https://hal.science/hal-03984499). 



Hey saluut je suis danielllll\n
Hey saluut je suis danielllll
Hey saluut je suis danielllll
Hey saluut je suis danielllll
Hey saluut je suis danielllll
Hey saluut je suis danielllll
Hey saluut je suis danielllll
Hey saluut je suis danielllll
Hey saluut je suis danielllll
Hey saluut je suis danielllll


@image html documentation/mainpage/errorDetectorInheritance.png?raw=true
![Error Detector inheritance graph.](documentation/mainpage/errorDetectorInheritance.png?raw=true)
![Error Detector inheritance graph.](https://github.com/Wilhelm2/ErrorDetectors/blob/master/documentation/mainpage/errorDetectorInheritance.png?raw=true)



## References

<a id="PC">[1]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. PaCT 2017.
<a id="PC">[1]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. Technical report 2017.
<a id="PC">[1]</a> A probabilistic Dynamic Clock Set to capture message causality. Wilhelm, Daniel and Arantes, Luciana and Sens, Pierre. Technical report 2017.
