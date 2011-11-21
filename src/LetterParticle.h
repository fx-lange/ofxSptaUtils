#pragma once

#include "Particle.h"

class letterParticle : public Particle{
public:
	ofPoint target;
	bool bMoveToTarget;

	float * maxSpeed;
	float * smoothSpeed;
	float * softKillStepSize;

	letterParticle(float _x = 0, float _y = 0, float _xv = 0, float _yv = 0) :
		Particle(_x,_y,_xv, _yv) {
		bMoveToTarget = false;
		bKillSoft = false;
	}

	void setSettingsPointer( float * maxSpeed, float * smoothSpeed,float * softKillStepSize){
		this->maxSpeed = maxSpeed;
		this->smoothSpeed = smoothSpeed;
		this->softKillStepSize = softKillStepSize;
	}

	virtual void updatePosition(float timeStep) {
		if(bFree)
			return;

		Particle::updatePosition(timeStep);

		if(bKillSoft){
			alpha -= *softKillStepSize;
			if(alpha<=0){
				bKillSoft = false;
				setFree(true);
			}
		}

		if(!bMoveToTarget)
			return;

		float smooth = *smoothSpeed;
		float dx,dy;
		dx = (target.x - x)* smooth;
		dy = (target.y - y)* smooth;

		if(abs(dx+dy)> *maxSpeed){
			float fac = *maxSpeed / (float)(abs(dx+dy));
			dx *= fac;
			dy *= fac;
		}

		x += dx;
		y += dy;
	}

	void setTarget(float x,float y){
		bMoveToTarget = true;
		bNoForce = true;
		target.set(x,y);
	}

	void removeTarget(){
		bNoForce = false;
		bMoveToTarget = false;
	}

	virtual void killSoft(){
		bKillSoft = true;
	}

};
