#pragma once
// #include <iostream>
#include <raylib.h>
class Square {
    public:
        Color color;
        
        std::string piece = "none";
        bool selected = false;
        bool pieceMoved = false;
        Square() {

        }
        Square(Color _color) : color(_color){
            // DO NOT CHANGE THIS NAME, LOTS OF LOGIC DEPENDS ON 'n' character
            piece = "none";
        }
        ~Square() {}
        void changePiece(std::string pieceName);
        std::string truePiece();
};

void Square::changePiece(std::string pieceName) {
    piece = pieceName;
}
std::string Square::truePiece() {
    return piece.substr(1, piece.length()-1);
}
