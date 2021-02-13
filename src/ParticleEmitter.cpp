#include "ofApp.h"
#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
	sys = new ParticleSystem();
	createdSys = true;
	init();
}

ParticleEmitter::ParticleEmitter(ParticleSystem *s) {
	if (s == NULL)
	{
		cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
		ofExit();
	}
	sys = s;
	createdSys = false;
	init();
}

ParticleEmitter::~ParticleEmitter() {
	if (createdSys) delete sys;
}

void ParticleEmitter::init() {
	rate = 1;
	velocity = ofVec3f(0, 20, 0);
	lifespan = 3;
	started = false;
	oneShot = false;
	fired = false;
	lastSpawned = 0;
	radius = 1;
	particleRadius = .1;
	visible = false;
	type = DirectionalEmitter;
	groupSize = 1;
}



void ParticleEmitter::draw() {
	if (visible) {
		switch (type) {
		case DirectionalEmitter:
			ofDrawSphere(trans, radius/10); 
			break;
		case SphereEmitter:
		case RadialEmitter:
			ofDrawSphere(trans, radius/10);  
			break;
		default:
			break;
		}
	}
	sys->draw();  
}
void ParticleEmitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void ParticleEmitter::stop() {
	started = false;
	fired = false;
}
void ParticleEmitter::update() {

	float time = ofGetElapsedTimeMillis();

	if (oneShot && started) {
		if (!fired) {
			for (int i = 0; i < groupSize; i++)
				spawn(time);

			lastSpawned = time;
		}
		fired = true;
		stop();
	}
	else if (((time - lastSpawned) > (1000.0 / rate)) && started) {
		for (int i= 0; i < groupSize; i++)
			spawn(time);
	
		lastSpawned = time;
	}

	sys->update();
}

void ParticleEmitter::spawn(float time) {

	Particle particle;
	particle.color = color;


	switch (type) {
	case RadialEmitter:
	{
		ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		float speed = velocity.length();
		particle.velocity = dir.getNormalized() * speed;
		particle.position = glm::vec3(trans);
	}
	break;
	case SphereEmitter:
		break;
	case DirectionalEmitter:
		particle.velocity = velocity;
		particle.position = glm::vec3(trans);
		break;
	}

	particle.lifespan = lifespan;
	particle.birthtime = time;
	particle.radius = particleRadius;

	sys->add(particle);
}
