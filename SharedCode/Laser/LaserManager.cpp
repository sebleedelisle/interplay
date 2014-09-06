//
//  LaserManager.cpp
//  LaserTest02
//
//  Created by Seb Lee-Delisle on 08/08/2013.
//
//

#include "LaserManager.h"


LaserManager * LaserManager :: laserManager = NULL;

LaserManager * LaserManager::instance() {
	if(laserManager == NULL) {
		laserManager = new LaserManager();
	}
	return laserManager;
}


LaserManager:: LaserManager() {
	pointPreviewImage.loadImage("img/LaserPointPreview.png");

	//etherdream.setWaitBeforeSend(true);
	
	beep.loadSound("beep.aif");
	beep.setVolume(0.01);
	lastState = "";
	textWriter.colourFlickerMin = 1;
	textWriter.lineRandomness = 0; 
}


void LaserManager:: connectToEtherdream() {
	
	etherdream.setup();
	shouldBeConnected = true;
	
}
void LaserManager:: disconnectFromEtherdream() {
	
	etherdream.kill();
	shouldBeConnected = false;
	
}

void LaserManager:: setup (int width, int height) {
    
    appWidth = width; // ofGetWidth();
    appHeight = height;//ofGetHeight();
    
    shouldBeConnected = false;
	//showSyncTest = false;
	//connectButton = false;
	
	minPoints = 100;
    
	restartCount = 0;
	
	white.set(1,1,1);
	black.set(0,0,0);
	
	//endCount = 1;
	//blankCount = 0;
	
	pmin.set(0,0);
	pmax.set(appWidth, appHeight);
	
	testPattern = 0;
	numTestPatterns = 5;
	showMovePoints = false;
	showLaserPath = true;
	intensity = 1;
	
    float x1 = appWidth * 0.2;
    float x2 = appWidth * 0.8;
    float y1 = appHeight * 0.2;
    float y2 = appHeight * 0.8;

    warp.label = "laserWarp";
	warp.setDstPoint(0, ofVec2f(x1,y1));
	warp.setDstPoint(1, ofVec2f(x2,y1));
	warp.setDstPoint(2, ofVec2f(x2,y2));
	warp.setDstPoint(3, ofVec2f(x1,y2));
	
	warp.setSrcPoint(0, ofVec2f(0,0));
	warp.setSrcPoint(1, ofVec2f(appWidth,0));
	warp.setSrcPoint(2, ofVec2f(appWidth,appHeight));
	warp.setSrcPoint(3, ofVec2f(0,appHeight));
    
    warp.loadSettings();
    
	intensity = 1;

	parameters.setName("Laser Manager");
	
	connectButton.setup("Etherdream connect");
	connectButton.addListener(this, &LaserManager::connectButtonPressed);
	//parameters.add(&connectButton);
	parameters.add(etherdreamStatus.set("", "test"));
	
	parameters.add(intensity.set("intensity", 1, 0, 1));

	parameters.add(testPattern.set("test pattern", 0, 0, numTestPatterns));
	parameters.add(delay.set("sync delay", 0, 0, 0.4));
	parameters.add(pps.set("points per second", 80000, 30000, 100000));

	
	parameters.add(maskMarginTop.set("mask margin top", 0, 0, appHeight));
	parameters.add(maskMarginBottom.set("mask margin bottom", 0, 0, appHeight));
	parameters.add(maskMarginLeft.set("mask margin left", 0, 0, appWidth));
	parameters.add(maskMarginRight.set("mask margin right", 0, 0, appWidth));
	parameters.add(useMaskBitmap.set("use mask bitmap", true));
	parameters.add(showMaskBitmap.set("show mask bitmap", true));
	
	parameters.add(showWarpPoints.set("show warp points", false));


	//parameters.add(colourCorrection.set("colour correction", ofColor(255,255,255),ofColor(200,200,200), ofColor(255,255,255)));
	parameters.add(colourChangeDelay.set("colour change offset", -6, -15, 15));
	
	parameters.add(flipX.set("flip x", true));
	parameters.add(flipY.set("flip y", true));
	
	parameters.add(showMovePoints.set("laser on during move", false));

	parameters.add(renderLaserPreview.set("preview laser", true));

	parameters.add(showLaserPath.set("preview laser dots", true));
	parameters.add(showPostTransformPreview.set("post transform dots", false));

	
	parameters.add(moveSpeed.set("move speed", 3,2,20));
	parameters.add(movePointsPadding.set("move points padding", 1,0,20));
	
	parameters.add(shapePreBlank.set("shape pre blank points", 1, 0, 20));
	parameters.add(shapePostBlank.set("shape post blank points", 1, 0, 20));
	
	parameters.add(dotMaxPoints.set("dot max points", 7, 0, 100));
	
	parameters.add(accelerationLine.set("line acceleration", 0.5, 0.01, 4));
	parameters.add(speedLine.set("line speed", 20,2, 40));
	parameters.add(overlapCircle.set("circle overlap", 20,0, 100));
	parameters.add(accelerationCircle.set("circle acceleration", 0.5, 0.01, 4));
	parameters.add(speedCircle.set("circle speed", 20,2, 40));

	parameters.add(speedEasedLine.set("eased line speed", 8, 2, 20));
	parameters.add(paddingEasedLine.set("eased line padding", 1,0, 20));
  	parameters.add(spiralSpacing.set("spiral spacing", 10,1, 20));
    
    // where is this getting set up? 
	homographyParameters.setName("Laser Homography");
	
	
	redParams.add(red100.set("red 100", 1,0,1));
	redParams.add(red75.set("red 75", 0.75,0,1));
	redParams.add(red50.set("red 50", 0.5,0,1));
	redParams.add(red25.set("red 25", 0.25,0,1));
	redParams.add(red0.set("red 0", 0,0,0.5));
	
	greenParams.add(green100.set("green 100", 1,0,1));
	greenParams.add(green75.set("green 75", 0.75,0,1));
	greenParams.add(green50.set("green 50", 0.5,0,1));
	greenParams.add(green25.set("green 25", 0.25,0,1));
	greenParams.add(green0.set("green 0", 0,0,0.5));
	
	blueParams.add(blue100.set("blue 100", 1,0,1));
	blueParams.add(blue75.set("blue 75", 0.75,0,1));
	blueParams.add(blue50.set("blue 50", 0.5,0,1));
	blueParams.add(blue25.set("blue 25", 0.25,0,1));
	blueParams.add(blue0.set("blue 0", 0,0,0.5));
	
	redParams.setName("Laser red calibration");
	greenParams.setName("Laser green calibration");
	blueParams.setName("Laser blue calibration");
	
	colourChangeDelay = -6;
	
	warp.visible = true;
    
	warp.dragSpeed = 1;
	
	maskMarginTop.addListener(this, &LaserManager::updateMaskRectangleParam);
	maskMarginBottom.addListener(this, &LaserManager::updateMaskRectangleParam);
	maskMarginLeft.addListener(this, &LaserManager::updateMaskRectangleParam);
	maskMarginRight.addListener(this, &LaserManager::updateMaskRectangleParam);
	
	updateMaskRectangle();
	if(!maskBitmap.loadImage("img/LaserMask.png")) {
		maskBitmap.allocate(appWidth, appHeight, OF_IMAGE_COLOR); 
	};
}

void LaserManager :: updateMaskRectangle() {
	maskRectangle.x = maskMarginLeft;
	maskRectangle.y = maskMarginTop;
	maskRectangle.width = appWidth-maskMarginRight - maskMarginLeft;
	maskRectangle.height = appHeight - maskMarginBottom- maskMarginTop;
	maskRectangleBrightness = 1;
	
}
void LaserManager :: updateMaskRectangleParam(float& value) {
	updateMaskRectangle();
	
}


void LaserManager:: connectButtonPressed(){
	
	//if(connectButton!=isConnected) {
		if(!shouldBeConnected) {
			connectToEtherdream();
		} else {
			disconnectFromEtherdream();
		}
	//}

	
}


void LaserManager:: update() {
	

	//resetIldaPoints();
	
	// clear all pre-existing points.
	ildaPoints.clear();
	// clear the pathMesh - the blue and white laser preview
	pathMesh.clear();
	
	
	string etherdreamstate = etherdream.getStateString();
	
	etherdreamStatus = etherdream.getStateString();//etherdream.getDeviceStateString() + " " + (etherdreamstate) + " "+ ofToString(shouldBeConnected)+" " +ofToString(restartCount);
		
	if(etherdream.state==ETHERDREAM_DISCONNECTED){
	
		restartCount++;
		disconnectFromEtherdream();
		beep.play();
		connectToEtherdream();
	}
	
			/*
	if(showSyncTest) {
		addDelayTest();
	}*/
	
	
	
	//addLaserSpiral(ofPoint(200,200), ofFloatColor::white, 100, 200);
	

	
		
	// at this point we should report if the laser has disconnected.
	
	
	lastState = etherdreamstate;
	
}

void LaserManager::draw() {

	ofPoint startPosition = maskRectangle.getCenter();

//	if(testPattern>0) {
//	
//	}
//	
	if(testPattern==1) {
		
		//addLaserRectEased(pmin, pmax, white);
		//addLaserLineEased(maskRectangle.getTopLeft(), maskRectangle.getBottomRight(), white);
		//addLaserLineEased(maskRectangle.getTopRight(), maskRectangle.getBottomLeft(), white);
		
		ofPoint v = maskRectangle.getBottomRight() - maskRectangle.getTopLeft();
		
		for(float x =0 ; x<=1; x+=0.2) {
			
			for(float y = 0; y<=1; y+=0.2) {
				//addLaserDot(ofPoint(maskRectangle.x + (v.x*x), maskRectangle.y + (v.y*y)), white, 1);
				
				if(x ==0) {
					addLaserLineEased(ofPoint(maskRectangle.getLeft(), maskRectangle.getTop()+v.y*y),ofPoint(maskRectangle.getRight(), maskRectangle.getTop()+v.y*y), ofColor::white );
				}
			}
			
			addLaserLineEased(ofPoint(maskRectangle.x + v.x*x, maskRectangle.getTop()),ofPoint(maskRectangle.x + v.x*x, maskRectangle.getBottom()), ofColor::red );
			
		}
		
		addLaserCircle(maskRectangle.getCenter(), white, 10);
		addLaserCircle(maskRectangle.getCenter(), ofFloatColor(1,0,0), 50);
		
		
		/*
		 addLaserDot(pmin, white, 1);
		 addLaserDot(ofPoint(pmax.x, pmin.y), white, 1);
		 addLaserDot(pmax, white, 1);
		 addLaserDot(ofPoint(pmin.x, pmax.y), white, 1);
		 */
		
		
	} else if((testPattern>=2) && (testPattern<=5)) {
		ofColor c;
		
		ofRectangle rect(appWidth*0.3, appHeight*0.5, appWidth*0.4, appHeight*0.1);
		
		moveLaser(startPosition,rect.getTopLeft());

		for(int row = 0; row<5; row ++ ) {
			
			
			float y = rect.getTop() + (rect.getHeight()*row/4);
			
			ofPoint left = ofPoint(rect.getLeft(), y);
			
			ofPoint right = ofPoint(rect.getRight(), y);
			
			
			//moveLaser(left);
			
			for(int i = 0; i<shapePreBlank; i++) {
				addIldaPoint(left, black, 1);
			}
			
			if(testPattern == 2) c.set(255,0,0);
			else if(testPattern == 3) c.set(0,255,0);
			else if(testPattern == 4) c.set(0,0,255);
			else if(testPattern == 5) c.set(255,255,255);

			
			
			
			
			switch (row) {
				case 0 :
					c.r *= red100;
					c.g *= green100;
					c.b *= blue100;
					break;
				case 1 :
					c.r *= red75;
					c.g *= green75;
					c.b *= blue75;
					break;
				case 2 :
					c.r *= red50;
					c.g *= green50;
					c.b *= blue50;
					break;
				case 3 :
					c.r *= red25;
					c.g *= green25;
					c.b *= blue25;
					break;
				case 4 :
					c.r *= red0;
					c.g *= green0;
					c.b *= blue0;
					break;
			}
			
			// TODO - this should just be a fix pre and post blank.
			for(int i = 0; i<shapePreBlank; i++) {
				addIldaPoint(left, c, 1, true);
			}
			
			float speed = 20 * ( 1- (row*0.25));
			if(speed<5) speed = 5;
			
			for(float x = rect.getLeft(); x<=rect.getRight(); x+=speed) {
				addIldaPoint(ofPoint(x,y),c,1, false); 
				
			}
			
			for(int i = 0; i<shapePostBlank; i++) {
				addIldaPoint(right, c, 1, false);
			}

			for(int i = 0; i<shapePostBlank; i++) {
				addIldaPoint(right, black, 1);
			}

		
			// 0 = normalspeed;
			// 1 = normalspeed * 0.75
			// 2 = normalspeed * 0.5
			// 3 = normalspeed * 0.25

			
			// 0 = 1;
			// 1 = 0.75
			// 2 = 0.5;
			// 3 = 0.25
			//float brightness = 1 - (row*0.25);
			
			//if(brightness < 0) brightness =
	
		}
		moveLaser(rect.getBottomRight(), startPosition);
		
	}

	
	// if we're using the clever laser render delay
	// system
	
	if(delay > 0) {
		float currentTime = ofGetElapsedTimef(); 
		// cout << "------------pushing history " << shapes.size() << endl;
		// add the current shapes and time to the
		// histories
		
		shapesHistory.push_back(shapes);
		frameTimes.push_back(currentTime);
		
		//start from the oldest shapes and while the next set of
		// shapes are due, delete the oldest
		int numDeleted = 0; 
		while((frameTimes.size()>1) && (frameTimes[1]+delay < currentTime)) {
				
			shapes = shapesHistory.front();
			
			// DELETE ALL SHAPES IN HISTORY.FRONT
			for(int i = 0; i<shapes.size(); i++) {
				delete shapes[i];
			}

			shapesHistory.pop_front();
			frameTimes.pop_front();

			//cout << "deleting oldest " << endl;
			numDeleted++; 
			
		}
		shapes = shapesHistory.front();
		if(numDeleted == 0 ) {
			//cout << "NONE DELETED" << endl;
			
		}

		
	} else if(shapesHistory.size()!=0) {
		// TODO need to also delete shapes otherwise memory leak
		for(int i = 0; i<shapesHistory.size(); i++) {
			
			for(int j = 0; j<shapesHistory[i].size(); j++) {
				delete shapesHistory[i][j];
			}
		}
		
		shapesHistory.clear();
		frameTimes.clear();
	}
	
	
	drawShapes();
	
	if(renderLaserPreview) {
		
		renderPreview();
		
		//ofRect(0,0,100,100);
		
	}
	
	// TODO FIX
	while(ildaPoints.size()<minPoints) {
		LaserCircle blank(startPosition + ofPoint(0,10), black, 10);
//		addIldaPoint(startPosition, black);
		drawLaserCircle(blank);
	}
	
	vector<ofxIlda::Point> adjustedPoints;

	// SORT OUT COLOUR CHANGE DELAY.
	
	for(int i = 0; i<ildaPoints.size(); i++) {
		
		ofxIlda::Point p = ildaPoints.at(i);
		
		int colourPointIndex = i+colourChangeDelay;
		while(colourPointIndex<0) colourPointIndex+=ildaPoints.size();
		
		ofxIlda::Point colourPoint = ildaPoints.at(colourPointIndex%ildaPoints.size());
		
		p.r = round(colourPoint.r);
		p.g = round(colourPoint.g);
		p.b = round(colourPoint.b);
		
		adjustedPoints.push_back(p);
		
	}
	
	etherdream.setPoints(adjustedPoints);
	etherdream.setPPS(pps);
	

	ofPushStyle();
	
	if(maskRectangleBrightness>0) {
		ofNoFill();
		ofSetColor(maskRectangleBrightness * 255);
		
		ofRect(maskRectangle);
		maskRectangleBrightness-=0.01;
		
	}
	
	if(showMaskBitmap) {
		ofPushStyle();
//		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		ofSetColor(50,0,0);
		maskBitmap.draw(0,0,appWidth, appHeight);
		ofPopStyle(); 
	}
	
	
	ofPopStyle();
	
	// TODO if we're not using the delay system, let's
	// delete all the shapes
	if(delay==0) {
		for(int i = 0; i<shapes.size(); i++) {
			delete shapes[i];
		}
		
	}
	
	// clear the shapes vector no matter what
	shapes.clear();
	
	
}

string LaserManager::ildaPointToString(ofxIlda::Point& p) {
	return(ofToString(p.x) + ","+ofToString(p.y) +","+ofToString(p.r) +","+ofToString(p.g) +","+ofToString(p.b));
}


void LaserManager::renderLaserPath(ofRectangle previewRectangle, bool overrideSettings) {
	
	ofPushStyle();
	
	
	if((showLaserPath)||(overrideSettings)) {
		
		ofPushMatrix();
		
		ofTranslate(previewRectangle.getTopLeft());
		float scale =  previewRectangle.width / appWidth;
		ofScale(scale, scale);
		
		//ofDisableBlendMode();
		ofNoFill();
		ofSetLineWidth(1);
		ofSetColor(0,0,255);
		pathMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
		pathMesh.draw();
		
		
		ofSetColor(255,255,255);
		pathMesh.setMode(OF_PRIMITIVE_POINTS);
		pathMesh.draw();
		ofPopMatrix();
		
		if(ildaPoints.size()>0) {
			//int pointindex = previewPointIndex % ildaPoints.size();//
			//previewPointIndex++;
			
			int pointindex =floor(ofMap(ofGetMouseX(), previewRectangle.x, previewRectangle.getRight(), 0, (int)ildaPoints.size(), true));
			
			if(pointindex>=ildaPoints.size()) pointindex =ildaPoints.size();
			
			ofPoint p = ildaPointToOfPoint(ildaPoints[pointindex]);
			ofSetColor(0,255,0); 
			ofCircle(ofMap(p.x, 0, appWidth, previewRectangle.x, previewRectangle.getRight()), ofMap(p.y, 0, appHeight, previewRectangle.y, previewRectangle.getBottom()), 5);

		}
	}
	
	// TODO - this needs to go somewhere else!
	warp.visible = showWarpPoints || (overrideSettings);
	warp.draw();

	ofPopStyle();

}


void LaserManager::addLaserDot(const ofPoint& ofpoint, ofFloatColor colour, float intensity){
	
	//ofPoint target = warp.getWarpedPoint(ofpoint);
	
	shapes.push_back(new LaserDot(gLProject(ofpoint), colour, intensity));
}



void LaserManager:: addLaserLineEased(const ofPoint&startpoint, const ofPoint&endpoint, ofFloatColor colour) {
	shapes.push_back(new LaserLine(gLProject(startpoint), gLProject(endpoint), colour, 1));
}


void LaserManager::addLaserCircle(const ofPoint& ofpoint, ofFloatColor colour, float radius, float intensity){
	
	ofPoint p = gLProject(ofpoint);
	float scalar = gLGetScaleForZ(ofpoint.z);
	
	shapes.push_back(new LaserCircle(p, colour, radius*scalar , intensity, overlapCircle));
}

void LaserManager::addLaserSpiral(const ofPoint& position, ofFloatColor col, float rad1,float rad2, float fadeoutpoint,  float intens){
	
	ofPoint p = gLProject(position); 
	float scalar = gLGetScaleForZ(position.z);
	
	shapes.push_back(new LaserSpiral(p, col, rad1 * scalar, rad2 * scalar,  spiralSpacing, fadeoutpoint, intens));
	
}

void LaserManager::addLaserPolyline(const ofPolyline& line, ColourSystem* coloursystem, float intens){
	
	if((line.getVertices().size()==0)||(line.getPerimeter()<0.1)) return;
	
	// convert to 2D
	ofPolyline polyline;
	for(int i = 0; i<line.getVertices().size(); i++) {
		polyline.addVertex(gLProject(line.getVertices()[i])); 
		
	}
	
	shapes.push_back(new LaserPolyline(polyline, coloursystem, intens));
	
	
}

void LaserManager :: addLaserSVG(ofxSVG & svg, ofPoint pos, ofPoint scale, float rotation, float intens ) {
	
	ofVec3f centrePoint = ofVec3f(svg.getWidth()/2, svg.getHeight()/2);
	cout << "addLaserSVG  paths = " <<svg.getNumPath() << endl;
	for(int i=0; i<svg.getNumPath(); i++ ) {
		
		vector<ofPolyline>& lines = svg.getPathAt(i).getOutline();
		ofColor col = svg.getPathAt(i).getStrokeColor();
		//cout << "  col = " <<col << endl;
		
		for(int j=0; j<lines.size(); j++) {
			ofPolyline line = lines[j];
			
			vector<ofVec3f>& vertices = line.getVertices();
			for(int i = 0; i<vertices.size(); i++) {
				ofVec3f& v = vertices[i];
				v-=centrePoint;
				v.rotate(rotation, ofPoint(0,0,1)); 
				v*=scale;
				//v.x *= scaleX;
				//v.x+=width/2;
				//v.y+=height/2;
				
				//v.x+=APP_WIDTH/2;
				//v.y +=APP_HEIGHT*0.3;
				v+=pos;
				//line.
				
			}
			line.simplify(0.1);
			//cout << "brightness : " << brightness << endl;
			addLaserPolyline(line,new ColourSystem(col),1);
		}
	}
	
}

void LaserManager:: drawShapes() {
	
	
	// sort the dots by nearest neighbour
	
	if(shapes.size()==0) return; 
	vector<LaserShape*> sortedShapes;
	
	for(int i =0; i<shapes.size(); i++ ) {
		shapes[i]->tested = false;
		shapes[i]->reversed = false;
		
	}
	
	int numberSorted = 0;
	int currentIndex = 0;
	
	int nextDotIndex = NULL;
	
	// SORT THE SHAPES
	
	bool reversed = false;
	
	do {
		
		
		LaserShape * shape1 = shapes.at(currentIndex);
		
		//LaserDot & nextDot = dots.at(nextDotIndex);
		shape1->tested = true;
		sortedShapes.push_back(shape1);
		shape1->reversed = reversed; 
		
		float shortestDistance = INFINITY;
		nextDotIndex = -1;
		
		
		for(int j = 0; j<shapes.size(); j++) {
			
			LaserShape * shape2 = shapes.at(j);
			if((shape1==shape2) || (shape2->tested)) continue;
		
			if(shape1->getEndPos().distanceSquared(shape2->getStartPos()) < shortestDistance) {
				shortestDistance = shape1->getEndPos().distanceSquared(shape2->getStartPos());
				nextDotIndex = j;
				reversed = false;
			}
			
			if((shape2->reversable) && (shape1->getEndPos().distanceSquared(shape2->getEndPos()) < shortestDistance)) {
				shortestDistance = shape1->getEndPos().distanceSquared(shape2->getEndPos());
				nextDotIndex = j;
				reversed = true;
			}
			
			
		}
				
		currentIndex = nextDotIndex;
		
		
		
	} while (currentIndex>-1);
	
	offScreen = false;
	
	ofPoint startPosition = maskRectangle.getCenter(); //sortedShapes[0]->getStartPos();
	ofPoint currentPosition = startPosition;
	
	addIldaPoint(currentPosition, ofColor::black);
	
	for(int i = 0; i<sortedShapes.size(); i++) {
		
		LaserShape* shape = sortedShapes.at(i);

		if(!currentPosition.match(shape->getStartPos(), 0.01)) {
			moveLaser(currentPosition, shape->getStartPos());
			
			// PRE BLANK
			for(int i = 0; i<shapePreBlank; i++) {
				addIldaPoint(shape->getStartPos(), black, 1);
			}
		} else {
			// no blanks if we're there already? 
			// PRE BLANK
			//for(int i = 0; i<shapePreBlank; i++) {
			//	addIldaPoint(shape->getStartPos(), shape.getC, 1);
			//}
		}
		
		
		
		
		// Is it a dot?
		LaserDot * dot = dynamic_cast<LaserDot*>(shape);
		if(dot) {
			drawLaserDot(*dot); 
		}
		// Is it a line?
		LaserLine * line = dynamic_cast<LaserLine*>(shape);
		if(line) {
			drawLaserLine(*line);
		}
		// Is it a circle?
		LaserCircle * circle = dynamic_cast<LaserCircle*>(shape);
		if(circle) {
			drawLaserCircle(*circle);
		}
		// Is it a line?
		LaserPolyline * poly = dynamic_cast<LaserPolyline*>(shape);
		if(poly) {
			drawLaserPolyline(*poly);
		}
		
		// Is it a spiral?
		LaserSpiral * spiral = dynamic_cast<LaserSpiral*>(shape);
		if(spiral) {
			drawLaserSpiral(*spiral);
		}
	
		// what's the point of pre and post blank? just one blank is enough, right? 
		// POST BLANK
		for(int i = 0; i<shapePostBlank; i++) {
			addIldaPoint(shape->getEndPos(), black, 1);
		}
		currentPosition = shape->getEndPos();
		
	}
	
	moveLaser(currentPosition, maskRectangle.getCenter());
	
		
}

void LaserManager :: moveLaser(const ofPoint & currentPosition, const ofPoint & targetpoint){
	
	ofPoint target = targetpoint;
	ofPoint start = currentPosition;

	ofPoint v = target-start;
		
	float blanknum = v.length()/moveSpeed + movePointsPadding;
	
	for(int j = 0; j<blanknum; j++) {
		
		float t = Quint::easeInOut((float)j, 0.0f, 1.0f, blanknum);
		
		ofPoint c = (v* t) + start;
		addIldaPoint(c, (showMovePoints && j%2==0) ? ofColor(100,0,0) : ofColor::black);
		
	}
	
}


void LaserManager:: drawLaserDot(LaserDot &dot) {
	//cout << i << " DOT!" << endl;
	int particlecount = dotMaxPoints * dot.intensity;// ceil(dotMaxPoints* dot->intensity);

	for(int i = 0; i<particlecount; i++) {
		addIldaPoint(dot.getStartPos(), dot.colour);
	}
	
}

void LaserManager:: drawLaserCircle(LaserCircle &circle){
	
	float distanceTravelled = 2*PI*circle.radius + circle.overlapDistance;
	vector<float> unitDistances = getPointsAlongDistance(distanceTravelled, accelerationCircle, speedCircle);
	
	ofPoint p;
	ofColor segmentColour;
	
	for(int i = 0; i<unitDistances.size(); i++) {
		
		float unitDistance = unitDistances[i];
		float angle;
		if(!circle.reversed) angle = ofMap(unitDistance,0,1,-circle.overlapAngle/2,360+(circle.overlapAngle/2)) ;
		else angle = ofMap(unitDistance,1,0,-circle.overlapAngle/2,360+(circle.overlapAngle/2)) ;
		
		segmentColour = circle.colour;
		
		if(angle<circle.overlapAngle/2) {
			segmentColour*= ofMap(angle, -circle.overlapAngle/2,circle.overlapAngle/2, 0, 1);
		} if(angle> 360 - circle.overlapAngle/2) {
			segmentColour *= ofMap(angle, 360 -circle.overlapAngle/2,360 + circle.overlapAngle/2, 1, 0);
		}			
		
		p.set(circle.pos);
		p.x+=sin(ofDegToRad(angle))*circle.radius;
		p.y-=cos(ofDegToRad(angle))*circle.radius;
		
		addIldaPoint(p, circle.colour, circle.intensity);
	}
}


void LaserManager::addLaserText(string line, ofVec3f pos, float size, ofColor colour, bool centred) {

	textWriter.colour = colour;

	laserWordMesh = textWriter.getMesh(line, pos, size, centred);;
	vector<ofVec3f>& vertices = laserWordMesh.getVertices();

	for(int i = 0; i<vertices.size(); i+=2) {
		if(i+1>=vertices.size()) break;
		// bit of a hack in case you wanted to move your
		// writing around in 3D
		vertices[i].z = pos.z;
		vertices[i+1].z = pos.z;
		addLaserLineEased(vertices[i], vertices[i+1], laserWordMesh.getColors()[i]);
	}

}


vector<float> LaserManager:: getPointsAlongDistance(float distance, float acceleration, float maxspeed){
	
	vector<float> unitDistances;
	
	float acceleratedistance = (maxspeed*maxspeed) / (2*acceleration);
	float timetogettospeed = maxspeed / acceleration;
	
	float totaldistance = distance;
	
	float constantspeeddistance = totaldistance - (acceleratedistance*2);
	float constantspeedtime = constantspeeddistance/maxspeed;
	
	if(totaldistance<(acceleratedistance*2)) {
		
		constantspeeddistance = 0 ;
		constantspeedtime = 0;
		acceleratedistance = totaldistance/2;
		maxspeed = sqrt( acceleratedistance * 2 * acceleration);
		timetogettospeed = maxspeed / acceleration;
		
	}
	
	float totaltime = (timetogettospeed*2) + constantspeedtime;
	
	float timeincrement = totaltime / (floor(totaltime));
	
	float currentdistance;
	
	float t = 0;
		
	while (t <= totaltime + 0.001) {
		
		if(t>totaltime) t = totaltime;
		
		if(t <=timetogettospeed) {
			currentdistance = 0.5 * acceleration * (t*t);
			//ofSetColor(0,255,0);
		} else if((t>timetogettospeed) && (t<=timetogettospeed+constantspeedtime)){
			currentdistance = acceleratedistance + ((t-timetogettospeed) * maxspeed);
			//ofSetColor(255);
		} else  {
			float t3 = t - (timetogettospeed + constantspeedtime);
			
			currentdistance = (acceleratedistance + constantspeeddistance) + (maxspeed*t3)+(0.5 *(-acceleration) * (t3*t3));
			//ofSetColor(255,0,0);
			
		}
		
		unitDistances.push_back(currentdistance/totaldistance); 
		
		t+=timeincrement;
		
	}
	
	return unitDistances; 
	
}





void LaserManager:: drawLaserLine(LaserLine& line) {
	
	ofPoint start = line.getStartPos();// warp.getWarpedPoint(startpoint);
	ofPoint end = line.getEndPos();//warp.getWarpedPoint(endpoint);
	
	ofPoint vec = end - start;
	
	float speed = speedEasedLine;
	
	int iterations = floor(vec.length()/speed) + paddingEasedLine; // arbitrary amount to create enough ease points!
	
	// TODO add start and end blanks for lines?
	 
	for(float i = 0; i<iterations; i++) {
		
		float t = Quad::easeInOut(i/iterations, 0, 1, 1);
		
		addIldaPoint(start + (vec*t), line.colour, line.intensity);
		
	}
	
	
}

void LaserManager::drawLaserPolyline(LaserPolyline& laserpoly, bool onlyUpdatePreviewMesh) {
	
	ofPolyline& poly = laserpoly.polyline;
	
	
	/*
	vector<ofVec3f>& vertices = poly.getVertices();
	
	for(int i=0; i<vertices.size()-1; i++) {
		ofPoint start = vertices[i];
		ofPoint end = vertices[i+1];
		
		ofPoint vec = end - start;
		
		float speed = speedEasedLine;
		
		int iterations = floor(vec.length()/speed) + paddingEasedLine; // arbitrary amount to create enough ease points!
		
		// TODO add start and end blanks for lines?
		
		for(float j = 0; j<iterations; j++) {
			
			float t = Quad::easeInOut(j/iterations, 0, 1, 1);
			
			addIldaPoint(start + (vec*t), ofColor::white, laserpoly.intensity);
			
		}
		
	}*/
	
	laserpoly.previewMesh.clear();
	laserpoly.previewMesh.setMode(OF_PRIMITIVE_LINES);
	int startpoint = 0;
	int endpoint = 0;
	
	while(endpoint<(int)poly.getVertices().size()-1) {

		do {
			endpoint++;
		} while ((endpoint< (int)poly.getVertices().size()-1) && abs(poly.getAngleAtIndex(endpoint)) < 30);
		
		
		float startdistance = poly.getLengthAtIndex(startpoint);
		float enddistance = poly.getLengthAtIndex(endpoint);
		
		float length = enddistance - startdistance;
		
		ofPoint lastpoint;
		
		if(length>0) {
			
			vector<float> unitDistances = getPointsAlongDistance(length, accelerationLine, speedLine);
			
			ofColor pointcolour;
			
			for(int i = 0; i<unitDistances.size(); i++) {
				ofPoint p = poly.getPointAtLength((unitDistances[i]*0.999* length) + startdistance);
				pointcolour = laserpoly.getColourForPoint(unitDistances[i], p);
				
				if(!onlyUpdatePreviewMesh) addIldaPoint(p, pointcolour, laserpoly.intensity);
				
				if(i>0) {
					laserpoly.previewMesh.addVertex(lastpoint);
					laserpoly.previewMesh.addVertex(p);
					laserpoly.previewMesh.addColor(pointcolour*laserpoly.intensity);
					laserpoly.previewMesh.addColor(pointcolour*laserpoly.intensity);
					//cout << lastpoint << " " << p << " " << pointcolour << endl;
					
				}
				lastpoint = p; 
			}
			
		}
		
		startpoint=endpoint;
		
	}
	
	
}

// can the shape itself be put in the object?

void LaserManager::drawLaserSpiral(LaserSpiral& spiral){
	
	// should probably be a setting
	
	//float spacing = spiralSpacing;
	float revolutions = ((spiral.radius2 - spiral.radius1)/spiralSpacing);
	//float spaceBetweenRevs = (spiral.radius2 - spiral.radius1)/revolutions;
	
	float maxAngle = 360 * revolutions;
	
	float currentAngle = 0;
	
	
	ofVec2f pos(spiral.getStartPos());
	
	float speed = 0;
	float maxSpeed = 20;
	
	float rotateSpeed = 1; 
	ofPolyline path;
	//path.setFilled(false);
	//path.setStrokeWidth(1);
	
	path.addVertex(pos);
	
	// TODO - DRAW BACKWARDS if reversed!

	while (currentAngle<=maxAngle + 360) {
		
		pos.set(ofMap(currentAngle, 0, maxAngle, spiral.radius1, spiral.radius2, true), 0);
		pos.rotate(currentAngle);
		pos+=spiral.pos;
		
		//rotateSpeed = ofMap(currentAngle, 1, 45,0,maxSpeed, true);
		// should be a setting!
		currentAngle+=rotateSpeed;
		path.addVertex(pos); 
		
			
		//addIldaPoint(pos, spiral.colour, spiral.intensity);
		
	}
	
	float totaldistance = path.getPerimeter();
	
	vector<float> unitDistances = getPointsAlongDistance(totaldistance, accelerationCircle, speedCircle);

	for(int i = 0; i<unitDistances.size(); i++) {
		pos = path.getPointAtLength(min(unitDistances[i]*totaldistance, totaldistance-0.01f));
		//cout << unitDistances[i] << " " <<pos << endl;
		addIldaPoint(pos, spiral.colour, spiral.intensity);
		
	}
	
	
}



void LaserManager::addLaserRectEased(const ofPoint&topLeft, const ofPoint&dimensions, ofFloatColor colour){
	
	addLaserLineEased(topLeft, ofPoint(topLeft.x+dimensions.x,topLeft.y), colour);
	addLaserLineEased(ofPoint(topLeft.x+dimensions.x,topLeft.y), topLeft+dimensions, colour);
	addLaserLineEased(topLeft+dimensions, ofPoint(topLeft.x,topLeft.y+dimensions.y), colour);
	addLaserLineEased(ofPoint(topLeft.x,topLeft.y+dimensions.y), topLeft, colour);
	
}

ofxIlda::Point LaserManager::ofPointToIldaPoint(const ofPoint& ofpoint, ofFloatColor colour){
	
	ofPoint p = ofpoint;//gLProject(ofpoint.x, ofpoint.y, ofpoint.z, appHeight);
	
	
	
	ofxIlda::Point ildapoint;
	
	
	if(flipY) p.y= appHeight-p.y;
	if(flipX) p.x= appWidth-p.x;
	
	p.x = ofClamp(p.x, 0, appWidth);
	p.y = ofClamp(p.y, 0, appHeight);
	

	ildapoint.set(p, colour, pmin, pmax);
	return ildapoint;
	
}







ofPoint LaserManager::ildaPointToOfPoint(const ofxIlda::Point& ildapoint){
	
	ofxIlda::Point p = ildapoint; 
	

	p.x = ofMap(p.x, kIldaMinPoint, kIldaMaxPoint, 0, appWidth);
	p.y = ofMap(p.y, kIldaMinPoint, kIldaMaxPoint, 0, appHeight);
	if(flipY) p.y= appHeight-p.y;
	if(flipX) p.x= appWidth-p.x;
	
	return  warp.getUnWarpedPoint(ofVec3f(p.x, p.y));;
}

void LaserManager::addIldaPoint(ofPoint p, ofFloatColor c, float pointIntensity, bool useCalibration){
	
	//bool offScreen = false;

	// TODO should be smarter about this. Ideally we should mark the on / off screen
	// points and add a move between them.
	
	// inside doesn't work because I need points on the edge of the rect to work. 
	//if(!maskRectangle.inside(p)) {
	if(p.x<maskRectangle.getLeft() ||
	   p.x>maskRectangle.getRight() ||
	   p.y<maskRectangle.getTop() ||
	   p.y>maskRectangle.getBottom()) {
		
		//offScreen = true;
		p.x = ofClamp(p.x, maskRectangle.getLeft(), maskRectangle.getRight()); 
		p.y = ofClamp(p.y, maskRectangle.getTop(), maskRectangle.getBottom());
		
		
		//p.x = max(maskRectangle.x, p.x);
		//p.x = min(maskRectangle.getRight(), p.x);
		//p.y = max(maskRectangle.y, p.y);
		//p.y = min(maskRectangle.getBottom(), p.y);
		
		if(!offScreen) {
			offScreen = true;
			offScreenPoint = p;
			addPoint(offScreenPoint, c, useCalibration);

		}
		lastClampedOffScreenPoint = p; 
		
		
	} else {
		
		if(offScreen){
			// ease between offScreenPoint and lastClampedOffScreenPoint
			offScreen = false;
			
			ofPoint target = lastClampedOffScreenPoint;
			ofPoint start = offScreenPoint;
			
			ofPoint v = target-start;
			
			float blanknum = v.length()/moveSpeed + movePointsPadding;
			
			for(int j = 0; j<blanknum; j++) {
				
				float t = Quint::easeInOut((float)j, 0.0f, 1.0f, blanknum);
				
				ofPoint c = (v* t) + start;
				addPoint(c, (showMovePoints && j%2==0) ? ofColor(100,0,0) : ofColor::black, useCalibration);
				
			}
		}
		
		c.r*=pointIntensity; // ((float)colourCorrection->r/255.0f);
		c.g*=pointIntensity; // ((float)colourCorrection->g/255.0f);
		c.b*=pointIntensity; // ((float)colourCorrection->b/255.0f);
		
		if(useMaskBitmap) {
			float brightness = maskBitmap.getColor(p.x/appWidth* (float)maskBitmap.getWidth(), p.y/appHeight * (float)maskBitmap.getHeight()).getBrightness();
			c*= brightness/255.0f;
			//cout << "brightness " << brightness << endl;
			
		}
		
		addPoint(p, c, useCalibration);

	}
}

void LaserManager::addPoint(ofPoint p, ofFloatColor c, bool useCalibration) {
	
	if(useCalibration) {
		c.r = calculateCalibratedBrightness(c.r, intensity, red100, red75, red50, red25, red0);
		c.g = calculateCalibratedBrightness(c.g, intensity, green100, green75, green50, green25, green0);
		c.b = calculateCalibratedBrightness(c.b, intensity, blue100, blue75, blue50, blue25, blue0);
	}
	
	if(!showPostTransformPreview) pathMesh.addVertex(p);

	
	ofPoint warpedpoint = warp.getWarpedPoint(p);
	
	ofxIlda::Point ildapoint = ofPointToIldaPoint(warpedpoint, c);
	ildaPoints.push_back(ildapoint);
	
	if(showPostTransformPreview){
		ofPoint previewpoint;
		previewpoint.x = ofMap(ildapoint.x, kIldaMinPoint, kIldaMaxPoint, 0, appWidth);
		previewpoint.y = ofMap(ildapoint.y, kIldaMinPoint, kIldaMaxPoint, 0, appHeight);
		pathMesh.addVertex(previewpoint);
		
	}
	
	if (showColourPoints) pathMesh.addColor(c);
	//currentPosition = p;
}

float LaserManager::calculateCalibratedBrightness(float value, float intensity, float level100, float level75, float level50, float level25, float level0){
	
	value *=intensity;
	if(value<0.25) {
		return ofMap(value, 0, 0.25, level0, level25);
	} else if(value<0.5) {
		return ofMap(value, 0.25, 0.5,level25, level50); 
	} else if(value<0.75) {
		return ofMap(value, 0.5, 0.75,level50, level75);
	} else {
		return ofMap(value, 0.75, 1,level75, level100);
	}
	
}

bool LaserManager:: toggleRegistration() {
	
	testPattern++;
	if(testPattern>numTestPatterns) testPattern = 0; 
	
}

void LaserManager :: renderPreview() {
	
	// do dots first

	
	ofPushStyle();
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	ofMesh mesh;
	
	vector <ofVec3f> vertices;
	vector <ofVec2f> texCoords;
	ofImage* image = &pointPreviewImage; 
	
	vertices.push_back(ofVec3f(    -0.5, -0.5 ));
	vertices.push_back(ofVec3f(  -0.5,  0.5 ));
	vertices.push_back(ofVec3f(  0.5,  -0.5 ));
	vertices.push_back(ofVec3f( 0.5,  0.5 ));
	
	texCoords.clear();
	texCoords.push_back(ofVec2f(0,0));
	texCoords.push_back(ofVec2f(0, image->height));
	texCoords.push_back(ofVec2f(image->width, 0));
	texCoords.push_back(ofVec2f(image->width, image->height));
	
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	
	vector<LaserShape*> newshapes = (shapesHistory.size()>0) ? shapesHistory.back() : shapes;
	
	for(int i = 0; i<newshapes.size(); i++) {
		
		LaserShape* shape = newshapes[i];

			
		// Is it a dot?
		LaserDot * dot = dynamic_cast<LaserDot*>(shape);
		if(dot) {
			
			int vertexIndex = mesh.getNumVertices();
			
			for(int i = 0; i<vertices.size(); i++) {
				ofVec3f v = vertices[i];
				
				v*=dot->intensity*10;
				v+=dot->getStartPos();
				mesh.addVertex(v);
				mesh.addColor(dot->colour);
				mesh.addTexCoord(texCoords[i]);
				
			}
			
			mesh.addTriangle(vertexIndex, vertexIndex+1, vertexIndex+2);
			mesh.addTriangle(vertexIndex+1, vertexIndex+2, vertexIndex+3);
			

		}
				
		
	}
	image->bind();
	mesh.draw();
	image->unbind();
	
	
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	
	
	for(int i = 0; i<newshapes.size(); i++) {
		
		LaserShape* shape = newshapes[i];
		
		
		// Is it a dot?
		LaserCircle * circle = dynamic_cast<LaserCircle*>(shape);
		if(circle) {
			
			ofVec3f v(0,-circle->radius);
			mesh.addColor(ofColor::black);
			mesh.addVertex(v + circle->pos);
			
			for(int i = 0; i<=360; i+=10) {
				v.set(0, -circle->radius);
				v.rotate(i, ofVec3f(0,0,1)); 
				mesh.addColor(circle->colour);
				mesh.addVertex(v+circle->pos);
			}
			
			v.set(0, -circle->radius);
			mesh.addColor(ofColor::black);
			mesh.addVertex(v+circle->pos);
		}
	
		// Is it a line?
		LaserLine * line = dynamic_cast<LaserLine*>(shape);
		if(line) {
			mesh.addColor(ofColor::black);
			mesh.addVertex(line->getStartPos());
			
			mesh.addColor(line->colour);
			mesh.addVertex(line->getStartPos());
			mesh.addColor(line->colour);
			mesh.addVertex(line->getEndPos());
			
			mesh.addColor(ofColor::black);
			mesh.addVertex(line->getEndPos());
		}
		
		LaserSpiral* spiralptr = dynamic_cast<LaserSpiral*>(shape);
		if(spiralptr) {
			mesh.addColor(ofColor::black);
			mesh.addVertex(spiralptr->getStartPos());
			
			
			LaserSpiral& spiral = *spiralptr;
			float revolutions = ((spiral.radius2 - spiral.radius1)/spiralSpacing);
			
			float maxAngle = 360 * revolutions;
			
			float currentAngle = 0;
			
			ofVec2f pos;
			
			float speed = 0;
			float maxSpeed = 20;
			
			float rotateSpeed = 1;
			ofPolyline path;
			
			
			
			while (currentAngle<=maxAngle + 360) {
				
				pos.set(ofMap(currentAngle, 0, maxAngle, spiral.radius1, spiral.radius2, true), 0);
				pos.rotate(currentAngle);
				pos+=spiral.pos;
				
				//rotateSpeed = ofMap(currentAngle, 1, 45,0,maxSpeed, true);
				// should be a setting!
				currentAngle+=rotateSpeed;
				path.addVertex(pos);
			}
			
			float totaldistance = path.getPerimeter();
			
			vector<float> unitDistances = getPointsAlongDistance(totaldistance, accelerationLine, speedLine);
			
			for(int i = 0; i<unitDistances.size(); i++) {
				pos = path.getPointAtLength(min(unitDistances[i]*totaldistance, totaldistance-0.01f));
				
				mesh.addVertex(pos);
				mesh.addColor(spiral.colour);
			}
			
			mesh.addColor(ofColor::black);
			mesh.addVertex(spiralptr->getEndPos());
			
			
			
		}
		
		LaserPolyline* laserpoly = dynamic_cast<LaserPolyline*>(shape);
		if(laserpoly) {
			if(laserpoly->previewMesh.getVertices().size() ==0 ) {
				drawLaserPolyline(*laserpoly, true);
			}
			ofSetColor(255);
			ofNoFill();
			ofSetLineWidth(2);
			laserpoly->previewMesh.draw();
			
			//ofSetLineWidth(3);
			//laserpoly->previewMesh.draw();
			
		}
			
		
	}
	
	ofSetLineWidth(1);
	mesh.draw();
	
	vector<ofFloatColor>& colours = mesh.getColors();
	
	for(int i = 0; i<colours.size(); i++) {
		colours[i].r*=0.2;
		colours[i].g*=0.2;
		colours[i].b*=0.2;
	}
	
	ofSetLineWidth(5);
	mesh.draw();
	
	ofSetLineWidth(9);
	mesh.draw();


	
	ofPopStyle();
	
}

