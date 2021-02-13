// Author: Ben Foley

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetBackgroundColor(ofColor::black);
	
	// Load Font
	paladin.load("paladins.ttf", 19);

	// Set game state
	gamestate = Title;

	// Set background image
	backgroundImage.load("space.png");
	
	// Set up Sounds
	bulletSound.load("exorcism.wav");
	bulletSound.setVolume(0.2f);
	//bulletSound.load("bullet_sound.wav");
	bulletSound.setMultiPlay(true);

	hitSound.load("hit.wav");
	hitSound.setLoop(false);
	hitSound.setMultiPlay(true);

	startingTheme.load("opening_theme.wav");
	startingTheme.setLoop(true);
	startingTheme.play();

	missleFire.load("lazer.wav");
	missleFire.setLoop(true);
	//missleFire.setMultiPlay(false);
	missleFire.setVolume(0.15f);

	damageSound.load("ship_hit.wav");
	//damageSound.load("chain_frost.wav");
	damageSound.setMultiPlay(true);

	bossThemeSound.load("possession.wav");
	bossThemeSound.setVolume(0.4f);
	bossThemeSound.setLoop(true);

	introSound.load("intro_sound.wav");
	introSound.setVolume(0.7f);

	flowerSound.load("large_exorcism.wav");
	flowerSound.setVolume(0.15f);

	handSound.load("hand_explode.wav");
	handSound.setVolume(0.2f);
	handSound.setMultiPlay(true);
	
	bossLaugh.load("laugh.wav");
	bossLaugh.setVolume(0.2f);

	finalSound.load("final_explosion.wav");
	finalSound.setVolume(0.5f);
	
	finalTheme.load("final_music.wav");
	finalTheme.setVolume(0.0f);

	// Load title image
	titleImage.load("title.png");
	titleX = ofGetWindowWidth() / 2 - 365;
	titleY = 60;
	imageCount = 1;
	imgX = 0;
	imgY = 0;
	time = ofGetElapsedTimeMillis();
	
	// images
	ofImage turret_img, bullet_img, boss_img, boss_spawn_img, turret_bullet_img, 
		    bullet_small_img, back_img, layer_img, hand_img;
	//turret_img.load("gameship.png");
	turret_img.load("gameship_animation.png");
	bullet_img.load("enemy_bullet_small.png");
	boss_img.load("boss_eye_animations.png");
	boss_spawn_img.load("enemy_eye.png");
	turret_bullet_img.load("gameship_bullet.png");
	bullet_small_img.load("enemy_bullet_small_2.png");
	back_img.load("space.png");
	layer_img.load("space-layer2.png");
	hand_img.load("bossHand.png");

	// Set up emitter that Boss will emit occasionally
	bossSpawnEmitter = new Emitter(new SpriteSystem());
	bossSpawnEmitter->setImage(boss_spawn_img);
	bossSpawnEmitter->setChildImage(bullet_small_img);
	bossSpawnEmitter->setVelocity(glm::vec3(0, 200, 0));
	bossSpawnEmitter->setMotionPath(Line);
	bossSpawnEmitter->setShootPattern(GoldenRatio);
	bossSpawnEmitter->setMoveVelocity(glm::vec3(0, 200, 0));
	bossSpawnEmitter->setRate(10);
	bossSpawnEmitter->setEmitterLifetime(100);

	// Set up turret emitter
	turret = new Emitter(new SpriteSystem());
	// Set turret image
	turret->setImage(turret_img);
	turret->setChildImage(turret_bullet_img);
	turret->setPosition(glm::vec3(ofGetWindowWidth() / 2, 800, 0));
	turret->setVelocity(glm::vec3(0, -600, 0));
	turret->emitterOffset = glm::vec3(0, 25, 0);
	turret->setPhysics(true);
	turret->setMoveSpeed(500);
	turret->setMoveDamping(0.7);
	turret->animateImage(1, 6, 84, turret_img.getHeight());
	turret->imageIsCircle();


	// Set up boss emitter
	bossEmitter = new Emitter(new SpriteSystem(), bossSpawnEmitter);
	bossEmitter->setImage(boss_img);
	bossEmitter->animateImage(1, 19, boss_img.getHeight(), boss_img.getHeight());
	bossEmitter->setChildImage(bullet_img);
	bossEmitter->setPosition(glm::vec3(ofGetWindowWidth() / 2, 500, 0));
	bossEmitter->setVelocity(glm::vec3(0, 300, 0));
	bossEmitter->setGrVelocity(glm::vec3(200, 0, 0));
	//bossEmitter->setSoundRef(bulletSound);
	bossEmitter->emitterOffset = glm::vec3(0, -40, 0);
	bossEmitter->setEmitterSpawnRate(0.05);
	bossEmitter->imageIsCircle();
	bossEmitter->setShootPattern(Sqrt4);
	bossEmitter->setRate(12);
	bossEmitter->setMotionPath(Rise);
	bossEmitter->setMoveVelocity(glm::vec3(0, -50, 0));
	bossEmitter->setMoveAcceleration(glm::vec3(0, -50, 0));
	bossEmitter->setTargetEmitter(turret);
	bossEmitter->setPhysics(true);
	bossEmitter->setShootPattern(Normal);
	bossEmitter->stopEmitter();

	// Spawner for enemy hands
	enemyEmitterRight = new Emitter(new SpriteSystem());
	enemyEmitterRight->setChildImage(hand_img);
	enemyEmitterRight->setPosition(glm::vec3(ofGetWindowWidth() / 2, -40, 0));
	enemyEmitterRight->setVelocity(glm::vec3(0, 600, 0));
	enemyEmitterRight->setLifespan(2500);
	enemyEmitterRight->setRate(1.5);
	enemyEmitterRight->setShootPattern(Sinewave);
	enemyEmitterRight->sys->isSinewave = true;
	enemyEmitterRight->sys->backwards = true;
	enemyEmitterRight->setMotionPath(BackAndForth);
	enemyEmitterRight->setMoveAcceleration(glm::vec3(150, 0, 0));
	enemyEmitterRight->setMoveVelocity(glm::vec3(150, 0, 0));
	enemyEmitterRight->spriteLifePoints = 2;
	enemyEmitterRight->stop();
	
	backgroundEmitter = new Emitter(new SpriteSystem());
	backgroundEmitter->setChildImage(back_img);
	backgroundEmitter->setPosition(glm::vec3(ofGetWindowWidth() / 2, -667, 0));
	backgroundEmitter->setVelocity(glm::vec3(0, 200, 0));
	backgroundEmitter->setLifespan(10000);
	backgroundEmitter->setRate(0.2);
	backgroundEmitter->start();

	backgroundLayerEmitter = new Emitter(new SpriteSystem());
	backgroundLayerEmitter->setChildImage(layer_img);
	backgroundLayerEmitter->setPosition(glm::vec3(ofGetWindowWidth() / 2, -667, 0));
	backgroundLayerEmitter->setVelocity(glm::vec3(0, 300, 0));
	backgroundLayerEmitter->setLifespan(10000);
	backgroundLayerEmitter->setRate(0.4);
	backgroundLayerEmitter->start();

	// Setup GUI
	gui.setup();
	gui.add(rateoffireSlider.setup("rate", 8, 1, 30));
	gui.add(velocitySlider.setup("velocity", glm::vec3(0, -600, 0), glm::vec3(-1000, -1000, -1000), glm::vec3(1000, 1000, 1000)));
	gui.add(goldenratioToggle.setup("use golden ratio", false));
	gui.add(sqrttwoToggle.setup("use sqrt 2 ratio", false));
	gui.add(sqrtfourToggle.setup("use sqrt 4 ratio", false));
	gui.add(explosionPosition.setup("explosion", glm::vec3(372.449, 479.592, 785.714), glm::vec3(-2000, -2000, -2000), glm::vec3(2000, 2000, 2000)));

	// Scoreboard
	score = 0;
	paladin.drawString(std::to_string(score), ofGetWindowWidth() / 2, 40);

	// HealthBar
	healthbar = ofRectangle(20, 20, 100, 20);

	// Explosion particle emitter
	explosion.sys->addForce(new ImpulseRadialForce(500, 1.0));
	explosion.setOneShot(true);
	explosion.setEmitterType(RadialEmitter);
	explosion.setGroupSize(50);
	explosion.setLifespan(2.0);
	explosion.setRate(1.0);
	//explosion.setPosition(glm::vec3(500, 500, -500));
	explosion.setPosition(glm::vec3(explosionPosition));
	explosion.setColor(ofColor(181.0, 0.0, 255.0));

	// Explosion for hand
	handExplosion.sys->addForce(new ImpulseRadialForce(30000, 1.0));
	handExplosion.setOneShot(true);
	handExplosion.setEmitterType(RadialEmitter);
	handExplosion.setGroupSize(100);
	handExplosion.setLifespan(0.5);
	handExplosion.setRate(1.0);
	handExplosion.setParticleRadius(5.0);
	handExplosion.setColor(ofColor(100.0, 0.0, 0.0));

	// Explosion for enemy bullet
	bulletExplosion.sys->addForce(new ImpulseRadialForce(10000, 1.0));
	bulletExplosion.setOneShot(true);
	bulletExplosion.setEmitterType(RadialEmitter);
	bulletExplosion.setGroupSize(100);
	bulletExplosion.setLifespan(0.5);
	bulletExplosion.setRate(1.0);
	bulletExplosion.setParticleRadius(1.0);
	bulletExplosion.setColor(ofColor(181.0, 0.0, 255.0));

	finalExplosion.sys->addForce(new ImpulseRadialForce(50000, 1.0));
	finalExplosion.setOneShot(true);
	finalExplosion.setEmitterType(RadialEmitter);
	finalExplosion.setGroupSize(1000);
	finalExplosion.setLifespan(0.5);
	finalExplosion.setRate(1.0);
	finalExplosion.setParticleRadius(5.0);
	finalExplosion.setColor(ofColor(181.0, 0.0, 255.0));

	childExplosion.sys->addForce(new ImpulseRadialForce(10000, 1.0));
	childExplosion.setOneShot(true);
	childExplosion.setEmitterType(RadialEmitter);
	childExplosion.setGroupSize(100);
	childExplosion.setLifespan(0.5);
	childExplosion.setRate(1.0);
	childExplosion.setParticleRadius(1.0);
	childExplosion.setColor(ofColor(133.0, 200.0, 89.0));
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	switch (gamestate) {
	case StartPressed:
	{
		float currentTime = ofGetElapsedTimeMillis();
		if (currentTime - lastFade > 1000.0) {
			--fadeNumber;
			if (fadeNumber == 0) {
				gamestate = Intro;
				introSound.play();
				fadeNumber = 0;
			}
		}
	}
	case Title:
		// Logic for beginning animation
		if (time > 10) {
			if (imageCount == 4) {
				imgX = 0;
				imgY = 0;
				imageCount = 1;
			}
			else {
				imgX += 768;
				imageCount++;
			}
			time = 0;
		}
		time += 1 + (1 / 60) * ofGetFrameRate();

		break;
	case Intro:
	{
		float currentTime = ofGetElapsedTimeMillis();
		if (currentTime - lastFade > 1000.0 && fadeNumber < 255 && !isFadedIn) {
			bossEmitter->setAlpha(++fadeNumber);
			if (fadeNumber == 255) {
				isFadedIn = true;
				bossEmitter->animateImage(20, 57, 336, 336);
				lastFade = currentTime;
			}
		}
		else if (isFadedIn && (currentTime - lastFade > 12000.0)) {
			gamestate = Running;
			bossEmitter->animateImage(1, 19, 336, 336);
			ofSetBackgroundColor(ofColor::black);
			introSound.stop();
			bossThemeSound.play();
			bossEmitter->setMotionPath(BackAndForth);
			bossEmitter->setMoveVelocity(glm::vec3(-150, 0, 0));
			bossEmitter->setMoveAcceleration(glm::vec3(-150, 0, 0));
			lastPatternChange = ofGetElapsedTimeMillis();
			lastNormalShotSwap = ofGetElapsedTimeMillis();
		}
		
		if (bossEmitter->getCurrentFrameCount() == 57) {
			bossEmitter->animateImage(58, 60, 336, 336);
		}

		else if (bossEmitter->getCurrentFrameCount() == 59) {
			explosion.sys->reset();
			explosion.start();
		}
		
		explosion.update();
		bossEmitter->update();

	}
		break;
	case Running:
	{
		// end the game
		if (bosshealth <= 0) {
			enemyEmitterRight->stop();
			bossEmitter->stop();
			finalExplosion.setPosition(bossEmitter->getPosition());
			finalExplosion.start();
			endGameTimer = ofGetElapsedTimeMillis();
			gamestate = Endgame;

			// Sound
			bossThemeSound.stop();
			finalSound.play();
			bossLaugh.stop();
			flowerSound.stop();

			break;
		}

		// If audio messes up, restart sound
		if (!bossThemeSound.isPlaying())
			bossThemeSound.play();

		// Shooting patterns
		float time = ofGetElapsedTimeMillis();

		// Delay hand emission at the beginning of the battle
		if (handsWait) {
			if ((time - lastPatternChange) > 1500) {
				enemyEmitterRight->start();
				handsWait = false;
			}
		}

		if (bossEmitter->getShootPattern() == Normal) { // starting shooting pattern

			if ((time - lastNormalShotSwap) > 1000) {
				if (!bulletSound.isPlaying()) {
					bulletSound.play();
				}
				else {
					emitCount++;
					patternCount++;
				}

				bossEmitter->swap();
				lastNormalShotSwap = time;

				// Fire child emitters
				if (emitCount == 4) {
					bossEmitter->swapEmitter();
					emitCount = 0;
				}

				// Change pattern
				if (patternCount == 3) {
					bossEmitter->stop();
					bossEmitter->stopEmitter();
					bossEmitter->setShootPattern(Sqrt2);
					bossEmitter->setMotionPath(Center);
					bossEmitter->setRate(15);
					patternCount = 0;
				}
			}
		}
		else if (bossEmitter->getShootPattern() == Sqrt2 || bossEmitter->getShootPattern() == Sqrt4) { // Second phase
			if (bossEmitter->getPosition().x == (ofGetWindowWidth() / 2) && animateFlag) {
				bossEmitter->animateImage(20, 58, 336, 336);
				animateFlag = false;
			}

			if (bossEmitter->getCurrentFrameCount() == 57) {
				bossEmitter->animateImage(58, 60, 336, 336);
				bossEmitter->start();
				flowerSound.play();
			}
			else if (bossEmitter->getCurrentFrameCount() == 59) {
				explosion.sys->reset();
				explosion.start();
			}

			if ((time - lastNormalShotSwap) > 5000) {
				if (bossEmitter->getShootPattern() == Sqrt2)
					bossEmitter->setShootPattern(Sqrt4);
				else
					bossEmitter->setShootPattern(Sqrt2);
				patternCount++;
				lastNormalShotSwap = time;
			}

			if (patternCount == 4) {
				animateFlag = true;
				bossEmitter->stop();
				bossEmitter->setShootPattern(Normal);
				bossEmitter->setMotionPath(BackAndForth);
				bossEmitter->setRate(15);
				patternCount = 0;
				bossEmitter->animateImage(1, 19, 336, 336);
				bossEmitter->setMoveAcceleration(glm::vec3(-150, 0, 0));
				bossEmitter->setMoveVelocity(glm::vec3(-150, 0, 0));
			}

		}


		// Emitter updates
		bossEmitter->update();

		turret->setRate(rateoffireSlider);
		turret->setVelocity(glm::vec3(velocitySlider));
		turret->update();

		explosion.setPosition(glm::vec3(explosionPosition));

		backgroundEmitter->update();
		backgroundLayerEmitter->update();

		if (enemyEmitter != NULL)
			enemyEmitter->update();

		if (enemyEmitterRight != NULL)
			enemyEmitterRight->update();

		// check collisions between sprites
		checkCollisions();

		explosion.update();
		handExplosion.update();
		bulletExplosion.update();
		childExplosion.update();
	}
		break;
	case Endgame:
		backgroundEmitter->update();
		backgroundLayerEmitter->update();

		turret->update();
		explosion.update();
		handExplosion.update();
		finalExplosion.update();
		break;
	}
			
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (gamestate == Endgame) {

		backgroundEmitter->draw();
		backgroundLayerEmitter->draw();

		turret->draw();
		explosion.draw();
		childExplosion.draw();
		bulletExplosion.draw();
		finalExplosion.draw();

		// Show stats
		float time = ofGetElapsedTimeMillis();
		if ((time - endGameTimer) > 2000.0) {
			

			if (!flowerSound.isPlaying())
				flowerSound.play();
			

			ofSetColor(ofColor::white);
			paladin.drawString("Final Score: " + std::to_string(score), ofGetWindowWidth() / 2 - 180,  300);
			paladin.drawString("Thanks for playing!", ofGetWindowWidth() / 2 - 220, 325);
		}

		return;
	}

	// Return if game has not started yet
	if (gamestate == Title || gamestate == StartPressed) {
		ofSetColor(255, 255, 255, fadeNumber);
		// Animate title
		titleImage.drawSubsection(titleX, titleY, 768, 384, imgX, imgY, 768, 384);

		paladin.drawString("Press Spacebar", ofGetWindowWidth() / 2 - 180, 800);

		return;
	}
	else if (gamestate == Intro) {
		if (bossEmitter->getCurrentFrameCount() == 60)
			ofSetBackgroundColor(101, 0, 122);
		else
			ofSetBackgroundColor(ofColor::black);
		bossEmitter->draw();
		explosion.draw();
		return;
	}

	backgroundEmitter->draw();
	backgroundLayerEmitter->draw();

	// Draw scoreboard
	ofSetColor(255, 255, 255);
	//ofDrawBitmapString(score, ofGetWindowWidth() / 2, 40);
	paladin.drawString("Boss Health", 20, 40);
	paladin.drawString("score: " + std::to_string(score), ofGetWindowWidth() - 300, 40);

	ofSetColor(181, 0, 255);
	explosion.draw();

	// Emitter draw calls
	bossEmitter->draw();
	
	turret->draw();

	// explosion when hands are destroyed
	childExplosion.draw();
	bulletExplosion.draw();
	handExplosion.draw();
	

	if (enemyEmitter != NULL)
		enemyEmitter->draw();

	if (enemyEmitterRight != NULL)
		enemyEmitterRight->draw();

	ofSetColor(255, 0, 0);
	ofFill();
	ofDrawRectangle(ofRectangle(glm::vec3(20, 50, 0), bosshealth, 20));

	if (!hideGui)
		gui.draw();
}


//--------------------------------------------------------------
void ofApp::checkCollisions() {

	// Collision of turret bullets to boss
	// Iterate through each bullet shot by turret
	for (int i=0; i < turret->sys->sprites.size(); i++) {
		int bossFlag = bossEmitter->detectHit(turret->sys->sprites[i].getPosition(), turret->sys->sprites[i].getWidth(), turret->sys->sprites[i].getHeight());
		
		if (bossFlag == 1 || bossFlag == 0) { // Emitter was hit
			// Update score
			score++;
			
			// Play sound
			cout << "hit" << endl;
			hitSound.play();
			
			// Remove bullet
			turret->sys->remove(i);

			// highlight boss for visual feedback
			bossEmitter->wasDamaged();
			//wasBossHit = true;

			bosshealth--;
			break;

			if (bossFlag == 0) {
				break;
			}
		}

		// Detect if bullet has hit enemy of the hands
		if (!enemyEmitterRight->sys->sprites.empty()) {
			for (int j = 0; j < enemyEmitterRight->sys->sprites.size(); j++) {
				int flag = enemyEmitterRight->sys->sprites[j].detectHit(turret->sys->sprites[i].getPosition(), turret->sys->sprites[i].getWidth(), turret->sys->sprites[i].getHeight());
				if (flag == 1 || flag == 0) {
					score++;
					// Remove bullet
					turret->sys->remove(i);

					if (enemyEmitterRight->sys->sprites[j].lifePoints == 0) {
						
						// Explosion
						handExplosion.setPosition(enemyEmitterRight->sys->sprites[j].getPosition());
						handExplosion.start();
						handExplosion.sys->reset();
						enemyEmitterRight->sys->remove(j);
						handSound.play();
					}
					cout << "hand destroyed" << endl;
					hitSound.play();

					break;
				}
			}
		}
		
	}

	// Collision detection for bullets on turret
	// Iterate through spriteSystems
	for (int i = 0; i < bossEmitter->sys->sprites.size(); i++) {
		int flag = turret->detectHit(bossEmitter->sys->sprites[i].getPosition(), bossEmitter->sys->sprites[i].getWidth(), bossEmitter->sys->sprites[i].getHeight());
		if (flag == 1 || flag == 0) {

			bulletExplosion.setPosition(bossEmitter->sys->sprites[i].getPosition());
			bulletExplosion.start();
			bulletExplosion.sys->reset();

			score--;
			bossEmitter->sys->remove(i);
			//cout << "ouch!" << endl;
			damageSound.play();
			turret->wasDamaged();
		}
	}

	// Collision detection for hand sprites on turret
	for (int i = 0; i < enemyEmitterRight->sys->sprites.size(); i++) {
		int flag = turret->detectHit(enemyEmitterRight->sys->sprites[i].getPosition(), enemyEmitterRight->sys->sprites[i].getWidth(), enemyEmitterRight->sys->sprites[i].getHeight());
		if (flag == 1 || flag == 0) {
			score -= 20;

			// Explosion
			handExplosion.setPosition(enemyEmitterRight->sys->sprites[i].getPosition());
			handExplosion.start();
			handExplosion.sys->reset();
			enemyEmitterRight->sys->remove(i);
			handSound.play();
			bossLaugh.play();

			//cout << "ouch!" << endl;
			turret->wasDamaged();
		}
	}

	// Collision between ship and child emitter sprites
	if (!bossEmitter->emitSys->emitters.empty()) {

		for (int i = 0; i < bossEmitter->emitSys->emitters.size(); i++) {

			if (!bossEmitter->emitSys->emitters[i].sys->sprites.empty()) {
				for (int j = 0; j < bossEmitter->emitSys->emitters[i].sys->sprites.size(); j++) {
					int flag = turret->detectHit(bossEmitter->emitSys->emitters[i].sys->sprites[j].getPosition(),
						bossEmitter->emitSys->emitters[i].sys->sprites[j].getWidth(),
						bossEmitter->emitSys->emitters[i].sys->sprites[j].getHeight());
					if (flag == 1 || flag == 0) {
						score -= 5;
						cout << "HIt" << endl;
						childExplosion.setPosition(bossEmitter->emitSys->emitters[i].sys->sprites[j].getPosition());
						childExplosion.start();
						childExplosion.sys->reset();

						score--;
						bossEmitter->emitSys->emitters[i].sys->remove(j);
						damageSound.play();
						turret->wasDamaged();
					}
				}
			}
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case ' ':
		if (gamestate == Title) { // Start game
			gamestate = StartPressed;
			cout << "Game started" << endl;
			startingTheme.stop();
			lastFade = ofGetElapsedTimeMillis();
			bossLaugh.play();
		}
		else if (gamestate == Running || gamestate == Endgame) { // Fire projectiles
			if (!missleFire.isPlaying()) {
				turret->start();
				missleFire.play();
			}
		}
		break;
	case 'h':
	case 'H':
		hideGui = !hideGui;
		break;
	case OF_KEY_RIGHT:
		turret->moveDir(MoveRight);
		break;
	case OF_KEY_LEFT:
		turret->moveDir(MoveLeft);
		break;
	case OF_KEY_UP:
		turret->moveDir(MoveUp);
		break;
	case OF_KEY_DOWN:
		turret->moveDir(MoveDown);
		break; 
	case 'e':
	case 'E':
		if (gamestate == Running) {
			bossEmitter->start();
		}
		break;
	case 'r':
	case 'R':
		if (gamestate == Running) {
			bossEmitter->startEmitter();
		}
	case 'b':
	case 'B':
		// Bypass intro
		if (gamestate == Title) {
			startingTheme.stop();
			bossThemeSound.play();
			gamestate = Running;
			bossEmitter->setMotionPath(BackAndForth);
			bossEmitter->setMoveVelocity(glm::vec3(-150, 0, 0));
			bossEmitter->setMoveAcceleration(glm::vec3(-150, 0, 0));
			lastPatternChange = ofGetElapsedTimeMillis();
			bossEmitter->setPosition(glm::vec3(ofGetWindowWidth() / 2, 300, 0));
			lastNormalShotSwap = ofGetElapsedTimeMillis();
		}
		break;
	case 'g':
	case 'G':
		// Test explosion
		explosion.sys->reset();
		explosion.start();
		break;
	} 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case ' ':
		if (gamestate == Running || gamestate == Endgame) {
			turret->stop();
			missleFire.stop();
		}
		break;
	case OF_KEY_RIGHT:
		turret->stopDir(MoveRight);
		break;
	case OF_KEY_LEFT:
		turret->stopDir(MoveLeft);
		break;
	case OF_KEY_UP:
		turret->stopDir(MoveUp);
		break;
	case OF_KEY_DOWN:
		turret->stopDir(MoveDown);
		break;
	case 'e':
	case 'E':
		bossEmitter->stop();
		break;
	case 'r':
	case 'R':
		bossEmitter->stopEmitter();
		break;
		
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (!bDrag) return;	
	turret->moveWithMouse(glm::vec3(x, y, 0), lastMouse);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (turret->inside(glm::vec3(x, y, 0))) {
		bDrag = true;
		lastMouse = glm::vec3(x, y, 0);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDrag = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
