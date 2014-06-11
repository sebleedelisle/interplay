#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "AbletonController.h"
#include "ofxCv.h"

using namespace cv;
using namespace ofxCv;

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
        void exit(); 
    
    
    
	 
    AbletonController ableton;
    ofVideoGrabber grabber;
    ofImage current;
    ofPixels previous;
    ofImage diff;
    cv::Scalar diffMean;
    float smoothedMean;
    int motionBand;
    int currentScene; 

    
};
