#include "Explosion.h"

Explosion::Explosion(ofVec3f position_, bool type_) {
	position = position_;
	if (type_) {
		img.load("Explosion.png");
		img.resize(100, 100);
	}
	else {
		img.load("Explosion2.png");
		img.resize(70, 70);
	}
	rotation = ofRandom(360);
}

Explosion::~Explosion() {

}

void Explosion::draw() {
	ofPushMatrix();
	ofTranslate(position);
	ofRotate(rotation);
	float scale = lifespan/max_lifespan;
	ofScale(scale/6);
	img.draw(0, 0);
	ofPopMatrix();
	lifespan--;
}

bool Explosion::getIsDead() {
	if (lifespan > 0) {
		return false;
	}
	else {
		return true;
	}
}