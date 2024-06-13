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

// generic interface for commands

class Game {
    public:
        bool drawBoard = false;
        bool gameover = false;

        // determining UI update/drawing
        bool selectPlayerCount = true;
        bool selectColor = false;
        std::string mode = "none";

        //UI variable
        float buttonSize = SQUARESIZE*4;
        int buttonMargin = 48;
        const char* playerCount = "How many players?";
        const char* playerColour = "What colour pieces?";
        
        int uiFontSize = 50;

        // assets
        Image spritesheet;

        Texture2D sprites;

        // game stuff
        std::queue<Command*> cmdQueue;
        std::stack<Command*> undoStack;
        Player *black = new Player(false, 'b');
        Player *white = new Player(true, 'w');
        UI ui;
        Board board;
        InputHandler ih;

        // gameover stuff
        float widthGameOverUI = 512;
        float heightGameOverUI = 256;
        Vector2 gameOverUI = {(boardWH/2)-widthGameOverUI/2, (boardWH/2)-heightGameOverUI/2};
        float widthWinMessage = MeasureText("has won the game!", uiFontSize);
        Rectangle rec_gameResBtn = {(boardWH/2)-(196/2), (boardWH/2)-(64/2)+80, 196, 64};
        float restartGameSize = MeasureText("start over?", uiFontSize/2);

        // mp/sp buttons
        Rectangle spButton = {windowCenter - buttonSize - buttonMargin, boardWH/2-(buttonSize/2), buttonSize, buttonSize};
        Rectangle mpButton = {windowCenter + buttonMargin, boardWH/2-(buttonSize/2), buttonSize, buttonSize};

        // select colors
        Rectangle whiteButton = {boardWH/2 - (SQUARESIZE), boardWH/2, buttonSize, buttonSize};
        Rectangle blackButton = {boardWH/2 + (SQUARESIZE), boardWH/2, buttonSize, buttonSize};

        // start over button
        float startOverWidth = 232;
        float startOverHeight = 64; 
        Rectangle rec_startOver = {BOARDSIZE*SQUARESIZE+12, BOARDSIZE*SQUARESIZE- 128, startOverWidth, startOverHeight};

        // select color BACK button
        float backWidth = 232;
        float backHeight = 64; 
        Rectangle rec_back = {windowCenter - 128, boardWH - 128, 256, 64};

        // draw AI battle
        float aiWidth = 232;
        float aiHeight = 64; 
        Rectangle rec_ai = {windowCenter - 128, boardWH - 128, 256, 64};



        Game() {
            spritesheet = LoadImage("../assets/pawns.png");
            ImageResize(&spritesheet, 4*SQUARESIZE, 8*SQUARESIZE);
            sprites = LoadTextureFromImage(spritesheet);
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
        void updateStartOver(Vector2 pos);
        void updateBack(Vector2 pos);
        void updateAIBattle(Vector2 pos);
        void resetGame();
};

void Game::drawAIBattle() {
    DrawRectangleRec(rec_ai, WHITE);
    DrawText("AI Battle", rec_back.x, rec_back.y, 26, BLACK);
}

void Game::resetGame() {
    selectPlayerCount = true;
    drawBoard = false;
    selectColor = false;
    gameover = false;
    board.blackScore = 40;
    board.whiteScore = 40;
    board.computeScore();
    board.resetBoard();
    delete black;
    delete white;
    black = new Player(false, 'b');
    white = new Player(true, 'w');
}

void Game::updateAIBattle(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, rec_ai)) {
            selectPlayerCount = false;
            drawBoard = true;
            selectColor = false;
            gameover = false;
            board.frontColor = "white";
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
            resetGame();
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
            resetGame();
        }
    }
}

void Game::updateGameOver(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, rec_gameResBtn)) {
            resetGame();
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
        updateBoard();
        updateStartOver(pos);
    } else if(gameover) {
        updateGameOver(pos);
    }
};

void Game::updateSelectPlayerCount(Vector2 pos) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // only get to choose color if selected single player
        if(CheckCollisionPointRec(pos, spButton)) {
            mode = "sp";
            selectPlayerCount = false;
            selectColor = true;
            std::cout << "THE MODE IS: " << mode << std::endl;
        }
        if(CheckCollisionPointRec(pos, mpButton)) {
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
        if(CheckCollisionPointRec(pos, whiteButton)) {
            std::cout << "SELECTED COLOR WHITE " << std::endl;
            board.frontColor = "white";
            black->isAI = true;
            white->isAI = false;
            selectColor = false;
            drawBoard = true;
        }
        if(CheckCollisionPointRec(pos, blackButton)) {
            std::cout << "SELECTED COLOR BLACK "  << std::endl;
            board.frontColor = "black";
            board.resetBoard();
            white->isAI = true;
            black->isAI = false;
            selectColor = false;
            drawBoard = true;
        }
    }
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
    DrawRectangle(gameOverUI.x, gameOverUI.y, widthGameOverUI, heightGameOverUI, Color {0, 0, 0, 210});
    if(black->wonGame && white->lostGame) {
        float widthBlack = MeasureText("black", uiFontSize);
        DrawText("black" ,(boardWH/2)-widthBlack/2, (boardWH/2)-100, uiFontSize, WHITE);
        DrawText("has won the game!" ,(boardWH/2)-widthWinMessage/2, (boardWH/2)-16, uiFontSize, WHITE);
    } else if(white->wonGame && black->lostGame) {
        float widthWhite = MeasureText("white", uiFontSize);
        DrawText("white" ,(boardWH/2)-widthWhite/2, (boardWH/2)-100, uiFontSize, WHITE);
        DrawText("has won the game!" ,(boardWH/2)-widthWinMessage/2, (boardWH/2)-16, uiFontSize, WHITE);
    } else {
        float widthStalemate = MeasureText("stalemate", uiFontSize);
        DrawText("stalemate!" ,(boardWH/2)-widthStalemate/2, (boardWH/2)-100, uiFontSize, WHITE);
    }
    DrawRectangleRec(rec_gameResBtn, WHITE);
    DrawText("start over?" ,(boardWH/2)-restartGameSize/2, rec_gameResBtn.y+16, uiFontSize/2, BLACK);
}

void Game::drawSelectPlayerCount() {
    DrawRectangle(0, 0, (SQUARESIZE*BOARDSIZE) + UIWIDTH, boardWH, BLACK);
    float offset = MeasureText(playerCount, uiFontSize);
    DrawText(playerCount, windowCenter-(offset/2), 128, uiFontSize, WHITE);
    drawSP();
    drawMP();
}

void Game::drawSP() {
    DrawRectangleRec(spButton, WHITE);
    float offset = MeasureText("1", (SQUARESIZE*4)/2);
    DrawText("1", spButton.x+offset, spButton.y, (SQUARESIZE*4), BLACK);
};

void Game::drawMP() {
    DrawRectangleRec(mpButton, WHITE);
    float offset = MeasureText("2", (SQUARESIZE*4)/2);
    DrawText("2", mpButton.x+offset, mpButton.y, (SQUARESIZE*4), BLACK);
};

void Game::drawColorSelection() {
    float offset = MeasureText(playerColour, uiFontSize);
    DrawText(playerColour, windowCenter-(offset/2), 128, uiFontSize, WHITE);
    DrawRectangle(spButton.x, spButton.y, buttonSize, buttonSize, WHITE);
    Rectangle largeWhite = {0, 0, 4*SQUARESIZE, 4*SQUARESIZE};

    DrawTextureRec(sprites, largeWhite, Vector2{spButton.x, spButton.y}, WHITE);
    DrawRectangle(mpButton.x, mpButton.y, buttonSize, buttonSize, WHITE);
    Rectangle largeBlack = {0, 4*SQUARESIZE, 4*SQUARESIZE, 4*SQUARESIZE};
    DrawTextureRec(sprites, largeBlack, Vector2{mpButton.x, mpButton.y}, WHITE);
}
 


int main () {
    InitWindow(SQUARESIZE*BOARDSIZE + UIWIDTH, SQUARESIZE*BOARDSIZE, "ShallowMind: ChessAI");
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