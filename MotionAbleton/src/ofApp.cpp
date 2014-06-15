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
	//ofSetLogLevel(OF_LOG_VERBOSE);

    ableton.init("localhost");
    
	float imageScale = 0.5;
	
    grabber.setDeviceID(1);
    grabber.initGrabber(1280*imageScale, 720*imageScale, 30);
	//grabber.videoSettings();
    
	imitate(current, grabber);
	imitate(previous, grabber);
	imitate(diff, grabber);
	
//	current.allocate(1280, 720*0.5, OF_IMAGE_COLOR);
//	imitate(previous, current);
//	imitate(diff, current);
	
    thresholdLevel = 0;
	
	audienceSections.push_back(AudienceSection());
	audienceSections.push_back(AudienceSection());
	audienceSections.push_back(AudienceSection());
	audienceSections.push_back(AudienceSection());

	AudienceSection& audience1 = audienceSections[0];
	AudienceSection& audience2 = audienceSections[1];
	AudienceSection& audience3 = audienceSections[2];
	AudienceSection& audience4 = audienceSections[3];

	
	for(int i = 0; i<4; i++) {
		float x = ofMap(i, 0, 3, ofGetWidth()*0.2, ofGetWidth()*0.8);
		float y = ofGetHeight()/2;
		
		float halfwidth = (ofGetWidth()*0.6/6)-2;
		float halfheight = 320;
		
		
		ofRectangle rect(x-halfwidth, y - halfheight, halfwidth*2, halfheight*2);
		
		vector<ofPoint> points;
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.2, ofGetWidth()*0.6), ofGetHeight()*0.2));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.4, ofGetWidth()*0.8), ofGetHeight()*0.2));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.2, ofGetWidth()*0.9), ofGetHeight()*0.8));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.1, ofGetWidth()*0.8), ofGetHeight()*0.8));
		
		for(int i = 0; i<points.size(); i++) points[i]*=imageScale;
		
		audienceSections[i].init(i,rect, points);
		
	}
	
	audiencePreview.loadImage("img/FPORoom.jpg");
 
}

//--------------------------------------------------------------
void ofApp::update(){ 

    ableton.update();
    grabber.update();
    
    if(grabber.isFrameNew()) {
        
        copy(current, previous);
        
        // copy the new image into the current image
        copy(grabber.getPixelsRef(), current);
		
		current.mirror(false, true);
        
        // difference blend between the two images to highlight the
        // changes between frames
        absdiff(previous, current, diff);
        
        // and run a threshold filter on it if required
        if(thresholdLevel>0)
            threshold(diff, thresholdLevel);

        diff.update();
        
        // mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
		diffMean = mean(toCv(diff));
		
		// you can only do math between Scalars,
		// but it's easy to make a Scalar from an int (shown here)
		diffMean *= Scalar(0.1);
		
		for(int i = 0; i<audienceSections.size(); i++) {
			audienceSections[i].updateMotionImage(diff);
		}
        
        
    }
	for(int i = 0; i<audienceSections.size(); i++) {
		audienceSections[i].update();
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    ofBackground(0);
	ofSetColor(255);
	current.draw(0,0, 1280, 720);
	audiencePreview.draw(0,0,1280,720);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
    diff.draw(0,0, 1280, 720);
    ofDisableBlendMode();
	
	for(int i = 0; i<audienceSections.size(); i++) {
		audienceSections[i].draw();
	}
	
	
	

	ableton.draw();
	
	
	/*
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
    
    
	*/
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofxOscMessage m;
    
    if(key == ' ') {
       
        ableton.sendMessage("/live/stop");
    }
    else if(key == OF_KEY_RETURN) {
         ableton.sendMessage("/live/play");

    }
	/*
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
        m.setAddress("/live/play/clipsldi di	ot");
        m.addIntArg(0);
        m.addIntArg(3);
        clientSender.sendMessage(m);
    }*/
     
    

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

void ofApp::exit() {
    
    ableton.exit();
    
    
}
