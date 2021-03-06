#include "ofxTailAnimation.h"

void ofxTailAnimation::drawGUI(){
	ofPushStyle();
	ofxAnimationI::drawGUI();
	ofPushMatrix();
	ofTranslate(tx,ty);

	if(size>3 && visible.bActive){
		if(bClicked){
			calcNearest();
		}
		ofSetColor(255,255,255,100);
		ofLine(x,y,pts[startIdx].x,pts[startIdx].y);
		ofLine(timeGrabber,*this);
		ofSetColor(100);
		if(!run && !runEnd){
			ofEllipse(pts[idx],10,10);
//			idx = (idx+1)%size;
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
	ofPopMatrix();
	ofPopStyle();
}

void ofxTailAnimation::setColorEffect(int i,int now){
	//ofSetColor(255, 255, 255,200);
	//ofSetColor(255, 0, 255,ofMap(i,0,size,100,255));	
	ofSetColor(255, 255, 255,ofMap(now,tailLength,0,0,255));	
}



void ofxTailAnimation::drawFull(){
//	ofSetColor(255, 255, 255,200);
//	ofEllipse(pts[idx],20,20);
//	ofSetColor(255, 0, 255,200);
//	ofEllipse(pts[tailIdx],20,20);
	
	
	if (tailIdx>=idx && started) {
		for(int i=tailIdx;i<pts.size()-1;i++){
			ofSetColor(255, 255, 255);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
		for(int i=0;i<idx;i++){
			ofSetColor(255, 255, 255);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
	} else {
		for(int i=tailIdx;i<idx;i++){
			ofSetColor(255, 255, 255);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
		started = true;
	}
		
	if (run) {
		if(!looped){
			idx = (idx+1)%size; 
		}
		if (!looped && idx==startIdx) {
			looped=true;
			cout << "looped" << endl;
		}
	}
	
	if(runEnd){
		tailIdx=(tailIdx+1)%size;
		if(tailIdx==((startIdx-1)%size+size)%size){
			runEnd=false;
		}
	}
	

	/*
	if(idx<size-1 && run){
		idx = (idx+1)%size;
	}
	if(runEnd){
		//cout << tailIdx << endl;
		tailIdx=(tailIdx+1)%size;
		if(tailIdx==size-1){
			runEnd=false;
		}
	}
	 */
	
	
}	

void ofxTailAnimation::drawTail(){
	
	/*
	ofSetColor(255, 255, 255,30);
	ofEllipse(pts[idx],20,20);
	ofSetColor(255, 0, 255,30);
	ofEllipse(pts[tailIdx],20,20);
	*/
	
	
	if(tailIdx>idx){
		for(int i=0;i<idx;i++){
			setColorEffect(i,idx-i);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
		
		for(int i=tailIdx;i<size-1;i++){
			setColorEffect(i,size-1-i);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
	} else {
		for(int i=tailIdx;i<idx;i++){
			setColorEffect(i,idx-i);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
	}
	
	if(abs(idx-tailIdx)>tailLength && !(tailIdx<size-1 && runEnd)){
		tailIdx = (tailIdx+1)%size;
	}
	

	if (run) {
		idx = (idx+1)%size;
	} else {
//		if (idx<size-1) {
		if (idx!=startIdx) {
			idx = (idx+1)%size;
			tailIdx = (tailIdx+1)%size;
		}else if(tailIdx!=startIdx) {
			tailIdx = (tailIdx+1)%size;
		} else {
			runEnd = false;
		}
	}

}

void ofxTailAnimation::drawTailMulti(){
	
	/*
	ofSetColor(255, 255, 255,30);
	ofEllipse(pts[idx],20,20);
	ofSetColor(255, 0, 255,30);
	ofEllipse(pts[tailIdx],20,20);
	*/
	
	
	if(tailIdx>idx){
		for(int i=startIdx;i<idx;i++){
			setColorEffect(i,idx-i);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
		
		for(int i=tailIdx;i<size-1;i++){
			setColorEffect(i,size-1-i);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
	} else {
		for(int i=tailIdx;i<idx;i++){
			setColorEffect(i,idx-i);
			ofLine(pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
		}
	}
	

	if(abs(idx-tailIdx)>tailLength && !(tailIdx<size-1 && runEnd)){
		tailIdx = (tailIdx+1)%size;
	}
	
	
	if (run) {
		idx = (idx+1)%size;
	} else {
		if (idx<size-1) {
			idx = (idx+1)%size;
		}
		if (tailIdx<size-1) {
			tailIdx = (tailIdx+1)%size;
		}
	}
	
}




void ofxTailAnimation::drawAnimation(){
	ofPushStyle();
	ofSetLineWidth(settings->lineWidth);
	ofPushMatrix();
	ofTranslate(tx,ty);
	
	if(size>3  && (run || runEnd)){
		if (modi==0) {
			drawTail();
		} else if (modi==1) {
			drawFull();
		} else if (modi==2) {
			drawTailMulti();
		}
		
		//sidx++;
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxTailAnimation::setAndArrangePoints(vector<ofPoint> points){
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
	if(settings->bCloseTail){
		ofVec3f & next = points[0];
		float distance = drawPoint.distance(next);
		while(distance>tmpStepSize && distance < settings->maxCircleDistance){
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

void ofxTailAnimation::setup(float stepSize){
	startIdx = idx = 0;
	tmpStepSize = this->stepSize = stepSize;
	size = 0;
	modi = 0;
	tailLength = 10;
	tailIdx = 0;
	registerMoving();
	run = false;

}
