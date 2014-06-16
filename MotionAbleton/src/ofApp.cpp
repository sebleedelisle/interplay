#include "ofApp.h"

using namespace ofxCv;
using namespace cv;



//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);
	

	outputFbo.allocate(1280, 720, GL_RGB, 6);

    ableton.init("localhost");
    
	float imageScale = 1;
	
    grabber.setDeviceID(1);
    grabber.initGrabber(1280*imageScale, 720*imageScale, 30);
   
	//current.allocate(1280*imageScale, 720*imageScale, OF_IMAGE_COLOR);
	imitate(current, grabber);
	imitate(previous, current);
	imitate(diff, current);
	//diff.allocate(1280*imageScale, 720*imageScale, OF_IMAGE_GRAYSCALE);
	
	audienceSections.push_back(AudienceSection());
	audienceSections.push_back(AudienceSection());
	audienceSections.push_back(AudienceSection());
	audienceSections.push_back(AudienceSection());

	AudienceSection& audience1 = audienceSections[0];
	AudienceSection& audience2 = audienceSections[1];
	AudienceSection& audience3 = audienceSections[2];
	AudienceSection& audience4 = audienceSections[3];
    
    vector<ofColor> fillColours;
    fillColours.push_back(ofColor(255,206,39,255));
    fillColours.push_back(ofColor(124,206,248,255));
    fillColours.push_back(ofColor(35,121,172,255));
    fillColours.push_back(ofColor(229,49,35,255));
	
    vector<string> instrumentNames;
    instrumentNames.push_back("DRUMS");
    instrumentNames.push_back("BASS");
    instrumentNames.push_back("SYNTH");
    instrumentNames.push_back("GUITAR");

	
	for(int i = 0; i<4; i++) {
		float x = ofMap(i, 0, 3, ofGetWidth()*0.25, ofGetWidth()*0.75);
		float y = ofGetHeight()/2;
		
		float halfwidth = (ofGetWidth()*0.5/6)-4;
		float halfheight = 320;
		 
		 
		ofRectangle rect(x-halfwidth, y - (halfheight*1.5), halfwidth*2, halfheight*1);
		
		// warp points
		vector<ofPoint> points;
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.24, ofGetWidth()*0.63), ofGetHeight()*0.40));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.37, ofGetWidth()*0.76), ofGetHeight()*0.40));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.15, ofGetWidth()*1.2), ofGetHeight()*0.87));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*-0.2, ofGetWidth()*0.86), ofGetHeight()*0.87));
		
		for(int i = 0; i<points.size(); i++) points[i]*=imageScale;
		
		int secondTrack = -1;
		if(i==0) secondTrack = 4;
		else if(i==3) secondTrack = 5;
		audienceSections[i].init(i,rect, points, instrumentNames[i], fillColours[i], secondTrack);
		
	}
	
	audiencePreview.loadImage("img/FPORoom.jpg");
    
 
    initGui();
   
    
    
 
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
        if(motionThreshold>0)
            threshold(diff, motionThreshold);

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
	
	outputFbo.begin();
	
	ofSetupScreenPerspective(1280,720,100);
    
	
	ofFill();
    ofBackground(0);
	ofSetColor(255 * camBrightness);
	current.draw(0,0, 1280, 720);
	//ofSetColor(100);
	//audiencePreview.draw(0,0,1280,720);
	
	ofSetColor(255 * motionBrightness);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
    diff.draw(0,0, 1280, 720);
	ofSetColor(255);
    ofDisableBlendMode();
	
	for(int i = 0; i<audienceSections.size(); i++) {
		audienceSections[i].draw(appGui.getVisible());
	}
	
	ableton.draw();

	outputFbo.end();
	
	outputFbo.draw(0,0);
	
	appGui.draw();


}

void ofApp::initGui() {
	
	ofxBaseGui::setDefaultWidth(300);
    ofxBaseGui::setDefaultHeight(16);
	ofxBaseGui::setDefaultSpacing(3);
    ofxBaseGui::setDefaultElementSpacing(3);
	ofxBaseGui::setDefaultElementIndentation(1);
	ofxBaseGui::setDefaultTextPadding(5);
	ofxBaseGui::setUseTTF(true);
	
	ofxBaseGui::loadFont("VeraMoIt.ttf",8, true);
	
	appGui.setup("App settings", "settings.xml");
	appGui.setPosition(ofPoint(10,10));
	appGui.setVisible(false);
	
	for(int i = 0; i<audienceSections.size(); i++) {
	
		//appGui.add(audienceSections[i].enabled);
		appGui.add(audienceSections[i].motionLevelRaw);
		
		appGui.add(audienceSections[i].motionLevelMin);
		appGui.add(audienceSections[i].motionLevelMax);
	}
	
	appGui.add(camBrightness.set("camera brightness", 1,0,1));
	appGui.add(motionBrightness.set("motion brightness", 1,0,1));
	appGui.add(motionThreshold.set("motion threshold", 0,0,255));
	
	appGui.load();
	for(int i = 0; i<audienceSections.size(); i++) {
		audienceSections[i].enabled = true;
		audienceSections[i].motionLevelRaw = 0;
	}
	
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofxOscMessage m;
    
    if(key == ' ') {
       
//        ableton.sendMessage("/live/stop");
		ableton.togglePlaying();
		
    }
  	
	if(key=='f') {
		ofToggleFullscreen();
		
	}
	
	if(key=='\t') {
		appGui.toggleVisible();
		
	}
    else if(key == '1') {
        audienceSections[0].toggleEnabled();
    } else if(key == '2') {
        audienceSections[1].toggleEnabled();
    } else if(key == '3') {
        audienceSections[2].toggleEnabled();    
    } else if(key == '4') {
        audienceSections[3].toggleEnabled();
    }
	
	if(key=='s') {
		current.saveImage(ofGetTimestampString("%Y%m%d-%H%M%S")+".png");
	}
    
    

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

void ofApp::exit() {
    current.saveImage(ofGetTimestampString("%Y%m%d-%H%M%S")+".png");
    ableton.exit();
	appGui.save();
    
    
}
