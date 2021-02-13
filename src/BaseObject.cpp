// Author: Ben Foley

#include "ofApp.h"
#include "BaseObject.h"

/*
	== BaseObject Functions ===========================================
*/
BaseObject::BaseObject() {
	trans = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rot = 0;
}

void BaseObject::setRotation(float rot) {
	this->rot = rot;
}


void BaseObject::setPosition(glm::vec3 pos) {
	trans = pos;
}

glm::vec3 BaseObject::getPosition() {
	return trans;
}

float BaseObject::getRotation() {
	return rot;
}