#pragma once

class Projectile {	
public:
	Projectile(const ofVec2f&, ofVec2f, const int&, const int&, const bool&, bool);
	~Projectile();
	void draw();

	ofVec2f position;
	bool hit = false;
	bool friendly;
	ofVec2f trajectory;
	int traveltime = 0;
	int std_traveltime;
	int damage;
	bool type;
};