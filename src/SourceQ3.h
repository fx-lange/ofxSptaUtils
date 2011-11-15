#ifndef _SOURCE.H_
#define _SOURCE.H_

struct SettingSourceQ3{
	//PARTICLE
	float particleRadius0, percemtPerFrame0, particleAlphaStep0;
	float particleRadius1, percemtPerFrame1, particleAlphaStep1;

	//TAIL
	float tailLength,tailResolution;//TODO evtl als voreisntellung
	float maxCircleDistance;
	bool bCloseTail;
	float lineWidth;
	//resolution des tails & maxcircle <- fix
	//tailLength dynamisch

	//FORCETUNNEL
	float forceFactor;// <<- fix 0.01 - 0.001
	float radiusFT, scaleFT;
	float simplifyLevel;
};
#endif
