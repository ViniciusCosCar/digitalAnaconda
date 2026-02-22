#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

const int MAX_Y = 20, MAX_X = 50; // Dimensions of the screen (without borders)
const int BORDERS_WIDTH = 1; // Dimensions of borders

// BORDERS_WIDTH can be seen as the coordinates of a translation vector of the screen, <BORDERS_WIDTH, BORDERS_WIDTH>
const int B_MAX_Y = MAX_Y + 2*BORDERS_WIDTH, B_MAX_X = MAX_X + 2*BORDERS_WIDTH; // Dimension of FULL screen
char backgroundFill = ' ';

const float FRAME_PERIOD = 0; // Seconds between each frame

void error(std::string message) { throw std::runtime_error(message); }
void runBash(std::string command){ std::system(command.c_str()); }

class Screen{
private:
	// The origin is the top left of the screen,
	// where Oy points down and Ox points to the right
	// The coordinates are given in form <y, x>
	std::vector<std::string> dot{B_MAX_Y, std::string(B_MAX_X, backgroundFill)};

	char borderFill = '@';
	void writeBorders(){
		for(int x = 0; x < B_MAX_X; x++){
			dot[0][x] = borderFill;
			dot[B_MAX_Y - 1][x] = borderFill; 
		}
		for(int y = 0; y < B_MAX_Y; y++){ 
			dot[y][0] = borderFill;
			dot[y][B_MAX_X - 1] = borderFill;
		}
	}
	
	// Detect collision on a given point
	bool checkCollision(int y, int x, char dotFill){
		// Verify if dot is not in the screen
		if(!( y >= 0 && y < MAX_Y && x >= 0 && x < MAX_X )){
			// Collision with borders
			if(y == -1 || y == MAX_Y || x == -1 || x == MAX_X){
				// error("Collision with borders");
				return true;
			}
			error("Screen<obj>.write()\nDrawning object out of screen");
		}

		// Apply translation
		y += BORDERS_WIDTH; x += BORDERS_WIDTH;

		// Collision with objects
		if(dot[y][x] == backgroundFill || dotFill == backgroundFill)
			return false;
		return true;
	}

public:
	// Write borders on vector when object is created
	Screen(){ writeBorders(); }


	// Write a given dot in vector
	bool write(int y = 0, int x = 0, char dotFill = 'X'){
		if(!checkCollision(y, x, dotFill)){
			dot[y+BORDERS_WIDTH][x+BORDERS_WIDTH] = dotFill;
			return true;
		} 
		return false;
	}

	// Print the vector <y,x> each frame
	void update(){
		runBash("sleep " + std::to_string(FRAME_PERIOD) + " && clear");
		for(int y = 0; y < B_MAX_Y; y++){
			for(int x = 0; x < B_MAX_X; x++) std::cout << dot[y][x];
			std::cout << '\n';
		}
	}
};

// TODO: Maybe, for generalizing the class, Pallet should be a member of a higher level class, 
// maybe called <GameObjects>,
// containing all game objects we might add in the future, where <size> would be common private field
// and <y,x> a common public one. I didn't do it yet since I don't know how to do it properly. I just
// know that we would be able to access Pallet by making, for example, GameObjects::Pallet.
// I will study a bit and then try to implement
class Pallet{
private:
	// Paramethers are all ints and posY + height <= MAX_Y && posX <= MAX_X
	int yLen;

public:
	int posY = 0, posX = 0;
	char charFill = '|';

	// <posY, posX> indicates where to draw the top of the paddle
	Pallet(int l = 1){ 
		if(!(l > 0)) error("Pallet<object>: Size must be >= 1");
		yLen = l;
	}

	bool write(Screen &win){
		bool collision;
		for(int y = posY; y < posY + yLen; y++) {
			collision = !win.write(y, posX, charFill);
			if(collision) return false;
		}
		return true;
	}

	void move(int y, int x, Screen &win){
		// Clean past object's position on screen
		char aux = charFill;
		charFill = backgroundFill;
		write(win);

		// Write new's position
		charFill = aux;
		posY += y, posX += x;
		if(!write(win)){ 
			posY -= y, posX -= x; 
			write(win);
		}
		win.update();
	}
};


int main(void){
	try{
		Screen win;
		Pallet p1(4);
		Pallet p2(4);
		Pallet ball(1);
		ball.charFill = 'o';
		p2.posY = 0, p2.posX = MAX_X - 1;
		ball.posY = 9, ball.posX = 24;

		p1.write(win);
		p2.write(win);
		ball.write(win);
		win.update();


		char key;
		do{ 
			// TODO: Make user not have to press <Enter> to receive input. This freezes the game
			// that will wait for user input, which is stupid. Logic also must be added to handle
			// no propper input
			std::cout << "-> ";
			std::cin >> key; 

			// TODO: Implement random ball's initial velocity direction, except for a perperdicular one
			ball.move(0,1, win);

			// Mappings
			if(key == 'j') p1.move(1,0, win);
			if(key == 'k') p1.move(-1,0, win);
			if(key == 'a') p2.move(1,0, win);
			if(key == 's') p2.move(-1,0, win);
			win.update();
		} while(key != 'q');
		return 0;
	} catch(std::exception& e){
		std::cerr << "\n❌ " << e.what() << "\n" << std::endl;
		return 1;
	} catch(...){
		std::cerr << "\n❌ I don't know what's wrong :P" << "\n" << std::endl;
		return 2;
	}
}
