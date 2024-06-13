#pragma once
#include "player.h"
#include "util.h"

class Command {
    public:
        virtual ~Command() {}
        virtual void execute() = 0;
        virtual void undo() = 0;
};

class MovePieceCommand : public Command {
    public:
    MovePieceCommand(Player *_player, Board*_board) :
        start(_player->start), end(_player->end), board(_board), player(_player)
    {
        startPiece = board->getPiece(start);
        
        //determine if upgrading pawn
        std::string truePiece = startPiece.substr(1, startPiece.length()-1);
        if ((end.y == 0 || end.y == 7) && truePiece == "pawn") {
        // Upgrade the pawn to a queen by keeping the prefix (presumably the color)
            newPiece = startPiece[0] + std::string("queen");
        } else {
            newPiece = startPiece;
        }



        endPiece = board->getPiece(end);
        pColor = _player->color;
    }
    ~MovePieceCommand() {}
    void execute() {
        // if player has selected own piece
        char pSelectedColor = newPiece[0];
        if (pColor == pSelectedColor) {
            if(newPiece.substr(1, newPiece.length()) == "king") {
                if(pColor == 'b') {
                    board->bkingPosition = end;
                }
                if(pColor == 'w') {
                    board->wkingPosition = end;
                }
            }
            board->move(start, end, endPiece, newPiece);
            player->turnCount +=1;
        } else {printf("NOT YOUR PIECE");}
    }

    void undo() {
        board->undo(start, end, endPiece, startPiece);
        player->turnCount -=1;
        if(startPiece.substr(1, startPiece.length()) == "king") {
            if(pColor == 'b') {
                board->bkingPosition = start;
            }
            if(pColor == 'w') {
                board->wkingPosition = start;
            }
        }
    }


    private:

        std::string startPiece;
        std::string endPiece;
        std::string newPiece;
        xy start;
        xy end;
        Board* board;
        char pColor;
        Player* player;
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