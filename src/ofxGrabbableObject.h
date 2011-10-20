#ifndef __GRABBABLEOBJECT.H__
#define __GRABBABLEOBJECT.H__

#include "ofMain.h"
#include "ofxXmlSettings.h"

//TODO nach Q3 - grundformen rect ellipse triangle
class ofxGrabbableObject : public ofPoint{
protected:
	float pX, pY;
	bool bGrabbingEnabled;

	bool bClicked;
	bool bPressed;

	bool isOver(float px, float py) {
		if( abs(x - px) <= width/2.f && abs(y - py) <= height/2.f){
			return true;
		}else{
			return false;
		}
	}

	//CALLBACK so that one moving obj can move another
	void (*callbackPosDiff)(vector<void*>,float xdiff,float ydiff);
	void (*callbackPos)(vector<void*>,float xdiff,float ydiff);
	vector<void *> objs2CallPosDiff;
	vector<void *> objs2CallPos;

public:
	float width, height;
	ofColor color;
	bool fillMe;

	virtual ~ofxGrabbableObject(){
		unregisterGrabbing();
	}

	void setup(const ofRectangle & rect){
		setup(rect.x,rect.y,rect.width,rect.height);
	}

	void setup(const ofPoint &center,float w, float h){
		setup(center.x,center.y,w,h);
	}

	virtual void setup(float x,float y, float w, float h){
		this->x = x;
		this->y = y;
		this->width = w;
		this->height = h;
		color.set(255,0,0);
		fillMe = false;
		bClicked =  false;
		bPressed =	false;
		registerGrabbing();

		callbackPosDiff = NULL;
	}

	virtual void draw(){
		ofPushStyle();

		if(fillMe){
			ofFill();
		}else{
			ofNoFill();
		}
		ofSetColor(color);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRect(x,y,width,height);

		ofPopStyle();
	}

	//TODO copyconstructor and destrcutor
	virtual void registerGrabbing() {
		bGrabbingEnabled = true;
		ofAddListener(ofEvents.mousePressed, this, &ofxGrabbableObject::mousePressed);
		ofAddListener(ofEvents.mouseDragged, this, &ofxGrabbableObject::mouseDragged);
		ofAddListener(ofEvents.mouseReleased, this, &ofxGrabbableObject::mouseReleased);
	}

	virtual void unregisterGrabbing(){
		bGrabbingEnabled = false;
		ofRemoveListener(ofEvents.mousePressed, this, &ofxGrabbableObject::mousePressed);
		ofRemoveListener(ofEvents.mouseDragged, this, &ofxGrabbableObject::mouseDragged);
		ofRemoveListener(ofEvents.mouseReleased, this, &ofxGrabbableObject::mouseReleased);
	}

	void enableGrabbing() {
		setGrabbing(true);
	}

	void disableGrabbing() {
		setGrabbing(false);
	}

	virtual void setGrabbing(bool bGrabbing){
		bGrabbingEnabled = bGrabbing;
	}

	void toggleGrabbing(){
		setGrabbing(!bGrabbingEnabled);
	}

	bool isPressed(){
		return bPressed;
	}

	bool isClicked(){
		return bClicked;
	}

	bool isGrabbingEnabled(){
		return bGrabbingEnabled;
	}

	virtual void mousePressed(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled)
			return;

		if (!isOver(e.x, e.y)){
			bClicked = false;
			return;
		}

		bClicked = bPressed = true;

		pX = e.x;
		pY = e.y;
	}

	virtual void mouseDragged(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !bPressed)
			return;

		float xDiff = e.x - pX;
		float yDiff = e.y - pY;
		x += xDiff;
		y += yDiff;
		pX = e.x;
		pY = e.y;

		if(callbackPosDiff != NULL && objs2CallPosDiff.size()>0){
			callbackPosDiff(objs2CallPosDiff,xDiff,yDiff);
		}
		if(callbackPos != NULL && objs2CallPos.size()>0){
			callbackPos(objs2CallPos,x,y);
		}
	}

	virtual void mouseReleased(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !bPressed)
			return;

		bPressed = false;
	}

	virtual void saveToXml(ofxXmlSettings & xml){
		xml.setValue("X",x);
		xml.setValue("Y",y);
	}

	virtual void loadFromXml(ofxXmlSettings & xml){
		x = xml.getValue("X",0);
		y = xml.getValue("Y",0);
	}

	//CALLBACK functions
	void setCallbackFunc(void (*callbackFunc)(vector<void *>,float,float) ){
		callbackPosDiff = callbackFunc;
	}
	void addCallbackListenerPosDiff(void * objToCall){
		objs2CallPosDiff.push_back(objToCall);
	}
	void removeCallbackListenerPosDiff(void * objToRemove){
		vector<void*>::iterator it;
		for(it=objs2CallPosDiff.begin();it<objs2CallPosDiff.end();++it){
			if(*it == objToRemove){
				objs2CallPosDiff.erase(it);
				break;
			}
		}
	}
	static void callbackToMoveWrapper(vector<void *> objectsToCall,float xdiff,float ydiff){
		for(int i=0;i<objectsToCall.size();++i){
			ofxGrabbableObject * obj = (ofxGrabbableObject*)(objectsToCall[i]);
			obj->moveByCallback(xdiff,ydiff);
		}
	}
	void moveByCallback(float xdiff, float ydiff){
		x += xdiff;
		y += ydiff;
	}
};

class ofxGrabbableYFix:public ofxGrabbableObject{
protected:
public:
	float resetY;
	virtual void setup(float x,float y,float w,float h){
		ofxGrabbableObject::setup(x,y,w,h);
		resetY = y;
	}

	virtual void mouseDragged(ofMouseEventArgs &e){
		ofxGrabbableObject::mouseDragged(e);
		if (!bGrabbingEnabled || !bPressed)
			return;
		y = resetY;
	}
};

class toggleButton:public ofxGrabbableObject{//TODO vererbung hier krasses overhead - nach Q3 aufräumen!
protected:
public:
	float resetY;
	bool bActive;
	virtual void setup(float x,float y,float w,float h){
		ofxGrabbableObject::setup(x,y,w,h);
//		unregisterGrabbing(); war nur für timewindow sinnvoll
		resetY = y;
	}

	virtual void mousePressed(ofMouseEventArgs &e){
		ofxGrabbableObject::mousePressed(e);

		if (!isOver(e.x, e.y)){
			return;
		}
		bActive = !bActive;
	}

	virtual void draw(){
		fillMe = bActive;
		ofxGrabbableObject::draw();
	}
};

#endif
