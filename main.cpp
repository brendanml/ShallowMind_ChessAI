#include <iostream>
#include <raylib.h>
#include <queue>
#include <string.h>
#include <vector>
#include <stack>
#include "player.h"
#include "square.h"
#include "board.h"
#include "util.h"
#include "command.h"
#include "input.h"
#include "ui.h"


// BANDAID FIX TO RESET THE BOARD ONLY AFTER COLOR HAS BEEN SELECTED
bool reset = true;

// generic interface for commands

class Game {
    public:
        bool drawBoard = false;
        bool gameover = false;
        std::string selectedColor = "white";

        // determining UI update/drawing
        bool selectPlayerCount = true;
        bool selectColor = false;
        std::string mode = "none";

        //UI variable
        float buttonSize = 512;
        int buttonMargin = 48;
        const char* playerCount = "How many players?";
        const char* playerColour = "What colour pieces?";
        
        int uiFontSize = 50;
        Vector2 spButton = {windowCenter - buttonSize - buttonMargin, boardWH/2-(buttonSize/2)};
        Rectangle spButtonClickbox = {spButton.x, spButton.y, buttonSize, buttonSize};
        Vector2 mpButton = {windowCenter + buttonMargin, boardWH/2-(buttonSize/2)};
        Rectangle mpButtonClickbox = {mpButton.x, mpButton.y, buttonSize, buttonSize};

        // assets
        Image spritesheet = LoadImage("./assets/pawns.png");
        Texture2D sprites = LoadTextureFromImage(spritesheet);

        // game stuff
        std::queue<Command*> cmdQueue;
        std::stack<Command*> undoStack;
        Player *black = new Player(false, 'b');
        Player *white = new Player(true, 'w');
        UI ui;
        Board board;
        InputHandler ih;

        // gameover stuff
        char* winMessage = "has won the game!";
        float widthGameOverUI = 512;
        float heightGameOverUI = 256;
        Vector2 gameOverUI = {(boardWH/2)-widthGameOverUI/2, (boardWH/2)-heightGameOverUI/2};
        float widthWinMessage = MeasureText(winMessage, uiFontSize);
        Rectangle rec_gameResBtn = {(boardWH/2)-(196/2), (boardWH/2)-(64/2)+80, 196, 64};
        char* restartGame = "start over?";
        float restartGameSize = MeasureText(restartGame, uiFontSize/2);


        // start over button
        float startOverWidth = 232;
        float startOverHeight = 64; 
        Rectangle rec_startOver = {BOARDSIZE*SQUARESIZE+12, BOARDSIZE*SQUARESIZE- 128, startOverWidth, startOverHeight};

        // select color back button
        float backWidth = 232;
        float backHeight = 64; 
        Rectangle rec_back = {windowCenter - 128, boardWH - 128, 256, 64};

        // draw AI battle
        float aiWidth = 232;
        float aiHeight = 64; 
        Rectangle rec_ai = {windowCenter - 128, boardWH - 128, 256, 64};



        Game() {

        }
        // draws
        ~Game() {
            freeCommands(undoStack, cmdQueue);
            delete black;
            delete white;
        }
        void draw();
        void update();
        void drawSP();
        void drawMP();
        void drawSelectPlayerCount();
        void drawColorSelection();
        void drawGameOver();
        void drawStartOver();
        void drawBack();
        void drawAIBattle();

        // updates
        void updateSelectPlayerCount(Vector2 pos);
        void updateSelectColor(Vector2 pos);
        void updateBoard();
        void updateGameOver(Vector2 pos);
        void resetBoard();
        void updateStartOver(Vector2 pos);
        void updateBack(Vector2 pos);
        void updateAIBattle(Vector2 pos);
};

void Game::drawAIBattle() {
    DrawRectangleRec(rec_ai, WHITE);
    DrawText("AI Battle", rec_back.x, rec_back.y, 26, BLACK);
}

void Game::updateAIBattle(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, rec_ai)) {
            resetBoard();
            selectPlayerCount = false;
            drawBoard = true;
            selectColor = false;
            gameover = false;
            selectedColor = "white";
            black->isAI = true;
            white->isAI = true;
            black->speedup = 10;
            white->speedup = 10;
        }
    }
}

void Game::drawBack() {
    DrawRectangleRec(rec_back, WHITE);
    DrawText("<- go back to select gamemode...", rec_back.x, rec_back.y, 26, BLACK);
}
void Game::updateBack(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, rec_back)) {
            resetBoard();
            selectPlayerCount = true;
            drawBoard = false;
            selectColor = false;
            gameover = false;
            selectedColor = "white";
            black->isAI = false;
            white->isAI = false;
        }
    }
}

void Game::drawStartOver() {
    DrawRectangleRec(rec_startOver, WHITE);
    DrawText("start over...", rec_startOver.x+8, rec_startOver.y+8, 28, BLACK);
}

void Game::updateStartOver(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, rec_startOver)) {
            resetBoard();
            selectPlayerCount = true;
            drawBoard = false;
            selectColor = false;
            gameover = false;
            selectedColor = "white";
            black->isAI = false;
            white->isAI = false;
        }
    }
}

void Game::updateGameOver(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, rec_gameResBtn)) {
            resetBoard();
            selectPlayerCount = true;
            drawBoard = false;
            selectColor = false;
            gameover = false;
            selectedColor = "white";
            black->isAI = false;
            white->isAI = false;
        }
    }
}


void Game::update() {
    Vector2 pos = GetMousePosition();
    if(selectPlayerCount) {
        updateSelectPlayerCount(pos);
        updateAIBattle(pos);
    } else if(selectColor) {
        updateSelectColor(pos);
        updateBack(pos);
    } else if(drawBoard && !gameover){
        if(reset) resetBoard();
        updateBoard();
        updateStartOver(pos);
    } else if(gameover) {
        updateGameOver(pos);
    }
};

void Game::updateSelectPlayerCount(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, spButtonClickbox)) {
            mode = "sp";
            selectPlayerCount = false;
            selectColor = true;
            std::cout << "THE MODE IS: " << mode << std::endl;
        }
        if(CheckCollisionPointRec(pos, mpButtonClickbox)) {
            mode = "mp";
            selectPlayerCount = false;
            selectColor = false;
            drawBoard = true;
            std::cout << "THE MODE IS: " << mode << std::endl;
        }
    }
}
void Game::updateSelectColor(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if(CheckCollisionPointRec(pos, spButtonClickbox)) {
            std::cout << "SELECTED COLOR WHITE " << std::endl;
            selectedColor = "white";
            black->isAI = true;
            selectColor = false;
            drawBoard = true;
            reset = true;
        }
        if(CheckCollisionPointRec(pos, mpButtonClickbox)) {
            std::cout << "SELECTED COLOR BLACK "  << std::endl;
            selectedColor = "black";
            white->isAI = true;
            selectColor = false;
            drawBoard = true;
        }
    }
}

void Game::resetBoard() {
    board.frontColor = selectedColor;
    board.resetBoard();
    board.blackScore = 40;
    board.whiteScore = 40;
    reset = false;
}

void Game::updateBoard() {
        if(black->turn) { ih.handleInput(gameover, cmdQueue, black, &board, white); }
        else if(white->turn) ih.handleInput(gameover, cmdQueue, white, &board, black);
        while (!cmdQueue.empty()) {
            Command* command = cmdQueue.front();
            command->execute();
            cmdQueue.pop();
            undoStack.push(command);
            std::cout << "WEVE MOVED THE BLACK KING TO: " << board.bkingPosition.x << " " << board.bkingPosition.y <<std::endl;
        }
        if(IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_Z)) {
            if(!undoStack.empty()) {
                Command* command = undoStack.top();
                command->undo();
                undoStack.pop();
                // undo undoes a command that would have swapped turns -> invert turn boolean as well
                black->turn = !black->turn;
                white->turn = !white->turn;
                delete command;
            }
        }
}
 
void Game::draw() {
    if(selectPlayerCount) {
        drawSelectPlayerCount();
        drawAIBattle();
    } else if(selectColor){
        drawColorSelection();
        drawBack();
    } else if(drawBoard){
        board.draw();
        if(!gameover) drawStartOver();
        ui.draw(*white, *black, board);
        if(gameover) drawGameOver();
    }

};

void Game::drawGameOver() {
    DrawRectangle(gameOverUI.x, gameOverUI.y, widthGameOverUI, heightGameOverUI, Color {255, 255, 255, 122});
    if(black->wonGame) {
        char* blackWin = "black";
        float widthBlack = MeasureText(blackWin, uiFontSize);
        DrawText(blackWin ,(boardWH/2)-widthBlack/2, (boardWH/2)-100, uiFontSize, WHITE);
    } else if(white->wonGame) {
        char* whiteWin = "white";
        float widthWhite = MeasureText(whiteWin, uiFontSize);
        DrawText(whiteWin ,(boardWH/2)-widthWhite/2, (boardWH/2)-100, uiFontSize, WHITE);
    }
    DrawText(winMessage ,(boardWH/2)-widthWinMessage/2, (boardWH/2)-16, uiFontSize, WHITE);
    DrawRectangleRec(rec_gameResBtn, WHITE);
    DrawText(restartGame ,(boardWH/2)-restartGameSize/2, rec_gameResBtn.y+16, uiFontSize/2, BLACK);
}

void Game::drawSelectPlayerCount() {
    DrawRectangle(0, 0, (SQUARESIZE*BOARDSIZE) + UIWIDTH, boardWH, BLACK);
    float offset = MeasureText(playerCount, uiFontSize);
    DrawText(playerCount, windowCenter-(offset/2), 128, uiFontSize, WHITE);
    drawSP();
    drawMP();
}

void Game::drawSP() {
    DrawRectangle(spButton.x, spButton.y, buttonSize, buttonSize, WHITE);
    char* one = "1";
    float offset = MeasureText(one, 512)/2;
    DrawText(one, spButton.x+offset, spButton.y, 512, BLACK);
};

void Game::drawMP() {
    DrawRectangle(mpButton.x, mpButton.y, buttonSize, buttonSize, WHITE);
    char* two = "2";
    float offset = MeasureText(two, 512)/2;
    DrawText(two, mpButton.x+offset, mpButton.y, 512, BLACK);
};

void Game::drawColorSelection() {
    float offset = MeasureText(playerColour, uiFontSize);
    DrawText(playerColour, windowCenter-(offset/2), 128, uiFontSize, WHITE);
    DrawRectangle(spButton.x, spButton.y, buttonSize, buttonSize, WHITE);
    Rectangle sprite = {0, 0, 512, 512};
    DrawTextureRec(sprites, sprite, Vector2{spButton.x, spButton.y}, WHITE);
    DrawRectangle(mpButton.x, mpButton.y, buttonSize, buttonSize, WHITE);
    sprite = {0, 512, 512, 512};
    DrawTextureRec(sprites, sprite, Vector2{mpButton.x, mpButton.y}, WHITE);
}
 


int main () {
    InitWindow(SQUARESIZE*BOARDSIZE + UIWIDTH, SQUARESIZE*BOARDSIZE, "Chess Command Pattern");
    SetWindowPosition(0,0);
    SetTargetFPS(60);
    Game game;
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        game.update();
        game.draw();
        EndDrawing();
    }
    return 0;
}