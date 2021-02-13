// Author: Ben Foley


#include "ofApp.h"
#include "Emitter.h"


/*
	== Emitter =======================================================
*/
//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 10000;    // milliseconds
	started = false;
	emitterStarted = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = glm::vec3(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;

	useGoldenRatio = false;
	grVelocity = glm::vec3(100, 0, 0);
	shootPattern = Normal;
	soundCounter = 0;
	motionPath = None;
	lifePoints = -1;
	isCircle = false;
	emitterLifetime = -1;
	isAnimate = false;

	usePhysics = false;
	moveSpeed = 100.0;
	moveVelocity = glm::vec3(0, 0, 0);
	moveAcceleration = glm::vec3(0, 0, 0);

	emitSys = new EmitterSystem();
}

Emitter::Emitter(Emitter *emitter) {
	emit = emitter;
	lifespan = 5000;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = glm::vec3(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
	useGoldenRatio = false;
	grVelocity = glm::vec3(100, 00, 0);
	shootPattern = Normal;
	soundCounter = 0;
	motionPath = None;
	lifePoints = -1;
	isCircle = false;
	emitterLifetime = -1;

	usePhysics = false;
	moveSpeed = 100.0;
	moveVelocity = glm::vec3(0, 0, 0);
	moveAcceleration = glm::vec3(0, 0, 0);

	emitSys = new EmitterSystem();

}

Emitter::Emitter(SpriteSystem *spriteSys, Emitter *emitter) {
	sys = spriteSys;
	emit = emitter;
	lifespan = 5000;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = glm::vec3(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
	useGoldenRatio = false;
	grVelocity = glm::vec3(100, 00, 0);
	shootPattern = Normal;
	soundCounter = 0;
	motionPath = None;
	lifePoints = -1;
	isCircle = false;
	emitterLifetime = -1;

	usePhysics = false;
	moveSpeed = 100.0;
	moveVelocity = glm::vec3(0, 0, 0);
	moveAcceleration = glm::vec3(0, 0, 0);

	emitSys = new EmitterSystem();
}



// Curve evaluation
glm::vec3 Emitter::curveEval(float y, float scale, float cycles) {
	float u = (cycles * y * PI) / ofGetHeight();
	return (glm::vec3(-scale * sin(u) + (ofGetHeight() / 2),  y, 0));
}




//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			if (damaged) {
				ofSetColor(255, 0, 0, alpha);
				damaged = false;
			}
			else {
				ofSetColor(255, 255, 255, alpha);
			}

			if (isAnimate) { // Animate image of emitter
				image.drawSubsection(-width / 2.0 + trans.x + emitterOffset.x, -height / 2.0 + trans.y + emitterOffset.y, 
					width, height,
					imageX, imageY);
			}
			else { // Draw single sprite
				image.draw(-width / 2.0 + trans.x + emitterOffset.x, -height / 2.0 + trans.y + emitterOffset.y);
			}

		}
		else {
			ofSetColor(0, 0, 200);
			ofNoFill();
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
		}
	}

	// draw sprite system
	//
	if (sys != NULL)
		sys->draw();
	if (emit != NULL)
		emitSys->draw();
}


// Physics integrator for update()
void Emitter::integrate() {
	// Translation integration
	moveVelocity += moveAcceleration / ofGetFrameRate();
	moveVelocity *= moveDamping;
	trans += moveVelocity / ofGetFrameRate();
}


// Move emitter
// Helper function for update()
void Emitter::move() {

	// Move emitter
	if (usePhysics) {
		moveVelocity += moveAcceleration / ofGetFrameRate();
		moveVelocity *= 0.97;
	}

	glm::vec3 newTrans = trans + moveVelocity / ofGetFrameRate();

	switch (motionPath) {
	// Do not move unless being controlled
	case None:
		// Bounds checking
		if (width / 2.0 + newTrans.x - emitterOffset.x > ofGetWindowWidth()) { // right bound
			newTrans.x = ofGetWindowWidth() - width / 2.0;
			moveAcceleration.x = 0;
			moveVelocity.x = 0;
		}
		else if (newTrans.x - width / 2 + emitterOffset.x < 0) { // left bound
			newTrans.x = width;
			moveAcceleration.x = 0;
			moveVelocity.x = 0;
		}
		else if (newTrans.y - image.getHeight() / 2 - emitterOffset.y < 0) { // upper bound
			newTrans.y = image.getHeight() / 2.0;
			moveAcceleration.y = 0;
			moveVelocity.y = 0;
		}
		else if (image.getHeight() / 2.0 + newTrans.y - emitterOffset.y > ofGetWindowHeight()) { // lower bound
			newTrans.y = ofGetWindowHeight() - image.getHeight() / 2.0;
			moveAcceleration.y = 0;
			moveVelocity.y = 0;
		}
		else { // else move as normal
			trans = newTrans;
		}
		break;
	// Move in a linear Line
	case Line:
		
		trans = newTrans;

		break;
	case Rise: {
		if (usePhysics) {
			if (newTrans.y < 300) {
				moveAcceleration.y = 0;
			}
		}
		trans = newTrans;
		break;
	}
	case BackAndForth:
		if (usePhysics) {
			if (newTrans.x < 200) {
				moveAcceleration.x = fmax(-moveAcceleration.x, moveAcceleration.x);
			}
			else if (newTrans.x > ofGetWindowWidth() - 200) {
				moveAcceleration.x = fmin(-moveAcceleration.x, moveAcceleration.x);
			}
			trans = newTrans;
		}
		else {
			if (newTrans.x < 100 || newTrans.x > ofGetWindowWidth() - 100) {
				moveVelocity.x = -moveVelocity.x;
			}
			trans = newTrans;
		}

		break;
	case Center:
		if (usePhysics) {
			moveVelocity *= 0;
			moveAcceleration *= 0;
		}
		if (trans.x < ofGetWindowWidth() / 2) {
			if (ofGetWindowWidth() / 2 - trans.x < 1) {
				trans.x = ofGetWindowWidth() / 2;
			}
			else {
				trans.x += 1;
			}
		}
		else if (trans.x > ofGetWindowWidth() / 2) {
			if (ofGetWindowWidth() / 2 - trans.x > -1) {
				trans.x = ofGetWindowWidth() / 2;
			}
			else {
				trans.x -= 1;
			}
		}
	}
}


//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (started) {

		float time = ofGetElapsedTimeMillis();
		if (sys != NULL) { // sprite system	
			if ((time - lastSpawned) > (1000.0 / rate)) {
				// spawn a new sprite
				Sprite sprite;
				if (haveChildImage) sprite.setImage(childImage);
				sprite.setLifespan(lifespan);
				sprite.setPosition(trans);
				sprite.setBirthtime(time);
				if (spriteLifePoints != -1) {
					sprite.lifePoints = spriteLifePoints;
				}

				// Set shooting pattern
				glm::vec3 newVelocity;
				switch (shootPattern) {
				case GoldenRatio:
					newVelocity = glm::rotate(grVelocity, float(1 / PI), glm::vec3(0, 0, 1));
					sprite.setVelocity(newVelocity);
					grVelocity = newVelocity;
					break;
				case Sqrt2:
					newVelocity = glm::rotate(grVelocity, float(std::sqrt(2)), glm::vec3(0, 0, 1));
					sprite.setVelocity(newVelocity);
					grVelocity = newVelocity;
					break;
				case Sqrt4:
					newVelocity = glm::rotate(grVelocity, float(std::sqrt(4)), glm::vec3(0, 0, 1));
					sprite.setVelocity(newVelocity);
					grVelocity = newVelocity;
					break;
				case RisingSun:
					break;
				case Sinewave:
					sprite.setVelocity(newVelocity);
					break;
				case Normal:
					if (targetEmitter != NULL) { // shoot at target
						glm::vec3 headingVec = glm::normalize(targetEmitter->getPosition() - trans);
						sprite.setHeadingVec(headingVec);
						newVelocity = headingVec * velocity.y;
						sprite.setVelocity(newVelocity);
					}
					else {
						sprite.setVelocity(velocity);
					}
					break;
				}

				sys->add(sprite);
				lastSpawned = time;

				// Play sound effect
				if (soundRef != NULL) {
					if (rate < 10) {
						soundRef->play();
					}
					else {
						// Limit number of times sound can be played to save ears
						if (soundCounter % 3 == 0) {
							soundRef->play();
							soundCounter++;
						}
						soundCounter++;

					}
				}
			}
		}
	}
	if (emitterStarted) {
		float time = ofGetElapsedTimeMillis();
		if (emit != NULL) { // Emitter emitting emitters
			if ((time - lastEmitterSpawned) > (1000.0 / emitterSpawnRate)) {
				// spawn emitter
				Emitter emitter(new SpriteSystem());
				emitter.setImage(emit->getImage());
				emitter.setChildImage(emit->getChildImage());
				emitter.setPosition(trans);
				emitter.setShootPattern(emit->getShootPattern());
				//emitter.setSoundRef(emit->getSoundRef());
				emitter.emitterOffset = emit->emitterOffset;
				emitter.setMotionPath(emit->getMotionPath());
				emitter.setMoveVelocity(emit->getMoveVelocity());
				emitter.setRate(emit->getRate());
				emitter.setBirthtime(time);
				emitter.setEmitterLifetime(10000);

				// add new emitter to emitter vector
				emitSys->add(emitter);
				lastEmitterSpawned = time;
				emitSys->start();
			}

			// Play sound effect
			if (soundRef != NULL) {
				if (rate < 10) {
					soundRef->play();
				}
				else {
					// Limit number of times sound can be played to save ears
					if (soundCounter % 3 == 0) {
						soundRef->play();
						soundCounter++;
					}
					soundCounter++;

				}
			}
		}
	}
	
	// Update systems
	if (sys != NULL)
		sys->update();
	if (emit != NULL)
		emitSys->update();

	// Update Emitter position
	move();

	// Update Emitter animation
	if (haveImage && isAnimate) {
		if (frameCountDelay > 10 ) {
			if (imageFrameCount == imageCount) { // Reset subsection count
				imageX = width * (startImageCount - 1);
				//imageX = 0;
				//imageY = 0;
				imageFrameCount = startImageCount;
			}
			else {
				imageX += width;
				imageFrameCount++;
			}
			frameCountDelay = 0;
		}
		frameCountDelay += 1 + ofGetFrameRate() / 60;
	}

}


// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
}

void Emitter::stop() {
	started = false;
}

// Start/Stop emitting emitters
void Emitter::startEmitter() {
	emitterStarted = true;
}

void Emitter::stopEmitter() {
	emitterStarted = false;
}

void Emitter::moveDir(MoveDir dir) {
	if (usePhysics) {
		switch (dir) {
		case MoveRight:
			moveAcceleration += glm::vec3(1, 0, 0) * moveSpeed;
			break;
		case MoveLeft:
			moveAcceleration += glm::vec3(-1, 0, 0) * moveSpeed;
			break;
		case MoveUp:
			moveAcceleration += glm::vec3(0, -1, 0) * moveSpeed;
			break;
		case MoveDown:
			moveAcceleration += glm::vec3(0, 1, 0) * moveSpeed;
			break;
		}
	}
	else {
		switch (dir) {
		case MoveRight:
			moveVelocity.x = moveSpeed;
			break;
		case MoveLeft:
			moveVelocity.x = -moveSpeed;
			break;
		case MoveUp:
			moveVelocity.y = -moveSpeed;
			break;
		case MoveDown:
			moveVelocity.y = moveSpeed;
			break;
		}
	}
	
}

void Emitter::stopDir(MoveDir dir) {
	if (usePhysics) {
		switch (dir) {
		case MoveRight:
			moveAcceleration.x = fmin(0.0f, moveAcceleration.x);
			break;
		case MoveLeft:
			moveAcceleration.x = fmax(0.0f, moveAcceleration.x);
			break;
		case MoveUp:
			moveAcceleration.y = fmax(0.0f, moveAcceleration.y);
			break;
		case MoveDown:
			moveAcceleration.y = fmin(0.0f, moveAcceleration.y);
			break;
		}
	}
	else {
		switch (dir) {
		case MoveRight:
			moveVelocity.x -= moveSpeed;
			break;
		case MoveLeft:
			moveVelocity.x += moveSpeed;
			break;
		case MoveUp:
			moveVelocity.y += moveSpeed;
			break;
		case MoveDown:
			moveVelocity.y -= moveSpeed;
			break;
		}
	}
	
}

bool Emitter::inside(glm::vec3 p) {
	if (p.x > trans.x - (image.getWidth() / 2) &&
		p.x < trans.x + (image.getWidth() / 2) &&
		p.y > trans.y - (image.getHeight() / 2) &&
		p.y < trans.y + (image.getHeight() / 2)) return true;
	else return false;
}

void Emitter::moveWithMouse(glm::vec3 mousePoint, glm::vec3 &lastMouse) {

	glm::vec3 newTrans = trans + mousePoint - lastMouse;

	// Bounds Checking
	if (image.getWidth() / 2.0 + newTrans.x - 20 > ofGetWindowWidth()) { // right bound
		newTrans.x = ofGetWindowWidth() - image.getWidth() / 2.0;
		velocity.x = 0;
	}
	else if (newTrans.x - image.getWidth() / 2 + 20 < 0) { // left bound
		newTrans.x = image.getWidth() / 2.0;
		velocity.x = 0;
	}
	else if (newTrans.y - image.getHeight() / 2 - 20 < 0) { // upper bound
		newTrans.y = image.getHeight() / 2.0;
		velocity.y = 0;
	}
	else if (image.getHeight() / 2.0 + newTrans.y - 40 > ofGetWindowHeight()) { // lower bound
		newTrans.y = ofGetWindowHeight() - image.getHeight() / 2.0;
		velocity.y = 0;
	}
	else { // else move as normal
		trans = newTrans;
	}

	lastMouse = mousePoint;
}


// Detect if emitter has been hit be bullet and if so deduct life points accordingly
int Emitter::detectHit(glm::vec3 colliderPos, float colliderWidth, float colliderHeight) {
	bool hit = false;
	// Check for overlap between the sprite and the emitter
	// Circular hitboxes
	if (isCircle) {
		glm::vec3 collisionVec = glm::vec3(colliderPos - (trans + emitterOffset));
		if (glm::length(collisionVec) < width/2) {
			hit = true;
		}
	}
	// Rectangular hitboxes
	else if (colliderPos.x - (colliderWidth / 2) < trans.x + (width / 2) + emitterOffset.x &&
		colliderPos.x + (colliderWidth / 2) > trans.x - (width / 2) + emitterOffset.x &&
		colliderPos.y - (colliderHeight / 2) < trans.y + (height / 2) + emitterOffset.y &&
		colliderPos.y + (colliderHeight / 2) > trans.y - (height / 2) + emitterOffset.y) {
		hit = true;
	}


	if (hit) {
		if (lifePoints == -1) { // immortal emitter
			return 1;
		}

		// Deduct life points
		if (--lifePoints == 0) {
			// if life points are equal to zero send 0 flag and mark emitter for deletion
			return 0;
		}
		else {
			return 1;
		}
	}

	return 2;
}


// Special function to setup animation parameters for image
void Emitter::animateImage(uint32_t startImageCount, uint32_t imageCount, uint32_t width, uint32_t height) {
	isAnimate = true;
	this->width = width;
	this->height = height;
	this->imageCount = imageCount;
	this->startImageCount = startImageCount;
	this->imageX = width * (startImageCount - 1);
	this->imageY = 0;
	imageFrameCount = startImageCount;
}



void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(glm::vec3 v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = img.getWidth();
	height = img.getHeight();
}

void Emitter::setRate(float r) {
	rate = r;
}

void Emitter::setShootPattern(ShootPattern s) {
	shootPattern = s;
}


void Emitter::setSoundRef(ofSoundPlayer &sp) {
	soundRef = &sp;
}

void Emitter::setMotionPath(MotionPath p) {
	motionPath = p;
}

void Emitter::setEmitterSpawnRate(float r) {
	emitterSpawnRate = r;
}

void Emitter::setMoveVelocity(glm::vec3 m) {
	moveVelocity = m;
}

void Emitter::setGrVelocity(glm::vec3 g) {
	grVelocity = g;
}

void Emitter::setTargetEmitter(Emitter *e) {
	targetEmitter = e;
}

void Emitter::setPhysics(bool p) {
	usePhysics = p;
}

void Emitter::setMoveSpeed(float ms) {
	moveSpeed = ms;
}

void Emitter::setMoveAcceleration(glm::vec3 a) {
	moveAcceleration = a;
}

void Emitter::setAlpha(float a) {
	alpha = a;
}

void Emitter::setEmitterLifetime(float l) {
	emitterLifetime = l;
}

void Emitter::setBirthtime(float t) {
	birthTime = t;
}

void Emitter::setMoveDamping(float d) {
	moveDamping = d;
}


void Emitter::wasDamaged() {
	damaged = true;
}

float Emitter::getDistPerFrame() {
	return velocity.length() / ofGetFrameRate();
}

ofImage Emitter::getImage() {
	return image;
}

ofImage Emitter::getChildImage() {
	return childImage;
}

ShootPattern Emitter::getShootPattern() {
	return shootPattern;
}

ofSoundPlayer Emitter::getSoundRef() {
	return *soundRef;
}

MotionPath Emitter::getMotionPath() {
	return motionPath;
}

glm::vec3 Emitter::getMoveVelocity() {
	return moveVelocity;
}

float Emitter::getRate() {
	return rate;
}

float Emitter::getEmitterLifetime() {
	return emitterLifetime;
}

glm::vec3 Emitter::getMoveAcceleration() {
	return moveAcceleration;
}

uint32_t Emitter::getCurrentFrameCount() {
	return imageFrameCount;
}

float Emitter::getBirthtime() {
	return birthTime;
}


void Emitter::startEmitterSystem() {
	if (emit != NULL)
		emitSys->start();
	else
		cerr << "Cannot start. Emitter is NULL" << endl;	
}
void Emitter::stopEmitterSystem() {
	if (emit != NULL)
		emitSys->stop();
	else
		cerr << "Cannot stop. Emitter is NULL" << endl;
}

void Emitter::imageIsCircle() {
	isCircle = true;
}

void Emitter::swap() {
	started = !started;
}

void Emitter::swapEmitter() {
	emitterStarted = !emitterStarted;
}