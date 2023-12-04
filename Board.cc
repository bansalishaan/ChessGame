#include "Board.h"


Board::Board(int boardType): whitePieces{}, blackPieces{}, enPassantPawns{}, 
tDisplay{make_unique<TextDisplay> (&whitePieces, &blackPieces)}, 
gDisplay{make_unique<GraphicsDisplay> (&whitePieces, &blackPieces)} {}

bool Board::init(bool normal, char piece, int location, bool remove) {
    // Normal board setup
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
    // Player wants to remove a piece
    else if(remove) {
        whitePieces.erase(location);
        blackPieces.erase(location);
    }
    // Adding a piece to the board
    else {
        if(piece == 'p' || piece == 'r' || piece == 'n' || piece == 'b' || 
           piece == 'q' || piece == 'k') {
            if(whitePieces.count(location) == 1) whitePieces.erase(location);
            blackPieces[location] = piece;
        } else if(piece == 'P' || piece == 'R' || piece == 'N' || 
                  piece == 'B' || piece == 'Q' || piece == 'K') {
            if(blackPieces.count(location) == 1) blackPieces.erase(location);
            whitePieces[location] = piece;
        }
        else {
            std::cout << "Invalid Piece!\n";
            return false;
        }
    }
    return true;
}

bool Board::validInit() {
    vector<int> invalidPawns, whiteKings, blackKings;

    for(auto &piece : blackPieces) {
        if(piece.second == 'p' && (piece.first % 10 == 0 || 
                                   piece.first % 10 == 7))
            invalidPawns.emplace_back(piece.first); // Pawn on invalid row
        else if (piece.second == 'k') blackKings.emplace_back(piece.first);
        // Tracking location of black king(s) on the board
    }
    for(auto &piece : whitePieces) {
        if(piece.second == 'P' && (piece.first % 10 == 7 ||
                                   piece.first % 10 == 0)) 
            invalidPawns.emplace_back(piece.first); // Pawn on invalid row
        else if (piece.second == 'K') whiteKings.emplace_back(piece.first);
        // Tracking location of white king(s) on the board
    }
    // Valid board setup
    if(invalidPawns.size() == 0 && whiteKings.size() == 1 &&
       blackKings.size() == 1 && !inCheck(0) && !inCheck(1)) return true;


    // Invalid pawns and outputting which ones
    if(invalidPawns.size() == 1) std::cout << "ERROR: Pawn ";
    else if(invalidPawns.size() > 1) std::cout << "ERROR: Pawns ";
    
    if(invalidPawns.size() > 0) {
        char loc = 'a' + invalidPawns.at(0) / 10;
        std::cout << loc << invalidPawns.at(0) % 10 + 1;
        for(int i = 1; i < invalidPawns.size(); ++i) {
            loc = 'a' + invalidPawns.at(i) / 10;
            std::cout << ", " << loc << invalidPawns.at(i) % 10 + 1;
        }
    }

    if(invalidPawns.size() == 1) std::cout << " is on an invalid row!"
        << " Fix this problem to continue.\n";
    else if(invalidPawns.size() >= 1) std::cout << " are on an invalid row!"
        << " Fix this problem to continue.\n";


    // Invalid number of white kings
    if(whiteKings.size() == 0) std::cout << "ERROR: There are no white kings!"
        << " Fix this problem to continue.\n";
    else if(whiteKings.size() > 1) {
        std::cout << "ERROR: Multiple white kings on ";

        char loc = 'a' + whiteKings.at(0) / 10;
        std::cout << loc << whiteKings.at(0) % 10 + 1;
        for(int i = 1; i < whiteKings.size(); ++i) {
            loc = 'a' + whiteKings.at(i) / 10;
            std::cout << ", " << loc << whiteKings.at(i) % 10 + 1;
        }
        std::cout << ". Fix this problem to continue.\n";
    }

    // Invalid number of black kings
    if(blackKings.size() == 0) std::cout << "ERROR: There are no black kings!"
        << " Fix this problem to continue.\n";
    else if(blackKings.size() > 1) {
        std::cout << "ERROR: Multiple black kings on ";

        char loc = 'a' + blackKings.at(0) / 10;
        std::cout << loc << blackKings.at(0) % 10 + 1;
        for(int i = 1; i < blackKings.size(); ++i) {
            loc = 'a' + blackKings.at(i) / 10;
            std::cout << ", " << loc << blackKings.at(i) % 10 + 1;
        }
        std::cout << ". Fix this problem to continue.\n";
    }


    // White king is in check
    if(inCheck(0)) std::cout << "ERROR: White King is in check!"
        << " Fix this problem to continue.\n";
    
    // Black king is in check
    if(inCheck(1)) std::cout << "ERROR: Black King is in check!"
        << " Fix this problem to continue.\n";

    return false;
}

bool Board::movePutsKingInCheck(int pieceLoc, int moveLoc, int col,
        map<int, char> *ownPieces, map<int, char> *oppPieces, bool dontMove) {

    char oppPiece = '\0';
    if(oppPieces->count(moveLoc) == 1) {
        oppPiece = (*oppPieces)[moveLoc];
        oppPieces->erase(moveLoc);
    }
    (*ownPieces)[moveLoc] = (*ownPieces)[pieceLoc];
    ownPieces->erase(pieceLoc);
    // Checks if moving the piece puts King in check
    if(inCheck(col)) {
        if(oppPiece != '\0') (*oppPieces)[moveLoc] = oppPiece;
        (*ownPieces)[pieceLoc] = (*ownPieces)[moveLoc];
        ownPieces->erase(moveLoc);
        // std::cout << "INVALID MOVE: King is in check! "
        //             << "Try a different move.\n";
        return true;
    }
    if(dontMove) {
        if(oppPiece != '\0') (*oppPieces)[moveLoc] = oppPiece;
        (*ownPieces)[pieceLoc] = (*ownPieces)[moveLoc];
        ownPieces->erase(moveLoc);
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
                                && enPassantPawns.size() == 1
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
                                && enPassantPawns.size() == 1
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
    }
    // Piece is white or black rook
    else if((*ownPieces)[move.at(0)] == 'R' 
             || (*ownPieces)[move.at(0)] == 'r') {
        // Rook is moving horizontally
        if(currNum == moveNum) {
            int i = currLetter > moveLetter ? move.at(1) + 10 : move.at(0) + 10;
            int j = currLetter > moveLetter ? move.at(0) : move.at(1);
            for(i; i < j; i += 10) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "rh1INVALID MOVE: There's a piece in between. ";
                    std::cout << "Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << move.at(0) << " " << move.at(1) << "\n";
                std::cout << "rh2INVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }
            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;

            if(move.at(0) == 0) whiteLRookMoved = true;
            else if(move.at(0) == 70) whiteRRookMoved = true;
            else if(move.at(0) == 7) blackLRookMoved = true;
            else if(move.at(0) == 77) blackRRookMoved = true;
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
                std::cout << "rvINVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }

            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
            
            if(move.at(0) == 0) whiteLRookMoved = true;
            else if(move.at(0) == 70) whiteRRookMoved = true;
            else if(move.at(0) == 7) blackLRookMoved = true;
            else if(move.at(0) == 77) blackRRookMoved = true;
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
    }
    else if((*ownPieces)[move.at(0)] == 'N'
             || (*ownPieces)[move.at(0)] == 'n') {
        if((((currNum == moveNum + 1 || currNum == moveNum - 1) && 
            (currLetter == moveLetter + 2 || currLetter == moveLetter - 2)) 
            || ((currNum == moveNum + 2 || currNum == moveNum - 2) && 
            (currLetter == moveLetter + 1 || currLetter == moveLetter - 1)))
            && ownPieces->count(move.at(1)) == 0) {

            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
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
            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
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
            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
    }
    else if((*ownPieces)[move.at(0)] == 'Q'
             || (*ownPieces)[move.at(0)] == 'q') {
        // Queen is moving horizontally
        if(currNum == moveNum) {
            int i = currLetter > moveLetter ? move.at(1) + 10 : move.at(0) + 10;
            int j = currLetter > moveLetter ? move.at(0) : move.at(1);
            for(i; i < j; i += 10) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "qh1INVALID MOVE: There's a piece in between. ";
                    std::cout << "Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << "qh2INVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }

            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
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
                std::cout << "qvINVALID MOVE: There's a piece in between. ";
                std::cout << "Try a different move.\n";
                return false;
            }

            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
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
            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
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
            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
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

            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;

            if((*ownPieces)[move.at(1)] == 'K') whiteKingMoved = true;
            else blackKingMoved = true;
        } else if(col == 0 && !inCheck(0) && move.at(0) == 40 && 
                  ((move.at(1) == 60 && !whiteKingMoved && !whiteRRookMoved && (*ownPieces).count(70) == 1 && (*ownPieces)[70] == 'R') ||
                  (move.at(1) == 20 && !whiteKingMoved && !whiteLRookMoved &&
                  (*ownPieces).count(0) == 1 && (*ownPieces)[0] == 'R'))) {

            int i = move.at(1) > move.at(0) ? 50 : 10;
            int j = move.at(1) > move.at(0) ? 70 : 40;
            for(i; i < j; i += 10) {
                if((*ownPieces).count(i) == 1 || 
                    (*oppPieces).count(i) == 1) return false;
            }

            ownPieces->erase(40);
            (*ownPieces)[40 + (move.at(1) - move.at(0)) / 2] = 'K';
            if(inCheck(0)) {
                ownPieces->erase(40 + (move.at(1) - move.at(0)) / 2);
                (*ownPieces)[40] = 'K';
                std::cout << "INVALID MOVE: Crossing invalid square";
                return false;
            }
            ownPieces->erase(40 + (move.at(1) - move.at(0)) / 2);
            (*ownPieces)[move.at(1)] = 'K';
            if(move.at(1) == 60) {
                ownPieces->erase(70);
                (*ownPieces)[50] = 'R';
            }
            else {
                ownPieces->erase(0);
                (*ownPieces)[30] = 'R';
            }
            if(inCheck(0)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[40] = 'K';
                if(move.at(1) == 60) {
                    ownPieces->erase(50);
                    (*ownPieces)[70] = 'R';
                }
                else {
                    ownPieces->erase(30);
                    (*ownPieces)[0] = 'R';
                }
                std::cout << "INVALID MOVE: King in Check";
                return false;
            }
            whiteKingMoved = true;
        }
        else if(col == 1 && !inCheck(1) && move.at(0) == 47 && 
                  ((move.at(1) == 67 && !blackKingMoved && !blackRRookMoved && (*ownPieces).count(77) == 1 && (*ownPieces)[77] == 'r') ||
                  (move.at(1) == 27 && !blackKingMoved && !blackLRookMoved &&
                  (*ownPieces).count(7) == 1 && (*ownPieces)[7] == 'r'))) {

            int i = move.at(1) > move.at(0) ? 57 : 17;
            int j = move.at(1) > move.at(0) ? 77 : 47;
            for(i; i < j; i += 10) {
                if((*ownPieces).count(i) == 1 || 
                    (*oppPieces).count(i) == 1) return false;
            }

            ownPieces->erase(47);
            (*ownPieces)[47 + (move.at(1) - move.at(0)) / 2] = 'k';
            if(inCheck(1)) {
                ownPieces->erase(47 + (move.at(1) - move.at(0)) / 2);
                (*ownPieces)[47] = 'k';
                std::cout << "INVALID MOVE: Crossing invalid square";
                return false;
            }
            ownPieces->erase(47 + (move.at(1) - move.at(0)) / 2);
            (*ownPieces)[move.at(1)] = 'k';
            if(move.at(1) == 67) {
                ownPieces->erase(77);
                (*ownPieces)[57] = 'r';
            }
            else {
                ownPieces->erase(7);
                (*ownPieces)[37] = 'r';
            }
            if(inCheck(1)) {
                ownPieces->erase(move.at(1));
                (*ownPieces)[47] = 'K';
                if(move.at(1) == 67) {
                    ownPieces->erase(57);
                    (*ownPieces)[77] = 'r';
                }
                else {
                    ownPieces->erase(37);
                    (*ownPieces)[7] = 'r';
                }
                std::cout << "INVALID MOVE: King in Check";
                return false;
            }
            blackKingMoved = true;
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
    }
    if(enPassantPawns.size() == 2) enPassantPawns.erase(enPassantPawns.begin());
    else if(enPassantPawns.size() == 1 && enPassantPawns.at(0) != move.at(1))
        enPassantPawns.erase(enPassantPawns.begin());
    return true;
}

bool Board::noMoves(int col) {
    map<int, char> *ownPieces = col == 0? &whitePieces : &blackPieces;
    map<int, char> *oppPieces = col == 0? &blackPieces : &whitePieces;

    int kingLetter, kingNum, kingLoc;
    for(auto &piece: *ownPieces) {
        if(piece.second == 'K' || piece.second == 'k') {
            kingLetter = piece.first / 10;
            kingNum = piece.first % 10;
            kingLoc = piece.first;
        }
    }

    map<int, char> ownPiecesCopy = *ownPieces;
    for(auto piece: ownPiecesCopy) {
        int pieceLetter = piece.first / 10;
        int pieceNum = piece.first % 10;

        if(piece.second == 'P') {
            // Pawn moves forward 1 square
            if(ownPieces->count(piece.first + 1) == 0 &&
               oppPieces->count(piece.first + 1) == 0) {

                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 1] = 'P';
                bool check = inCheck(0);
                ownPieces->erase(piece.first + 1);
                (*ownPieces)[piece.first] = 'P';
                if(!check) {
                    std::cout << "HERE1\n";
                    return false;
                }
            }
            // Pawn moves forward 2 squares
            if(ownPieces->count(piece.first + 1) == 0 &&
                    oppPieces->count(piece.first + 1) == 0 &&
                    ownPieces->count(piece.first + 2) == 0 &&
                    oppPieces->count(piece.first + 2) == 0 &&
                    pieceNum == 1) {

                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 2] = 'P';
                bool check = inCheck(0);
                ownPieces->erase(piece.first + 2);
                (*ownPieces)[piece.first] = 'P';
                if(!check) {
                    std::cout << "HERE2\n";
                    return false;
                }
            }
            // Pawn takes diagonally left and doesn't perform en passant
            if(oppPieces->count(piece.first - 9) == 1) {
                char oppPiece = (*oppPieces)[piece.first - 9];
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first - 9);
                (*ownPieces)[piece.first - 9] = 'P';
                bool check = inCheck(0);
                ownPieces->erase(piece.first - 9);
                (*ownPieces)[piece.first] = 'P';
                (*oppPieces)[piece.first - 9] = oppPiece;
                if(!check) {
                    std::cout << "HERE3\n";
                    return false;
                }
            }
            // Pawn takes diagonally right and doesn't perform en passant
            if(oppPieces->count(piece.first + 11) == 1) {
                char oppPiece = (*oppPieces)[piece.first + 11];
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first + 11);
                (*ownPieces)[piece.first + 11] = 'P';
                bool check = inCheck(0);
                ownPieces->erase(piece.first + 11);
                (*ownPieces)[piece.first] = 'P';
                (*oppPieces)[piece.first + 11] = oppPiece;
                if(!check) {
                    std::cout << "HERE4\n";
                    return false;
                }
            }
            // Pawn takes diagonally left and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first - 10) {
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first - 10);
                (*ownPieces)[piece.first - 9] = 'P';
                bool check = inCheck(0);
                ownPieces->erase(piece.first - 9);
                (*ownPieces)[piece.first] = 'P';
                (*oppPieces)[piece.first - 10] = 'p';
                if(!check) {
                    std::cout << "HERE5\n";
                    return false;
                }
            }
            // Pawn takes diagonally right and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first + 10) {
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first + 10);
                (*ownPieces)[piece.first + 11] = 'P';
                bool check = inCheck(0);
                ownPieces->erase(piece.first + 11);
                (*ownPieces)[piece.first] = 'P';
                (*oppPieces)[piece.first + 10] = 'p';
                if(!check) {
                    std::cout << "HERE6\n";
                    return false;
                }
            }
        }

        else if(piece.second == 'p') {
            // Pawn moves forward 1 square
            if(ownPieces->count(piece.first - 1) == 0 &&
               oppPieces->count(piece.first - 1) == 0) {

                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 1] = 'p';
                bool check = inCheck(1);
                ownPieces->erase(piece.first - 1);
                (*ownPieces)[piece.first] = 'p';
                if(!check) {
                    std::cout << "HERE7 " << piece.first << "\n";
                    return false;
                }
            }
            // Pawn moves forward 2 squares
            if(ownPieces->count(piece.first - 1) == 0 &&
                    oppPieces->count(piece.first - 1) == 0 &&
                    ownPieces->count(piece.first - 2) == 0 &&
                    oppPieces->count(piece.first - 2) == 0 &&
                    pieceNum == 6) {

                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 2] = 'p';
                bool check = inCheck(1);
                ownPieces->erase(piece.first - 2);
                (*ownPieces)[piece.first] = 'p';
                if(!check) {
                    std::cout << "HERE8\n";
                    return false;
                }
            }
            // Pawn takes diagonally left and doesn't perform en passant
            if(oppPieces->count(piece.first - 11) == 1) {
                char oppPiece = (*oppPieces)[piece.first - 11];
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first - 11);
                (*ownPieces)[piece.first - 11] = 'p';
                bool check = inCheck(1);
                ownPieces->erase(piece.first - 11);
                (*ownPieces)[piece.first] = 'p';
                (*oppPieces)[piece.first - 11] = oppPiece;
                if(!check) {
                    std::cout << "HERE9\n";
                    return false;
                }            }
            // Pawn takes diagonally right and doesn't perform en passant
            if(oppPieces->count(piece.first + 9) == 1) {
                char oppPiece = (*oppPieces)[piece.first + 9];
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first + 9);
                (*ownPieces)[piece.first + 9] = 'p';
                bool check = inCheck(1);
                ownPieces->erase(piece.first + 9);
                (*ownPieces)[piece.first] = 'p';
                (*oppPieces)[piece.first + 9] = oppPiece;
                if(!check) {
                    std::cout << "HERE10\n";
                    return false;
                }            }
            // Pawn takes diagonally left and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first - 10) {
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first - 10);
                (*ownPieces)[piece.first - 11] = 'p';
                bool check = inCheck(1);
                ownPieces->erase(piece.first - 11);
                (*ownPieces)[piece.first] = 'p';
                (*oppPieces)[piece.first - 10] = 'P';
                if(!check) {
                    std::cout << "HERE11\n";
                    return false;
                }            }
            // Pawn takes diagonally right and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first + 10) {
                ownPieces->erase(piece.first);
                oppPieces->erase(piece.first + 10);
                (*ownPieces)[piece.first + 9] = 'p';
                bool check = inCheck(1);
                ownPieces->erase(piece.first + 9);
                (*ownPieces)[piece.first] = 'p';
                (*oppPieces)[piece.first + 10] = 'P';
                if(!check) {
                    std::cout << "HERE12\n";
                    return false;
                }            }
        }
        else if(piece.second == 'n' || piece.second == 'N') {
            if(ownPieces->count(piece.first - 21) == 0 && 
               piece.first - 21 >= 0 && piece.first % 10 != 0) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 21) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 21];
                    oppPieces->erase(piece.first - 21);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 21] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 21);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 21] = oppPiece;
                if(!check) {
                    std::cout << "HERE13\n";
                    return false;
                }            }
            if(ownPieces->count(piece.first - 19) == 0 && 
               piece.first - 19 >= 0 && piece.first % 10 != 8) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 19) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 19];
                    oppPieces->erase(piece.first - 19);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 19] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 19);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 19] = oppPiece;
                if(!check) {
                    std::cout << "HERE14\n";
                    return false;
                }            }
            if(ownPieces->count(piece.first - 8) == 0 && 
               piece.first - 8 >= 0 && piece.first % 10 != 7
               && piece.first % 10 != 6) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 8) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 8];
                    oppPieces->erase(piece.first - 8);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 8] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 8);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 8] = oppPiece;
                if(!check) {
                    std::cout << "HERE15\n";
                    return false;
                }            }
            if(ownPieces->count(piece.first - 12) == 0 && 
               piece.first - 12 >= 0 && piece.first % 10 != 0
               && piece.first % 10 != 1) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 12) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 12];
                    oppPieces->erase(piece.first - 12);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 12] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 12);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 12] = oppPiece;
                if(!check) {
                    std::cout << "HERE16\n";
                    return false;
                }            }
            if(ownPieces->count(piece.first + 8) == 0 && 
               piece.first + 8 < 78 && piece.first % 10 != 0
               && piece.first % 10 != 1) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 8) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 8];
                    oppPieces->erase(piece.first + 8);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 8] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 8);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 8] = oppPiece;
                if(!check) {
                    std::cout << "HERE17\n";
                    return false;
                }            }
            if(ownPieces->count(piece.first + 12) == 0 && 
               piece.first + 12 < 78 && piece.first % 10 != 6
               && piece.first % 10 != 7) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 12) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 12];
                    oppPieces->erase(piece.first + 12);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 12] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 12);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 12] = oppPiece;
                if(!check) {
                    std::cout << "HERE18\n";
                    return false;
                }            }
            if(ownPieces->count(piece.first + 19) == 0 && 
               piece.first + 19 < 78 && piece.first % 10 != 9
               && piece.first % 10 != 0) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 19) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 19];
                    oppPieces->erase(piece.first + 19);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 19] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 19);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 19] = oppPiece;
                if(!check) {
                    std::cout << "HERE19\n";
                    return false;
                }            }
            if(ownPieces->count(piece.first + 21) == 0 && 
               piece.first + 21 < 78 && piece.first % 10 != 7) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 21) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 21];
                    oppPieces->erase(piece.first + 21);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 21] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 21);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 21] = oppPiece;
                if(!check) {
                    std::cout << "HERE20\n";
                    return false;
                }            }
        }
        else if(piece.second == 'r' || piece.second == 'R') {
            ownPieces->erase(piece.first);
            for(int i = piece.first + 1; i < pieceLetter * 10 + 8; ++i) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    (*ownPieces)[piece.first] = piece.second;
                    std::cout << "HERE21\n";
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 1; i >= pieceLetter * 10; --i) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE22\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first + 10; i < 78; i += 10) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE23\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 10; i >= 0; i -= 10) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
                    
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE24\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            (*ownPieces)[piece.first] = piece.second;
        }
        else if(piece.second == 'b' || piece.second == 'B') {
            ownPieces->erase(piece.first);
            for(int i = piece.first + 11; i < 78 && i % 10 != 8; i += 11) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
                
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE25\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 11; i >= 0 && i % 10 != 9; i -= 11) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE26\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first + 9; i < 78 && i % 10 != 9; i += 9) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
                
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE27\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 9; i >= 0 && i % 10 != 8; i -= 9) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
                    
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE28\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            (*ownPieces)[piece.first] = piece.second;
        }
        else if(piece.second == 'Q' || piece.second == 'q') {
            ownPieces->erase(piece.first);
            for(int i = piece.first + 1; i < pieceLetter * 10 + 8; ++i) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
                    
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE29\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 1; i >= pieceLetter * 10; --i) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE30\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first + 10; i < 78; i += 10) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
                
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE31\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 10; i >= 0; i -= 10) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
 
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE32\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first + 11; i < 78 && i % 10 != 8; i += 11) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE33\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 11; i >= 0 && i % 10 != 9; i -= 11) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE34\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first + 9; i < 78 && i % 10 != 9; i += 9) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;

                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE35\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            for(int i = piece.first - 9; i >= 0 && i % 10 != 8; i -= 9) {
                char oppPiece = '\0';
                if(ownPieces->count(i) == 1) break;
                    
                if(oppPieces->count(i) == 1) {
                    oppPiece = (*oppPieces)[i];
                    oppPieces->erase(i);
                }
                (*ownPieces)[i] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(i);
                if(oppPiece != '\0') (*oppPieces)[i] = oppPiece;
                if(!check) {
                    std::cout << "HERE36\n";
                    (*ownPieces)[piece.first] = piece.second;
                    return false;
                }
                if(oppPiece != '\0') break;
            }
            (*ownPieces)[piece.first] = piece.second;
        }
        else if(piece.second == 'K' || piece.second == 'k') {
            if(ownPieces->count(piece.first + 1) == 0 && 
               piece.first + 1 < 78 && piece.first % 10 != 7) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 1) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 1];
                    oppPieces->erase(piece.first + 1);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 1] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 1);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 1] = oppPiece;
                if(!check) {
                    std::cout << "HERE37\n";
                    return false;
                }
            }
            if(ownPieces->count(piece.first - 1) == 0 && 
               piece.first - 1 >= 0 && piece.first % 10 != 0) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 1) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 1];
                    oppPieces->erase(piece.first - 1);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 1] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 1);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 1] = oppPiece;
                if(!check) {
                    std::cout << "HERE38\n";
                    return false;
                }
            }
            if(ownPieces->count(piece.first + 10) == 0 && 
               piece.first + 10 < 78) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 10) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 10];
                    oppPieces->erase(piece.first + 10);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 10] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 10);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 10] = oppPiece;
                if(!check) {
                    std::cout << "HERE39\n";
                    return false;
                }
            }
            if(ownPieces->count(piece.first - 10) == 0 && 
               piece.first - 10 >= 0) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 10) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 10];
                    oppPieces->erase(piece.first - 10);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 10] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 10);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 10] = oppPiece;
                if(!check) {
                    std::cout << "HERE40\n";
                    return false;
                }
            }
            if(ownPieces->count(piece.first + 11) == 0 && 
               piece.first + 11 < 78 && piece.first % 10 != 7) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 11) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 11];
                    oppPieces->erase(piece.first + 11);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 11] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 11);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 11] = oppPiece;
                if(!check) {
                    std::cout << "HERE41\n";
                    return false;
                }
            }
            if(ownPieces->count(piece.first + 9) == 0 && 
               piece.first + 9 < 78 && piece.first % 10 != 0) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first + 9) == 1) {
                    oppPiece = (*oppPieces)[piece.first + 9];
                    oppPieces->erase(piece.first + 9);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first + 9] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first + 9);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first + 9] = oppPiece;
                if(!check) {
                    std::cout << "HERE42\n";
                    return false;
                }
            }
            if(ownPieces->count(piece.first - 11) == 0 && 
               piece.first - 11 >= 0 && piece.first % 10 != 0) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 11) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 11];
                    oppPieces->erase(piece.first - 11);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 11] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 11);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 11] = oppPiece;
                if(!check) {
                    std::cout << "HERE43\n";
                    return false;
                }
            }
            if(ownPieces->count(piece.first - 9) == 0 && 
               piece.first - 9 >= 0 && piece.first % 10 != 7) {
                char oppPiece = '\0';
                if(oppPieces->count(piece.first - 9) == 1) {
                    oppPiece = (*oppPieces)[piece.first - 9];
                    oppPieces->erase(piece.first - 9);
                }
                ownPieces->erase(piece.first);
                (*ownPieces)[piece.first - 9] = piece.second;
                bool check = inCheck(col);
                ownPieces->erase(piece.first - 9);
                (*ownPieces)[piece.first] = piece.second;
                if(oppPiece != '\0') (*oppPieces)[piece.first - 9] = oppPiece;
                if(!check) {
                    std::cout << "HERE44\n";
                    return false;
                }
            }
        }
    }
    return true;
}

bool Board::inCheck(int col, int move) {
    map<int, char> *ownPieces = col == 0? &whitePieces : &blackPieces;
    map<int, char> *oppPieces = col == 0? &blackPieces : &whitePieces;

    int kingLetter, kingNum, kingLoc;
    if(move != -1) {
        kingLetter = move / 10;
        kingNum = move % 10;
        kingLoc = move;
    } else {
        for(auto &piece: *ownPieces) {
            if(piece.second == 'K' || piece.second == 'k') {
                kingLetter = piece.first / 10;
                kingNum = piece.first % 10;
                kingLoc = piece.first;
            }
        }
    }

    for(auto &piece: *oppPieces) {
        int pieceLetter = piece.first / 10;
        int pieceNum = piece.first % 10;
        
        if(piece.second == 'p' && 
           (piece.first == kingLoc + 11 || piece.first == kingLoc - 9))  
                return true;

        else if(piece.second == 'P' && 
                (piece.first == kingLoc + 9 || piece.first == kingLoc - 11))
                return true;

        else if(piece.second == 'r' || piece.second == 'R') {
            if(kingLetter == pieceLetter) {
                int i = kingNum > pieceNum ? piece.first + 1 : kingLoc + 1;
                int j = kingNum > pieceNum ? kingLoc : piece.first;
                for(i; i < j; ++i) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            else if(kingNum == pieceNum) {
                int i = kingLetter > pieceLetter ? piece.first + 10 : 
                kingLoc + 10;
                int j = kingLetter > pieceLetter ? kingLoc : piece.first;
                for(i; i < j; i += 10) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            else continue;
        }
        else if(piece.second == 'b' || piece.second == 'B') {
            if(kingNum - pieceNum == kingLetter - pieceLetter) {
                int i = kingNum < pieceNum ? kingLoc + 11 : piece.first + 11;
                int j = kingNum < pieceNum ? piece.first : kingLoc;

                for(i; i < j; i += 11) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            else if(kingNum - pieceNum == pieceLetter - kingLetter) {
                int i = kingNum < pieceNum ? piece.first + 9 : kingLoc + 9;
                int j = kingNum < pieceNum ? kingLoc : piece.first;
                for(i; i < j; i += 9) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            else continue;
        }
        else if(piece.second == 'q' || piece.second == 'Q') {
            if(kingLetter == pieceLetter) {
                int i = kingNum > pieceNum ? piece.first + 1 : kingLoc + 1;
                int j = kingNum > pieceNum ? kingLoc : piece.first;
                for(i; i < j; ++i) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            else if(kingNum == pieceNum) {
                int i = kingLetter > pieceLetter ? piece.first + 10 : 
                kingLoc + 10;
                int j = kingLetter > pieceLetter ? kingLoc : piece.first;
                for(i; i < j; i += 10) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1)
                        break;
                }
                if(i == j) return true;
            }
            else if(kingNum - pieceNum == kingLetter - pieceLetter) {
                int i = kingNum < pieceNum ? kingLoc + 11 : piece.first + 11;
                int j = kingNum < pieceNum ? piece.first : kingLoc;

                for(i; i < j; i += 11) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            else if(kingNum - pieceNum == pieceLetter - kingLetter) {
                int i = kingNum < pieceNum ? piece.first + 9 : kingLoc + 9;
                int j = kingNum < pieceNum ? kingLoc : piece.first;
                for(i; i < j; i += 9) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            else continue;
        }
        else if((piece.second == 'n' || piece.second == 'N') &&
                (kingLoc == piece.first - 21 || kingLoc == piece.first - 19 ||
                kingLoc == piece.first - 8 || kingLoc == piece.first - 12 ||
                kingLoc == piece.first + 8 || kingLoc == piece.first + 12 ||
                kingLoc == piece.first + 19 || kingLoc == piece.first + 21)) 
                return true;

        else if((piece.second == 'k' || piece.second == 'K') &&
                (kingLoc == piece.first + 1 || kingLoc == piece.first - 1 ||
                kingLoc == piece.first + 10 || kingLoc == piece.first - 10 ||
                kingLoc == piece.first + 11 || kingLoc == piece.first - 11 ||
                kingLoc == piece.first + 9 || kingLoc == piece.first - 9))
                return true;
    }
    return false;
}

void Board::display() {
    tDisplay->display(); 
    gDisplay->display();
}
