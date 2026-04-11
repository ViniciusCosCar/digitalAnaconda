#ifndef GAME_H
#define GAME_H

#include <vector>
#define PIXEL_SIZE 20

// The actual image of each game object
typedef char Sprite[PIXEL_SIZE];

// enums
enum Horizontal {
	NONE_X,
	RIGHT,
	LEFT,
};
enum Vertical {
	NONE_Y,
	UP,
	DOWN,
};


enum Object {
	NO_TARGET,
	BORDER,
	PALLET,
	BALL,
	N_OBJECTS,
};

// Collision related structs
struct Direction{
	Vertical   y = NONE_Y;
	Horizontal x = NONE_X;
};
struct Target {
	Object y  =  NO_TARGET;
	Object x  =  NO_TARGET;
	Object yx =  NO_TARGET;
};
typedef struct {
	Direction 	direction;
	Target 		target; 
} CollisionInfo;

/////// Pos /////////
typedef struct {
	int y;
	int x;
} Pos;

////// GameObjects /////////
class Window;
#include "../utils.h"
class GameObject { // Abstract class
public:

	// Collision handling methods - User defined
	void (*handleCollision_def)(CollisionInfo&) = [](CollisionInfo&){}; // Pointer to function to be executed
	void  handleCollision(CollisionInfo& collision, Window&){ handleCollision_def(collision); };

	Pos velocity = {0,0};

	// Collision detection returning related information
	virtual CollisionInfo	getCollisionInfo(Window&, Pos offset={0,0}) 	= 0;

	// Called when Window finds no collision
	virtual void 		writeToWindow	(Window&)		= 0;
	virtual void 		clear		(Window&)		= 0;
	virtual void 		moveInWindow	(Window&, Pos offset)	= 0;
};

class Pallet : public GameObject {
public:
	Pallet			(Window&, Pos pos, const Sprite sprite="⬜");
	static int constexpr 	SIZE = 3;
	Pos 			body[SIZE];

	CollisionInfo	getCollisionInfo(Window&, Pos offset={0,0}) 	override;
	
	// Assuming no collision
	void 		writeToWindow	(Window&) 			override;
	void 		clear		(Window&)			override;
	void 		moveInWindow	(Window&, Pos offset)		override;
};

class Ball : public GameObject {
public:
	Ball				(Window&, Pos pos, const Sprite sprite="🟡");
	Pos body;

	CollisionInfo	getCollisionInfo(Window&, Pos offset={0,0})	override;
	
	// Assuming no collision
	void 		writeToWindow	(Window&)			override;
	void 		clear		(Window&)			override;
	void 		moveInWindow	(Window&, Pos offset)		override;
};

////// Window /////////
/* TODO: Update window only when detect movement 
 * TODO: Add a responsive design (probably one of the most hard/laborous TODOs )*/
class Window{
public:
	static constexpr int MAX_Y = 20, MAX_X = 50; 	// Window's size 
	static constexpr int BORDERS_WIDTH = 1; 	// For now, only works with BORDERS_WIDTH = 0 or 1

	// Map
	Sprite objectsSprites[N_OBJECTS]; // Sprites related to each game object
	Object map[MAX_Y+2*BORDERS_WIDTH][MAX_X+2*BORDERS_WIDTH]; // Actual window

	// "Constructor" related
	void 	init();
	Window	();
	Window	(const Sprite bgSprite, const Sprite bdSprite);

	// TODO: Deconstructor
	
	// "Write" related
	bool 	write	(GameObject& obj);
	bool 	move	(GameObject& obj, Pos offset);
	bool 	clear	(Pos);

	// Detect collisions in window
	CollisionInfo getCollisionInfo(Pos current_pos, Pos pos_offset={0,0});
	// Find direction of movement
	Direction findDirection(Pos);
	
	// Print map's sprites
	void print();
};

////// Game //////
class Run{
public:
	constexpr static float FRAME_PERIOD = 0.01;

	// Update frame respecting desired frame period
	void	loop(void (*function_ptr)(char));
};

#endif
