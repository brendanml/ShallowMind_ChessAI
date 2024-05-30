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
    int barMargin = 256;

};

void UI::draw(Player white, Player black, Board board) {
    white.turn ? DrawText("WHITES", cpadding, 64, 48, WHITE) : DrawText("BLACKS", cpadding, 64, 48, WHITE);
    DrawText("TURN", cpadding, 128, 48, WHITE);
    drawBar(board);
}

void::UI::drawBar(Board board) {
    float whiteHeight = 256*board.whiteRatio;
    float blackHeight = 256*board.blackRatio;
    float widthMargin = UIWIDTH - (padding*2);
    if(board.frontColor == "black") {
        DrawRectangle(cpadding, barMargin, widthMargin, 512, Color {105, 127, 150, 255});
        DrawRectangle(cpadding, barMargin, widthMargin, whiteHeight, Color {255, 255, 255, 255});
    } else if(board.frontColor == "white") {
        DrawRectangle(cpadding, barMargin, widthMargin, 512, WHITE);
        DrawRectangle(cpadding, barMargin, widthMargin, blackHeight, Color {105, 127, 150, 255});
    }
}