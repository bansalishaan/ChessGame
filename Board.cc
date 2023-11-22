#include "Board.h"

Board::Display::Display(map<int, char> *whitePieces, map<int, char> *blackPieces): whitePieces{whitePieces}, blackPieces{blackPieces} {}

Board::Display::~Display() {}

Board::TextDisplay::TextDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces): Display{whitePieces, blackPieces} {}

Board::GraphicsDisplay::GraphicsDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces): Display{whitePieces, blackPieces} {}

void Board::TextDisplay::display() {
    for(int i = 7; i >= 0; --i) {
        for(int j = 0; j < 8; ++j) {
            // Checks if white has a piece at that location and prints it if
            // true
            if(whitePieces->count(j*10 + i) != 0) std::cout << 
                                                  (*whitePieces)[j*10 + i];
            // Checks if black has a piece at that location and prints it if
            // true
            else if(blackPieces->count(j*10 + i) != 0) std::cout << 
                                                       (*blackPieces)[j*10 + i];
            // Square is empty
            else std::cout << "_";
        }
        std::cout << "\n";
    }
}

void Board::GraphicsDisplay::display() { // Make one for Graphics
}

Board::Board(int boardType): whitePieces{}, blackPieces{}, enPassantPawns{}, d{make_unique<Board::TextDisplay> (&whitePieces, &blackPieces)} {}

bool Board::init(bool normal, char piece, int location) {
    if(normal) {
        for(int i = 0; i < 8; ++i) {
            whitePieces[i*10 + 1] = 'P';
            blackPieces[i*10 + 6] = 'p';

            if(i == 0 || i == 7) {
                whitePieces[i*10] = 'R';
                blackPieces[i*10 + 7] = 'r';
            } 
            else if(i == 1 || i == 6) {
                whitePieces[i*10] = 'N';
                blackPieces[i*10 + 7] = 'n';
            } 
            else if(i == 2 || i == 5) {
                whitePieces[i*10] = 'B';
                blackPieces[i*10 + 7] = 'b';
            } 
            else if(i == 3) {
                whitePieces[i*10] = 'Q';
                blackPieces[i*10 + 7] = 'q';
            } 
            else if(i == 4) {
                whitePieces[i*10] = 'K';
                blackPieces[i*10 + 7] = 'k';
            }
        }
    }
    else {
        if(whitePieces.count(location) != 1 && blackPieces.count(location) != 1
            && piece >= 97 && piece <= 122) blackPieces[location] = piece;
        else if(whitePieces.count(location) != 1 && blackPieces.count(location) 
            != 1) whitePieces[location] = piece;
        else return false;
    }
    return true;
}

bool Board::makeMove(vector<vector<int>> move, int col) {return true;}

bool Board::noMoves() {return true;}

bool Board::inCheck() {return true;}

void Board::display() {d->display();}