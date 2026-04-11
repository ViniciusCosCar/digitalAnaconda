#include "game.h"
#include "../utils.h"
#include <cstring>
#include <iostream>
using namespace std;
///////// Window /////////

// Return information about collision in current and future position
CollisionInfo Window::getCollisionInfo(Pos current_pos, Pos d){
	int y = current_pos.y;
	int x = current_pos.x;

	Direction direction; 	// Default = No direction
	Target target;		// Default = No target

	// Oblique movement
	if(d.y*d.x != 0){ 
		// Oblique collision
		target.yx = map[y+d.y+BORDERS_WIDTH][x+d.x+BORDERS_WIDTH];
		// Vertical and horizontal collision
		if((target.y = map[y+d.y+BORDERS_WIDTH][x+BORDERS_WIDTH])!=NO_TARGET) direction.y = (d.y>0)?DOWN:UP;
		if((target.x = map[y+BORDERS_WIDTH][x+d.x+BORDERS_WIDTH])!=NO_TARGET) direction.x = (d.x>0)?RIGHT:LEFT;
		return { direction, target };
	}

	// Horizontal movement
	if(d.y == 0 && d.x != 0){
		if((target.x = map[y+BORDERS_WIDTH][x+d.x+BORDERS_WIDTH])!=NO_TARGET)
			direction.x = (d.x>0)?RIGHT:LEFT;
		return { direction, target };
	}

	// Vertical movement
	if(d.y != 0 && d.x == 0){
		if((target.y = map[y+d.y+BORDERS_WIDTH][x+BORDERS_WIDTH])!=NO_TARGET)
			direction.y = (d.y>0)?DOWN:UP;
		return { direction, target };
	}

	// When placing object
	return { direction,  map[y+BORDERS_WIDTH][x+BORDERS_WIDTH] };
}

// Write object to screen if there is no collision. Returns "false" otherwise
bool Window::write(GameObject &obj){
	CollisionInfo collision = obj.getCollisionInfo(*this);

	if(collision.target.yx != NO_TARGET) {
		obj.handleCollision(collision, *this);
		return false;
	}

	obj.writeToWindow(*this);
	return true;
}

// Move object if no colllision, return false otherwise
bool Window::move(GameObject &obj, Pos d){
	CollisionInfo collision = obj.getCollisionInfo(*this, d);

	// Collision
	if(collision.target.yx != NO_TARGET || 
	   collision.target.y  != NO_TARGET || 
	   collision.target.x  != NO_TARGET) {
		obj.handleCollision(collision, *this);
		return false;
	}
	// No collision

	// Clear object last position
	if(d.y != 0 || d.x != 0) 
		obj.clear(*this);

	obj.moveInWindow(*this, d);
	return true;
}

// For now, crashes the game if position is invalid
bool Window::clear(Pos pos){
	int y = pos.y;
	int x = pos.x;

	// Assure valid position
	if(y < 0 || y >= MAX_Y || x < 0 || x >= MAX_X)	{
		error("Trying to clear point outside of screen");
		return false;
	}
	// Clear
	map[y+BORDERS_WIDTH][x+BORDERS_WIDTH] = NO_TARGET;
	return true;
}
// Print map
void Window::print(){
	runBash("sleep " + std::to_string(0.01) + " && clear");
	for(unsigned int y=0; y<MAX_Y+2*BORDERS_WIDTH; y++){
		for(unsigned int x=0; x<MAX_X+2*BORDERS_WIDTH; x++){
			cout<<objectsSprites[map[y][x]];
		}
		cout<<endl;
	}
}
// Initialize map with borders and background sprites
void Window::init(){
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
			map[y][x] = NO_TARGET;
}
// Default initializer
Window::Window(){
	strcpy(objectsSprites[NO_TARGET], "🏁");
	strcpy(objectsSprites[BORDER], "🔲");
	init(); 
}
// Initialize with given sprites for background and borders
Window::Window(const Sprite bgSprite, const Sprite bdSprite){
	if(strlen(bgSprite)>PIXEL_SIZE || strlen(bdSprite)>PIXEL_SIZE)
		error("At least one of given sprites exceed maximum pixel's length");

	strcpy(objectsSprites[NO_TARGET], bgSprite);
	strcpy(objectsSprites[BORDER], bdSprite);
	init();
}
