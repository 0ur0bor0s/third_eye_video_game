// Author: Ben Foley

#pragma once

#include "ofApp.h"
#include "BaseObject.h"
#include "Sprite.h"
#include "SpriteSystem.h"
#include "Emitter.h"

// To solve error with cyclic reference between emitter and emitter system
class Emitter;

// Emitter System to allow for emitters to easily hold other emitters
class EmitterSystem {
public:
	void add(Emitter);
	void remove(int);
	void update();
	void draw();
	void start();
	void stop();
	int hitDetect(glm::vec3, float, float);
	vector<Emitter> emitters;
};