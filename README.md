libtcv
======

Library Transceiver
-------------------
This library provide an application-independent API to get/set information from/to a transceiver compliant with the SFF Committee specifications.


More Info
---------
This API is compliant with these MSAs:
 - SFF-8472 rev 11.3 (SFP/SFP+) (ftp://ftp.seagate.com/sff/SFF-8472.PDF)
 - INF-8077i rev 4.5 (XFP) (ftp://ftp.seagate.com/sff/INF-8077.PDF)

More information about MSAs are in the [SFF Committee Spacifications](http://www.sffcommittee.com/ie/Specifications.html) page.

SFP internal devices (both memories and PHYs) are accessed by I²C bus. These accesses are application-dependent, so the API user must register a couple I²C read and write callbacks, to perform these actions.


Implementation
--------------
The *libtcv* is developed in C.

**Requirements**
 - Application-independent;
 - Platform-independent;
 - Read/Write I²C callbacks;
 - User transparent devices read, write and refresh;
 - UTs;
 - Support XFP;
 - Support SFP;
 - Support SFP+;
 - Support TCV internal PHY configuration.

**To do**
 - Design brainstorm;
 - Implement SFP/SFP+ I²C read interface;
 - Implement SFP/SFP+ I²C write interface;
 - Implement XFP I²C read interface;
 - Implement XFP I²C write interface;
 - Implement SFP/SFP+ basic information (devAddr 0xA0) get functions;
 - Implement SFP/SFP+ basic information (devAddr 0xA0) set functions;
 - Implement SFP/SFP+ diagnostic (devAddr 0xA2) get functions;
 - Implement SFP/SFP+ diagnostic (devAddr 0xA2) set functions;
 - Implement XFP basic information (devAddr 0xA0, fixed page) get functions;
 - Implement XFP basic information (devAddr 0xA0, fixed page) set functions;
 - Implement XFP diagnostic (devAddr 0xA0, fixed page) get functions;
 - Implement XFP diagnostic (devAddr 0xA0, fixed page) set functions;
 - Implement XFP serial ID data (devAddr 0xA0, page 1) get functions;
 - Implement XFP serial ID data (devAddr 0xA0, page 1) set functions;
 - Implement XFP user EPROM data (devAddr 0xA0, page 2) get functions;
 - Implement XFP user EPROM data (devAddr 0xA0, page 2) set functions;
 - Implement UT.
 - Implement XFP simulator;
 - Implement SFP simulator.
