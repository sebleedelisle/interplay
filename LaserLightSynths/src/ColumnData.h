//
//  ColumnData.h
//  LaserLightSynths
//
//  Created by Seb Lee-Delisle on 04/09/2014.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "EditableBezier.h"
#include "DragHandle.h"


class ColumnData {
	
	
	public :
	void init();

	void draw();

	void save();
	void load();

	//ofPoint getBezierPoint(float t);

	void mouseDragged(int x, int y);
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);

	ofPoint getColumnPosition(int index, float heightUnit, float rotationUnit);
	ofPoint getColumnBasePos(int i);
	void updatePoints();

	
	ofParameterGroup params;
	ofXml settings;
	bool editable;

	ofParameter<ofPoint> pos;
	ofParameter<ofPoint> rotation;
	
	ofParameter<float> colRadius1;
	ofParameter<float> colRadius2;
	ofParameter<float> colHeight;
	ofParameter<float> colSpacing;
	ofParameter<float> colOffset;
	
	vector<ofPoint> colPositions;
	int numCols; 
	
	//ofParameter<ofPoint> start, end, c1, c2, scaleXY;
	
	
	ofParameter<float> scale;

	//ofParameter<ofVec3f> level1, level2, level3, level4;

	//DragHandle level1Handle, level2Handle, level3Handle, level4Handle,
	//startHandle, c1Handle, c2Handle, endHandle;

	//vector<ofPoint> getLevelPoints();

	//EditableBezier level1;


	ofxPanel gui;

};