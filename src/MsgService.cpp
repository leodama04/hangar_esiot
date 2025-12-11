#include "Arduino.h"
#include "MsgService.h"
#include "global.h"

String content;
MsgServiceClass MsgService;


bool MsgServiceClass::isMsgAvailable(){
  return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::init(){
  Serial.begin(9600);
  content.reserve(256);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  

  delay(100);
  MsgService.sendMsg("READY");
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);  
}

void serialEvent() {
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      MsgService.currentMsg = new Msg(content);
      MsgService.msgAvailable = true;      
    } else {
      content += ch;      
    }
  }
}

bool MsgServiceClass::isMsgAvailable(Pattern& pattern){
  return (msgAvailable && pattern.match(*currentMsg));
}

Msg* MsgServiceClass::receiveMsg(Pattern& pattern){
  if (msgAvailable && pattern.match(*currentMsg)){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  } 
}

void MsgServiceClass::sendMsgTAKINGOFF() {
  Serial.println("DRONE STATE: TAKING_OFF");
}

void MsgServiceClass::sendMsgLANDING() {
  Serial.println("DRONE STATE: LANDING");
}

void MsgServiceClass::sendMsgOUTSIDE() {
  Serial.println("DRONE STATE: OPERATING");
}

void MsgServiceClass::sendMsgINSIDE() {
  Serial.println("DRONE STATE: REST");
}

void MsgServiceClass::sendMsgRESET() {
  Serial.println("RESET");
}

void MsgServiceClass::sendMsgALARM() {
  Serial.println("ALARM");
}

void MsgServiceClass::sendMsgDRUALARM() {
  Serial.println("IL SISTEMA E IN STATO DI ALLARME");
}

void MsgServiceClass::sendMsgPREALARM() {
  Serial.println("PREALARM");
}

void MsgServiceClass::sendDistance(float dist) {
  Serial.println("DISTANCE: " + String(dist));
}

void MsgServiceClass::sendTemp(float temp) {
  Serial.println("TEMP: " + String(temp));
}

enum Command { CMD_TAKEOFF, CMD_LAND, CMD_UNKNOWN };

static Command parseCommand(String content){
  if (content == "takeoff") return CMD_TAKEOFF;
  if (content == "land") return CMD_LAND;
  return CMD_UNKNOWN;
}

void MsgServiceClass::handleMessage() {
  if (MsgService.isMsgAvailable()){
    Msg* m = MsgService.receiveMsg();
    if (m == NULL) return;
    String content = m->getContent();
    Command cmd = parseCommand(content);
    switch(cmd){
      case CMD_TAKEOFF:
        if (drone.isDroneINSIDE()){
          drone.sendRequestTakeoff();
        } else {
          MsgService.sendMsg("ERROR: drone is in state OUTSIDE");
        }
        break;
      case CMD_LAND:
        if (drone.isDroneOUTSIDE()){
          drone.sendRequestLanding();
        } else {
          MsgService.sendMsg("ERROR: drone is in state INSIDE");
        }
        break;
      case CMD_UNKNOWN:
      default:
        MsgService.sendMsg("ECHO: " + content);
        break;
    }

    delete m;
  }
}