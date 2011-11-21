#pragma once

#include "Particle.h"

class letterParticle : public Particle{
public:
	ofPoint target;
	bool bMoveToTarget;

	float * maxSpeed;
	float * smoothSpeed;
	float * softKillStepSize;
	float * aperture;
	float * maxPointSize;

	letterParticle(float _x = 0, float _y = 0, float _xv = 0, float _yv = 0) :
		Particle(_x,_y,_xv, _yv) {
		bMoveToTarget = false;
		bKillSoft = false;
	}

	void setSettingsPointer( float * maxSpeed, float * smoothSpeed,float * softKillStepSize,float * aperture, float * maxPointSize){
		this->maxSpeed = maxSpeed;
		this->smoothSpeed = smoothSpeed;
		this->softKillStepSize = softKillStepSize;
		this->aperture = aperture;
		this->maxPointSize = maxPointSize;
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

	virtual void draw(float grey = 255) {
		if(bFree){
			return;
		}
		ofSetColor(grey,grey,grey,alpha);//TODO include z
		ofCircle(x,y,radius);
	}

	virtual void drawVertex(){
		if(bFree){
			return;
		}
		float opacity = 1;
		float size = 0;
		z = ofRandom(20);

		if (z!=0){
			size = ofRandom(20) * *aperture + 1.;
			size = min(size, *maxPointSize);
			float r = size / 2.f;
			opacity = 1 / (PI * r * r);
		}

		glPointSize(2+size);
		glBegin(GL_POINTS);
		ofSetColor(255,255,255,alpha*opacity);
		glVertex2f(x,y);
		glEnd();
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
