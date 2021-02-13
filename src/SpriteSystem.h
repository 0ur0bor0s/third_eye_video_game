// Author: Ben Foley

#pragma once

#include "ofApp.h"
#include "BaseObject.h"
#include "Sprite.h"


// Enum for movement
typedef enum {
	MoveRight,
	MoveLeft,
	MoveUp,
	MoveDown,
	StopMove,
} MoveDir;


//== Sprite System ============================
class SpriteSystem {
public:
	void add(Sprite);
	void remove(int);
	void update();
	void draw();
	void removeNear(glm::vec3, float, float);
	vector<Sprite> sprites;
	bool isSinewave = false;
	bool backwards = true;
private:
	glm::vec3 curveEval(float x, float y, float scale, float cycles);
};




