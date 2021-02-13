// Author: Ben Foley


#pragma once

#include "ofApp.h"
#include "BaseObject.h"

//== Sprite ==================================
class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();

	glm::mat4 getMatrix() {
		glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(trans));
		glm::mat4 M = glm::rotate(m, glm::radians(rot), glm::vec3(0, 0, 1));
		return M;
	}

	void setImage(ofImage);
	void setVelocity(glm::vec3 velocity);
	void setLifespan(float lifespan);
	void setBirthtime(float birthtime);
	void setHeadingVec(glm::vec3 headingVec);
	glm::vec3 getVelocity();
	float getLifespan();
	float getHeight();
	float getWidth();
	glm::vec3 getHeadingVec();
	int detectHit(glm::vec3, float, float);

	int lifePoints;

protected:
	float speed;    //   in pixels/sec
	glm::vec3 velocity; // in pixels/sec
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	string name;
	bool haveImage;
	float width, height;
	glm::vec3 headingVec;
};