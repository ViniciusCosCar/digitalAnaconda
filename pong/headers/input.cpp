#include <string>
using namespace std;

#ifndef INPUT_H
#define INPUT_H

// Check out more macros on https://sourceforge.net/p/predef/wiki/OperatingSystems/
#if defined _WIN32 || defined _WIN64
#include <conio.h>
// Get character input without pressing <Enter>
void getInput(string &s){
	char c;
	for(; (c=_getch()) && c!='q'; s+=c);
	s+=c;
};
#else 
// For now, consider any other OS as Linux
#include <termios.h>
#include <unistd.h>

// Get character input without pressing <Enter>
int raw_getch(int fd) {
	struct termios oldt, newt;
	tcgetattr(fd, &oldt);
	newt = oldt;

	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd, TCSANOW, &newt);

	char c;
	read(fd, &c, 1);

	tcsetattr(fd, TCSANOW, &oldt);
	return c;
}
/* Read input without waiting for <Enter>. Read each character and append to given string, until reachs "stop_key".
 * Append "stop_key" if "include_stop_key" is true, default option */
void getInput(string &s, char stop_key, bool include_stop_key=true){
	char c;
	for(; (c=raw_getch(0)) && c!='q'; s+=c);
	if(include_stop_key) s+=c;
};
#endif
#endif
