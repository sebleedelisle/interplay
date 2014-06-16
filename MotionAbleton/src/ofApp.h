#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "AudienceSection.h"

#include "AbletonController.h"
#include "ofxCv.h"
#include "ofxGui.h"

using namespace cv;
using namespace ofxCv;

class ofApp : public ofBaseApp {

	public:

		ofApp() : ableton(*AbletonController::instance()){
		};
		void setup();
		void update();
		void draw();
    
		void initGui();
	
		void keyPressed(int key);
		void keyReleased(int key);
        void exit(); 
    
    
	ofFbo outputFbo; 
	
	int thresholdLevel; 
    AbletonController& ableton;
	vector <AudienceSection> audienceSections;
		
    ofVideoGrabber grabber;
    ofImage current;
    ofPixels previous;
    ofImage diff;
    cv::Scalar diffMean;
    float smoothedMean;
    int motionBand;
    int currentScene;
	
	ofImage audiencePreview;
	
	ofxPanel appGui;
	ofParameter<bool> test; 
    

    
};
