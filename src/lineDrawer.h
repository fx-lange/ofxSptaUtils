#ifndef _LINEDRAWER_H_
#define _LINEDRAWER_H_

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ParticleSystem.h"
#include "maxima.h"
#include "stringToPoints.h"

enum tpcMode{
	MOVE,
	READ,
	FADEOUT
};

class textParticleContainer{
public:
	textParticleContainer() {
		alpha = 0;
		mode = MOVE;
		tLastUpdate = -1;
	}

	void die(){
		for(int i=0;i<particles.size();++i){
			particles[i]->killSoft();//TODO oder free?
		}
	}

	void fadeOut(float zStep){
		for(int i=0;i<particles.size();++i){
			particles[i]->z += zStep;
		}
	}

	vector<letterParticle*> particles;
	long timeStamp;
	string text;
	ofRectangle boundingBox;
	ofTrueTypeFont * font;
	float alpha;
	tpcMode mode;
	int tLastUpdate;
};

class lineDrawer {
public:

	lineDrawer(){ }

	void setupGuiPage(ofxSimpleGuiToo * gui);
	void setup(int kParticles);
	void update(vector<ofPoint> & linePoints,ofMatrix4x4 * transformMatrix,float scale);
	void sendText(messageData data);
	void draw();
	void drawParticle();
	void drawDebug();

protected:

	void updateForces(vector<ofPoint> & linePoints);
	void updateMaximas(vector<ofPoint> & linePoints);
	void updateTPCs();
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
	float softFadeOutStep;
	bool bDrawCircles;
	float aperture;
	float maxPointSize;

	float particleDampingForce;
	float particleMaxSpeed;
	float particleSmoothTargetSpeed;

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

	ofMatrix4x4 * transformMatrix;
	float scale; //TODO doppel gemoppelt

	//TPCS
	list<textParticleContainer> tpcs;
	int timeForEachText;
	int timeBeforeFadeout;
	float zStepText;
	float alphaStepOutText;
	float alphaStepInText;
	float switchToReadDistance;
	float moveYMin, moveYMax;

	int frameCount;
};

#endif
