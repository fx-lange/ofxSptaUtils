#ifndef __ANIMATION_I.H__
#define __ANIMATION_I.H__


#include "ofMain.h"
#include "ofxGrabbableObject.h"
#include "ofxXmlSettings.h"

/*	Eventinterface
 *
 */
class ofxAnimationI {
protected:
	ofxGrabbableObject timeGrabber;
	toggleButton visible;
public:
	virtual void start(int modi = 0) = 0;
	virtual void stop() = 0;
	virtual void update() = 0;
	virtual void drawGUI(){
		ofPushStyle();
		ofNoFill();
		timeGrabber.draw();
		if(visible.bActive)
			ofFill();
		ofSetColor(visible.color);
		ofEllipse(visible,visible.width,visible.height);
		ofPopStyle();
	}
	virtual void drawAnimation() = 0;
	virtual ofxGrabbableObject * getTimeGrabber(){
		return &timeGrabber;
	}
	virtual int getModiCount(){ return 2; }//TODO auf = 0
	virtual void setGrabbing(bool bGrabbing){
		timeGrabber.setGrabbing(bGrabbing);
//		visible.setGrabbing(bGrabbing);
	}

	virtual void saveToXml(ofxXmlSettings & xml){
		xml.addTag("TimeGrabber");
		xml.pushTag("TimeGrabber");
		timeGrabber.saveToXml(xml);
		xml.popTag();
		xml.addTag("Visible");
		xml.pushTag("Visible");
		visible.saveToXml(xml);
		xml.popTag();
	}
	virtual void loadFromXml(ofxXmlSettings & xml){
		xml.pushTag("TimeGrabber");
		timeGrabber.loadFromXml(xml);
		xml.popTag();
		xml.pushTag("Visible");
		visible.loadFromXml(xml);
		xml.popTag();
	}

};


#endif
