#ifndef __TIMEWINDOW.H__
#define __TIMEWINDOW.H__


#include "ofMain.h"
#include "ofxGrabbableObject.h"
#include "ofxAnimationI.h"
#include "ofxTimer.h"
#include "ofxXmlSettings.h"

struct TimeLineSettings{
	float * x0,* y0, * width, * height, * timerange, * stepSize;
};

class ofxTimeWindow : public ofxGrabbableObject{
protected:
	ofxAnimationI * animation;
	int animationIdx;
	ofxTimer timer ,timerStop;
	float resetY;
	float buttonSize;
	TimeLineSettings * settings;
	ofRectangle drawRect;

	void addModis(int count){
		for(int i=0;i<count;++i){
			toggleButton * obj = new toggleButton();
			obj->setup(x+width + i*(buttonSize*1.1),y,buttonSize,buttonSize);
			obj->disableGrabbing();
			modis.push_back(obj);
		}
	}
public:
	vector<toggleButton*> modis;
	ofxGrabbableYFix timeRangeGrabber;
	bool bDeleteMe;
	virtual ~ofxTimeWindow(){
		timer.stopTimer();
		ofRemoveListener(timer.TIMER_REACHED, this, &ofxTimeWindow::startAnimation);
	}

	virtual void setup(float x,float y,float w,float h,ofxAnimationI * animation){
		//precalc
		float calcY = y - 0.5*h;
		float calcH = h * 0.75;
		calcY += 0.5*calcH;
		int modiCount = animation->getModiCount()-1;
		buttonSize = 0.65 * h;
		float calcW = calcH + (modiCount * buttonSize*1.1);

		//init & settings
		ofxGrabbableObject::setup(x,calcY,calcH,calcH);
		this->animation = animation;
		drawRect.set(calcW/2-width/2,0,calcW,calcH);
		resetY = calcY;
		bDeleteMe = false;

		//additional grabber
		calcY = y + 0.25*h + 0.5*0.25*h;
		calcH = h * 0.25;
		float calcX = x - 0.25*w ;//- 0.5*0.25*w;
		timeRangeGrabber.setup(calcX,calcY,calcH,calcH);
		addModis(modiCount);

		//callback
		setCallbackFunc(ofxGrabbableObject::callbackToMoveWrapper);
		addCallbackListenerPosDiff(&timeRangeGrabber);
		for(int i=0;i<modis.size();++i){
			addCallbackListenerPosDiff(modis[i]);
		}
	}

	void setTimer(TimeLineSettings*settings){
		this->settings = settings;
		timer.setup(2000,false);
		calcStartTime();
		timer.stopTimer();
		timerStop.setup(2000,false);
		timerStop.stopTimer();
		ofAddListener(timer.TIMER_REACHED, this, &ofxTimeWindow::startAnimation);
		ofAddListener(timerStop.TIMER_REACHED, this, &ofxTimeWindow::stopAnimation);
	}

	void setAnimationIdx(int idx){
		animationIdx = idx;
	}

	ofxAnimationI * getAnimation(){
		return animation;
	}

	float calcStartTime(){
		float x0 = *(settings->x0);
		float w = *(settings->width);
		float range = *(settings->timerange);
		float time = (x - x0 - width/2) / w * range;
		timer.setTimer(time*1000);
		cout << "set timer to: " << time <<  "s" << endl;
		timer.stopTimer();
		return time;
	}

	void setPosXByStartTime(float time){
		float x0 = *(settings->x0);
		float w = *(settings->width);
		float range = *(settings->timerange);
		x = time / range * w;
		x = x + x0 + width/2;
	}

	float getAnimationLength(){
		float x0 = x-width/2;
		float xt = timeRangeGrabber.x - timeRangeGrabber.width/2;
		return abs(xt - x0) / *settings->width * *settings->timerange;
	}

	void setRangeGrabberByLength(float length){
		float x0 = x-width/2;
		float pixelLength = length / *settings->timerange * *settings->width;
		timeRangeGrabber.x = pixelLength + x0 + timeRangeGrabber.width/2;
	}

	int getModi(){
		int modi=0;
		for(int i=0;i<modis.size();++i){
			if(modis[i]->bActive){
				modi = i+1;
			}
		}
		return modi;
	}

	void setModi(int modiSet){
		for(int i=0;i<modis.size();++i){
			if(modiSet-1==i){
				modis[i]->bActive = true;
			}else{
				modis[i]->bActive = false;
			}
		}
	}

	float getEndTime(){
		float start = calcStartTime();
		float length = getAnimationLength();
		return start + length;
	}

	void moveModis(float moveX){
		for(int i=0;i<modis.size();++i){
			modis[i]->x += moveX;
		}
	}

	void startAnimation(ofEventArgs &e){
		cout << "start animation" << endl;

		animation->start(getModi());
//		cout << "Timerange: " <<getAnimationLength() << endl;
		timerStop.reset();
		timerStop.setTimer(getAnimationLength()*1000);
		timerStop.startTimer();
	}

	void stopAnimation(ofEventArgs &e){
//		cout << "stop animation" << endl;
		animation->stop();
	}

	void startTimer(){
//		cout << "start timer" << endl;
		timer.reset();
		timer.startTimer();
	}

	void stopTimer(){
		timer.stopTimer();
		timerStop.stopTimer();
		animation->stop();
	}

	virtual void mouseDragged(ofMouseEventArgs &e){
		ofxGrabbableObject::mouseDragged(e);
		if (!bGrabbingEnabled || !bPressed)
			return;
		calcStartTime();
		timeRangeGrabber.y = timeRangeGrabber.resetY;
		for(int i=0;i<modis.size();++i){
			toggleButton * button = modis[i];
			modis[i]->y = modis[i]->resetY;
		}
	}

	virtual void mouseReleased(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !bPressed)
			return;
		bPressed = false;
		float stepSize = *(settings->stepSize);
//		x = round(x/stepSize)*stepSize;
		calcStartTime();
		if(y < *(settings->y0)){
			bDeleteMe = true;
		}else{
			y = resetY;
		}
	}

	virtual void setGrabbing(bool grabbing){
		ofxGrabbableObject::setGrabbing(grabbing);
		timeRangeGrabber.setGrabbing(grabbing);
		for(int i=0;i<modis.size();++i){
			modis[i]->setClicking(grabbing);
		}
	}

	void draw(){
		ofPushStyle();
		ofEnableAlphaBlending();
		ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofSetColor(color,20);
		ofRect(x+drawRect.x,y+drawRect.y,drawRect.width,drawRect.height);
		ofNoFill();
		ofSetColor(0,0,0,255);
		ofRect(x+drawRect.x,y+drawRect.y,drawRect.width,drawRect.height);
		ofRect(*this,width,height);
		timeRangeGrabber.draw();

		for(int i=0;i<modis.size();++i){
			toggleButton * button = modis[i];
			button->fillMe = button->isClicked();
			modis[i]->draw();
		}

		ofEnableAlphaBlending();
		ofPopStyle();
	}

	virtual void saveToXml(ofxXmlSettings &  xml){
		xml.setValue("AnimationIdx",animationIdx);
		xml.setValue("Modi",getModi());
		xml.setValue("Start",calcStartTime());
		xml.setValue("Length",getAnimationLength());
	}

	virtual void loadFromXml(ofxXmlSettings &  xml){
		setModi(xml.getValue("Modi",0));
		float start = xml.getValue("Start",0);
		setPosXByStartTime(start);
		moveModis(x);
		calcStartTime();
		float length = xml.getValue("Length",0);
		setRangeGrabberByLength(length);
	}

};

#endif
