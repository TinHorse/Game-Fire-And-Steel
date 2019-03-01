#include "ofMain.h"
#include "Turret.h"

Turret::Turret() {
	isTurret = false;
};
Turret::Turret(ofVec2f pos, int type_, int unit_type_, bool friendly_, bool isTurret_) {
	position = pos;
	friendly = friendly_;
	standard_target = position + ofVec2f(0,-1);
	standard_target.normalize() *= 2000;
	target = standard_target;
	type = type_;
	unit_type = unit_type_;
	isTurret = isTurret_;
	// defense turret: guard post
	if (type == 1) {
		std_reload_time = 12.5;
		range = 120;
	} 
	// anti-personnel gun
	else if (type == 2) {
		std_reload_time = 7.5;
		range = 140;
	}
	// tank turret
	else if (type == 3) {
		std_reload_time = 40;
		range = 250;
	}
	// scattergun
	else if (type == 4) {
		std_reload_time = 40;
		range = 110;
	}
	// sniper
	else if (type == 5) {
		std_reload_time = 100;
		range = 400;
	}
	// artillery
	else if (type == 6) {
		std_reload_time = 200;
		range = 700;
	}
	// unit cannon
	else if (type == 7) {
		std_reload_time = 200;
		range = 700;
	}
	// defense turret: machine gun
	else if (type == 8) {
		std_reload_time = 7.5;
		range = 350;
	}
	// defense turret: shotgun
	else if (type == 9) {
		std_reload_time = 40;
		range = 200;
	}
	// defense turret: sniper
	else if (type == 10) {
		std_reload_time = 100;
		range = 600;
	}
}

Turret::~Turret() {}

void Turret::draw() {
	if (reload_time < std_reload_time) {
		reload_time++;
	}
	else {
		isReloaded = true;
	}
	ofDrawCircle(position, 2.5);
}
void Turret::update(ofVec2f pos) {
	if (unit_type == 1) {
		position = pos;
	}
}
void Turret::calculateTrajectory(ofVec2f targ) {
	if (position.distance(targ) < position.distance(target)) {
		target = targ;
		trajectory = target - position;
	}
}
ofVec2f& Turret::getPosition() {
	return position;
}
void Turret::setPosition(ofVec2f pos) {
	position = pos;
}