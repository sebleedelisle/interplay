#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);

    ofBackground(0,0,0);
    ofEnableSmoothing();
    
    room.loadImage("img/FPORoom.jpg");
//    room.resize(room.width / 1.5, room.height / 1.5);
    
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(36);
    
    
}

//need to do something to take out old ones, but this doesn't make sense to me right now. thinking :)
// write our boolean remove function
//bool shouldRemove(InstShape &p){
//    if(p.pos.y > ofGetHeight() )return true;
//    else return false;
//}



//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < drumShapes.size(); i++){
        drumShapes[i].update();
    }
    
    for(int i = 0; i < bassShapes.size(); i++) {
        bassShapes[i].update();
    }
    
    for(int i = 0; i < synthShapes.size(); i++) {
        synthShapes[i].update();
    }
    
    for(int i = 0; i < guitarShapes.size(); i++) {
        guitarShapes[i].update();
    }
 
    //ofRemove(guitarShapes,shouldRemove);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //draw the FPO audience space image for kicks
    room.draw(0,0);
    
    /*http://www.openframeworks.cc/tutorials/c++%20concepts/001_stl_vectors_basic.html */
    for(int i = 0; i < drumShapes.size(); i++){
        drumShapes[i].render();
    }
    
     for(int i = 0; i < bassShapes.size(); i++) {
        bassShapes[i].render();
    }
    
     for(int i = 0; i < synthShapes.size(); i++) {
        synthShapes[i].render();
    }
    
   for(int i = 0; i < guitarShapes.size(); i++) {
        guitarShapes[i].render();
    }

   
}

//--------------------------------------------------------------
void ofApp::makeDrumShapes(int num, float volume) {
    for ( int i =0; i < num; i++){
        InstShape d;
        d.init(1);
        float angle = PI + (ofRandom(-PI, PI));
        float speed = ofRandom(1,ofMap(volume, 0,100, 1,12));
        d.vel.set(sin(angle)*speed, cos(angle)*speed, speed);
        d.size = ofRandom(1, 12);
        d.pos.set(416,286, ofRandom(100));
        drumShapes.push_back(d);
    };
}


//--------------------------------------------------------------
void ofApp::makeBassShapes(int num, float volume) {
    for ( int i =0; i < num; i++){
        InstShape bass;
        bass.init(2);
        float angle = PI + (ofRandom(-PI, PI));
        float speed = ofRandom(1,ofMap(volume, 0,100, 1,12));
        bass.vel.set(sin(angle)*speed, cos(angle)*speed, speed);
        bass.size = ofRandom(1, 12);
        bass.rotation = ofRandom(0,90);
        bass.pos.set(568,286, ofRandom(100));
        bassShapes.push_back(bass);
    };
}


//--------------------------------------------------------------
void ofApp::makeSynthShapes(int num, float volume) {
    for ( int i =0; i < num; i++){
        InstShape synth;
        synth.init(3);
        float angle = PI + (ofRandom(-PI, PI));
        float speed = ofRandom(1,ofMap(volume, 0,100, 1,12));
        synth.vel.set(sin(angle)*speed, cos(angle)*speed, speed);
        synth.rotation = ofRandom(0,90);
        synth.scale = ofRandom(1.5,2.5);
        synth.pos.set(756,286, ofRandom(100));
        synthShapes.push_back(synth);
    };
}


//--------------------------------------------------------------
void ofApp::makeGuitarShapes(int num, float volume) {
    for ( int i =0; i < num; i++){
        InstShape guit;
        guit.init(4);
        float angle = PI + (ofRandom(-PI, PI));
        float speed = ofRandom(1,ofMap(volume, 0,100, 1,12));
        guit.vel.set(sin(angle)*speed, cos(angle)*speed, speed);
        guit.rotation = ofRandom(0,90);
        guit.size = ofRandom(1, 12);
        guit.pos.set(918,286, ofRandom(100));
        guitarShapes.push_back(guit);
    };
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a') {
        makeDrumShapes(1, 80);
    };
    
    if (key == 's') {
        makeBassShapes(1, 80);
    };
    
    if (key == 'd') {
        makeSynthShapes(1, 80);
    };
    
    if (key == 'f') {
        makeGuitarShapes(1, 80);
    };
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
