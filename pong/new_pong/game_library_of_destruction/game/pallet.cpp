#include "game.h"
#include "../utils.h"
#include <cstring>
using namespace std;

///////// Pallet /////////
Pallet::Pallet(Window& win, Pos pos, const Sprite sprite){
	if(!(pos.y >= 0 && pos.y < win.MAX_Y && pos.x >= 0 && pos.x < win.MAX_X)) 
		error("Trying to write Pallet outside of screen's map");
	for(int i = 0; i < SIZE; i++){
		body[i].y = pos.y + i; 
		body[i].x = pos.x;
		strcpy(win.objectsSprites[PALLET], sprite);
	}
}

// If a collision is found, immediatly return collision info. 
CollisionInfo Pallet::getCollisionInfo(Window& win, Pos d) {
	if(d.y>0) return win.getCollisionInfo(body[SIZE-1], d);
	if(d.y<0) return win.getCollisionInfo(body[0], d);

	CollisionInfo collision;
	for(Pos& e : body){
		collision.target = win.getCollisionInfo(e, d).target;
		if(collision.target.yx != NO_TARGET || collision.target.y  != NO_TARGET || collision.target.x  != NO_TARGET)
			break;
	}
	return collision;
}

void Pallet::writeToWindow(Window& win){
	for(Pos& e : body)
		win.map[e.y + win.BORDERS_WIDTH][e.x + win.BORDERS_WIDTH] = PALLET;
}

void Pallet::clear(Window& win){
	for(Pos& e : body)
		win.map[e.y + win.BORDERS_WIDTH][e.x + win.BORDERS_WIDTH] = NO_TARGET;
}

void Pallet::moveInWindow(Window& win, Pos offset){
	for(Pos& e : body) {
		e.y += offset.y;
		e.x += offset.x;
		win.map[e.y + win.BORDERS_WIDTH][e.x + win.BORDERS_WIDTH] = PALLET;
	}
}
