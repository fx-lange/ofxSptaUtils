#ifndef __GRABBABLEBEZIER.H__
#define __GRABBABLEBEZIER.H__

#include "ofMain.h"
#include "ofxGrabbableObject.h"

//TODO resolution
//TODO Farbeinstellung
//TODO p1 nur in länge veränderbar
//TODO intuitiver?!
class ofxGrabbableBezier{
protected:
	ofPolyline curve;
	ofPolyline drawing;

	bool bDrawNext;

	toggleButton marker;
	ofxGrabbableObject start,p1,p2,end;
public:
	void setup(){
		//setup grabber and marker
		start.setup(50,50,20,20);
		p1.setup(50,100,20,20);
		p2.setup(500,200,20,20);
		end.setup(400,500,20,20);
		marker.setup(50,150,10,10);
		marker.color.set(0,255,255);
		start.color.set(0,255,255);
		p1.color.set(0,0,255);
		p2.color.set(0,255,0);
		end.color.set(255,0,0);

		//callbacks
		start.setCallbackFunc(ofxGrabbableObject::callbackToMoveWrapper);
		start.addCallbackListenerPosDiff(&p1);
		end.setCallbackFunc(ofxGrabbableObject::callbackToMoveWrapper);
		end.addCallbackListenerPosDiff(&p2);

		bDrawNext = true;
	}

	void update(){
		if(!bDrawNext)
			return;
		drawing.clear();
		drawing.addVertex(start);
		drawing.bezierTo(p1,p2,end,50);
		if(!marker.bActive){
			setBezierGrabbing(false);
		}else{
			setBezierGrabbing(marker.isGrabbingEnabled());
		}
	}

	void draw(){
		ofPushStyle();
		ofSetColor(255);
		curve.draw();
		if(curve.size()>0){
			ofPushStyle();
			ofSetColor(255,255,255,100);
			ofLine(marker,curve[0]); // TODO getFirst von curve wird benötigt - erst wenn der überhaupt einen hat
			ofPopStyle();
		}
		marker.draw();
		if(bDrawNext){
			drawing.draw();
			start.draw();
			p1.draw();
			p2.draw();
			end.draw();
			ofSetColor(50);
			ofLine(start,p1);
			ofLine(end,p2);
		}
		ofPopStyle();
	}

	void addCurve(){
		if(!bDrawNext)
			return;
		vector<ofPoint> & points = drawing.getVertices();
		curve.addVertexes(points);
		start.x = end.x;
		start.y = end.y;
		//p1 spiegel von p2 an end
		ofVec3f newP1 = (end*2 - p2);
		p1.x = newP1.x;
		p1.y = newP1.y;
		//p2 mit zu end
		//end
		end.x += 100;
		p2.x = end.x;
		p2.y = end.y +100;
		start.disableGrabbing();
		drawing.clear();
		bDrawNext = false;
	}

	void createNextCurve(float x,float y){
		end.x = x;
		end.y = y;
		ofVec3f newP2 = (p1 - start)+end;
		p2.x = newP2.x;
		p2.y = newP2.y;
		bDrawNext = true;
	}

	vector<ofVec3f> getVertexes(){
		return curve.getVertices();
	}

	bool isActive(){
		return marker.bActive;
	}

	void deactivate(){
		marker.bActive = false;
	}

	bool isDrawingFinished(){
		return drawing.size() <= 0;
	}

	void setGrabbing(bool bGrabbing){
		marker.setGrabbing(bGrabbing);
		setBezierGrabbing(bGrabbing);
	}

	void setBezierGrabbing(bool bGrabbing){
		start.setGrabbing(bGrabbing);
		end.setGrabbing(bGrabbing);
		p1.setGrabbing(bGrabbing);
		p2.setGrabbing(bGrabbing);
	}

	virtual void saveToXml(ofxXmlSettings & xml){
		xml.addTag("Start");
		xml.pushTag("Start");
		start.saveToXml(xml);
		xml.popTag();
		xml.addTag("End");
		xml.pushTag("End");
		end.saveToXml(xml);
		xml.popTag();
		xml.addTag("P1");
		xml.pushTag("P1");
		p1.saveToXml(xml);
		xml.popTag();
		xml.addTag("P2");
		xml.pushTag("P2");
		p2.saveToXml(xml);
		xml.popTag();
		xml.addTag("Marker");
		xml.pushTag("Marker");
		marker.saveToXml(xml);
		xml.popTag();
		xml.addTag("Points");
		xml.pushTag("Points");
		for(int i=0;i<curve.getVertices().size();++i){
			ofPoint & p = curve.getVertices()[i];
			int tagNum = xml.addTag("Point");
			xml.pushTag("Point",tagNum);
			xml.setValue("Id",i);
			xml.setValue("X",p.x);
			xml.setValue("Y",p.y);
			xml.popTag();
		}
		xml.popTag();

	}
};

#endif
