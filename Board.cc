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
                      << location % 10 + 1 << "!\n";
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

bool Board::makeMove(vector<int> move, int col) {
    map<int,char> *ownPieces = nullptr;
    map<int,char> *oppPieces = nullptr;

    // Checking if there's a piece on that square
    if(col == 0 && whitePieces.count(move.at(0)) == 1) {
        ownPieces = &whitePieces;
        oppPieces = &blackPieces;
    }
    else if(col == 1 && blackPieces.count(move.at(0)) == 1) {
        ownPieces = &blackPieces;
        oppPieces = &whitePieces;
    }
    else if(whitePieces.count(move.at(0)) == 1
            || blackPieces.count(move.at(0)) == 1) {
        char loc = 'a' + move.at(0) / 10;
        std::cout << "Other side's piece on " << loc << move.at(0) % 10
                  << ". Try a different move.\n";
        return false;
    }
    else {
        char loc = 'a' + move.at(0) / 10;
        std::cout << "There is no piece at " << loc << move.at(0) % 10
                  << ". Try a different move.\n";
        return false;
    }

    int currLetter = move.at(0) / 10;
    int currNum = move.at(0) % 10;
    int moveLetter = move.at(1) / 10;
    int moveNum = move.at(1) % 10;

    // Piece is a white pawn
    if((*ownPieces)[move.at(0)] == 'P') {
        // Pawn moves 2 squares forward
        if(currLetter == moveLetter && currNum == moveNum - 2
           && currNum == 1 && ownPieces->count(move.at(1) - 1) == 0
                               && ownPieces->count(move.at(1)) == 0
                               && oppPieces->count(move.at(1) - 1) == 0
                               && oppPieces->count(move.at(1)) == 0) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'P';
            // Checks if moving pawn puts King in check
            if(inCheck(0)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'P';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
           if(currLetter != 0 && oppPieces->count(move.at(1) - 10) == 1
              && (*oppPieces)[move.at(1) - 10] == 'p') {
                enPassantPawns.emplace_back(move.at(1));
              }
            if(currLetter != 7 && oppPieces->count(move.at(1) + 10) == 1
              && (*oppPieces)[move.at(1) + 10] == 'p') 
              enPassantPawns.emplace_back(move.at(1));
        }
        // Pawn moves 1 square forward and isn't going to be promoted
        else if(currLetter == moveLetter && currNum == moveNum - 1
                && moveNum != 7 && ownPieces->count(move.at(1)) == 0
                                && oppPieces->count(move.at(1)) == 0) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'P';
            // Checks if moving pawn puts King in check
            if(inCheck(0)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'P';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn moves 1 square forward to be promoted
        else if(currLetter == moveLetter && currNum == moveNum - 1
                                && ownPieces->count(move.at(1)) == 0
                                && oppPieces->count(move.at(1)) == 0) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = move.at(2);
            // Checks if moving pawn puts King in check
            if(inCheck(0)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'P';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn moves diagonally one square and takes a piece and doesn't
        // get promoted
        else if((currLetter == moveLetter + 1 
                 || currLetter == moveLetter - 1) 
                                && moveNum != 7 && currNum == moveNum - 1 
                                && oppPieces->count(move.at(1)) == 1) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'P';
            char oppPiece = (*oppPieces)[move.at(1)];
            oppPieces->erase(move.at(1));
            // Checks if moving pawn puts King in check
            if(inCheck(0)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'P';
                (*oppPieces)[move.at(1)] = oppPiece;
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn moves diagonally one square and takes a piece and gets promoted
        else if((currLetter == moveLetter + 1 
                 || currLetter == moveLetter - 1) 
                                && currNum == moveNum - 1 
                                && oppPieces->count(move.at(1)) == 1) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = move.at(2);
            char oppPiece = (*oppPieces)[move.at(1)];
            oppPieces->erase(move.at(1));
            // Checks if moving pawn puts King in check
            if(inCheck(0)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'P';
                (*oppPieces)[move.at(1)] = oppPiece;
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn performs en passant
        else if((currLetter == moveLetter + 1 
                 || currLetter == moveLetter - 1) 
                                && moveNum == 5 && currNum == moveNum - 1 
                                && oppPieces->count(move.at(1)) == 0
                                && enPassantPawns.at(0) == move.at(1) - 1) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'P';
            oppPieces->erase(enPassantPawns.at(0));
            // Checks if moving pawn puts King in check
            if(inCheck(0)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'P';
                (*oppPieces)[move.at(1) - 1] = 'p';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // There is no more valid moves
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
        return true;
    }
    // Piece is a black pawn
    else if((*ownPieces)[move.at(0)] == 'p') {
        // Pawn moves 2 squares forward
        if(currLetter == moveLetter && currNum == moveNum + 2
           && currNum == 6 && ownPieces->count(move.at(1) + 1) == 0
                               && ownPieces->count(move.at(1)) == 0
                               && oppPieces->count(move.at(1) + 1) == 0
                               && oppPieces->count(move.at(1)) == 0) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'p';
            // Checks if moving pawn puts King in check
            if(inCheck(1)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'p';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
           if(currLetter != 0 && oppPieces->count(move.at(1) - 10) == 1
              && (*oppPieces)[move.at(1) - 10] == 'P') {
                enPassantPawns.emplace_back(move.at(1));
              }
            if(currLetter != 7 && oppPieces->count(move.at(1) + 10) == 1
              && (*oppPieces)[move.at(1) + 10] == 'P') 
              enPassantPawns.emplace_back(move.at(1));
        }
        // Pawn moves 1 square forward and isn't going to be promoted
        else if(currLetter == moveLetter && currNum == moveNum + 1
                && moveNum != 0 && ownPieces->count(move.at(1)) == 0
                                && oppPieces->count(move.at(1)) == 0) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'p';
            // Checks if moving pawn puts King in check
            if(inCheck(1)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'p';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn moves 1 square forward to be promoted
        else if(currLetter == moveLetter && currNum == moveNum + 1
                                && ownPieces->count(move.at(1)) == 0
                                && oppPieces->count(move.at(1)) == 0) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = move.at(2);
            // Checks if moving pawn puts King in check
            if(inCheck(1)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'p';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn moves diagonally one square and takes a piece and doesn't
        // get promoted
        else if((currLetter == moveLetter + 1 
                 || currLetter == moveLetter - 1) 
                                && moveNum != 0 && currNum == moveNum + 1
                                && oppPieces->count(move.at(1)) == 1) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'p';
            char oppPiece = (*oppPieces)[move.at(1)];
            oppPieces->erase(move.at(1));
            // Checks if moving pawn puts King in check
            if(inCheck(1)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'p';
                (*oppPieces)[move.at(1)] = oppPiece;
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn moves diagonally one square and takes a piece and gets promoted
        else if((currLetter == moveLetter + 1 
                 || currLetter == moveLetter - 1) 
                                && currNum == moveNum + 1 
                                && oppPieces->count(move.at(1)) == 1) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = move.at(2);
            char oppPiece = (*oppPieces)[move.at(1)];
            oppPieces->erase(move.at(1));
            // Checks if moving pawn puts King in check
            if(inCheck(1)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'p';
                (*oppPieces)[move.at(1)] = oppPiece;
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // Pawn performs en passant
        else if((currLetter == moveLetter + 1 
                 || currLetter == moveLetter - 1) 
                                && moveNum == 2 && currNum == moveNum + 1 
                                && oppPieces->count(move.at(1)) == 0
                                && enPassantPawns.at(0) == move.at(1) + 1) {
            ownPieces->erase(move.at(0));
            (*ownPieces)[move.at(1)] = 'p';
            oppPieces->erase(enPassantPawns.at(0));
            // Checks if moving pawn puts King in check
            if(inCheck(1)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = 'p';
                (*oppPieces)[move.at(1) - 1] = 'P';
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
           }
        }
        // There is no more valid moves
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
        return true;
    }
    // Piece is white or black rook
    else if((*ownPieces)[move.at(0)] == 'R' 
             || (*ownPieces)[move.at(0)] == 'r') {
        // Rook is moving horizontally
        if(currNum == moveNum) {
            int i = currLetter > moveLetter ? move.at(1) + 10 : move.at(0) + 10;
            int j = currLetter > moveLetter ? move.at(0) : move.at(1);
            for(int i; i < j; i += 10) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: There's a piece in between. ";
                    std::cout << "Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }

            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        // Rook is moving vertically
        else if(currLetter == moveLetter) {
            int i = currNum > moveNum ? move.at(1) + 1 : move.at(0) + 1;
            int j = currNum > moveNum ? move.at(0) : move.at(1);
            for(i; i < j; ++i) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }

            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
        return true;
    }
    else if((*ownPieces)[move.at(0)] == 'N'
             || (*ownPieces)[move.at(0)] == 'n') {
        if((((currNum == moveNum + 1 || currNum == moveNum - 1) && 
            (currLetter == moveLetter + 2 || currLetter == moveLetter - 2)) 
            || ((currNum == moveNum + 2 || currNum == moveNum - 2) && 
            (currLetter == moveLetter + 1 || currLetter == moveLetter - 1)))
            && ownPieces->count(move.at(1)) == 0) {
            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
        return true;
    }
    else if((*ownPieces)[move.at(0)] == 'B'
             || (*ownPieces)[move.at(0)] == 'b') {
        if(moveNum - currNum == moveLetter - currLetter) { // right diagonal
            int i = moveNum - currNum < 0 ? move.at(1) + 11 : move.at(0) + 11;
            int j = moveNum - currNum < 0 ? move.at(0) : move.at(1);
            for(i; i < j; i += 11) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
            }
            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else if(moveNum - currNum == currLetter - moveLetter) { // left diagonal
            int i = moveNum - currNum < 0 ? move.at(0) + 9 : move.at(1) + 9;
            int j = moveNum - currNum < 0 ? move.at(1) : move.at(0);
            for(i; i < j; i += 9) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
            }
            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
        return true;
    }
    else if((*ownPieces)[move.at(0)] == 'Q'
             || (*ownPieces)[move.at(0)] == 'q') {
        // Queen is moving horizontally
        if(currNum == moveNum) {
            int i = currLetter > moveLetter ? move.at(1) + 10 : move.at(0) + 10;
            int j = currLetter > moveLetter ? move.at(0) : move.at(1);
            for(int i; i < j; i += 10) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: There's a piece in between. ";
                    std::cout << "Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }

            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        // Queen is moving vertically
        else if(currLetter == moveLetter) {
            int i = currNum > moveNum ? move.at(1) + 1 : move.at(0) + 1;
            int j = currNum > moveNum ? move.at(0) : move.at(1);
            for(i; i < j; ++i) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }

            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else if(moveNum - currNum == moveLetter - currLetter) { // right diagonal
            int i = moveNum - currNum < 0 ? move.at(1) + 11 : move.at(0) + 11;
            int j = moveNum - currNum < 0 ? move.at(0) : move.at(1);
            for(i; i < j; i += 11) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
            }
            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else if(moveNum - currNum == currLetter - moveLetter) { // left diagonal
            int i = moveNum - currNum < 0 ? move.at(0) + 9 : move.at(1) + 9;
            int j = moveNum - currNum < 0 ? move.at(1) : move.at(0);
            for(i; i < j; i += 9) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
            }
            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
        return true;
    }
    else if((*ownPieces)[move.at(0)] == 'K'
             || (*ownPieces)[move.at(0)] == 'k') {
        if((currNum == moveNum && 
          (currLetter == moveLetter - 1 || currLetter == moveLetter + 1))
          || (currLetter == moveLetter && 
          (currNum == moveNum - 1 || currNum == moveNum + 1))
          || ((currNum == moveNum - 1 || currNum == moveNum + 1) &&
          (currLetter == moveLetter - 1 || currLetter == moveLetter + 1))) {
            
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "INVALID MOVE: Try a different move.\n";
                    return false;
            }

            char oppPiece = '\0';
            if(oppPieces->count(move.at(1)) == 1) {
                oppPiece = (*oppPieces)[move.at(1)];
                oppPieces->erase(move.at(1));
            }
            (*ownPieces)[move.at(1)] = (*ownPieces)[move.at(0)];
            ownPieces->erase(move.at(0));
            // Checks if moving rook puts King in check
            if(inCheck(col)) {
                if(oppPiece != '\0') (*oppPieces)[move.at(1)] = oppPiece;
                ownPieces->erase(move.at(1));
                (*ownPieces)[move.at(0)] = (*ownPieces)[move.at(0)];
                std::cout << "INVALID MOVE: King is in check! "
                          << "Try a different move.\n";
                return false;
            }
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
        return true;
    }
}

bool Board::noMoves() {return true;}

bool Board::inCheck(int col) {return false;}

void Board::display() {d->display();}