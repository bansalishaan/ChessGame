#ifndef BOARD
#define BOARD
#include "Display.h"
#include <iostream>
#include <map>
#include <vector>
#include <memory>
using namespace std;

class Board {

    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteLRookMoved = false;
    bool whiteRRookMoved = false;
    bool blackLRookMoved = false;
    bool blackRRookMoved = false;

    map<int, char> whitePieces;
    map<int, char> blackPieces;
    vector<int> enPassantPawns;

    unique_ptr<TextDisplay> tDisplay;
    unique_ptr<GraphicsDisplay> gDisplay;
    friend class Bot1;
    friend class Bot2;
    friend class Bot3;
    friend class Bot4;

    bool movePutsKingInCheck(int pieceLoc, int moveLoc, int col,
        map<int, char> *ownPieces, map<int, char> *oppPieces, 
        bool dontMove = false);

    public:
        Board(int boardType);
        bool init(bool normal = true, char piece='\0', int location = 0, 
            bool remove = false);
        bool validInit();
        bool makeMove(vector<int> move, int col);
        bool noMoves(int col);
        bool inCheck(int col, int move = -1);
        void display();
};

#endif
