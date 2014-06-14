//
//  AudienceSection.cpp
//  MotionAbleton
//
//  Created by Seb Lee-Delisle on 13/06/2014.
//
//

#include "AudienceSection.h"
using namespace cv;
using namespace ofxCv;

void AudienceSection::update(ofImage &motionImage){
	
	AbletonController& ableton = *AbletonController::instance();
	// mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
	diffMean = mean(toCv(motionImage));
	
	// you can only do math between Scalars,
	// but it's easy to make a Scalar from an int (shown here)
	diffMean *= Scalar(0.1);
		
	motionLevel = ofClamp(diffMean[0],0,1);
    
    int newmotionband = (smoothedMean * 4.0f);
    if(newmotionband!=motionBand) {
        motionBand = newmotionband;
        ableton.playClip(motionBand, 0);
    }
	
}

void AudienceSection :: draw() {
	ofNoFill();
    smoothedMean += (motionLevel - smoothedMean) *0.06;
    ofSetColor(0,255,255);
    ofRect(0, 0, 640, smoothedMean*380);
	
	ofSetColor(255,255,255);
    ofRect(0, motionBand * (380.0f/4.0f), 640, (380.0f/4.0f));
    ofSetColor(255,0,0,40);
    ofFill();
    ofRect(0, currentScene * (380.0f/4.0f), 640, (380.0f/4.0f));

}
