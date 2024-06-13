#pragma once
#include "square.h"
#include "board.h"
#include <string>
class Player {
    public:
        xy start;
        xy end;

        bool inCheck = false;

        bool turn = false;
        char color;
        bool selecting = true;
        bool isAI = false;
        bool turnStarting = true;
        int turnCount = 0;
        
        bool wonGame = false;
        bool lostGame = false;


        // if player is AI
        Timer timer;
        int waitPeriod = 0;
        double speedup = 1;




        Player(bool _turn, char _color) : turn(_turn), color(_color){
            start = {-1, -1};
            end = {-1, -1};
        }
        void setDestination(xy tileCoordinates);
        void select(xy tileCoordinates);
        void endTurn();
};

void Player::select(xy tileCoordinates) {
    start = tileCoordinates;
    selecting = false;
}

void Player::setDestination(xy tileCoordinates) {
    end = tileCoordinates;
    std::cout << "you're moving from tile: " << start.x << ", "<< start.y << "to tile: " << end.x << ", " << end.y << std::endl;
}