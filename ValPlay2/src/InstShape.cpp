#include "InstShape.h"

InstShape::InstShape(){
    init(1);
    
}

void InstShape::init(int inst) {
    instrument = inst;
    drag = 1;
    gravity = -0.9999;
    scale = 1;
    alpha = 1;
    rotation= 45;

}

void InstShape::update(){
    
    vel.y += gravity;
    vel * drag;
    pos += vel;
    
    if (rotation > 0 ){
        rotation++;
    }
}

void InstShape::render(){
	
	ofPushStyle();
    //for all shapes
    ofNoFill();
    ofSetLineWidth(1);
    
    switch (instrument) {
        
        case 1: //drums
        drawDrums();
        break;
            
        case 2: //bass
        drawBass();
        break;
            
        case 3: //synth
        drawSynth();
        
        break;
        
        case 4: //guitar
        drawGuitar();
        break;
            
        default: //drums
        
        break;
        
    };
    ofPopStyle();
    
}

void InstShape::drawDrums() {
    ofSetColor(255,206,39);
	ofPushMatrix();
	ofTranslate(pos.x, pos.y, pos.z);
    ofCircle(0,0,0, size);
	ofRotateX(90);
	ofCircle(0,0,0, size);
	ofRotateY(90);
	ofCircle(0,0,0, size);

	ofPopMatrix();
	
}


void InstShape::drawBass() {
    ofSetColor(124,213,248); //lt blue
    
    ofPushMatrix();
    ofTranslate(pos.x,pos.y,pos.z);
    ofRotate(rotation);
    ofScale(scale,scale);
    //ofRect(0,0,0,size*1.25,size);
    
    ofLine(-11,-6,-7,-12);
    ofLine(-7,-12,7,-12);
    ofLine(7,-12,11,-6);
    ofLine(11,-6,0,11);
    ofLine(0,11,-11,-6);
    
    ofLine(-11,-6,11,-6);
    
    ofLine(-7,-12,-3,-6);
    ofLine(-3,-6,0,-12);
    ofLine(0,-12,4,-6);
    ofLine(4,-6,7,-12);
    
    ofLine(-3,-6,0,11);
    ofLine(0,11,4,-6);
    
    ofPopMatrix();
}

void InstShape::drawSynth() {
    ofSetColor(35,121,172); //dk blue
    ofPushMatrix();
    ofTranslate(pos.x,pos.y,pos.z);
    ofRotate(rotation);
    ofScale(scale,scale);
    //ofTriangle(0,0,0,10,5,10, 10,15,10);
    
    
    ofLine(-2,-5,10,-3);
    ofLine(10,-3,-10,6);
    ofLine(-10,6,-2,-5);
    
    ofLine(-10,6,3,-4);

    ofPopMatrix();
}

void InstShape::drawGuitar() {
//    ofColor c;
//    c.setHsb(4,85,90);
//    ofSetColor(c);
    ofSetColor(229,149,35); //red
   
    ofPushMatrix();
    ofTranslate(pos.x,pos.y,pos.z);
    ofRotate(rotation);
    ofScale(scale,scale);
   
    ofLine(7,-8,7,-5);
    ofLine(7,-5,-7,-5);
    ofLine(-7,-5,-7,-2);
    ofLine(-7,-2,7,-2);
    ofLine(7,-2,7,0);
    ofLine(7,0,-7,0);
    ofLine(-7,0,-7,2);
    ofLine(-7,2,7,2);
    ofLine(7,2,7,4);
    ofLine(7,4,-7,4);
    ofLine(-7,4,-7,7);

    ofPopMatrix();
}




