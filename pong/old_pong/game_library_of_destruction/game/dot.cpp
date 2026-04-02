#include "game_objects.h"
#include "../utils.h"
#include <cstring>

///////// Dot  /////////
// Default initialization
Game_objects::Dot::Dot() : y(0), x(0) {}
Game_objects::Dot::Dot(unsigned int y, unsigned int x, const Sprite sprite) : y(y), x(x) {
	if(strlen(sprite)>PIXEL_SIZE) error("Sprite size overflow when creating dot");
	strcpy(this->sprite, sprite); 
}
