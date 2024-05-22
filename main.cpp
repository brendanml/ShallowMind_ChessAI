#include <iostream>
#include <raylib.h>
#include <queue>
#include <string.h>
#include <vector>
#include <stack>
#include "player.h"
#include "piece.h"
#include "square.h"
#include "board.h"
#include "util.h"


// BANDAID FIX TO RESET THE BOARD ONLY AFTER COLOR HAS BEEN SELECTED
bool reset = true;

// generic interface for commands
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

class Game {
    public:
        bool start = false;
        bool selectColor = false;
        std::string selectedColor = "black";
        float buttonSize = 512;
        int buttonMargin = 48;
        const char* playerCount = "How many players?";
        const char* playerColour = "What colour pieces?";
        int playerCountFont = 50;
        std::string mode = "none";
        Image spritesheet = LoadImage("./pawns.png");
        Texture2D sprites = LoadTextureFromImage(spritesheet);
        

        Vector2 spButton = {windowCenter - buttonSize - buttonMargin, boardWH/2-(buttonSize/2)};
        Rectangle spButtonClickbox = {spButton.x, spButton.y, buttonSize, buttonSize};

        Vector2 mpButton = {windowCenter + buttonMargin, boardWH/2-(buttonSize/2)};
        Rectangle mpButtonClickbox = {mpButton.x, mpButton.y, buttonSize, buttonSize};

        Game() {}
        // ~Game() {
        //     UnloadTexture(sprites);
        //     UnloadImage(spritesheet);
        // }
        void draw();
        void update();
        void drawSP();
        void drawMP();
        void drawSelectColor();
};

void Game::draw() {
    if(!selectColor) {
        DrawRectangle(0, 0, (SQUARESIZE*BOARDSIZE) + UIWIDTH, boardWH, BLACK);
        float offset = MeasureText(playerCount, playerCountFont);
        DrawText(playerCount, windowCenter-(offset/2), 128, playerCountFont, WHITE);

        drawSP();
        drawMP();
    } else {
        drawSelectColor();
    }

};

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

void Game::drawSelectColor() {
    float offset = MeasureText(playerColour, playerCountFont);
    DrawText(playerColour, windowCenter-(offset/2), 128, playerCountFont, WHITE);
    DrawRectangle(spButton.x, spButton.y, buttonSize, buttonSize, WHITE);
    Rectangle sprite = {0, 0, 512, 512};
    DrawTextureRec(sprites, sprite, Vector2{spButton.x, spButton.y}, WHITE);
    DrawRectangle(mpButton.x, mpButton.y, buttonSize, buttonSize, WHITE);
    sprite = {0, 512, 512, 512};
    DrawTextureRec(sprites, sprite, Vector2{mpButton.x, mpButton.y}, WHITE);
    DrawRectangle(windowCenter - 128, boardWH - 128, 256, 64, BLUE);
}

void Game::update() {
    Vector2 pos = GetMousePosition();
    if(!selectColor) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(CheckCollisionPointRec(pos, spButtonClickbox)) {
                mode = "sp";
                std::cout << "THE MODE IS: " << mode << std::endl;
            }
            if(CheckCollisionPointRec(pos, mpButtonClickbox)) {
                mode = "mp";
                selectColor = true;
                std::cout << "THE MODE IS: " << mode << std::endl;
            }
        }
    } else if(selectColor) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(CheckCollisionPointRec(pos, spButtonClickbox)) {
                std::cout << "SELECTED COLOR GREEN " << std::endl;
                selectedColor = "white";
                start = true;
                reset = true;
            }
            if(CheckCollisionPointRec(pos, mpButtonClickbox)) {
                selectedColor = "black";
                start = true;
                std::cout << "SELECTED COLOR RED "  << std::endl;
            }
        }
    }
};

class Command {
    public:
        virtual ~Command() {}
        virtual void execute() = 0;
        virtual void undo() = 0;
};

class MovePieceCommand : public Command {
    public:
    MovePieceCommand(Player *_player, Board*_board) :
        prev(_player->start), next(_player->end), board(_board)
    {
        Piece piecePrev = board->getPiece(prev);
        Piece pieceNext = board->getPiece(next);
        prevPieceName = piecePrev.name;
        nextPieceName = pieceNext.name;
        pColor = _player->color;
    }
    ~MovePieceCommand() {}
    void execute() {
        // if player has selected own piece
        char pSelectedColor = prevPieceName[0];
        if (pColor == pSelectedColor) {
            if(prevPieceName.substr(1, prevPieceName.length()) == "king") {
                if(pColor == 'b') {
                    board->bkingPosition = next;
                }
                if(pColor == 'w') {
                    board->wkingPosition = next;
                }
            }
            board->move(prev, next, nextPieceName, prevPieceName);
        } else {printf("NOT YOUR PIECE");}
    }

    void undo() {
        board->undo(prev, next, nextPieceName, prevPieceName);
        if(prevPieceName.substr(1, prevPieceName.length()) == "king") {
            if(pColor == 'b') {
                board->bkingPosition = prev;
            }
            if(pColor == 'w') {
                board->wkingPosition = prev;
            }
        }
    }


    private:
        std::string prevPieceName;
        std::vector<int> prev;
        std::vector<int> next;
        Board* board;
        char pColor;
        std::string nextPieceName;
};


class InputHandler {
    public:
    // takes in queue of pointers to commands to create NEW commands
    void handleInput(std::queue<Command*> &cmdQueue, Player*player, Board *board, Player *other);

    // has selected start ? -> create command

    // no selected ? -> set selected
};

// each player has selected property? if selected create movement command

void InputHandler::handleInput(std::queue<Command*> &cmdQueue, Player*player, Board *board, Player *other) {
    Vector2 mousePos = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // find the square position in array with mathhh
        int x = mousePos.x/ SQUARESIZE;
        int y = mousePos.y/SQUARESIZE;
        std::vector<int> tileCoordinates = {x,y};
        std::cout << "you clicked tile x: " << tileCoordinates[0] << "and y: " << tileCoordinates[1] << std::endl;

        if(player->selecting) {
            Board derefBoard = *board;
            Piece selectedP = derefBoard.getPiece(tileCoordinates);
            // only select own pieces
            if(player->color == selectedP.name[0]) {
                player->select(tileCoordinates);
                // playable moves my piece selected
                board->highlightTile(player->start, true);
                board->determineMoves(player->start, selectedP.name.substr(1, selectedP.name.length()-1), board->highlights, board->board);
            }
        } else {
            player->setDestination(tileCoordinates);

            // DETERMINING MOVES
            Board tempBoard;
            int potentialMoves[BOARDSIZE][BOARDSIZE];
            copyBoard(tempBoard.board, board->board);
            // start with empty potential moves
            zeroIntArray(potentialMoves);
            // the potential resulting board from move
            tempBoard.board[player->start[1]][player->start[0]].piece = "none";
            tempBoard.board[player->end[1]][player->end[0]].piece = board->board[player->start[1]][player->start[0]].piece;
            std::vector<int> wKingPos = tempBoard.findKing('w', tempBoard.board);
            std::vector<int> bKingPos = tempBoard.findKing('b', tempBoard.board);

            // check if opponent has any move that could put in check
            if(player->color == 'b') {
                for(int i = 0; i<BOARDSIZE; i++) {
                    for(int j = 0; j<BOARDSIZE; j++) {
                        // check OPPOSING MOVES for EACH piece
                        std::string currPieceName = board->board[i][j].piece;
                        std::string truePieceName = currPieceName.substr(1, currPieceName.length()-1);
                        if(currPieceName[0]=='w') {
                            // remember j = x, i = y
                            std::vector<int> sel = {j, i};
                            tempBoard.determineMoves(sel, truePieceName, potentialMoves, tempBoard.board);
                        }
                    }
                }
            } else if(player->color == 'w') {
                for(int i = 0; i<BOARDSIZE; i++) {
                    for(int j = 0; j<BOARDSIZE; j++) {
                        // check OPPOSING MOVES for EACH piece
                        std::string currPieceName = board->board[i][j].piece;
                        std::string truePieceName = currPieceName.substr(1, currPieceName.length()-1);
                        if(currPieceName[0]=='b') {
                            // remember j = x, i = y
                            std::vector<int> sel = {j, i}; 
                            tempBoard.determineMoves(sel, truePieceName, potentialMoves, tempBoard.board);
                        }
                    }
                }
            }

            std::cout << "x: " << wKingPos[0] << " y: "<< wKingPos[1] << std::endl;
            std::cout << "x: " << bKingPos[0] << " y: "<< bKingPos[1] << std::endl;

            std::cout << "**OPPOSING MOVES**" << std::endl;
            visualizeArray(potentialMoves);
            std::cout << player->color << std::endl;
            if(player->color == 'w' && potentialMoves[wKingPos[1]][wKingPos[0]] == 2) {
                std::cout << "white is in check, protect it!" << std::endl;
            } else if(player->color == 'b' && potentialMoves[bKingPos[1]][bKingPos[0]] == 2) {
                std::cout << "black is in check, protect it!" << std::endl;
            } else

            if(board->highlights[player->end[1]][player->end[0]] != 0) {
                cmdQueue.push(new MovePieceCommand(player, board));
                player->selecting = true;
                player->turn = false;
                other->turn = true;
            }
            player->selecting = true;
            board->highlightTile(player->start, false);
            board->clearHighlights();
        }
    }
};


void freeCommands(std::stack<Command*> &undoStack, std::queue<Command*> &cmdQueue) {
    // must also removes all the pointers to objects after deleting with pop
    while(!undoStack.empty()) {
        delete undoStack.top();
        undoStack.pop();
    }
    while(!cmdQueue.empty()) {
        delete cmdQueue.front();
        cmdQueue.pop();
    }
}


int main () {
    InitWindow(SQUARESIZE*BOARDSIZE + UIWIDTH, SQUARESIZE*BOARDSIZE, "Chess Command Pattern");
    SetWindowPosition(700,0);
    SetTargetFPS(60);
    std::queue<Command*> cmdQueue;
    std::stack<Command*> undoStack;
    Game game;
    Player *black = new Player(false, 'b');
    Player *white = new Player(true, 'w');
    InputHandler ih;
    UI ui;
    Board board;
    while(!WindowShouldClose()) {
        BeginDrawing();
        // draw the menu
        ClearBackground(BLACK);
        if(game.start == false) {
            game.update();
            game.draw();
        } 
        else {
            // play the board
            if(reset) {
                board.frontColor = game.selectedColor;
                board.resetBoard();
                reset = false;
            }
            if(black->turn) { ih.handleInput(cmdQueue, black, &board, white); }
            else if(white->turn) ih.handleInput(cmdQueue, white, &board, black);
            while (!cmdQueue.empty()) {
                Command* command = cmdQueue.front();
                command->execute();
                cmdQueue.pop();
                undoStack.push(command);
                std::cout << "WEVE MOVED THE BLACK KING TO: " << board.bkingPosition[0] << " " << board.bkingPosition[1] <<std::endl;
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
            board.draw();
            ui.draw(*white, *black, board);
        }
        EndDrawing();
    }
    freeCommands(undoStack, cmdQueue);
    delete black;
    delete white;
    return 0;
}