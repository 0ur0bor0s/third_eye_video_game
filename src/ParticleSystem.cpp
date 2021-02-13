	
// Kevin M.Smith - CS 134 SJSU

#include "ofApp.h"
#include "ParticleSystem.h"

void ParticleSystem::add(const Particle &p) {
	particles.push_back(p);
}

void ParticleSystem::addForce(ParticleForce *f) {
	forces.push_back(f);
}

void ParticleSystem::remove(int i) {
	particles.erase(particles.begin() + i);
}

void ParticleSystem::setLifespan(float l) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].lifespan = l;
	}
}

void ParticleSystem::reset() {
	for (int i = 0; i < forces.size(); i++) {
		forces[i]->applied = false;
	}
}

void ParticleSystem::update() {
	if (particles.size() == 0) return;

	vector<Particle>::iterator p = particles.begin();
	vector<Particle>::iterator tmp;

	while (p != particles.end()) {
		if (p->lifespan != -1 && p->age() > p->lifespan) {
			tmp = particles.erase(p);
			p = tmp;
		}
		else p++;
	}


	for (int i = 0; i < particles.size(); i++) {
		for (int k = 0; k < forces.size(); k++) {
			if (!forces[k]->applied)
				forces[k]->updateForce( &particles[i] );
		}
	}


	for (int i = 0; i < forces.size(); i++) {
		if (forces[i]->applyOnce)
			forces[i]->applied = true;
	}


	for (int i = 0; i < particles.size(); i++)
		particles[i].integrate();

}


int ParticleSystem::removeNear(const ofVec3f & point, float dist) { return 0; }


void ParticleSystem::draw() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
}


GravityForce::GravityForce(const ofVec3f &g) {
	gravity = g;
}

void GravityForce::updateForce(Particle * particle) {
	particle->forces += gravity * particle->mass;
}

void GravityForce::setGravity(const ofVec3f &g) {
	gravity = g;
}


TurbulenceForce::TurbulenceForce(const ofVec3f &min, const ofVec3f &max) {
	tmin = min;
	tmax = max;
}

void TurbulenceForce::updateForce(Particle * particle) {
	particle->forces.x += ofRandom(tmin.x, tmax.x);
	particle->forces.y += ofRandom(tmin.y, tmax.y);
	particle->forces.z += ofRandom(tmin.z, tmax.z);
}

void TurbulenceForce::setTurbulence(const ofVec3f &min, const ofVec3f &max) {
	tmin = min;
	tmax = max;
}

ImpulseRadialForce::ImpulseRadialForce(float magnitude, float height) {
	this->magnitude = magnitude;
	this->height = height;
	applyOnce = true;
}

void ImpulseRadialForce::updateForce(Particle * particle) {

	// Clamp direction vector
	ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-height, height), ofRandom(-1, 1));

	// Then clamp magnitude
	glm::vec3 dirMag = dir.getNormalized() * magnitude;
	float clampY = glm::min(glm::max(-dirMag.y * height, dirMag.y), dirMag.y * height);

	// Add forces
	particle->forces += glm::vec3(dirMag.x, clampY, dirMag.z);
}

void ImpulseRadialForce::setRadialForce(float magnitude, float height) {
	this->magnitude = magnitude;
	this->height = height;
}