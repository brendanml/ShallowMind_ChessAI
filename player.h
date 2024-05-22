#pragma once
#include "square.h"
#include "board.h"
#include "piece.h"
#include <string>
class Player {
    public:
        std::vector<int> start;
        std::vector<int> end;
        bool turn;
        char color;
        bool selecting = true;

        Player(bool _turn, char _color) : turn(_turn), color(_color){
            start = {-1, -1};
            end = {-1, -1};
        }
        void setDestination(std::vector<int> tileCoordinates);
        void select(std::vector<int> tileCoordinates);
};

void Player::select(std::vector<int> tileCoordinates) {
    start = tileCoordinates;
    selecting = false;
}

void Player::setDestination(std::vector<int> tileCoordinates) {
    end = tileCoordinates;
    std::cout << "you're moving from tile: " << start[0] << ", "<< start[1] << "to tile: " << end[0] << ", " << end[1] << std::endl;
    // start = {-1, -1};
    // end = {-1, -1}; // RESET POSITION WHEN? after move is executed?
}