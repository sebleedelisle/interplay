//
//  PipeOrganData.cpp
//  LaserShow
//
//  Created by Seb Lee-Delisle on 11/03/2014.
//
//

#include "PipeOrganData.h"

PipeOrganData::PipeOrganData() {


	editable = false; 

	// needs to be in laser space
	ofRectangle rect(556,631,170,190);
	int numPipes = 21;

	for (int i = 0; i<numPipes; i++) {
		
		pipes.push_back(Pipe(i));
		Pipe& pipe = pipes[pipes.size()-1];
		
		pipe.setTop(ofMap(i,0,numPipes, rect.getLeft(), rect.getRight()), rect.getTop());
		pipe.setBottom(pipe.top.x, rect.getBottom());
	
		params.add(pipe.topParam);
		params.add(pipe.bottomParam);
		
		
	}
	
//	gui.setup(params, "pipeOrganData.xml");
	
//	updatePipes();



};

void PipeOrganData::draw() {
	if(!editable) return; 
	
		
	for(int i = 0; i<pipes.size(); i++) {
		pipes[i].draw();
	
	}
	
	
	
	

}



void PipeOrganData::mouseDragged(int x, int y){
	if(!editable) return; 
	//for(int i = 0; i<pipes.size(); i++) {
    pipes[0].mouseDragged(x, y);
    pipes[10].mouseDragged(x, y);
    pipes.back().mouseDragged(x, y);
		
	//}
	updatePipes();
	
}

void PipeOrganData::mousePressed(int x, int y){
	if(!editable) return; 
	//for(int i = 0; i<pipes.size(); i++) {
		// only click the first one it finds
    pipes[0].mousePressed(x, y);
    pipes[10].mousePressed(x, y);
    pipes.back().mousePressed(x, y);
    updatePipes();
	//}
	
	
}

void PipeOrganData::mouseReleased(int x, int y){
	if(!editable) return; 
	//for(int i = 0; i<pipes.size(); i++) {
    pipes[0].mouseReleased(x, y);
    pipes[10].mouseReleased(x, y);
    
    pipes.back().mouseReleased(x, y);
    updatePipes();
	//}
	
	save();
	
}

void PipeOrganData::updatePipes() {
    
    Pipe & first = pipes[0];
    Pipe & mid = pipes[10];
    Pipe & last = pipes[20];
    for(int i = 1; i<10; i++) {
        Pipe& pipe = pipes[i];
        pipe.top = (mid.top - first.top) * ((float)i / 10.0f) + first.top;
        pipe.bottom = (mid.bottom - first.bottom) * ((float)i / 10.0f) + first.bottom;
        
    }
    for(int i = 11; i<20; i++) {
        Pipe& pipe = pipes[i];
        pipe.top = (last.top - mid.top) * ((float)(i-10) / 10.0f) + mid.top;
        pipe.bottom = (last.bottom - mid.bottom) * ((float)(i-10) / 10.0f) + mid.bottom;
        
    }
    
    
}


void PipeOrganData::save(){

	settings.serialize(params);
	settings.save("pipeOrganData.xml");
}



void PipeOrganData::load(){
	
	// how do we know if it loaded?
	if(settings.load("pipeOrganData.xml")) {
		settings.deserialize(params);
		for(int i = 0; i<pipes.size(); i++) {		
			pipes[i].copyDataFromParams();
		}
	}
	
}



