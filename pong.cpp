///////// OBJECTIVES /////////
/* TODO: Review code at least once a week */
/* TODO: I sincerally think that breaking this code in diferent parts would seriously enhance debugging and comphreensabitly. 
 * 	 Maybe put all the classes, constants and libraries somewhere else */
/* TODO: Add score, define limit of points, maybe let the user decide, count score, find winner */
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

using namespace std;

// 4 bytes maximum for emojis; + 1 for the null terminator
#define PIXEL_SIZE 5 // This value could be increased later on, but is sufficient for now 

/* TODO: Add Pixel definition size verification */
typedef char Pixel[PIXEL_SIZE];

const int MAX_Y = 20, MAX_X = 50; // Dimensions of the screen (without borders)
// Dimensions of borders
const unsigned int BORDERS_WIDTH = 0;// Somehow, when changing pixel's size, I fucked up with the borders. I will investigate that🕵️!
			             // So, for now, keep it's value as zero

// BORDERS_WIDTH can be seen as the coordinates of a translation vector of the screen, <BORDERS_WIDTH, BORDERS_WIDTH>
const int B_MAX_Y = MAX_Y + 2*BORDERS_WIDTH, B_MAX_X = MAX_X + 2*BORDERS_WIDTH; // Dimension of FULL screen

// Seconds between each frame
const float FRAME_PERIOD = 0.01; // Magic constant found empiraclly 

void error(string message) { throw runtime_error(message); }
void runBash(string command){ system(command.c_str()); }

// The origin is the top left of the screen,
// where Oy points down and Ox points to the right
// The coordinates are given in form <y, x>
/* TODO: There is no need for a vector in this scenario, could have used an array instead */
class Screen{
public:
	// Write borders on vector when object is created
	Screen();
	bool write(Pixel dotFill, int y = 0, int x = 0); // Write dot in vector
	Pixel backgroundFill = "⬛";
	Pixel borderFill = "⬜";
	void print(); // Print screen each frame
private:
	/* TODO: Change vector to an array */
	// vector<string> dot{B_MAX_Y, string(B_MAX_X, backgroundFill)};
	Pixel dot[B_MAX_Y][B_MAX_X];
	
	bool dotOutsideScreen(int y, int x) const { return  !(y >= 0 && y < MAX_Y && x >= 0 && x < MAX_X); } // "true" for dot outside
	bool collisionWithBorders(int y, int x) const { return y == -1 || y == MAX_Y || x == -1 || x == MAX_X; } // "true" for collision on point
	bool collision(int y, int x, Pixel dotFill) const { // "true" for colliision on point
		// Apply translation
		y += BORDERS_WIDTH; x += BORDERS_WIDTH;

		// Allows objects to be erased before writing new position
		if(!strcmp(dotFill, backgroundFill))
			return false;

		// Collision with objects; If they are not equal
		return strcmp(dot[y][x], backgroundFill);
	}
};

class Pallet{
private:
	unsigned int yLen;
	// Clean past object's position on screen
	void killGhost(Screen &win){
		Pixel aux;
		memcpy(aux,pixelFill, sizeof(Pixel));
		memcpy(pixelFill, win.backgroundFill, sizeof(Pixel));
		draw(win);
		memcpy(pixelFill, aux, sizeof(Pixel));
	}
public:
	int posY = 0, posX = 0; // Top of paddle
	Pixel pixelFill = "❌";

	// Possible initializations
	Pallet(int yL);
	Pallet(int yL, int y);
	Pallet(int yL, int y, int x);

	bool draw(Screen &win); // Write object to screen
	void move(int y, int x, Screen &win); // Write ball's new position on screen
};


/* TODO: Organize following code and enhance cross OS: The "sleep" command in Screen::Print is an issue */
#if defined _WIN32 || defined _WIN64
#include <conio.h>
// Function to run in a thread: Avoid freezing the game when waiting for input
void getInput(string &s){
	char c;
	for(; (c=_getch()) && c!='q'; s+=c);
	s+=c;
};
#else 
// For now, consider any other OS as Linux
#include <termios.h>
#include <unistd.h>

// Get user input without pressing <enter>
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
// Function to run in a thread: Avoid freezing the game when waiting for input
void getInput(string &s){
	char c;
	for(; (c=raw_getch(0)) && c!='q'; s+=c);
	s+=c;
};
#endif
int main(void){
	try{

		Screen win;
		Pallet p1(4, 7);
		Pallet p2(4, 7, MAX_X-1);
		Pallet ball(1, 9, 24); memcpy(ball.pixelFill,"👹",sizeof(Pixel));
		p1.draw(win); p2.draw(win), ball.draw(win);
		win.print();

		string commands;
		thread t(getInput, ref(commands));


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


bool Screen::write(Pixel dotFill, int y, int x){
	if(collisionWithBorders(y,x) || dotOutsideScreen(y,x) || collision(y,x,dotFill))
		return false;

	// Write dot
	memcpy(dot[y+BORDERS_WIDTH][x+BORDERS_WIDTH], dotFill, sizeof(Pixel));
	return true;
}


Screen::Screen(){ 
	/* TODO: It only works if BORDERS_WIDTH == 1. Fix so that
		 any dimension of borders can be implemented */
	for(int x = 0; x < B_MAX_X; x++){
		memcpy(dot[0][x], borderFill, sizeof(Pixel));
		memcpy(dot[B_MAX_Y-1][x], borderFill, sizeof(Pixel));
	}
	for(int y = 0; y < B_MAX_Y; y++){ 
		memcpy(dot[y][0], borderFill, sizeof(Pixel));
		memcpy(dot[y][B_MAX_X-1], borderFill, sizeof(Pixel));
	}
	// Initialize
	for(int y=BORDERS_WIDTH; y<MAX_Y; y++)
		for(int x=BORDERS_WIDTH; x<MAX_X; x++)
			memcpy(dot[y][x], backgroundFill, sizeof(Pixel));
}

void Screen::print(){
	runBash("sleep " + to_string(FRAME_PERIOD) + " && clear");
	for(int y = 0; y < B_MAX_Y; y++){
		for(int x = 0; x < B_MAX_X; x++) cout << dot[y][x];
		cout << '\n';
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

bool Pallet::draw(Screen &win){
	for(int y = posY; y < posY + (signed)yLen; y++) {
		if(!win.write(pixelFill, y, posX))
			return false; // Don't draw if there is collision
	}
	return true; // Draw otherwise
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
