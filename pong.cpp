#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

const int MAX_Y = 20, MAX_X = 50; // Dimensions of the screen (without borders)
const unsigned int BORDERS_WIDTH = 1; // Dimensions of borders

// BORDERS_WIDTH can be seen as the coordinates of a translation vector of the screen, <BORDERS_WIDTH, BORDERS_WIDTH>
const int B_MAX_Y = MAX_Y + 2*BORDERS_WIDTH, B_MAX_X = MAX_X + 2*BORDERS_WIDTH; // Dimension of FULL screen
char backgroundFill = ' ';

const float FRAME_PERIOD = 0; // Seconds between each frame

void error(std::string message) { throw std::runtime_error(message); }
void runBash(std::string command){ std::system(command.c_str()); }

// The origin is the top left of the screen,
// where Oy points down and Ox points to the right
// The coordinates are given in form <y, x>
class Screen{
private:
	std::vector<std::string> dot{B_MAX_Y, std::string(B_MAX_X, backgroundFill)};
	char borderFill = '@';

	bool dotOutsideScreen(int y, int x) const { return  !(y >= 0 && y < MAX_Y && x >= 0 && x < MAX_X); } // "true" for dot outside
	bool collisionWithBorders(int y, int x) const { return y == -1 || y == MAX_Y || x == -1 || x == MAX_X; } // "true" for collision on point
	bool collision(int y, int x, char dotFill) const { // "true" for colliision on point
		// Apply translation
		y += BORDERS_WIDTH; x += BORDERS_WIDTH;

		// Allows objects to be erased before writing new position
		if(dotFill == backgroundFill)
			return false;

		// Collision with objects
		return dot[y][x] != backgroundFill;
	}

public:
	// Write borders on vector when object is created
	Screen();
	bool write(int y = 0, int x = 0, char dotFill = 'X'); // Write dot in vector
	void print(); // Print screen each frame
};

class Pallet{
private:
	unsigned int yLen;
	// Clean past object's position on screen
	void killGhost(Screen &win){
		char aux = charFill;
		charFill = backgroundFill;
		draw(win);
		charFill = aux;
	}
public:
	int posY = 0, posX = 0; // Top of paddle
	char charFill = '|';

	// Possible initializations
	Pallet(int yL);
	Pallet(int yL, int y);
	Pallet(int yL, int y, int x);

	bool draw(Screen &win); // Write object to screen
	void move(int y, int x, Screen &win); // Write ball's new position on screen
};

class Ball{
private:
	void killGhost(Screen &win){
		char aux = charFill;
		charFill = backgroundFill;
		draw(win);
		charFill = aux;
	}
	unsigned i = 0;


public:
	//TODO: eliminate magic numbers
	int posY = 9, posX = 24;
	char charFill = 'o';

	// possible initializations
	Ball(){};
	Ball(int y):posY{y}{};
	Ball(int y,int x):posY{y},posX{x}{};

	// functions
	bool draw(Screen &win){ return win.write(posY,posX); }; // Write ball to screen
	void move(int y, int x, Screen &win); // Write ball's new position
};

// TODO: Add something so that user doesn't need to press <Enter> to give input
void getInput(std::string &s){
	char c;
	for(; std::cin>>c && c!='q'; std::cout<<"> ", s+=c);
	s+=c;
};

int main(void){
	try{

		Screen win;
		Pallet p1(4, 7);
		Pallet p2(4, 7, MAX_X-1);
		Pallet ball(1, 9, 24); ball.charFill='o';
		p1.draw(win); p2.draw(win), ball.draw(win);
		win.print();

		std::string commands;
		std::thread t(getInput, ref(commands));


		char key;
		int i = 0;
		do{ 
			if(!commands.empty()){ 
				key=commands[0];
				commands.erase(0, 1); 
			}
			else key=' ';

			// TODO: Implement random ball's initial velocity direction, except for a perperdicular one
			// TODO: WHAT THE HELL IS THIS!!! Please, implement something a bit more sophisticated
			// for controlling velocity
			if(i==2e1) {
				ball.move(0,1, win);
				i=0;
			}


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
			i++;
		} while(key != 'q');
		t.join(); // does it really makes sense to use it in this case?

		return 0;
	} catch(std::exception& e){
		std::cerr << "\n❌ " << e.what() << "\n" << std::endl;
		return 1;
	} catch(...){
		std::cerr << "\n❌ I don't know what's wrong :P" << "\n" << std::endl;
		return 2;
	}
}


bool Screen::write(int y, int x, char dotFill){
	if(collisionWithBorders(y,x)){ 
		return false;
	}

	if(dotOutsideScreen(y,x)){
		return false;
	}

	if(collision(y,x, dotFill)){
		return false;
	}

	// Write dot
	dot[y+BORDERS_WIDTH][x+BORDERS_WIDTH] = dotFill;
	return true;
}


bool Pallet::draw(Screen &win){
	for(int y = posY; y < posY + (signed)yLen; y++) {
		if(!win.write(y, posX, charFill))
			return false;
	}
	return true;
}

Screen::Screen(){ 
	// TODO: It only works if BORDERS_WIDTH == 1. Fix so that
	// any dimension of borders can be implemented
	for(int x = 0; x < B_MAX_X; x++){
		dot[0][x] = borderFill;
		dot[B_MAX_Y - 1][x] = borderFill; 
	}
	for(int y = 0; y < B_MAX_Y; y++){ 
		dot[y][0] = borderFill;
		dot[y][B_MAX_X - 1] = borderFill;
	}
}

void Screen::print(){
	runBash("sleep " + std::to_string(FRAME_PERIOD) + " && clear");
	for(int y = 0; y < B_MAX_Y; y++){
		for(int x = 0; x < B_MAX_X; x++) std::cout << dot[y][x];
		std::cout << '\n';
	}
}


void Ball::move(int y, int x, Screen &win){
	// Clean Ball's last position
	killGhost(win);

	// Write new's position
	posY += y, posX += x;
	if(!draw(win)){
		posY-=y, posX-=x;
		draw(win);
	}
}

void Pallet::move(int y, int x, Screen &win){
	// Clean object's past position on the screen
	killGhost(win);

	// Write new's position
	posY += y, posX += x;
	if(!draw(win)){
		posY-=y, posX-=x;
		draw(win);
	}
}

Pallet::Pallet(int yL){
	if(yL < 1) error("Paddle size must be >= 1");
	yLen = yL;
}
Pallet::Pallet(int yL, int y, int x){
	if(yL < 1) error("Paddle size must be >= 1");
	yLen = yL;
	posY = y, posX = x;
}
Pallet::Pallet(int yL, int y){
	if(yL < 1) error("Paddle size must be >= 1");
	yLen = yL;
	posY = y;
}
