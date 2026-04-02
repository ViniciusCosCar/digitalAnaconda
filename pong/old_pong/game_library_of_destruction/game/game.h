#ifndef GAME_H
#define GAME_H

#define PIXEL_SIZE 20

typedef char Sprite[PIXEL_SIZE];

namespace game_library_of_destruction{
enum Direction{
		NONE = 0,
		RIGHT,
		LEFT,
		TOP,
		BOTTOM,
		TOP_RIGHT,
		TOP_LEFT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT,
	};
enum Object {
		NO_ONE = 0,
		BORDER,
		PALLET,
		BALL,
		N_OBJECTS,
	};
typedef struct {
	Direction direction;
	Object target;
	} CollisionInfo;

///////// Dot /////////
class Dot{
public:
	unsigned int y, x;
	Sprite sprite;
	Dot();
	Dot(unsigned int, unsigned int, const Sprite);
};

///////// Screen /////////
class Screen {
	static constexpr unsigned MAX_Y = 20, MAX_X = 50, BORDERS_WIDTH = 1; // Screen size (For now, only works with BORDERS_WIDTH=0or1)
	const float FRAME_PERIOD = 0.01;
	struct cell {
		Sprite sprite;
		Object target = NO_ONE;
	};
	cell map[MAX_Y+2*BORDERS_WIDTH][MAX_X+2*BORDERS_WIDTH]; 	// Screen map
	Sprite bgSprite, bdSprite; 				// background and borders sprites 
public:
	void init();						// Initialize map array with default values
	Screen();
	Screen(const Sprite, const Sprite);
	CollisionInfo checkCollision(Dot, int y, int x);
	bool write(GameObject&);
	bool move(Dot, Object);
	void clear(Dot);					// Clean given dot in map
	void print();						// Print map to screen
};

class GameObject{
public:
///////// Pallet /////////
class Pallet{
	static constexpr unsigned SIZE = 5;
	Dot body[SIZE];
public:
	Pallet(unsigned int y, unsigned int x, const Sprite);
	bool write(Screen&);
	bool move(int y, Screen&);
};
///////// Ball /////////
class Ball{
	Dot body;
public:
	Ball(unsigned int y, unsigned int x, const Sprite);
	bool write(Screen&);
	// Move object on map
	bool move(int y, int x, Screen&);
};
};
};

#endif
















