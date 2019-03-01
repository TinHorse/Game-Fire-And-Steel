#include "ofMain.h"
#include "Projectile.h"

Projectile::Projectile(const ofVec2f& from_, ofVec2f trajectory_, const int& std_traveltime_, const int& damage_, const bool& friendly_, bool type_) {
	position = from_;
	std_traveltime = std_traveltime_;
	trajectory = trajectory_.normalize() * 20;
	if (std_traveltime > 100) {
		trajectory *= 2;
		std_traveltime /= 2;
	}
	friendly = friendly_;
	damage = damage_;
	type = type_;
}
Projectile::~Projectile() {}
void Projectile::draw() {
	position += trajectory;
	traveltime++;
	if (traveltime >= std_traveltime/2) {
		hit = true;
	}
	ofDrawCircle(position.x, position.y, 2 + damage/30, 2 + damage/30);
}