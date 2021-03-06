#include "ofApp.h"

using namespace ofxCv;
using namespace cv;



//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);
    
    labelFont.loadFont("Variable Black", 25, true, true, true);
    labelFont.setLetterSpacing(1);
    labelFont.setSpaceSize(.4);


	outputFbo.allocate(1280, 720, GL_RGB, 6);

    ableton.init("localhost");
    
	float imageScale = 1;
	
    grabber.setDeviceID(1);
    grabber.initGrabber(1280*imageScale, 720*imageScale, 30);
   
	//current.allocate(1280*imageScale, 720*imageScale, OF_IMAGE_COLOR);
	imitate(current, grabber);
	imitate(previous, current);
	imitate(diff, current);
	//diff.allocate(1280*imageScale, 720*imageScale, OF_IMAGE_GRAYSCALE);
	
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
    
    showTitle = false;
    
    int screenWidth = 1280;
    int screenHeight = 720;
	
	for(int i = 0; i<4; i++) {
		float x = ofMap(i, 0, 3, screenWidth*0.25, screenWidth*0.75);
		float y = ofGetHeight()/2;
		
		float halfwidth = (screenWidth*0.5/6)-4;
		float halfheight = 320;
		 
		 
		ofRectangle rect(x-halfwidth, y - (halfheight*1.5), halfwidth*2, halfheight*1);
		
		// warp points
		vector<ofPoint> points;
     // Settings for 50 degree angle
//		points.push_back(ofPoint(ofMap(i, 0, 3, screenWidth*0.24, screenWidth*0.63), screenHeight*0.40));
//		points.push_back(ofPoint(ofMap(i, 0, 3, screenWidth*0.37, screenWidth*0.76), screenHeight*0.40));
//		points.push_back(ofPoint(ofMap(i, 0, 3, screenWidth*0.15, screenWidth*1.2), screenHeight*0.87));
//		points.push_back(ofPoint(ofMap(i, 0, 3, screenWidth*-0.2, screenWidth*0.86), screenHeight*0.87));
        
        // Settings for 30 degree angle
        float topWidth = (float) screenWidth *  0.52 / 4.0;
        float topLeft = (float) screenWidth * 0.24;
        float botWidth = (float) screenWidth *  1.05 / 4.0;
        float botLeft = (screenWidth/2.0f) - (botWidth*4.0f/2.0f); //  (float) screenWidth*-0.2;
        
        points.push_back(ofPoint(ofMap(i, 0, 3, topLeft, topLeft + (topWidth*3)), screenHeight*0.32));
		points.push_back(ofPoint(ofMap(i, 0, 3, topLeft + topWidth, topLeft + (topWidth*4)), screenHeight*0.32));
		points.push_back(ofPoint(ofMap(i, 0, 3, botLeft + botWidth, botLeft + (botWidth*4)), screenHeight*0.87));
		points.push_back(ofPoint(ofMap(i, 0, 3, botLeft, botLeft + (botWidth*3)), screenHeight*0.87));
        
		for(int i = 0; i<points.size(); i++) points[i]*=imageScale;
		
		int secondTrack = -1;
		if(i==0) secondTrack = 4;
		else if(i==3) secondTrack = 5;
		audienceSections[i].init(i,rect, points, instrumentNames[i], fillColours[i], secondTrack);
		
	}
	
    //Load our images
    audiencePreview.loadImage("img/RoomPreview.png");
    titleScreen.loadImage("img/TitleScreen.jpg");
    bandLogo.loadImage("img/BandLogoSmall.png");
    
 
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
        if(motionThreshold>0)
            threshold(diff, motionThreshold);

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
		audienceSections[i].update(motionSensitivityPreset);
	}
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	outputFbo.begin();
	
	ofSetupScreenPerspective(1280,720,100);
    
	
	ofFill();
    ofBackground(0);
	
	ofPushMatrix();
	if(disableMirror) {
		ofTranslate(ofGetWidth(), 0);
		ofScale(-1,1);
	}
	
	ofSetColor(255 * camBrightness);
	current.draw(0,0, 1280, 720);
	
    ofSetColor(100);
	if(showPreview) audiencePreview.draw(0,0,1280,720);
   

	
	ofSetColor(255 * motionBrightness);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
    diff.draw(0,0, 1280, 720);
	ofSetColor(255);
    ofDisableBlendMode();
	ofPopMatrix(); 
	
    if(disableMirror) {
        ofDrawBitmapString("MIRROR DISABLED", 300,20);
        
        
    }
	
	for(int i = 0; i<audienceSections.size(); i++) {
		audienceSections[i].draw(appGui.getVisible());
	}
	
    //draw the band logo - need transparency here
    ofEnableAlphaBlending();
    bandLogo.draw((1280*0.5)-(bandLogo.width/2),20);
   
    // draw countdown bars
//    int countdown = 8- (ableton.currentBeatNum % 8);
//    
//    string numstring = "CHANGE IN " + ofToString(countdown);
//    if(countdown>=7) numstring = "CHANGE";
//    else if (countdown>4) numstring = "";
//    

    
//    float halfWidth = labelFont.stringWidth(numstring)/2;
//    float halfHeight = labelFont.stringHeight(numstring)/2;
//    labelFont.drawString(numstring, (1280/2) - halfWidth, halfHeight + 150);

    int countdown =  (ableton.currentBeatNum % 8);
    ofSetColor(220,140,255,200);
    ofRect(ofMap(countdown,0,8,350,930),150,(930-350)/8,20);
    ofNoFill();
    ofSetColor(255);
    ofRect(350,150,930-350,20);
    
	ableton.draw();
    
    // draw the title image on top if flagged
    if (showTitle){
        titleScreen.draw(0,0,1280,720);
    }
	outputFbo.end();
    
    
    float scale = (float)ofGetWidth()/1280.0f;
    float vertscale = (float)ofGetHeight()/720.0f;
    if( vertscale < scale) scale = vertscale;
    ofPushMatrix();
    ofScale(scale, scale);
	outputFbo.draw(0,0);
	ofPopMatrix();
    
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
	appGui.setVisible(false);
	
	for(int i = 0; i<audienceSections.size(); i++) {
	
		//appGui.add(audienceSections[i].enabled);
		appGui.add(audienceSections[i].motionLevelRaw);
		
		appGui.add(audienceSections[i].motionLevelMin);
		appGui.add(audienceSections[i].motionLevelMax);
	}
//	motionSensitivityPreset
	appGui.add(motionSensitivityPreset.set("motion change speed", 1,0,3));
	appGui.add(camBrightness.set("camera brightness", 1,0,1));
	appGui.add(motionBrightness.set("motion brightness", 1,0,1));
	appGui.add(motionThreshold.set("motion threshold", 0,0,255));
	
	appGui.add(disableMirror.set("disable mirror", false));
	appGui.add(showPreview.set("show preview", false));
	
	appGui.load();
	for(int i = 0; i<audienceSections.size(); i++) {
		audienceSections[i].enabled = true;
		audienceSections[i].motionLevelRaw = 0;
	}
	disableMirror = false;
    showPreview = false;
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
	
	if(key=='\t') {
		appGui.toggleVisible();
		
	}
    else if(key == '1') {
        audienceSections[0].toggleEnabled();
    } else if(key == '2') {
        audienceSections[1].toggleEnabled();
    } else if(key == '3') {
        audienceSections[2].toggleEnabled();    
    } else if(key == '4') {
        audienceSections[3].toggleEnabled();
    }
	
	if(key=='s') {
		current.saveImage(ofGetTimestampString("%Y%m%d-%H%M%S")+".png");
	}
    
    if (key=='t') {
       // Toggle the title image
        if (!showTitle){
            showTitle = true;
        }else {
            showTitle = false;
        }
    } else if (key=='m') {
        disableMirror = !disableMirror;
    }  else if (key=='p') {
        showPreview = !showPreview;
    }

	if(appGui.getVisible()) {
		if(key == 'm') { // set maximum
			for(int i = 0; i<audienceSections.size(); i++) {
				
				AudienceSection& section = audienceSections[i];
				section.motionLevelMax = section.motionLevelRaw*0.9;
				
			}
			
			
		} else if(key == 'n') { // set minimum
			for(int i = 0; i<audienceSections.size(); i++) {
				
				AudienceSection& section = audienceSections[i];
				section.motionLevelMin = section.motionLevelRaw*1.05;
				
			}
		
		} else if(key == 'r') { // reset all motion areas!
			
			for(int i = 0; i<audienceSections.size(); i++) {
				
				AudienceSection& section = audienceSections[i];
				section.motionLevelMin = 3;
				section.motionLevelMax = 10;
				
			}
			
			
		}
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

void ofApp::exit() {
    //current.saveImage(ofGetTimestampString("%Y%m%d-%H%M%S")+".png");
    ableton.exit();
	appGui.save();
    
    
}
