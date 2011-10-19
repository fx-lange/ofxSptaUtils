#ifndef _TAILANIMATION.H_
#define _TAILANIMATION.H_

#include "ofMain.h"
#include "ofxAnimationI.h"
#include "ofxGrabbableObject.h"

class ofxTailAnimation : public ofxGrabbableObject, public ofxAnimationI{
protected:
	int idx,tailIdx;
	int startIdx;
	int size;
	float stepSize;
	int tmpStepSize;
	vector<ofPoint> pts;
	ofVec3f drawPoint;
	float mouseX, mouseY;
	int tailLength;
	bool run;
	int modi;


	void calcNearest();
	virtual void setup(float stepSize);

public:

	virtual void setup(float x,float y,float w, float h, float stepSize=1){
		ofxGrabbableObject::setup(x,y,w,h);
		setup(stepSize);
		timeGrabber.setup(x+50,y+50,w,h);
		timeGrabber.color.set(255,255,0);
		visible.setup(x+50+w,y+50,w,h);
		visible.bActive = true;
		visible.color.set(255,255,0);
		color.set(255,255,0);
		timeGrabber.fillMe = true;
	}

	virtual void drawGUI();

	virtual void start(int modi = 0){
		cout << "startet mit modi: " << modi << endl;
		reset();
		run = true;
		this->modi=modi;
	}

	virtual void stop(){
		run = false;
	}

	virtual void update(){}

	virtual void drawAnimation();

	void setTailLength(float tailLength){
		this->tailLength = round(tailLength/stepSize);
	}

	void setAndArrangePoints(vector<ofPoint> points,bool close=true);

	void reset(){
		tailIdx = idx = startIdx;
	}

	int getSize(){
		return size;
	}

	void registerMoving() {
		ofAddListener(ofEvents.mouseMoved, this, &ofxTailAnimation::mouseMoved);
	}

	void mouseMoved(ofMouseEventArgs &e) {
		mouseX = e.x;
		mouseY = e.y;
	}

	virtual void setGrabbing(bool grabbing){
		visible.setGrabbing(grabbing);
		setGrabbingRest(grabbing);
	}

	void setGrabbingRest(bool bGrabbing){
		ofxAnimationI::setGrabbing(bGrabbing);
		bGrabbingEnabled = bGrabbing;
	}
};

#endif
