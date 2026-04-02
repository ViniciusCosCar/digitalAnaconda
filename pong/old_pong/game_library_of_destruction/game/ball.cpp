#include "game_objects.h"
#include "../utils.h"
#include <cstring>

using namespace Game_objects;
///////// Ball /////////
// Initialize with (y,x) coordinates and given sprite
Ball::Ball(unsigned int y, unsigned int x, const Sprite sprite) {
	if(strlen(sprite)>PIXEL_SIZE)
		error("At least one of given sprites exceed maximum pixel's length");
	strcpy(body.sprite, sprite);
	body.y = y; body.x = x;
}
// Write body to map. 'true' if process was allowed by 'win'
bool Ball::write(Screen &win){ return win.write(body, BALL); }

bool Ball::tryMove(int y, int x, Screen &win){
	win.clear(body);
	body.y += y, body.x += x;
	if(write(win)) return true;
	return false;
}

bool Ball::move(int y, int x, Screen &win){
	bool collision = !tryMove(y, x, win);
	if(collision){ // There was a collision
		body.y -= y, body.x -= x;
		write(win);
		return false;
	}
	return true; // There was no collision
}
