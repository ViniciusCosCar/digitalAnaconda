#include <iostream>
#ifndef INPUT_H
#define INPUT_H

// get raw character
int raw_getch(int);
// get <Enter>-less input until stop_key is pressed
void getInput(std::string&, char, bool);

#endif
