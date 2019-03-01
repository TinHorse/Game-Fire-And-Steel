#include "ofApp.h"
//--------------------------------------------------------------

void ofApp::initialise() { /// Initialises global variables
	cam.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 920); // initialises camera position
	xOffset = 0; yOffset = 0; zOffset = 0; xOff = 0; yOff = 0; // offsets for scrolling and zooming
	mouse = { float(ofGetMouseX()), float(ofGetMouseY() * -1 + ofGetHeight()) }; // mouse vector
	Iterator = 0; // global iterator

	Selection_X = 0; Selection_Y = 0; // mouse selection variables
	Mouse_iterator = 2; // iterator for mouse selection (prevents double mouse-click)
	buildmode = false; // mode for spawning buildings (in freeplay mode)
	spawnmode = true; // mode for spawning units (in-game)
	lock = true; // lock for Golem selection
	rotation = 1; // rotation for placing buildings
	global_key = '1000'; // stores building type selection
	spawn_key = '1000'; // stores unit type selection
	freeplay = false; // switches between scenario creation mode and play mode
	globaltimer = 1500;
	show_health = false;
	no_golem = true;
}

void ofApp::setup(){
	ofBackground(255);
	framerate = 30;
	ofSetFrameRate(framerate);
	menu = true;
	menu_image.load("menu_image3.jpg"); // loads image for menu in between scenarios
	start_playing = false;
	resource_boost = 2; // allows for easy difficulty adjustments

	// set resource variables
	materials = 0; supplies = 0;
	unlock1 = 0; unlock2 = 0; unlock3 = 0; unlock4 = 0;

	// set scenario
	scenario = 1;
	std::ifstream in("scenario.txt");
	string line;
	while (getline(in, line)) {
		scenario = stoi(line);
	}
	

	/// Sound Initialisation
	ofSoundSetVolume(1.5);
	T1.load("tank1.wav");
	T1.setVolume(0.8);
	T2.load("tank2.wav");
	T3.load("tank3.wav");
	T3.setVolume(0.7);
	T4.load("tank4.wav");
	T4.setVolume(0.03);
	T5.load("tank5.wav");
	T5.setVolume(0.03);
	T_mov.load("tank_movement.wav");
	T_mov.setVolume(0.02);
	H_mov.load("helicopter.wav");
	H_mov.setVolume(0.002);

	C1.load("Cav1.wav");
	C1.setVolume(0.005);
	C2.load("Cav2.wav");
	C2.setVolume(0.005);
	C3.load("Cav3.wav");
	C3.setVolume(0.005);
	C4.load("Cav4.wav");
	C4.setVolume(0.005);
	C5.load("Cav5.wav");
	C5.setVolume(0.025);
	I1.load("infantry_fighting.wav");
	I1.setVolume(0.003);

	Capture1.load("Capture1.wav");
	Capture1.setVolume(0.05);
	ReCapture1.load("ReCapture1.wav");
	ReCapture1.setVolume(0.05);

	Victory_point1.load("VictoryPoint1.wav");
	Victory_point1.setVolume(0.05);
	Victory_point2.load("VictoryPoint2.wav");
	Victory_point2.setVolume(0.05);
	Victory_point1_lost.load("VictoryPoint1_lost.wav");
	Victory_point1_lost.setVolume(0.05);
	Victory_point2_lost.load("VictoryPoint2_lost.wav");
	Victory_point2_lost.setVolume(0.05);
	Victory_sound.load("Victory.wav");
	Victory_sound.setVolume(0.05);

	order.load("order.wav");
	order.setVolume(0.08);
	target_acquired.load("target_acquired.wav");
	target_acquired.setVolume(0.08);
	reclaim_order.load("reclaim.wav");
	reclaim_order.setVolume(0.08);

	/// Load UI buildmode Images
	img2.load("lightTank1.png");
	img3.load("heavyTank1.png");
	img4.load("bulldozer1.png");
	img5.load("interceptor1.png");
	img6.load("artillery1.png");
	img7.load("infantry1.png");
	img8.load("cavalry1.png");
	img9.load("golem1.png");

	info2.load("info2.png");
	info3.load("info3.png");
	info4.load("info4.png");
	info5.load("info5.png");
	info6.load("info6.png");
	info7.load("info7.png");
	info8.load("info8.png");
	info9.load("info9.png");

	sel2.load("selection2.png");
	sel3.load("selection3.png");
	sel4.load("selection4.png");
	sel5.load("selection5.png");
	sel6.load("selection6.png");
	sel7.load("selection7.png");
	sel8.load("selection8.png");
	sel9.load("selection9.png");

	top_bar.load("top_bar.jpg");

	string font_dir = "courier new";

	scenario_string.load(font_dir, 80);
	scenario_note_string.load(font_dir, 22);

	/// Resource Font initialisation
	materials_font.load(font_dir, 17); supplies_font.load(font_dir, 17); timer_font_.load(font_dir, 17);
	points_font.load(font_dir, 17);
	spawn_selected = "none";
	selection.load(font_dir, 25);
	victory.load(font_dir, 50);
	spawnmode_info.load(font_dir, 18);
	disclaimer.load(font_dir, 30);

}
//--------------------------------------------------------------
void ofApp::update() {
	if (menu == false) {
		if ((Iterator % 20) == 0) {
			globaltimer--;
			if (soundtrack.isPlaying() == false) {
				float r = ofRandom(3);
				if (r <= 1) {
					soundtrack.load("reaper_three.wav");
					soundtrack.setVolume(0.015);
				}
				else if (r >= 2) {
					soundtrack.load("Galleon_piano.wav");
					soundtrack.setVolume(0.040);
				}
				else {
					soundtrack.load("track1.wav");
					soundtrack.setVolume(0.025);
				}
				soundtrack.play(); // plays soundtrack
			}

			if (scenario == 1) {
				int amount_of_heavy_tanks = 0;
				int amount_of_turrets = 0;
				for (int i = 0; i < tanks.size(); i++) {
					if (tanks[i]->type == 3 && tanks[i]->friendly) {
						amount_of_heavy_tanks++;
					}
					else if ((tanks[i]->type == 7 || tanks[i]->type == 8) && tanks[i]->friendly == false) {
						tanks[i]->search_and_destroy = true;
					}
					else if (tanks[i]->type == 10) {
						amount_of_turrets++;
					}
				}
				if (amount_of_heavy_tanks >= 5) {
					for (int i = 0; i < tanks.size(); i++) {
						if (tanks[i]->friendly == false) {
							tanks[i]->search_and_destroy = true;
						}
					}
				}
				if (amount_of_turrets == 0) {
					amount_of_turrets = -1;
					for (int i = 0; i < tanks.size(); i++) {
						if (tanks[i]->friendly == false && tanks[i]->type == 9) {
							tanks.push_back(new Tank(tanks[i]->getPosition(), 2, false));
							tanks.push_back(new Tank(tanks[i]->getPosition(), 3, false));
							tanks.push_back(new Tank(tanks[i]->getPosition(), 3, false));
							tanks.push_back(new Infantry(tanks[i]->getPosition(), 7, false));
							tanks.push_back(new Infantry(tanks[i]->getPosition(), 7, false));
							tanks.push_back(new Tank(tanks[i]->getPosition(), 4, false));
							tanks[i]->search_and_destroy = true;
						}
					}
				}
			}

		}
		calculateMouse();

		// TANK reclaim, search and destroy
		tank_Turrets_Reclaim_SearchandDestroy();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	cam.begin();

	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetColor(255);
	if (menu) {
		if (ofGetKeyPressed(OF_KEY_RETURN)) {
			start_playing = true;
			resource_boost = 2;
		}
		else if (ofGetKeyPressed(OF_KEY_TAB)) {
			start_playing = true;
			resource_boost = 1;
		}
		if (start_playing) {
			cout << resource_boost << endl;
			menu = false;
			victory_points = 2;
			if (scenario == 1) {
				background_image.load("backgroundPlain_tutorial.jpg");
				materials = 3000; supplies = 20;
			}
			else if (scenario == 2) {
				background_image.load("backgroundPlain.jpg");
				materials = 1500; supplies = 15;
			}
			else if (scenario == 3) {
				background_image.load("backgroundRivers.jpg");
				materials = 2500; supplies = 20;
				unlock2 = true;
			}
			else if (scenario == 4) {
				background_image.load("backgroundRivers.jpg");
				materials = 7000; supplies = 70;
				unlock1 = true; unlock2 = true;
				victory_points = 4;
			}
			else if (scenario == 5) {
				background_image.load("backgroundSwampland.jpg");
				materials = 4000; supplies = 25;
				unlock1 = true; unlock2 = true; unlock3 = true; unlock4 = true;
			}
			else if (scenario == 6) {
				background_image.load("backgroundCastle.jpg");
				materials = 7000; supplies = 25;
				unlock1 = true; unlock2 = true; unlock3 = true; unlock4 = true;
				victory_points = 0;
			}
			else {
				materials = 4000; supplies = 20;
			}
			materials += (1000 * (resource_boost - 1)); 
			supplies += (10 * (resource_boost - 1));
			initialise();
			loadgame();
		}
	}
	else {
		background_image.draw(0, 0);
		Iterator++;
	}

	// ENEMY spawn
	if ((Iterator % 1000) == 0 && freeplay == false && scenario != 1 && scenario != 6) {
		for (int i = 0; i < tanks.size(); i++) {
			if ((Iterator % 4000) == 0 && Iterator >= 10) {
				if (tanks[i]->type == 9 && tanks[i]->friendly == false) {
					float ran = ofRandom(0, 2);
					if (ran > 1) {
						tanks.push_back(new Tank(tanks[i]->getPosition(), ofRandom(2,6), false));
						tanks.push_back(new Infantry(tanks[i]->getPosition(), ofRandom(7, 9), false));
						tanks.push_back(new Tank(tanks[i]->getPosition(), ofRandom(2, 6), false));
						cout << Iterator << endl;
					}
				}
			}
			if ((scenario == 4 || scenario == 6) && Iterator == 1500) {
				if (tanks[i]->type == 9 && tanks[i]->friendly == false) {
					tanks.push_back(new Tank(tanks[i]->getPosition(), ofRandom(2, 6), false));
					tanks.push_back(new Infantry(tanks[i]->getPosition(), ofRandom(7, 9), false));
				}
			}
			if ((Iterator % 3000) == 0 && Iterator >= 10) {
				if (tanks[i]->type == 9 && tanks[i]->friendly == false) {
					float ran = ofRandom(0, 2);
					if (ran > 1) {
						tanks.push_back(new Tank(tanks[i]->getPosition(), 5, false));
						tanks.push_back(new Infantry(tanks[i]->getPosition(), 8, false));
					}
					if (scenario == 4) {
						tanks.push_back(new Tank(tanks[i]->getPosition(), ofRandom(2,6), false));
						tanks.push_back(new Infantry(tanks[i]->getPosition(), ofRandom(7,9), false));
					}
				}
			}
			if ((Iterator % 10000) == 0) {
				if (tanks[i]->type == 9 && tanks[i]->friendly == false) {
					tanks.push_back(new Infantry(tanks[i]->getPosition(), ofRandom(7, 9), false));
					tanks.push_back(new Tank(tanks[i]->getPosition(), ofRandom(2, 6), false));
					cout << Iterator << endl;
				}
			}
			if ((Iterator % 15000) == 0 && Iterator >= 10) {
				if (tanks[i]->type == 9 && tanks[i]->friendly == false) {
					tanks.push_back(new Infantry(tanks[i]->getPosition(), 8, false));
					tanks.push_back(new Infantry(tanks[i]->getPosition(), 8, false));
					tanks.push_back(new Infantry(tanks[i]->getPosition(), 5, false));
					cout << Iterator << endl;
				}
			}
			if ((Iterator % 30000) == 0 && Iterator >= 10) {
				float ran = ofRandom(0, 4);
				ofVec2f pos = { 0,0 };
				if (ran < 1) {
					pos = { 3000,1000 };
				}
				else if (ran >= 1 && ran < 2) {
					pos = { 3000,-1000 };
				}
				else if (ran >= 2 && ran < 3) {
					pos = { -3000,1000 };
				}
				else {
					pos = { -3000,-1000 };
				}
				for (int k = 0; k < 50; k++) {
					tanks.push_back(new Infantry(pos, 8, false));
				}
				if (scenario == 6) {
					for (int k = 0; k < 10; k++) {
						tanks.push_back(new Tank(pos, 4, false));
					}
				}
				for (int k = 0; k < 15; k++) {
					tanks.push_back(new Tank(pos, 5, false));
				}
				cout << Iterator << endl;
			}
			if (tanks[i]->friendly == false && tanks[i]->type < 9) {
				bool calculate = false;
				bool attack = false;
				tanks[i]->search_and_destroy = true;
				if (tanks[i]->type == 7 || tanks[i]->type == 8) {
					float r = ofRandom(0, 3);
					if (r >= 2) {
						tanks[i]->reclaim = true;
						tanks[i]->search_and_destroy = false;
					}
				}
				for (int j = 0; j < tanks.size(); j++) {
					if (i != j) {
						if (tanks[j]->type == 9 && tanks[j]->friendly == false && tanks[i]->getPosition().distance(tanks[j]->getPosition()) < 20) {
							calculate = true;
							tanks[i]->search_and_destroy = false;
						}
					}
				}
				if (calculate) {
					if(tanks[i]->type <= 8){
						for (int j = 0; j < primary_AI_targets.size(); j++) {
							tanks[i]->compareTargets(*primary_AI_targets[j]);
						}
					}
				}
			}
		}
	}

	if (freeplay) {
		ofSetColor(0, 0, 255);
		for (ofVec2f * t : primary_AI_targets) {
			ofDrawCircle(*t, 20);
		}
	}

	// FIELDS draw
	for (int i = 0; i < trees.size(); i++) {
		if (trees[i]->getType() == 107 && trees[i]->getPosition().distance(centre_vector) < centre_distance) {
			trees[i]->draw();
		}
	}

	// RECTS draw, update, health check, delete
	//ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetLineWidth(1);
	ofSetColor(255);
	// rects[i].getType() != 98 && 
	for (int i = 0; i < rects.size(); i++) {
		if (rects[i]->getType() != 98 && rects[i]->getPosition().distance(centre_vector) < centre_distance) {
			rects[i]->draw();
		}
		if (show_health && rects[i]->getType() != 98 && rects[i]->getType() != 106 && rects[i]->getType() != 107) {
			rects[i]->drawCapture();
		}
		if ((Iterator % 200) == 0) {
			if (rects[i]->friendly) {
				if (rects[i]->changeColour == true) {
					if (rects[i]->looted == false) {
						if (rects[i]->getType() == 97) {
							materials += 70 * resource_boost / 1.5;
						}
						else if (rects[i]->getType() == 102) {
							materials += 300 * resource_boost / 1.5;
							victory_points += 3;
							Victory_point2.play();
						}
						else if (rects[i]->getType() == 115) {
							materials += 500 * resource_boost / 1.5;
							supplies += 2;
							Capture1.play();
						}
						else if (rects[i]->getType() == 103) {
							materials += 500 * resource_boost / 1.5;
							supplies += 2 * resource_boost;;
							Capture1.play();
						}
						else if (rects[i]->getType() == 104) {
							supplies += 5;
							materials += 50 * resource_boost / 1.5;
						}
						rects[i]->looted = true;
					}
					rects[i]->changeColour = false;
					string nam = rects[i]->name + to_string(rects[i]->image_type + 1) + ".png";
					rects[i]->rect_image.load(nam);
					rects[i]->rect_image.resize(rects[i]->sizeX, rects[i]->sizeY);
				}
				if ((Iterator % 1000) == 0) {
					if (rects[i]->getType() == 97) {
						materials += 10 * resource_boost;
					}
					else if (rects[i]->getType() == 102) {
						materials += 20 * resource_boost;
					}
					else if (rects[i]->getType() == 115 || rects[i]->getType() == 103) {
						tanks.push_back(new Infantry(rects[i]->getPosition(), 1, true));
					}
				}
			}
			else if (rects[i]->changeColour == false) {
				if (rects[i]->getType() == 104) {
					supplies -= 5;
					rects[i]->looted = false;
				}
				else if (rects[i]->getType() == 102) {
					victory_points -= 3;
					Victory_point2_lost.play();
				}
				else if(rects[i]->getType() == 115){
					ReCapture1.play();
				}
				else if (rects[i]->getType() == 103) {
					ReCapture1.play();
				}
				rects[i]->changeColour = true;
				string nam = rects[i]->name + to_string(rects[i]->image_type) + ".png";
				rects[i]->rect_image.load(nam);
			}
		}
		if ((Iterator % 30) == 0) {
			rects[i]->update();
			if (rects[i]->kill) {
				rects.erase(rects.begin() + i);
			}
		}
	}
	if (freeplay) {
		for (int i = 0; i < rects.size(); i++) {
			if (rects[i]->getPosition().distance(mouse) < 30 && ofGetKeyPressed(OF_KEY_DEL)) {
				rects.erase(rects.begin() + i);
			}
		}
		for (int i = 0; i < trees.size(); i++) {
			if (trees[i]->getPosition().distance(mouse) < 30 && ofGetKeyPressed(OF_KEY_DEL)) {
				trees.erase(trees.begin() + i);
			}
		}
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->getPosition().distance(mouse) < 30 && ofGetKeyPressed(OF_KEY_DEL)) {
				tanks.erase(tanks.begin() + i);
			}
		}
		for (int i = 0; i < systems.size(); i++) {
			if (systems[i]->getPosition().distance(mouse) < 30 && ofGetKeyPressed(OF_KEY_DEL)) {
				systems.erase(systems.begin() + i);
			}
		}
	}

	// TANK selection
	tank_selection();

	// TANK draw, overlap, update, health check, delete
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofFill();
	ofSetColor(255,255);
	ofSetLineWidth(2);
	for (int i = 0; i < tanks.size(); i++) {
		if ((Iterator % 3) == 0) {
			int amount = 0;
			for (int j = 0; j < tanks.size(); j++) {
				if (i != j) {
					if (tanks[i]->type != 1 && !(tanks[i]->type >= 9)) {
						if (tanks[i]->type != 5 && tanks[j]->type == 13 && tanks[j]->getPosition().distance(tanks[i]->getPosition()) < 90) {
							tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), 90, 20);
							amount++;
						}
						else if (tanks[i]->type != 5 && tanks[j]->type == 14 && tanks[j]->getPosition().distance(tanks[i]->getPosition()) < 280 && tanks[i]->friendly) {
							tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), 140, 9);
							amount++;
						}
						else if (tanks[i]->sizeY >= 48) {
							if (tanks[j]->sizeY >= 48) {
								if (tanks[j]->type == 9) {
									tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), tanks[j]->sizeY * 3, -0.3);
								}
								else {
									tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), tanks[j]->sizeY, 2);
								}
							}
							else {
								tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), tanks[j]->sizeY, 0.1);
							}
						}
						else if (tanks[i]->type == 7 && tanks[j]->type == 8) {
							tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), tanks[j]->sizeY, 0.1);
						}
						else if ((tanks[i]->type == 5 || tanks[j]->type == 5) && !(tanks[j]->type == 5 && tanks[i]->type == 5)) {
							tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), tanks[j]->sizeY, 0.1);
						}
						else {
							tanks[i]->checkOverlap_Unit(tanks[j]->getPosition(), tanks[j]->sizeY, 2);
						}
					}
					else if (tanks[i]->type == 1 && tanks[j]->type == 9) {
						if (tanks[i]->getPosition().distance(tanks[j]->getPosition()) < 10) {
							tanks[i]->health = 0;
							materials += 150 * resource_boost;
						}
					}
					if ((tanks[i]->getPosition().distance(tanks[j]->getPosition()) < 72) && (tanks[i]->friendly) && (tanks[j]->friendly == false)) {
						if (tanks[i]->sizeY >= 48 && tanks[i]->fly == false && tanks[j]->type == 7) {
							tanks[i]->health -= 5;
							tanks[j]->health -= 1;
							tanks[j]->attacking = true;
							if (tanks[i]->type == 4) {
								tanks[j]->health -= 2;
							}
						}
						else if (tanks[i]->type == 7 && tanks[j]->sizeY >= 48 && tanks[j]->fly == false) {
							if (tanks[j]->type <= 8) {
								tanks[j]->health -= 5;
							}
							else {
								tanks[j]->health -= 2;
							}
							tanks[i]->health -= 1;
							tanks[i]->attacking = true;
							if (tanks[j]->type == 4) {
								tanks[i]->health -= 2;
							}
						}
						else if (tanks[j]->type == 8 && tanks[i]->sizeY >= 48) {
							tanks[i]->health -= 14;
							tanks[j]->attacking = true;
						}
						else if (tanks[j]->sizeY >= 48 && tanks[i]->type == 8) {
							if (tanks[j]->type <= 8) {
								tanks[j]->health -= 14;
								tanks[i]->attacking = true;
							}
							else {
								tanks[j]->health -= 3;
								tanks[i]->attacking = true;
							}
						}
						else if (tanks[j]->type == 8 && tanks[i]->type == 7) {
							tanks[j]->health -= 9;
							tanks[i]->health -= 3;
							tanks[j]->attacking = true;
							tanks[i]->attacking = true;
						}
						else if (tanks[j]->type == 7 && tanks[i]->type == 8) {
							tanks[i]->health -= 9;
							tanks[j]->health -= 3;
							tanks[i]->attacking = true;
							tanks[j]->attacking = true;
						}
						else if (tanks[j]->type == 8 && tanks[i]->type == 8) {
							tanks[j]->health -= 3;
							tanks[i]->health -= 3;
							tanks[j]->attacking = true;
							tanks[i]->attacking = true;
						}
						else if (tanks[j]->type == 7 && tanks[i]->type == 7) {
							tanks[j]->health -= 3;
							tanks[i]->health -= 3;
							tanks[j]->attacking = true;
							tanks[i]->attacking = true;
						}
						else {
							tanks[i]->health--;
							tanks[j]->health--;
							tanks[i]->attacking = true;
							tanks[j]->attacking = true;
						}
					}
				}
			}
			if ((Iterator % 9) == 0) {
				for (int j = 0; j < rects.size(); j++) {
					if (rects[j]->getType() == 98) {
						if (tanks[i]->getPosition().distance(rects[j]->getPosition()) < 150 && tanks[i]->type != 9 && tanks[i]->type != 1 && tanks[i]->type != 5) {
							amount++;
						}
						else if (tanks[i]->type == 9 && tanks[i]->friendly) {
							if (tanks[i]->getPosition().distance(rects[j]->getPosition()) < 200) {
								rects[j]->kill = true;
							}
						}
					}
					else if (tanks[i]->type != 1 && tanks[i]->type != 9 && tanks[i]->type != 10 && tanks[i]->type != 11 && tanks[i]->type != 12 && tanks[i]->type != 13 && tanks[i]->type != 14) {
						tanks[i]->checkOverlap_Building(rects[j]->getPosition(), rects[j]->sizeY / 2, rects[j]->capture_status, rects[j]->getType());
					}
				}
			}
			if (amount > 0 && !tanks[i]->throttle) {
				tanks[i]->throttle = true;
			}
		}
		if (tanks[i]->getPosition().distance(centre_vector) < centre_distance && tanks[i]->type != 5) {
			tanks[i]->draw();
			if (show_health && tanks[i]->type != 5) {
				tanks[i]->drawHealth();
			}
		}
		if ((Iterator % 700) == 0) {
			if (tanks[i]->friendly) {
				materials -= 5;
			}
		}

		if(tanks[i]->friendly){
			for (int p = 0; p < projectiles.size(); p++) {
				if (projectiles[p]->friendly == false && (projectiles[p]->position.distance(tanks[i]->getPosition()) < tanks[i]->sizeY)) {
					tanks[i]->health -= projectiles[p]->damage*10;
					explosions.push_back(new Explosion(projectiles[p]->position, projectiles[p]->type));
					projectiles.erase(projectiles.begin() + p);
				}
			}
		}
		else if(tanks[i]->type != 13){
			for (int p = 0; p < projectiles.size(); p++) {
				if (projectiles[p]->friendly && (projectiles[p]->position.distance(tanks[i]->getPosition()) < tanks[i]->sizeY)) {
					if (tanks[i]->type != 14) {
						tanks[i]->health -= projectiles[p]->damage * 10;
						explosions.push_back(new Explosion(projectiles[p]->position, projectiles[p]->type));
						projectiles.erase(projectiles.begin() + p);
					}
					else if (projectiles[p]->position.distance(tanks[i]->getPosition()) < 100){
						tanks[i]->health -= projectiles[p]->damage * 10;
						explosions.push_back(new Explosion(projectiles[p]->position, projectiles[p]->type));
						projectiles.erase(projectiles.begin() + p);
					}
				}
			}
		}
		tanks[i]->update();
		if (tanks[i]->isDead()) {
			if (tanks[i]->friendly) {
				if (tanks[i]->type == 2 || tanks[i]->type == 7 || tanks[i]->type == 8) {
					supplies++;
				}
				else if (tanks[i]->type == 6) {
					supplies += 3;
				}
				else if (tanks[i]->type == 9) {
					supplies += 5;
					materials += 500;
					Victory_point1_lost.play();
				}
				else if(tanks[i]->type != 1){
					supplies += 2;
				}
			}
			else if (tanks[i]->type == 9) {
				victory_points++;
				Victory_point1.play();
			}
			else if (tanks[i]->friendly == false) {
				if (tanks[i]->type == 2) {
					materials += 5 * resource_boost;
				}
				else if (tanks[i]->type == 3) {
					materials += 10 * resource_boost;
				}
				else if (tanks[i]->type == 4) {
					materials += 15 * resource_boost;
				}
				else if (tanks[i]->type == 5) {
					materials += 10 * resource_boost;
				}
				else if (tanks[i]->type == 6) {
					materials += 20 * resource_boost;
				}
			}
			tanks.erase(tanks.begin() + i);
		}
	}
	if (ofGetKeyPressed(OF_KEY_DEL)) {
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->selected) {
				tanks[i]->health -= 5000;
			}
		}
	}

	// TREES draw
	for (int i = 0; i < trees.size(); i++) {
		if (trees[i]->getType() == 106 && trees[i]->getPosition().distance(centre_vector) < centre_distance) {
			trees[i]->draw();
		}
	}

	// TANK TURRETS draw and SOUND
	ofSetColor(0,180);
	ofFill();
	float line_width = 6 - (zOffset / 230);
	ofSetLineWidth(line_width);
	int T1_sound = 0; int T2_sound = 0; int T3_sound = 0; int T4_sound = 0; int T5_sound = 0; int T6_sound = 0;
	int C1_sound = 0;
	int H_sound = 1;
	if ((Iterator % 30) == 0) {
		H_sound = 0;
	}

	for (int i = 0; i < tanks.size(); i++) {
		if (tanks[i]->type == 5) {
			ofSetColor(255);
			tanks[i]->draw();
			if (show_health) {
				tanks[i]->drawHealth();
			}
			ofSetColor(0, 180);
			if (H_sound < 1 && tanks[i]->getPosition().distance(centre_vector) < centre_distance) {
				H_mov.play();
			}
		}
		else if (spawnmode && tanks[i]->type == 9 && tanks[i]->friendly) {
			ofNoFill();
			ofDrawCircle(tanks[i]->getPosition(), 200);
			ofFill();
		}
		float rotation = 0;
		if (tanks[i]->attacking) {
			rotation = -tanks[i]->turret_trajectory.angle(ofVec2f(0, -1));
			if (tanks[i]->getPosition().distance(centre_vector) < centre_distance) {
				if (tanks[i]->turret_type == 2) {
					if (tanks[i]->shot && T1_sound < 1) {
						T1.play();
						T1_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->turret_type == 3) {
					if (tanks[i]->shot && T2_sound < 1) {
						T2.play();
						T2_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->turret_type == 4) {
					if (tanks[i]->shot && T3_sound < 1) {
						T3.play();
						T3_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->turret_type == 5) {
					if (tanks[i]->shot && T4_sound < 1) {
						T4.play();
						T4_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->turret_type == 6) {
					if (tanks[i]->shot && T5_sound < 1) {
						T5.play();
						T5_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->turret_type == 8) {
					if (tanks[i]->shot && T6_sound < 1) {
						T2.play();
						T6_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->turret_type == 9) {
					if (tanks[i]->shot && T3_sound < 1) {
						T3.play();
						T3_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->turret_type == 10) {
					if (tanks[i]->shot && T1_sound < 1) {
						T1.play();
						T1_sound++;
						tanks[i]->shot = false;
					}
				}
				else if (tanks[i]->type == 8 || tanks[i]->type == 7) {
					tanks[i]->iterator++;
					if ((tanks[i]->iterator % 30) == 0 && C1_sound < 1) {
						float ran = ofRandom(0, 4);
						if (ran < 1) {
							C1.play();
						}
						else if (ran >= 1 && ran < 2) {
							C2.play();
						}
						else if (ran >= 2 && ran < 3) {
							C3.play();
						}
						else {
							C4.play();
						}
						C1_sound++;
					}
					if (tanks[i]->type == 7 && I1.isPlaying() == false) {
						I1.play();
					}
				}
			}
		}
		else if (tanks[i]->trajectory.x != 0 && tanks[i]->getPosition().distance(centre_vector) < centre_distance) {
			if (tanks[i]->type == 8) {
				if (C5.isPlaying() == false) {
					C5.play();
				}
			}
			else if (tanks[i]->type == 2 || tanks[i]->type == 3 || tanks[i]->type == 4 || tanks[i]->type == 6 || tanks[i]->type == 9) {
				if (T_mov.isPlaying() == false) {
					T_mov.play();
				}
			}
		}
		if (tanks[i]->isTurret && tanks[i]->type != 5) {
			if (tanks[i]->attacking == false) {
				rotation = -tanks[i]->trajectory.angle(ofVec2f(0, -1));
			}
			ofPushMatrix();
			ofTranslate(tanks[i]->getPosition());
			ofRotate(rotation);
			ofDrawLine(0, 0, 0, -tanks[i]->sizeY / 1.5);
			ofPopMatrix();
		}
		if ((tanks[i]->selected && tanks[i]->target.x != 0) || tanks[i]->type == 1) {
			ofSetLineWidth(0.5);
			ofDrawLine(tanks[i]->getPosition(), tanks[i]->target);
			ofSetLineWidth(line_width);
		}
	}

	ofSetColor(0);
	// PROJECTILES draw, delete
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i]->draw();
		if (projectiles[i]->hit) {
			explosions.push_back(new Explosion(projectiles[i]->position, projectiles[i]->type));
			projectiles.erase(projectiles.begin() + i);
		}
	}

	ofSetColor(255);
	// EXPLOSIONS draw, delete
	for (int i = 0; i < explosions.size(); i++) {
		explosions[i]->draw();
		if (explosions[i]->getIsDead()) {
			explosions.erase(explosions.begin() + i);
		}
	}

	if (global_key == 110) {
		int sizeX = 80;
		int sizeY = 40;
		if (rotation == 2 || rotation == 4) {
			sizeX = 40;
			sizeY = 80;
		}
		ofSetColor(0, 50);
		ofDrawRectangle(mouse.x,mouse.y,sizeX,sizeY);
	}
	if (global_key == 109) {
		int sizeX = 240;
		int sizeY = 120;
		if (rotation == 2 || rotation == 4) {
			sizeX = 120;
			sizeY = 240;
		}
		ofSetColor(0, 50);
		ofDrawRectangle(mouse.x, mouse.y, sizeX, sizeY);
	}

	// PARTICLES DRAW
	ofSetColor(40, 160);
	for (int i = 0; i < systems.size(); i++) {
		if ((Iterator % 2) == 0) {
			systems[i]->update();
		}
		systems[i]->drawSmoke();
	}

	cam.end();

	// optimize for framerate
	
	if ((Iterator % 20) == 0) {
		float framerate = ofGetFrameRate();
		float inc = 1.3; float inc2 = 2;
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->speed > 0) {
				if (framerate < 15 && framerate >= 10) {
					tanks[i]->speed = tanks[i]->speed_buffer;
					tanks[i]->speed *= inc;
				}
				else if (framerate < 10) {
					tanks[i]->speed = tanks[i]->speed_buffer;
					tanks[i]->speed *= inc2;
				}
				else {
					tanks[i]->speed = tanks[i]->speed_buffer;
				}
			}
			if (tanks[i]->isTurret) {
				if (framerate < 15 && framerate >= 10) {
					tanks[i]->std_reload_time = tanks[i]->reload_time_buffer;
					tanks[i]->std_reload_time *= inc;
				}
				else if (framerate < 10) {
					tanks[i]->std_reload_time = tanks[i]->reload_time_buffer;
					tanks[i]->std_reload_time *= inc2;
				}
				else {
					tanks[i]->std_reload_time = tanks[i]->reload_time_buffer;
				}
			}
		}
		if (framerate < 15 && framerate >= 10) {
			resource_boost = inc;
		}
		else if (framerate < 10) {
			resource_boost = inc2;
		}
		else {
			resource_boost = 1;
		}
	}
	




	// SPAWN units
	ofSetColor(255);
	if (spawnmode && freeplay == false) {
		ofPushMatrix();
		ofTranslate(0, ofGetHeight() / 2 - 200);
		sel2.draw(-700, 0, 150, 150);
		sel3.draw(-500, 0, 150, 150);
		sel4.draw(-300, 0, 150, 150);
		if (unlock1) {
			sel5.draw(-100, 0, 150, 150);
		}
		if (unlock2) {
			sel6.draw(100, 0, 150, 150);
		}
		sel7.draw(300, 0, 150, 150);
		if (unlock3) {
			sel8.draw(500, 0, 150, 150);
		}
		if (unlock4) {
			sel9.draw(700, 0, 150, 150);
		}

		img2.draw(-700, 0, 70, 60);
		img3.draw(-500, 0, 80, 80);
		img4.draw(-300, 0, 80, 80);
		if (unlock1) {
			img5.draw(-100, 0, 100, 65);
		}
		if (unlock2) {
			img6.draw(100, 0, 70, 110);
		}
		img7.draw(300, 0, 70, 70);
		if (unlock3) {
			img8.draw(500, 0, 20, 50);
		}
		if (unlock4) {
			img9.draw(700, 0, 135, 105);
		}
		ofPopMatrix();

		if (ofGetMouseY() >= 800 && ofGetMouseY() <= 910) {
			if (ofGetMouseX() >= 180 && ofGetMouseX() <= 340) {
				info2.draw(0,0,300,400);
				if (ofGetMousePressed()) {
					spawn_key = 50;
					spawn_selected = "Light Tank";
				}
			}
			else if (ofGetMouseX() >= 380 && ofGetMouseX() <= 540) {
				info3.draw(0, 0, 300, 400);
				if (ofGetMousePressed()) {
					spawn_key = 51;
					spawn_selected = "Heavy Tank";
				}
			}
			else if (ofGetMouseX() >= 580 && ofGetMouseX() <= 740) {
				info4.draw(0, 0, 300, 400);
				if (ofGetMousePressed()) {
					spawn_key = 52;
					spawn_selected = "Bulldozer";
				}
			}
			else if (ofGetMouseX() >= 780 && ofGetMouseX() <= 940 && unlock1) {
				info5.draw(0, 0, 300, 400);
				if (ofGetMousePressed()) {
					spawn_key = 53;
					spawn_selected = "Interceptor";
				}
			}
			else if (ofGetMouseX() >= 980 && ofGetMouseX() <= 1140 && unlock2) {
				info6.draw(0, 0, 300, 400);
				if (ofGetMousePressed()) {
					spawn_key = 54;
					spawn_selected = "Artillery";
				}
			}
			else if (ofGetMouseX() >= 1080 && ofGetMouseX() <= 1340) {
				info7.draw(0, 0, 300, 400);
				if (ofGetMousePressed()) {
					spawn_key = 55;
					spawn_selected = "Infantry";
				}
			}
			else if (ofGetMouseX() >= 1280 && ofGetMouseX() <= 1540 && unlock3) {
				info8.draw(0, 0, 300, 400);
				if (ofGetMousePressed()) {
					spawn_key = 56;
					spawn_selected = "Cavalry";
				}
			}
			else if (ofGetMouseX() >= 1480 && ofGetMouseX() <= 1740 && unlock4) {
				info9.draw(0, 0, 300, 400);
				if (ofGetMousePressed()) {
					spawn_key = 57;
					spawn_selected = "Golem";
				}
			}
		}
		int amount_golems = 0;
		ofSetColor(255);
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->type == 9 && tanks[i]->friendly) {
				amount_golems++;
				if ((Mouse_iterator % 2) == 0) {
					if (ofGetMousePressed) {
						if (mouse.distance(tanks[i]->getPosition()) < 200) {
							if (spawn_key == 50) {
								if (materials >= 50 && supplies >= 1) {
									materials -= 50;
									tanks.push_back(new Tank(mouse, 2, true));
									supplies--;
								}
							}
							else if (spawn_key == 51) {
								if (materials >= 100 && supplies >= 2) {
									materials -= 100;
									tanks.push_back(new Tank(mouse, 3, true));
									supplies -= 2;
								}
							}
							else if (spawn_key == 52) {
								if (materials >= 200 && supplies >= 2) {
									materials -= 200;
									tanks.push_back(new Tank(mouse, 4, true));
									supplies -= 2;
								}
							}
							else if (spawn_key == 53) {
								if (materials >= 200 && supplies >= 2 && unlock1) {
									materials -= 200;
									tanks.push_back(new Tank(mouse, 5, true));
									supplies -= 2;
								}
							}
							else if (spawn_key == 54) {
								if (materials >= 300 && supplies >= 3 && unlock2) {
									materials -= 300;
									tanks.push_back(new Tank(mouse, 6, true));
									supplies -= 3;
								}
							}
							else if (spawn_key == 55) {
								if (materials >= 50) {
									materials -= 50;
									supplies--;
									tanks.push_back(new Infantry(mouse, 7, true));
								}
							}
							else if (spawn_key == 56) {
								if (materials >= 50 && unlock3) {
									materials -= 50;
									supplies--;
									tanks.push_back(new Infantry(mouse, 8, true));
								}
							}
							Mouse_iterator++;
						}
					}
				}
			}
		}
		if (amount_golems < 1) {
			no_golem = true;
		}
		else if (no_golem) {
			no_golem = false;
		}

		if ((Mouse_iterator % 2) == 0) {
			if (ofGetMousePressed) {
				if (spawn_key == 57) {
					bool alive = false;
					for (int i = 0; i < tanks.size(); i++) {
						if (tanks[i]->type == 9 && tanks[i]->friendly) {
							alive = true;
						}
					}
					if (alive) {
						if (materials >= 1500 && supplies >= 5 && unlock4) {
							materials -= 1500;
							supplies -= 5;
							victory_points++;
							Victory_point1.play();
							tanks.push_back(new Tank(mouse, 9, true));
						}
					}
					else {
						materials -= 500;
						victory_points -= 2;
						Victory_point1_lost.play();
						tanks.push_back(new Tank(mouse, 9, true));
					}
					Mouse_iterator++;
				}
			}
		}
	}

	// DRAW STRINGS
	if (menu == false) {
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);

		if (spawnmode) {
			ofSetColor(0);
			ofDrawRectangle(10, 70, 350, 40);
			ofDrawRectangle(10, 120, 350, 40);
			ofDrawRectangle(10, 170, 350, 40);
			ofDrawRectangle(10, 220, 350, 40);
			ofDrawRectangle(10, 270, 350, 40);
			ofDrawRectangle(10, 320, 350, 40);

			ofDrawRectangle(1470, 70, 550, 70);
		}

		ofSetColor(255);
		top_bar.draw(0, 10, ofGetWidth(), 50);

		if (no_golem) {
			ofPushMatrix();
			ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
			victory.drawString("PLACE YOUR GOLEM", -280, 0);
			disclaimer.drawString("move your mouse to your chosen spawn location and press 9", -650, 60);
			ofPopMatrix();
		}

		materials_font.drawString(to_string(materials), 165, 43);
		supplies_font.drawString(to_string(supplies), 410, 43);
		points_font.drawString(to_string(victory_points), 1600, 43);
		timer_font_.drawString(to_string(globaltimer), 1800, 43);
		if (spawnmode) {
			spawnmode_info.drawString("Search and Destroy: TAB", 20, 100);
			spawnmode_info.drawString("Reclaim: R", 20, 150);
			spawnmode_info.drawString("Show health: H", 20, 200);
			spawnmode_info.drawString("Toggle Golem Movement: L", 20, 250);
			spawnmode_info.drawString("Delete: Del", 20, 300);
			spawnmode_info.drawString("Change framerate: +/-", 20, 350);
			spawnmode_info.drawString("FPS " + to_string(framerate), 20, 400);
			selection.drawString("Selection: " + spawn_selected, 1500, 120);
		}
		if (victory_points >= 10) {
			ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
			victory.drawString("VICTORY", -150, 0);
			if (victory_points < 15) {
				victory_points = 15;
				Iterator = -200;
				cout << "you win" << endl;
				scenario++;
				std::ofstream f("scenario.txt");
				f << scenario;
				if (Victory_sound.isPlaying() == false) {
					Victory_sound.play();
				}
			}
			if (Iterator >= 0) {
				menu = true;
				start_playing = false;
				rects.clear();
				tanks.clear();
				trees.clear();
				projectiles.clear();
				systems.clear();
			}
		}
		else if (victory_points <= -4) {
			ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
			victory.drawString("DEFEAT", -150, 0);
			if (victory_points > -10) {
				victory_points = -15;
				Iterator = -200;
				cout << "you lost" << endl;
				if (Victory_sound.isPlaying() == false) {
					Victory_sound.play();
				}
			}
			if (Iterator >= 0) {
				menu = true;
				start_playing = false;
				rects.clear();
				tanks.clear();
				trees.clear();
				projectiles.clear();
				systems.clear();
			}
		}
	}
	else {
		ofBackground(0);
		menu_image.draw(0, 0);
		/*
		std::ifstream in("scenario.txt");
		string line;
		while (getline(in, line)) {
			scenario = stoi(line);
		}
		*/
		ofSetColor(0);
		scenario_string.drawString("Scenario " + to_string(scenario), 280, 400);
		scenario_note_string.drawString("(you can select scenarios with number keys)", 220, 440);
	}

	// SCREEN MOVEMENT with arrow keys
	if (ofGetKeyPressed(OF_KEY_RIGHT)) {
		xOff += 50;
		cam.truck(50);
	}
	if (ofGetKeyPressed(OF_KEY_LEFT)) {
		xOff -= 50;
		cam.truck(-50);
	}
	if (ofGetKeyPressed(OF_KEY_UP)) {
		yOff += 50;
		cam.boom(50);
	}
	if (ofGetKeyPressed(OF_KEY_DOWN)) {
		yOff -= 50;
		cam.boom(-50);
	}
	else if (ofGetKeyPressed(OF_KEY_RETURN)) {
		show_ranges = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	// RECLAIM command
	if (key == 'r') {
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->selected && tanks[i]->friendly && tanks[i]->sizeY < 48) {
				tanks[i]->reclaim = true;
				if (reclaim_order.isPlaying() == false) {
					reclaim_order.play();
				}
			}
		}
	}
	else if (key == 's') {
		if (freeplay) {
			for (int i = 0; i < tanks.size(); i++) {
				if (tanks[i]->type != 9 && tanks[i]->type != 10 && tanks[i]->type != 11 && tanks[i]->type != 12 && tanks[i]->type != 13 && tanks[i]->type != 14) {
					if (tanks[i]->friendly == false && tanks[i]->sizeY < 48) {
						tanks[i]->reclaim = true;
					}
				}
			}
		}
	}

	// SEARCH AND DESTROY command
	/*
	else if (key == 'a') {
		if (freeplay) {
			for (int i = 0; i < tanks.size(); i++) {
				if (tanks[i]->type != 9 && tanks[i]->type != 10 && tanks[i]->type != 11 && tanks[i]->type != 12 && tanks[i]->type != 13 && tanks[i]->type != 14) {
					if (tanks[i]->friendly == false) {
						tanks[i]->reclaim = false;
						tanks[i]->search_and_destroy = true;
					}
				}
			}
		}
	}
	*/
	else if (key == OF_KEY_TAB) {
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->selected && tanks[i]->friendly) {
				tanks[i]->reclaim = false;
				tanks[i]->search_and_destroy = true;
				if (target_acquired.isPlaying() == false) {
					target_acquired.play();
				}
			}
		}
	}

	// LOCK command for moving golems
	else if (key == 'l') {
		if (lock) {
			lock = false;
		}
		else {
			lock = true;
		}
	}

	// TOGGLE spawmode
	else if ((GetKeyState(VK_SPACE) & 0x100) != 0) {
		if (spawnmode) {
			spawnmode = false;
			Mouse_iterator = 0;
		}
		else {
			spawnmode = true;
			Mouse_iterator = 0;
		}
	}

	// TOGGLE buildmode
	else if (key == '?') {
		if (freeplay) {
			if (buildmode) {
				buildmode = false;
				cout << "buildmode off" << endl;
			}
			else {
				if ((Iterator % 100) == 0) {
					Iterator++;
				}
				rotation = 1;
				buildmode = true;
				cout << "buildmode on" << endl;
			}
		}
	}

	// MISC
	else if (key == 'i') {
		cout << "units: " << tanks.size() << endl;
		cout << "projectiles: " << projectiles.size() << endl;
		cout << "rects: " << rects.size() << endl;
		cout << "trees: " << trees.size() << endl;
		cout << ofGetFrameRate() << endl;
	}
	if (key == 'h') {
		if (show_health) {
			show_health = false;
		}
		else {
			show_health = true;
		}
	}
	if (key != 'r') {
		if (spawnmode) {
			spawn_key = key;
			if (key == 50) {
				spawn_selected = "Light Tank";
			}
			else if (key == 51) {
				spawn_selected = "Heavy Tank";
			}
			else if (key == 52) {
				spawn_selected = "Bulldozer";
			}
			else if (key == 53 && unlock1) {
				spawn_selected = "Interceptor";
			}
			else if (key == 54 && unlock2) {
				spawn_selected = "Artillery";
			}
			else if (key == 55) {
				spawn_selected = "Infantry";
			}
			else if (key == 56 && unlock3) {
				spawn_selected = "Cavalry";
			}
			else if (key == 57) {
				spawn_selected = "Golem";
			}
		}
		else if(key != OF_KEY_UP && key != OF_KEY_DOWN && key != OF_KEY_LEFT && key != OF_KEY_RIGHT){
			global_key = key;
		}
		//cout << global_key << endl;
	}
	else {
		rotation++;
		if (rotation >= 5) {
			rotation = 1;
		}
	}
	if (key == '*') {
		//savegame();
	}
	else if (key == '~') {
		//loadgame();
	}

	// Scenario Picker
	if (menu) {
		int max_scenario = 0;
		std::ifstream in("scenario.txt");
		string line;
		while (getline(in, line)) {
			max_scenario = stoi(line);
		}
		switch (key) {
		case 49:
			scenario = 1;
			break;
		case 50:
			scenario = 2;
			break;
		case 51:
			if (max_scenario >= 3) {
				scenario = 3;
			}
			break;
		case 52:
			if (max_scenario >= 4) {
				scenario = 4;
			}
			break;
		case 53:
			if (max_scenario >= 5) {
				scenario = 5;
			}
			break;
		case 54:
			if (max_scenario >= 6) {
				scenario = 6;
			}
			break;
		default:
			break;
		}
	}

	// set AI targets
	if (freeplay) {
		if (key == ',') {
			primary_AI_targets.push_back(new ofVec2f(mouse));
		}
		if (key == '_') {
			for (int i = 0; i < tanks.size(); i++) {
				if (tanks[i]->type >= 1 && tanks[i]->type <= 9) {
					tanks.erase(tanks.begin() + i);
				}
			}
		}
	}
	if (key == '&') {
		victory_points++;
	}

	// set game speed
	if (key == '+' && (framerate < 60)) {
		framerate++;
		ofSetFrameRate(framerate);
	}
	else if (key == '-' && (framerate > 20)) {
		framerate--;
		ofSetFrameRate(framerate);
	}

	// give up
	if (key == '%') {
		victory_points = -4;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_RETURN) {
		show_ranges = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	// SPAWN turrets
	if (freeplay) {
		if (global_key == 57) {
			tanks.push_back(new Tank(mouse, 9, true));
		}
		if (global_key == 48) {
			tanks.push_back(new Tank(mouse, 9, false));
		}
		else if (global_key == 121) {
			tanks.push_back(new Static_Turret(mouse, 10, rotation));
		}
		else if (global_key == 120) {
			tanks.push_back(new Static_Turret(mouse, 11, rotation));
		}
		else if (global_key == 99) {
			tanks.push_back(new Static_Turret(mouse, 12, rotation));
		}
		else if (global_key == 110) {
			tanks.push_back(new Static_Turret(mouse, 13, rotation));
		}
		else if (global_key == 109) {
			tanks.push_back(new Static_Turret(mouse, 14, rotation));
		}
		// build resources
		if (buildmode) {
			if (global_key == 107) {
				trees.push_back(new Rect(mouse, 107, rotation, false));
			}
			else if (global_key == 106) {
				trees.push_back(new Rect(mouse, 106, rotation, false));
			}
		}
		// BUILD rects
		if (buildmode) {
			if (global_key == 97 || global_key == 98 || global_key == 115 || global_key == 102 || global_key == 103 || global_key == 104) {
				if (global_key == 103) {
					if (rotation == 1) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(-60, 45), 3));
						systems.push_back(new ParticleSystem(mouse + ofVec2f(60, 10), 3));
					}
					else if (rotation == 2) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(50, 50), 3));
						systems.push_back(new ParticleSystem(mouse + ofVec2f(10, -50), 3));
					}
					else if (rotation == 3) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(-60, -10), 3));
						systems.push_back(new ParticleSystem(mouse + ofVec2f(60, -45), 3));
					}
					else if (rotation == 4) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(-50, -50), 3));
						systems.push_back(new ParticleSystem(mouse + ofVec2f(10, 50), 3));
					}
				}
				else if (global_key == 104) {
					if (rotation == 1) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(-20, 10), 2));
					}
					else if (rotation == 2) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(10, 20), 2));
					}
					else if (rotation == 3) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(20, -10), 2));
					}
					else if (rotation == 4) {
						systems.push_back(new ParticleSystem(mouse + ofVec2f(-25, -10), 2));
					}
				}
				rects.push_back(new Rect(mouse, global_key, rotation, false));
			}
		}
	}

	// (FREEPLAY) SPAWN units
	if (freeplay) {
		if (spawnmode) {
			if (global_key == 49) {
				tanks.push_back(new Infantry(mouse, 1, true));
			}
			else if (global_key == 50) {
				tanks.push_back(new Tank(mouse, 2, true));
			}
			else if (global_key == 51) {
				tanks.push_back(new Tank(mouse, 3, true));
			}
			else if (global_key == 52) {
				tanks.push_back(new Tank(mouse, 4, true));
			}
			else if (global_key == 53) {
				tanks.push_back(new Tank(mouse, 5, true));
			}
			else if (global_key == 54) {
				tanks.push_back(new Tank(mouse, 6, true));
			}
			else if (global_key == 55) {
				tanks.push_back(new Infantry(mouse, 7, true));
			}
			else if (global_key == 56) {
				tanks.push_back(new Infantry(mouse, 8, true));
			}
			Mouse_iterator++;
		}


		// SPAWN enemies
		if (global_key == 113) {
			tanks.push_back(new Tank(mouse, ofRandom(2, 7), false));
		}
		else if (global_key == 119) {
			tanks.push_back(new Infantry(mouse, ofRandom(7, 9), false));
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	Mouse_iterator++;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}
void  ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	// SCROLLING
	if (zOffset <= 920) {
		if (scrollY < 0) {
			xOffset -= 84;
			cam.dolly(80);
			zOffset += 120;
			yOffset -= 48;
		}
	}
	if (zOffset >= -460) {
		if (scrollY > 0) {
			xOffset += 84;
			cam.dolly(-80);
			zOffset -= 120;
			yOffset += 48;
		}
	}
}

vector<string> tokenise(string input, char sep) {
	vector<string> vec;
	int end;
	int start = input.find_first_not_of(sep, 0);
	string token;
	//  std::cout << "processing '"<< input << "'" << input.length() << std::endl;
	// find index of sep
	do {
		end = input.find_first_of(sep, start);
		//std::cout << start << " - " << end << std::endl;
		if (start == input.length() || start == end) {// whitespace at the end
			break;
		}
		if (end >= 0) {// we found it - use end for substr
			token = input.substr(start, end - start);
		}
		else { // we did not find it, use the remaining length of the string
			token = input.substr(start, input.length() - start);
		}
		//std::cout << "got token " << token << std::endl;
		vec.push_back(token);
		// did we find it?
		start = end + 1;
	} while (end > 0);  // yes? continue
	return vec;
}

void ofApp::savegame() {
	string directory = "scenario" + to_string(scenario);
	std::ofstream f(directory + "/" + "positions.txt");
	std::ofstream f2(directory + "/" + "types.txt");
	std::ofstream f3(directory + "/" + "river.txt");
	std::ofstream f4(directory + "/" + "resource_positions.txt");
	std::ofstream f5(directory + "/" + "unit_positions.txt");
	std::ofstream f6(directory + "/" + "unit_types.txt");
	std::ofstream f7(directory + "/" + "friendly.txt");
	std::ofstream f8(directory + "/" + "unit_friendly.txt");
	std::ofstream f9(directory + "/" + "rotation.txt");
	std::ofstream f10(directory + "/" + "resource_types.txt");
	std::ofstream f11(directory + "/" + "AI_targets.txt");
	std::ofstream f12(directory + "/" + "smoke.txt");
	std::ofstream f13(directory + "/" + "rect_rotations.txt");
	vector<ofVec2f> positions;
	vector<int> types;
	vector<int> friendly;
	vector<int> rect_rotations;
	vector<ofPoint> river_positions;
	vector<ofVec2f> resource_positions;
	vector<int> resource_types;
	vector<ofVec2f> unit_positions;
	vector<int> unit_types;
	vector<int> unit_friendly;
	vector<int> rotation;
	vector<ofVec2f*> AI_targets;
	vector<ofVec2f> smoke;
	for (int i = 0; i < rects.size(); i++) {
		positions.push_back(rects[i]->getPosition());
		types.push_back(rects[i]->getType());
		friendly.push_back(rects[i]->friendly);
		rect_rotations.push_back(rects[i]->rotation);
	}
	for (int i = 0; i < tanks.size(); i++) {
		unit_positions.push_back(tanks[i]->getPosition());
		unit_types.push_back(tanks[i]->type);
		unit_friendly.push_back(tanks[i]->friendly);
		if (tanks[i]->type == 13 || tanks[i]->type == 14) {
			rotation.push_back(tanks[i]->rotation);
		}
		else {
			rotation.push_back(5);
		}
	}
	for (int i = 0; i < trees.size(); i++) {
		resource_positions.push_back(trees[i]->getPosition());
		resource_types.push_back(trees[i]->getType());
	}
	for (int i = 0; i < systems.size(); i++) {
		smoke.push_back(systems[i]->getPosition());
	}
	AI_targets = primary_AI_targets;
	for (vector<ofVec2f>::const_iterator i = positions.begin(); i != positions.end(); ++i) {
		f << *i << '\n';
	}
	for (vector<int>::const_iterator i = types.begin(); i != types.end(); ++i) {
		f2 << *i << '\n';
	}
	for (vector<ofPoint>::const_iterator i = river_positions.begin(); i != river_positions.end(); ++i) {
		f3 << *i << "\n";
	}
	for (vector<ofVec2f>::const_iterator i = resource_positions.begin(); i != resource_positions.end(); ++i) {
		f4 << *i << "\n";
	}
	for (vector<ofVec2f>::const_iterator i = unit_positions.begin(); i != unit_positions.end(); ++i) {
		f5 << *i << '\n';
	}
	for (vector<int>::const_iterator i = unit_types.begin(); i != unit_types.end(); ++i) {
		f6 << *i << '\n';
	}
	for (vector<int>::const_iterator i = friendly.begin(); i != friendly.end(); ++i) {
		f7 << *i << '\n';
	}
	for (vector<int>::const_iterator i = unit_friendly.begin(); i != unit_friendly.end(); ++i) {
		f8 << *i << '\n';
	}
	for (vector<int>::const_iterator i = rotation.begin(); i != rotation.end(); ++i) {
		f9 << *i << '\n';
	}
	for (vector<int>::const_iterator i = resource_types.begin(); i != resource_types.end(); ++i) {
		f10 << *i << "\n";
	}
	for (vector<ofVec2f*>::const_iterator i = AI_targets.begin(); i != AI_targets.end(); ++i) {
		f11 << *i << "\n";
	}
	for (vector<ofVec2f>::const_iterator i = smoke.begin(); i != smoke.end(); ++i) {
		f12 << *i << "\n";
	}
	for (vector<int>::const_iterator i = rect_rotations.begin(); i != rect_rotations.end(); ++i) {
		f13 << *i << "\n";
	}

	cout << "saved successfully" << endl;
}
void ofApp::loadgame() {
	string directory = "scenario" + to_string(scenario);
	vector<ofVec2f> positions;
	vector<int> types;
	vector<int> friendly;
	vector<int> rect_rotations;
	vector<ofVec2f> unit_positions;
	vector<int> unit_types;
	vector<int> unit_friendly;
	vector<int> rotation;
	vector<ofPoint> river_positions_a;
	vector<ofPoint> river_positions_b;
	vector<ofVec2f> resources;
	vector<ofVec2f> AI_targets;
	vector<int> resource_types;
	vector<string> strings;
	string line;

	std::ifstream in(directory + "/" + "positions.txt");
	while (getline(in, line)) {
		strings.push_back(line);
	}
	for (int i = 0; i < strings.size(); i++) {
		vector<string> tokenised_strings = tokenise(strings[i], ',');
		int x = stoi(tokenised_strings[0]);
		int y = stoi(tokenised_strings[1]);
		positions.push_back(ofVec2f(x, y));
	}
	std::ifstream in2(directory + "/" + "types.txt");
	while (getline(in2, line)) {
		types.push_back(stoi(line));
	}
	std::ifstream in7(directory + "/" + "friendly.txt");
	while (getline(in7, line)) {
		friendly.push_back(stoi(line));
	}
	std::ifstream in13(directory + "/" + "rect_rotations.txt");
	while (getline(in13, line)) {
		rect_rotations.push_back(stoi(line));
	}
	for (int i = 0; i < positions.size(); i++) {
		if (scenario == 4) {
			if (types[i] == 97 || types[i] == 115 || types[i] == 103 || types[i] == 104) {
				friendly[i] = true;
			}
			Rect r(positions[i], types[i], rect_rotations[i], friendly[i]);
			if (types[i] == 97 || types[i] == 115 || types[i] == 103 || types[i] == 104) {
				string nam = r.name + to_string(r.image_type + 1) + ".png";
				r.rect_image.load(nam);
				r.rect_image.resize(r.sizeX, r.sizeY);
				r.looted = true;
				r.capture_status = 0;
				r.changeColour = false;
			}
			if (types[i] != 102) {
				rects.push_back(new Rect(r));
			}
		}
		else {
			rects.push_back(new Rect(positions[i], types[i], rect_rotations[i], friendly[i]));
		}
	}

	strings.clear();
	std::ifstream in5(directory + "/" + "unit_positions.txt");
	while (getline(in5, line)) {
		strings.push_back(line);
	}
	for (int i = 0; i < strings.size(); i++) {
		vector<string> tokenised_strings = tokenise(strings[i], ',');
		int x = stoi(tokenised_strings[0]);
		int y = stoi(tokenised_strings[1]);
		unit_positions.push_back(ofVec2f(x, y));
	}
	std::ifstream in6(directory + "/" + "unit_types.txt");
	while (getline(in6, line)) {
		unit_types.push_back(stoi(line));
	}
	std::ifstream in8(directory + "/" + "unit_friendly.txt");
	while (getline(in8, line)) {
		unit_friendly.push_back(stoi(line));
	}
	std::ifstream in9(directory + "/" + "rotation.txt");
	while (getline(in9, line)) {
		rotation.push_back(stoi(line));
	}
	for (int i = 0; i < unit_positions.size(); i++) {
		if (unit_types[i] == 2 || unit_types[i] == 3 || unit_types[i] == 4 || unit_types[i] == 5 || unit_types[i] == 6 || unit_types[i] == 9) {
			tanks.push_back(new Tank(unit_positions[i], unit_types[i], unit_friendly[i]));
		}
		if (unit_types[i] == 1 || unit_types[i] == 7 || unit_types[i] == 8) {
			tanks.push_back(new Infantry(unit_positions[i], unit_types[i], unit_friendly[i]));
		}
		else if(unit_types[i] == 10 || unit_types[i] == 11 || unit_types[i] == 12 || unit_types[i] == 13 || unit_types[i] == 14){
			tanks.push_back(new Static_Turret(unit_positions[i], unit_types[i], rotation[i]));
		}
	}

	std::ifstream in10(directory + "/" + "resource_types.txt");
	while (getline(in10, line)) {
		resource_types.push_back(stoi(line));
	}

	strings.clear();
	std::ifstream in4(directory + "/" + "resource_positions.txt");
	while (getline(in4, line)) {
		strings.push_back(line);
	}
	for (int i = 0; i < strings.size(); i++) {
		vector<string> tokenised_strings = tokenise(strings[i], ',');
		int x = stoi(tokenised_strings[0]);
		int y = stoi(tokenised_strings[1]);
		trees.push_back(new Rect(ofVec2f(x, y), resource_types[i], ofRandom(1, 4), false));
	}

	strings.clear();
	std::ifstream in11(directory + "/" + "AI_targets.txt");
	while (getline(in11, line)) {
		strings.push_back(line);
	}
	for (int i = 0; i < strings.size(); i++) {
		vector<string> tokenised_strings = tokenise(strings[i], ',');
		int x = stoi(tokenised_strings[0]);
		int y = stoi(tokenised_strings[1]);
		primary_AI_targets.push_back(new ofVec2f(x, y));
	}
	if (scenario != 1) {
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->friendly == false && tanks[i]->type != 9 && tanks[i]->type != 10 && tanks[i]->type != 11 && tanks[i]->type != 12 && tanks[i]->type != 13 && tanks[i]->type != 14) {
				tanks[i]->search_and_destroy = true;
			}
		}
	}

	strings.clear();
	std::ifstream in12(directory + "/" + "smoke.txt");
	while (getline(in12, line)) {
		strings.push_back(line);
	}
	for (int i = 0; i < strings.size(); i++) {
		vector<string> tokenised_strings = tokenise(strings[i], ',');
		int x = stoi(tokenised_strings[0]);
		int y = stoi(tokenised_strings[1]);
		systems.push_back(new ParticleSystem(ofVec2f(x, y),3));
	}
	cout << "loaded successfully" << endl;
}

void ofApp::tank_Turrets_Reclaim_SearchandDestroy() {
	if ((Iterator % 10) == 0) {
		for (int j = 0; j < tanks.size(); j++) {
			if (tanks[j]->isTurret) {
				tanks[j]->turret_target = tanks[j]->turret_standard_target;
				tanks[j]->calculateTurretTrajectory(tanks[j]->turret_standard_target, 2);
				if (tanks[j]->friendly) {
					vector<ofVec2f> targets;
					vector<int> types;
					for (int i = 0; i < tanks.size(); i++) {
						if (i != j) {
							if (tanks[i]->friendly == false) {
								if ((tanks[j]->getPosition().distance(tanks[i]->getPosition())) <= tanks[j]->range && tanks[i]->isDead() == false) {
									targets.push_back(tanks[i]->getPosition());
									types.push_back(tanks[i]->type);
								}
							}
						}
					}
					tanks[j]->attacking = false;
					for (int i = 0; i < targets.size(); i++) {
						if (types[i] != 13) {
							if (types[i] != 5 && tanks[j]->type != 6) {
								tanks[j]->calculateTurretTrajectory(targets[i], types[i]);
								tanks[j]->attacking = true;
							}
							else if (tanks[j]->type == 2 || tanks[j]->type == 5) {
								tanks[j]->calculateTurretTrajectory(targets[i], types[i]);
								tanks[j]->attacking = true;
							}
							else if (tanks[j]->type == 6 && (types[i] >= 9)) {
								tanks[j]->calculateTurretTrajectory(targets[i], types[i]);
								tanks[j]->attacking = true;
							}
						}
					}
					if (tanks[j]->isReloaded && tanks[j]->attacking) {
						tanks[j]->shot = true;
						if (tanks[j]->turret_type == 2) {
							if (tanks[j]->target_type == 7) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 35, 15, true, false));
							}
							else if (tanks[j]->target_type == 5) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 35, 6, true, false));
							}
							else {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 35, 3, true, false));
							}
						}
						if (tanks[j]->turret_type == 3) {
							projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 50, 12, true, true));
						}
						else if (tanks[j]->turret_type == 4) {
							for (int g = 0; g < 22; g++) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory + sin(g) * 15, 20, 1, true, false));
							}
						}
						else if (tanks[j]->turret_type == 5) {
							projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 20, 8, true, false));
						}
						else if (tanks[j]->turret_type == 6) {
							if (tanks[j]->target_type >= 9)
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 100, 80, true, true));
						}
						else if (tanks[j]->turret_type == 7) {
							projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 35, 20, true, true));
						}
						tanks[j]->isReloaded = false;
						tanks[j]->reload_time = 0;
					}
				}
				else {
					vector<ofVec2f> targets;
					vector<int> types;
					for (int i = 0; i < tanks.size(); i++) {
						if (i != j) {
							if (tanks[i]->friendly) {
								if ((tanks[j]->getPosition().distance(tanks[i]->getPosition())) <= tanks[j]->range && tanks[i]->isDead() == false) {
									targets.push_back(tanks[i]->getPosition());
									types.push_back(tanks[i]->type);
								}
							}
						}
					}
					tanks[j]->attacking = false;
					for (int i = 0; i < targets.size(); i++) {
						if (types[i] != 1 && types[i] != 5 && tanks[j]->type != 12) {
							tanks[j]->calculateTurretTrajectory(targets[i], types[i]);
							tanks[j]->attacking = true;
						}
						else if (tanks[j]->type == 12 && (types[i] == 1 || types[i] == 5)) {
							tanks[j]->calculateTurretTrajectory(targets[i], types[i]);
							tanks[j]->attacking = true;
						}
						else if (tanks[j]->type == 2 || tanks[j]->type == 5) {
							tanks[j]->calculateTurretTrajectory(targets[i], types[i]);
							tanks[j]->attacking = true;
						}
					}
					if (tanks[j]->isReloaded && tanks[j]->attacking) {
						tanks[j]->shot = true;
						if (tanks[j]->turret_type == 2) {
							if (tanks[j]->target_type == 7) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 35, 15, false, false));
							}
							else if (tanks[j]->target_type == 5) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 35, 6, false, false));
							}
							else {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 35, 3, false, false));
							}
						}
						else if (tanks[j]->turret_type == 3) {
							projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 50, 12, false, true));
						}
						else if (tanks[j]->turret_type == 4) {
							for (int g = 0; g < 22; g++) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory + sin(g) * 15, 20, 1, false, false));
							}
						}
						else if (tanks[j]->turret_type == 5) {
							if (tanks[j]->target_type >= 9) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 20, 1, false, false));
							}
							else {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 20, 8, false, false));
							}
						}
						else if (tanks[j]->turret_type == 6) {
							projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 100, 20, false, true));
						}
						else if (tanks[j]->turret_type == 8) {
							projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory, 60, 40, false, true));
						}
						else if (tanks[j]->turret_type == 9) {
							for (int g = 0; g < 15; g++) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory + sin(g) * 15, 35, 2, false, false));
							}
						}
						else if (tanks[j]->turret_type == 10) {
							for (int g = 0; g < 5; g++) {
								projectiles.push_back(new Projectile(tanks[j]->getPosition(), tanks[j]->turret_trajectory + sin(g) * 20, 60, 2, false, false));
							}
						}
						tanks[j]->isReloaded = false;
						tanks[j]->reload_time = 0;
					}
				}
			}
			else {
				tanks[j]->attacking = false;
			}
			if ((Iterator % 40) == 0) {
				if (tanks[j]->reclaim) {
					int amount = 0;
					int distance;
					if (tanks[j]->friendly) {
						distance = 800;
					}
					else {
						distance = 3000;
					}
					tanks[j]->target = tanks[j]->standard_target;
					tanks[j]->compareTargets(tanks[j]->standard_target);
					for (int i = 0; i < rects.size(); i++) {
						if (rects[i]->getType() != 98 && (tanks[j]->type == 7 || tanks[j]->type == 8 || tanks[j]->type == 5)) {
							if ((rects[i]->friendly && tanks[j]->friendly == false) || (rects[i]->friendly == false && tanks[j]->friendly)) {
								if (tanks[j]->getPosition().distance(rects[i]->getPosition()) < distance) {
									tanks[j]->compareTargets(rects[i]->getPosition());
									amount++;
								}
							}
						}
					}
					if (amount <= 0) {
						tanks[j]->reclaim = false;
						tanks[j]->search_and_destroy = true;
					}
				}
				else if (tanks[j]->search_and_destroy && tanks[j]->type < 10 && tanks[j]->type != 1) {
					int amount = 0;
					int throttle_amount = 0;
					int distance;
					if (tanks[j]->friendly) {
						distance = 1000;
						tanks[j]->target = tanks[j]->standard_target;
						tanks[j]->compareTargets(tanks[j]->standard_target);
					}
					else {
						distance = 20000;
						ofVec2f tar = { 32000,0 };
						tanks[j]->target = tar;
						tanks[j]->compareTargets(tar);
					}
					for (int i = 0; i < tanks.size(); i++) {
						if (i != j) {
							if (tanks[i]->isDead() == false && tanks[j]->getPosition().distance(tanks[i]->getPosition()) < distance && tanks[i]->type != 13) {
								if ((tanks[j]->friendly && tanks[i]->friendly == false) || (tanks[j]->friendly == false && tanks[i]->friendly)) {
									if (tanks[j]->type == 5 || tanks[j]->type == 2) {
										if (tanks[j]->type == 5 && tanks[j]->friendly == false && tanks[i]->fly) {
											amount++;
											tanks[j]->compareTargets(tanks[i]->getPosition());
										}
										else if((tanks[j]->type == 2 || tanks[j]->type == 5) && tanks[j]->friendly){
											amount++;
											tanks[j]->compareTargets(tanks[i]->getPosition());
										}
										else if (tanks[j]->type == 2 && tanks[j]->friendly == false){
											amount++;
											tanks[j]->compareTargets(tanks[i]->getPosition());
										}
									}
									else if (tanks[i]->type != 5) {
										amount++;
										tanks[j]->compareTargets(tanks[i]->getPosition());
									}
									if (tanks[j]->isTurret && tanks[j]->getPosition().distance(tanks[i]->getPosition()) < (tanks[j]->range - 10)) {
										throttle_amount++;
									}
								}
							}
						}
					}
					if (amount <= 1) {
						tanks[j]->trajectory = { 0,0 };
						tanks[j]->target_distance = 0;
						tanks[j]->throttle = false;
					}
					if (throttle_amount == 0) {
						tanks[j]->throttle = false;
					}
					else {
						if (tanks[j]->type == 6) {
							tanks[j]->trajectory = { 0,0 };
						}
						else {
							tanks[j]->throttle = true;
						}
					}
				}
				else if (tanks[j]->type == 1 || (tanks[j]->type == 5 && tanks[j]->friendly == false)) {
					if (tanks[j]->type == 1) {
						ofVec2f tar = { 32000,0 };
						tanks[j]->target = tar;
						tanks[j]->compareTargets(tar);
						for (int i = 0; i < tanks.size(); i++) {
							if (tanks[i]->type == 9 && tanks[i]->friendly) {
								tanks[j]->compareTargets(tanks[i]->getPosition());
							}
						}
					}
					else if(tanks[j]->type == 5 && tanks[j]->friendly == false){
						ofVec2f tar = { 32000,0 };
						tanks[j]->target = tar;
						tanks[j]->compareTargets(tar);
						for (int i = 0; i < tanks.size(); i++) {
							if (tanks[i]->type == 1 || (tanks[i]->type == 9 && tanks[i]->friendly)) {
								tanks[j]->compareTargets(tanks[i]->getPosition());
							}
						}
					}
				}
				else {
					tanks[j]->throttle = false;
				}
			}
		}
	}
}

void ofApp::calculateMouse() {
	float mouseX = float(ofGetMouseX());
	float mouseY = float(ofGetMouseY() * -1 + ofGetHeight());
	mouse.x = ofMap(mouseX, 0, ofGetWidth(), xOffset + xOff, ofGetWidth() - xOffset + xOff);
	mouse.y = ofMap(mouseY, 0, ofGetHeight(), yOffset + yOff, ofGetHeight() - yOffset + yOff);
	centre_vector = { ofGetWidth() / 2 + xOff,ofGetHeight() / 2 + yOff };
	centre_distance = 1000 - xOffset;
}

void ofApp::tank_selection() {
	ofSetRectMode(OF_RECTMODE_CORNER);
	if (ofGetMousePressed()) {
		if ((Mouse_iterator % 2) == 0) {
			if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
				Mouse_origin.x = mouse.x;
				Mouse_origin.y = mouse.y;
			}
			Mouse_iterator++;
		}
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0 && spawnmode == false && buildmode == false) {
			Selection_X = mouse.x - Mouse_origin.x;
			Selection_Y = mouse.y - Mouse_origin.y;
			ofNoFill();
			ofDrawRectangle(Mouse_origin, Selection_X, Selection_Y);
			for (int i = 0; i < tanks.size(); i++) {
				if (tanks[i]->selected) {
					tanks[i]->selected = false;
				}
			}
		}
		ofVec2f destination(0.0, 0.0);
		ofVec2f origin(0, 0);
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			origin = { float(mouse.x), float(mouse.y) };
		}
		if ((GetKeyState(VK_RBUTTON) & 0x80) != 0) {
			destination = { float(mouse.x), float(mouse.y) };
		}
		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i]->friendly) {
				if (tanks[i]->getPosition().x >= Mouse_origin.x - 10 && tanks[i]->getPosition().x <= Mouse_origin.x + Selection_X + 10) {
					if (tanks[i]->getPosition().y <= Mouse_origin.y - 10 && tanks[i]->getPosition().y >= Mouse_origin.y + Selection_Y + 10) {
						tanks[i]->selected = true;
						if ((tanks[i]->type == 9 && lock) || tanks[i]->type == 1) {
							tanks[i]->selected = false;
						}
					}
				}
				if (tanks[i]->selected && destination.x != 0) {
					tanks[i]->search_and_destroy = false;
					tanks[i]->reclaim = false;
					tanks[i]->calculateTrajectory(destination);
					if (order.isPlaying() == false) {
						order.play();
					}
				}
			}
		}
		destination.x = 0;
	}
}