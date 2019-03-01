// smoke on damaged tanks
// building capture_int font double size



#pragma once

#include "ofMain.h"
#include "Unit.h"
#include "Rect.h"
#include "Projectile.h"
#include "ParticleSystem.h"
#include "Explosion.h"

class Line { /// Line class for drawing lines
public:
	ofPoint a;
	ofPoint b;
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);

		void initialise();
		void savegame();
		void loadgame();
		void tank_Turrets_Reclaim_SearchandDestroy();
		void calculateMouse();
		void tank_selection();

		/// Camera and Mouse
		ofCamera cam;
		ofVec2f mouse;
		// offsets for scrolling
		float xOffset; float yOffset; float zOffset;
		// offsets for arrow keys
		float xOff; float yOff;
		// centre vector and distance to screen centre
		ofVec2f centre_vector;
		float centre_distance;
		float X, Y;
		int Mouse_iterator;
		ofVec2f Mouse_origin;
		float Selection_X, Selection_Y;

		/// Global Vectors
		vector<Unit*> tanks;
		vector<Rect*> rects;
		vector<Rect*> trees;
		vector<Projectile*> projectiles;
		vector<ParticleSystem*> systems;
		vector<Explosion*> explosions;

		/// Gameplay variables
		int Iterator;
		int resource_boost;
		int globaltimer;

		bool buildmode;
		bool spawnmode;
		bool lock;
		bool show_ranges;
		int rotation;
		int global_key;
		int spawn_key;
		bool freeplay;
		int victory_points;
		bool show_health;

		/// Other
		ofImage background_image;
		ofImage menu_image;
		int scenario;
		ofTrueTypeFont scenario_string;
		ofTrueTypeFont scenario_note_string;

		bool no_golem;
		ofTrueTypeFont disclaimer;

		int framerate;
		
		/// Sound Player
		ofSoundPlayer T1;
		ofSoundPlayer T2;
		ofSoundPlayer T3;
		ofSoundPlayer T4;
		ofSoundPlayer T5;
		ofSoundPlayer H_mov;
		ofSoundPlayer T_mov;
		ofSoundPlayer C1;
		ofSoundPlayer C2;
		ofSoundPlayer C3;
		ofSoundPlayer C4;
		ofSoundPlayer C5;
		ofSoundPlayer I1;

		ofSoundPlayer Capture1;
		ofSoundPlayer ReCapture1;
		ofSoundPlayer Victory_point1;
		ofSoundPlayer Victory_point2;
		ofSoundPlayer Victory_point1_lost;
		ofSoundPlayer Victory_point2_lost;
		ofSoundPlayer Victory_sound;

		ofSoundPlayer order;
		ofSoundPlayer target_acquired;
		ofSoundPlayer reclaim_order;

		ofSoundPlayer soundtrack;

		vector<ofVec2f*> primary_AI_targets;

		/// UI buildmode images
		ofImage img2, img3, img4, img5, img6, img7, img8, img9;
		ofImage info2, info3, info4, info5, info6, info7, info8, info9;
		ofImage sel2, sel3, sel4, sel5, sel6, sel7, sel8, sel9;
		ofImage top_bar;
		ofImage one, two, three, four, five, six;

		/// Resources											static buildings:									resources:
		
		//int silver;		// hire mercenaries					loot (house, church, bank), all buildings			
		int materials;	// build tanks						market, factory										wood, mining shaft
		int supplies;	// used by units over time			farmstead		
		//ofTrueTypeFont silver_font;
		ofTrueTypeFont materials_font;
		ofTrueTypeFont supplies_font; 
		ofTrueTypeFont timer_font_;
		ofTrueTypeFont points_font;
		string spawn_selected;
		ofTrueTypeFont selection;
		ofTrueTypeFont victory;
		ofTrueTypeFont spawnmode_info;

		/// units unlocked bools
		bool unlock1;
		bool unlock2;
		bool unlock3;
		bool unlock4;
		bool menu; 
		bool start_playing;

};