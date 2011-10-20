#pragma once

#include "ofMain.h"
#include "ofGraphics.h"

class Particle {
public:
	float x, y;
	float xv, yv;
	float xf, yf;

	Particle(float _x = 0, float _y = 0, float _xv = 0, float _yv = 0) :
			x(_x), y(_y), xv(_xv), yv(_yv) {
	}
	virtual void updatePosition(float timeStep) {
		// f = ma, m = 1, f = a, v = int(a)
		xv += xf;
		yv += yf;
		x += xv * timeStep;
		y += yv * timeStep;
	}
	void resetForce() {
		xf = 0;
		yf = 0;
	}
	void bounceOffWalls(float left, float top, float right, float bottom, float damping = .3) {
		bool collision = false;

		if (x > right) {
			x = right - 1;
			xv *= -1;
			collision = true;
		} else if (x < left) {
			x = left + 1;
			xv *= -1;
			collision = true;
		}
		if (y > bottom) {
			y = bottom -1;
			yv *= -1;
			collision = true;
		} else if (y < top) {
			y = top +1;
			yv *= -1;
			collision = true;
		}

		if (collision == true) {
			xv *= damping;
			yv *= damping;
		}
	}
	void addDampingForce(float timeStep, float damping = .01) {
		xf = xf - xv * damping;
		yf = yf - yv * damping;
	}
	virtual void draw() {
		glVertex2f(x, y);
//		ofEllipse(x, y, 6, 6);

	}
};

inline float InvSqrt(float x) {
	float xhalf = 0.5f * x;
	int i = *(int*) &x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*) &i; // convert new bits into float
	x = x * (1.5f - xhalf * x * x); // One round of Newton's method
	return x;
}
