#include "game_library_of_destruction/game_library_of_destruction.h"
#include <cstdlib>
using namespace std;

void updateBall(Window& win, Ball& ball){
	win.move(ball, ball.velocity);
}

int main(void){

	// Setup
	static Window win("  ", "🔲");
	static Pallet p1(win, {8,  1}, "🟩");
	static Pallet p2(win, {8, 48}, "🟩");
	static Ball ball(win, {9, 24}, "🟡");

	// Ball's random initial velocity
	srand((unsigned)time(NULL));

	int dy = ((int) (2 * (float)rand()/RAND_MAX)) ? 1 : -1;
	int dx = ((int) (2 * (float)rand()/RAND_MAX)) ? 1 : -1;

	ball.velocity = {dy, dx};

	// Handle ball collision
	ball.handleCollision_def = [](CollisionInfo& collision){
		if(collision.direction.y == UP || collision.direction.y == DOWN)
			ball.velocity.y *= -1;
		if(collision.direction.x == RIGHT || collision.direction.x == LEFT)
			ball.velocity.x *= -1;
	};

	// Write game objects to screen
	win.write(p1);
	win.write(p2);
	win.write(ball);

	// Game loop
	Run run;
	run.loop([](char key){

		updateBall(win, ball);

		// Mappings
		switch (key) {
			case 'a': 
				win.move(p1, {1,0});
				break;
			case 's': 
				win.move(p1, {-1,0});
				break;
			case 'A': 
				win.move(p2, {1,0});
				break;
			case 'B': 
				win.move(p2, {-1,0});
				break;
		}

		// Print window
		win.print();
	});
}

