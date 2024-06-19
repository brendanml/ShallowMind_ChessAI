#pragma once
#include "board.h"
#include "player.h"


class UI {
    public:
    UI() {

    }
    void draw(Player white, Player black, Board board);
    void drawBar(Board board);
    private:
    int padding = 12;
    int cpadding = BOARDSIZE*SQUARESIZE + padding;
    int barMargin = 2*SQUARESIZE;

};

void UI::draw(Player white, Player black, Board board) {
    white.turn ? DrawText("WHITES", cpadding, 64, 48, WHITE) : DrawText("BLACKS", cpadding, 64, 48, WHITE);
    DrawText("TURN", cpadding, 128, 48, WHITE);
    drawBar(board);
}

void::UI::drawBar(Board board) {
    float whiteHeight = SQUARESIZE*2*board.whiteRatio;
    float blackHeight = SQUARESIZE*2*board.blackRatio;
    float widthMargin = UIWIDTH - (padding*2);
    if(board.frontColor == "black") {
        DrawRectangle(cpadding, barMargin, widthMargin, SQUARESIZE*4, Color {57, 74, 80, 255});
        // bandaid fix, sometimes winning side overflows 100%
        if(whiteHeight>SQUARESIZE*4) whiteHeight = SQUARESIZE*4;
        DrawRectangle(cpadding, barMargin, widthMargin, whiteHeight, WHITE);
    } else if(board.frontColor == "white") {
        DrawRectangle(cpadding, barMargin, widthMargin, SQUARESIZE*4, WHITE);
        // bandaid fix, sometimes winning side overflows 100%
        if(blackHeight>SQUARESIZE*4) blackHeight = SQUARESIZE*4;
        DrawRectangle(cpadding, barMargin, widthMargin, blackHeight, Color {57, 74, 80, 255});
    }
}