#include <cstring>
#include <iostream>
#include "game_objects.h"
using namespace std;

void error(string message){ throw runtime_error(message); }
void runBash(string command){ system(command.c_str()); }

Screen::Screen(){   
	// Write borders
	for(int x = 0; x < B_MAX_X; x++){
		strcpy(dot[0][x], borderFill);
		strcpy(dot[B_MAX_Y-1][x], borderFill);
	}
	for(int y = 0; y < B_MAX_Y; y++){ 
		strcpy(dot[y][0], borderFill);
		strcpy(dot[y][B_MAX_X-1], borderFill);
	}
	// Write background
	for(int y=BORDERS_WIDTH; y<MAX_Y; y++)
		for(int x=BORDERS_WIDTH; x<MAX_X; x++)
			strcpy(dot[y][x], backgroundFill);
}
Screen::Screen(const Pixel bgFill, const Pixel bdFill){
	strcpy(this->backgroundFill,bgFill); strcpy(this->borderFill, bdFill);
	Screen();
}

// Return false if there is collision, true if none was found
bool Screen::write(Pixel dotFill, int y, int x){
	if(collisionWithBorders(y,x) || dotOutsideScreen(y,x) || collision(y,x,dotFill)) return false;
	// Write dot
	strcpy(dot[y+BORDERS_WIDTH][x+BORDERS_WIDTH], dotFill);
	return true;
}
void Screen::print(){
	runBash("sleep " + to_string(FRAME_PERIOD) + " && clear");
	for(int y = 0; y < B_MAX_Y; y++){
		for(int x = 0; x < B_MAX_X; x++) cout << dot[y][x];
		cout << '\n';
	}
}


Pallet::Pallet(int yL){
	if(yL < 1) error("Paddle size must be >= 1");
	yLen = yL;
}
Pallet::Pallet(int yL, int y, int x){
	if(yL < 1) error("Paddle size must be >= 1");
	yLen = yL;
	posY = y, posX = x;
}
Pallet::Pallet(int yL, int y){
	if(yL < 1) error("Paddle size must be >= 1");
	yLen = yL;
	posY = y;
}

bool Pallet::draw(Screen &win){
	for(int y = posY; y < posY + (signed)yLen; y++) {
		if(!win.write(pixelFill, y, posX))
			return false; // Don't draw if there is collision
	}
	return true; // Draw otherwise
}
void Pallet::move(int y, int x, Screen &win){
	// Clean object's past position on the screen
	killGhost(win);

	// Write new's position
	posY += y, posX += x;
	if(!draw(win)){
		posY-=y, posX-=x;
		draw(win);
	}
}
