#ifndef __GRABBABLESOURCE.H__
#define __GRABBABLESOURCE.H__

#include "ofMain.h"
#include "ofxGrabbableVector.h"
#include "ofxAnimationI.h"
#include "ParticleSystem.h"

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
	float calcRadius;
public:
	virtual void setup(float x,float y, float w, float h,ParticleSystem * ps){
		ofxGrabbableVector::setup(x,y,w,h);
		timeGrabber.setup(x+50,y,15,15);//TODO gui / auslagern
		timeGrabber.color.set(20,240,30);
		timeGrabber.fillMe = true;
		color.set(20,240,30,180);
		colorHover.set(20,240,30,255);
		particleSystem = ps;
		run = false;
		modi = 0;
	}

	virtual void start(int modi = 0){
		cout << "startet mit modi: " << modi << endl;
		this->modi = modi;
		calcRadius = getRadius();
		run = true;
	}

	virtual void stop(){
		run = false;
	}

	virtual void update();//TODO radius und zeit für bestimmung wie oft und wieviele!!!

	virtual void drawGUI(){
		ofxGrabbableVector::draw();
		timeGrabber.draw();
	}

	virtual void drawAnimation(){}//TODO nothing?!
	virtual int getModiCount(){ return 2; }//TODO !!

	virtual void setGrabbing(bool bGrabbing){
		bGrabbingEnabled = bGrabbing;
		timeGrabber.setGrabbing(bGrabbing);
	}

	virtual void saveToXml(ofxXmlSettings & xml){
		ofxAnimationI::saveToXml(xml);
		xml.setValue("Typ","SOURCE");
		ofxGrabbableVector::saveToXml(xml);
	}
};

#endif
