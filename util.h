#pragma once
#include <raylib.h>
#define BOARDSIZE 8
#define SQUARESIZE 128
#define UIWIDTH 256
#include <iostream>
#include <random>

#include <iomanip>

float windowWidth = BOARDSIZE*SQUARESIZE + UIWIDTH;
float boardWH = BOARDSIZE*SQUARESIZE;
float windowCenter = windowWidth /2 ;
double currentTime = 0;
double lastUpdateTime = 0;

bool TimePassed(double interval) {
    double currentTime = GetTime();
    if(currentTime-lastUpdateTime>=interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int random_integer(int min, int max) {
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distrib(min, max); // Define the range
    return distrib(gen); // Generate the random number
}

void visualizeArray(int arr[8][8]) {
    // Print the array with some formatting
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // setw(4) ensures each number takes up 4 spaces for alignment
            std::cout << std::setw(4) << arr[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "*******" << std::endl;
}

void zeroIntArray(int (&ints)[BOARDSIZE][BOARDSIZE]) {
    for(int i = 0; i<BOARDSIZE; i++) {
        for(int j = 0; j<BOARDSIZE; j++) {
            ints[i][j] = 0;
        }
    }
}
void copyBoard(Square (&squares)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    for(int i = 0; i<BOARDSIZE; i++) {
        for(int j = 0; j<BOARDSIZE; j++) {
            squares[i][j].piece = bord[i][j].piece;
        }
    }
}

// t

struct xy {
    int x;
    int y;
};

struct Move{
    int type = 0; //1 is moveable, 2 is taking
    xy start;
    xy end;
    std::string piece;
    Move(int t, xy s, xy e, const std::string& p) : type(t), start(s), end(e), piece(p) {}
};
void zeroMoveArray(std::vector<Move> (&moves)[BOARDSIZE][BOARDSIZE]) {
    for(int i = 0; i<BOARDSIZE; i++) {
        for(int j = 0; j<BOARDSIZE; j++) {
            moves[i][j].clear();
        }
    }
}

// for ai moves
struct Element {
    int value;
    xy start;
    xy end;
    std::string piece;
};

// Custom comparison function to compare two Element objects based on their key
struct Compare {
    bool operator()(const Element& a, const Element& b) const {
        // Compare based on the key value
        return a.value < b.value; // Change to > for max heap
    }
};

class Timer {
public:
    Timer() : last_time(std::chrono::steady_clock::now()) {}

    bool hasTimePassed(double seconds) {
        // Get the current time
        auto current_time = std::chrono::steady_clock::now();

        // Calculate the duration since the last time the function was called
        std::chrono::duration<double> elapsed_seconds = current_time - last_time;

        // Check if the specified amount of time has passed
        if (elapsed_seconds.count() >= seconds) {
            // Update the last time to the current time
            last_time = current_time;
            return true;
        }

        return false;
    }
    void reset() {
        last_time = std::chrono::steady_clock::now();
    }

private:
    std::chrono::steady_clock::time_point last_time;
};
