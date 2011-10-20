#include "ofxGrabbableSource.h"

void ofxGrabbableSource::update(){//TODO radius und zeit für bestimmung wie oft und wieviele!!!
	if(!run)
		return;
	float alphaStep;
	float percentPerFrame;
	float radius;
	if(modi == 0){
		alphaStep = settings->particleAlphaStep0;
		percentPerFrame = settings->percemtPerFrame0;
		radius = settings->particleRadius0;
	}else{
		alphaStep = settings->particleAlphaStep1;
		percentPerFrame = settings->percemtPerFrame1;
		radius = settings->particleRadius1;
	}
	radius = radius>1 ? radius : 1;
	float particleArea = radius * radius * PI;
	int particlePerFrame = ceil(percentPerFrame / (1.f/(calcArea/particleArea)));

	for(int i=0;i<particlePerFrame;++i){
		ParticleQ3 * p;
		do{
			p = (ParticleQ3*)particleSystem->getNext();
		}while(!p->bFree);
		float px = ofRandom(-calcRadius,calcRadius) + x;//TODO es entsteht ein rechteck kein kreis
		float py = ofRandom(-calcRadius,calcRadius) + y;
		p->x = px;
		p->y = py;
		p->setFree(false);
		p->setMode(alphaStep,getVectorLength(),radius);
		p->xv = p->yv = 0;
		p->resetForce();
	}
}
