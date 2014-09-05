//
//  LaserManager.h
//  LaserTest02
//
//  Created by Seb Lee-Delisle on 08/08/2013.
//
//

#pragma once

#include "ofxEtherdream.h"
#include <math.h>
#include "Quint.h"
#include "Quad.h"
#include "Cubic.h"
#include "CubicBezier.h"
#include "QuadWarp.h"
#include "ofxCv.h"
#include "LaserDot.h"
#include "LaserShape.h"
#include "LaserCircle.h"
#include "LaserSpiral.h"
#include "LaserPolyline.h"
#include "LaserLine.h"
#include "ofxGui.h"
#include "ofxSvg.h"
#include "ofMain.h"

#include "TextWriter.h"

// TODO : load / save settings

class LaserManager {

	public:
	
	static LaserManager * instance();
	static LaserManager * laserManager;

	
	LaserManager();
	
	void setup(int width, int height);
    void update();
	void draw();
	
	void renderPreview();
	void renderLaserPath(ofRectangle previewRectangle, bool overrideSettings);
	
	//void updatePreviewScreenRect(ofRectangle pscreenrect);

	bool my_compare( ofPoint a, ofPoint b){
		return a.y < b.y;
	}
	
	void addLaserDot(const ofPoint& ofpoint, ofFloatColor colour, float intensity =1);
	
	void addLaserCircle(const ofPoint& ofpoint, ofFloatColor colour, float radius, float intensity =1);
	
	void addLaserSpiral(const ofPoint& position, ofFloatColor col, float rad1,float rad2, float fadeoutpoint = 1,  float intens = 1);
	
	void addLaserPolyline(const ofPolyline& line, ColourSystem* coloursystem = NULL, float intens = 1);
	
	void addLaserSVG(ofxSVG & svg, ofPoint pos, ofPoint scale = ofPoint(1,1,1), float rotation  = 0, float intens =1 );

	void addLaserLineEased(const ofPoint&start, const ofPoint&end, ofFloatColor colour);
	
	void addLaserRectEased(const ofPoint&topLeft, const ofPoint&dimensions, ofFloatColor colour);

	void moveLaser(const ofPoint & currentPosition, const ofPoint & targetpoint);

	
	void addLaserText(string line, ofVec3f pos, float size, ofColor colour, bool centred);
	
	void drawShapes();
	
	void drawLaserLine(LaserLine& line);
	void drawLaserPolyline(LaserPolyline& laserPolyline, bool onlyUpdatePreviewMesh = false);
	void drawLaserCircle(LaserCircle& circle);
	void drawLaserDot(LaserDot& dot);
	void drawLaserSpiral(LaserSpiral& spiral);
	
	//void resetIldaPoints();
	void addIldaPoint(ofPoint p, ofFloatColor c, float pointIntensity = 1, bool useCalibration = true);
	
	void addPoint(ofPoint p, ofFloatColor c, bool useCalibration); 
	
	ofxIlda::Point ofPointToIldaPoint(const ofPoint& ofpoint, ofFloatColor colour);
	ofPoint ildaPointToOfPoint(const ofxIlda::Point& ildapoint);
	
	vector<float> getPointsAlongDistance(float distance, float acceleration, float maxspeed);
	
	//----------------------------------------
	// converts openGL coords to screen coords //
	ofVec3f gLProject(ofVec3f p) {
		return gLProject(p.x, p.y, p.z);
		
	}
	ofVec3f gLProject( float ax, float ay, float az ) {
		GLdouble model_view[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
		
		GLdouble projection[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
		
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		GLdouble X, Y, Z = 0;
		gluProject( ax, ay, az, model_view, projection, viewport, &X, &Y, &Z);
		
		//return ofVec3f(X, Y, 0.f);
		return ofVec3f(X, Y, 0.0f);
	}
	
	float gLGetScaleForZ(float z) {
		
		return gLProject(appWidth/2 + 1, appHeight/2, z).x - appWidth/2;
	}
	
	void connectButtonPressed();
	
	void connectToEtherdream();
	void disconnectFromEtherdream();
	bool toggleRegistration();

	//void addDelayTest() ;
	
	string ildaPointToString(ofxIlda::Point& p);
	
	void updateMaskRectangleParam(float& value);
	void updateMaskRectangle();
	
	float calculateCalibratedBrightness(float value, float intensity, float level100, float level75, float level50, float level25, float level0);
	
	vector <LaserShape*> shapes;
	// to store previous frames
	deque <vector <LaserShape*> > shapesHistory;
	deque <float> frameTimes;
	
	ofxEtherdream etherdream;
	
	ofFloatColor white;
	ofFloatColor black;
	
	ofSoundPlayer beep; 
	
	//ofPoint currentPosition;
	
	ofxButton connectButton;
	ofParameter<string> etherdreamStatus;
	ofParameter<int> pps; 
	ofParameter<bool> showWarpPoints;
	ofParameter<bool> flipX;
	ofParameter<bool> flipY;
	ofParameter<int> testPattern;
	int numTestPatterns; 
	//ofParameter<bool> showMaskRectangle;
	ofParameter<float> maskMarginBottom;
	ofParameter<float> maskMarginTop;
	ofParameter<float> maskMarginLeft;
	ofParameter<float> maskMarginRight;
	ofParameter<bool> useMaskBitmap;
	ofParameter<bool> showMaskBitmap;
	
	ofImage maskBitmap;
	
	
	//ofParameter<bool> showSyncTest;
	ofParameter<bool> showLaserPath;
	ofParameter<bool> renderLaserPreview;
	ofParameter<bool> showPostTransformPreview; 
	
	// overall brightness applied to any laser colour
	ofParameter<float> intensity;
	ofParameter<float> delay;
	// speaks for itself : 
	ofParameter<ofColor> colourCorrection;
	// allows you to shift the colour changes back or
	// forward to compensate for crap lasers
	ofParameter<int> colourChangeDelay;
	
	// how many blank points before and after drawing a dot
	ofParameter<int> shapePreBlank;
	ofParameter<int> shapePostBlank;
	// the number of points for a dot
	ofParameter<int> dotMaxPoints;
	
	// show the movement between shapes as a dotted red line
	ofParameter<bool> showMovePoints;
	
	// the speed for movement and acceleration
	ofParameter<float> moveSpeed;
	ofParameter<int> movePointsPadding;
	
	ofParameter<float> speedLine;
	ofParameter<float> accelerationLine;
	
	ofParameter<float> overlapCircle; 
	
	ofParameter<float> speedCircle;
	ofParameter<float> accelerationCircle;
	
	ofParameter<float> speedEasedLine;
	ofParameter<int> paddingEasedLine;
	
	ofParameter<float>spiralSpacing;
	
	
	
	ofParameter<float>red100;
	ofParameter<float>red75;
	ofParameter<float>red50;
	ofParameter<float>red25;
	ofParameter<float>red0;

	ofParameter<float>green100;
	ofParameter<float>green75;
	ofParameter<float>green50;
	ofParameter<float>green25;
	ofParameter<float>green0;
	
	ofParameter<float>blue100;
	ofParameter<float>blue75;
	ofParameter<float>blue50;
	ofParameter<float>blue25;
	ofParameter<float>blue0;

	bool showColourPoints = false;
	
	
	float maskRectangleBrightness; 

	ofParameterGroup redParams, greenParams, blueParams;
	ofParameterGroup parameters;
    ofParameterGroup homographyParameters;

    QuadWarp warp;
	
	TextWriter textWriter;
	ofMesh laserWordMesh;
	
	bool shouldBeConnected;
	string lastState; 
	
	ofRectangle maskRectangle;
	
	//ofMesh previewMesh;
	ofMesh pathMesh;
	ofPoint pmin;
	ofPoint pmax;
	
	int minPoints;
	int restartCount; 
	
	//bool laserDirty;
	
	vector<ofxIlda::Point> ildaPoints;
	//vector<ofPoint> ofPoints;
	
	float appWidth;
	float appHeight;
	
	ofImage pointPreviewImage;
	int previewPointIndex = 0; 
	
	bool offScreen;
	ofVec2f offScreenPoint;
	ofVec2f lastClampedOffScreenPoint;
	
	


};