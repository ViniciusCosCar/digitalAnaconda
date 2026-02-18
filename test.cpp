#include <iostream>
#include <vector>

const int MAX_Y = 30, MAX_X = 100;
void error(std::string message) { throw std::runtime_error(message); }

class Screen{
public:
	std::vector<std::string> dots{MAX_Y, std::string(MAX_X, ' ')};
	void print(){
		for(int y = 0; y < MAX_Y; y++){
			for(int x = 0; x < MAX_X; x++) std::cout << dots[y][x];
			std::cout << '\n';
		}
	}
};

class Pallet{
private:
	/* Paramethers are all ints and posY + height <= MAX_Y && posX <= MAX_X */
	int posY, posX, height; 
	char charFill = '|';

public:
	Pallet(int y, int x, int h){
		if(!(y >= 0 && h > 0 && y + h <= MAX_Y && x >= 0 && x <= MAX_X)) 
			error("class Pallet object initialization -> No valid input.\n"
				  "posY + height <= MAX_Y && posX <= MAX_X");
		posY = y, posX = x, height = h;
	}

	/* <posY, posX> indicates where to draw the top of the paddle */
	void draw(Screen *win){
		for(int y = 0; y < height; y++) win->dots[posY + y][posX] = charFill;
	}
};

int main(void){
	try{
		Screen win;
		Pallet p1(10, 50, 20); p1.draw(&win);
		win.print();
		return 0;
	} catch(std::exception& e){
		std::cerr << "error: " << e.what() << '\n';
		return 1;
	} catch(...){
		std::cerr << "Something went wrong, but I don't know what :P\n";
		return 2;
	}

}
