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

class AudienceSection {

	public :
	
	AudienceSection();

	void init(int channel, ofRectangle area, vector<ofPoint> srcMotionPoints);
	void update();
	void draw();
	
	void updateMotionImage(ofImage& motionImage); 

	
	ofImage unwarped;
	vector<cv::Point2f> warpPoints;
	
	ofRectangle area; 
	
	cv::Scalar diffMean;

	float motionLevel;
	float motionLevelMin;
	float motionLevelMax;
	float smoothedMotionLevel;

	int abletonChannel; 
	int motionBand;
    int currentScene;
	int numScenes;
    
    vector<ofColor> fillColours;

};