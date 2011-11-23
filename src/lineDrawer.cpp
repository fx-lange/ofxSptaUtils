#include "lineDrawer.h"

void lineDrawer::setup(int kParticles){
	/** -- PARTICLE -- **/
		int binPower = 3;
		particleSystem.setup(ofGetWidth(), ofGetHeight(), binPower);
		particleSystem.particles.reserve(kParticles*1000);
		float padding = 0;
		for(int i = 0; i < kParticles * 1000; i++) {
			float x = ofRandom(padding, ofGetWidth() - padding);
			float y = ofRandom(padding, ofGetHeight() - padding);
			letterParticle * particle= new letterParticle(x,y);
			particle->setFree(true);
			particle->setSettingsPointer(&particleMaxSpeed,&particleSmoothTargetSpeed,&softFadeOutStep,&aperture,&maxPointSize);
			particle->alpha = 255;
//			particle->xv = 0.3;
			particleSystem.add(particle);
		}
}

void lineDrawer::setupGuiPage(ofxSimpleGuiToo * gui){

	gui->addPage("LineDrawer 1");
	gui->addTitle("Particle");
	gui->addSlider("Neighborhood",particleNeighborhood,0,10);
	gui->addSlider("Repulsion",particleRepulsion,-20,20);
	gui->addSlider("DumpingForce",particleDampingForce,0,0.5);

	gui->addSlider("Soft Fade Out StepSize",softFadeOutStep,0,10);
	gui->addToggle("Draw as Circle",bDrawCircles);

	gui->addTitle("Text").newColumn = true;


	gui->addTitle("Maximas");
	gui->addSlider("Amplitude Difference", ampliDiff, 0, 1);
	gui->addSlider("Max Distance Max",maxDistanceToMaxima,20,500);
	gui->addSlider("Max Maxima Age",maxMaximaAge,1,100);
	gui->addSlider("Particle Mode",particleMode,0,2);

	gui->addTitle("Mode 0 - maxima attraction");
	gui->addSlider("Maxima Radius",maximaNeighboorhood,0,100);
	gui->addSlider("Maxima Attraction",maximaAttraction,-20,20);
	gui->addTitle("Mode 1 - maxima forces");
	gui->addSlider("Force",directedForceScale,0,20);
	gui->addSlider("Radius",directedForceRadius,0,100);
	gui->addSlider("DecreaseGradient",decreaseGradient,0,1);
	gui->addTitle("Mode 2 - line forces");
	gui->addSlider("LineForce",lineForceScale,0,10);
	gui->addSlider("LineRadius",lineForceRadius,0,50);
	gui->addSlider("Line Stepsize",lineForceStepSize,1,30);
	gui->addSlider("Line DecreaseGradient",decreseLineGradient,0,1);

	gui->addTitle("Particle per Unit").newColumn = true;
	gui->addSlider("randomRadius",randomRadius,0,20);
	gui->addSlider("per Unit Mode",particlePerFrameMode,0,1);
	gui->addTitle("Mode 0");
	gui->addSlider("per Unit",nPerFrame,0,10);
	gui->addTitle("Mode 1");
	gui->addSlider("max Amplitude",maxAmplitude,0,ofGetHeight()/2);

	gui->addPage("LineDrawer - Text");
	gui->addTitle("Movement");
	gui->addSlider("Max Target Speed",particleMaxSpeed,0,30);
	gui->addSlider("Smooth Target Speed",particleSmoothTargetSpeed,0,0.1);
	gui->addSlider("isClose Distance",switchToReadDistance,0,50);
	gui->addSlider("Move Y Min",moveYMin,0,500);
	gui->addSlider("Move Y Max",moveYMax,100,500);
	gui->addTitle("Read");
	gui->addSlider("how long ms",timeForEachText,1000,30000);
	gui->addSlider("time to read",timeBeforeFadeout,500,10000);
	gui->addTitle("Fadeout");
	gui->addSlider("Text Z Step",zStepText,0,5);
	gui->addSlider("Text Alpha IN Step",alphaStepInText,0,10);
	gui->addSlider("Text Alpha OUT Step",alphaStepOutText,0,10);
	gui->addSlider("Aperture",aperture,0,0.15);
	gui->addSlider("Max Point Size",maxPointSize,1,100);
}

void lineDrawer::update(vector<ofPoint> & linePoints, ofMatrix4x4 * transformMatrix, float scale){
	this->transformMatrix = transformMatrix;
	this->scale = scale;
	/** -- PARTICLE -- **/
		particleSystem.setTimeStep(0.5);
		particleSystem.setupForces();
		//ADD DUMPING TODO
		for(int i = 0; i < particleSystem.size(); i++) {
			Particle& cur = particleSystem[i];
			if(cur.bFree || cur.bNoForce)
				continue;
			// global force on other particles
			particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
			// forces on this particle
//			cur.bounceOffWalls(0, 0, ofGetWidth(), ofGetHeight());
			cur.addDampingForce(1, particleDampingForce);//TODO
		}

	pointsPtr = &linePoints;

	updateTPCs();
	updateMaximas(linePoints);
	updateForces(linePoints);

	particleSystem.update(false);

    frameCount = (frameCount + 1) % 120;
    if(frameCount==0){
    	ofLog(OF_LOG_NOTICE,"%d Particle frei",particleSystem.nFree);
    }
}

void lineDrawer::updateTPCs(){
	if(tpcs.size()>0){
		textParticleContainer & tpc = tpcs.front();
		if(ofGetSystemTime() - tpc.timeStamp > (long)timeForEachText ){
			tpc.die();
			tpcs.pop_front();
		}

		int now = ofGetSystemTime();
		list<textParticleContainer>::iterator it = tpcs.begin();
		for(;it!=tpcs.end();++it){
			//move
			if(it->mode == MOVE){
				//if close (<switchToReadDistance) it->mode = READ -set updateTime = now
				int nClose = 0;
				for(int ii=0;ii<10;++ii){
					int idx = ofRandom(it->particles.size()-1);
					if(it->particles[idx]->getDistanceToTarget()<switchToReadDistance){
						++nClose;
					}
				}
				if(nClose>=5){
					ofLog(OF_LOG_NOTICE,"READ");
					it->mode = READ;
					it->tLastUpdate = now;
				}
			}
			//read
			else if(it->mode == READ){
				it->alpha += alphaStepInText;
				it->alpha = min(it->alpha,255.f);
				//if time over -> it->mode = FADEOUT
				if(now - it->tLastUpdate > timeBeforeFadeout){
					it->mode = FADEOUT;
					ofLog(OF_LOG_NOTICE,"FADEOUT");
					cout << now - it->tLastUpdate << endl;
				}
			}
			//fadeout
			else if(it->mode == FADEOUT){
				it->fadeOut(zStepText);
				it->alpha -= alphaStepOutText;
			}
		}
	}
}

void lineDrawer::updateMaximas(vector<ofPoint> & linePoints){
	//increase old and kill old maximas
	list<maxima>::iterator it;
	for(it=maximas.begin();it!=maximas.end();){
		maxima & m = *it;
		if(m.old > maxMaximaAge){
			//m.die();
			ofLog(OF_LOG_VERBOSE,"Size vor: %d",maximas.size());
			it = maximas.erase(it);
			ofLog(OF_LOG_VERBOSE,"Size nach: %d",maximas.size());
		}else{
			m.increaseOld();
			it++;
		}
	}

	int particlePerUnit = nPerFrame;
	for(int i = 1; i < linePoints.size() - 1; ++i){
		ofPoint & linePointRaw = linePoints[i];
		ofPoint linePoint =(linePointRaw* (*transformMatrix));
//		ofPoint & nextLinePoint = linePoint[i+1];
		if( (linePointRaw.y >= linePoints[i-1].y && linePointRaw.y >= linePoints[i+1].y)
				|| (linePointRaw.y <= linePoints[i-1].y && linePointRaw.y <= linePoints[i+1].y)){

			//MAXIMA FOUND
			maxima * maximaPtr = NULL;

			//TODO REVISIT immoment wird der erste genommen. evtl sollte das "nächste" genommen werden (+ begrenzung)
			bool createNewMaxima = true;
			list<maxima>::iterator it;
			for(it=maximas.begin();it!=maximas.end();it++){
				maxima & m = *it;
				if(m.isNear(linePoint.x,linePoint.y,maxDistanceToMaxima)){
					m.updatePosition(linePoint.x,linePoint.y);
					createNewMaxima = false;
					maximaPtr = &(*it);
					break;
				}
			}

			if(createNewMaxima){
				ofLog(OF_LOG_VERBOSE,"new Maxima (Count:%d)",maximas.size());
				//sorted insert
				maxima m(linePoint.x,linePoint.y);
				for(it=maximas.begin();it!=maximas.end();it++){
					if(it->x < m.x){
						ofLog(OF_LOG_VERBOSE,"einfügen mitten drin oder am ende");
						it++;
						it = maximas.insert(it,m);
						maximaPtr = &(*it);
						break;
					}
				}
				if(maximaPtr==NULL){
					ofLog(OF_LOG_VERBOSE,"einfügen am anfang");
					it = maximas.insert(maximas.begin(),m);
					maximaPtr = &(*it);
				}
			}

			if(particlePerFrameMode==1){
				particlePerUnit = (int)ofMap(abs(linePoint.y),0,maxAmplitude,0,nPerFrame);
			}
			for(int pi=0;pi<particlePerUnit;++pi){
				letterParticle * particle = (letterParticle*)particleSystem.getNext();
				particle->setFree(false);
				particle->x = linePoint.x+ofRandom(-randomRadius,+randomRadius);
				particle->y = linePoint.y+ofRandom(-randomRadius,+randomRadius);
				particle->xv = 0;
				particle->yv = 0;
//				particle->alpha=255;
				maximaPtr->addParticle(particle);
			}
		}
	}
}

void lineDrawer::updateForces(vector<ofPoint> & linePoints){
	if(particleMode==0){
		list<maxima>::iterator it;
		for(it=maximas.begin();it!=maximas.end();it++){
			ofPoint & p = *it;// global force on other particles
			particleSystem.addAttractionForce(p, maximaNeighboorhood, maximaAttraction );
		}
	}else if(particleMode==1 && maximas.size()>1){
		float angle;
		float angleDiff;
		float rotateAngle;
		list<maxima>::iterator it=maximas.begin();
		list<maxima>::iterator next = maximas.begin();
		advance(next,1);
		for(it=maximas.begin();it!=maximas.end();it++,next++){
			ofPoint & p = *it;// global force on other particles
			ofPoint & nextP = *next;
			ofPoint normDirection = nextP-p;//TODO stürzt hier bei extremer Frequenz ab - wieso?
			normDirection.normalize();
			angle = normDirection.angle(ofVec3f(0,-1));
			if(angle > 90){
				angleDiff = -(angle-90);
			}else{
				angleDiff = +(90-angle);
			}
			rotateAngle = angleDiff * decreaseGradient;
			normDirection.rotate(0,0,rotateAngle);
			particleSystem.addDirectedForce(p.x,p.y, directedForceRadius, directedForceScale, normDirection);
		}
		//TODO sonderfall letzter maxima
	}else{
		ofPoint p,next;
		float angle;
		float angleDiff;
		float rotateAngle;
		for(int i = 0; i < linePoints.size() - lineForceStepSize; i+=lineForceStepSize){
			p.set(linePoints[i].x,linePoints[i].y);
			p.set(linePoints[i+lineForceStepSize].x,linePoints[i+lineForceStepSize].y);
			ofPoint normDirection = next-p;//TODO stürzt hier bei extremer Frequenz ab - wieso?
			normDirection.normalize();
			angle = normDirection.angle(ofVec3f(0,-1));
			if(angle > 90){
				angleDiff = -(angle-90);
			}else{
				angleDiff = +(90-angle);
			}
			rotateAngle = angleDiff * -decreseLineGradient;
			normDirection.rotate(0,0,rotateAngle);
			particleSystem.addDirectedForce(p.x,p.y, lineForceRadius, lineForceScale, normDirection);
		}
	}
}

void lineDrawer::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofSetColor(100, 100, 100);



	glPointSize(1);
	drawPoints();

	ofPushMatrix();
	ofScale(1, 1 + ampliDiff, 1);
	ofSetColor(190, 50, 30, 150);

	glPointSize(1);
	drawPoints();

//	ofSetColor(255, 70, 50, 70); //TODO noise?!
//	glPointSize(2);
//	drawPoints(noiseY);
	ofPopMatrix();

	ofPushMatrix();
	ofScale(1, 1 - ampliDiff, 1);
	ofSetColor(190, 50, 30, 150);
	glPointSize(1);
	drawPoints();

//	ofSetColor(255, 70, 50, 70); //TODO noise?!
//	glPointSize(2);
//	drawPoints(noiseY);
	ofPopMatrix();

	ofPopStyle();
	ofPopMatrix();
}

void lineDrawer::drawParticle(){
	//	ofSetRectMode(OF_RECTMODE_CENTER);
	glPointSize(1);
	list<textParticleContainer>::iterator it = tpcs.begin();
	for(;it!=tpcs.end();++it){
		if(it->mode == MOVE){
			continue;
		}
		ofSetColor(255,255,255,it->alpha);
		it->font->drawString(it->text,it->boundingBox.x,it->boundingBox.y);
	}
	glPointSize(2);
	particleSystem.draw(bDrawCircles);
}

void lineDrawer::drawDebug(){
	//draw maxima
	ofSetColor(25, 80, 120, 70);
	list<maxima>::iterator it;
	for(it=maximas.begin();it!=maximas.end();it++){
		maxima & m = *it;
		if(m.bAlive)
		ofCircle(m.x, m.y, 20);
	}
}

void lineDrawer::drawPoints(){
	glBegin(GL_POINTS);
	vector<ofPoint>::iterator it;
	for(it = pointsPtr->begin(); it != pointsPtr->end(); it++){
		ofPoint & p = *it;
		glVertex2f(p.x, p.y);
	}
	glEnd();
}

int lineDrawer::chooseMaxima(){
	return 1;
}

void lineDrawer::sendText(messageData data){
	textParticleContainer tpc;
	tpc.timeStamp = ofGetSystemTime();
	tpc.text = data.text;
	tpc.boundingBox = data.boundingBox;
	tpc.font = data.font;

	ofLog(OF_LOG_VERBOSE,"%d particle benoetigt",data.points.size());
	int maximaIdx = chooseMaxima();//TODO könnte auch gleich den iterator zurück geben

	list<maxima>::iterator maximaIt = maximas.begin();
	advance(maximaIt,maximaIdx);
	maxima & m = *maximaIt;

	ofLog(OF_LOG_VERBOSE,"maxima hat %d particle",m.particles.size());

	int countDiff = data.points.size() - m.particles.size();
	if(countDiff > 0){
		m.particles.reserve(m.particles.size()+countDiff);
		for(int i=0;i<countDiff;++i){
			letterParticle * particle = (letterParticle *)particleSystem.getNext();
			particle->setFree(false);
			particle->x = m.x+ofRandom(-randomRadius,+randomRadius);
			particle->y = m.y+ofRandom(-randomRadius,+randomRadius);
			particle->xv = 0;
			particle->yv = 0;
			particle->alpha=255;
			m.addParticle(particle);
		}
	}
	ofMatrix4x4 inverse = transformMatrix->getInverse();
	ofPoint reTrans = m * inverse;
	float move = ofRandom(moveYMin,moveYMax);
	float vorz;
	if(reTrans.y > 0){
		vorz = 1;
	}else{
		vorz = -1;
	}

	for(int i=0;i<data.points.size();++i){
		letterParticle * p = m.particles[i];
		ofPoint target = data.points[i]+m;

		target.x -= data.boundingBox.width / 2.f;
		target.y += vorz * move;
		p->setTarget(target.x,target.y);
		tpc.particles.push_back(p);

	}
	tpc.boundingBox.x = m.x - tpc.boundingBox.width/2.f;
	tpc.boundingBox.y = m.y +vorz * move;
	tpcs.push_back(tpc);
	m.particles.clear(); //No more binding needed //TODO revisit - sure?
}
