#ifndef __GRABBABLETUNNEL.H__
#define __GRABBABLETUNNEL.H__

#include "ofMain.h"
#include "ofxGrabbablePolyline.h"
#include "ofxAnimationI.h"
#include "ParticleSystem.h"

class ofxGrabbableTunnel : public ofxGrabbablePolyline, public ofxAnimationI{
protected:
	bool run;
	int modi;
	ParticleSystem * particleSystem;
public:
	float radius;
	float scale;

	virtual void setup(float rectSize,float radius, float scale,ParticleSystem * ps){
		ofxGrabbablePolyline::setup(rectSize);
		this->radius = radius;
		this->scale = scale;
		timeGrabber.setup(50,350,15,15);//TODO gui / auslagern
		timeGrabber.color.set(255,0,0);
		timeGrabber.fillMe = true;
		visible.setup(50+15,350,15,15);
		visible.bActive = true;
		visible.color.set(255,0,0);
		particleSystem = ps;
		modi =0;
		run = false;
	}

	void setPoints(ofPolyline line,float simplyfyLevel){
		line.simplify(simplyfyLevel);
		for (int i = 0; i < line.size() - 1 ; i++) {
			addVertex(line[i],&(line[i+1]));
		}
	}

	void addVertex( const ofPoint &p, ofPoint * next)  {
		ofxGrabbableVector * go = new ofxGrabbableVector();
		go->setup(p.x,p.y,rectWidth,rectWidth);

		if(next!=NULL){
			ofVec3f v1(next->x,next->y);
			ofVec3f newLever = ((v1 - *go).getNormalized() * scale)+*go;
			go->lever.x = newLever.x;
			go->lever.y = newLever.y;
		}

		go->setRadius(radius);
//		cout << "push vertex to tunnel" << endl;
		points.push_back(go);
	}
	virtual void start(int modi = 0){
		cout << "startforce tunnel" << endl;
		run = true;
		this->modi = modi;
	}

	virtual void stop(){
		run = false;
	}
	virtual void update(){//TODO werte in GUI
		if(!run)
			return;
		cout << points.size() << endl;
        for(int i=0;i<points.size();++i){
        	ofxGrabbableVector * object = (ofxGrabbableVector*)points[i];
			particleSystem->addDirectedForce(object->x, object->y, object->getRadius(), object->getVectorLength()/200,object->getNormVector());
        }
	}
	virtual void drawGUI(){
		ofPushStyle();
		ofxAnimationI::drawGUI();
		if(visible.bActive){
			ofSetColor(255,255,255,100);
			ofLine(timeGrabber,*points[0]);
			ofxGrabbablePolyline::draw(false);
			setGrabbingRest(visible.isGrabbingEnabled());
		}else{
			setGrabbingRest(false);
		}
		ofPopStyle();
	}
	virtual void drawAnimation(){}//TODO nothing!
	virtual int getModiCount(){ return 2; }//TODO
	virtual void setGrabbing(bool grabbing){
		visible.setGrabbing(grabbing);
		setGrabbingRest(grabbing);
	}

	void setGrabbingRest(bool grabbing){
		ofxAnimationI::setGrabbing(grabbing);
		ofxGrabbablePolyline::setGrabbing(grabbing);
	}

	virtual void saveToXml(ofxXmlSettings & xml){
		ofxAnimationI::saveToXml(xml);
		xml.setValue("Typ","FORCETUNNEL");//TODO über enum lösen!
		xml.addTag("Points");
		xml.pushTag("Points");
		for(int i=0;i<points.size();++i){
			int tagNum = xml.addTag("Point");
			xml.pushTag("Point",tagNum);
			xml.setValue("Id",i);
			points[i]->saveToXml(xml);
			xml.popTag();
		}
		xml.popTag();
	}
};

#endif
