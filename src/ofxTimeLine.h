#ifndef __TIMELINE.H__
#define __TIMELINE.H__

#include "ofMain.h"
#include "ofxAnimationI.h"
#include "ofxTimeWindow.h"
#include "ofxXmlSettings.h"


class ofxTimeLine : public ofxGrabbableObject{
protected:
	vector<ofxTimeWindow*> timeWindows;
	vector<ofxAnimationI *> animations;
	TimeLineSettings settings;
	ofxXmlSettings xml;

	long startTime;

	void checkAnimations(){
		for(int i=0;i<animations.size();i++){
			ofxGrabbableObject * obj = animations[i]->getTimeGrabber();
			if(dropZone.inside(obj->x,obj->y) && obj->isPressed() == false){
				obj->y = y - 20;
				//TODO animation wird timeline hinzugefügt
				ofxTimeWindow * tw = new ofxTimeWindow();
				tw->setup(obj->x,y+dropZone.height-lineHeight*(i+0.5),lineHeight,lineHeight,animations[i]);
				tw->color.set(0,0,200);
				tw->setTimer(&settings);
				tw->setAnimationIdx(i);
				timeWindows.push_back(tw);
			}
		}
	}

	void checkTimeWindows(){
		vector<ofxTimeWindow*>::iterator it;
		for(it = timeWindows.begin();it<timeWindows.end();){
			ofxTimeWindow * tw = *it;
			if(tw->bDeleteMe){
				delete tw;
				it = timeWindows.erase(it);
			}else{
				++it;
			}
		}
	}

public:
	ofRectangle dropZone;
	float timerange;
	float stepSize;
	float lineHeight;
	bool bShowAnimationObjects;

	virtual void setup(float x,float y,float w,float h){
		lineHeight = 30;
		h = round(h/lineHeight)*lineHeight;
		y = ofGetHeight()-h;
		ofxGrabbableObject::setup(x,y,20,20);
		dropZone.set(x,y,w,h);
		timerange = 300;
		stepSize = 15;
		settings.x0 = &(dropZone.x);
		settings.y0 =&(dropZone.y);
		settings.timerange = &timerange;
		settings.width = &(dropZone.width);
		settings.height = &(dropZone.height);
		settings.stepSize = &stepSize;
		bShowAnimationObjects = true;
	}

	void update(){
		checkAnimations();
		checkTimeWindows();
	}

	virtual void draw(){
		ofPushStyle();
		ofxGrabbableObject::draw();
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(190,240,0,100);
		ofRect(dropZone);
		ofNoFill();
		ofSetColor(30,30,30,255);
		ofRect(dropZone);

		float diff = (ofGetSystemTime()-startTime)/1000.f;
		float lineX = diff * timerange / width;
		ofSetColor(0,0,0,255);
		ofLine(lineX,y,lineX,dropZone.y+dropZone.height);


		//rows
		ofSetColor(30,30,30,80);
		for(int yi=dropZone.y;yi<dropZone.y+dropZone.height;yi+=(int)lineHeight){
			ofLine(dropZone.x,yi,dropZone.width+dropZone.x,yi);
		}
		ofDisableAlphaBlending();
		//ticks
		float tickSize = dropZone.width / timerange;
		int count = 0;
		for(int xi=dropZone.x;xi<dropZone.x+dropZone.width;xi+=(int)tickSize){
			if(count++ % 30 == 0){
				ofPushStyle();
				ofSetColor(0,0,0,240);
				ofLine(xi,dropZone.y-lineHeight,xi,dropZone.y+dropZone.height);
				ofPopStyle();
			}else{
				ofLine(xi,dropZone.y,xi,dropZone.y+dropZone.height);
			}
		}
		for(int i=0;i<timeWindows.size();++i){
			timeWindows[i]->draw();
		}
		if(bShowAnimationObjects){
			for(int i=0;i<animations.size();++i){
				animations[i]->drawGUI();
			}
		}
		ofPopStyle();
	}

	void addAnimation(ofxAnimationI * animation){
		animations.push_back(animation);
	}

	void startAnimations(){
		//TODO offset mit eigenem timer
		for(int i=0;i<timeWindows.size();++i){
			timeWindows[i]->startTimer();
		}
		startTime = ofGetSystemTime();
	}

	void stopAnimations(){
		//TODO offset mit eigenem timer
		for(int i=0;i<timeWindows.size();++i){
			timeWindows[i]->stopTimer();
		}
	}

	virtual void mouseDragged(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !bPressed)
			return;
		if(e.x<0 || e.x>ofGetWidth())
			return;

		float xDiff = e.x - pX;
		float yDiff = e.y - pY;
		x += xDiff;
		dropZone.x -= (xDiff*3.f);
		pX = e.x;
		pY = e.y;

		for(int i=0;i<timeWindows.size();++i){
			ofxTimeWindow * window = timeWindows[i];
			window->x -=(xDiff*3.f);
			window->timeRangeGrabber.x -=(xDiff*3.f);
			for(int j=0;j<window->modis.size();++j){
				window->modis[j]->x -=(xDiff*3.f);
			}
		}
	}

	virtual void mouseReleased(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !bPressed )
			return;
		bPressed = false;

		for(int i=0;i<timeWindows.size();++i){
			timeWindows[i]->calcStartTime();
		}
	}

	virtual void setGrabbing(bool bGrabbing){
		bGrabbingEnabled = bGrabbing;
		for(int i=0;i<timeWindows.size();++i){
			timeWindows[i]->setGrabbing(bGrabbing);
		}
		for(int i=0;i<animations.size();++i){
			animations[i]->setGrabbing(bGrabbing);
		}
	}

	void saveTrack(){
		int tagNum;
		//--- track details
		tagNum = xml.addTag("TRACK");
		cout << "save track nr:" << tagNum << endl;
		xml.pushTag("TRACK",tagNum);
		xml.addTag("Details");
		xml.pushTag("Details");
		//TODO länge des tracks - minimal letztes ende einer animation
		xml.setValue("Length",0);
		xml.popTag();

		//--- save animations
		xml.addTag("Animations");
		xml.pushTag("Animations");
		for(int i=0;i<animations.size();++i){
			tagNum = xml.addTag("Animation");
			xml.pushTag("Animation",tagNum);
			xml.setValue("Id",i);
			animations[i]->saveToXml(xml);
			xml.popTag();
		}
		xml.popTag();
		// --

		//--- save timewindows
		xml.addTag("TimeWindows");
		xml.pushTag("TimeWindows");
		for(int i=0;i<timeWindows.size();++i){
			tagNum = xml.addTag("TimeWindow");
			xml.pushTag("TimeWindow",tagNum);
			xml.setValue("Id",i);
			timeWindows[i]->saveToXml(xml);
			xml.popTag();
		}
		xml.popTag();
		// --
		xml.popTag();
		xml.saveFile("track.xml");
	}
};

#endif
