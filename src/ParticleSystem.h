#pragma once

class ParticleSystem{
private:
	ofVec2f position;
public:
	ParticleSystem();
	ParticleSystem(ofVec2f, int);
	~ParticleSystem();
	vector<ofVec2f> particles;
	vector<int> particle_lifespans;
	ofVec2f getPosition();
	void update();
	void drawSmoke();
	int size = 1;

};