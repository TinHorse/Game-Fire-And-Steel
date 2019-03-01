#include "ofMain.h"
#include "Resource.h"

Resource::Resource(ofVec2f position_, int type_, int targX_, int targY_) {
	position = position_;
	position.x += 
	type = type_;
	targX = targX_;
	targY = targY_;
	initialize();
}
Resource::~Resource() {}

void Resource::initialize() {
	// lead drills
	if (type == -1) {
		sizeX = ofRandom(20, 30);
		sizeY = sizeX;
		color = ofColor{ 52, 152, 219, 150 };
		health = 10;
		growth = 1;
	}
	// iron forest
	else if (type == -2) {
		sizeX = 7;
		sizeY = sizeX;
		color = ofColor{ 191, 191, 191, 120 };
		health = 20;
		growth = 1;
	}
	// rice
	else if (type == -3) {
		sizeX = 6;
		sizeY = sizeX;
		color = ofColor{ 100, 120 };
		health = 10;
		growth = 1;
	}
	// coal
	else if (type == -4) {
		sizeX = 7;
		sizeY = sizeX;
		color = ofColor{ 40, 150 };
		health = 500;
	}
	// sulphur
	else if (type == -5) {
		sizeX = 5;
		sizeY = sizeX;
		color = ofColor{ 245, 230, 83, 150 };
		health = 500;
	}
	// copper
	else if (type == -6) {
		sizeX = 8;
		sizeY = sizeX;
		color = ofColor{ 180,120,50, 150 };
		health = 500;
	}
	// limestone
	else if (type == -7) {
		sizeX = 25;
		sizeY = sizeX;
		color = ofColor{ 215, 210, 200, 150 };
		health = 5000;
	}
	ofVec2f origin = position;
	position.x += ofRandom(-targX*2, targX*2);
	position.y += ofRandom(-targY*2, targY*2);
	while (position.x > (origin.x - targX / 2) && position.x < (origin.x + targX / 2) && position.y >(origin.y - targY / 2) && position.y < (origin.y + targY/2)) {
		if (sizeX > 15) {
			position += 10;
		}
		else {
			position -= 10;
		}
	}
}

void Resource::update() {
	if (growth > 0) {
		if (health < 100) {
			health += growth;
		}
	}
}

void Resource::draw() {
	ofSetColor(color);
	ofDrawRectangle(position.x - (sizeX / 2), position.y - (sizeY / 2), sizeX, sizeY);
}
const ofVec2f& Resource::getPosition() {
	return position;
}