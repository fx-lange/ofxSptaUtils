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
	bool run,runEnd;
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
		runEnd=false;
		this->modi=modi;
	}

	virtual void stop(){
		run = false;
		runEnd=true;
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
	
	void drawTail();
	void drawTailMulti();
	void drawFull();
	
	void setColorEffect(int i,int now);

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

	virtual void saveToXml(ofxXmlSettings & xml){
		ofxAnimationI::saveToXml(xml);
		ofxGrabbableObject::saveToXml(xml);
		xml.setValue("Type","TAILANIMATION");//TODO über enum lösen!
		xml.setValue("StartIdx",startIdx);
		xml.addTag("Points");
		xml.pushTag("Points");
		for(int i=0;i<pts.size();++i){
			int tagNum = xml.addTag("Point");
			xml.pushTag("Point",tagNum);
			ofPoint & p = pts[i];
			xml.setValue("Id",i);
			xml.setValue("X",p.x);
			xml.setValue("Y",p.y);
			xml.popTag();
		}
		xml.popTag();
	}

	virtual void loadFromXml(ofxXmlSettings & xml){
		ofxAnimationI::loadFromXml(xml);
		ofxGrabbableObject::loadFromXml(xml);
		xml.pushTag("Points");
		for(int i=0;i<xml.getNumTags("Point");++i){
			xml.pushTag("Point",i);
			ofPoint p;
			p.x = xml.getValue("X",0);
			p.y = xml.getValue("Y",0);
			pts.push_back(p);
			xml.popTag();
		}
		xml.popTag();
		size = pts.size();
		startIdx = xml.getValue("StartIdx",0);
	}
};

#endif
