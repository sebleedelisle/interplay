//
//  AbletonController.h
//  MotionAbleton
//
//  Created by Seb Lee-Delisle on 09/06/2014.
//
//
#pragma once

#include "ofxOsc.h"

class AbletonController {

    public:

    void init(string host, int portnumber);
    
    void update();
    
    void draw(); 
    
    void playClip(int num, int part);

    
    
    void exit(); 
    
    ofxOscSender clientSender; // all-important ofxOscSender object
    string clientDestination; // IP address we're sending to
    int clientSendPort; // port we're sending to
    
    ofxOscReceiver clientReceiver; // OSC receiver
    int clientRecvPort; // port where we listen for messages
    
    // Parse an OscMessage into a string for easy logging
    string getOscMsgAsString(ofxOscMessage m);

    
    float masterMeter;
    vector<float> trackLevels;
    bool playing;





};