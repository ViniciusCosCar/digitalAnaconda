#include "game_objects.h"
#include "../utils.h"
#include <cstring>
///////// Pallet /////////
// Default initialization
Game_objects::Pallet::Pallet(unsigned int y, unsigned int x, const Sprite sprite){
	if(strlen(sprite)>PIXEL_SIZE)
		error("At least one of given sprites exceed maximum pixel's length");
	for(unsigned int i=0; i<SIZE; i++){
		strcpy(body[i].sprite, sprite);
		body[i].y = y+i; body[i].x = x;
	}
}
// Write body to map. 'true' if process was allowed by 'win'
bool Game_objects::Pallet::write(Screen &win){
	// First check for collision
	for(Dot& dot : body){
		if(win.collisionOnDot(dot)) return false;
	}
	// If it's alright, then write the object to screen
	for(Dot& dot : body){
		win.write(dot);
	}
	return true;
}
// Update body coordinates in screen. "true" if was able to move object on map
bool Game_objects::Pallet::tryMove(int y, Screen &win){
	for(Dot& dot : body){
		win.clear(dot);
		dot.y += y;
	}
	return write(win);
}
// Move object on map
bool Game_objects::Pallet::move(int y, Screen &win){
	if(!tryMove(y, win)){ // There was a collision
		for(Dot& dot : body) dot.y -= y;
		write(win);
		return false;
	}
	// There were no collisions
	return true;
}
