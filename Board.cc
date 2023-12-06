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
        std::cout << "Other side's piece on " << loc << move.at(0) % 10 + 1
                  << ". Try a different move.\n";
        return false;
    }
    else {
        char loc = 'a' + move.at(0) / 10;
        std::cout << "There is no piece at " << loc << move.at(0) % 10 + 1
                  << ". Try a different move.\n";
        return false;
    }

    int currLetter = move.at(0) / 10;
    int currNum = move.at(0) % 10;
    int moveLetter = move.at(1) / 10;
    int moveNum = move.at(1) % 10;

    bool *possibleRookDeath = nullptr;
    if(move.at(1) == 0 && col == 1 && !whiteLRookMoved) 
        possibleRookDeath = &whiteLRookMoved;
    else if(move.at(1) == 70 && col == 1 && !whiteRRookMoved) 
        possibleRookDeath = &whiteRRookMoved;
    else if(move.at(1) == 7 && col == 0 && !blackLRookMoved) 
        possibleRookDeath = &blackLRookMoved;
    else if(move.at(1) == 77 && col == 0 && !blackRRookMoved) 
        possibleRookDeath = &blackRRookMoved;

    // Piece is a white pawn
    if((*ownPieces)[move.at(0)] == 'P') {
        // Pawn moves 2 squares forward
        if(currLetter == moveLetter && currNum == moveNum - 2
           && currNum == 1 && ownPieces->count(move.at(1) - 1) == 0
                           && ownPieces->count(move.at(1)) == 0
                           && oppPieces->count(move.at(1) - 1) == 0
                           && oppPieces->count(move.at(1)) == 0) {
            
            if(movePutsKingInCheck(move.at(0), move.at(1), 0, ownPieces, 
            oppPieces)) return false;
    
            if(currLetter != 0 && oppPieces->count(move.at(1) - 10) == 1
              && (*oppPieces)[move.at(1) - 10] == 'p')
                enPassantPawns.emplace_back(move.at(1));

            if(currLetter != 7 && oppPieces->count(move.at(1) + 10) == 1
              && (*oppPieces)[move.at(1) + 10] == 'p') 
              enPassantPawns.emplace_back(move.at(1));
        }
        
        // Pawn moves 1 square forward
        else if(currLetter == moveLetter && currNum == moveNum - 1
                && ownPieces->count(move.at(1)) == 0
                && oppPieces->count(move.at(1)) == 0) {
            
            if(moveNum == 7) (*ownPieces)[move.at(0)] = move.at(2);
            if(movePutsKingInCheck(move.at(0), move.at(1), 0, ownPieces, 
            oppPieces)) {
                if(moveNum == 7) (*ownPieces)[move.at(0)] = 'P';
                return false;
            }
        }

        // Pawn moves diagonally 1 square
        else if((currLetter == moveLetter + 1 || 
                 currLetter == moveLetter - 1) && currNum == moveNum - 1
                && oppPieces->count(move.at(1)) == 1) {
            
            if(moveNum == 7) (*ownPieces)[move.at(0)] = move.at(2);
            if(movePutsKingInCheck(move.at(0), move.at(1), 0, ownPieces, 
            oppPieces)) {
                if(moveNum == 7) (*ownPieces)[move.at(0)] = 'P';
                return false;
            }
        }

        // Pawn performs en passant
        else if((currLetter == moveLetter + 1 || 
                 currLetter == moveLetter - 1) && moveNum == 5 
                && currNum == moveNum - 1 
                && enPassantPawns.size() == 1
                && enPassantPawns.at(0) == move.at(1) - 1) {

            oppPieces->erase(enPassantPawns.at(0));
            if(movePutsKingInCheck(move.at(0), move.at(1), 0, ownPieces, 
            oppPieces)) {
                (*oppPieces)[move.at(1) - 1] = 'p';
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
                                
            if(movePutsKingInCheck(move.at(0), move.at(1), 1, ownPieces, 
            oppPieces)) return false;

           if(currLetter != 0 && oppPieces->count(move.at(1) - 10) == 1
              && (*oppPieces)[move.at(1) - 10] == 'P')
                enPassantPawns.emplace_back(move.at(1));

            if(currLetter != 7 && oppPieces->count(move.at(1) + 10) == 1
              && (*oppPieces)[move.at(1) + 10] == 'P') 
              enPassantPawns.emplace_back(move.at(1));
        }

        // Pawn moves 1 square forward
        else if(currLetter == moveLetter && currNum == moveNum + 1
                && ownPieces->count(move.at(1)) == 0
                && oppPieces->count(move.at(1)) == 0) {
            
            if(moveNum == 0) (*ownPieces)[move.at(0)] = move.at(2);
            if(movePutsKingInCheck(move.at(0), move.at(1), 1, ownPieces, 
            oppPieces)) {
                if(moveNum == 0) (*ownPieces)[move.at(0)] = 'p';
                return false;
            }
        }

        // Pawn moves diagonally 1 square
        else if((currLetter == moveLetter + 1 ||
                 currLetter == moveLetter - 1) && currNum == moveNum + 1
                && oppPieces->count(move.at(1)) == 1) {
            
            if(moveNum == 0) (*ownPieces)[move.at(0)] = move.at(2);
            if(movePutsKingInCheck(move.at(0), move.at(1), 1, ownPieces, 
            oppPieces)) {
                if(moveNum == 0) (*ownPieces)[move.at(0)] = 'P';
                return false;
            }
        }
        
        // Pawn performs en passant
        else if((currLetter == moveLetter + 1 
                 || currLetter == moveLetter - 1) && moveNum == 2 
                && currNum == moveNum + 1 
                && enPassantPawns.size() == 1
                && enPassantPawns.at(0) == move.at(1) + 1) {

            oppPieces->erase(enPassantPawns.at(0));
            if(movePutsKingInCheck(move.at(0), move.at(1), 0, ownPieces, 
            oppPieces)) {
                (*oppPieces)[move.at(1) + 1] = 'P';
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
    else if((*ownPieces)[move.at(0)] == 'R' ||
            (*ownPieces)[move.at(0)] == 'r') {
        // Rook is moving horizontally
        if(currNum == moveNum) {
            int i = currLetter > moveLetter ? move.at(1) + 10 : move.at(0) + 10;
            int j = currLetter > moveLetter ? move.at(0) : move.at(1);
            for( ; i < j; i += 10) {
                if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) {
                    std::cout << "INVALID MOVE: There's a piece in between. ";
                    std::cout << "Try a different move.\n";
                    return false;
                }
            }
            if(ownPieces->count(move.at(1)) == 1) {
                std::cout << move.at(0) << " " << move.at(1) << "\n";
                std::cout << "INVALID MOVE: There's a piece in between. ";
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
            for( ; i < j; ++i) {
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
    else if((*ownPieces)[move.at(0)] == 'N' || 
            (*ownPieces)[move.at(0)] == 'n') {
        // Checks every possible knight move
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
    else if((*ownPieces)[move.at(0)] == 'B' ||
            (*ownPieces)[move.at(0)] == 'b') {
        // Bishop is moving on the right diagonal
        if(moveNum - currNum == moveLetter - currLetter) {
            int i = moveNum < currNum ? move.at(1) + 11 : move.at(0) + 11;
            int j = moveNum < currNum ? move.at(0) : move.at(1);
            for( ; i < j; i += 11) {
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

        // Bishop is moving on the left diagonal
        else if(moveNum - currNum == currLetter - moveLetter) {
            int i = moveNum < currNum ? move.at(0) + 9 : move.at(1) + 9;
            int j = moveNum < currNum ? move.at(1) : move.at(0);
            for( ; i < j; i += 9) {
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
    else if((*ownPieces)[move.at(0)] == 'Q' || 
            (*ownPieces)[move.at(0)] == 'q') {
        // Queen is moving horizontally
        if(currNum == moveNum) {
            int i = currLetter > moveLetter ? move.at(1) + 10 : move.at(0) + 10;
            int j = currLetter > moveLetter ? move.at(0) : move.at(1);
            for( ; i < j; i += 10) {
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

            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
        }

        // Queen is moving vertically
        else if(currLetter == moveLetter) {
            int i = currNum > moveNum ? move.at(1) + 1 : move.at(0) + 1;
            int j = currNum > moveNum ? move.at(0) : move.at(1);
            for( ; i < j; ++i) {
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

            if(movePutsKingInCheck(move.at(0), move.at(1), col, ownPieces, 
                oppPieces)) return false;
        }

        // Queen is moving on right diagonal
        else if(moveNum - currNum == moveLetter - currLetter) {
            int i = moveNum < currNum ? move.at(1) + 11 : move.at(0) + 11;
            int j = moveNum < currNum ? move.at(0) : move.at(1);
            for( ; i < j; i += 11) {
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
            int i = moveNum < currNum ? move.at(0) + 9 : move.at(1) + 9;
            int j = moveNum < currNum ? move.at(1) : move.at(0);
            for( ; i < j; i += 9) {
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

    else if((*ownPieces)[move.at(0)] == 'K' || 
            (*ownPieces)[move.at(0)] == 'k') {

        // King moves 1 square
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

        // White king attempts to castle
        } else if(col == 0 && !inCheck(0) && move.at(0) == 40 && 
                  ((move.at(1) == 60 && !whiteKingMoved && !whiteRRookMoved && (*ownPieces).count(70) == 1 && (*ownPieces)[70] == 'R') ||
                  (move.at(1) == 20 && !whiteKingMoved && !whiteLRookMoved &&
                  (*ownPieces).count(0) == 1 && (*ownPieces)[0] == 'R'))) {

            int i = move.at(1) > move.at(0) ? 50 : 10;
            int j = move.at(1) > move.at(0) ? 70 : 40;
            for( ; i < j; i += 10) {
                if((*ownPieces).count(i) == 1 || 
                    (*oppPieces).count(i) == 1) return false;
            }
            i = 40 + (move.at(1) - move.at(0)) / 2;
            for( ; ((i > move.at(0) && i <= move.at(1)) ||
                    (i < move.at(0) && i >= move.at(1))) &&
                    !movePutsKingInCheck(40, i, 0, ownPieces, oppPieces, true);
                    i += (move.at(1) - move.at(0)) / 2);
            if(i != move.at(1)) return false;
            else {
                (*ownPieces)[move.at(1)] = 'K';
                ownPieces->erase(move.at(0));
                ownPieces->erase(move.at(1) > move.at(0) ? 70 : 0);
                (*ownPieces)[move.at(1) > move.at(0) ? 50 : 30] = 'R';
                whiteKingMoved = true;
                if(move.at(1) > move.at(0)) whiteRRookMoved = true;
                else whiteLRookMoved = true;
            }

        // Black king attempts to castle
        } else if(col == 1 && !inCheck(1) && move.at(0) == 47 && 
                  ((move.at(1) == 67 && !blackKingMoved && !blackRRookMoved && (*ownPieces).count(77) == 1 && (*ownPieces)[77] == 'r') ||
                  (move.at(1) == 27 && !blackKingMoved && !blackLRookMoved &&
                  (*ownPieces).count(7) == 1 && (*ownPieces)[7] == 'r'))) {

            int i = move.at(1) > move.at(0) ? 57 : 17;
            int j = move.at(1) > move.at(0) ? 77 : 47;
            for( ; i < j; i += 10) {
                if((*ownPieces).count(i) == 1 || 
                    (*oppPieces).count(i) == 1) return false;
            }
            i = 47 + (move.at(1) - move.at(0)) / 2;
            for( ; ((i > move.at(0) && i <= move.at(1)) ||
                    (i < move.at(0) && i >= move.at(1))) &&
                    !movePutsKingInCheck(47, i, 0, ownPieces, oppPieces, true);
                    i += (move.at(1) - move.at(0)) / 2);
            if(i != move.at(1)) return false;
            else {
                (*ownPieces)[move.at(1)] = 'K';
                ownPieces->erase(move.at(0));
                ownPieces->erase(move.at(1) > move.at(0) ? 77 : 7);
                (*ownPieces)[move.at(1) > move.at(0) ? 57 : 37] = 'R';
                blackKingMoved = true;
                if(move.at(1) > move.at(0)) blackRRookMoved = true;
                else blackLRookMoved = true;
            }
        }
        else {
            std::cout << "INVALID MOVE: Try a different move\n";
            return false;
        }
    }
    if(enPassantPawns.size() == 2) enPassantPawns.erase(enPassantPawns.begin());
    else if(enPassantPawns.size() == 1 && enPassantPawns.at(0) != move.at(1))
        enPassantPawns.erase(enPassantPawns.begin());
    
    if(possibleRookDeath) *possibleRookDeath = true;
    return true;
}

bool Board::noMoves(int col) {
    map<int, char> *ownPieces = col == 0? &whitePieces : &blackPieces;
    map<int, char> *oppPieces = col == 0? &blackPieces : &whitePieces;

    // Initializes and grabs Kings location information
    int kingLetter, kingNum, kingLoc;
    for(auto &piece: *ownPieces) {
        if(piece.second == 'K' || piece.second == 'k') {
            kingLetter = piece.first / 10;
            kingNum = piece.first % 10;
            kingLoc = piece.first;
        }
    }

    // Creating copy of ownPieces so that ownPieces can be manipulated without
    // affecting the for loop
    map<int, char> ownPiecesCopy = *ownPieces;
    for(auto piece: ownPiecesCopy) {
        // Piece information
        int pieceLetter = piece.first / 10;
        int pieceNum = piece.first % 10;

        // Piece is a white pawn
        if(piece.second == 'P') {
            // Pawn moves forward 1 square
            if(ownPieces->count(piece.first + 1) == 0 &&
               oppPieces->count(piece.first + 1) == 0 &&
               !movePutsKingInCheck(piece.first, piece.first + 1, 0, ownPieces,
               oppPieces, true)) return false;

            // Pawn moves forward 2 squares
            if(ownPieces->count(piece.first + 1) == 0 &&
                    oppPieces->count(piece.first + 1) == 0 &&
                    ownPieces->count(piece.first + 2) == 0 &&
                    oppPieces->count(piece.first + 2) == 0 &&
                    pieceNum == 1 && !movePutsKingInCheck(piece.first, 
                    piece.first + 2, 0, ownPieces, oppPieces, true)) 
                return false;

            // Pawn takes diagonally left and doesn't perform en passant
            if(oppPieces->count(piece.first - 9) == 1 && 
            !movePutsKingInCheck(piece.first, piece.first - 9, 0, ownPieces, 
            oppPieces, true)) return false;

            // Pawn takes diagonally right and doesn't perform en passant
            if(oppPieces->count(piece.first + 11) == 1 &&
            !movePutsKingInCheck(piece.first, piece.first + 11, 0, ownPieces, 
            oppPieces, true)) return false;

            // Pawn takes diagonally left and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first - 10) {
    
                oppPieces->erase(piece.first - 10);
                if(!movePutsKingInCheck(piece.first, piece.first - 9, 0, 
                ownPieces, oppPieces, true)) {
                    (*oppPieces)[piece.first - 10] = 'p';
                    return false;
                } else (*oppPieces)[piece.first - 10] = 'p';
            }

            // Pawn takes diagonally right and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first + 10) {

                oppPieces->erase(piece.first + 10);
                if(!movePutsKingInCheck(piece.first, piece.first + 11, 0, 
                ownPieces, oppPieces, true)) {
                    (*oppPieces)[piece.first + 10] = 'p';
                    return false;
                } else (*oppPieces)[piece.first + 10] = 'p';
            }
        }

        // Piece is a black pawn
        else if(piece.second == 'p') {
            // Pawn moves forward 1 square
            if(ownPieces->count(piece.first - 1) == 0 &&
               oppPieces->count(piece.first - 1) == 0 &&
               !movePutsKingInCheck(piece.first, piece.first - 1, 1, ownPieces, oppPieces, true)) return false;

            // Pawn moves forward 2 squares
            if(ownPieces->count(piece.first - 1) == 0 &&
                    oppPieces->count(piece.first - 1) == 0 &&
                    ownPieces->count(piece.first - 2) == 0 &&
                    oppPieces->count(piece.first - 2) == 0 &&
                    pieceNum == 6 && !movePutsKingInCheck(piece.first, 
                    piece.first - 2, 1, ownPieces, oppPieces, true))
                return false;

            // Pawn takes diagonally left and doesn't perform en passant
            if(oppPieces->count(piece.first - 11) == 1 &&
            !movePutsKingInCheck(piece.first, piece.first - 11, 1, ownPieces, 
            oppPieces, true)) return false;
    
            // Pawn takes diagonally right and doesn't perform en passant
            if(oppPieces->count(piece.first + 9) == 1 &&
            !movePutsKingInCheck(piece.first, piece.first + 9, 1, ownPieces, oppPieces, true)) return false;

            // Pawn takes diagonally left and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first - 10) {
                
                oppPieces->erase(piece.first - 10);
                if(!movePutsKingInCheck(piece.first, piece.first - 11, 1, ownPieces, oppPieces, true)) {
                    (*oppPieces)[piece.first - 10] = 'P';
                    return false;
                } else (*oppPieces)[piece.first - 10] = 'P';
            }
            
            // Pawn takes diagonally right and performs en passant
            if(enPassantPawns.size() == 1 &&
                    enPassantPawns.at(0) == piece.first + 10) {

                oppPieces->erase(piece.first + 10);
                if(!movePutsKingInCheck(piece.first, piece.first + 9, 1, ownPieces, oppPieces, true)) {
                    (*oppPieces)[piece.first + 10] = 'P';
                    return false;
                } else (*oppPieces)[piece.first + 10] = 'P';
            }
        }

        // Piece is a knight
        else if(piece.second == 'n' || piece.second == 'N') {
            // Knight moves 2 left, 1 down
            if(ownPieces->count(piece.first - 21) == 0 && 
               piece.first - 21 >= 0 && piece.first % 10 != 0 &&
               !movePutsKingInCheck(piece.first, piece.first - 21, col, 
               ownPieces, oppPieces, true)) return false;

            // Knight moves 2 left, 1 up
            if(ownPieces->count(piece.first - 19) == 0 && 
               piece.first - 19 >= 0 && piece.first % 10 != 7 &&
               !movePutsKingInCheck(piece.first, piece.first - 19, col, 
               ownPieces, oppPieces, true)) return false;

            // Knight moves 1 left, 2 up
            if(ownPieces->count(piece.first - 8) == 0 && 
               piece.first - 8 >= 0 && piece.first % 10 <= 5 &&
               !movePutsKingInCheck(piece.first, piece.first - 8, col, 
               ownPieces, oppPieces, true)) return false;

            // Knight moves 1 left, 2 down
            if(ownPieces->count(piece.first - 12) == 0 && 
               piece.first - 12 >= 0 && piece.first % 10 >= 2 &&
               !movePutsKingInCheck(piece.first, piece.first - 12, col, 
               ownPieces, oppPieces, true)) return false;

            // Knight moves 1 right, 2 down
            if(ownPieces->count(piece.first + 8) == 0 && 
               piece.first + 8 < 78 && piece.first % 10 >= 2 &&
               !movePutsKingInCheck(piece.first, piece.first + 8, col, 
               ownPieces, oppPieces, true)) return false;

            // Knight moves 1 right, 2 up
            if(ownPieces->count(piece.first + 12) == 0 && 
               piece.first + 12 < 78 && piece.first % 10 <= 5 &&
               !movePutsKingInCheck(piece.first, piece.first + 12, col, 
               ownPieces, oppPieces, true)) return false;

            // Kngiht moves 2 right, 1 down
            if(ownPieces->count(piece.first + 19) == 0 && 
               piece.first + 19 < 78 && piece.first % 10 != 0 &&
               !movePutsKingInCheck(piece.first, piece.first + 19, col, 
               ownPieces, oppPieces, true)) return false;

            // Knight moves 2 right, 1 up
            if(ownPieces->count(piece.first + 21) == 0 && 
               piece.first + 21 < 78 && piece.first % 10 != 7 &&
               !movePutsKingInCheck(piece.first, piece.first + 21, col, 
               ownPieces, oppPieces, true)) return false;
        }

        // Piece is a rook
        else if(piece.second == 'r' || piece.second == 'R') {
            // Rook moves up
            for(int i = piece.first + 1; i < pieceLetter * 10 + 8 && 
                    ownPieces->count(i) != 1; ++i) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Rook moves down
            for(int i = piece.first - 1; i >= pieceLetter * 10 &&
                    ownPieces->count(i) != 1; --i) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }
            
            // Rook moves right
            for(int i = piece.first + 10; i < 78 && 
                    ownPieces->count(i) != 1; i += 10) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Rook moves left
            for(int i = piece.first - 10; i >= 0 && 
                    ownPieces->count(i) != 1; i -= 10) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }
        }

        // Piece is a bishop
        else if(piece.second == 'b' || piece.second == 'B') {
            // Bishop moves right and up
            for(int i = piece.first + 11; i < 78 && i % 10 != 8 && 
                    ownPieces->count(i) != 1; i += 11) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Bishop moves left and down
            for(int i = piece.first - 11; i >= 0 && i % 10 != 9 &&
                    ownPieces->count(i) != 1; i -= 11) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Bishop moves right and down
            for(int i = piece.first + 9; i < 78 && i % 10 != 9 &&
                    ownPieces->count(i) != 1; i += 9) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Bishop moves left and up
            for(int i = piece.first - 9; i >= 0 && i % 10 != 8 &&
                    ownPieces->count(i) != 1; i -= 9) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }
        }

        // Piece is a queen
        else if(piece.second == 'Q' || piece.second == 'q') {
            // Queen moves up
            for(int i = piece.first + 1; i < pieceLetter * 10 + 8 && 
                    ownPieces->count(i) != 1; ++i) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Queen moves down
            for(int i = piece.first - 1; i >= pieceLetter * 10 &&
                    ownPieces->count(i) != 1; --i) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }
            
            // Queen moves right
            for(int i = piece.first + 10; i < 78 && 
                    ownPieces->count(i) != 1; i += 10) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Queen moves left
            for(int i = piece.first - 10; i >= 0 && 
                    ownPieces->count(i) != 1; i -= 10) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Queen moves right and up
            for(int i = piece.first + 11; i < 78 && i % 10 != 8 && 
                    ownPieces->count(i) != 1; i += 11) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Queen moves left and down
            for(int i = piece.first - 11; i >= 0 && i % 10 != 9 &&
                    ownPieces->count(i) != 1; i -= 11) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Queen moves right and down
            for(int i = piece.first + 9; i < 78 && i % 10 != 9 &&
                    ownPieces->count(i) != 1; i += 9) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }

            // Queen moves left and up
            for(int i = piece.first - 9; i >= 0 && i % 10 != 8 &&
                    ownPieces->count(i) != 1; i -= 9) {
                if(!movePutsKingInCheck(piece.first, i, col, ownPieces, 
                oppPieces, true)) return false;
                if(oppPieces->count(i) == 1) break;
            }
        }

        // Piece is a king
        else if(piece.second == 'K' || piece.second == 'k') {
            // King moves up 1
            if(ownPieces->count(piece.first + 1) == 0 && 
               piece.first % 10 != 7 &&
               !movePutsKingInCheck(piece.first, piece.first + 1, col, 
               ownPieces, oppPieces, true)) return false;

            // King moves down 1
            if(ownPieces->count(piece.first - 1) == 0 && 
               piece.first % 10 != 0 &&
               !movePutsKingInCheck(piece.first, piece.first - 1, col, 
               ownPieces, oppPieces, true)) return false;

            // King moves right 1
            if(ownPieces->count(piece.first + 10) == 0 && 
               piece.first + 10 < 78 &&
               !movePutsKingInCheck(piece.first, piece.first + 10, col, 
               ownPieces, oppPieces, true)) return false;

            // King moves left 1
            if(ownPieces->count(piece.first - 10) == 0 && 
               piece.first - 10 >= 0 &&
               !movePutsKingInCheck(piece.first, piece.first - 10, col, 
               ownPieces, oppPieces, true)) return false;

            // King moves right 1, up 1
            if(ownPieces->count(piece.first + 11) == 0 && 
               piece.first + 11 < 78 && piece.first % 10 != 7 &&
               !movePutsKingInCheck(piece.first, piece.first + 11, col, 
               ownPieces, oppPieces, true)) return false;

            // King moves right 1, down 1
            if(ownPieces->count(piece.first + 9) == 0 && 
               piece.first + 9 < 78 && piece.first % 10 != 0 &&
               !movePutsKingInCheck(piece.first, piece.first + 9, col, 
               ownPieces, oppPieces, true)) return false;

            // King moves left 1, down 1
            if(ownPieces->count(piece.first - 11) == 0 && 
               piece.first - 11 >= 0 && piece.first % 10 != 0 &&
               !movePutsKingInCheck(piece.first, piece.first - 11, col, 
               ownPieces, oppPieces, true)) return false;

            // King moves left 1, up 1
            if(ownPieces->count(piece.first - 9) == 0 && 
               piece.first - 9 >= 0 && piece.first % 10 != 7 &&
               !movePutsKingInCheck(piece.first, piece.first - 9, col, 
               ownPieces, oppPieces, true)) return false;
            
            // White king castles left
            if(col == 0 && !inCheck(0) && !whiteKingMoved && !whiteLRookMoved) {
                int i = 10, j = 40;
                for( ; i < j; i += 10) {
                    if((*ownPieces).count(i) == 1 || 
                        (*oppPieces).count(i) == 1) break;
                }
                if(i == j) {
                    i = 30;
                    for( ; i >= 20 && !movePutsKingInCheck(40, i, 0, ownPieces, oppPieces, true); i -= 10);
                    if(i == 10) return false;
                }
            }

            // White King castles right
            if(col == 0 && !inCheck(0) && !whiteKingMoved && !whiteRRookMoved) {
                int i = 50, j = 70;
                for( ; i < j; i += 10) {
                    if((*ownPieces).count(i) == 1 || 
                        (*oppPieces).count(i) == 1) break;
                }
                if(i == j) {
                    i = 50;
                    for( ; i <= 60 && !movePutsKingInCheck(40, i, 0, ownPieces, oppPieces, true); i += 10);
                    if(i == 70) return false;
                }
            }

            // Black king castles left
            if(col == 1 && !inCheck(1) && !blackKingMoved && !blackLRookMoved) {
                int i = 17, j = 47;
                for( ; i < j; i += 10) {
                    if((*ownPieces).count(i) == 1 || 
                        (*oppPieces).count(i) == 1) break;
                }
                if(i == j) {
                    i = 37;
                    for( ; i >= 27 && !movePutsKingInCheck(47, i, 1, ownPieces, oppPieces, true); i -= 10);
                    if(i == 17) return false;
                }
            }

            // Black King castles right
            if(col == 1 && !inCheck(1) && !blackKingMoved && !blackRRookMoved) {
                int i = 57, j = 77;
                for( ; i < j; i += 10) {
                    if((*ownPieces).count(i) == 1 || 
                        (*oppPieces).count(i) == 1) break;
                }
                if(i == j) {
                    i = 50;
                    for( ; i <= 60 && !movePutsKingInCheck(47, i, 1, ownPieces, oppPieces, true); i += 10);
                    if(i == 77) return false;
                }
            }
        }
    }
    return true;
}

bool Board::inCheck(int col, int move) {
    map<int, char> *ownPieces = col == 0 ? &whitePieces : &blackPieces;
    map<int, char> *oppPieces = col == 0 ? &blackPieces : &whitePieces;

    int kingLetter, kingNum, kingLoc;
    // Initializes with arbitrary piece being checked if being taken
    if(move != -1) {
        kingLetter = move / 10;
        kingNum = move % 10;
        kingLoc = move;
    // Initializes with King as the piece being checked if being taken
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
        
        // Checks if can be taken by black pawn
        if(piece.second == 'p' && 
           (piece.first == kingLoc + 11 || piece.first == kingLoc - 9))  
                return true;
        
         // Checks if can be taken by white pawn
        else if(piece.second == 'P' && 
                (piece.first == kingLoc + 9 || piece.first == kingLoc - 11))
                return true;

        // Checks if can be taken by rook
        else if(piece.second == 'r' || piece.second == 'R') {
            // Opponent rook is on the same letter column
            if(kingLetter == pieceLetter) {
                int i = kingNum > pieceNum ? piece.first + 1 : kingLoc + 1;
                int j = kingNum > pieceNum ? kingLoc : piece.first;
                for( ; i < j; ++i) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            // Opponent rook is on the same number row
            else if(kingNum == pieceNum) {
                int i = kingLetter > pieceLetter ? piece.first + 10 : 
                                                   kingLoc + 10;
                int j = kingLetter > pieceLetter ? kingLoc : piece.first;
                for( ; i < j; i += 10) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
        }
        // Checks if can be taken by bishop
        else if(piece.second == 'b' || piece.second == 'B') {
            // Opponent bishop is on the same right diagonal
            if(kingNum - pieceNum == kingLetter - pieceLetter) {
                int i = kingNum < pieceNum ? kingLoc + 11 : piece.first + 11;
                int j = kingNum < pieceNum ? piece.first : kingLoc;
                for( ; i < j; i += 11) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            // Opponent bishop is on the same left diagonal
            else if(kingNum - pieceNum == pieceLetter - kingLetter) {
                int i = kingNum < pieceNum ? piece.first + 9 : kingLoc + 9;
                int j = kingNum < pieceNum ? kingLoc : piece.first;
                for( ; i < j; i += 9) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
        }
        // Checks if can be taken by queen
        else if(piece.second == 'q' || piece.second == 'Q') {
            // Opponent queen on the same letter column
            if(kingLetter == pieceLetter) {
                int i = kingNum > pieceNum ? piece.first + 1 : kingLoc + 1;
                int j = kingNum > pieceNum ? kingLoc : piece.first;
                for( ; i < j; ++i) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            // Opponent queen on the same number column
            else if(kingNum == pieceNum) {
                int i = kingLetter > pieceLetter ? piece.first + 10 : 
                                                   kingLoc + 10;
                int j = kingLetter > pieceLetter ? kingLoc : piece.first;
                for( ; i < j; i += 10) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1)
                        break;
                }
                if(i == j) return true;
            }
            // Opponent queen on the same right diagonal
            else if(kingNum - pieceNum == kingLetter - pieceLetter) {
                int i = kingNum < pieceNum ? kingLoc + 11 : piece.first + 11;
                int j = kingNum < pieceNum ? piece.first : kingLoc;
                for( ; i < j; i += 11) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
            // Opponent queen on the same left diagonal
            else if(kingNum - pieceNum == pieceLetter - kingLetter) {
                int i = kingNum < pieceNum ? piece.first + 9 : kingLoc + 9;
                int j = kingNum < pieceNum ? kingLoc : piece.first;
                for( ; i < j; i += 9) {
                    if(ownPieces->count(i) == 1 || oppPieces->count(i) == 1) 
                        break;
                }
                if(i == j) return true;
            }
        }

        // Checks if can be taken by knight
        else if((piece.second == 'n' || piece.second == 'N') &&
                (kingLoc == piece.first - 21 || kingLoc == piece.first - 19 ||
                kingLoc == piece.first - 8 || kingLoc == piece.first - 12 ||
                kingLoc == piece.first + 8 || kingLoc == piece.first + 12 ||
                kingLoc == piece.first + 19 || kingLoc == piece.first + 21)) 
                return true;

        // Checks if can be taken by king
        else if((piece.second == 'k' || piece.second == 'K') &&
                (kingLoc == piece.first + 1 || kingLoc == piece.first - 1 ||
                kingLoc == piece.first + 10 || kingLoc == piece.first - 10 ||
                kingLoc == piece.first + 11 || kingLoc == piece.first - 11 ||
                kingLoc == piece.first + 9 || kingLoc == piece.first - 9))
                return true;
    }
    // No piece could take it
    return false;
}

void Board::display() {
    tDisplay->display(); 
    gDisplay->display();
}
