#include "ofxTailAnimation.h"

virtual void ofxTailAnimation::drawGUI(){
	ofPushStyle();
	ofxAnimationI::drawGUI();

	if(size>3 && visible.bActive){
		if(bClicked){
			calcNearest();
		}
		ofSetColor(255,255,255,100);
		ofLine(x,y,pts[startIdx].x,pts[startIdx].y);
		ofLine(timeGrabber,*this);
		ofSetColor(100);
		if(!run){
			ofEllipse(pts[idx],10,10);
			idx = (idx+1)%size;
		}
		ofSetColor(255,0,0);
		ofNoFill();
		ofEllipse(pts[startIdx],10,10);
		setGrabbingRest(visible.isGrabbingEnabled());
	}else if(!visible.bActive){
		setGrabbingRest(false);
	}

	ofEnableAlphaBlending();
	if(bClicked){
		ofSetColor(color,255);
	}else{
		ofSetColor(color,100);
	}
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofRect(x,y,width,height);
	ofDisableAlphaBlending();
	ofPopStyle();
}

virtual void ofxTailAnimation::drawAnimation(){
	if(size>3 && run){
		for(int i=tailIdx;i<=idx;i++){
			ofEllipse(pts[i],10,10);
		}
		idx = (idx+1)%size;
		if(abs(idx-tailIdx)>tailLength){
			tailIdx = (tailIdx+1)%size;
		}
	}
}

void ofxTailAnimation::setAndArrangePoints(vector<ofPoint> points,bool close){
	if(points.size()<=0)
		return;
	size = points.size();
	pts.clear();
	pts.reserve(size);
	idx = 0;
	drawPoint = points[idx];
	while(idx<size-1){
		while(true){
			ofVec3f & next = points[idx+1];

			float distance = drawPoint.distance(next);
			if(distance > tmpStepSize){
				drawPoint = (next-drawPoint).getNormalized()*tmpStepSize + drawPoint;
				pts.push_back(drawPoint);
				tmpStepSize = stepSize;
				break;
			}else{
				drawPoint = next;
				tmpStepSize -= distance;
				idx++;
			}
		}
	}
	//schliessen
	if(close){
		ofVec3f & next = points[0];
		float distance = drawPoint.distance(next);
		while(distance>tmpStepSize){
			drawPoint = (next-drawPoint).getNormalized()*tmpStepSize + drawPoint;
			pts.push_back(drawPoint);
			distance -= tmpStepSize;
			tmpStepSize = stepSize;
		}
	}
	size= pts.size();
	tailIdx = idx = startIdx;
}



// ------------------------- protected -------------------------- //


void ofxTailAnimation::calcNearest(){
	int nearestIdx = 0;
	ofVec3f mouse(mouseX,mouseY);
	float minDistance = pts[0].distance(mouse);
	for(int i=1;i<size;++i){
		float disttmp = pts[i].distance(mouse);
		if(disttmp < minDistance){
			minDistance = disttmp;
			nearestIdx = i;
		}
	}
	startIdx = nearestIdx;
}

virtual void ofxTailAnimation::setup(float stepSize){
	startIdx = idx = 0;
	tmpStepSize = this->stepSize = stepSize;
	size = 0;
	modi = 0;
	tailLength = 10;
	tailIdx = 0;
	registerMoving();
	run = false;

}
