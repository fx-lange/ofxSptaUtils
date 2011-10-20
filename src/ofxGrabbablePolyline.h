#ifndef __GRABBABLEPOLYLINE.H__
#define __GRABBABLEPOLYLINE.H__

#include "ofxGrabbableObject.h"

//TODO possible to extend ofPolyline! need copyconstructor
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
//		cout << "grabbable poly draw" << points.size() << endl;
		for(int i=0;i<points.size();++i){
//			cout << i << endl;
			points[i]->draw();
		}
		ofPopStyle();
	}

	void addVertex(float x, float y){
		addVertex(ofPoint(x,y));
	}

	void addVertex( const ofPoint &p)  {
		ofxGrabbableObject * go = new ofxGrabbableObject();
		go->setup(p,rectWidth,rectWidth);
		points.push_back(go);
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

//TODO statt nur drawing und *this könnte es auch aus vielen stücken zusammen gesetzt werden
// dann *this statt drawing und nen extra vektor
// wäre auch für bezier gut - für rückgängig!!!
//TODO sehr ähnliches verhalten und sinn zu grabbable bezier
class ofxPolyPainter : public ofPolyline{
protected:
	toggleButton marker;
	ofPolyline drawing;
	bool isPressed;
public:
	void setup(){
		marker.setup(10,300,10,10);
		marker.color.set(255,0,255);
		isPressed = false;
	}

	void pressedFirst(float x,float y){
		if(!isActive() || marker.isPressed())
			return;
		drawing.clear();
		drawing.addVertex(x, y);
		isPressed = true;
	}

	void paint(float x,float y){
		if(isPressed){
			drawing.addVertex(x, y);
		}
	}

	void finishPainting(float simplifyLevel){
		if(!isActive() || marker.isPressed()){
			isPressed = false;
		}
		if(!isPressed )
			return;
		drawing.setClosed(false);
//		drawing.simplify(6); woanders nicht hier

		clear();
		addVertexes(drawing.getVertices());

		drawing.clear();
		isPressed =false;
		marker.bActive = false;
	}

	virtual void draw(){
		ofPushStyle();
		ofSetColor(255);
		ofPolyline::draw();
		marker.draw();
		if(drawing.size()>0){
			ofSetColor(159);
			drawing.draw();
		}
		ofPopStyle();
	}

	bool isActive(){
		return marker.bActive;
	}

	void deactivate(){
		marker.bActive = false;
	}

	void setGrabbing(bool grabbing){
		marker.setGrabbing(grabbing);
	}

	void setClicking(bool grabbing){
		marker.setClicking(grabbing);
	}


	virtual void saveToXml(ofxXmlSettings & xml){

		xml.addTag("Marker");
		xml.pushTag("Marker");
		marker.saveToXml(xml);
		xml.popTag();
		xml.addTag("Points");
		xml.pushTag("Points");
		for(int i=0;i<getVertices().size();++i){
			ofPoint & p = getVertices()[i];
			int tagNum = xml.addTag("Point");
			xml.pushTag("Point",tagNum);
			xml.setValue("Id",i);
			xml.setValue("X",p.x);
			xml.setValue("Y",p.y);
			xml.popTag();
		}
		xml.popTag();

	}

	virtual void loadFromXml(ofxXmlSettings & xml){
		xml.pushTag("Marker");
		marker.loadFromXml(xml);
		marker.bActive = false;
		xml.popTag();
		xml.pushTag("Points");

		for(int i=0;i<xml.getNumTags("Point");++i){
			ofPoint p;
			xml.pushTag("Point",i);
			p.x = xml.getValue("X",0);
			p.y = xml.getValue("Y",0);
			addVertex(p);
			xml.popTag();
		}
		xml.popTag();
	}
};

#endif
