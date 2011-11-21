#ifndef _LINEDRAWER_H_
#define _LINEDRAWER_H_

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ParticleSystem.h"
#include "maxima.h"
#include "stringToPoint.h"

class lineDrawer {
public:

	lineDrawer(){ }

	void setupGuiPage(ofxSimpleGuiPage * gui);
	void setup(int kParticles);
	void update(vector<ofPoint> & linePoints,ofMatrix4x4 & transformMatrix);
	//TODO extra transformation matrix
	void sendText(messageData data);
	void draw();
	void drawParticle();
	void drawDebug();

protected:

	void updateForces(vector<ofPoint> & linePoints);
	void updateMaximas(vector<ofPoint> & linePoints,ofMatrix4x4 & transformMatrix);
	void drawPoints();
	int chooseMaxima();

	list<maxima> maximas;
	vector<ofPoint> noise;
	vector<ofPoint> * pointsPtr;

	//SETTINGS
	ParticleSystem particleSystem;
	int kParticles;
	float particleNeighborhood;
	float particleRepulsion;

	float particleDampingForce;

	float maxDistanceToMaxima;
	int maxMaximaAge;
	int particleMode;
	//mode0
	float maximaAttraction;
	float maximaNeighboorhood;
	//mode1
	float directedForceScale;
	float directedForceRadius;
	float decreaseGradient;
	//mode2
	float lineForceScale;
	float lineForceRadius;
	int lineForceStepSize;
	float decreseLineGradient;

	int particlePerFrameMode;
	float randomRadius;
	//mode0
	int nPerFrame; //TODO nPerSec (siehe Q3)
	//mode1 => nPerFrame = (int)ofMap(amplitude,0,maxAmplitude,0,nPerFrame)
	float maxAmplitude;

	//Design?!
	float ampliDiff; //TODO more!
};

#endif
