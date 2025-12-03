#ifndef __DRONE__
#define __DRONE__

#include "DroneState.h"

class Drone {
    volatile bool requestTakeoff;
    volatile bool requestLanding;
    DroneState currentState;

public:
    Drone();
    void setDroneLANDING();
    void setDroneTAKINGOFF();
    void setDroneINSIDE();
    void setDroneOUTSIDE();
    bool isDroneLANDING();
    bool isDroneTAKINGOFF();
    bool isDroneINSIDE();
    bool isDroneOUTSIDE();
    void sendRequestTakeoff();
    bool isRequestTakeOffSent();
    void consumeRequestTakeOff();
    void sendRequestLanding();
    bool isRequestLandingSent();
    void consumeRequestLanding();
};


#endif
