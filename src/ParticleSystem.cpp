#include "ofMain.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {}
ParticleSystem::ParticleSystem(ofVec2f pos, int size_) {
	position = pos;
	size = size_;
}
ParticleSystem::~ParticleSystem() {};

ofVec2f ParticleSystem::getPosition() {
	return position;
}

void ParticleSystem::update() {
	particles.push_back(ofVec2f(position));
	particle_lifespans.push_back(ofRandom(30,80));
}
void ParticleSystem::drawSmoke() {
	for (int i = 0; i < particles.size(); i++) {
		ofDrawCircle(particles[i], size);
		particles[i].x += ofRandom(0.2, 0.7);
		particles[i].y -= ofRandom(-2.5, 1);
		particle_lifespans[i]--;
		if (particle_lifespans[i] < 0) {
			particles.erase(particles.begin() + i);
			particle_lifespans.erase(particle_lifespans.begin() + i);
		}
	}
}
