#ifndef __GRABBABLEOBJECT.H__
#define __GRABBABLEOBJECT.H__

#include "ofMain.h"

class ofxGrabbableObject : public ofPoint{
protected:
	float width, height;

	bool bGrabbingEnabled;

	bool bPressed;

	bool isOver(float px, float py) {
		if( abs(x - px) < width && abs(y - py) < height){
			return true;
		}else{
			return false;
		}
	}

public:
	ofColor color;

	void setup(const ofRectangle & rect){
		setup(rect.x,rect.y,rect.width,rect.height);
	}

	void setup(const ofPoint &center,float w, float h){
		setup(center.x,center.y,w,h);
	}

	void setup(float x,float y, float w, float h){
		this->x = x;
		this->y = y;
		this->width = w;
		this->height = h;
		color.set(255,0,0);
		registerGrabbing();
	}

	void draw(){
		ofPushStyle();

		ofNoFill();
		ofSetColor(color);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRect(x,y,width,height);

		ofPopStyle();
	}

	void registerGrabbing() {
		bGrabbingEnabled = true;
		ofAddListener(ofEvents.mousePressed, this, &ofxGrabbableObject::mousePressed);
		ofAddListener(ofEvents.mouseDragged, this, &ofxGrabbableObject::mouseDragged);
		ofAddListener(ofEvents.mouseReleased, this, &ofxGrabbableObject::mouseReleased);
	}

	void enableGrabbing() {
		bGrabbingEnabled = true;
	}

	void disableGrabbing() {
		bGrabbingEnabled = false;
	}

	void setGrabbing(bool bGrabbing){
		bGrabbingEnabled = bGrabbing;
	}

	void toggleGrabbing(){
		bGrabbingEnabled = !bGrabbingEnabled;
	}

	void mousePressed(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled)
			return;

		if (!isOver(e.x, e.y))
			return;

		bPressed = true;

		x = e.x;
		y = e.y;
	}

	void mouseDragged(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !bPressed)
			return;

		x = e.x;
		y = e.y;

	}

	void mouseReleased(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !bPressed)
			return;

		bPressed = false;
	}
};

#endif
