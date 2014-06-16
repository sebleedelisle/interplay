//
//  AbletonController.cpp
//  MotionAbleton
//
//  Created by Seb Lee-Delisle on 09/06/2014.
//
//

#include "AbletonController.h"

AbletonController * AbletonController :: ableton = NULL;
AbletonController * AbletonController::instance() {
	if(ableton ==NULL) {
		ableton = new AbletonController();
	}
	return ableton;
}

void AbletonController::init(string host) {
    
 	//Client side
	clientDestination = host;
	clientSendPort = 9000;
	clientSender.setup(clientDestination, clientSendPort);
    
	clientRecvPort = 9001;
	clientReceiver.setup(clientRecvPort);
    
    playing = false;
    barTriggered = false;
	beatLength = 60000/120; // defaults to 120bpm
	lastBeatTime = -1;
	beatProgress = 0;
    
    masterMeter = 0;
    
}

void AbletonController::update() {
    
	barTriggered = false;
	
    
	// check for waiting messages
	while(clientReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		clientReceiver.getNextMessage(&m);
        if(m.getAddress().find("meter")==string::npos)
            ofLogNotice("Client : ", getOscMsgAsString(m));
        
		// check the address of the incoming message
		if(m.getAddress() == "/live/master/meter"){
            
            if(m.getArgAsInt32(0) == 0 ) {
				
				masterMeter =m.getArgAsFloat(1);
				
            }
            
		} else if(m.getAddress() == "/live/track/meter"){
            int tracknum = m.getArgAsInt32(0);
            
            while(tracknum>=trackLevels.size()) {
                trackLevels.push_back(0);
            }
            
            trackLevels[tracknum] =m.getArgAsFloat(2);
            
		}
        else if(m.getAddress() == "/live/play"){
            if(m.getArgAsInt32(0) == 2) playing = true;
            else if(m.getArgAsInt32(0) == 1) playing = false;
            
		} else if(m.getAddress() == "/live/beat"){
            currentBeatNum = m.getArgAsInt32(0);
            
            if(currentBeatNum%16 == 0 ) barTriggered = true;
			
			if(lastBeatTime>=0) {
				long thisBeatTime = ofGetElapsedTimeMillis();
				
				beatLength = thisBeatTime - lastBeatTime;
				lastBeatTime = thisBeatTime;
				
				
				
			} else lastBeatTime = ofGetElapsedTimeMillis();
            
            
		} else if (m.getAddress() == "/live/clip/info") {
			int tracknum = m.getArgAsInt32(0);
			int clipnum = m.getArgAsInt32(1);
			int state = m.getArgAsInt32(2);
			if(tracknum >= trackClips.size()) {
				trackClips.resize(tracknum+1);
			}
			if(state == 2) trackClips[tracknum] = clipnum;
						
			cout << "setting trackstate " << tracknum << " c:" << clipnum << " state:" << state << endl;
			
			
		} else if (m.getAddress() == "/live/clip/position") {
			int tracknum = m.getArgAsInt32(0);
			int clipnum = m.getArgAsInt32(1);
			float position = m.getArgAsFloat(2);
			float length = m.getArgAsFloat(3);
			float loopstart = m.getArgAsFloat(4);
			
			if(tracknum >= trackClips.size()) {
				trackClips.resize(tracknum+1);
			}
			if(tracknum >= trackProgress.size()) {
				trackProgress.resize(tracknum+1);
			}
			
			trackClips[tracknum] = clipnum;
			trackProgress[tracknum] = fmod(position - loopstart, length) / length;
			
			cout << "setting trackprogress " << trackProgress[tracknum] << endl;
			
			
		}
    
	}

	//this is purely workaround for a mysterious OSCpack bug on 64bit linux
	// after startup, reinit the receivers
	// must be a timing problem, though - in debug, stepping through, it works.
	if(ofGetFrameNum() == 60){
		clientReceiver.setup(clientRecvPort);
	}
	
	if(lastBeatTime>0) beatProgress = (float)(ofGetElapsedTimeMillis() - lastBeatTime )/(float)beatLength;
	//latency = ofGetMouseY();
}


void AbletonController::draw() {
    
	ofPushStyle();
    ofSetColor(255);
	/*
    ofRect(0,0,masterMeter*ofGetWidth(),10);
    
    for(int i = 0; i<trackLevels.size(); i++) {
        
        ofRect(0, i*20 + 20, trackLevels[i]*ofGetWidth(), 10);
        
    }
	 */
    ofNoFill();
	ofRect(500,0,20,20);
	ofFill();
	ofRect(500,0,20*beatProgress, 20);
	
    ofPopStyle();
}

//--------------------------------------------------------------
string AbletonController::getOscMsgAsString(ofxOscMessage m){
	string msg_string;
	msg_string = m.getAddress();
	msg_string += ":";
	for(int i = 0; i < m.getNumArgs(); i++){
		// get the argument type
		msg_string += " " + m.getArgTypeName(i);
		msg_string += ":";
		// display the argument - make sure we get the right type
		if(m.getArgType(i) == OFXOSC_TYPE_INT32){
			msg_string += ofToString(m.getArgAsInt32(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
			msg_string += ofToString(m.getArgAsFloat(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
			msg_string += m.getArgAsString(i);
		}
		else{
			msg_string += "unknown";
		}
	}
	return msg_string;
}




void AbletonController::playClip(int num, int part){
    
    sendMessage("/live/play/clipslot", part,num);
	playing = true;

}

void AbletonController::sendMessage(string msg) {
	ofxOscMessage m;
	m.setAddress(msg);
	clientSender.sendMessage(m);
	
}
bool AbletonController:: togglePlaying(){
	if(playing) stopAll();
	else playFromBeginning();
	
	return playing;
	
	
}

int AbletonController :: getPlayingClipForTrack(int track){
	
	//cout << "getFirstPlayingClipForTrack " << track << endl;
	if(track>=trackClips.size()) {
		//	cout << "no data" << endl;
		return -1;
		
	} else return (trackClips[track]);
	
	
}

float AbletonController :: getProgressForTrack(int track){
	
	//cout << "getFirstPlayingClipForTrack " << track << endl;
	if(track>=trackProgress.size()) {
		//	cout << "no data" << endl;
		return 1;
		
	} else return (trackProgress[track]);
	
	
}

float AbletonController :: getLevelForTrack(int track){
	
	//cout << "getFirstPlayingClipForTrack " << track << endl;
	if(track>=trackLevels.size()) {
		//	cout << "no data" << endl;
		return 0;
		
	} else return (trackLevels[track]);
	
	
}

void AbletonController::playFromBeginning() {
		
	sendMessage("/live/play");
	playing = true;
	
}

void AbletonController::sendMessage(string msg, int p1) {
	ofxOscMessage m;
	m.setAddress(msg);
	m.addIntArg(p1);

	clientSender.sendMessage(m);
	
}
void AbletonController::sendMessage(string msg, int p1, int p2) {
	ofxOscMessage m;
	m.setAddress(msg);
	m.addIntArg(p1);
    m.addIntArg(p2);
	clientSender.sendMessage(m);
	
}


void AbletonController::stopAll() {
	
	
    ofxOscMessage m;
    
    m.setAddress("/live/stop");
    clientSender.sendMessage(m);
	playing = false;
	
	
}

void AbletonController::exit() {
	
	stopAll();
	
}