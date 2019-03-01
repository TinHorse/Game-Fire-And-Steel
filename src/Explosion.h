#pragma once
#include "ofMain.h"

class Explosion {
private:
	ofVec3f position;
	int lifespan = int(ofRandom(15,30));
	int max_lifespan = lifespan / 4;
	ofImage img;
	float rotation;
public:
	Explosion(ofVec3f position_, bool type_);
	~Explosion();
	void draw();
	bool getIsDead();
};