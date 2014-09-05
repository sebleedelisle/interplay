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
	//motionLevelRawSmoothed = 0;
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
	ableton.sendMessage("/live/mute", abletonTrack, 0);
	if(abletonTrack2>=0) ableton.sendMessage("/live/mute", abletonTrack2, 0);

	ableton.sendMessage("/live/stop");
	
	//gui.add(motionLevel.set("motionLevel", 0, 0,20));
	motionLevelRaw.set("", 0, 0,20);
	gui.add(motionLevelMin.set("motionLevelMin" + ofToString(abletonTrack) , 4, 0,20));
	gui.add(motionLevelMax.set("motionLevelMax" + ofToString(abletonTrack) , 10, 0,20));
	
	enabled.set("enabled", false);

}


void AudienceSection::update(int motionsetting){
	
	AbletonController& ableton = *AbletonController::instance();
	
	//if(ableton.isBarTriggered()) currentClip = motionBand;
	
	// mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
	
    
	long time = ofGetElapsedTimeMillis();
	
	// updates 60 times a second
	while((float)time/30.0f > updateCount) {
		
		diffMean = mean(toCv(unwarped));
		
		if(time<2000) {
			diffMean[0] = 0;
			motionLevelRaw = 0;
//			motionLevelRawSmoothed = 0;
			motionLevel = 0;
			smoothedMotionLevel = 0;
		}
		motionLevelRaw += (diffMean[0]-motionLevelRaw) *0.1;
//		motionLevelRawSmoothed += (motionLevelRaw - motionLevelSmoothed)
		motionLevel = ofMap(motionLevelRaw,motionLevelMin, motionLevelMax, 0,1,true);

		float resetSpeed;
		float moveSpeed;
		if(motionsetting == 0) {
			resetSpeed = 0.997;
			moveSpeed = 0.005;
			
		} else if(motionsetting == 1) {
			resetSpeed = 0.989;
			moveSpeed = 0.015;
			
		} else if(motionsetting == 2) {
			resetSpeed = 0.98;
			moveSpeed = 0.025;
			
		} else if(motionsetting == 3) {
			resetSpeed = 0.96;
			moveSpeed = 0.04;
			
		}
		
		
		smoothedMotionLevel*= resetSpeed;//0.96;//0.997;
		smoothedMotionLevel +=  (motionLevel*moveSpeed);//(motionLevel*0.005);
		if(smoothedMotionLevel>=0.99) smoothedMotionLevel = 0.99;
		updateCount++;
		
		if(updateCount%4 == 0) {
			//for(int i = 0; i<numClips; i++) {
			//	ableton.sendMessage("/live/track/info", abletonTrack);
			
			//}
		}
	}
    
 	
    int newmotionband = (smoothedMotionLevel+ (1.0f/((numClips*2.0f)-1.0f))) * ((float)numClips-0.5f);// + (1.0f/((numClips*2.0f)-1.0f));
    if(newmotionband!=motionBand) {
        
        if(ableton.isPlaying() && enabled) {
			motionBand = newmotionband;
			ableton.playClip(motionBand, abletonTrack);
			if(abletonTrack2>=0) ableton.playClip(motionBand, abletonTrack2);
		}
    }
	
	if(enabled) currentClip = ableton.getPlayingClipForTrack(abletonTrack);
	if(currentClip < 0) currentClip = 0;

}




void AudienceSection :: draw(bool showMotionDebug, bool showMotionAreas) {
	
	AbletonController& ableton = *AbletonController::instance();

	float angle = 30.0f; // 50 for NYC
	
	
	ofPushStyle();
	ofEnableAlphaBlending();
    
    
    // TO SHOW MOTION AREAS
    if(showMotionAreas) {
        ofPushMatrix();     // --------------------------- 1
        
        
        ofNoFill();
        //ofScale(2,2);
        ofSetColor(colour);
        ofDisableBlendMode();
        
        for(int i = 0; i<warpPoints.size(); i++) {
            
            ofPoint p(warpPoints[i].x, warpPoints[i].y);
            
            ofCircle(p, 2);
            ofDrawBitmapString(ofToString(abletonTrack)+ " "+ ofToString(i), p);
            
            
        }
        ofBeginShape();
        for(int i = 0; i<=warpPoints.size(); i++) {
            
            ofPoint p(warpPoints[i%4].x, warpPoints[i%4].y);
            
            ofVertex(p);
            //ofDrawBitmapString(ofToString(abletonTrack)+ " "+ ofToString(i), p);
            
            
        }
        ofEndShape();
        ofPopMatrix();     // --------------------------- 0

    }
    
	
	ofNoFill();
    
	if(showMotionDebug) {
		//showing the motion image in the top right
		ofPushMatrix();     // --------------------------- 1
		ofTranslate(1280,0);
		ofScale(0.4,0.4);
		unwarped.draw((abletonTrack-4)*(unwarped.width+10), 0);
		ofPopMatrix();     // --------------------------- 0
	}
	
	   
	//rect defining the channel
    ofRectangle rect = area;
	
	
	
	ofPushMatrix();     // ---------------------------1
    //translate to middle
	ofTranslate(0,355, 20);
	//cout << ofGetMouseY() << endl;
	ofTranslate(rect.getCenter());
    //flatten it down a bit, perspectivy
	ofRotateX(angle);
    //translate to bottom left
	ofTranslate(-rect.width/2,rect.height/2);
	//ofTranslate(0,-50,-50);
	//flipping the y scale -- might mess with the numbers
    ofScale(1,-1);
	
	
    //outline of each channel
  	ofSetColor(colour);
    //ofRect(0,0,rect.width, rect.height);
	
  
	// outline blocks
	ofRectangle block;
	for(int i =0; i<numClips; i++) {
		block = getRectForClip(i);
		ofSetColor(colour, 150);
		ofRect(block);
		
	}

	
	ofFill();

  
    float thickness = 5;
 
	//ofSetLineWidth(4);
    
    if((ofGetElapsedTimeMillis()%300>150) ||(currentClip == motionBand) ) {
		ofSetColor(colour.r, colour.g, colour.b,50);

		block = getRectForClip(motionBand);
		
		//block.set(0,motionBand * (rect.height/numClips), rect.width, rect.height/numClips);
		if(currentClip!=motionBand) ofRect(block);
		
		/*
		ofPushMatrix();
		ofTranslate(block.x, block.y);
		ofRect(0, 0, thickness, block.height); // left
		ofRect(thickness, 0, block.width - (thickness*2), thickness); // top
		ofRect(block.width-thickness,0, thickness, block.height); // right
		ofRect(thickness, block.height - thickness, block.width - (thickness*2), thickness); // top
		ofPopMatrix();     // ---------------------------
		 */
		
		// draw outline block
		ofPushMatrix();     // --------------------------- 2
		ofTranslate(block.x, block.y);
		ofSetColor(colour.r, colour.g, colour.b);
		ofRect(0, 0, thickness, block.height); // left
		ofRect(thickness, 0, block.width - (thickness*2), thickness); // top
		ofRect(block.width-thickness,0, thickness, block.height); // right
		ofRect(thickness, block.height - thickness, block.width - (thickness*2), thickness); // top
		ofPopMatrix();     // --------------------------- 1

	
	
	}
	
    ofSetColor(colour);
	
	float progress = ableton.getProgressForTrack(abletonTrack);
	
	// draw currently playing rectangle
	block = getRectForClip(currentClip);
	
	
	ofPushMatrix();     // --------------------------- 2
	
	ofTranslate(block.x, block.y);
	
	// draw outline block
	//ofSetColor(colour.r, colour.g, colour.b);
	//ofRect(0, 0, thickness, block.height); // left
	//ofRect(thickness, 0, block.width - (thickness*2), thickness); // top
	//ofRect(block.width-thickness,0, thickness, block.height); // right
	//ofRect(thickness, block.height - thickness, block.width - (thickness*2), thickness); // top
	
	
	ofSetColor(colour.r, colour.g, colour.b,100);
	// draw filling up block
	//ofRect(thickness, thickness, block.width - (thickness*2), (block.height- (thickness*2)) * (1-progress)); // top
	ofRect(0, 0, block.width , (block.height));
	ofRect(0, 0, block.width , (block.height) * (1-progress));
	ofPopMatrix();     // --------------------------- 1

	ofSetColor(255,255,255);
	ofRect(0,(smoothedMotionLevel*(rect.height-5)), rect.width,10);

	
	// TRACK NUMBERS
	ofColor lighter = colour;
	lighter.setSaturation(100);
	
	for(int i =0; i<numClips; i++) {
		block = getRectForClip(i);
		//ofSetColor(colour, 150);
		//ofRect(block);
		
		ofPushMatrix();     // --------------------------- 2
		ofTranslate(block.getCenter());
		ofScale(0.8,(i==0)? -0.6 : -0.8);
		ofSetColor(lighter);
		string numstring = ofToString(i);
		float halfWidth = labelFont.stringWidth(numstring)/2;
		float halfHeight = labelFont.stringHeight(numstring)/2;
		labelFont.drawString(numstring,  - halfWidth, halfHeight);
		ofPopMatrix();     // --------------------------- 1
		
		
		
	}
	
	
	// LABELS
    //box for label at top
    ofSetColor(colour);
	if(!enabled) ofSetColor(colour*0.3);
    int boxWidth = area.width;
    int boxHeight = 60;
	float volume = ableton.getLevelForTrack(abletonTrack)*100 + ableton.getLevelForTrack(abletonTrack2)*100;
    ofTranslate(0,rect.height,0);
    ofScale(1,-1);

    ofRotateX(-angle);
    ofTranslate(0,-boxHeight);
    ofRect(0, 0, boxWidth, boxHeight);
	if(enabled) {
		ofSetColor(colour, 100);
		ofRect(0,0,boxWidth, -volume);
    }
	
	// label
    ofSetColor(255,255,255);
	if(!enabled) ofSetColor(0);

    string message = name;
    float halfWidth = labelFont.stringWidth(message)/2;
    float halfHeight = labelFont.stringHeight(message)/2;
    labelFont.drawString(message, (boxWidth/2 - halfWidth), (boxHeight/2)+halfHeight);
    
	ofPopMatrix();     // --------------------------- 0
	
	
	
	

	

	ofPopStyle();
	
	
}

ofRectangle AudienceSection::getRectForClip(int index) {
	
	float y;// = index * (area.height/numClips)
	float height;
	
	if(index == 0) {
		y = 0;
		height = (area.height/(numClips-0.5)) *0.5;
	} else  {
		y = (index-0.5) * (area.height*((numClips+0.5) / numClips )/numClips) ;
		height = (area.height/(numClips-0.5));
	}
	
	return ofRectangle(0,y, area.width, height);
	
	
}


void AudienceSection::updateMotionImage(ofImage &motionImage) {

	//copy(quads[i].begin(), quads[i].end(), back_inserter(warpPoints));
	unwarpPerspective(motionImage, unwarped, warpPoints);
	unwarped.update();

}

bool AudienceSection::toggleEnabled() {
	
	AbletonController& ableton = *AbletonController::instance();

	enabled = !enabled;
	if(!enabled) {
		ableton.sendMessage("/live/mute", abletonTrack, 1);
		if(abletonTrack2>=0) ableton.sendMessage("/live/mute", abletonTrack2, 1);
		reset();
		
		
		
	} else {
		ableton.sendMessage("/live/mute", abletonTrack, 0);
		if(abletonTrack2>=0) ableton.sendMessage("/live/mute", abletonTrack2, 0);
	
		
	}
	
	return enabled;

}

void AudienceSection::reset() {
	
	AbletonController& ableton = *AbletonController::instance();

	ableton.playClip(0,abletonTrack);
	if(abletonTrack2>=0) ableton.playClip(0, abletonTrack2);
	
	currentClip = 0;
	motionBand = 0;
	smoothedMotionLevel = 0;
	motionLevel = 0;


}