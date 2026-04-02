#include "utils.h"
#include <stdexcept>

void error(std::string message){ throw std::runtime_error(message); }
void runBash(std::string command){ system(command.c_str()); }
