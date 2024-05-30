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
        prev(_player->start), next(_player->end), board(_board), player(_player)
    {
        prevPieceName = board->getPiece(prev);
        nextPieceName = board->getPiece(next);
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
            player->turnCount +=1;
        } else {printf("NOT YOUR PIECE");}
    }

    void undo() {
        board->undo(prev, next, nextPieceName, prevPieceName);
        player->turnCount -=1;
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
        xy prev;
        xy next;
        Board* board;
        char pColor;
        std::string nextPieceName;
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