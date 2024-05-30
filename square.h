#pragma once
// #include <iostream>
#include <raylib.h>
class Square {
    public:
        Color color;
        Color selectedColor = ORANGE;
        std::string piece = "none";
        bool selected = false;
        Square() {

        }
        Square(Color _color) : color(_color){
            // DO NOT CHANGE THIS NAME, LOTS OF LOGIC DEPENDS ON 'n' character
            piece = "none";
        }
        ~Square() {}
        void changePiece(std::string pieceName);
        void changeColor(Color _color);
};

void Square::changePiece(std::string pieceName) {
    piece = pieceName;
}

void Square::changeColor(Color _color) {
    std::cout << "WERE CHANGING THE COLOR TO RED" << std::endl;
    color = _color;
}
