#include "ofxGrabbableVector.h"

virtual void ofxGrabbableVector::draw(){
	ofPushStyle();
	ofEnableAlphaBlending();
	ofNoFill();
	if(bHoverd){
		ofSetColor(colorHover);
	}else{
		ofSetColor(color);
	}
//		lever.draw();
	ofLine(x,y,lever.x,lever.y);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofRect(x,y,width,height);
	ofEllipse(x,y,getRadius()*2,getRadius()*2);
	ofEllipse(radius.x,radius.y,radius.width,radius.height);
	ofFill();
	ofRect(lever.x,lever.y,lever.width,lever.height);
	ofDisableAlphaBlending();
	ofPopStyle();
}

void ofxGrabbableVector::setup(float x,float y, float w, float h){
	ofxGrabbableObject::setup(x,y,w,h);
	bHoverd = false;
	color.set(255,0,0,100);
	colorHover.set(255,100,0,255);
	registerMoving();

	//lever
	lever.setup(x,y-30,8,8);
	lever.disableGrabbing();

	//radius
	radius.setup(x+20,y,8,8);
	lever.disableGrabbing();

	//callback
	setCallbackFunc(ofxGrabbableObject::callbackToMoveWrapper);
	addCallbackListenerPosDiff((void *)&radius);
	addCallbackListenerPosDiff((void *)&lever);
}
