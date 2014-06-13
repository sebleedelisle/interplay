#include "shape.h"

Shape::Shape(){
    // Set the initial color
   
//    color.set( ofRandom(255), ofRandom(255), ofRandom(255));
    // Initial x position of the ball
    x = ofRandom( ofGetWindowWidth() );
    
    // Initial y position of the ball
    y = ofRandom( ofGetWindowHeight() );
}

void Shape::moveTo(){
    
}

void Shape::draw(){
    
    ofFill();
    ofCircle( x, y, 30);
}