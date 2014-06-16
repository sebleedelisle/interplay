#pragma once

#include "ofxGuiGroup.h"

class ofxGuiGroup;

class ofxPanel : public ofxGuiGroup {
public:
	ofxPanel();
	ofxPanel(const ofParameterGroup & parameters, string filename="settings.xml", float x = 10, float y = 10);

	ofxPanel * setup(string collectionName="", string filename="settings.xml", float x = 10, float y = 10);
	ofxPanel * setup(const ofParameterGroup & parameters, string filename="settings.xml", float x = 10, float y = 10);

	bool mouseReleased(ofMouseEventArgs & args);

	ofEvent<void> loadPressedE;
	ofEvent<void> savePressedE;
    
    void setVisible(bool visible);
    bool getVisible();
    void toggleVisible();
    
    virtual void draw();

protected:
	void render();
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void loadIcons();

    bool isVisible;

private:
	ofRectangle loadBox, saveBox;
	static ofImage loadIcon, saveIcon;
    
    ofPoint grabPt;
	bool bGrabbed;
};
