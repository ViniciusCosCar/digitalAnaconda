#include <cstring>

#ifndef PONG_OBJECTS_H
#define PONG_OBJECTS_H

#define PIXEL_SIZE 10 // TODO: Please, make some research and find a good value for this constant

typedef char Pixel[PIXEL_SIZE];

class Screen{
public:
	Pixel backgroundFill = "⬜", borderFill="⬛";
	
	// Initialization write borders and background for array
	Screen();
	Screen(const Pixel bgFill, const Pixel bdFill); // Implicit conversion from "char const *" was removed in C++11

	void print(); 					 // "cout" the array
	bool write(Pixel dotFill, int y = 0, int x = 0); // Write dot in array
	
private:
	// Dimensions of the screen (without borders) 	 There is some bug with the borders, so it's 0 for now. I will take a look later
	static constexpr unsigned MAX_Y = 20, MAX_X = 50, BORDERS_WIDTH = 0;
	static constexpr unsigned B_MAX_Y = MAX_Y+2*BORDERS_WIDTH, B_MAX_X = MAX_X+2*BORDERS_WIDTH; // Dimensions of screen (with borders)
	const float FRAME_PERIOD = 0.01; // Seconds between each frame
	// Map
	Pixel dot[B_MAX_Y][B_MAX_X];

	bool dotOutsideScreen(int y, int x) const { return  !(y >= 0 && y < MAX_Y && x >= 0 && x < MAX_X); } // "true" for dot outside
	bool collisionWithBorders(int y, int x) const { return y == -1 || y == MAX_Y || x == -1 || x == MAX_X; } // "true" for collision
	bool collision(int y, int x, Pixel dotFill) const { // "true" for colliision on point
		y += BORDERS_WIDTH; x += BORDERS_WIDTH; // Apply translation
		if(!strcmp(dotFill, backgroundFill)) return false; // Allow writing for cleaning map
		return strcmp(dot[y][x], backgroundFill); // Collision with objects; If they are not equal
	}
};
class Pallet{
public:
	int posY = 0, posX = 0; // Top of paddle
	Pixel pixelFill = "⬛";

	// Possible initializations
	Pallet(int yL);
	Pallet(int yL, int y);
	Pallet(int yL, int y, int x);

	bool draw(Screen &win); // Write object to screen
	void move(int y, int x, Screen &win); // Write ball's new position on screen
private:
	unsigned int yLen;
	// Clean past object's position on screen
	void killGhost(Screen &win){
		Pixel aux;
		strcpy(aux,pixelFill);
		strcpy(pixelFill, win.backgroundFill);
		draw(win);
		strcpy(pixelFill, aux);
	}
};
class Ball{};

#endif
