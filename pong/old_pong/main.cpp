#include "game_library_of_destruction/game/game.h"
#include "game_library_of_destruction/input.h"
#include <thread>
#include <iostream>
using namespace std;
using namespace game_library_of_destruction;


int main(void){
	try{
		Screen win;
		GameObject::Pallet p1(9,1,"⬜"), p2(9, 48, "⬜");
		GameObject::Ball ball(9, 24, "🟡");
		p1.write(win), p2.write(win), ball.write(win);
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
				ball.move(1, 1, win);
				i=0;
			} i++;

			// Mappings
			switch (key) {
				case 'j': 
					p1.move(1,win);
					break;
				case 'k': 
					p1.move(-1,win);
					break;
				case 'a': 
					p2.move(1,win);
					break;
				case 's': 
					p2.move(-1, win);
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

