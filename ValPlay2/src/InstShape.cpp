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
    
    
}

void InstShape::drawDrums() {
    ofSetColor(255,206,39);
    ofCircle(pos.x, pos.y, pos.z, size);
}


void InstShape::drawBass() {
    ofSetColor(124,213,248); //lt blue
    
    ofPushMatrix();
    ofTranslate(pos.x,pos.y,pos.z);
    ofRotate(rotation);
    ofScale(scale,scale);
    ofRect(0,0,0,size*1.25,size);
    ofPopMatrix();
}

void InstShape::drawSynth() {
    ofSetColor(35,121,172); //dk blue
    ofPushMatrix();
    ofTranslate(pos.x,pos.y,pos.z);
    ofRotate(rotation);
    ofScale(scale,scale);
    ofTriangle(0,0,0,10,5,10, 10,15,10);
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
    ofRect(0,0,0,10,10);
    ofPopMatrix();
}




