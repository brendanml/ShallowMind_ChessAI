# Compiler
COMP = g++

PKGS = raylib

# Compiler flags
FLAGS = -std=c++11 -Wall $(shell pkg-config --libs $(PKGS)) $(shell pkg-config --cflags $(PKGS))

# Header files
HDRS = board.h square.h player.h util.h command.h input.h ui.h

# Source file and executable name
SRCS = main.cpp
EXEC = game

# Build target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(SRCS) $(HDRS)
	$(COMP) $(FLAGS) -o $(EXEC) $(SRCS)

# Clean target to remove the executable
clean:
	rm -f $(EXEC)
