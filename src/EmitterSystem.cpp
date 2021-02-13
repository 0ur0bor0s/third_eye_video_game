// Author: Ben Foley

#include "ofApp.h"
#include "EmitterSystem.h"


void EmitterSystem::add(Emitter e) {
	emitters.push_back(e);
}

void EmitterSystem::remove(int i) {
	emitters.erase(emitters.begin() + i);
}


void EmitterSystem::update() {
	if (emitters.empty()) return;
	vector<Emitter>::iterator s = emitters.begin();
	vector<Emitter>::iterator tmp;
	while (s != emitters.end()) {
		if (s->getEmitterLifetime() != -1 && ((ofGetElapsedTimeMillis() - s->getBirthtime()) > s->getEmitterLifetime())) {
			tmp = emitters.erase(s);
			s = tmp;
		}
		else s++;
	}


	for (auto &e : emitters) {
		e.update();
	}
}

void EmitterSystem::draw() {
	for (auto &e : emitters) {
		e.draw();
	}
}


void EmitterSystem::start() {
	for (auto &e : emitters) {
		e.start();
	}
}

void EmitterSystem::stop() {
	for (auto &e : emitters) {
		e.stop();
	}
}

int EmitterSystem::hitDetect(glm::vec3 colliderPoint, float colliderWidth, float colliderHeight) {
	vector<Emitter>::iterator e = emitters.begin();
	vector<Emitter>::iterator tmp;

	int hitCounter = 0;

	while (e != emitters.end()) {
		// Check for overlap between entities

		int i = e->detectHit(colliderPoint, colliderWidth, colliderHeight);

		if (i == 1 || i == 0) {
			hitCounter++;
			break;
		}

		e++;
	}
	return hitCounter;
}