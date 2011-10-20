#ifndef __GRABBABLESOURCE.H__
#define __GRABBABLESOURCE.H__

#include "ofMain.h"
#include "ofxGrabbableVector.h"
#include "ofxAnimationI.h"
#include "ParticleSystem.h"
#include "ParticleQ3.h"
#include "SourceQ3.h"

class ofxGrabbableSource: public ofxGrabbableVector, public ofxAnimationI{
	/**
	 * Modis
	 * auf alpha 0 setzen oder auf alpha 255 enstehen lassen => modicount = 2
	 * evtl noch anziehung auf die mitte (kreisförmig)
	 */
protected:
	bool run;
	int modi;
	ParticleSystem * particleSystem;
	float calcRadius,calcArea;
public:
	SettingSourceQ3 * settings;
	virtual void setup(float x,float y, float w, float h,ParticleSystem * ps,SettingSourceQ3 * settings){
		ofxGrabbableVector::setup(x,y,w,h);
		timeGrabber.setup(x+50,y,15,15);//TODO gui / auslagern
		timeGrabber.color.set(20,240,30);
		timeGrabber.fillMe = true;
		visible.setup(x+50+w,y,w,h);
		visible.bActive = true;
		visible.color.set(20,240,30);
		color.set(20,240,30,180);
		colorHover.set(20,240,30,255);
		particleSystem = ps;
		run = false;
		modi = 0;
		this->settings = settings;
	}

	virtual void start(int modi = 0){
		cout << "startet mit modi: " << modi << endl;
		this->modi = modi;
		calcRadius = getRadius();
		calcArea = PI * calcRadius * calcRadius;
		run = true;
	}

	virtual void stop(){
		run = false;
	}

	virtual void update();

	virtual void drawGUI(){
		ofxAnimationI::drawGUI();
		if(visible.bActive){
			ofxGrabbableVector::draw();
			setGrabbingRest(visible.isGrabbingEnabled());
		}else{
			setGrabbingRest(false);
		}
	}

	virtual void drawAnimation(){}//TODO nothing?!
	virtual int getModiCount(){ return 2; }

	virtual void setGrabbing(bool bGrabbing){
		visible.setGrabbing(bGrabbing);
		setGrabbingRest(bGrabbing);
	}

	void setGrabbingRest(bool bGrabbing){
		ofxAnimationI::setGrabbing(bGrabbing);
		bGrabbingEnabled = bGrabbing;
	}

	virtual void saveToXml(ofxXmlSettings & xml){
		ofxAnimationI::saveToXml(xml);
		xml.setValue("Type","SOURCE");
		ofxGrabbableVector::saveToXml(xml);
	}

	virtual void loadFromXml(ofxXmlSettings & xml){
		ofxAnimationI::loadFromXml(xml);
		ofxGrabbableVector::loadFromXml(xml);
	}
};

#endif
