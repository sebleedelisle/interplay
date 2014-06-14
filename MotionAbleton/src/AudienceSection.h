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
	


	void update(ofImage& motionImage);
	void draw();

	cv::Scalar diffMean;

	float motionLevel; 
	float smoothedMean;

	int motionBand;
    int currentScene;


};