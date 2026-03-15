///////// OBJECTIVES /////////
/* ⚠️⚠️⚠️TODO -> ULTIMATE IMPORTANCE: REVIEW CODE */
/* TODO: Add score, define limit on points, maybe let the user decide, count score, find winner */
/* TODO: Let player give theirselvs names, maybe whithing a 10 letters range. Probably much more difficult than it souds */
/* TODO: Add a memory file contaning the results of last matches, name of the winner, total ammount of points */
/* TODO: Add a menu, where user will decide between playing, exploring records or changing sprites, like changing the emoji 
 * 	 for the background, ball and etc */
/* TODO: Make program compatible with other operating systems, like Windows */
///////////////////////////////
#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <thread>
#include "headers/game_objects.cpp"
#include "headers/input.cpp"

using namespace std;

int main(void){
	try{
		Screen win;
		Pallet p1(4, 7);
		Pallet p2(4, 7, 49);
		Pallet ball(1, 9, 24); strcpy(ball.pixelFill,"⚽");
		p1.draw(win); p2.draw(win), ball.draw(win);
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
				ball.move(0,1, win);
				i=0;
			} i++;

			// Mappings
			switch (key) {
				case 'j': 
					p1.move(1,0,win);
					break;
				case 'k': 
					p1.move(-1,0,win);
					break;
				case 'a': 
					p2.move(1,0,win);
					break;
				case 's': 
					p2.move(-1,0,win);
					break;
			}
			win.print();
		} while(key != 'q');
		t.join(); // Assegure that thread will be finished

		return 0;
	} catch(exception& e){
		cerr << "\n❌ " << e.what() << "\n" << endl;
		return 1;
	} catch(...){
		cerr << "\n❌ I don't know what's wrong :P" << "\n" << endl;
		return 2;
	}
}
