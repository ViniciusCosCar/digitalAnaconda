#include <iostream>
#include <cstdlib>

void prompt(std::string text);

int main(void){
	prompt("Morte aos humanos!!!");
}

void prompt(std::string text) {

	text = "cowsay " + text;
	std::system(text.c_str());
}
