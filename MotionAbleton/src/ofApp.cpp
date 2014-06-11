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

    ableton.init("localhost", 9000);
    
    
    grabber.initGrabber(1280, 720);
    imitate(current, grabber);
    imitate(previous, grabber);
    imitate(diff, grabber);
            
    
}

//--------------------------------------------------------------
void ofApp::update(){ 

    ableton.update();
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
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    ofBackground(0);
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
        ableton.playClip(motionBand, 0);
    }
    
    ofSetColor(255,255,255);
    ofRect(0, motionBand * (380.0f/4.0f), 640, (380.0f/4.0f));
    ofSetColor(255,0,0,40);
    ofFill();
    ofRect(0, currentScene * (380.0f/4.0f), 640, (380.0f/4.0f));
    
    
	
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofxOscMessage m;

    
    /*
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
     
     */

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

void ofApp::exit() {
    
    ableton.exit();
    
    
}
/*
 
 
 
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
*/
