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
	vector<ofColor> colors;
	TimeLineSettings settings;
	bool checkTimeGrabber;

	long startTime;

	void checkAnimations(){
		if(!checkTimeGrabber)
			return;
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
	float lineCount;

	virtual void setup(float x,float y,float w,float h){
		lineHeight = 30;
		lineCount = round(h/lineHeight);
		cout << "linecount: " << lineCount << endl;
		h = lineCount*lineHeight;
		y = ofGetHeight()-h;
		ofxGrabbableObject::setup(x,y,20,20);
		bGrabbingEnabled = false;
		dropZone.set(x,y,w,h);
		timerange = 300;
		settings.x0 = &(dropZone.x);
		settings.y0 =&(dropZone.y);
		settings.timerange = &timerange;
		settings.width = &(dropZone.width);
		settings.height = &(dropZone.height);
		settings.stepSize = &stepSize;
		checkTimeGrabber = false;
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
		for(int i=0;i<animations.size();i++){
			ofPushStyle();
			ofSetColor(colors[i]);
			ofSetRectMode(OF_RECTMODE_CENTER);
			float yy = y+dropZone.height-lineHeight*(i+0.5);
			ofxGrabbableObject * timegrabber = animations[i]->getTimeGrabber();

			ofRect(10,yy,5,5);
			ofSetColor(255);
			ofLine(10,yy,timegrabber->x,timegrabber->y);
			ofPopStyle();
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
		ofPopStyle();
	}

	void addAnimation(ofxAnimationI * animation){
		animations.push_back(animation);
		colors.push_back(animation->getColor());
		if(animations.size()>lineCount){
			lineCount++;
			y-=lineHeight;
			dropZone.y -= lineHeight;
			dropZone.height += lineHeight;
		}
	}

	void startAnimations(){
		for(int i=0;i<timeWindows.size();++i){
			timeWindows[i]->startTimer();
		}
		startTime = ofGetSystemTime();
	}

	void stopAnimations(){
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

		for(int i=0;i<animations.size();++i){
			animations[i]->setGrabbing(bGrabbing);
		}
		setTimeWindowGrabbing(bGrabbing);
	}

	virtual void setTimeWindowGrabbing(bool bGrabbing){
		checkTimeGrabber = bGrabbing;
		for(int i=0;i<timeWindows.size();++i){
			timeWindows[i]->setGrabbing(bGrabbing);
		}
	}

	float getTrackLength(){
		float max = 0;
		for(int i=0;i<timeWindows.size();++i){
			float end = timeWindows[i]->getEndTime();
			max = end > max ? end : max;
		}
		return max;
	}

	void saveTrackToXml(string filename){
		ofxXmlSettings xml;
		int tagNum;
		//--- track details
		tagNum = xml.addTag("TRACK");
		cout << "save track nr:" << tagNum << endl;
		xml.pushTag("TRACK",tagNum);
		xml.addTag("Details");
		xml.pushTag("Details");
		xml.setValue("Length",getTrackLength());
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


		xml.saveFile(filename);
		cout << "Track gespeichert unter " << filename << endl;
	}

	void loadTrackFromXml(ofxXmlSettings & xml){
		int tagNum;
		//--- track details
		xml.pushTag("TRACK");
		//--- load animations
		//schon in app
		// --

		//--- save timewindows
		xml.pushTag("TimeWindows");
		for(int i=0;i<xml.getNumTags("TimeWindow");++i){
			xml.pushTag("TimeWindow",i);
			int aIdx = xml.getValue("AnimationIdx",0);
			ofxAnimationI * animation = animations[aIdx];
			ofxTimeWindow * tw = new ofxTimeWindow();
			tw->setup(0,y+dropZone.height-lineHeight*(aIdx+0.5),lineHeight,lineHeight,animation);
			tw->setTimer(&settings);
			tw->loadFromXml(xml);
			tw->setAnimationIdx(aIdx);
			tw->setGrabbing(false);
			timeWindows.push_back(tw);
			xml.popTag();
		}
		xml.popTag();
		// --
		xml.popTag();
	}
};

#endif
