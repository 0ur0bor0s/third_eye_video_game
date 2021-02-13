// Author: Ben Foley

#include "ofApp.h"
#include "SpriteSystem.h"

/*
	== Sprite System =================================================
*/
//  Add a Sprite to the Sprite System
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}

// remove all sprites within a given dist of point
//
void SpriteSystem::removeNear(glm::vec3 colliderPoint, float colliderHeight, float colliderWidth) {
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	while (s != sprites.end()) {
		// Check for overlap between the two sprites
		glm::vec3 spritePoint = s->getPosition();
		float spriteWidth = s->getWidth();
		float spriteHeight = s->getHeight();
		if (colliderPoint.x - (colliderWidth / 2)  < spritePoint.x + (spriteWidth / 2) &&
			colliderPoint.x + (colliderWidth / 2)  > spritePoint.x - (spriteWidth / 2) &&
			colliderPoint.y - (colliderHeight / 2) < spritePoint.y + (spriteHeight / 2) &&
			colliderPoint.y + (colliderHeight / 2) > spritePoint.y - (spriteHeight / 2)) {

			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}
}

glm::vec3 SpriteSystem::curveEval(float x, float y, float scale, float cycles) {
	float u = (cycles * y * PI) / ofGetHeight();
	return (glm::vec3(scale * sin(u) + x, y, 0));
}


void SpriteSystem::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	while (s != sprites.end()) {
		if (s->getLifespan() != -1 && s->age() > s->getLifespan()) {
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	if (isSinewave) {
		for (int i = 0; i < sprites.size(); i++) {
			glm::vec3 lastPos = sprites[i].getPosition();
			
			// Set position on curve
			//glm::vec3 curvePos = curveEval(lastPos.x, lastPos.y + 2, 4.0, 2.0);

			sprites[i].setPosition(curveEval(lastPos.x, lastPos.y + 8, 4.0, 2.0));
			

			
			glm::vec3 directionVec = glm::normalize(sprites[i].getPosition() - lastPos);
			float angle = glm::orientedAngle(directionVec, sprites[i].getHeadingVec(), lastPos);

			// Determine direction
			if (glm::cross(sprites[i].getHeadingVec(), directionVec).z < 0)
				angle = -angle;

			// Set rotation
			sprites[i].setRotation(sprites[i].getRotation() + angle);

			// Set new headingvec
			sprites[i].setHeadingVec(directionVec);
		}
	}
	else {
		for (int i = 0; i < sprites.size(); i++) {
			sprites[i].setPosition(sprites[i].getPosition() + (sprites[i].getVelocity() / ofGetFrameRate()));
		}
	}

}

//  Render all the sprites
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}