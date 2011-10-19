#ifndef __GRABBABLESQUARE.H__
#define __GRABBABLESQUARE.H__

#include "ofMain.h"

class ofxGrabbableSquare : public ofxGrabbableObject{

protected:
	ofxGrabbableObject * tl, * tr, * bl, * br;
	bool bHoverd;
	bool isInsideCircle(float x,float y){
		float dist = this->distance(ofVec3f(x,y,0));
		return maxDistance()+10>dist;
	}
	float maxDistance(){
		float result = 0.0f;
		float m1 = max(this->distance(*tl),this->distance(*tr));
		float m2 = max(this->distance(*bl),this->distance(*br));
		result = max(m1,m2);
		return result;
	}
public:


	void setup(float x,float y, float w, float h){
		ofxGrabbableObject::setup(x,y,w,h);
		bHoverd = false;
		registerMoving();
		//callback
		setCallbackFunc(ofxGrabbableObject::callbackToMoveWrapper);
	}

	void setTL(ofxGrabbableObject * tl){
		removeCallbackListenerPosDiff(this->tl);
		this->tl = tl;
		addCallbackListenerPosDiff(tl);
	}
	void setTR(ofxGrabbableObject * tr){
		removeCallbackListenerPosDiff(this->tr);
		this->tr = tr;
		addCallbackListenerPosDiff(tr);
	}
	void setBL(ofxGrabbableObject * bl){
		removeCallbackListenerPosDiff(this->bl);
		this->bl = bl;
		addCallbackListenerPosDiff(bl);
	}
	void setBR(ofxGrabbableObject * br){
		removeCallbackListenerPosDiff(this->br);
		this->br = br;
		addCallbackListenerPosDiff(br);
	}

	void update(){
		ofVec3f newCenter = (*tl + *br)/2.f;
		x = newCenter.x;
		y = newCenter.y;
	}

	virtual void draw(){
		update();

		ofPushStyle();

		ofNoFill();
		ofEnableAlphaBlending();
		if(bHoverd){
			ofSetColor(color,150);
		}else{
			ofSetColor(color,100);
		}
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRect(x,y,width,height);

		if(tl != NULL && tr != NULL && bl != NULL && br != NULL){
			tl->draw();
			tr->draw();
			bl->draw();
			br->draw();
			ofFill();
			ofBeginShape();
			ofVertex(*tl);
			ofVertex(*tr);
			ofVertex(*br);
			ofVertex(*bl);
			ofEndShape(true);
		}

		ofDisableAlphaBlending();
		ofPopStyle();
	}

	void registerMoving() {
		ofAddListener(ofEvents.mouseMoved, this, &ofxGrabbableSquare::mouseMoved);
	}

	void mouseMoved(ofMouseEventArgs &e) {
		if (!bGrabbingEnabled || !(tl != NULL && tr != NULL && bl != NULL && br != NULL))
			return;
		if(isOver(e.x, e.y)){
			bHoverd = true;
			tl->enableGrabbing();
			tr->enableGrabbing();
			bl->enableGrabbing();
			br->enableGrabbing();
		}else if(!(bHoverd && isInsideCircle(e.x,e.y))){
			bHoverd = false;
			tl->disableGrabbing();
			tr->disableGrabbing();
			bl->disableGrabbing();
			br->disableGrabbing();
		}
	}
};

class ofxMultipleSquares{
protected:
	vector<ofxGrabbableSquare*> squares;
public:
	void setup(float x, float y,float w, float h){
		ofxGrabbableSquare * square = new ofxGrabbableSquare();
		square->setup(x,y,20,20);
		ofxGrabbableObject * tl, * tr ,* bl,* br;
		tl = new ofxGrabbableObject();
		tl->setup(-w/2,-h/2,10,10);
		tr = new ofxGrabbableObject();
		tr->setup(w/2,-h/2,10,10);
		bl = new ofxGrabbableObject();
		bl->setup(-w/2,h/2,10,10);
		br = new ofxGrabbableObject();
		br->setup(w/2,h/2,10,10);
		square->setTL(tl);
		square->setTR(tr);
		square->setBL(bl);
		square->setBR(br);
		squares.push_back(square);
	}

	void draw(){
		for(int i=0;i<=squares.size();++i){
			squares[i]->draw();
		}
	}
};

#endif
