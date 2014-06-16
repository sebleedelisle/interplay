#include "ofApp.h"

using namespace ofxCv;
using namespace cv;



//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);
	

	outputFbo.allocate(1280, 720, GL_RGB, 6);

    ableton.init("localhost");
    
	float imageScale = 0.5;
	
    grabber.setDeviceID(1);
    grabber.initGrabber(1280*imageScale, 720*imageScale, 30);
   
	imitate(current, grabber);
	imitate(previous, grabber);
	imitate(diff, grabber);
	
    thresholdLevel = 0;
	
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
		
		
		ofRectangle rect(x-halfwidth, y - halfheight, halfwidth*2, halfheight*2);
		
		vector<ofPoint> points;
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.2, ofGetWidth()*0.6), ofGetHeight()*0.2));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.4, ofGetWidth()*0.8), ofGetHeight()*0.2));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.2, ofGetWidth()*0.9), ofGetHeight()*0.8));
		points.push_back(ofPoint(ofMap(i, 0, 3, ofGetWidth()*0.1, ofGetWidth()*0.8), ofGetHeight()*0.8));
		
		for(int i = 0; i<points.size(); i++) points[i]*=imageScale;
		
		audienceSections[i].init(i,rect, points, instrumentNames[i], fillColours[i], i+4);
		
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
	
	outputFbo.begin();
	
	//ofSetupScreenPerspective(1280,720,100);
    
	
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
	appGui.setVisible(true);
	appGui.add(test.set("test", false));
	
	for(int i = 0; i<audienceSections.size(); i++) {
	
		appGui.add(audienceSections[i].enabled);
		appGui.add(audienceSections[i].motionLevelRaw);
		
		appGui.add(audienceSections[i].motionLevelMin);
		appGui.add(audienceSections[i].motionLevelMax);
	}
	//laserGui.add(&laserManager.connectButton);
	//laserGui.add(laserManager.parameters);
	
	appGui.load();
	for(int i = 0; i<audienceSections.size(); i++) {
		audienceSections[i].enabled = false;
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
	appGui.save();
    
    
}
