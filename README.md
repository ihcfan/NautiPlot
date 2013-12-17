NautiPlot
=========

Wireless Nautical Navigation System

The intent of this project is to make a fully functional marine ECDIS and electronics integration environment.  Based heavily on the great work done by the OpenCPN project, this project will refactor and rewrite the code to be fully portable across most operating systems (OSX/iOS/Linux/Android/etc.).  In addition, the system will become a distributed, service oriented model, to facilitate better implementation in marine environments.  To that end, the following are high level requriements:

1) The HMI shall be portable across OSX, Windows, Linux, iOS, and Android.
2) Chart management shall be implemented as seperable services.
3) Sensor information shall be implemented as seperable services.
4) Any HMI on the network shall be capable of synchronous or independant operation of other HMI's.

These high level requirements resulted in several early decisions about the architecture.

1) The open source cocos2d-x framework was chosen over OpenCPN's wxwidgets implementation for the HMI.  The nature of the primary HMI (a chartplotter type environment) is such that 2d game engines better meet the need.  Furthermore, cocos2d-x is uber portable, thus enabling concurrent HMI development across all the target platforms without branching.

2) The "seperable service" requirements facilitate process scaling based on the available HMI devices.  Because the HMI needs to operate effectively on an iPhone and a PC, it is necessary to decompose the architecture such that lower capability devices can still function well.  This gives rise to the service oriented architecture concepts implemented throughout the system.

project lead: Keith Ballantyne
