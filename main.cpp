#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
 
void error(std::string message){ throw std::runtime_error(message); }
void runBash(std::string command){ std::system(command.c_str()); }

const int MAX_X = 20, MAX_Y = 20;

const int FPS = 1; /* default frame-rate */
void clearFrame(const int fps = FPS){
	runBash("sleep " + std::to_string((float)1/fps) + " && clear");
}

/* vector containing all suported cowsay's animals/creatures */
const std::vector<std::string> ANIMALS = [](){
		/* Create "cowsay.txt" file */
		std::string fName = ".cowsay.txt";
		runBash("touch " + fName);

		/* Create pointer to file */
		FILE *fptr = std::fopen(fName.c_str(), "r"); 
		if (fptr == NULL) error("Couldn't open " + fName + ".txt");

		/* Create vector 'animals' containing each type of character allowed in 'cowsay' */
		runBash("cowsay -l > " + fName);
		std::vector<std::string> animals; std::string animal; char c; bool firstLine = true;
		while((c = fgetc(fptr)) != EOF){
			if (!firstLine){ 
				if (c == ' ' || c == '\n'){ 
					animals.push_back(animal); 
					animal = ""; 
				} else animal += c;
			} 
			else if (c == '\n') firstLine = false;
		}

		/* Terminate pointer and file */
		fclose(fptr);
		runBash("rm -f " + fName);

		/* 'cow' isn't a 'cowsay -f <argument>' */
		animals.push_back("cow");
		return animals;
}();

class Creature{
private: 
	std::string type;
public: 
	Creature(std::string s){
		/* 'type' must belong to ANIMALS */
		for(std::string animal : ANIMALS){
			if(s == animal) {
				type = s;
				return; 
		}} 
		error("'" + s + "' isn't a valid 'type'!\nNot a 'cowsay -f <paramether>'\nMake 'cowsay -l' to check all possible characters");
	}

	void says(const std::string message, const int fps = FPS){
		/*  'cow' isn't a valid <paramether> for cowsay */
		if(type == "cow")  runBash("cowsay " + message);
		else runBash("cowsay -f " + type + " " + message);
		clearFrame(fps);
	}
};

int main(){
	try{ /* Program */
		Creature Molly("cow"), Bob("dragon");

		/* Greetings */
		Molly.says("Good morning!", 0.1);
		Bob.says("I will kill you!", 0.1);

		/* Death scene */
		runBash("cowsay -f dragon-and-cow DIE DIE DIE!");

	} catch(std::exception& e){ /* Catch runtime or out of range error */
		std::cerr << "error: " << e.what() << '\n';
		return 1;
	} catch(...){ /* Catch anything else (hardware errors not included, eg. divisions per zero )*/
		std::cerr << "error: I don't know :P\n";
		return 2;
	}
}
