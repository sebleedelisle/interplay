#ifndef InstShape_h
#define InstShape_h

#include "ofMain.h"

class InstShape {
public:
    // Constructor
    InstShape();
    
    // Methods
    void init(int inst);
    void update();
    void render();
    
    void drawDrums();
    void drawBass();
    void drawSynth();
    void drawGuitar();
      
    
    // Properties
    ofVec3f pos;
    ofVec3f vel;
  
    
    int drag;
    int gravity;
    int alpha;
    int rotation;
    int size;
    int scale;
   
    ofColor colour;
   int instrument;
    
    
   
};
#endif