#pragma once
#include "ofMain.h"
class Unit
{
protected:
	ofVec2f position;
	ofVec2f * position_ptr;
	bool dead = false;
public:
	Unit();
	Unit(ofVec2f, bool);
	~Unit();
	void draw();
	void drawHealth();
	void update();
	bool isDead();
	void calculateTrajectory(const ofVec2f&);
	void compareTargets(const ofVec2f&);
	const ofVec2f& getPosition();
	void checkOverlap_Unit(const ofVec2f&, const int&, const int&);
	void checkOverlap_Building(const ofVec2f&, const int&, int&, const int&);
	int sizeX = 0;
	int sizeY = 0;
	ofVec2f trajectory;
	ofVec2f trajectory_buffer;
	ofVec2f standard_target;
	ofVec2f target;
	bool selected = false;
	int colour;
	int colour_buffer;
	int health = 1;
	bool friendly;
	bool initialised = false;
	int type;
	int move_timer = 0;
	float target_distance = 0;
	bool search_and_destroy = false;
	bool reclaim = false;
	bool fly = false;
	int mass = 0;
	int mass_buffer;
	int speed = 0;
	int speed_buffer;
	bool throttle = false;
	bool attacking = false;
	ofImage unit_image = "dot.png";
	int target_type = 0;
	ofTrueTypeFont health_font;

	// turret
	int reload_time = 5;
	int std_reload_time = 5;
	int reload_time_buffer = 5;
	int range;
	bool isTurret = false;
	bool isReloaded = false;
	ofVec2f turret_trajectory;
	ofVec2f turret_standard_target;
	ofVec2f turret_target;
	int turret_type;
	virtual void calculateTurretTrajectory(const ofVec2f&, const int&);
	int rotation;
	int iterator = 0;
	bool shot = false;
};

class Tank : public Unit {
private:
public:
	Tank(ofVec2f, int, bool);
	~Tank();
	// turret
	void calculateTurretTrajectory(const ofVec2f&, const int&);
};

class Infantry : public Unit {
private:
public:
	Infantry(ofVec2f, int, bool);
	~Infantry();
};

class Static_Turret : public Unit{
private:
public:
	Static_Turret(ofVec2f, int, const int&);
	~Static_Turret();
	// turret
	void calculateTurretTrajectory(const ofVec2f&, const int&);
};