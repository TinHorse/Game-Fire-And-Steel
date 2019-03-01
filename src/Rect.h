#pragma once
class Rect {
private:
	ofVec2f position;
	int type = 0; 
	float range = 0;
public:
	Rect();
	Rect(ofVec2f, int, int, bool);
	~Rect();
	bool initialised = false;
	void update();
	void draw();
	float sizeX = 40, sizeY = 40;
	bool friendly;
	int capture_status = 1;
	ofImage rect_image;
	bool looted = false;
	bool kill = false;
	bool harvested = false;
	int health = 1;
	int image_type;
	string name = "dot";
	bool changeColour = true;
	int rotation = 1;
	ofTrueTypeFont capture_stat;

	const ofVec2f& getPosition();
	ofVec2f* getPosition_ptr();
	int& getType();
	void drawCapture();
};
