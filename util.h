#pragma once
#include <raylib.h>
#define BOARDSIZE 8
#define SQUARESIZE 128
#define UIWIDTH 256

#include <iomanip>

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


float windowWidth = BOARDSIZE*SQUARESIZE + UIWIDTH;
float boardWH = BOARDSIZE*SQUARESIZE;
float windowCenter = windowWidth /2 ;