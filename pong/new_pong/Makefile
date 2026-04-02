# Just use "make", "make clean" and "./main"
# Shorthand: "make && ./main && make clean"

# I needed it but didn't take the time to invest on learning Makefiles, so I just asked gemini
# I promise that I will study it (someday)

# Compiler and Flags
CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I. -I./game_library_of_destruction -I./game_library_of_destruction/game
LDFLAGS  := 

# Target Binary
TARGET   := main

# Directories
LIB_DIR  := game_library_of_destruction
GAME_DIR := $(LIB_DIR)/game

# Source Files
SRCS     := main.cpp \
            $(LIB_DIR)/input.cpp \
            $(LIB_DIR)/utils.cpp \
            $(GAME_DIR)/ball.cpp \
            $(GAME_DIR)/game_object.cpp \
            $(GAME_DIR)/pallet.cpp \
            $(GAME_DIR)/screen.cpp

# Object Files (maps .cpp to .o)
OBJS     := $(SRCS:.cpp=.o)

# Default Rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation Rule for .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJS) $(TARGET)

# Phony Targets
.PHONY: all clea
