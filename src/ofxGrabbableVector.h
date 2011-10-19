#ifndef __GRABBABLEVECTOR.H__
#define __GRABBABLEVECTOR.H__

#include "ofMain.h"
#include "ofxGrabbableObject.h"

class ofxGrabbableVector : public ofxGrabbableObject{
protected:
	bool bHoverd;

	bool isInsideCircle(float x, float y){
		float dist = this->distance(ofVec3f(x,y,0));
		return dist < this->distance(lever) + 15 || dist < getRadius() + 15;
	}

public:
	ofColor colorHover;
	ofxGrabbableObject lever;
	ofxGrabbableObject radius;

	void setup(float x,float y, float w, float h);

	ofVec3f getVector(){
		ofVec3f sub = lever - *this;
//		cout << sub.x << " " << sub.y << endl;
		return sub;
	}

	void setRadius(float radius){
		this->radius.set(x+radius,y);
	}

	ofVec3f getNormVector(){
		return getVector().getNormalized();
	}

	float getVectorLength(){
		return getVector().length();
	}

	float getRadius(){
		return this->distance(radius);
	}

	virtual void draw();

	//TODO copyconstructor and destrcutor - NACH q3
	void registerMoving() {
		ofAddListener(ofEvents.mouseMoved, this, &ofxGrabbableVector::mouseMoved);
	}

	void mouseMoved(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled)
			return;
		if(isOver(e.x, e.y)){
			bHoverd = true;
			lever.enableGrabbing();
			radius.enableGrabbing();
		}else if(!(bHoverd && isInsideCircle(e.x,e.y))){
			bHoverd = false;
			lever.disableGrabbing();
			radius.disableGrabbing();
		}
	}
};
#endif
