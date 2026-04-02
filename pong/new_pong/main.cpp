#include "game_library_of_destruction/game/game.h"
#include "game_library_of_destruction/game_library_of_destruction.h"
#include <thread>
#include <iostream>
using namespace std;

// void Ball::update(CollisionInfo&, Screen&){
// }

int main(void){
	try{
		Screen win;
		Pallet p1(win, {8,  1}, "⬜");
		Pallet p2(win, {8, 48}, "⬜");
		Ball ball(win, {9, 24}, "🟡");
		ball.velocity = {0,1};

		win.write(p1), win.write(p2), win.write(ball);
		win.print();

		char stop_key = 'q';
		string commands;
		thread t(getInput, ref(commands), stop_key, true);

		char key;
		int i = 0;
		do{ 
			if(!commands.empty()){ 
				key=commands[0];
				commands.erase(0, 1); // Eliminate command read from string
			}
			else key=' ';

			/* TODO: Implement random ball's initial velocity direction, except for a perperdicular one */
			/* TODO: WHAT THE HELL IS THIS!!! Please, implement something a bit more sophisticated	
				 for controlling ball's velocity */
			if(i==10) {
				win.move(ball, ball.velocity);
				i=0;
			} i++;

			// Mappings
			switch (key) {
				case 'j': 
					win.move(p1, {1,0});
					break;
				case 'k': 
					win.move(p1, {-1,0});
					break;
				case 'a': 
					win.move(p2, {1,0});
					break;
				case 's': 
					win.move(p2, {-1,0});
					break;
			}
			win.print();
		} while(key != 'q');
		t.join(); // Assegure that thread will be finished

		return 0;

	} catch(exception& e){
		cerr << "\n❌ " << e.what() << endl;
		return 1;
	} catch(...){
		cerr << "\n❌ I don't know what's wrong :P" << "\n" << endl;
		return 2;
	}
}

