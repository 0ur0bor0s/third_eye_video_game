// Author: Ben Foley
// Initial classes provided by Kevin Smith

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BaseObject.h"
#include "Sprite.h"
#include "SpriteSystem.h"
#include "Emitter.h"
#include "EmitterSystem.h"
#include "ParticleEmitter.h"

typedef enum {
	Title,
	StartPressed,
	Intro,
	Running,
	Endgame,
} GameState;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void checkCollisions();

		bool hideGui = true;

		Emitter *bossSpawnEmitter = NULL;
		Emitter *bossEmitter = NULL;
		Emitter *turret = NULL;
		Emitter *enemyEmitter = NULL;
		Emitter *backgroundEmitter = NULL;
		Emitter *backgroundLayerEmitter = NULL;
		Emitter *enemyEmitterRight = NULL;
		bool bDrag = false;
		glm::vec3 lastMouse;

		ofImage backgroundImage;

		ofImage titleImage;
		unsigned int titleX, titleY, imageCount, imgX, imgY;
		float time;

		// Gui
		ofxPanel gui;
		ofxFloatSlider rateoffireSlider;
		ofxVec3Slider velocitySlider;
		ofxToggle sqrttwoToggle;
		ofxToggle sqrtfourToggle;
		ofxToggle goldenratioToggle;
		ofxVec3Slider explosionPosition;

		// Sound
		ofSoundPlayer bulletSound;
		ofSoundPlayer startingTheme;
		ofSoundPlayer hitSound;
		ofSoundPlayer missleFire;
		ofSoundPlayer damageSound;
		ofSoundPlayer bossThemeSound;
		ofSoundPlayer introSound;
		ofSoundPlayer flowerSound;
		ofSoundPlayer handSound;
		ofSoundPlayer bossLaugh;
		ofSoundPlayer finalSound;
		ofSoundPlayer finalTheme;

		// Time data
		float lastFade;
		float fadeNumber = 255.0;
		float lastPatternChange;
		float lastNormalShotSwap;
		float endGameTimer = 0;

		// detect boss hit
		bool wasBossHit;

		// score tracker
		int score;
		int bosshealth = 400;
		ofRectangle healthbar;

		// Game state enum
		GameState gamestate;

		// Intro fade
		bool isFadedIn = false;
		uint32_t introCounter = 0;

		uint8_t emitCount = 0;
		uint32_t patternCount = 0;

		bool animateFlag = true;

		ParticleEmitter explosion;
		ParticleEmitter handExplosion;
		ParticleEmitter bulletExplosion;
		ParticleEmitter finalExplosion;
		ParticleEmitter childExplosion;

		bool handsWait = true;
		float handsWaitTime;

		// Typefont
		ofTrueTypeFont paladin;
};
