// Author: Ben Foley

#include "ofMain.h"
#include "ofApp.h"
#include "Sprite.h"

/*
	== Sprite ========================================================
*/
// Constructor
Sprite::Sprite() {
	speed = 300;
	velocity = glm::vec3(0, 0, 0);
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	name = "UnamedSprite";
	width = 60;
	height = 80;
	headingVec = glm::vec3(1.0f, 1.0f, 0.0f);
	lifePoints = -1;
}

// Return a sprite's age in milliseconds
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

// Render the sprite
void Sprite::draw() {

	ofSetColor(255, 255, 255, 255);

	// draw image centered and add in translation amount
	//
	if (haveImage) {
		ofPushMatrix();
		ofMultMatrix(getMatrix());
		ofRotateRad(rot);
		image.draw(-width / 2.0, -height / 2.0, width, height);
		ofPopMatrix();
	}
	else {
		// in case no image is supplied, draw something.
		// 
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
	}
}


// Set an image for the sprite. If you don't set one, a rectangle
// gets drawn.
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}

void Sprite::setVelocity(glm::vec3 velocity) {
	this->velocity = velocity;
}

void Sprite::setLifespan(float lifespan) {
	this->lifespan = lifespan;
}

void Sprite::setBirthtime(float birthtime) {
	this->birthtime = birthtime;
}

void Sprite::setHeadingVec(glm::vec3 headingVec) {
	this->headingVec = headingVec;
}

float Sprite::getLifespan() {
	return lifespan;
}

glm::vec3 Sprite::getVelocity() {
	return velocity;
}

float Sprite::getHeight() {
	return height;
}

float Sprite::getWidth() {
	return width;
}

glm::vec3 Sprite::getHeadingVec() {
	return headingVec;
}

// Detect if emitter has been hit be bullet and if so deduct life points accordingly
int Sprite::detectHit(glm::vec3 colliderPos, float colliderWidth, float colliderHeight) {
	bool hit = false;
	// Check for overlap between the sprite and the emitter

	// Rectangular hitboxes
	if (colliderPos.x - (colliderWidth / 2) < trans.x + (width / 2) &&
		colliderPos.x + (colliderWidth / 2) > trans.x - (width / 2) &&
		colliderPos.y - (colliderHeight / 2) < trans.y + (height / 2) &&
		colliderPos.y + (colliderHeight / 2) > trans.y - (height / 2)) {
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
			// return 1 for hit
			return 1;
		}
	}

	// return 2 for not hit
	return 2;
}