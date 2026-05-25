#ifndef __DRONE__
#define __DRONE__


class Drone {
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

private:
    enum DroneState { INSIDE, OUTSIDE, LANDING, TAKING_OFF };
    DroneState currentState;
    volatile bool requestTakeoff;
    volatile bool requestLanding;


};


#endif
