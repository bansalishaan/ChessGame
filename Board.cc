#include "Board.h"

Board::Display::Display(map<int, char> *whitePieces, map<int, char> *blackPieces): whitePieces{whitePieces}, blackPieces{blackPieces} {}

Board::Display::~Display() {}

Board::TextDisplay::TextDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces): Display{whitePieces, blackPieces} {}

Board::GraphicsDisplay::GraphicsDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces): Display{whitePieces, blackPieces} {}

void Board::TextDisplay::display() {
    for(int i = 7; i >= 0; --i) {
        std::cout << i+1 << " ";
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
    std::cout << "\n  abcdefgh\n";
}

void Board::GraphicsDisplay::display() { // Make one for Graphics
}

Board::Board(int boardType): whitePieces{}, blackPieces{}, enPassantPawns{}, d{make_unique<Board::TextDisplay> (&whitePieces, &blackPieces)} {}

bool Board::init(bool normal, char piece, int location, bool remove) {
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
    else if(remove) {
        whitePieces.erase(location);
        blackPieces.erase(location);
    }
    else {
        if(blackPieces.count(location) != 1 && (piece == 'p' || piece == 'r' || 
            piece == 'n' || piece == 'b' || piece == 'q' || piece == 'k')) {
                blackPieces[location] = piece;
        }
        else if(whitePieces.count(location) != 1 && (piece == 'P' || 
                piece == 'R' || piece == 'N' || piece == 'B' || piece == 'Q'
                || piece == 'K')) {
                    whitePieces[location] = piece;
        }
        else if(blackPieces.count(location) == 1
                || whitePieces.count(location) == 1) {
            char loc = 'a' + location / 10;
            std::cout << "There already exists a piece at " << loc 
                      << location % 10 << "!\n";
            return false;
        }
        else {
            std::cout << "Invalid Piece!\n";
            return false;
        }
    }
    return true;
}

bool Board::validInit() {
    vector<int> invalidPawns,  whiteKings, blackKings;

    for(auto &piece : blackPieces) {
        if(piece.second == 'p' && piece.first % 10 == 0)
            invalidPawns.emplace_back(piece.first);
        else if (piece.second == 'k') blackKings.emplace_back(piece.first);
    }
    for(auto &piece : whitePieces) {
        if(piece.second == 'P' && piece.first % 10 == 7) 
            invalidPawns.emplace_back(piece.first);
        else if (piece.second == 'K') whiteKings.emplace_back(piece.first);
    }
    if(invalidPawns.size() == 0 && whiteKings.size() == 1 &&
       blackKings.size() == 1) return true;

    if(invalidPawns.size() == 1) std::cout << "ERROR: Pawn ";
    else if(invalidPawns.size() > 1) std::cout << "ERROR: Pawns ";
    
    if(invalidPawns.size() > 0) {
        char loc = 'a' + invalidPawns.at(0) / 10;
        std::cout << loc << invalidPawns.at(0) % 10;
        for(int i = 1; i < invalidPawns.size(); ++i) {
            loc = 'a' + invalidPawns.at(i) / 10;
            std::cout << ", " << loc << invalidPawns.at(i) % 10;
        }
    }

    if(invalidPawns.size() == 1) std::cout << " is on the last square!"
        << " Fix this problem to continue.\n";
    else if(invalidPawns.size() >= 1) std::cout << " are on the last square!"
        << " Fix this problem to continue.\n";
    
    if(whiteKings.size() == 0) std::cout << "ERROR: There are no white kings!"
        << " Fix this problem to continue.\n";
    else if(whiteKings.size() > 1) {
        std::cout << "ERROR: Multiple white kings on ";

        char loc = 'a' + whiteKings.at(0) / 10;
        std::cout << loc << whiteKings.at(0) % 10;
        for(int i = 1; i < whiteKings.size(); ++i) {
            loc = 'a' + whiteKings.at(i) / 10;
            std::cout << ", " << loc << whiteKings.at(i) % 10;
        }
        std::cout << ". Fix this problem to continue.\n";
    }


    if(blackKings.size() == 0) std::cout << "ERROR: There are no black kings!"
        << " Fix this problem to continue.\n";
    else if(blackKings.size() > 1) {
        std::cout << "ERROR: Multiple black kings on ";

        char loc = 'a' + blackKings.at(0) / 10;
        std::cout << loc << blackKings.at(0) % 10;
        for(int i = 1; i < blackKings.size(); ++i) {
            loc = 'a' + blackKings.at(i) / 10;
            std::cout << ", " << loc << blackKings.at(i) % 10;
        }
        std::cout << ". Fix this problem to continue.\n";
    }

    return false;
}

bool Board::makeMove(vector<vector<int>> move, int col) {return true;}

bool Board::noMoves() {return true;}

bool Board::inCheck() {return true;}

void Board::display() {d->display();}