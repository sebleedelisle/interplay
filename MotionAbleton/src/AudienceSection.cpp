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


AudienceSection::AudienceSection() {
	unwarped.allocate(150, 400, OF_IMAGE_COLOR);

	currentScene = 0;
	motionBand = 0;
	numScenes = 4;
	motionLevel = 0;
	smoothedMotionLevel = 0;
	motionLevelMin = 2;
	motionLevelMax = 10;
    
    //load type for our labels
    labelFont.loadFont("Variable", 24, true, true);
	//labelFont.loadFont("Futura", 16, true, true);
	labelFont.setLetterSpacing(1);
    labelFont.setSpaceSize(.4);


    
}


void AudienceSection::init(int channel, ofRectangle audienceArea, vector<ofPoint> srcMotionPoints, string instrumentName, ofColor instrumentColour) {
	
	abletonChannel = channel;
	
	for(int i = 0; i<srcMotionPoints.size(); i++) {
		warpPoints.push_back(Point2f(srcMotionPoints[i].x, srcMotionPoints[i].y));
	}
	
	area = audienceArea;
	name = instrumentName;
    colour = instrumentColour;

}


void AudienceSection::update(){
	
	AbletonController& ableton = *AbletonController::instance();
	
	if(ableton.barTriggered) currentScene = motionBand;
	
	// mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
	diffMean = mean(toCv(unwarped));
	
	// you can only do math between Scalars,
	// but it's easy to make a Scalar from an int (shown here)
	//diffMean *= Scalar(0.1);
	//motionLevel = ofClamp(diffMean[0],0,1);
	motionLevel = diffMean[0];
	
//	if(motionLevel>motionLevelMax) motionLevelMax+=(motionLevel - motionLevelMax) *0.1;
//	if(motionLevel<motionLevelMin) motionLevelMin+=(motionLevel - motionLevelMin) *0.1;
//	motionLevelMax*=0.99;
//	motionLevelMin*=1.01;
	
	motionLevel = ofMap(motionLevel,motionLevelMin, motionLevelMax, 0,1,true);
	
    
    int newmotionband = (smoothedMotionLevel * 4.0f);
    if(newmotionband!=motionBand) {
        motionBand = newmotionband;
        ableton.playClip(motionBand, abletonChannel);
		
    }
	
	//smoothedMotionLevel += (motionLevel - smoothedMotionLevel) *0.01;
	smoothedMotionLevel*=0.995;
	smoothedMotionLevel += (motionLevel*0.005);
    
    
 	
}




void AudienceSection :: draw() {
 	
	ofPushStyle();
	ofEnableAlphaBlending();
	ofNoFill();
    
	//showing the motion image in the top left
    ofPushMatrix();
	ofScale(0.2,0.2);
	unwarped.draw(abletonChannel*(unwarped.width+10), 0);
	ofPopMatrix();
	
	//
//	ofSetColor(255,0,255);
//	for(int i = 0; i<warpPoints.size(); i++) {
//		
//		ofPoint p(warpPoints[i].x, warpPoints[i].y);
//		
//		ofCircle(p, 2);
//		ofDrawBitmapString(ofToString(abletonChannel)+ " "+ ofToString(i), p);
//		
//		
//	}
    
	//rect defining the channel
    ofRectangle rect = area;
	
	
	
	ofPushMatrix();
    //translate to middle
	ofTranslate(0,28);
	//cout << ofGetMouseY() - ofGetHeight()/2 << endl;
	ofTranslate(rect.getCenter());
    //flatten it down a bit, perspectivy
	ofRotateX(70);
    //translate to bottom left
	ofTranslate(-rect.width/2,rect.height/2);
	//ofTranslate(0,-50,-50);
	//flipping the y scale -- might mess with the numbers
    ofScale(1,-1);
	
	
    //outline of each channel
  	ofSetColor(colour);
    ofRect(0,0,rect.width, rect.height);
	
  
	ofFill();
	
   ofSetColor(colour.r, colour.g, colour.b,100);
    
 
	ofSetLineWidth(4);
    
   // if((ofGetElapsedTimeMillis() % 400 < 200) && (currentScene!=motionBand))
	ofRect(0, (motionBand * (rect.height/numScenes)), rect.width, (rect.height/numScenes));
    
	
    ofSetColor(colour);
	ofRect(0,  0, rect.width, (rect.height/numScenes));

	ofSetColor(255,255,255);
	ofRect(0,smoothedMotionLevel*rect.height, rect.width/2,5);
	ofRect(rect.width/2,motionLevel*rect.height, rect.width/2,5);
    
    //box
    ofSetColor(colour);
    int boxWidth = area.width;
    int boxHeight = 60;
    ofTranslate(0,rect.height,0);
    ofScale(1,-1);

    ofRotateX(-70);
    ofTranslate(0,-boxHeight);
    ofRect(0, 0, boxWidth, boxHeight);
    
    ofSetColor(255,255,255);
    string message = name;
    float halfWidth = labelFont.stringWidth(message)/2;
    float halfHeight = labelFont.stringHeight(message)/2;
    labelFont.drawString(message, (boxWidth/2 - halfWidth), (boxHeight/2)+halfHeight);
    
	ofPopMatrix();
	
    
    //draw labels
    
    
    //float labelX = (200*abletonChannel);
    //float labelY = 100;

    /*
    //text
    ofPushMatrix();
    ofTranslate(labelX,labelY);
    ofSetColor(255,255,255);
    string message = instrumentNames[abletonChannel];
    float halfWidth = labelFont.stringWidth(message)/2;
    float halfHeight = labelFont.stringHeight(message)/2;
    labelFont.drawString(message, (boxWidth/2 - halfWidth), (boxHeight/2)+halfHeight);

    ofPopMatrix();
    */
	
	ofPopStyle();
}


void AudienceSection::updateMotionImage(ofImage &motionImage) {
	
	
	
	//copy(quads[i].begin(), quads[i].end(), back_inserter(warpPoints));
	unwarpPerspective(motionImage, unwarped, warpPoints);
	unwarped.update();



}