#pragma once

#include "Particle.h"

class ParticleQ3 : public Particle {
public:
	float d,initD;
	int modus;
	bool bPhase1Done;
	float alphaStep1, compareDistance;
	ofPoint start,middle;

	ParticleQ3(float _x = 0, float _y = 0, float _xv = 0, float _yv = 0):
		Particle(_x,_y,_xv,_yv){
	}

	virtual void setMode(float alphaStep1,float compareDistance,float radius=5,int modi=0){
		bPhase1Done = false;
		this->alphaStep1 = alphaStep1;
		this->compareDistance = compareDistance;
		this->initD = this->d = radius*2;
		start.x = x;
		start.y = y;
		modus = modi;
		alpha = 0;
	}

	virtual void updatePosition(float timeStep) {
		Particle::updatePosition(timeStep);
		if(bFree)
			return;
		if(!bPhase1Done){
			alpha += alphaStep1;
			if(alpha>255){
				bPhase1Done = true;
				middle.x = x;
				middle.y = y;
				compareDistance -= middle.distance(start);
			}
		}else{
			float distance = middle.distance(ofVec3f(x,y));
//			float distance = middle.squareDistance(ofVec3f(x,y));
//			distance = InvSqrt(distance);
			alpha = ofMap(distance,0,compareDistance,255,0);
			if(abs(compareDistance-distance)<10){
//				cout << "d: " << distance << endl;
				setFree(true);
			}
		}
	}

	virtual bool bounceOffWalls(float left, float top, float right, float bottom, float damping = .3) {
		if(Particle::bounceOffWalls(left,top,right,bottom,damping)){
			bFree = true;
			return true;
		}else{
			return false;
		}
	}

	virtual void boost(float radiusBoost){
		if(bFree || radiusBoost<=1)
			return;
		d += radiusBoost*2;

	}

	void resetForce() {
		xf = 0;
		yf = 0;
	}

	virtual void draw(float grey=255) {
		if(bFree || d < 1)
			return;
		ofSetColor(grey,grey,grey,alpha);
		ofEllipse(x, y, d, d);
		d = initD;
	}
};
