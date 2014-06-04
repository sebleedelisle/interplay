#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "ofxCv.h"

using namespace cv;
using namespace ofxCv;

/***************************************************************

 oscChatSystemExample

 - this app demonstrates using the ofxOsc addon to create a
 simple chat room
 - it sends chat messages to a server running in the same app
 - optionally, it can talk to another instance of this app on another IP.
 - get the IP address of the second computer
 - in ofApp.cpp inside the setup() function, change the value of
 clientDestination to the other IP as shown in the comment.

 ***************************************************************/

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void playClip(int num, int part); 

	
		//----------------------------------------
		// Client side:

		ofxOscSender clientSender; // all-important ofxOscSender object
		string clientDestination; // IP address we're sending to
		int clientSendPort; // port we're sending to
		
		ofxOscReceiver clientReceiver; // OSC receiver
		int clientRecvPort; // port where we listen for messages

        // Parse an OscMessage into a string for easy logging
		string getOscMsgAsString(ofxOscMessage m);
    
    
    ofVideoGrabber grabber;
    ofImage current;
    ofPixels previous;
    ofImage diff;
    cv::Scalar diffMean;
    float smoothedMean;
    int motionBand;
    int currentScene; 

    
    float masterMeter;
    vector<float> trackLevels;
    bool playing;

};
