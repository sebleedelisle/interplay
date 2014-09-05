//
//  ColumnData.cpp
//  LaserLightSynths
//
//  Created by Seb Lee-Delisle on 04/09/2014.
//
//

#include "ColumnData.h"



void draw();



void ColumnData :: init() {
	
	
	params.add(pos.set("pos", ofPoint(640,480), ofPoint(0,0, -400), ofPoint(1280,960, 0)));
	params.add(scale.set("scale", 1, 0.1, 4));
	
	params.add(rotation.set("rot", ofPoint(30,0,0), ofPoint(0,-10,-10), ofPoint(50,10,10)));
	
	params.add(colSpacing.set("col spacing", 20, 1, 300));
	
	params.add(colRadius1.set("col radius1", 20, 1, 400));
	params.add(colRadius2.set("col radius2", 20, 1, 400));
	params.add(colHeight.set("col height", 20, 1, 600));
	
	params.add(colOffset.set("col offset", 20, 1, 200));
	
	
//	params.add(start.set("start", ofPoint(-500,300), ofPoint(-640,-480),ofPoint(640,480)));
//	params.add(c1.set("c1", ofPoint(-480, -50), ofPoint(-640,-480),ofPoint(640,480)));
//	params.add(c2.set("c2", ofPoint(-350,-220), ofPoint(-640,-480),ofPoint(640,480)));
//	params.add(end.set("end", ofPoint(0,-220), ofPoint(-640,-480),ofPoint(640,480)));
//	
//	params.add(level1.set("level1", ofPoint(-500,0), ofPoint(-640,-480),ofPoint(0,480)));
//	params.add(level2.set("level2", ofPoint(-500,100), ofPoint(-640,-480),ofPoint(0,480)));
//	params.add(level3.set("level3", ofPoint(-500,200), ofPoint(-640,-480),ofPoint(0,480)));
//	params.add(level4.set("level4", ofPoint(-500,300), ofPoint(-640,-480),ofPoint(0,480)));
//	
//	params.add(scaleXY.set("scaleXY", ofPoint(1,1,1), ofPoint(0.1,0.1,1),ofPoint(3,3,1)));
//	params.add(divAngle.set("division angle",12, 0, 30));
	
	gui.setup("Column Data", "columnData.xml");
	gui.setVisible(true);
	
	gui.add(params);
	gui.load();
	
//	level1Handle.setPoint(&level1, ofPoint(-940,-480));
//	level2Handle.setPoint(&level2, ofPoint(-940,-480));
//	level3Handle.setPoint(&level3, ofPoint(-940,-480));
//	level4Handle.setPoint(&level4, ofPoint(-940,-480));
//	startHandle.setPoint(&start, ofPoint(-940,-480));
//	startHandle.handleSize *=2;
//	endHandle.setPoint(&end, ofPoint(-940,-480));
//	endHandle.handleSize *=2;
//	c1Handle.setPoint(&c1, ofPoint(-940,-480));
//	c2Handle.setPoint(&c2, ofPoint(-940,-480));
	
	numCols = 4;
	for(int i = 0; i<numCols; i++) {
		colPositions.push_back(ofPoint());
		//ofCylinderPrimitive& cylinder =cylinders.back();
		//cylinder.set(colRadius, colHeight);
		
	}
	
	updatePoints();

	
}


void ColumnData :: draw() {
	editable = gui.getVisible();
	
	if(!editable) return;
	
	//level1.draw();
	
	
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(940,480);
	
	
	
	
	ofSetColor(0,255,255);
	
	ofTranslate(-300,0);
	
	ofPopMatrix();
	ofPushMatrix();
	
	ofTranslate(pos);
	
	ofRotateX(rotation->x);
	ofRotateY(rotation->y);
	ofRotateZ(rotation->z);
	ofScale(scale, scale, scale);
	
	
	ofSetColor(255, 100);
	ofSetLineWidth(1);
	//ofNoFill();
	
	updatePoints();

	for(int i = 0; i<colPositions.size(); i++) {
		ofPoint& colpos = colPositions[i];
		
		//col.setResolution(20, 1)	;
//		colpos.set((float(i) - ((float)colPositions.size()-1.0f)/2.0f)*colSpacing + ((i<(float)colPositions.size()/2.0f)? (-(float)colOffset) : (float)colOffset),0,0);
//
		
		for(float i = 0; i<=1.0001; i+=0.05) {
		
			float y = ofMap (i, 0, 1, colpos.y - colHeight/2, colpos.y + colHeight/2);
		
			ofPushMatrix();
			ofTranslate(colpos.x, y, colpos.z);
			ofRotateX(90);
			ofCircle(0,0,0, ofMap(i, 0,1 ,colRadius1, colRadius2));
			ofPopMatrix();
			
		}
		
		
		
		//col.draw();
		
		
	}

	
	
	
	ofPopStyle();
	ofPopMatrix();
	for(int i = 0; i<colPositions.size(); i++) {
		ofCircle(getColumnPosition(i, 1, (float)ofGetMouseY()/ (float)ofGetHeight()),5);
	}
	
//	level1Handle.render();
//	level2Handle.render();
//	level3Handle.render();
//	level4Handle.render();
//	startHandle.render();
//	c1Handle.render();
//	c2Handle.render();
//	endHandle.render();
	
/*
	ofSetLineWidth(3);
	ofSetColor(255,0,255);
	ofBezier(start, c1, c2, end);
	ofLine(start, c1);
	ofLine(end,c2);
	
	ofSetColor(0,255,255);
	
	ofTranslate(-300,0);
	
	ofPopMatrix();
	ofPushMatrix();
	
	ofTranslate(pos);
	
	ofRotateX(rotation->x);
	ofRotateY(rotation->y);
	ofRotateZ(rotation->z);
	ofScale(scale, scale, scale);
	
	
	ofSetColor(255, 100);
	ofSetLineWidth(0.5);
	
	ofPushMatrix();
	ofScale(scaleXY->x, scaleXY->y, scaleXY->z);
	for(float x = level4->x; x <= -level4->x; x+= level4->x / -5.0f) {
		ofLine(x, level4->y, x, end->y);
	}
	for(float y = level4->y; y >= end->y; y+= (end->y - level4->y)/10.0f) {
		
		ofLine(level4->x, y, -level4->x, y);
	}
	
	
	ofPopMatrix();
	
	ofSetColor(0,255,0);
	ofPoint scalarXY = scaleXY.get();
	
	for(float angle = 0; angle>= -90; angle-=divAngle) {
		ofPushMatrix();
		ofRotateY( angle);
		ofScale(scaleXY->x, scaleXY->y, scaleXY->z);
		ofSetLineWidth(0.5);
		
		ofBezier(start, c1, c2, end);
		ofSetLineWidth(3);
		
		ofCircle(level1, 1);
		ofCircle(level2, 1);
		ofCircle(level3, 1);
		ofCircle(level4, 1);
		
		ofPopMatrix();
		
		ofPushMatrix();
		ofRotateY( -180 - angle);
		ofScale(scaleXY->x, scaleXY->y, scaleXY->z);
		ofSetLineWidth(0.5);
		
		//ofBezier(start, c1, c2, end);
		ofSetLineWidth(3);
		
		ofCircle(level1, 1);
		ofCircle(level2, 1);
		ofCircle(level3, 1);
		ofCircle(level4, 1);
		
		ofPopMatrix();
		
	}
	
	ofPopStyle();
	ofPopMatrix();
	*/
	
	//gui.draw();
	
}
void ColumnData::mouseDragged(int x, int y){
	if(!editable) return;

}

ofPoint ColumnData::getColumnBasePos(int i) {
	
	//ofPoint colpos = colPositions[i];// + (ofVec3f)pos;
	//colpos.y += colHeight/2;
	//colpos.z -= colRadius2/2;
	//colpos.rotate(rotation->x, rotation->y, rotation->z);
	
	//colpos += (ofVec3f)pos;// + colPositions[i];
	
	return getColumnPosition(i, 1, 0.5);
	
}

ofPoint ColumnData:: getColumnPosition(int index, float heightUnit, float rotationUnit) {
	
	float radius = ofMap(heightUnit, 0, 1, colRadius1, colRadius2 );
	ofPoint colpos = ofPoint(-radius,0,0 );
	colpos.rotate(0, ofMap(rotationUnit, 0, 1, 0, 180),0);
	colpos.y += ofMap(heightUnit, 0, 1, -colHeight/2.0f, colHeight/2.0f);
	colpos += colPositions[index];
	colpos.rotate(rotation->x, rotation->y, rotation->z);
	
	colpos += (ofVec3f)pos;// + colPositions[i];
	return colpos;
	
	
}

void ColumnData::mousePressed(int x, int y){
	if(!editable) return;
	
	
}

void ColumnData::mouseReleased(int x, int y){
	if(!editable) return;
	
	
	save();
	
}


void ColumnData::save(){
	
	gui.save();
}



void ColumnData::load(){
	gui.load();
	
}

void ColumnData::updatePoints() {

	for(int i = 0 ; i< colPositions.size(); i++)  {
		ofPoint& colpos = colPositions[i];
		
		//col.setResolution(20, 1)	;
		colpos.set((float(i) - ((float)colPositions.size()-1.0f)/2.0f)*colSpacing + ((i<(float)colPositions.size()/2.0f)? (-(float)colOffset) : (float)colOffset),0,0);
		
	}
}