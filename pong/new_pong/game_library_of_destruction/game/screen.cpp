#include "game.h"
#include "../utils.h"
#include <cstring>
#include <iostream>
using namespace std;
///////// Screen /////////
Direction Screen::movementDirection(Pos offset){
	int dy = offset.y;
	int dx = offset.x;

	if	( dy <  0 && dx == 0 )	return 	TOP;
	else if	( dy >  0 && dx == 0 ) 	return	BOTTOM;
	else if	( dy == 0 && dx <  0 )	return  LEFT;
	else if	( dy == 0 && dx >  0 )	return  RIGHT;
	else if	( dy <  0 && dx <  0 )	return 	TOP_LEFT;
	else if	( dy <  0 && dx >  0 )	return	TOP_RIGHT;
	else if	( dy >  0 && dx <  0 )	return	BOTTOM_LEFT;
	else				return  BOTTOM_RIGHT;
}

// Return information about collision in current and future position
CollisionInfo Screen::getCollisionInfo(Pos current_pos, Pos offset){
	int dy = offset.y;
	int dx = offset.x;

	int y = current_pos.y;
	int x = current_pos.x;
	
	// Border collisions
	if(y+dy < 0     ) 	return { TOP, BORDER };
	if(y+dy >= MAX_Y) 	return { BOTTOM, BORDER };
	if(x+dx < 0     ) 	return { LEFT, BORDER };
	if(x+dx >= MAX_X) 	return { RIGHT, BORDER };

	// Object in intended position
	Object target = map[y+dy+BORDERS_WIDTH][x+dx+BORDERS_WIDTH];

	// No movement collision
	if( dy == 0 && dx == 0 )	return { NONE, target };

	// Movement
	return { movementDirection(offset), target };
}

// Write object to screen if there is no collision. Returns "false" otherwise
bool Screen::write(GameObject &obj){
	CollisionInfo collision = obj.getCollisionInfo(*this);

	if(collision.target != NO_ONE)	{
		obj.handleCollision(collision, *this);
		return false;
	}

	obj.writeToScreen(*this);
	return true;
}

bool Screen::move(GameObject &obj, Pos offset){
	CollisionInfo collision = obj.getCollisionInfo(*this, offset);

	// Collision
	if(collision.target != NO_ONE)	{ 
		obj.handleCollision(collision, *this);
		return false;
	}
	// No collision

	// Clear object last position
	if(offset.y != 0 || offset.x != 0) 
		obj.clear(*this);

	obj.moveInScreen(*this, offset);
	return true;
}

bool Screen::clear(Pos pos){
	int y = pos.y;
	int x = pos.x;

	// Assure valid position
	if(y < 0 || y >= MAX_Y || x < 0 || x >= MAX_X)	{
		error("Trying to clear point outside of screen");
		return false;
	}
	// Clear
	map[y+BORDERS_WIDTH][x+BORDERS_WIDTH] = NO_ONE;
	return true;
}
// Print map
void Screen::print(){
	runBash("sleep " + std::to_string(0.01) + " && clear");
	for(unsigned int y=0; y<MAX_Y+2*BORDERS_WIDTH; y++){
		for(unsigned int x=0; x<MAX_X+2*BORDERS_WIDTH; x++){
			cout<<objectsSprites[map[y][x]];
		}
		cout<<endl;
	}
}

// Initialize map with borders and background sprites
void Screen::init(){
	// Write borders
	for(int x = 0; x < MAX_X+2*BORDERS_WIDTH; x++){
		map[0][x] = BORDER;
		map[MAX_Y+2*BORDERS_WIDTH-1][x] = BORDER;
	}
	for(int y = 1; y < MAX_Y+2*BORDERS_WIDTH-1; y++){ 
		map[y][0] = BORDER;
		map[y][MAX_X+2*BORDERS_WIDTH-1] = BORDER;
	}
	// Write background
	for(int y=BORDERS_WIDTH; y<MAX_Y+BORDERS_WIDTH; y++)
		for(int x=BORDERS_WIDTH; x<MAX_X+BORDERS_WIDTH; x++)
			map[y][x] = NO_ONE;
}
// Default initializer
Screen::Screen(){
	strcpy(objectsSprites[NO_ONE], "🏁");
	strcpy(objectsSprites[BORDER], "🔲");
	init(); 
}
// Initialize with given sprites for background and borders
Screen::Screen(const Sprite bgSprite, const Sprite bdSprite){
	if(strlen(bgSprite)>PIXEL_SIZE || strlen(bdSprite)>PIXEL_SIZE)
		error("At least one of given sprites exceed maximum pixel's length");

	strcpy(objectsSprites[NO_ONE], bgSprite);
	strcpy(objectsSprites[BORDER], bdSprite);
	init();
}
