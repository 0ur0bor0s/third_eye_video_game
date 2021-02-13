#pragma once

#include "ofMain.h"
#include "ofApp.h"

class ParticleForceField;

class Particle {
public:
	Particle();

	glm::vec3 position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
	float   birthtime;
	void    integrate();
	void    draw();
	float   age();        // sec
	ofColor color;
};


