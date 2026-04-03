#ifndef GAME_H
#define GAME_H

#include <vector>
#define PIXEL_SIZE 20

typedef char Sprite[PIXEL_SIZE];

enum Direction {
		NONE,
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
		NO_ONE,
		BORDER,
		PALLET,
		BALL,
		N_OBJECTS,
				};
typedef struct {
	Direction	direction;
	Object 		target; 
} CollisionInfo;

/////// Pos /////////
typedef struct {
	int y;
	int x;
} Pos;

////// GameObjects /////////
class Screen;
class GameObject {
public:
	Pos velocity = {0,0};

	virtual CollisionInfo	getCollisionInfo(Screen&, Pos offset={0,0}) 	= 0;
	/* Not so sure about defining update and handleCollion as virtuals.
	 * It will be the base definition of the objects, but I if want to
	 * define a different behavior, then I have to tell that I will be
	 * overriding it with the 'override' keyword. But what if I don't
	 * have any function to override? It doesn't handle the case where
	 * user might or might not define it! Maybe if the function was a variable? */
	virtual void 		handleCollision	(CollisionInfo&, Screen&)	{}; // TODO: Modify
	// Assuming no collision
	virtual void 		writeToScreen	(Screen&)		= 0;
	virtual void 		clear		(Screen&)		= 0;
	virtual void 		moveInScreen	(Screen&, Pos offset)	= 0;
};
class Pallet : public GameObject {
	static int constexpr 	SIZE = 3;
public:
	Pallet			(Screen&, Pos pos, const Sprite sprite="⬜");
	Pos 			body[SIZE];

	CollisionInfo	getCollisionInfo(Screen&, Pos offset={0,0}) 	override;
	// Assuming no collision
	void 		writeToScreen	(Screen&) 			override;
	void 		clear		(Screen&)			override;
	void 		moveInScreen	(Screen&, Pos offset)		override;
	// void 		handleCollision	(CollisionInfo&, Screen&)	override;
};
class Ball : public GameObject {
public:
	Ball				(Screen&, Pos pos, const Sprite sprite="🟡");
	Pos body;

	CollisionInfo	getCollisionInfo(Screen&, Pos offset={0,0})	override;
	// Assuming no collision
	void 		writeToScreen	(Screen&)			override;
	void 		clear		(Screen&)			override;
	void 		moveInScreen	(Screen&, Pos offset)		override;
	// void 		handleCollision	(CollisionInfo&, Screen&)	override;
};

////// Screen /////////
/* TODO: Update window only when detect movement */
class Screen{
public: 					     // ( For now, only works with BORDERS_WIDTH = 0 or 1 )
	static constexpr int 	MAX_Y = 20, MAX_X = 50, BORDERS_WIDTH = 1;// Screen size 

	// Contains the sprite for each Object. NO_ONE and BORDER are defined at Screen's initialization,
	// but others are defined only when their respective class is writen
	Sprite 	objectsSprites	[N_OBJECTS];
	Object 	map		[MAX_Y+2*BORDERS_WIDTH][MAX_X+2*BORDERS_WIDTH];

	// Fill map
	void 	init();
	Screen	();
	Screen	(const Sprite bgSprite, const Sprite bdSprite);

	Direction 	movementDirection	(Pos);				// Get movement direction
	bool 		write			(GameObject& obj);		// "true" if successful to write game object
	bool 		move			(GameObject& obj, Pos offset);	// "true" if successful to move game object
	bool 		clear			(Pos);				// Clear past position. Set it to NO_ONE
	void 		print			();				// Print map according to objectsSprite vector
	CollisionInfo 	getCollisionInfo(Pos current_pos, Pos pos_offset={0,0});// Detect and provides information regarding collision
};

////// Game //////
class Run{
public:
	const	float 	FRAME_PERIOD = 0.01;

	void	loop(void (*function_ptr)(char));
};

#endif
