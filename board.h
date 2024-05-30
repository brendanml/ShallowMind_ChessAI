#pragma once
#include "util.h"
#include "square.h"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <list>
class Board {
    Image spritesheet;
    Texture2D sprites;
    public:
        Square board[BOARDSIZE][BOARDSIZE];
        std::vector<Move> highlights[BOARDSIZE][BOARDSIZE];
        xy wkingPosition;
        xy bkingPosition;
        Board() {
            spritesheet = LoadImage("./assets/pieces.png");
            sprites = LoadTextureFromImage(spritesheet);
            resetBoard();
        }
        Board(std::string assets){
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
        void move(xy prev, xy next, std::string nextPieceName, std::string prevPieceName);
        void undo(xy prev, xy next, std::string nextPieceName, std::string prevPieceName);
        void highlightTile(xy selected, bool removeHighlight);
        // high is an arbitrary int grid that determines potential moves -- used for many things like check, bots, players moves, etc
        void determineMoves(xy selected, std::string piece, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]); //dtermines all moves for piece given name
        void determineAllPieces(char color, Board &tempBoard, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE]); // calls determine moves for all pieces of a given side (white or black)
        void determineRook(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determineBishop(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determineQueen(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determinePawn(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void pawnHelper(int dir, char selectedCol, xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void determineKnight(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void knightHelper(xy selected, char selectedCol, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE], int nextX, int nextY);
        void determineKing(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        std::priority_queue<Element, std::vector<Element>, Compare> rankMoves(char playerColor, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Board &board);
        xy findKing(char color, Square (&bord)[BOARDSIZE][BOARDSIZE]);
        // check if piece is takeable - for king
        void kingMoveable(xy sel, int nextX, int nextY, char selectedCol, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]);
        void checkMoves(); //for checking if king is moving into check/is in check/bots potential moves
        std::string getPiece(xy coordinates);
        float whiteScore = 40;
        float blackScore = 40;
        std::string frontColor = "white";

        // used to calculate winning bar, starts at 1 instead of 0.5 because we're amplifying it with exponents (how many pieces I have vs how many they)^2
        float whiteRatio = 1;
        float blackRatio = 1;
};

std::priority_queue<Element, std::vector<Element>, Compare> Board::rankMoves(char playerColor, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Board &board) {
    std::priority_queue<Element, std::vector<Element>, Compare> maxHeap;
    // need a way to mark tiles that are covered by other pieces (if we take a piece right now it won't show that an opposing piece could take it because one of their own pieces used to be there)
    // std::vector<Move> opposingMoves[BOARDSIZE][BOARDSIZE];
    // if(playerColor == 'b') { board.determineAllPieces('w', board, opposingMoves);}
    // else if(playerColor == 'w') board.determineAllPieces('b', board, opposingMoves);


    for(int i = 0; i<BOARDSIZE; i++) {
        for(int j = 0; j<BOARDSIZE; j++) {
            while(!potentialMoves[i][j].empty()) {
                // std::string truePiece = board.board[i][j].piece.substr(1, board.board[i][j].piece.length()-1);
                Move curr = potentialMoves[i][j].back();
                potentialMoves[i][j].pop_back();
                std::string taking = board.board[curr.end.y][curr.end.x].piece.substr(1, board.board[curr.end.y][curr.end.x].piece.length());
                // default "none" value is 0
                int myPieceValue = 0;
                int takingPieceValue = 0;
                if(curr.piece == "queen") {myPieceValue = 10;}
                else if(curr.piece == "pawn") {myPieceValue = 1;}
                else if(curr.piece == "rook") {myPieceValue = 5;}
                else if(curr.piece == "bishop") {myPieceValue = 3;}
                else if(curr.piece == "knight") {myPieceValue = 3;}

                if(taking == "queen") {takingPieceValue = 10;}
                else if(taking == "pawn") {takingPieceValue = 1;}
                else if(taking == "rook") {takingPieceValue = 5;}
                else if(taking == "bishop") {takingPieceValue = 3;}
                else if(taking == "knight") {takingPieceValue = 3;}
                // int moveValue = takingPieceValue - myPieceValue;
                int moveValue = takingPieceValue;

                maxHeap.push({moveValue, curr.start, curr.end, curr.piece});
                // std::cout << curr.piece << ": start x: " << curr.start.x << "start 3c: " << curr.start.y << ", end x: " << curr.end.x << "end y: " << curr.end.y << std::endl;
            }
        }
    }
    return maxHeap;
}

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
            highlights[i][j].clear();
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

void Board::determineAllPieces(char color, Board &tempBoard, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE]) {
    for(int i = 0; i<BOARDSIZE; i++) {
        for(int j = 0; j<BOARDSIZE; j++) {
            // check OPPOSING MOVES for EACH piece
            std::string currPieceName = tempBoard.board[i][j].piece;
            std::string truePieceName = currPieceName.substr(1, currPieceName.length()-1);
            if(currPieceName[0]==color) {
                // remember j = x, i = y
                xy sel = {j, i};
                tempBoard.determineMoves(sel, truePieceName, potentialMoves, tempBoard.board);
            }
        }
    }
}

void Board::determineMoves(xy selected, std::string piece, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
//
    // std::cout << "were determining a: " << piece << std::endl;
    if(piece == "rook") determineRook(selected, potentialMoves, bord);
    if(piece == "pawn") determinePawn(selected, potentialMoves, bord);
    if(piece == "bishop") determineBishop(selected, potentialMoves, bord);
    if(piece == "queen") determineQueen(selected, potentialMoves, bord);
    if(piece == "knight") determineKnight(selected, potentialMoves, bord);
    if(piece == "king") determineKing(selected, potentialMoves, bord);
}

void Board::determineKing(xy sel, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
// top and to right
    char selectedCol = getPiece(sel)[0];
    // majority of cases -> highlight all surrounding pieces
    // determineCastling();

    kingMoveable(sel, sel.x - 1, sel.y - 1, selectedCol, potentialMoves, bord);
    kingMoveable(sel, sel.x, sel.y - 1, selectedCol, potentialMoves, bord);
    kingMoveable(sel, sel.x + 1, sel.y - 1, selectedCol, potentialMoves, bord);
    kingMoveable(sel, sel.x + 1, sel.y, selectedCol, potentialMoves, bord);
    kingMoveable(sel, sel.x + 1, sel.y + 1, selectedCol, potentialMoves, bord);
    kingMoveable(sel, sel.x, sel.y + 1, selectedCol, potentialMoves, bord);
    kingMoveable(sel, sel.x - 1, sel.y + 1, selectedCol, potentialMoves, bord);
    kingMoveable(sel, sel.x - 1, sel.y, selectedCol, potentialMoves, bord);
}

// void Board::determineCastling() {

// }

void Board::kingMoveable(xy sel, int currX, int currY, char selectedCol, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    // check x within bounds
    if(currX>= 0 && currX <= 7) {
        // check y within neg
        if(currY >=0 && currY <=7) {
            char curr = bord[currY][currX].piece[0];
            if((curr == 'b' && selectedCol == 'w') || (curr == 'w' && selectedCol == 'b')) potentialMoves[currY][currX].push_back(Move {2, sel, xy{currX, currY}, "king"});
            if(curr == 'n') {
                potentialMoves[currY][currX].push_back(Move {1, sel, xy{currX, currY}, "king"});
            }
        }
    }   
}

xy Board::findKing(char color, Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    xy position = {-1, -1};
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

void Board::determineRook(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
        // vertical+
    char selectedCol = getPiece(selected)[0];
    // std::cout << "ROOK  FN: " << selectedCol << std::endl;

    int next = selected.y;
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next + 1;
        if(next < BOARDSIZE) {
            std::string curr = bord[next][selected.x].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[next][selected.x].push_back(Move {2, selected, xy{selected.x, next}, "rook"});
            if(curr == "none") {
                potentialMoves[next][selected.x].push_back(Move {1, selected, xy{selected.x, next}, "rook"});
            } else break;
        }
    }
    // horizontal+
    next = selected.x;
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next + 1;
        if(next < BOARDSIZE) {
            std::string curr = bord[selected.y][next].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[selected.y][next].push_back(Move {2, selected, xy{next, selected.y}, "rook"});
            if(curr == "none") {
                potentialMoves[selected.y][next].push_back(Move {1, selected, xy{next, selected.y}, "rook"});
            } else break;
        }
    }
    // vertical-
    next = selected.y;
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next - 1;
        if(next >= 0) {
            std::string curr = bord[next][selected.x].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[next][selected.x].push_back(Move {2, selected, xy{selected.x, next}, "rook"});
            if(curr == "none") {
                potentialMoves[next][selected.x].push_back(Move {1, selected, xy{selected.x, next}, "rook"});
                // std::cout << "WERE SUPPOSED TO BE MARKING " << std::endl;
            } else break;
        }
    }
    // horizontal-
    next = selected.x;
    for(int i = 1; i<BOARDSIZE; i++) {
        next = next - 1;
        if(next >= 0) {
            std::string curr = bord[selected.y][next].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[selected.y][next].push_back(Move {2, selected, xy{next, selected.y}, "rook"});
            if(curr == "none") {
                potentialMoves[selected.y][next].push_back(Move {1, selected, xy{next, selected.y}, "rook"});
            } else break;
        }
    }
}

void Board::determineBishop(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    
    // down to right
    char selectedCol = getPiece(selected)[0];
    // std::cout << "BISHOP FN: " << selectedCol << std::endl;

    int nextY = selected.y;
    int nextX = selected.x;
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY + 1;
        nextX = nextX + 1;
        if(nextY < BOARDSIZE && nextX < BOARDSIZE) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "bishop"});
            if(curr == "none") {
                potentialMoves[nextY][nextX].push_back(Move {1, selected, xy{nextX, nextY}, "bishop"});
            } else break;
        }
    }
    // down and to left
    nextY = selected.y;
    nextX = selected.x;
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY + 1;
        nextX = nextX - 1;
        if(nextY < BOARDSIZE && nextX >= 0) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "bishop"});
            if(curr == "none") {
                potentialMoves[nextY][nextX].push_back(Move {1, selected, xy{nextX, nextY}, "bishop"});
            } else break;
        }
    }
    // up and to left
    nextY = selected.y;
    nextX = selected.x;
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY - 1;
        nextX = nextX - 1;
        if(nextY >= 0 && nextX >= 0) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "bishop"});
            if(curr == "none") {
                potentialMoves[nextY][nextX].push_back(Move {1, selected, xy{nextX, nextY}, "bishop"});
            } else break;
        }
    }
    // up and to right
    nextY = selected.y;
    nextX = selected.x;
    for(int i = 1; i<BOARDSIZE; i++) {
        nextY = nextY - 1;
        nextX = nextX + 1;
        if(nextY >= 0 && nextX < BOARDSIZE) {
            std::string curr = bord[nextY][nextX].piece;
            if((curr[0] == 'b' && selectedCol == 'w') || (curr[0] == 'w' && selectedCol == 'b')) potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "bishop"});
            if(curr == "none") {
                potentialMoves[nextY][nextX].push_back(Move {1, selected, xy{nextX, nextY}, "bishop"});
            } else break;
        }
    }
}

void Board::determineQueen(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    // std::cout << "QUEEN FN: "  << std::endl;
    determineBishop(selected, potentialMoves, bord);
    determineRook(selected, potentialMoves, bord);
}

void Board::determineKnight(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    char selectedCol = getPiece(selected)[0];
    // std::cout << "KNIGT FN: " << selectedCol << std::endl;
    int nextX = selected.x - 1;
    int nextY = selected.y - 2;
    if(nextX>=0 && nextY >=0) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
    nextY = selected.y + 2;
    if(nextX>=0 && nextY <= 7) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
    nextX = selected.x + 1;
    if(nextX<=7 && nextY <=7) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
    nextX = selected.x + 1;
    nextY = selected.y - 2;
    if(nextX<=7 && nextY >= 0) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
    nextX = selected.x + 2;
    nextY = selected.y + 1;
    if(nextX<=7 && nextY <= 7) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
    nextY = selected.y - 1;
    if(nextX<=7 && nextY >= 0) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
    nextX = selected.x - 2;
    if(nextX>=0 && nextY >= 0) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
    nextY = selected.y + 1;
    nextX = selected.x - 2;
    if(nextX>= 0 && nextY <= 7) knightHelper(selected, selectedCol, potentialMoves, bord, nextX, nextY);
}

void Board::knightHelper(xy selected, char selectedCol, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE], int nextX, int nextY) {
    if((selectedCol == 'w' && bord[nextY][nextX].piece[0] == 'n') || (selectedCol == 'b' && bord[nextY][nextX].piece[0] == 'n')) potentialMoves[nextY][nextX].push_back(Move {1, selected, xy{nextX, nextY}, "knight"});
    if(selectedCol == 'w' && bord[nextY][nextX].piece[0] == 'b') potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "knight"});
    if(selectedCol == 'b' && bord[nextY][nextX].piece[0] == 'w') potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "knight"});
}


void Board::determinePawn(xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {
    char selectedCol = getPiece(selected)[0];
    // std::cout << "DETERMINING A: " << selectedCol << std::endl;
    // std::cout << "SELECTED COLOR IS: " << selectedCol << std::endl;
    int direction = 0;
    if(frontColor == "black") {
        if(selectedCol=='w') {direction = 1;}
        else if(selectedCol=='b') {direction = -1;}
    } else if(frontColor == "white") {
        if(selectedCol=='w') {direction = -1;}
        else if(selectedCol=='b') {direction = 1;}
    }

    pawnHelper(direction, selectedCol, selected, potentialMoves, bord);
}

void Board::pawnHelper(int dir, char selectedCol, xy selected, std::vector<Move> (&potentialMoves)[BOARDSIZE][BOARDSIZE], Square (&bord)[BOARDSIZE][BOARDSIZE]) {

    int nextY = selected.y+(1*dir);
    // horizontal takes
    if(nextY <= 7 && nextY >= 0) {
        int nextX = selected.x-1;
        // taking piece left
        if(nextX >= 0) {
            if((selectedCol == 'b' && bord[nextY][nextX].piece[0] == 'w') || (selectedCol == 'w' && bord[nextY][nextX].piece[0] == 'b')) potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "pawn"});
        }
        nextX = selected.x+1;
        // taking piece right
        if(nextX <= 7) {
            if((selectedCol == 'b' && bord[nextY][nextX].piece[0] == 'w') || (selectedCol == 'w' && bord[nextY][nextX].piece[0] == 'b')) potentialMoves[nextY][nextX].push_back(Move {2, selected, xy{nextX, nextY}, "pawn"});
        }
        // moving 1 forward
        nextX = selected.x;
        if(bord[nextY][nextX].piece[0] == 'n') potentialMoves[nextY][nextX].push_back(Move {1, selected, xy{nextX, nextY}, "pawn"});
        if((selected.y==1 || selected.y==6)&& bord[selected.y+(1*dir)][selected.x].piece[0] == 'n') {
            nextY = selected.y + (2*dir);
            if(bord[nextY][nextX].piece[0] == 'n') {
                potentialMoves[nextY][nextX].push_back(Move {1, selected, xy{nextX, nextY}, "pawn"});
            }
        }
    }

}

// highlight boolean changes whether when tile is draw if it will draw orange on top

void Board::highlightTile(xy selected, bool highlight) {
    // std::cout << "THE SELECTED COORDINATES ARE: " << selected[0] << selected .y << std::endl;
    board[selected.y][selected.x].selected = highlight;
}

void Board::move(xy prev, xy next, std::string nextPieceName, std::string prevPieceName) {
    
    // update score each time, mod param is to allow undo to inc score
    board[prev.y][prev.x].changePiece("none");
    board[next.y][next.x].changePiece(prevPieceName);
    int mod = 1;
    computeScore(nextPieceName, mod);
}
void Board::undo(xy prev, xy next, std::string nextPieceName, std::string prevPieceName) {
    board[prev.y][prev.x].changePiece(prevPieceName);
    board[next.y][next.x].changePiece(nextPieceName);
    int mod = -1;
    computeScore(nextPieceName, mod);
}

// update score each time, mod param is to allow undo to inc score
void Board::computeScore(std::string nextPieceName, int mod) {
    std::string player = nextPieceName.substr(0, 1);
    if(player == "n") return;
    std::string truePiece = nextPieceName.substr(1, nextPieceName.length()-1);
    // std::cout << truePiece << std::endl;
    if(truePiece == "rook") player == "w" ? whiteScore -= 5*mod : blackScore -= 5*mod;
    if(truePiece == "pawn") player == "w" ? whiteScore -= 1*mod : blackScore -= 1*mod;
    if(truePiece == "knight") player == "w" ? whiteScore -= 3*mod : blackScore -= 3*mod;
    if(truePiece == "queen") player == "w" ? whiteScore -= 10*mod : blackScore -= 10*mod;
    if(truePiece == "bishop") player == "w" ? whiteScore -= 3*mod : blackScore -= 3*mod;

    // std::cout << "blacks score is: " << blackScore << "whites score is: " << whiteScore << std::endl;

    whiteRatio = pow(whiteScore * 2 / (whiteScore + blackScore), 3) ;
    blackRatio = pow(blackScore * 2 / (blackScore + whiteScore), 3) ;
}

std::string Board::getPiece(xy coords) {
    return board[coords.y][coords.x].piece;
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
            if(!highlights[i][j].empty() && highlights[i][j].front().type == 1) DrawCircle(x +(SQUARESIZE/2), y+(SQUARESIZE/2), SQUARESIZE/4, Color {0, 0 , 0, 50});
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
            if(!highlights[i][j].empty() && highlights[i][j].front().type == 2) DrawRectangle(x, y, SQUARESIZE, SQUARESIZE, Color { 230, 41, 55, 120 } );
        }
    }
    // DrawTexture(sprites, 0, 0, WHITE);
}