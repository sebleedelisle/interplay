#include "ofApp.h"

using namespace ofxCv;
using namespace cv;
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);

	// set bg to a nice grey!
	ofBackground(150);

	// Enable some logging information
	ofSetLogLevel(OF_LOG_VERBOSE);

	//Client side
	clientDestination = "localhost";
	//	clientDestination	= "192.168.0.115"; // if you send to another instance enter IP here
	clientSendPort = 9000;
	clientSender.setup(clientDestination, clientSendPort);

	clientRecvPort = clientSendPort + 1;
	clientReceiver.setup(clientRecvPort);
    
    playing = false;
    
    
    masterMeter = 0;
    
    
    grabber.initGrabber(1280, 720);
    imitate(current, grabber);
    imitate(previous, grabber);
    imitate(diff, grabber);
            
    
}

//--------------------------------------------------------------
void ofApp::update(){ 

	// OSC receiver queues up new messages, so you need to iterate
	// through waiting messages to get each incoming message
    
    grabber.update();
    
    
    if(grabber.isFrameNew()) {
        
       
        
        copy(current, previous);
        
        // copy the new image into the current image
        copy(grabber.getPixelsRef(), current);
        
        // difference blend between the two images to highlight the
        // changes between frames
        absdiff(previous, current, diff);
        
        // and run a threshold filter on it if required
       // if(thresholdLevelParam>0)
       //     threshold(diff, thresholdLevelParam);
        
        
        diff.update();
        
        // mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
		diffMean = mean(toCv(diff));
		
		// you can only do math between Scalars,
		// but it's easy to make a Scalar from an int (shown here)
		diffMean *= Scalar(0.1);
        
        
    }
    
    
    
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
                masterMeter = m.getArgAsFloat(1);
            }
            
		} else if(m.getAddress() == "/live/track/meter"){
            int tracknum = m.getArgAsInt32(0);

            while(tracknum>=trackLevels.size()) {
                trackLevels.push_back(0);
            }
            
            trackLevels[tracknum] =m.getArgAsFloat(2);

		}
        else if(m.getAddress() == "/live/play"){
            if(m.getArgAsInt32(1) == 2) playing = true;
            else if(m.getArgAsInt32(1) == 1) playing = false;
            
		} else if(m.getAddress() == "/live/beat"){
            int beatnum = m.getArgAsInt32(0);
            
            if(beatnum%4 == 0 ) currentScene = motionBand; 
            
            
		}
        
	}

	//this is purely workaround for a mysterious OSCpack bug on 64bit linux
	// after startup, reinit the receivers
	// must be a timing problem, though - in debug, stepping through, it works.
	if(ofGetFrameNum() == 60){
		clientReceiver.setup(clientRecvPort);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    ofBackground(0);
    ofSetColor(255);
    ofRect(0,0,masterMeter*ofGetWidth(),10);

    for(int i = 0; i<trackLevels.size(); i++) {
        
        ofRect(0, i*20 + 20, trackLevels[i]*ofGetWidth(), 10);
        
    }
    grabber.draw(0,0, 640, 380);
    diff.draw(0,420, 640, 380);
    
    float motionLevel = ofClamp(diffMean[0],0,1);
    
    ofNoFill();
    smoothedMean += (motionLevel - smoothedMean) *0.06;
    ofSetColor(0,255,255);
    ofRect(0, 0, 640, smoothedMean*380);
    
    int newmotionband = (smoothedMean * 4.0f);
    if(newmotionband!=motionBand) {
        motionBand = newmotionband;
        playClip(motionBand, 0);
    }
    
    ofSetColor(255,255,255);
    ofRect(0, motionBand * (380.0f/4.0f), 640, (380.0f/4.0f));
    ofSetColor(255,0,0,40);
    ofFill();
    ofRect(0, currentScene * (380.0f/4.0f), 640, (380.0f/4.0f));
    
    
	
}


void ofApp::playClip(int num, int part){
    ofxOscMessage m;
    m.setAddress("/live/play/clipslot");
    m.addIntArg(part);
    m.addIntArg(num);
    clientSender.sendMessage(m);
    
    
};

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofxOscMessage m;

    if(key == ' ') {
       
        m.setAddress("/live/stop");
        clientSender.sendMessage(m);
    }
    else if(key == OF_KEY_RETURN) {
         m.setAddress("/live/play");
        clientSender.sendMessage(m);
    }
    else if(key == '1') {
        m.setAddress("/live/play/clipslot");
        m.addIntArg(0);
        m.addIntArg(0);
        clientSender.sendMessage(m);
    }
    else if(key == '2') {
        m.setAddress("/live/play/clipslot");
        m.addIntArg(0);
        m.addIntArg(1);
        clientSender.sendMessage(m);
    }
    else if(key == '3') {
        m.setAddress("/live/play/clipslot");
        m.addIntArg(0);
        m.addIntArg(2);
        clientSender.sendMessage(m);
    }
    else if(key == '4') {
        m.setAddress("/live/play/clipslot");
        m.addIntArg(0);
        m.addIntArg(3);
        clientSender.sendMessage(m);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
string ofApp::getOscMsgAsString(ofxOscMessage m){
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


