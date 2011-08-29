#ifndef __GRABBABLEPOLYLINE.H__
#define __GRABBABLEPOLYLINE.H__

#include "ofxGrabbableObject.h"

//TODO possible to extend ofPolyline?
class ofxGrabbablePolyline {

protected:
	vector<ofxGrabbableObject*> points;
	float rectWidth;

public:

	void setup(float rectSize){
		this->rectWidth = rectSize;
	}

	void draw(bool closed = true){
		ofPushStyle();
		ofSetColor(0,100,160);
		if(points.size()>1){
			int i=1;
			for(;i<points.size();++i){
				ofLine(points[i]->x,points[i]->y,points[i-1]->x,points[i-1]->y);
			}
			if(closed){
				ofLine(points[0]->x,points[0]->y,points[i-1]->x,points[i-1]->y);
			}
		}
		for(int i=0;i<points.size();++i){
			points[i]->draw();
		}
		ofPopStyle();
	}

	void addVertex(float x, float y){
		addVertex(ofPoint(x,y));
	}

	void addVertex(const ofPoint &p){
		ofxGrabbableObject * go = new ofxGrabbableObject();
		go->setup(p,rectWidth,rectWidth);
		//TODO
//		go->setColor();
		points.push_back(go);
	}

	//TODO not really effective - its just a workaround
	std::vector<ofVec2f>& getVertexes(){
		vector<ofVec2f> pts;
		for(int i=0;i<points.size();++i){
			pts.push_back(ofVec2f(points[i]->x,points[i]->y));
		}
		return pts;
	}

	int size(){
		return points.size();
	}

	ofxGrabbableObject* operator[] (int index){
		return points[index];
	}

	void clear(){
		for(int i=0;i<points.size();++i){
			delete points[i];
		}
		points.clear();
	}

	void setGrabbing(bool bGrabbing){
		for(int i=0;i<points.size();++i){
			points[i]->setGrabbing(bGrabbing);
		}
	}

	void toggleGrabbing(){
		for(int i=0;i<points.size();++i){
			points[i]->toggleGrabbing();
		}
	}
};

#endif
