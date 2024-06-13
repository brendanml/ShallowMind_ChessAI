#pragma once
#include <queue>
#include "command.h"
#include <iostream>
#include "board.h"
#include <vector>
#include "util.h"

class InputHandler {
    public:
    // takes in queue of pointers to commands to create NEW commands
    void handleInput(bool &start, std::queue<Command*> &cmdQueue, Player*player, Board *board, Player *other);
    void handleAI(bool &gameover, std::queue<Command*> &cmdQueue, Player*player, Board *board, Player *other);
    void aiMove(bool &gameover, Player* ai, Board* board, std::queue<Command*> &cmdQueue, Player* other);
    void handlePerson(bool &gameover, std::queue<Command*> &cmdQueue, Player*player, Board *board, Player *other); //handles a persons turn
    void personSelect(bool &gameover, Player *player, xy tileCoordinates, Board *board); // person player attempt to select a piece
    bool validateMove(Player *player, Board* board, Player* other);\
    bool checkCheckmate(bool &gameover, Player* player, Board* board, Player* other);
};

void InputHandler::handleInput(bool &gameover, std::queue<Command*> &cmdQueue, Player*player, Board *board, Player *other) {
    if(player->turnStarting) {
        if(board->checkCheck(player->color)) {
            player->inCheck = true;
            std::cout << "PLAYER IS IN CHECK" << std::endl;
        }
        if(player->isAI) {
            player->timer.reset();
            player->waitPeriod = random_integer(1, 3);
            player->turnStarting = false;
        }
        else if(!player->isAI) {
            Player* robot = new Player(true, player->color);
            robot->isAI = true;
            bool gameDone = checkCheckmate(gameover, robot, board, other);
            if(gameDone) {
                std::cout << "**************THE GAME IS OVER*************" << std::endl;
                if(player->color == 'w') board->whiteScore = 0;
                if(player->color == 'b') board->blackScore = 0;
                board->computeScore();
                if(player->inCheck) {
                    other->wonGame = true;
                    player->lostGame = true;
                }
            }
        }
        player->turnStarting = false;
    }
    if(player->isAI == false) { handlePerson(gameover, cmdQueue, player, board, other); }
    else if(player->isAI == true ) {handleAI(gameover, cmdQueue, player, board, other);}
};

void InputHandler::handleAI(bool &gameover, std::queue<Command*> &cmdQueue, Player*ai, Board *board, Player *other) {
    if(ai->selecting && ai->timer.hasTimePassed(ai->waitPeriod / ai->speedup)) {
        aiMove(gameover, ai, board, cmdQueue, other);
    }
};

bool InputHandler::checkCheckmate(bool &gameover, Player* player, Board* board, Player* other) {
    Board derefBoard = *board;
    std::vector<Move> potentialMoves[BOARDSIZE][BOARDSIZE];
    board->determineAllPieces(player->color, derefBoard.board, potentialMoves);
    std::priority_queue<Element, std::vector<Element>, Compare> moves = derefBoard.rankMoves(player->color ,potentialMoves, derefBoard);
    int topMoveValue = 0;
    if(!moves.empty()) topMoveValue = moves.top().value;


    // count the # of top moves
    int topMoveCount = 0;
    std::vector<Element> topMoves;
    while(!moves.empty()) {
        Element curr = moves.top();
        if(curr.value < topMoveValue) break;
        topMoves.push_back(curr);
        topMoveCount ++;
        moves.pop();
    }

    // pick a random top move and remove it from top moves, while also choosing it as the preferred move
    int randInt = random_integer(0, topMoveCount-1);
    Element curr = topMoves[randInt];
    topMoves.erase(topMoves.begin() + randInt);

    // re insert remaining top moves
    while(!topMoves.empty()) {
        moves.push(topMoves.front());
        topMoves.erase(topMoves.begin());
    }

    // assign move start and end based on selected move
    player->select(curr.start);
    player->setDestination(curr.end);
    if(validateMove(player, board, other)) {
        std::cout << "MOVE HAS BEEN FOUND" << std::endl;
        return false;
    } else {
        bool validMove = false;
        while(!validMove && !moves.empty()) {
            Element curr = moves.top();
            player->select(curr.start);
            player->setDestination(curr.end);
            moves.pop();
            validMove = validateMove(player, board, other);
        } 
        if(validMove) {
            std::cout << "MOVE HAS BEEN FOUND" << std::endl;
            return false;
        } else{
            std::cout << "can't find a valid move" << std::endl;
            // CHANGE THE GAME STATE TO CHECKMATE
            gameover = true;
            return true;
        }
    }
}

void InputHandler::aiMove(bool &gameover, Player* ai, Board* board, std::queue<Command*> &cmdQueue, Player* other) {
    Board derefBoard = *board;
    std::vector<Move> potentialMoves[BOARDSIZE][BOARDSIZE];
    board->determineAllPieces(ai->color, derefBoard.board, potentialMoves);
    std::priority_queue<Element, std::vector<Element>, Compare> moves = derefBoard.rankMoves(ai->color ,potentialMoves, derefBoard);
    int topMoveValue = 0;
    if(!moves.empty()) topMoveValue = moves.top().value;


    // count the # of top moves
    int topMoveCount = 0;
    std::vector<Element> topMoves;
    while(!moves.empty()) {
        Element curr = moves.top();
        if(curr.value < topMoveValue) break;
        topMoves.push_back(curr);
        topMoveCount ++;
        moves.pop();
    }

    // pick a random top move and remove it from top moves, while also choosing it as the preferred move
    int randInt = random_integer(0, topMoveCount-1);
    Element curr = topMoves[randInt];
    topMoves.erase(topMoves.begin() + randInt);

    // re insert remaining top moves
    while(!topMoves.empty()) {
        moves.push(topMoves.front());
        topMoves.erase(topMoves.begin());
    }

    // assign move start and end based on selected move
    ai->select(curr.start);
    ai->setDestination(curr.end);
    if(validateMove(ai, board, other)) {
        std::cout << "MOVE HAS BEEN FOUND" << std::endl;
        cmdQueue.push(new MovePieceCommand(ai, board));
        ai->selecting = true;
        ai->turn = false;
        other->turn = true;
    } else {
        bool validMove = false;
        while(!validMove && !moves.empty()) {
            Element curr = moves.top();
            ai->select(curr.start);
            ai->setDestination(curr.end);
            moves.pop();
            validMove = validateMove(ai, board, other);
        } 
        if(validMove) {
            std::cout << "MOVE HAS BEEN FOUND" << std::endl;
            cmdQueue.push(new MovePieceCommand(ai, board));
            ai->selecting = true;
            ai->turn = false;
            other->turn = true;
        } else{
            std::cout << "can't find a valid move" << std::endl;
            // CHANGE THE GAME STATE TO CHECKMATE
            if(ai->inCheck) {
                other->wonGame = true;
                ai->lostGame = true;
            }
            gameover = true;
        }
    }
    ai->turnStarting = true;
}

void InputHandler::handlePerson(bool &start, std::queue<Command*> &cmdQueue, Player*player, Board *board, Player *other) {
    Vector2 mousePos = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePos.x <= BOARDSIZE*SQUARESIZE && mousePos.y <= BOARDSIZE*SQUARESIZE) {
        // find the square position in array with mathhh
        xy tileCoordinates = {(int)mousePos.x/ SQUARESIZE,(int)mousePos.y/SQUARESIZE};

        if(player->selecting) {
            personSelect(start, player, tileCoordinates, board);
        } else if(tileCoordinates.x >= 0 && tileCoordinates.y >= 0 && tileCoordinates.x <= 7 && tileCoordinates.y <= 7){
            std::cout << "were*************************in here" << std::endl;
            player->setDestination(tileCoordinates);
            if(validateMove(player, board, other)) {
                cmdQueue.push(new MovePieceCommand(player, board)); 
                player->selecting = true;
                player->turnStarting = true;
                player->turn = false;
                other->turn = true;
            }
            // player has ATTEMPTED to move, clear things
            player->selecting = true;
            board->highlightTile(player->start, false);
            board->clearHighlights();
        }
    }
};

void InputHandler::personSelect(bool &start, Player *player, xy tileCoordinates, Board *board) {
    Board derefBoard = *board;
    std::string selectedP = derefBoard.getPiece(tileCoordinates);
            // only select own pieces
    if(player->color == selectedP[0]) {
        player->select(tileCoordinates);
        // playable moves my piece selected
        board->highlightTile(player->start, true);
        board->determineMoves(player->start, selectedP.substr(1, selectedP.length()-1), board->highlights, board->board);
    }
}

bool InputHandler::validateMove(Player *player, Board* board, Player* other) {
    Board tempBoard("no-assets");
    std::vector<Move> potentialMoves[BOARDSIZE][BOARDSIZE];
    copyBoard(tempBoard.board, board->board);

    // the potential resulting board from move
    tempBoard.board[player->start.y][player->start.x].piece = "none";
    tempBoard.board[player->end.y][player->end.x].piece = board->board[player->start.y][player->start.x].piece;
    xy wKingPos = tempBoard.findKing('w', tempBoard.board);
    xy bKingPos = tempBoard.findKing('b', tempBoard.board);
    char currPlayer = player->color;

    // for checking if opponent has any move that could put in check
    if(currPlayer == 'b') { tempBoard.determineAllPieces('w', tempBoard.board, potentialMoves); }
    else if(currPlayer == 'w') tempBoard.determineAllPieces('b', tempBoard.board, potentialMoves);
    // visualizeArray(potentialMoves); //for varifying opponents potential moves


    if(currPlayer == 'w' && (!potentialMoves[wKingPos.y][wKingPos.x].empty() && potentialMoves[wKingPos.y][wKingPos.x].front().type == 2)) { 
        std::cout << "white is in check, protect it!" << std::endl; 
        return false;
    }
    else if(currPlayer == 'b' && (!potentialMoves[bKingPos.y][bKingPos.x].empty() && potentialMoves[bKingPos.y][bKingPos.x].front().type == 2)) {
        std::cout << "black is in check, protect it!" << std::endl;
        return false;
        // only commiting to creating movement commands if it doesn't put the player in check
    } else if(player->isAI || (!board->highlights[player->end.y][player->end.x].empty() && board->highlights[player->end.y][player->end.x].front().type != 0)) {
        return true;
    }
    else return false;
}