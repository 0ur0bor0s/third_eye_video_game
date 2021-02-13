#pragma once

#include "ofMain.h"
#include "BaseObject.h"
#include "Particle.h"


class ParticleForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
	void add(const Particle &);
	void addForce(ParticleForce *);
	void remove(int);
	void update();
	void setLifespan(float);
	void reset();
	int removeNear(const ofVec3f & point, float dist);
	void draw();
	vector<Particle> particles;
	vector<ParticleForce *> forces;
};



class GravityForce: public ParticleForce {
	ofVec3f gravity;
public:
	GravityForce(const ofVec3f & gravity);
	void updateForce(Particle *);
	void setGravity(const ofVec3f &);
};

class TurbulenceForce : public ParticleForce {
	ofVec3f tmin, tmax;
public:
	TurbulenceForce(const ofVec3f &min, const ofVec3f &max);
	void updateForce(Particle *);
	void setTurbulence(const ofVec3f &min, const ofVec3f &max);
};

class ImpulseRadialForce : public ParticleForce {
	float magnitude;
	float height;
public:
	ImpulseRadialForce(float magnitude, float height); 
	void updateForce(Particle *);
	void setRadialForce(float magnitude, float height);
};
