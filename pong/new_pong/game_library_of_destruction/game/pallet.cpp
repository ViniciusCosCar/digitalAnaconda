#include "game.h"
#include "../utils.h"
#include <cstring>
using namespace std;

///////// Pallet /////////
Pallet::Pallet(Screen& win, Pos pos, const Sprite sprite){
	if(!(pos.y >= 0 && pos.y < win.MAX_Y && pos.x >= 0 && pos.x < win.MAX_X)) 
		error("Trying to write Pallet outside of screen's map");
	for(int i = 0; i < SIZE; i++){
		body[i].y = pos.y + i; 
		body[i].x = pos.x;
		strcpy(win.objectsSprites[PALLET], sprite);
	}
}

// If a collision is found, immmediatly return colllion info. 
CollisionInfo Pallet::getCollisionInfo(Screen& win, Pos offset) {
	switch (win.getMovInfo(offset).sense) {
		case TOP:
			return win.getCollisionInfo(body[0], offset);
		case BOTTOM:
			return win.getCollisionInfo(body[SIZE-1], offset);
		default:
			CollisionInfo collision;
			for(Pos& e : body)
				if((collision = win.getCollisionInfo(e, offset)).target != NO_TARGET)
					break;
			return collision;
	}
}

void Pallet::writeToScreen(Screen& win){
	for(Pos& e : body)
		win.map[e.y + win.BORDERS_WIDTH][e.x + win.BORDERS_WIDTH] = PALLET;
}

void Pallet::clear(Screen& win){
	for(Pos& e : body)
		win.map[e.y + win.BORDERS_WIDTH][e.x + win.BORDERS_WIDTH] = NO_TARGET;
}

void Pallet::moveInScreen(Screen& win, Pos offset){
	for(Pos& e : body) {
		win.map[e.y + offset.y + win.BORDERS_WIDTH][e.x + offset.x + win.BORDERS_WIDTH] = PALLET;
		e.y += offset.y;
		e.x += offset.x;
	}
}
