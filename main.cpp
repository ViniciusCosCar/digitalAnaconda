#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
 
void error(std::string message){ throw std::runtime_error(message); }
void executeBash(std::string command){ std::system(command.c_str()); }
const int MAX_X = 20, MAX_Y = 20;

const int FPS = 1; /* default frame-rate */
void clearFrame(const int fps = FPS){
	executeBash("sleep " + std::to_string((float)1/fps) + " && clear");
}

/* vector containing all suported cowsay's animals/creatures */
const std::vector<std::string> ANIMALS = [](){
		/* Create "cowsay.txt" file */
		std::string fName = ".cowsay.txt";
		executeBash("touch " + fName);

		/* Create pointer to file */
		FILE *fptr = std::fopen(fName.c_str(), "r"); if (fptr == NULL) error("Couldn't open " + fName + ".txt");

		/* Create vector 'animals' containing each type of character allowed in 'cowsay' */
		executeBash("cowsay -l > " + fName);
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
		executeBash("rm -f " + fName);

		/* 'cow' isn't a 'cowsay -f <argument>' */
		animals.push_back("cow");
		return animals;
}();

class Creature{
public: 
	std::string type;
	void says(const std::string message, const int fps = FPS){
		/*  'cow' isn't a valid <paramether> for cowsay */
		if(type == "cow")  executeBash("cowsay " + message);
		else executeBash("cowsay -f " + type + " " + message);

		clearFrame(fps);
	}
};

Creature createCreature(std::string type){
	/* 'type' must belong to ANIMALS */
	for(std::string animal : ANIMALS)
	if(type == animal) { 
		Creature someone; someone.type = type;
		return someone;
	};
	error("'" + type + "' isn't a valid 'type'!\nNot a 'cowsay -f <paramether>'");
}

class Paddle{
	int h = 5;
	const int wall_space = 2, celling_space = 1;
	char particle;
	void draw(){
	}
};

int main(){
	try{
		/* Program */
		Creature Molly = createCreature("cow");
		Creature Bob = createCreature("dragon");
		
		/* SCENE: Molly and Bob greet */
		Molly.says("Good morning");
		Bob.says("I will kill you!");

		/* SCENE: Bob kills Molly */
		Bob.type = "dragon-and-cow";
		Bob.says("DIE!");
		Bob.type = "dragon";

	} catch(std::exception& e){ /* Catch runtime or out of range error */
		std::cerr << "error: " << e.what() << '\n';
		return 1;
	} catch(...){ /* Catch anything else (hardware errors not included, eg. divisions per zero )*/
		std::cerr << "error: I don't know :P\n";
		return 2;
	}
}
