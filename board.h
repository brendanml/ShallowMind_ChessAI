#pragma once
#include "util.h"
#include "square.h"
#include "raylib.h"
#include <vector>
#include <cmath>
class Board {
    Image spritesheet = LoadImage("./pieces3.png");
    Texture2D sprites = LoadTextureFromImage(spritesheet);
    public:
        Square board[BOARDSIZE][BOARDSIZE];
        int highlights[BOARDSIZE][BOARDSIZE];
        int opposingMoves[BOARDSIZE][BOARDSIZE];
        std::vector<int> wkingPosition;
        std::vector<int> bkingPosition;
        Board(){
            resetBoard();
        }
        void draw();
        void resetBoard();
        void clearBoard();
        void clearHighlights();
        void initPawns();
        void initRooks();
        void initKnights();
        void initBishops();
        void initKings();
        void initQueens();
        void computeScore(std::string nextPieceName, int mod);
        void move(std::vector<int> prev, std::vector<int> next, std::string nextPieceName, std::string prevPieceName);
        void undo(std::vector<int> prev, std::vector<int> next, std::string nextPieceName, std::string prevPieceName);
        void highlightTile(std::vector<int> selected, bool removeHighlight);
        // high is an arbitrary int grid that determines potential moves -- used for many things like check, bots, players moves, etc
        void determineMoves(std::vector<int> selected, std::string piece, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determineRook(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determineBishop(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determineQueen(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determinePawn(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determineKnight(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void knightHelper(char selectedCol, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE], int nextX, int nextY);
        void determineKing(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        std::vector<int> findKing(char color, Square (&bord)[BOARDSIZE][BOARDSIZE]);
        // check if piece is takeable - for king
        void kingMoveable(int nextX, int nextY, char selectedCol, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void checkMoves(); //for checking if king is moving into check/is in check/bots potential moves
        Piece getPiece(std::vector<int> coordinates);
        float whiteScore = 40;
        float blackScore = 40;
        std::string frontColor = "white";

        // used to calculate winning bar, starts at 1 instead of 0.5 because we're amplifying it with exponents (how many pieces I have vs how many they)^2
        float whiteRatio = 1;
        float blackRatio = 1;
};

void Board::resetBoard() {
    clearBoard();
    clearHighlights();
    initPawns();
    initRooks();
    initKnights();
    initBishops();
    initKings();
    initQueens();
}

void Board::clearHighlights() {
    for(int i = 0; i < BOARDSIZE; i++) {
        for(int j = 0; j < BOARDSIZE; j ++) {
            highlights[i][j] = 0;
        }
    }
}


void Board::clearBoard() {
    for(int i = 0; i < BOARDSIZE; i++) {
        for(int j = 0; j < BOARDSIZE; j ++) {
            Square newSquare = Square((i + j) % 2 == 0 ? RAYWHITE : GRAY);
            board[i][j] = newSquare;
        }
    }
}

void Board::determineMoves(std::vector<int> selected, std::string piece, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
//
    std::cout << "were determining a: " << piece << std::endl;
    if(piece == "rook") determineRook(selected, high, bord);
    if(piece == "pawn") determinePawn(selected, high, bord);
    if(piece == "bishop") determineBishop(selected, high, bord);
    if(piece == "queen") determineQueen(selected, high, bord);
    if(piece == "knight") determineKnight(selected, high, bord);
    if(piece == "king") determineKing(selected, high, bord);
}

void Board::determineKing(std::vector<int> sel, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
// top and to right
    char selectedCol = getPiece(sel).name[0];
    // majority of cases -> highlight all surrounding pieces

    kingMoveable(sel[0] - 1, sel[1] - 1, selectedCol, high, bord);
    kingMoveable(sel[0], sel[1] - 1, selectedCol, high, bord);
    kingMoveable(sel[0] + 1, sel[1] - 1, selectedCol, high, bord);
    kingMoveable(sel[0] + 1, sel[1], selectedCol, high, bord);
    kingMoveable(sel[0] + 1, sel[1] + 1, selectedCol, high, bord);
    kingMoveable(sel[0], sel[1] + 1, selectedCol, high, bord);
    kingMoveable(sel[0] - 1, sel[1] + 1, selectedCol, high, bord);
    kingMoveable(sel[0] - 1, sel[1], selectedCol, high, bord);
}

void Board::kingMoveable(int currX, int currY, char selectedCol, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    // check x within bounds
    if(currX>= 0 && currX <= 7) {
        // check y within neg
        if(currY >=0 && currY <=7) {
            char curr = bord[currY][currX].piece[0];
            if((curr == 'b' && selectedCol == 'w') || (curr == 'w' && selectedCol == 'b')) high[currY][currX] = 2;
            if(curr == 'n') {
                high[currY][currX] = 1;
            }
        }
    }   
}

std::vector<int> Board::findKing(char color, Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    std::vector<int> position = {-1, -1};
    for(int i = 0; i<BOARDSIZE; i++) {
        for(int j = 0; j<BOARDSIZE; j++) {
            std::string currPiece = bord[i][j].piece;
            if(currPiece == "wking" && color == 'w') {
                position = {j, i};
                return position;
            }
            else if(currPiece == "bking" && color == 'b') {
                position = {j, i};
                return position;
            }
        }
    }
    // should not be possible... unless called wrong or another error
    return position;
}

void Board::determineRook(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
        // vertical+
    char selectedCol = getPiece(selected).name[0];
    std::cout << "ROOK  FN: " << selectedCol << std::endl;

    int next = selected[1];
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next + 1;
        if(next < BOARDSIZE) {
            std::string curr = bord[next][selected[0]].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[next][selected[0]] = 2;
            if(curr == "none") {
                high[next][selected[0]] = 1;
            } else break;
        }
    }
    // horizontal+
    next = selected[0];
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next + 1;
        if(next < BOARDSIZE) {
            std::string curr = bord[selected[1]][next].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[selected[1]][next] = 2;
            if(curr == "none") {
                high[selected[1]][next] = 1;
            } else break;
        }
    }
    // vertical-
    next = selected[1];
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next - 1;
        if(next >= 0) {
            std::string curr = bord[next][selected[0]].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[next][selected[0]] = 2;
            if(curr == "none") {
                high[next][selected[0]] = 1;
                std::cout << "WERE SUPPOSED TO BE MARKING " << std::endl;
            } else break;
        }
    }
    // horizontal-
    next = selected[0];
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next - 1;
        if(next >= 0) {
            std::string curr = bord[selected[1]][next].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[selected[1]][next] = 2;
            if(curr == "none") {
                high[selected[1]][next] = 1;
            } else break;
        }
    }
}

void Board::determineBishop(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    
    // down to right
    char selectedCol = getPiece(selected).name[0];
    std::cout << "BISHOP FN: " << selectedCol << std::endl;

    int nextY = selected[1];
    int nextX = selected[0];
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY + 1;
        nextX = nextX + 1;
        if(nextY < BOARDSIZE && nextX < BOARDSIZE) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[nextY][nextX] = 2;
            if(curr == "none") {
                high[nextY][nextX] = 1;
            } else break;
        }
    }
    // down and to left
    nextY = selected[1];
    nextX = selected[0];
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY + 1;
        nextX = nextX - 1;
        if(nextY < BOARDSIZE && nextX >= 0) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[nextY][nextX] = 2;
            if(curr == "none") {
                high[nextY][nextX] = 1;
            } else break;
        }
    }
    // up and to left
    nextY = selected[1];
    nextX = selected[0];
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY - 1;
        nextX = nextX - 1;
        if(nextY >= 0 && nextX >= 0) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[nextY][nextX] = 2;
            if(curr == "none") {
                high[nextY][nextX] = 1;
            } else break;
        }
    }
    // up and to right
    nextY = selected[1];
    nextX = selected[0];
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY - 1;
        nextX = nextX + 1;
        if(nextY >= 0 && nextX < BOARDSIZE) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) high[nextY][nextX] = 2;
            if(curr == "none") {
                high[nextY][nextX] = 1;
            } else break;
        }
    }
}

void Board::determineQueen(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    std::cout << "QUEEN FN: "  << std::endl;
    determineBishop(selected, high, bord);
    determineRook(selected, high, bord);
}

void Board::determineKnight(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    char selectedCol = getPiece(selected).name[0];
    // std::cout << "KNIGT FN: " << selectedCol << std::endl;
    int nextX = selected[0] - 1;
    int nextY = selected[1] - 2;
    if(nextX>=0 && nextY >=0) knightHelper(selectedCol, high, bord, nextX, nextY);
    nextY = selected[1] + 2;
    if(nextX>=0 && nextY <= 7) knightHelper(selectedCol, high, bord, nextX, nextY);
    nextX = selected[0] + 1;
    if(nextX<=7 && nextY <=7) knightHelper(selectedCol, high, bord, nextX, nextY);
    nextX = selected[0] + 1;
    nextY = selected[1] - 2;
    if(nextX<=7 && nextY >= 0) knightHelper(selectedCol, high, bord, nextX, nextY);
    nextX = selected[0] + 2;
    nextY = selected[1] + 1;
    if(nextX<=7 && nextY <= 7) knightHelper(selectedCol, high, bord, nextX, nextY);
    nextY = selected[1] - 1;
    if(nextX<=7 && nextY >= 0) knightHelper(selectedCol, high, bord, nextX, nextY);
    nextX = selected[0] - 2;
    if(nextX>=0 && nextY >= 0) knightHelper(selectedCol, high, bord, nextX, nextY);
    nextY = selected[1] + 1;
    nextX = selected[0] - 2;
    if(nextX>= 0 && nextY <= 7) knightHelper(selectedCol, high, bord, nextX, nextY);
}

void Board::knightHelper(char selectedCol, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE], int nextX, int nextY) {
    if((selectedCol == 'w' && bord[nextY][nextX].piece[0] == 'n') || (selectedCol == 'b' && bord[nextY][nextX].piece[0] == 'n')) high[nextY][nextX] = 1;
    if(selectedCol == 'w' && bord[nextY][nextX].piece[0] == 'b') high[nextY][nextX] = 2;
    if(selectedCol == 'b' && bord[nextY][nextX].piece[0] == 'w') high[nextY][nextX] = 2;
}


void Board::determinePawn(std::vector<int> selected, int (&high)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    char selectedCol = getPiece(selected).name[0];
    std::cout << "DETERMINING A: " << selectedCol << std::endl;
    std::cout << "SELECTED COLOR IS: " << selectedCol << std::endl;

    // for whites moves
    if(selectedCol=='w') {
        // only move negative direction
        if(frontColor=="white") {
            int nextY = selected[1]-1;
            // horizontal takes
            if(nextY >= 0) {
                int nextX = selected[0]-1;
                if(nextX >= 0) {
                    if(bord[nextY][nextX].piece[0] == 'b') high[nextY][nextX] = 2;
                }
                nextX = selected[0]+1;
                if(nextX <= 7) {
                    if(bord[nextY][nextX].piece[0] == 'b') high[nextY][nextX] = 2;
                }
                nextX = selected[0];
                if(bord[nextY][nextX].piece[0] == 'n') high[nextY][nextX] = 1;
                if(selected[1]==6) {
                    nextY = selected[1] - 2;
                    if(bord[nextY][nextX].piece[0] == 'n') {
                        high[nextY][nextX] = 1;
                    }
                }
            }
        }
        // move in positive direction
        if(frontColor=="black") {
            int nextY = selected[1]+1;
            // horizontal takes
            if(nextY <= 7) {
                int nextX = selected[0]-1;
                if(nextX >= 0) {
                    if(bord[nextY][nextX].piece[0] == 'b') high[nextY][nextX] = 2;
                }
                nextX = selected[0]+1;
                if(nextX <= 7) {
                    if(bord[nextY][nextX].piece[0] == 'b') high[nextY][nextX] = 2;
                }
                nextX = selected[0];
                if(bord[nextY][nextX].piece[0] == 'n') high[nextY][nextX] = 1;
                if(selected[1]==1) {
                    nextY = selected[1] + 2;
                    if(bord[nextY][nextX].piece[0] == 'n') {
                        high[nextY][nextX] = 1;
                    }
                }
            }
        }
    }
    if(selectedCol=='b') {
        // only move negative direction
        std::cout << "SELECTED BLACK" << std::endl;
        if(frontColor=="black") {
            int nextY = selected[1]-1;
            // horizontal takes
            if(nextY >= 0) {
                int nextX = selected[0]-1;
                if(nextX >= 0) {
                    if(bord[nextY][nextX].piece[0] == 'w') high[nextY][nextX] = 2;
                }
                nextX = selected[0]+1;
                if(nextX <= 7) {
                    if(bord[nextY][nextX].piece[0] == 'w') high[nextY][nextX] = 2;
                }
                nextX = selected[0];
                if(bord[nextY][nextX].piece[0] == 'n') high[nextY][nextX] = 1;
                if(selected[1]==6) {
                    nextY = selected[1] - 2;
                    if(bord[nextY][nextX].piece[0] == 'n') {
                        high[nextY][nextX] = 1;
                    }
                }
            }
        }
        // move in positive direction
        if(frontColor=="white") {
            int nextY = selected[1]+1;
            // horizontal takes
            if(nextY <= 7) {
                int nextX = selected[0]-1;
                if(nextX >= 0) {
                    if(bord[nextY][nextX].piece[0] == 'w') high[nextY][nextX] = 2;
                }
                nextX = selected[0]+1;
                if(nextX <= 7) {
                    if(bord[nextY][nextX].piece[0] == 'w') high[nextY][nextX] = 2;
                }
                nextX = selected[0];
                if(bord[nextY][nextX].piece[0] == 'n') high[nextY][nextX] = 1;
                if(selected[1]==1) {
                    nextY = selected[1] + 2;
                    if(bord[nextY][nextX].piece[0] == 'n') {
                        high[nextY][nextX] = 1;
                    }
                }
            }
        }
    }
}

// highlight boolean changes whether when tile is draw if it will draw orange on top

void Board::highlightTile(std::vector<int> selected, bool highlight) {
    std::cout << "THE SELECTED COORDINATES ARE: " << selected[0] << selected [1] << std::endl;
    board[selected[1]][selected[0]].selected = highlight;
}

void Board::move(std::vector<int> prev, std::vector<int> next, std::string nextPieceName, std::string prevPieceName) {
    
    // update score each time, mod param is to allow undo to inc score
    board[prev[1]][prev[0]].changePiece("none");
    board[next[1]][next[0]].changePiece(prevPieceName);
    int mod = 1;
    computeScore(nextPieceName, mod);
}
void Board::undo(std::vector<int> prev, std::vector<int> next, std::string nextPieceName, std::string prevPieceName) {
    board[prev[1]][prev[0]].changePiece(prevPieceName);
    board[next[1]][next[0]].changePiece(nextPieceName);
    int mod = -1;
    computeScore(nextPieceName, mod);
}

// update score each time, mod param is to allow undo to inc score
void Board::computeScore(std::string nextPieceName, int mod) {
    std::string player = nextPieceName.substr(0, 1);
    if(player == "n") return;
    std::string truePiece = nextPieceName.substr(1, nextPieceName.length()-1);
    std::cout << truePiece << std::endl;
    if(truePiece == "rook") player == "w" ? whiteScore -= 5*mod : blackScore -= 5*mod;
    if(truePiece == "pawn") player == "w" ? whiteScore -= 1*mod : blackScore -= 1*mod;
    if(truePiece == "knight") player == "w" ? whiteScore -= 3*mod : blackScore -= 3*mod;
    if(truePiece == "queen") player == "w" ? whiteScore -= 10*mod : blackScore -= 10*mod;
    if(truePiece == "bishop") player == "w" ? whiteScore -= 3*mod : blackScore -= 3*mod;

    std::cout << "blacks score is: " << blackScore << "whites score is: " << whiteScore << std::endl;

    whiteRatio = pow(whiteScore * 2 / (whiteScore + blackScore), 3) ;
    blackRatio = pow(blackScore * 2 / (blackScore + whiteScore), 3) ;
}

Piece Board::getPiece(std::vector<int> coords) {
    return board[coords[1]][coords[0]].piece;
}

void Board::initPawns() {

    for(int j = 0; j < BOARDSIZE; j++) {
        if(frontColor=="black") {
            board[1][j].changePiece("wpawn");
            board[6][j].changePiece("bpawn");
        }
        if(frontColor=="white") {
            board[1][j].changePiece("bpawn");
            board[6][j].changePiece("wpawn");
        }
    }
}

void Board::initRooks() {

    if(frontColor=="black") {
        board[0][0].changePiece("wrook");
        board[0][7].changePiece("wrook");
        board[7][7].changePiece("brook");
        board[7][0].changePiece("brook");
    }
    if(frontColor=="white") {
        board[0][0].changePiece("brook");
        board[0][7].changePiece("brook");
        board[7][7].changePiece("wrook");
        board[7][0].changePiece("wrook");
    }
}

void Board::initKnights() {
    if(frontColor=="black") {
        board[0][1].changePiece("wknight");
        board[0][6].changePiece("wknight");
        board[7][1].changePiece("bknight");
        board[7][6].changePiece("bknight");
    }
    if(frontColor=="white") {
        board[0][1].changePiece("bknight");
        board[0][6].changePiece("bknight");
        board[7][1].changePiece("wknight");
        board[7][6].changePiece("wknight");
    }
}

void Board::initBishops() {
    if(frontColor=="black") {
        board[0][2].changePiece("wbishop");
        board[0][5].changePiece("wbishop");
        board[7][2].changePiece("bbishop");
        board[7][5].changePiece("bbishop");
    }
    if(frontColor=="white") {
        board[0][2].changePiece("bbishop");
        board[0][5].changePiece("bbishop");
        board[7][2].changePiece("wbishop");
        board[7][5].changePiece("wbishop");
    }
}
void Board::initKings() {
    if(frontColor=="black") {
        board[0][3].changePiece("wking");
        board[7][3].changePiece("bking");
        // inverted because leftmost [] is y, rightmost is x
        wkingPosition = {3,0};
        bkingPosition = {3,7};
    }
    if(frontColor=="white") {
        board[0][4].changePiece("bking");
        board[7][4].changePiece("wking");
        wkingPosition = {4,7};
        bkingPosition = {4,0};
    }
}
void Board::initQueens() {
    if(frontColor=="black") {
        board[0][4].changePiece("wqueen");
        board[7][4].changePiece("bqueen");
    }
    if(frontColor=="white") {
        board[0][3].changePiece("bqueen");
        board[7][3].changePiece("wqueen");
    }
}

void Board::draw() {
    for(int i = 0; i < BOARDSIZE; i++) {
        for(int j = 0; j < BOARDSIZE; j ++) {
            float y = i*SQUARESIZE;
            float x = j*SQUARESIZE;
            DrawRectangle(x, y, SQUARESIZE, SQUARESIZE, board[i][j].color);
            if(highlights[i][j] == 1) DrawCircle(x +(SQUARESIZE/2), y+(SQUARESIZE/2), SQUARESIZE/4, Color {0, 0 , 0, 50});
            if(board[i][j].selected) DrawRectangle(x, y, SQUARESIZE, SQUARESIZE, board[i][j].selectedColor);
            Rectangle sprite = {0, 0, SQUARESIZE, SQUARESIZE};
            if(board[i][j].piece == "none") {
            } else if (board[i][j].piece == "wpawn") {
                sprite = {0, 0, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "bpawn") {
                sprite = {0, 128, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "brook") {
                sprite = {3*SQUARESIZE, SQUARESIZE, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "wrook") {
                sprite = {3*SQUARESIZE, 0, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "bknight") {
                sprite = {1*SQUARESIZE, SQUARESIZE, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "wknight") {
                sprite = {1*SQUARESIZE, 0, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "bbishop") {
                sprite = {2*SQUARESIZE, SQUARESIZE, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "wbishop") {
                sprite = {2*SQUARESIZE, 0, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "bking") {
                sprite = {4*SQUARESIZE, SQUARESIZE, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "wking") {
                sprite = {4*SQUARESIZE, 0, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "bqueen") {
                sprite = {5*SQUARESIZE, SQUARESIZE, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            } else if (board[i][j].piece == "wqueen") {
                sprite = {5*SQUARESIZE, 0, SQUARESIZE, SQUARESIZE};
                DrawTextureRec(sprites, sprite, Vector2{x, y}, WHITE);
            }
            // killable pieces
            if(highlights[i][j]==2) DrawRectangle(x, y, SQUARESIZE, SQUARESIZE, Color { 230, 41, 55, 120 } );
        }
    }
    // DrawTexture(sprites, 0, 0, WHITE);
}