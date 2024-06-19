# Compiler
COMP = clang++

# Compiler flags
FLAGS = -std=c++11 -Wall -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL ../lib/libraylib.a -I ../../raylib/src


# Source file and executable name
SRCS = images.cpp
EXEC = images

# Build target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(SRCS)
	$(COMP) $(FLAGS) -o $(EXEC) $(SRCS)

# Clean target to remove the executable
clean:
	rm -f $(EXEC)
