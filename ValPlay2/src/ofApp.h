#pragma once

#include "ofMain.h"
#include "InstShape.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        bool shouldRemove(InstShape p);
    
    
    
        void makeDrumShapes(int num, float volume);
        void makeBassShapes(int num, float volume);
        void makeSynthShapes(int num, float volume);
        void makeGuitarShapes(int num, float volume);
        vector<InstShape> drumShapes;
        vector<InstShape> bassShapes;
        vector<InstShape> synthShapes;
        vector<InstShape> guitarShapes;
    
        ofImage room;
      
		
};
