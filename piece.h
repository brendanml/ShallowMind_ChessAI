#pragma once
#include <string>
#include <iostream>

class Piece {
    // PIECES are the actors not the players....
    public:
        std::string name;
        Piece() {
            
        }
        Piece(std::string _name) : name(_name){
        }
        ~Piece(){}
};