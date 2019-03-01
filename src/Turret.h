#pragma once
class Turret {
private:
	ofVec2f position;
public:
	Turret();
	Turret(ofVec2f, int, int, bool, bool);
	~Turret();
	void draw();
	void update(ofVec2f);
	void calculateTrajectory(ofVec2f);
	ofVec2f& getPosition();
	void setPosition(ofVec2f);

	ofVec2f trajectory;
	ofVec2f standard_target;
	ofVec2f target;
	int reload_time = 5;
	int std_reload_time = 5;
	int range = 0;
	bool isReloaded = true;
	int type = 1;
	bool friendly = false;
	int unit_type = 5;
	bool isTurret = true;
	bool attacking = false;
};