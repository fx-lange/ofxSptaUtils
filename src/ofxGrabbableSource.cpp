#include "ofxGrabbableSource.h"

void ofxGrabbableSource::update(){//TODO radius und zeit für bestimmung wie oft und wieviele!!!
	if(!run)
		return;
	Particle * p = particleSystem->getNext();
	float px = ofRandom(-calcRadius,calcRadius) + x;//TODO es entsteht ein rechteck kein kreis
	float py = ofRandom(-calcRadius,calcRadius) + y;
	p->x = px;
	p->y = py;
	p->xv = p->yv = 0;
	p->resetForce();
}
