#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    ofBackground(0,0,0);
    ofEnableSmoothing();
    
    room.loadImage("img/FPORoom.jpg");
//    room.resize(room.width / 1.5, room.height / 1.5);
    
}




//--------------------------------------------------------------
void ofApp::update(){
    theShape.x += ( mouseX - theShape.x )*0.1;
    theShape.y += ( mouseY - theShape.y )*0.1;
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
   
    ofSetColor(ofColor::yellow);
    
    //draw the FPO audience space image for kicks
    room.draw(0,0);
    
    theShape.draw();
   
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
