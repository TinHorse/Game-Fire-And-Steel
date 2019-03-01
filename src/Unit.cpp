#include "Unit.h"




Unit::Unit()
{
	health_font.load("arial", 13);
}

Unit::Unit(ofVec2f position_, bool friendly_)
{
	position = position_;
	friendly = friendly_;
	health_font.load("arial", 13);
}

Unit::~Unit()
{
	position_ptr = nullptr;
}

void Unit::update() {
	if (health <= 0) {
		dead = true;
	}
	if (move_timer >= target_distance) {
		trajectory = { 0,0 };
		move_timer = 0;
		target_distance = 0;
	}
	if (trajectory != ofVec2f(0, 0)) {
		move_timer++;
		if (throttle) {
			if (type != 7 && type != 8) {
				trajectory.normalize() *= 0.3;
			}
			else {
				trajectory.normalize() *= 0.5;
			}
			move_timer--;
		}
		mass = mass_buffer;
		position += trajectory;
		trajectory = trajectory_buffer;
	}
	if (isTurret) {
		if (reload_time < std_reload_time) {
			reload_time++;
		}
		else {
			isReloaded = true;
		}
	}
}
bool Unit::isDead() {
	return dead;
}
void Unit::draw() {
	ofPushMatrix();
	ofTranslate(position);
	ofRotate(-trajectory.angle(ofVec2f(0,-1)));
	if (selected) {
		ofSetColor(150,0,0, 200);
		ofNoFill();
		if (type == 1 || type == 9) {
			ofDrawCircle(0,0, range);
		}
		ofDrawRectangle(0,0,sizeX,sizeY);
		ofSetColor(255);
		ofFill();
	}
	unit_image.draw(0,0);
	ofPopMatrix();
}
void Unit::drawHealth() {
	iterator++;
	if ((iterator % 200) == 0) {
		if (health_font.isLoaded() == false) {
			health_font.load("STENCIL", 13);
		}
		iterator++;
	}
	
	ofPushMatrix();
	ofTranslate(position);
	ofTranslate(-sizeX/3,sizeY/2);
	health_font.drawString(to_string(health/10), 0, 0);
	ofPopMatrix();
}

void Unit::calculateTrajectory(const ofVec2f& targ) {
	move_timer = 0;
	target = targ;
	trajectory = target - position;
	trajectory.normalize() *= (speed / 3.5);
	target_distance = position.distance(target);
	target_distance /= (speed / 3.5);
	trajectory_buffer = trajectory;
}
void Unit::compareTargets(const ofVec2f& targ) {
	move_timer = 0;
	if (position.distance(targ) < position.distance(target)) {
		target = targ;
		trajectory = target - position;
		trajectory.normalize() *= (speed / 3.5);
		target_distance = position.distance(target);
		target_distance /= (speed / 3.5);
		trajectory_buffer = trajectory;
	}
}
const ofVec2f& Unit::getPosition() {
	return position;
}
void Unit::checkOverlap_Unit(const ofVec2f& pos, const int& size, const int& magnitude) {
	if (pos.distance(position) <= (sizeY*1.2)) {
		ofVec2f move_away = position - pos;
		move_away.normalize() /= (mass/4);
		position += move_away * magnitude;
	}
}
void Unit::checkOverlap_Building(const ofVec2f& pos, const int& size, int& capture_status, const int& rect_type) {
	if (pos.distance(position) <= ((sizeY*0.8) + size) && type != 5 && rect_type != 115) {
		ofVec2f move_away = position - pos;
		move_away.normalize();
		move_away.x *= 8;		
		if (sizeY >= 48) {
			position += move_away + ofRandom(-2,2);
			move_timer-=3;
		}
		else if (rect_type != 106) {
			position += move_away + ofRandom(-2, 2);
			move_timer-=3;
		}
	}
	if (type == 7 || type == 8 || type == 5) {
		if (pos.distance(position) <= 160) {
			if (friendly && capture_status > 0) {
				capture_status-=2;
			}
			else if (capture_status < 200) {
				capture_status++;
			}
		}
	}
}
void Unit::calculateTurretTrajectory(const ofVec2f& targ, const int& target_type_) {
	if (position.distance(targ) < position.distance(turret_target)) {
		turret_target = targ;
		turret_trajectory = turret_target - position;
		target_type = target_type_;
	}
}

Tank::Tank(ofVec2f position_, int type_, bool friendly_)
{
	position = position_;
	type = type_;
	friendly = friendly_;
	isTurret = true;
	turret_standard_target = position + ofVec2f(0, -1);
	turret_standard_target.normalize() *= 10000;
	turret_target = turret_standard_target;
	if (friendly) {
		// light tank
		if (type == 2) {
			sizeX = 40;
			sizeY = 40;
			health = 500;
			mass_buffer = 2;
			speed = 4;
			turret_type = 2;
			unit_image.load("lightTank1.png");
			unit_image.resize(sizeX, sizeY);
		}
		// heavy tank
		else if (type == 3) {
			sizeX = 60;
			sizeY = 60;
			health = 900;
			mass_buffer = 5;
			speed = 2;
			turret_type = 3;
			unit_image.load("heavyTank1.png");
			unit_image.resize(sizeX, sizeY);
		}
		// bulldozer
		else if (type == 4) {
			sizeX = 48;
			sizeY = 48;
			health = 1400;
			mass_buffer = 5;
			speed = 2;
			turret_type = 4;
			unit_image.load("bulldozer1.png");
			unit_image.resize(sizeX, sizeY);
		}
		// interceptor
		else if (type == 5) {
			sizeX = 55;
			sizeY = 35;
			health = 500;
			mass_buffer = 0;
			speed = 5;
			turret_type = 5;
			fly = true;
			unit_image.load("interceptor1.png");
		}
		// artillery
		else if (type == 6) {
			sizeX = 32;
			sizeY = 72;
			health = 500;
			mass_buffer = 20;
			speed = 1;
			turret_type = 6;
			unit_image.load("artillery1.png");
			unit_image.resize(sizeX, sizeY);
		}
		// Golem
		else if (type == 9) {
			sizeX = 175;
			sizeY = 140;
			health = 5000;
			mass_buffer = 200;
			speed = 1;
			turret_type = 2;
			unit_image.load("golem1.png");
			unit_image.resize(sizeX, sizeY);
		}
	}
	else {
		if (type == 2) {
			sizeX = 40;
			sizeY = 40;
			health = 700;
			mass_buffer = 2;
			speed = 4;
			turret_type = 2;
			unit_image.load("lightTank2.png");
			unit_image.resize(sizeX, sizeY);
		}
		else if (type == 3) {
			sizeX = 60;
			sizeY = 60;
			health = 1000;
			mass_buffer = 5;
			speed = 2;
			turret_type = 3;
			unit_image.load("heavyTank2.png");
			unit_image.resize(sizeX, sizeY);
		}
		else if (type == 4) {
			sizeX = 48;
			sizeY = 48;
			health = 1600;
			mass_buffer = 5;
			speed = 2;
			turret_type = 4;
			unit_image.load("bulldozer2.png");
			unit_image.resize(sizeX, sizeY);
		}
		else if (type == 5) {
			sizeX = 55;
			sizeY = 35;
			health = 500;
			mass_buffer = 0;
			speed = 5;
			turret_type = 5;
			unit_image.load("interceptor2.png");
		}
		else if (type == 6) {
			sizeX = 32;
			sizeY = 64;
			health = 500;
			mass_buffer = 20;
			speed = 1;
			turret_type = 6;
			unit_image.load("artillery2.png");
			unit_image.resize(sizeX, sizeY);
		}
		else if (type == 9) {
			sizeX = 60;
			sizeY = 66;
			health = 4000;
			mass_buffer = 2000;
			speed = 0;
			isTurret = false;
			float ran = ofRandom(0, 4);
			if (ran < 1) {
				unit_image.load("golem2A.png");
			}
			else if (ran >= 1 && ran < 2) {
				unit_image.load("golem2B.png");
				sizeX = 66;
				sizeY = 60;
			}
			else if (ran >= 2 && ran < 3) {
				unit_image.load("golem2C.png");
			}
			else {
				unit_image.load("golem2D.png");
				sizeX = 66;
				sizeY = 60;
			}
			unit_image.resize(sizeX, sizeY);
		}
	}
	// defense turret: guard post
	if (turret_type == 1) {
		std_reload_time = 12.5;
		range = 200;
	}
	// Support cannon
	else if (turret_type == 2) {
		std_reload_time = 30;
		range = 350;
	}
	// tank turret
	else if (turret_type == 3) {
		std_reload_time = 40;
		range = 500;
	}
	// scattergun
	else if (turret_type == 4) {
		std_reload_time = 60;
		range = 200;
	}
	// interceptor gun
	else if (turret_type == 5) {
		std_reload_time = 30;
		range = 200;
	}
	// artillery
	else if (turret_type == 6) {
		std_reload_time = 200;
		range = 1000;
	}
	// unit cannon
	else if (turret_type == 7) {
		std_reload_time = 30;
		range = 200;
	}
	health *= 3;
	speed *= 2;
	speed_buffer = speed;
	reload_time_buffer = std_reload_time;
	health_font.load("arial", 13);
	if (health_font.isLoaded() == false) {
		health_font.load("arial", 13);
	}
}


Tank::~Tank()
{
	position_ptr = nullptr;
}
void Tank::calculateTurretTrajectory(const ofVec2f& targ, const int& target_type_) {
	if (position.distance(targ) < position.distance(turret_target)) {
		turret_target = targ;
		turret_trajectory = turret_target - position;
		target_type = target_type_;
	}
}

Infantry::Infantry(ofVec2f position_, int type_, bool friendly_)
{
	position = position_;
	type = type_;
	friendly = friendly_;
	isTurret = false;
	mass_buffer = 1;
	if (friendly) {
		// harvester drone
		if (type == 1) {
			health = 100;
			sizeX = 25;
			sizeY = 25;
			speed = 3;
			mass_buffer = 0;
			unit_image.load("harvester.png");
			unit_image.resize(sizeX, sizeY);
			range = 200;
			fly = true;
		}
		// heavy infantry
		if (type == 7) {
			health = 500;
			sizeX = 40;
			sizeY = 40;
			speed = 3;
			mass_buffer = 2;
			unit_image.load("infantry1.png");
			unit_image.resize(sizeX, sizeY);
		}
		// lancer
		else if (type == 8) {
			health = 500;
			sizeX = 16;
			sizeY = 28;
			speed = 7;
			unit_image.load("cavalry1.png");
			unit_image.resize(sizeX, sizeY);
		}
	}
	else {
		// heavy infantry
		if (type == 7) {
			health = 600;
			sizeX = 40;
			sizeY = 40;
			speed = 3;
			mass_buffer = 2;
			unit_image.load("infantry2.png");
			unit_image.resize(sizeX, sizeY);
		}
		// lancer
		else if (type == 8) {
			health = 600;
			sizeX = 12;
			sizeY = 28;
			speed = 7;
			unit_image.load("cavalry2.png");
			unit_image.resize(sizeX, sizeY);
		}
	}
	health *= 3;
	speed *= 2;
	speed_buffer = speed;
	health_font.load("arial", 13);
	if (health_font.isLoaded() == false) {
		health_font.load("arial", 13);
	}
}

Infantry::~Infantry()
{
	position_ptr = nullptr;
}

Static_Turret::Static_Turret(ofVec2f position_, int type_, const int& rotation_) {
	position = position_;
	type = type_;
	friendly = false;
	mass_buffer = 2000;
	mass = 2000;
	turret_standard_target = position + ofVec2f(0, -1);
	turret_standard_target.normalize() *= 10000;
	turret_target = turret_standard_target;
	rotation = rotation_;
	if (type == 10) {
		string name = "point defense";
		sizeX = 60;
		sizeY = 60;
		health = 8000;
		speed = 0;
		isTurret = true;
		turret_type = 8;
		unit_image.load("turret.png");
	}
	else if (type == 11) {
		string name = "scattergun";
		sizeX = 80;
		sizeY = 80;
		health = 14000;
		speed = 0;
		isTurret = true;
		turret_type = 9;
		unit_image.load("turret.png");
	}
	else if (type == 12) {
		string name = "FLAK";
		sizeX = 40;
		sizeY = 40;
		health = 4000;
		speed = 0;
		isTurret = true;
		turret_type = 10;
		unit_image.load("turret.png");
	}
	else if (type == 13) {
		string name = "walls";
		sizeX = 80;
		sizeY = 40;
		health = 6000;
		mass_buffer = 5000;
		speed = 0;
		isTurret = false;
		if (rotation == 1) {
			unit_image.load("wall.png");
		}
		else if (rotation == 2) {
			unit_image.load("wall2.png");
		}
		else if (rotation == 3) {
			unit_image.load("wall3.png");
		}
		else if (rotation == 4) {
			unit_image.load("wall4.png");
		}
	}
	else if (type == 14) {
		string name = "gate";
		sizeX = 240;
		sizeY = 120;
		health = 10000;
		mass_buffer = 5000;
		speed = 0;
		isTurret = false;
		if (rotation == 1) {
			unit_image.load("gate.png");
		}
		else if (rotation == 2) {
			unit_image.load("gate2.png");
		}
		else if (rotation == 3) {
			unit_image.load("gate3.png");
		}
		else if (rotation == 4) {
			unit_image.load("gate4.png");
		}
	}
	// defense turret: Heavy Machine Gun
	if (turret_type == 8) {
		std_reload_time = 40;
		range = 600;
	}
	// defense turret: Shotgun Turret
	else if (turret_type == 9) {
		std_reload_time = 40;
		range = 350;
	}
	// defense turret: Flak Battery
	else if (turret_type == 10) {
		std_reload_time = 20;
		range = 600;
	}
	health *= 3;
	if (rotation == 2 || rotation == 4) {
		float sizeX_buffer = sizeX;
		sizeX = sizeY;
		sizeY = sizeX_buffer;

	}
	unit_image.resize(sizeX, sizeY);
	reload_time_buffer = std_reload_time;
	health_font.load("arial", 13);
	if (health_font.isLoaded() == false) {
		health_font.load("arial", 13);
	}
}
Static_Turret::~Static_Turret() { position_ptr = nullptr; };

void Static_Turret::calculateTurretTrajectory(const ofVec2f& targ, const int& target_type_) {
	if (position.distance(targ) < position.distance(turret_target)) {
		turret_target = targ;
		turret_trajectory = turret_target - position;
		target_type = target_type_;
	}
}