#include "ofMain.h"
#include "Rect.h"

Rect::Rect() {}
Rect::Rect(ofVec2f position_, int type_, int rotation_, bool friendly_) {
	position = position_;
	type = type_;
	rotation = rotation_;
	friendly = friendly_;
	capture_stat.load("arial", 20);
	// ENEMY BUILDINGS
	// 97, 115, 100, 102, 103, 104, 105, 106, 107, 108
	// 121, 120, 99, 118, 98, 110, 109

	if (type == 97) {
		name = "house";
		sizeX = ofRandom(70,100);
		sizeY = sizeX;
		capture_status = 120;
		float r = ofRandom(1, 3);
		if (r > 2) {
			if (rotation == 1) {
				rect_image.load("house1.png");
				image_type = 1;
			}
			else if (rotation == 2) {
				rect_image.load("house3.png");
				image_type = 3;
			}
			else if (rotation == 3) {
				rect_image.load("house5.png");
				image_type = 5;
			}
			else if (rotation == 4) {
				rect_image.load("house7.png");
				image_type = 7;
			}
		}
		else {
			if (rotation == 1) {
				rect_image.load("house9.png");
				image_type = 9;
			}
			else if (rotation == 2) {
				rect_image.load("house11.png");
				image_type = 11;
			}
			else if (rotation == 3) {
				rect_image.load("house13.png");
				image_type = 13;
			}
			else if (rotation == 4) {
				rect_image.load("house15.png");
				image_type = 15;
			}
		}
	}
	else if (type == 115) {
		name = "market";
		sizeX = 240;
		sizeY = 240;
		capture_status = 250;
		rect_image.load("market1.png");
		image_type = 1;
	}
	else if (type == 102) {
		name = "church";
		sizeX = 150;
		sizeY = 100;
		capture_status = 1500;
		if (rotation == 1) {
			rect_image.load("church1.png");
			image_type = 1;
		}
		else if (rotation == 2) {
			rect_image.load("church3.png");
			image_type = 3;
		}
		else if (rotation == 3) {
			rect_image.load("church5.png");
			image_type = 5;
		}
		else if (rotation == 4) {
			rect_image.load("church7.png");
			image_type = 7;
		}
	}
	else if (type == 103) {
		name = "steelworks";
		sizeX = 150;
		sizeY = 120;
		capture_status = 300;
		if (rotation == 1) {
			rect_image.load("steelworks1.png");
			image_type = 1;
		}
		else if (rotation == 2) {
			rect_image.load("steelworks3.png");
			image_type = 3;
		}
		else if (rotation == 3) {
			rect_image.load("steelworks5.png");
			image_type = 5;
		}
		else if (rotation == 4) {
			rect_image.load("steelworks7.png");
			image_type = 7;
		}
		rect_image.resize(sizeX, sizeY);
	}
	else if (type == 104) {
		name = "farmstead";
		sizeX = 70;
		sizeY = 50;
		capture_status = 100;
		if (rotation == 1) {
			rect_image.load("farmstead1.png");
			image_type = 1;
		}
		else if (rotation == 2) {
			rect_image.load("farmstead3.png");
			image_type = 3;
		}
		else if (rotation == 3) {
			rect_image.load("farmstead5.png");
			image_type = 5;
		}
		else if (rotation == 4) {
			rect_image.load("farmstead7.png");
			image_type = 7;
		}

	}
	// trees
	else if (type == 106) {
		health = 45;
		float size = ofRandom(30, 60);
		sizeX = size;
		sizeY = size;
		float r = ofRandom(0, 12);
		if (r < 1) {
			rect_image.load("tree1A.png");
		}
		else if (r >= 1 && r < 2) {
			rect_image.load("tree1B.png");
		}
		else if (r >= 2 && r < 3) {
			rect_image.load("tree1C.png");
		}
		else if (r >= 3 && r < 4) {
			rect_image.load("tree1D.png");
		}
		else if (r >= 4 && r < 5) {
			rect_image.load("tree2A.png");
		}
		else if (r >= 5 && r < 6) {
			rect_image.load("tree2B.png");
		}
		else if (r >= 6 && r < 7) {
			rect_image.load("tree2C.png");
		}
		else if (r >= 7 && r < 8) {
			rect_image.load("tree2D.png");
		}
		else if (r >= 8 && r < 9) {
			rect_image.load("tree3A.png");
		}
		else if (r >= 9 && r < 10) {
			rect_image.load("tree3B.png");
		}
		else if (r >= 10 && r < 11) {
			rect_image.load("tree3C.png");
		}
		else if (r >= 11 && r < 12) {
			rect_image.load("tree3D.png");
		}
		rect_image.resize(sizeX, sizeY);
	}

	// fields
	else if (type == 107) {
		sizeX = ofRandom(180, 300);
		sizeY = ofRandom(180, 300);
		if (rotation == 1) {
			rect_image.load("fieldA.png");
		}
		else if (rotation == 2) {
			rect_image.load("fieldB.png");
		}
		else if (rotation == 3) {
			rect_image.load("fieldC.png");
		}
		else if (rotation == 4) {
			rect_image.load("fieldD.png");
		}
		rect_image.resize(sizeX, sizeY);
	}
	/*
	// texture
	else if (type == 108) {
		sizeX = ofRandom(500,800);
		sizeY = ofRandom(500,800);
		if (rotation == 1) {
			rect_image.load("textureA.png");
		}
		else if (rotation == 2) {
			rect_image.load("textureB.png");
		}
		else if (rotation == 3) {
			rect_image.load("textureC.png");
		}
		else if (rotation == 4) {
			rect_image.load("textureD.png");
		}
		rect_image.resize(sizeX, sizeY);
	}
	*/

	// throttling_object
	else if (type == 98) {
		sizeX = 150;
		sizeY = 150;
		//rect_image.load("tree3A.png");
		//rect_image.resize(sizeX, sizeY);
	}

	if (rotation == 2 || rotation == 4) {
		float sizeX_buffer = sizeX;
		sizeX = sizeY;
		sizeY = sizeX_buffer;

	}
}
Rect::~Rect() {}

void Rect::update() {
	if (capture_status <= 0) {
		friendly = true;
	}
	else if (capture_status >= 200) {
		friendly = false;
	}
}

void Rect::draw() {
	if (friendly) {
		ofPushMatrix();
		ofTranslate(position);
		rect_image.draw(0, 0);
		ofPopMatrix();
	}
	else {
		ofPushMatrix();
		ofTranslate(position);
		rect_image.draw(0,0);
		ofPopMatrix();
	}
}

void Rect::drawCapture() {
	ofPushMatrix();
	ofTranslate(position);
	ofTranslate(-sizeX / 3, sizeY / 2);
	capture_stat.drawString(to_string(capture_status/10), 0, 0);
	ofPopMatrix();
}

const ofVec2f& Rect::getPosition() {
	return position;
}
ofVec2f* Rect::getPosition_ptr() {
	return &position;
}
int& Rect::getType() {
	return type;
}