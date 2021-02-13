// Author: Ben Foley


#pragma once

#include "ofApp.h"
#include "BaseObject.h"
#include "Sprite.h"
#include "SpriteSystem.h"
#include "EmitterSystem.h"

// Enum for shooting patterns
typedef enum {
	Normal,
	Sqrt2,
	Sqrt4,
	GoldenRatio,
	RisingSun,
	Sinewave,
} ShootPattern;

typedef enum {
	None,
	Line,
	Sine,
	RandomWalk,
	BackAndForth,
	Rise,
	Center,
} MotionPath;

//== Emitter =================================
class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	Emitter(Emitter *);
	Emitter(SpriteSystem *, Emitter *);
	void draw();
	void start();
	void stop();
	void startEmitter();
	void stopEmitter();
	void setLifespan(float);
	void setVelocity(glm::vec3);
	void setChildImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();
	void moveDir(MoveDir dir);
	void stopDir(MoveDir dir);
	void setShootPattern(ShootPattern);
	void setSoundRef(ofSoundPlayer&);
	void setGrVelocity(glm::vec3);
	bool inside(glm::vec3);
	void moveWithMouse(glm::vec3, glm::vec3&);
	void setMotionPath(MotionPath);
	void setEmitterSpawnRate(float);
	void setMoveVelocity(glm::vec3);
	void startEmitterSystem();
	void stopEmitterSystem();
	void setEmitterLifetime(float);
	void imageIsCircle();
	int detectHit(glm::vec3, float, float);
	void setTargetEmitter(Emitter *e);
	void setBirthtime(float);
	void setPhysics(bool);
	void setMoveSpeed(float);
	void setMoveAcceleration(glm::vec3);
	void setAlpha(float);
	void swap();
	void swapEmitter();
	// Visual Feedback for hit
	void wasDamaged();
	void setMoveDamping(float);

	// set animation params for emitter image
	void animateImage(uint32_t startImageCount, uint32_t imageCount, uint32_t width, uint32_t height);

	// Getters
	float getDistPerFrame();
	ofImage getImage();
	ofImage getChildImage();
	ShootPattern getShootPattern();
	ofSoundPlayer getSoundRef();
	MotionPath getMotionPath();
	glm::vec3 getMoveVelocity();
	float getRate();
	float getEmitterLifetime();
	glm::vec3 getMoveAcceleration();
	uint32_t getCurrentFrameCount();
	float getBirthtime();

	SpriteSystem *sys = NULL;
	EmitterSystem *emitSys = NULL;
	Emitter *emit = NULL;
	glm::vec3 emitterOffset;

	int spriteLifePoints = -1;


private:
	// Helper function to change emitter position
	void move();

	// Helper function for curve evaluation
	glm::vec3 curveEval(float, float, float);

	// Helper function to integrate for physics
	void integrate();

	// boolean for circular hitboxes
	bool isCircle;

	// emitter spawn data
	float lastEmitterSpawned;
	float emitterSpawnRate;

	bool useGoldenRatio;
	ShootPattern shootPattern;
	MotionPath motionPath;
	glm::vec3 grVelocity;

	ofSoundPlayer *soundRef = NULL;
	unsigned int soundCounter;
	
	// Emitter movement
	float moveSpeed;
	glm::vec3 moveVelocity;
	glm::vec3 moveAcceleration;
	float moveDamping = 0.99;

	float rate;
	glm::vec3 velocity;
	float lifespan;
	bool started;
	bool emitterStarted;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	float emitterLifetime;
	float birthTime;
	float alpha = 255;

	// Parameters for image animation
	bool isAnimate;
	uint32_t imageX;
	uint32_t imageY;
	uint32_t imageFrameCount;
	uint32_t startImageCount;
	uint32_t imageCount;
	uint32_t frameCountDelay = 11;

	// Points for life of enemy emitters
	// if Set to -1 emitter cannot die
	uint16_t lifePoints;

	// Emitter to shoot at
	Emitter *targetEmitter = NULL;

	// Boolean to turn physics on or off
	bool usePhysics;

	// Boolean to detect if emitter was hit
	bool damaged = false;

};