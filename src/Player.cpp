/*
 * Player.cpp
 *
 *  Created on: May 8, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include "Player.h"

// Default constructor
Player::Player() {
	setPosition(64, 360/2-112/2);
	w = 128;
	h = 64;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	holdPower = false;
	shoot = false;
	shootTimer = -1;
	speedX = 0.0;
	speedY = 0.0;
	xPower = 0.0;
	yPower = 0.0;
	health = 100.0;
	angle = 0.0;
	turretAngle = 0.0;
	sPongScore = NULL;
	alpha = 255;
	flashTimer = 0;
	flash = false;
	alive = true;
}

void Player::reset() {
	setPosition(64, 360/2-112/2);
	w = 128;
	h = 64;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	holdPower = false;
	shoot = false;
	shootTimer = -1;
	speedX = 0.0;
	speedY = 0.0;
	xPower = 0.0;
	yPower = 0.0;
	health = 100.0;
	angle = 0.0;
	turretAngle = 0.0;
	alpha = 255;
	flashTimer = 0;
	flash = false;
	alive = true;
}

// Set Player position
void Player::init(Mix_Chunk *sPongScore) {
	this->sPongScore = sPongScore;
}

// Set Player position
void Player::setPosition(int spawnX, int spawnY) {
	x = spawnX;
	y = spawnY;
}

// Set Player x position
void Player::setX(int spawnX) {
	x = spawnX;
}

// Set Player y position
void Player::setY(int spawnY) {
	y = spawnY;
}

// Add XPower
void Player::addXPower(float amount) {
	if (xPower < 5) {
		xPower += amount;
	}
}

// Add YPower
void Player::addYPower(float amount) {
	yPower = amount;
}

// Hurt Player
void Player::Hurt(float damage) {
	health -= damage;
}

// Return Player x position
float Player::getX() {
	return x;
}

// Return Player y position
float Player::getY() {
	return y;
}

// Get Player left side
float Player::getLeftSide() {
	return x;
}

// Get Player right side
float Player::getRightSide() {
	return x+w;
}

// Get Player top side
float Player::getTopSide() {
	return y;
}

// Get Player bottom side
float Player::getBottomSide() {
	return y+h;
}

// Return Player width
float Player::getWidth() {
	return w;
}

// Return Player height
float Player::getHeight() {
	return h;
}

// Return Player width
float Player::getCenterX() {
	return x+w/2;
}

// Return Player height
float Player::getCenterY() {
	return y+h/2;
}

// Return Player y speed
float Player::getSpeedY() {
	return speedY;
}

// Update Player
void Player::update(Particle &part, Particle particles[]) {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	if (moveLeft) {
		if (angle > -20) {
			angle -= 1;
		}
	}
	else if (moveRight) {
		if (angle < 20) {
			angle += 1;
		}
	}

	// Move player left
	if (moveLeft && speedX > -6) {
		speedX -= 0.26;
	}
	// Move player right
	else if (moveRight && speedX < 6) {
		speedX += 0.37;
	}

	// No movement, start decay
	if (!moveLeft && !moveRight) {
		speedX = speedX - speedX * 0.05;

		// Level coptor after tilting
		if (angle != 0) {
			angle = angle - angle * 0.05;
		}
	}

	// Add movement
	x += speedX;

	// Move player up
	if (moveUp && speedY > -6) {
		speedY -= 0.25;
	}
	// Move player down
	else if (moveDown && speedY < 6) {
		speedY += 0.25;
	}

	// No movement, start decay
	if (!moveUp && !moveDown) {
	//	speedY = speedY - speedY * 0.09;
	}

	// Gravity
	speedY += grav;

	// Add movement
	y += speedY;

	// Player left of screen bound
	if (x < 0) {
		x = 0;
		speedX = 0.0;
	}
	// Player right of screen bound
	if (x + w > screenWidth) {
		x = screenWidth - w;
		speedX = 0.0;
	}

	// Player top of screen bound
	if (y < 0) {
		y = 0;
		speedY = 0.0;
	}
	// Player bottom of screen bound
	if (y + h > screenHeight - 28) {
		y = screenHeight - 28 - h;

		// If not flashing damaged
		if (!flash) {
			// Hurt player
			speedY = -5.0;
			flash = true;

			// Play SFX
			Mix_PlayChannel(-1, sPongScore, 0);

			// Spawn explosion
			part.SpawnExplosion(particles,
					this->x + this->w / 2,
					this->y + this->h / 2, { 200,
							200, 200 });

			// Health
			health -= 25;
		}
	}

	// Shoot power
	if (holdPower) {
		addXPower(0.1);
	}

	if (health <= 0) {
		health = 0;
		alive = false;
	}

	// Flash enemy
	if (flash) {

		flashTimer += 2;

		if (flashTimer < 15) {
			alpha = 90;
		}

		else if (flashTimer >= 15 && flashTimer < 30) {
			alpha = 170;
		}

		else if (flashTimer >= 30 && flashTimer < 45) {
			alpha = 90;
		}

		else if (flashTimer >= 45 && flashTimer < 60) {
			alpha = 170;
		} else {
			alpha = 255;
			flashTimer = 0;
			flash = false;
		}
	}
}

// Draw Player
void Player::render(SDL_Renderer *gRenderer) {

	// Draw rectangle
	//SDL_Rect tempRect = {x, y, w, h};
	//SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	//SDL_RenderDrawRect(gRenderer, &tempRect);

	// Create Health bar
	SDL_Rect tempRect = {x, y-20, w, 15};
	SDL_SetRenderDrawColor(gRenderer, 0, 50, 0, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	tempRect = {x, y-20, (w*health)/100, 15};
	SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	tempRect = {x, y-20, w, 15};
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);
}
