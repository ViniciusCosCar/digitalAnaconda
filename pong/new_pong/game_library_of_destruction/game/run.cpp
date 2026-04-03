#include "game.h"
#include <thread>
#include <ctime>
#include <string>
#include "../input.h"

/* TODO: 
 * 	Add FRAME_PERIOD delay between player's code function calls.
 * 	Also, synchronous input? An important question to answer!
 * */
void Run::loop(void (*function_ptr)(char)){

	char stop_key = 'q';
	std::string commands;

	std::thread t(getInput, ref(commands), stop_key, true);

	char key;

	// long init;
	do{
		// init = time(NULL);
		if(!commands.empty()){ 
			key=commands[0];
			commands.erase(0, 1); // Eliminate command read from string
		}
		else key=' ';

		function_ptr(key);

		// while(time(NULL)-init < FRAME_PERIOD);
	}while(key != stop_key);
	t.join();
}
