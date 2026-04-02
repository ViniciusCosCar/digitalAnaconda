#include "game_objects.h"
#include "../utils.h"
#include <cstring>
#include <iostream>
using namespace std;
using namespace Game_objects;
///////// Screen /////////
// Initialize map with borders and background sprites
void Screen::init(){
	// Write borders
	for(int x = 0; x < MAX_X+2*BORDERS_WIDTH; x++){
		strcpy(map[0][x].sprite, bdSprite);
		strcpy(map[MAX_Y+2*BORDERS_WIDTH-1][x].sprite, bdSprite);
		map[0][x].who = map[MAX_Y+2*BORDERS_WIDTH-1][x].who = BORDER;
	}
	for(int y = 1; y < MAX_Y+2*BORDERS_WIDTH-1; y++){ 
		strcpy(map[y][0].sprite, bdSprite);
		strcpy(map[y][MAX_X+2*BORDERS_WIDTH-1].sprite, bdSprite);
		map[y][0].who = map[y][MAX_X+2*BORDERS_WIDTH-1].who = BORDER;
	}
	// Write background
	for(int y=BORDERS_WIDTH; y<MAX_Y+BORDERS_WIDTH; y++)
		for(int x=BORDERS_WIDTH; x<MAX_X+BORDERS_WIDTH; x++){
			strcpy(map[y][x].sprite, bgSprite);
		}
}
// Default initializer
Screen::Screen() : bgSprite("🏁"), bdSprite("🔲"){ init(); }
// Initialize with given sprites for background and borders
Screen::Screen(const Sprite bgSprite, const Sprite bdSprite){
	if(strlen(bgSprite)>PIXEL_SIZE || strlen(bdSprite)>PIXEL_SIZE)
		error("At least one of given sprites exceed maximum pixel's length");
	strcpy(this->bgSprite, bgSprite);
	strcpy(this->bdSprite, bdSprite);
	init();
}
// Check for object outside of map and collisions with borders and game objects, does not tell the side of collision with objects
CollisionInfo Screen::checkCollision(Dot dot, int y_move = 0, int x_move = 0){
	int y = dot.y; 
	int x = dot.x;
	
	// Border collisions
	if(y < 0) 	return { TOP, BORDER };
	if(y >= MAX_Y) 	return { BOTTOM, BORDER };
	if(x < 0) 	return { LEFT, BORDER };
	if(x >= MAX_X) 	return { RIGHT, BORDER };

	Object who = map[y+BORDERS_WIDTH][x+BORDERS_WIDTH].who;

	// No movement collision
	if(y_move==0 && x_move==0)	return { NONE, who }; 	

	// Determine collision direction
	CollisionSide side;
	if(y_move < 0 && x_move == 0)		side = TOP;
	else if(y_move > 0 && x_move == 0) 	side = BOTTOM;
	else if(y_move == 0 && x_move < 0)	side = LEFT;
	else if(y_move == 0 && x_move > 0)	side = RIGHT;
	else if(y_move < 0 && x_move < 0)	side = TOP_LEFT;
	else if(y_move < 0 && x_move > 0)	side = TOP_RIGHT;
	else if(y_move > 0 && x_move < 0)	side = BOTTOM_LEFT;
	else					side = BOTTOM_RIGHT;
	
	return { side, who };
	
}

// Write object to screen if there is no collision. Returns "false" otherwise
bool Screen::write(Dot dot, Object who){
	int y = dot.y, x = dot.x;
	CollisionInfo collision = checkCollision(dot);
	if(collision.who == NOTHING){ // No collision
		y += BORDERS_WIDTH, x += BORDERS_WIDTH;
		strcpy(map[y][x].sprite, dot.sprite);
		map[y][x].who = who;
		return true;
	} 
	return false; // Collision
}
// Clear past objects position on map
void Screen::clear(Dot dot){
	dot.y += BORDERS_WIDTH, dot.x += BORDERS_WIDTH;
	strcpy(map[dot.y][dot.x].sprite, bgSprite);
	map[dot.y][dot.x].who = NOTHING;
}
// Print map
void Screen::print(){
	runBash("sleep " + to_string(FRAME_PERIOD) + " && clear");
	for(unsigned int y=0; y<MAX_Y+2*BORDERS_WIDTH; y++){
		for(unsigned int x=0; x<MAX_X+2*BORDERS_WIDTH; x++)
			cout<<map[y][x].sprite;
		cout<<endl;
	}

}
