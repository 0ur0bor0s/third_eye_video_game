#pragma once

#include "ofApp.h"

//== Basic Object =============================
class BaseObject {
public:
	BaseObject();
	void setPosition(glm::vec3 position);
	void setRotation(float rot);
	glm::vec3 getPosition();
	float getRotation();
protected:
	glm::vec3 trans, scale;
	float rot;
	bool bSelected;
};