#ifndef _MAXIMA_H_
#define _MAXIMA_H_

//TODO REVISIT - maximas could be sorted for better performance

#include "ofPoint.h"
#include "LetterParticle.h"

class maxima : public ofPoint{
public:
	maxima(float x,float y)
	:ofPoint(x,y){
		bAlive = true;
		old=0;
		particles.clear();
		particles.reserve(500);
	}

	~maxima(){
		die();
	}

	void addParticle(letterParticle*particle){
		particle->removeTarget();
		particles.push_back(particle);
	}

	bool isNear(ofPoint & other, float maxDistance){
		return isNear(other.x,other.y,maxDistance);
	}

	bool isNear(float x,float y,float maxDistance){
		return bAlive && distance(ofVec3f(x,y)) <= maxDistance;
	}

	void updatePosition(float _x,float _y){
		set(_x,_y);
		old = 0;
	}

	void increaseOld(){
		if(bAlive)
		++old;
	}

	void die(){
		ofLog(OF_LOG_VERBOSE,"maxima at %f/%f died",x,y);
		old = -1;
		bAlive = false;
		for(int i=0;i<particles.size();++i){
			particles[i]->setFree(true);
			//TODO statt free eher sowas wie KILL benutzen
		}
	}
//protected:
	bool bAlive;
	int old;
	vector<letterParticle*> particles;

};

#endif
