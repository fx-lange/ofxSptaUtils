#pragma once

#include "Particle.h"

class letterParticle : public Particle{
public:
	ofPoint target;
	bool bMoveToTarget;

	letterParticle(float _x = 0, float _y = 0, float _xv = 0, float _yv = 0) :
		Particle(_x,_y,_xv, _yv) {
		bMoveToTarget = false;
	}

	virtual void updatePosition(float timeStep) {
		if(bFree)
			return;

		Particle::updatePosition(timeStep);

		if(!bMoveToTarget)
			return;

		float smooth = 0.05;//source->movedSpeedFactor; //TODO GUI
		float dx,dy;
		dx = (target.x - x)* smooth;
		dy = (target.y - y)* smooth;

//		if(abs(dx+dy)>source->maxSpeed){ //TODO GUI
//			float fac = source->maxSpeed / (float)(abs(dx+dy));
//			dx *= fac;
//			dy *= fac;
//		}

		x += dx;
		y += dy;
	}

	void setTarget(float x,float y){
		bMoveToTarget = true;
		target.set(x,y);
	}

	void removeTarget(){
		bMoveToTarget = false;
	}

};
