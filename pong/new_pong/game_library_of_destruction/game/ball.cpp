#include "game.h"
#include "../utils.h"
#include <cstring>

///////// Ball /////////
Ball::Ball(Window &win, Pos pos, const Sprite sprite){
	if(!(pos.y >= 0 && pos.y < win.MAX_Y && pos.x >= 0 && pos.x < win.MAX_X))
		error("Trying to write Ball outside of screen's map");
	body.y = pos.y;
	body.x = pos.x;
	strcpy(win.objectsSprites[BALL], sprite);
}

// Get information about collision after offset({0,0} as default)
CollisionInfo Ball::getCollisionInfo(Window &win, Pos d) {
	return win.getCollisionInfo(
		{body.y, body.x},
			d
	);
}
// Assuming no collision, write object to map
void Ball::writeToWindow(Window& win){
	win.map[body.y + win.BORDERS_WIDTH][body.x + win.BORDERS_WIDTH] = BALL;
}
void Ball::clear(Window& win){
	win.map[body.y + win.BORDERS_WIDTH][body.x + win.BORDERS_WIDTH] = NO_TARGET;
}

void Ball::moveInWindow(Window& win, Pos d){
	// Write to map
	win.map[body.y + d.y + win.BORDERS_WIDTH][body.x + d.x + win.BORDERS_WIDTH] = BALL;
	
	// Update position
	body.y += d.y;
	body.x += d.x;
}
