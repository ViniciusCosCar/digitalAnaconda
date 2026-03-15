#include <iostream>
using namespace std;

#ifndef INPUT_H
#define INPUT_H

int raw_getch(int fd);
void getInput(string &s, char stop_key, bool include_stop_key=true);

#endif
