#pragma once
class Resource {
private:
	ofVec2f position;
	int targX{ 0 }, targY{ 0 };
public:
	ofColor color{ 255 };
	int type = 0;
	bool dead = false;
	float health;
	int growth = 0;
	Resource(ofVec2f, int, int, int);
	~Resource();
	void initialize();
	bool initialized = false;
	void update();
	void draw();
	float sizeX = 10, sizeY = 10;
	ofVec2f platform;
	ofVec2f platform2;

	const ofVec2f& getPosition();

};