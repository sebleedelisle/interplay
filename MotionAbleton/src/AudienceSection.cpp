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
	unwarped.allocate(80, 200, OF_IMAGE_COLOR);

	currentClip = 0;
	motionBand = 0;
	numClips = 5;
	motionLevel = 0;
	smoothedMotionLevel = 0;
	motionLevelRaw = 0;
	motionLevelMin = 2;
	motionLevelMax = 10;
    
    //load type for our labels
    labelFont.loadFont("Variable Black", 25, true, true, true);

	labelFont.setLetterSpacing(1);
    labelFont.setSpaceSize(.4);
	
	updateCount = 0;
 
}


void AudienceSection::init(int tracknum, ofRectangle audienceArea, vector<ofPoint> srcMotionPoints, string instrumentName, ofColor instrumentColour, int realtrack) {
	
	AbletonController& ableton = *AbletonController::instance();
	
	abletonTrack = tracknum;
	abletonTrack2 = realtrack;
	
	for(int i = 0; i<srcMotionPoints.size(); i++) {
		warpPoints.push_back(Point2f(srcMotionPoints[i].x, srcMotionPoints[i].y));
	}
	
	area = audienceArea;
	name = instrumentName;
    colour = instrumentColour;
	
	ableton.sendMessage("/live/play/clipslot", abletonTrack, 0);
	if(abletonTrack2>=0) ableton.sendMessage("/live/play/clipslot", abletonTrack2, 0);

	ableton.sendMessage("/live/stop");
	
	//gui.add(motionLevel.set("motionLevel", 0, 0,20));
	motionLevelRaw.set("", 0, 0,20);
	gui.add(motionLevelMin.set("motionLevelMin" + ofToString(abletonTrack) , 4, 0,20));
	gui.add(motionLevelMax.set("motionLevelMax" + ofToString(abletonTrack) , 10, 0,20));
	
	enabled.set("enabled", false);

}


void AudienceSection::update(){
	
	AbletonController& ableton = *AbletonController::instance();
	
	//if(ableton.isBarTriggered()) currentClip = motionBand;
	
	// mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
	
    
	long time = ofGetElapsedTimeMillis();
	
	// updates 60 times a second
	while((float)time/30.0f > updateCount) {
		
		diffMean = mean(toCv(unwarped));
		
		if(time<5000) {
			diffMean[0] = 0;
			motionLevelRaw = 0;
			motionLevel = 0;
			smoothedMotionLevel = 0;
		}
		motionLevelRaw += (diffMean[0]-motionLevelRaw) *0.3;
		motionLevel = ofMap(motionLevelRaw,motionLevelMin, motionLevelMax, 0,1,true);

		
		smoothedMotionLevel*=0.997;
		smoothedMotionLevel += (motionLevel*0.005);
		if(smoothedMotionLevel>=0.99) smoothedMotionLevel = 0.99;
		updateCount++;
		
		if(updateCount%4 == 0) {
			//for(int i = 0; i<numClips; i++) {
			//	ableton.sendMessage("/live/track/info", abletonTrack);
			
			//}
		}
	}
    
 	
    int newmotionband = (smoothedMotionLevel * numClips);
    if(newmotionband!=motionBand) {
        motionBand = newmotionband;
        if(ableton.isPlaying()) {
			ableton.playClip(motionBand, abletonTrack);
			if(abletonTrack2>=0) ableton.playClip(motionBand, abletonTrack2);
		}
    }
	
	currentClip = ableton.getPlayingClipForTrack(abletonTrack);
	if(currentClip < 0) currentClip = 0;

}




void AudienceSection :: draw() {
	
	AbletonController& ableton = *AbletonController::instance();

	ofPushStyle();
	ofEnableAlphaBlending();
	
	ofNoFill();
    
	//showing the motion image in the top left
    ofPushMatrix();
	ofScale(0.2,0.2);
	unwarped.draw(abletonTrack*(unwarped.width+10), 0);
	ofPopMatrix();
	
	//
//	ofSetColor(255,0,255);
//	for(int i = 0; i<warpPoints.size(); i++) {
//		
//		ofPoint p(warpPoints[i].x, warpPoints[i].y);
//		
//		ofCircle(p, 2);
//		ofDrawBitmapString(ofToString(abletonTrack)+ " "+ ofToString(i), p);
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
	ofRectangle block;
  
    float thickness = 15;
 
	//ofSetLineWidth(4);
    
    if((ofGetElapsedTimeMillis()%300>150) && (currentClip!=motionBand)) {
		ofSetColor(colour.r, colour.g, colour.b,100);

		block.set(0,motionBand * (rect.height/numClips), rect.width, rect.height/numClips);
		ofRect(block);
		
		/*
		ofPushMatrix();
		ofTranslate(block.x, block.y);
		ofRect(0, 0, thickness, block.height); // left
		ofRect(thickness, 0, block.width - (thickness*2), thickness); // top
		ofRect(block.width-thickness,0, thickness, block.height); // right
		ofRect(thickness, block.height - thickness, block.width - (thickness*2), thickness); // top
		ofPopMatrix();
		 */

	
	
	}
	
    ofSetColor(colour);
	
	float progress = ableton.getProgressForTrack(abletonTrack);
	block.set(0,currentClip * (rect.height/numClips), rect.width, rect.height/numClips);
	
	
	ofPushMatrix();
	
	ofTranslate(block.x, block.y);
	ofSetColor(colour.r, colour.g, colour.b);
	
	ofRect(0, 0, thickness, block.height); // left
	ofRect(thickness, 0, block.width - (thickness*2), thickness); // top
	ofRect(block.width-thickness,0, thickness, block.height); // right
	ofRect(thickness, block.height - thickness, block.width - (thickness*2), thickness); // top
	
	ofSetColor(colour.r, colour.g, colour.b,100);
	
	ofRect(thickness, thickness, block.width - (thickness*2), (block.height- (thickness*2)) * (1-progress)); // top
	//ofRect(0, 0, block.width , (block.height) * (1-progress)); // top
	
	ofPopMatrix();

	ofSetColor(255,255,255);
	ofRect(0,smoothedMotionLevel*rect.height, rect.width/2,5);
	ofRect(rect.width/2,motionLevel*rect.height, rect.width/2,5);
    
    //box
    ofSetColor(colour);
    int boxWidth = area.width;
    int boxHeight = 60;
	float volume = ableton.getLevelForTrack(abletonTrack)*100 + ableton.getLevelForTrack(abletonTrack2)*100;
    ofTranslate(0,rect.height,0);
    ofScale(1,-1);

    ofRotateX(-70);
    ofTranslate(0,-boxHeight);
    ofRect(0, 0, boxWidth, boxHeight);
	ofSetColor(colour, 100);
	ofRect(0,0,boxWidth, -volume);
    
	// label
    ofSetColor(255,255,255);
    string message = name;
    float halfWidth = labelFont.stringWidth(message)/2;
    float halfHeight = labelFont.stringHeight(message)/2;
    labelFont.drawString(message, (boxWidth/2 - halfWidth), (boxHeight/2)+halfHeight);
    
	ofPopMatrix();
	
    
	ofPopStyle();
}


void AudienceSection::updateMotionImage(ofImage &motionImage) {

	//copy(quads[i].begin(), quads[i].end(), back_inserter(warpPoints));
	unwarpPerspective(motionImage, unwarped, warpPoints);
	unwarped.update();


}