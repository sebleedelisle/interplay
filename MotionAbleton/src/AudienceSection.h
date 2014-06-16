//
//  AudienceSection.h
//  MotionAbleton
//
//  Created by Seb Lee-Delisle on 13/06/2014.
//
//

#pragma once

#include "AbletonController.h"
#include "ofxCv.h"
#include "ofxGui.h"

class AudienceSection {

	public :
	
	AudienceSection();

	void init(int tracknum, ofRectangle area, vector<ofPoint> srcMotionPoints, string instrumentName, ofColor instrumentColour, int realtrack = -1);
	void update();
	void draw();
	void reset();
	
	bool toggleEnabled(); 
	
	void updateMotionImage(ofImage& motionImage); 

	
	ofImage unwarped;
	vector<cv::Point2f> warpPoints;
	
	ofRectangle area; 
	
	cv::Scalar diffMean;
	
    ofParameter<bool> enabled;
	ofParameter<float> motionLevelRaw;
	ofParameter<float> motionLevel;
	ofParameter<float> motionLevelMin;
	ofParameter<float> motionLevelMax;
	
	ofxPanel gui;
	
	float smoothedMotionLevel;

	int abletonTrack;  // "fake" track for group tracks
	int abletonTrack2; // real track with audio on
	int motionBand;
    int currentClip;
	int numClips;
	
	long updateCount; 
    
    ofColor colour;
    string name;
    ofTrueTypeFont	labelFont;

};