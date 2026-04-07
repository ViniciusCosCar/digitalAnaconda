#include "game.h"
#include "../utils.h"
#include <cstring>
#include <iostream>

///////// Ball /////////
Ball::Ball(Screen &win, Pos pos, const Sprite sprite){
	if(!(pos.y >= 0 && pos.y < win.MAX_Y && pos.x >= 0 && pos.x < win.MAX_X))
		error("Trying to write Ball outside of screen's map");
	body.y = pos.y;
	body.x = pos.x;
	strcpy(win.objectsSprites[BALL], sprite);
}

// Get information about collision after offset({0,0} as default)
CollisionInfo Ball::getCollisionInfo(Screen &win, Pos offset) {
	return win.getCollisionInfo(
		{body.y, body.x},
			offset
	);
}
// Assuming no collision, write object to map
void Ball::writeToScreen(Screen& win){
	win.map[body.y + win.BORDERS_WIDTH][body.x + win.BORDERS_WIDTH] = BALL;
}
void Ball::clear(Screen& win){
	win.map[body.y + win.BORDERS_WIDTH][body.x + win.BORDERS_WIDTH] = NO_TARGET;
}

void Ball::moveInScreen(Screen& win, Pos offset){
	// Write to map
	win.map[body.y + offset.y + win.BORDERS_WIDTH][body.x + offset.x + win.BORDERS_WIDTH] = BALL;
	
	// Update position
	body.y += offset.y;
	body.x += offset.x;
}
