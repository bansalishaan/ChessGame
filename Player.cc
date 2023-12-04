#include "Player.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

int charToInt(char num)
{
    string n;
    n += num;
    istringstream numStr{n};
    int numInt;
    numStr >> numInt;
    return numInt;
}

Human::Human() : Player{} {}
/*
vector<int> Human::getMove() {
    vector<int> move;

    string commandLine;
    getline(cin, commandLine);
    cout << commandLine << endl; // REMOVE
    istringstream input {commandLine};
    string piece, moveLoc, promotion;

    while(!(input >> piece >> moveLoc) || piece.length() != 2 || moveLoc.length() != 2 || (input >> promotion && promotion.length() != 1) || piece[0] < 97 || piece[0] > 104 || piece[1] < 48 || piece[1] > 57 || moveLoc[0] < 97 || moveLoc[0] > 104 || moveLoc[1] < 48 || moveLoc[1] > 57) {          // ADD CHECK FOR PROMOTION BEING VALID
        if(cin.fail()) {
            cin.clear();
            cin.ignore();
        }
        cout << "Please give a valid move (current square, new square)" << endl;
        getline(cin, commandLine);
        input = istringstream{commandLine};
    }
    move.emplace_back((piece[0] - 97) * 10 + charToInt(piece[1]));
    move.emplace_back((moveLoc[0] - 97) * 10 + charToInt(moveLoc[1]));

    if(!(promotion == "")) {    // ADD PROMOTION TO MOVE
        move.emplace_back();
    }

    return move;
} */

vector<int> Human::getMove(string start, string end)
{
    if (start.length() != 2 || end.length() != 2 || start[0] < 'a' || start[0] 
    > 'h' || start[1] < '1' || start[1] > '8' || end[0] < 'a' || end[0] > 
    'h' || end[1] < '1' || end[1] > '8')
    {
        cout << "Enter a valid location." << endl;
    }
    int first_start = start[0] - 'a';
    int second_start = start[1] - '1';
    int first_end = end[0] - 'a';
    int second_end = end[1] - '1';
    int start_location_int = first_start * 10 + second_start;
    int end_location_int = first_end * 10 + second_end;
    vector<int> move = {start_location_int, end_location_int};
    return move;
}

Bot::Bot(Board *b, int col) : board{b}, col{col} {}

template <typename T>
void Bot::shuffler(T *list)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng{seed};

    std::shuffle(list->begin(), list->end(), rng);
}

Bot1::Bot1(Board *b, int col) : Bot{b, col} {}

vector<int> Bot1::getMove(string start, string end) {
    // Initalizes own and opponent pieces
    map<int, char> *ownPieces = (col == 0 ? &board->whitePieces : 
                                            &board->blackPieces);
    map<int, char> *oppPieces = (col == 0 ? &board->blackPieces : 
                                            &board->whitePieces);
    // Collects the own piece's location and randomly shuffles them
    vector<int> mapLoc;
    for (auto i = ownPieces->begin(); i != ownPieces->end(); ++i)
        mapLoc.emplace_back(i->first);
    shuffler(&mapLoc);

    // Goes through shuffled list of own pieces one by one
    for (auto pieceLoc = mapLoc.begin(); pieceLoc != mapLoc.end(); ++pieceLoc) {
        char pieceType = (*ownPieces)[*pieceLoc];
        vector<vector<int>> movesList;

        // Piece is a white pawn
        if (pieceType == 'P') {
            // Pawn moves forward 2 squares
            if (*pieceLoc / 10 == 1 &&
                ownPieces->count(*pieceLoc + 1) == 0 &&
                ownPieces->count(*pieceLoc + 2) == 0 &&
                oppPieces->count(*pieceLoc + 1) == 0 &&
                oppPieces->count(*pieceLoc + 2) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 2, col,
                                            ownPieces, oppPieces, true)) 
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 2});

            // Pawn moves forward 1 square
            if (ownPieces->count(*pieceLoc + 1) == 0 &&
                oppPieces->count(*pieceLoc + 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1, col,
                                            ownPieces, oppPieces, true)) {

                // Promotion occurs
                if(*pieceLoc == 6) {
                    vector<int> promotion {'Q', 'R', 'B', 'N'};
                    for(auto i = promotion.begin(); i != promotion.end(); ++i) {
                        movesList.emplace_back(vector<int> {*pieceLoc, 
                        *pieceLoc + 1, *i});
                    }
                } else 
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 1});
            }

            // Pawn moves diagoanlly 1 square right without en passant
            if (oppPieces->count(*pieceLoc + 11) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                                            col, ownPieces, oppPieces, true)) {

                // Promotion occurs
                if(*pieceLoc == 6) {
                    vector<int> promotion {'Q', 'R', 'B', 'N'};
                    for(auto i = promotion.begin(); i != promotion.end(); ++i) {
                        movesList.emplace_back(vector<int> {*pieceLoc, 
                        *pieceLoc + 11, *i});
                    }
                } else 
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 11});
            }

            // Pawn moves diagonally 1 square left without en passant
            if (oppPieces->count(*pieceLoc - 9) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9, col,
                                            ownPieces, oppPieces, true)) {

                // Promotion occurs
                if(*pieceLoc == 6) {
                    vector<int> promotion {'Q', 'R', 'B', 'N'};
                    for(auto i = promotion.begin(); i != promotion.end(); ++i) {
                        movesList.emplace_back(vector<int> {*pieceLoc, 
                        *pieceLoc - 9, *i});
                    }
                } else 
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 9});
            }

            // Pawn performs en passant on the right
            if (*pieceLoc / 10 == 4 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc + 10) {

                oppPieces->erase(*pieceLoc + 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc,
                                                          *pieceLoc + 11, col, 
                                                          ownPieces, oppPieces, 
                                                          true);
                (*oppPieces)[*pieceLoc + 10] = 'p';
                if (!inCheck)
                    movesList.emplace_back(vector<int> {*pieceLoc, 
                                                        *pieceLoc + 11});
            }

            // Pawn performs en passant on the right
            if (*pieceLoc / 10 == 4 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc - 10) {

                oppPieces->erase(*pieceLoc - 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc,
                                                          *pieceLoc - 9, col, 
                                                          ownPieces, oppPieces, 
                                                          true);
                (*oppPieces)[*pieceLoc - 10] = 'p';
                if (!inCheck)
                    movesList.emplace_back(vector<int> {*pieceLoc, 
                                                        *pieceLoc - 9});
            }
        }

        // Piece is a black pawn
        else if (pieceType == 'p') {
            // Pawn moves forward 2 squares
            if (*pieceLoc / 10 == 6 &&
                ownPieces->count(*pieceLoc - 1) == 0 &&
                ownPieces->count(*pieceLoc - 2) == 0 &&
                oppPieces->count(*pieceLoc - 1) == 0 &&
                oppPieces->count(*pieceLoc - 2) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 2, col,
                                            ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 2});

            // Pawn moves forward 1 square
            if (ownPieces->count(*pieceLoc - 1) == 0 &&
                oppPieces->count(*pieceLoc - 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1, col,
                                            ownPieces, oppPieces, true)) {

                // Promotion occurs
                if(*pieceLoc == 1) {
                    vector<int> promotion {'q', 'r', 'b', 'n'};
                    for(auto i = promotion.begin(); i != promotion.end(); ++i) {
                        movesList.emplace_back(vector<int> {*pieceLoc, 
                        *pieceLoc - 1, *i});
                    }
                } else 
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 1});
            }

            // Pawn moves diagonally 1 square right without en passant
            if (oppPieces->count(*pieceLoc - 11) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                                            col, ownPieces, oppPieces, true)) {

                // Promotion occurs
                if(*pieceLoc == 1) {
                    vector<int> promotion {'q', 'r', 'b', 'n'};
                    for(auto i = promotion.begin(); i != promotion.end(); ++i) {
                        movesList.emplace_back(vector<int> {*pieceLoc, 
                        *pieceLoc - 11, *i});
                    }
                } else 
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 11});
            }

            // Pawn moves diagonally 1 square left without en passant
            if (oppPieces->count(*pieceLoc + 9) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9, col,
                                            ownPieces, oppPieces, true)) {

                // Promotion occurs
                if(*pieceLoc == 1) {
                    vector<int> promotion {'q', 'r', 'b', 'n'};
                    for(auto i = promotion.begin(); i != promotion.end(); ++i) {
                        movesList.emplace_back(vector<int> {*pieceLoc, 
                        *pieceLoc + 9, *i});
                    }
                } else 
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 9});
            }

            // Pawn performs en passant on the right
            if (*pieceLoc / 10 == 3 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc + 10) {

                oppPieces->erase(*pieceLoc + 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc,
                                                          *pieceLoc + 9, col, 
                                                          ownPieces, oppPieces, 
                                                          true);
                (*oppPieces)[*pieceLoc + 10] = 'P';
                if (!inCheck)
                    movesList.emplace_back(vector<int> {*pieceLoc, 
                                                        *pieceLoc + 9});
            }

            // Pawn performs en passant on the left
            if (*pieceLoc / 10 == 3 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc - 10) {

                oppPieces->erase(*pieceLoc - 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc,
                                                          *pieceLoc - 11, col, 
                                                          ownPieces, oppPieces, 
                                                          true);
                (*oppPieces)[*pieceLoc - 10] = 'P';
                if (!inCheck)
                    movesList.emplace_back(vector<int> {*pieceLoc, 
                                                        *pieceLoc - 11});
            }
        }

        // Piece is a rook
        else if (pieceType == 'r' || pieceType == 'R') {
            // Rook moves up
            for (int i = *pieceLoc + 1; i % 10 != 8 &&
                    ownPieces->count(i) == 0; ++i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Rook moves right
            for (int i = *pieceLoc + 10; i < 78 &&
                    ownPieces->count(i) == 0; i += 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Rook moves down
            for (int i = *pieceLoc - 1; i >= *pieceLoc / 10 * 10 &&
                                        ownPieces->count(i) == 0; --i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Rook moves left
            for (int i = *pieceLoc - 10; i >= 0 &&
                                         ownPieces->count(i) == 0; i -= 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            for(auto x : movesList)
                std::cout << x.at(0) << " " << x.at(1) << "\n";
        }

        else if (pieceType == 'b' || pieceType == 'B') {
            // Bishop moves right and up
            for (int i = *pieceLoc + 11; i < 78 && i % 10 != 8 &&
                                         ownPieces->count(i) == 0; i += 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Bishop moves right and down
            for (int i = *pieceLoc + 9; i < 78 && i % 10 != 9 &&
                    ownPieces->count(i) == 0; i += 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
 
                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Bishop moves left and down
            for (int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 &&
                                         ownPieces->count(i) == 0; i -= 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Bishop moves left and up
            for (int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 &&
                                        ownPieces->count(i) == 0; i -= 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }
        }

        // Piece is a queen
        else if (pieceType == 'q' || pieceType == 'Q') {
            // Queen moves up
            for (int i = *pieceLoc + 1; i % 10 != 8 &&
                    ownPieces->count(i) == 0; ++i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Queen moves right
            for (int i = *pieceLoc + 10; i < 78 &&
                    ownPieces->count(i) == 0; i += 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Queen moves down
            for (int i = *pieceLoc - 1; i >= *pieceLoc / 10 * 10 &&
                                        ownPieces->count(i) == 0; --i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Queen moves left
            for (int i = *pieceLoc - 10; i >= 0 &&
                                         ownPieces->count(i) == 0; i -= 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Queen moves right and up
            for (int i = *pieceLoc + 11; i < 78 && i % 10 != 8 &&
                                         ownPieces->count(i) == 0; i += 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Queen moves right and down
            for (int i = *pieceLoc + 9; i < 78 && i % 10 != 9 &&
                    ownPieces->count(i) == 0; i += 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
 
                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Queen moves left and down
            for (int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 &&
                                         ownPieces->count(i) == 0; i -= 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }

            // Queen moves left and up
            for (int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 &&
                                        ownPieces->count(i) == 0; i -= 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                movesList.emplace_back(vector<int> {*pieceLoc, i});
                if (oppPieces->count(i) == 1) break;
            }
        }

        // Piece is a knight
        else if (pieceType == 'n' || pieceType == 'N') {
            // Knight moves left 2, down 1
            if (*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc - 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 21,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 21});

            // Knight moves left 2, up 1
            if (*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc - 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 19,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 19});

            // Knight moves left 1, down 2
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 >= 2 &&
                ownPieces->count(*pieceLoc - 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 12,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 12});

            // Knight moves left 1, up 2
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 <= 5 &&
                ownPieces->count(*pieceLoc - 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 8,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 8});

            // Knight moves right 2, up 1
            if (*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc + 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 21,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 21});

            // Knight moves right 2, down 1
            if (*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc + 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 19,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 19});

            // Knight moves right 1, up 2
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 <= 5 &&
                ownPieces->count(*pieceLoc + 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 12,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 12});

            // Knight moves right 1, down 2
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 >= 2 &&
                ownPieces->count(*pieceLoc + 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 8,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 8});
        }

        // Piece is a king
        else if (pieceType == 'k' || pieceType == 'K') {
            // King moves up 1
            if (*pieceLoc % 10 != 7 && ownPieces->count(*pieceLoc + 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 1});

            // King moves down 1
            if (*pieceLoc % 10 != 0 && ownPieces->count(*pieceLoc - 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 1});

            // King moves right 1
            if (*pieceLoc / 10 != 7 && ownPieces->count(*pieceLoc + 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 10,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 10});

            // King moves left 1
            if (*pieceLoc / 10 != 0 && ownPieces->count(*pieceLoc - 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 10,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 10});

            // King moves right 1, up 1
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc + 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 11});

            // King moves down 1, left 1
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc - 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 11});

            // King moves right 1, down 1
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc + 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc + 9});

            // King moves left 1, up 1
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc - 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9,
                                            col, ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int> {*pieceLoc, *pieceLoc - 9});
            
            // White king castles left
            if(col == 0 && !board->whiteKingMoved && !board->whiteLRookMoved) {
                int i = *pieceLoc - 10;
                for( ; i >= 10 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && (i == 10 || 
                       !board->movePutsKingInCheck(40, i, 0, 
                       ownPieces, oppPieces, true)); i -= 10);
                if(i == 0) movesList.emplace_back(vector<int> {*pieceLoc, 20});
            }

            // White king castles right
            if(col == 0 && !board->whiteKingMoved && !board->whiteRRookMoved) {
                int i = *pieceLoc + 10;
                for( ; i <= 60 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && !board->movePutsKingInCheck(
                       40, i, 0, ownPieces, oppPieces, true); i += 10);
                if(i == 70) movesList.emplace_back(vector<int> {*pieceLoc, 60});
            }

            // Black king castles left
            if(col == 1 && !board->blackKingMoved && !board->blackLRookMoved) {
                int i = *pieceLoc - 10;
                for( ; i >= 17 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && (i == 17 || !board->movePutsKingInCheck(47, i, 1, 
                       ownPieces, oppPieces, true)); i -= 10);
                if(i == 7) movesList.emplace_back(vector<int> {*pieceLoc, 27});
            }

            // Black king castles right
            if(col == 1 && !board->blackKingMoved && !board->blackRRookMoved) {
                int i = *pieceLoc + 10;
                for( ; i <= 67 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && !board->movePutsKingInCheck(
                       47, i, 1, ownPieces, oppPieces, true); i += 10);
                if(i == 77) movesList.emplace_back(vector<int> {*pieceLoc, 67});
            }
        }

        // If there are no moves possible, check next piece
        if (movesList.size() == 0) continue;
        std::cout << movesList.size();
        // Shuffles available moves
        shuffler(&movesList);
        return movesList.at(0);
    }
}

Bot2::Bot2(Board *b, int col) : Bot1{b, col} {}

vector<int> Bot2::getMove(string start, string end) {
    // Initializes own and opponent pieces
    map<int, char> *ownPieces = (col == 0 ? &board->whitePieces : 
                                            &board->blackPieces);
    map<int, char> *oppPieces = (col == 0 ? &board->blackPieces : 
                                            &board->whitePieces);
    // Collects the own piece's location and randomly shuffles them
    vector<int> mapLoc;
    for (auto i = ownPieces->begin(); i != ownPieces->end(); ++i)
        mapLoc.emplace_back(i->first);
    shuffler(&mapLoc);

    vector<vector<int>> movesList;
    // Goes through shuffled list of own pieces one by one
    for (auto pieceLoc = mapLoc.begin(); pieceLoc != mapLoc.end(); ++pieceLoc) {
        std::cout << *pieceLoc << " DONE\n";
        char pieceType = (*ownPieces)[*pieceLoc];

        // Piece is a white pawn
        if (pieceType == 'P') {
            // Pawn moves forward 2 squares and puts opponent king in check
            if (*pieceLoc / 10 == 1 &&
                ownPieces->count(*pieceLoc + 1) == 0 &&
                ownPieces->count(*pieceLoc + 2) == 0 &&
                oppPieces->count(*pieceLoc + 1) == 0 &&
                oppPieces->count(*pieceLoc + 2) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 2, col,
                                            ownPieces, oppPieces, true) &&
                board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 2, 1,
                                           ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int>{*pieceLoc, *pieceLoc + 2});

            // Pawn moves forward 1 square and puts opponent king in check
            if (ownPieces->count(*pieceLoc + 1) == 0 &&
                oppPieces->count(*pieceLoc + 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1, col,
                                            ownPieces, oppPieces, true)) {
                
                // Promotion occurs
                if (*pieceLoc / 10 == 6) {
                    vector<int> promotion{'Q', 'B', 'N', 'R'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i) 
                    {
                        (*ownPieces)[*pieceLoc] = *i;
                        if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 
                        1, 1, ownPieces, oppPieces, true))
                            movesList.emplace_back(
                                vector<int>{*pieceLoc, *pieceLoc + 1, *i});
                    }
                    (*ownPieces)[*pieceLoc] = 'P';
                }
                else if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1, 
                1, ownPieces, oppPieces, true))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 1});
            }

            // Pawn moves 1 square diagonally right without en passant
            if (oppPieces->count(*pieceLoc + 11) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                                            col, ownPieces, oppPieces, true)) {
                // Promotion occurs
                if (*pieceLoc / 10 == 6) {
                    vector<int> promotion{'Q', 'B', 'N', 'R'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                        movesList.emplace_back(
                                vector<int>{*pieceLoc, *pieceLoc + 11, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 11});
            }

            // Pawn moves 1 square diagonally left without en passant
            if (oppPieces->count(*pieceLoc - 9) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9, col,
                                            ownPieces, oppPieces, true)) {
                // Promotion occurs
                if (*pieceLoc / 10 == 6) {
                    vector<int> promotion{'Q', 'B', 'N', 'R'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                        movesList.emplace_back(
                            vector<int>{*pieceLoc, *pieceLoc - 9, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc - 9});
            }

            // Pawn performs en passant on the right
            if (*pieceLoc / 10 == 4 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc + 10) {
                
                oppPieces->erase(*pieceLoc + 10);
                bool whiteInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc + 11, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc + 10] = 'p';
                if(!whiteInCheck) movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 11});
            }

            // Pawn performs en passant on the left
            if (*pieceLoc / 10 == 4 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc - 10) {
                
                oppPieces->erase(*pieceLoc - 10);
                bool whiteInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc - 9, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc - 10] = 'p';
                if(!whiteInCheck) movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc - 9});
            }
        }

        // Piece is a black pawn
        else if (pieceType == 'p') {
            // Pawn moves forward 2 squares and puts opponent king in check
            if (*pieceLoc / 10 == 6 &&
                ownPieces->count(*pieceLoc - 1) == 0 &&
                ownPieces->count(*pieceLoc - 2) == 0 &&
                oppPieces->count(*pieceLoc - 1) == 0 &&
                oppPieces->count(*pieceLoc - 2) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 2, col,
                                            ownPieces, oppPieces, true) &&
                board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 2, 0,
                                           ownPieces, oppPieces, true))
                movesList.emplace_back(vector<int>{*pieceLoc, *pieceLoc - 2});

            // Pawn moves forward 1 square and puts opponent king in check
            if (ownPieces->count(*pieceLoc - 1) == 0 &&
                oppPieces->count(*pieceLoc - 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1, col,
                                            ownPieces, oppPieces, true)) {

                if (*pieceLoc / 10 == 1) {
                    vector<int> promotion{'q', 'b', 'n', 'r'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i) 
                    {
                        (*ownPieces)[*pieceLoc] = *i;
                        if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 
                        1, 0, ownPieces, oppPieces, true))
                            movesList.emplace_back(
                                vector<int>{*pieceLoc, *pieceLoc - 1, *i});
                    }
                    (*ownPieces)[*pieceLoc] = 'p';
                }
                else if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1, 
                0, ownPieces, oppPieces, true)) movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc - 1});
            }

            // Pawn moves 1 square diagonally left without en passant
            if (oppPieces->count(*pieceLoc - 11) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                                            col, ownPieces, oppPieces, true)) {
                // Promotion occurs
                if (*pieceLoc / 10 == 1) {
                    vector<int> promotion{'q', 'b', 'n', 'r'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                            movesList.emplace_back(
                                vector<int>{*pieceLoc, *pieceLoc - 11, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc - 11});
            }

            // Pawn moves 1 square diagonally right without en passant
            if (oppPieces->count(*pieceLoc + 9) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9, col,
                                            ownPieces, oppPieces, true)) {

                if (*pieceLoc / 10 == 1) {
                    vector<int> promotion{'q', 'b', 'n', 'r'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                        movesList.emplace_back(
                            vector<int>{*pieceLoc, *pieceLoc + 9, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 9});
            }

            // Pawn performs en passant on the right
            if (*pieceLoc / 10 == 3 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc + 10) {

                oppPieces->erase(*pieceLoc + 10);
                bool blackInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc + 9, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc + 10] = 'P';
                if (!blackInCheck) movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 9});
            }

            // Pawn performs en passant on the left
            if (*pieceLoc / 10 == 3 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc - 10) {

                oppPieces->erase(*pieceLoc - 10);
                bool blackInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc - 11, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc - 10] = 'P';
                if (!blackInCheck) movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 11});
            }
        }

        // Piece is a rook
        else if (pieceType == 'r' || pieceType == 'R') {
            // Rook moves up
            for (int i = *pieceLoc + 1; i % 10 != 8 &&
                                        ownPieces->count(i) == 0; ++i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
            
            // Rook moves right
            for (int i = *pieceLoc + 10; i < 78 &&
                                         ownPieces->count(i) == 0; i += 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Rook moves down
            for (int i = *pieceLoc - 1; i >= *pieceLoc / 10 * 10 &&
                                        ownPieces->count(i) == 0; --i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Rook moves left
            for (int i = *pieceLoc - 10; i >= 0 &&
                                         ownPieces->count(i) == 0; i -= 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
        }

        // Piece is a bishop
        else if (pieceType == 'b' || pieceType == 'B') {
            // Bishop moves up and right
            for (int i = *pieceLoc + 11; i < 78 && i % 10 != 8 &&
                                         ownPieces->count(i) == 0; i += 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Bishop moves down and right
            for (int i = *pieceLoc + 9; i < 78 && i % 10 != 9 &&
                                        ownPieces->count(i) == 0; i += 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Bishop moves down and left
            for (int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 &&
                                         ownPieces->count(i) == 0; i -= 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Bishop moves down and right
            for (int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 &&
                                        ownPieces->count(i) == 0; i -= 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
        }

        // Piece is a queen
        else if (pieceType == 'q' || pieceType == 'Q') {
            // Queen moves up
            for (int i = *pieceLoc + 1; i % 10 != 8 &&
                                        ownPieces->count(i) == 0; ++i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
            
            // Queen moves right
            for (int i = *pieceLoc + 10; i < 78 &&
                                         ownPieces->count(i) == 0; i += 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves down
            for (int i = *pieceLoc - 1; i >= *pieceLoc / 10 * 10 &&
                                        ownPieces->count(i) == 0; --i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves left
            for (int i = *pieceLoc - 10; i >= 0 &&
                                         ownPieces->count(i) == 0; i -= 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves up and right
            for (int i = *pieceLoc + 11; i < 78 && i % 10 != 8 &&
                                         ownPieces->count(i) == 0; i += 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves down and right
            for (int i = *pieceLoc + 9; i < 78 && i % 10 != 9 &&
                                        ownPieces->count(i) == 0; i += 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;

                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves down and left
            for (int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 &&
                                         ownPieces->count(i) == 0; i -= 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves down and right
            for (int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 &&
                                        ownPieces->count(i) == 0; i -= 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
        }

        // Piece is a knight
        else if (pieceType == 'n' || pieceType == 'N') {
            // Knight moves 2 left, 1 down
            if (*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc - 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 21,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 21,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 21) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 21});
            }

            // Knight moves 2 left, 1 up
            if (*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc - 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 19,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 19,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 19) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 19});
            }

            // Knight moves 1 left, 2 down
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 >= 2 &&
                ownPieces->count(*pieceLoc - 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 12,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 12,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 12) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 12});
            }

            // Knight moves 1 left, 2 up
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 <= 5 &&
                ownPieces->count(*pieceLoc - 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 8,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 8,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 8) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 8});
            }

            // Knight moves 2 right, 1 up
            if (*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc + 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 21,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 21,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 21) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 21});
            }

            // Knight moves 2 right, 1 down
            if (*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc + 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 19,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 19,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 19) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 19});
            }

            // Knight moves 1 right, 2 up
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 <= 5 &&
                ownPieces->count(*pieceLoc + 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 12,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 12,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 12) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 12});
            }

            // Knight moves 1 right, 2 down
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 >= 2 &&
                ownPieces->count(*pieceLoc + 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 8,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 8,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 8) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 8});
            }
        }

        // Piece is a king
        else if (pieceType == 'k' || pieceType == 'K') {
            // King moves up 1
            if (*pieceLoc % 10 != 7 && ownPieces->count(*pieceLoc + 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 1) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 1});
            }

            // King moves down 1
            if (*pieceLoc % 10 != 0 && ownPieces->count(*pieceLoc - 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 1) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 1});
            }

            // King moves right 1
            if (*pieceLoc / 10 != 7 && ownPieces->count(*pieceLoc + 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 10,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 10,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 10) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 10});
            }

            // King moves left 1
            if (*pieceLoc / 10 != 0 && ownPieces->count(*pieceLoc - 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 10,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 10,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 10) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 10});
            }

            // King moves up 1, right 1
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc + 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 11) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 11});
            }

            // King moves left 1, down 1
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc - 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 11) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 11});
            }

            // King right 1, down 1
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc + 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc + 9) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 9});
            }

            // King moves left 1, up 1
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc - 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9,
                col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        oppPieces->count(*pieceLoc - 9) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 9});
            }
            
            // White king castles left
            if(col == 0 && !board->whiteKingMoved && !board->whiteLRookMoved) {
                int i = *pieceLoc - 10;
                for( ; i >= 10 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && (i == 10 || 
                       !board->movePutsKingInCheck(40, i, 0, 
                       ownPieces, oppPieces, true)); i -= 10);
                if(i == 0) {
                    ownPieces->erase(0);
                    (*ownPieces)[30] = 'R';
                    bool inCheck = board->movePutsKingInCheck(40, 20, 1, ownPieces, oppPieces, true);
                    ownPieces->erase(30);
                    (*ownPieces)[0] = 'R';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 20});
                }
            }

            // White king castles right
            if(col == 0 && !board->whiteKingMoved && !board->whiteRRookMoved) {
                int i = *pieceLoc + 10;
                for( ; i <= 60 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && !board->movePutsKingInCheck(
                       40, i, 0, ownPieces, oppPieces, true); i += 10);
                if(i == 70) {
                    ownPieces->erase(70);
                    (*ownPieces)[50] = 'R';
                    bool inCheck = board->movePutsKingInCheck(40, 60, 1, ownPieces, oppPieces, true);
                    ownPieces->erase(50);
                    (*ownPieces)[70] = 'R';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 60});
                }
            }

            // Black king castles left
            if(col == 1 && !board->blackKingMoved && !board->blackLRookMoved) {
                int i = *pieceLoc - 10;
                for( ; i >= 17 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && (i == 17 || !board->movePutsKingInCheck(47, i, 1, 
                       ownPieces, oppPieces, true)); i -= 10);
                if(i == 7) {
                    ownPieces->erase(7);
                    (*ownPieces)[37] = 'r';
                    bool inCheck = board->movePutsKingInCheck(47, 27, 0, ownPieces, oppPieces, true);
                    ownPieces->erase(37);
                    (*ownPieces)[7] = 'r';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 27});
                }
            }

            // Black king castles right
            if(col == 1 && !board->blackKingMoved && !board->blackRRookMoved) {
                int i = *pieceLoc + 10;
                for( ; i <= 67 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && !board->movePutsKingInCheck(
                       47, i, 1, ownPieces, oppPieces, true); i += 10);
                if(i == 77) {
                    ownPieces->erase(77);
                    (*ownPieces)[57] = 'r';
                    bool inCheck = board->movePutsKingInCheck(47, 67, 0, ownPieces, oppPieces, true);
                    ownPieces->erase(57);
                    (*ownPieces)[77] = 'r';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 67});
                }
            }
        }
    }
    std::cout << "FORLOOPENDS\n";
    // There is a preferred move available
    if (movesList.size() != 0) {
        shuffler(&movesList);
        std::cout << movesList.at(0).at(0) << " " << movesList.at(0).at(1) << "\n";
        return movesList.at(0);
    // Do a random move
    } else {
        std::cout << "BOT!\n";
        return getMove(start, end);
    }
}


Bot3::Bot3(Board *b, int col) : Bot1{b, col} {}

bool Bot3::avoidsCapture(int loc, int move) {
    map<int, char> *ownPieces = (col == 0 ? &board->whitePieces : 
                                            &board->blackPieces);
    map<int, char> *oppPieces = (col == 0 ? &board->blackPieces : 
                                            &board->whitePieces);
    char oppPiece = '\0';
    if(oppPieces->count(move) == 1) {
        oppPiece = (*oppPieces)[move];
        oppPieces->erase(move);
    }
    (*ownPieces)[move] = (*ownPieces)[loc];
    ownPieces->erase(loc);
    bool captured = board->inCheck(col, move);
    if(oppPiece != '\0') (*oppPieces)[move] = oppPiece;
    (*ownPieces)[loc] = (*ownPieces)[move];
    ownPieces->erase(move);

    if(captured) return false;
    return true;
}

vector<int> Bot3::getMove(string start, string end) {
    // Initializes own and opponent pieces
    map<int, char> *ownPieces = (col == 0 ? &board->whitePieces : 
                                            &board->blackPieces);
    map<int, char> *oppPieces = (col == 0 ? &board->blackPieces : 
                                            &board->whitePieces);
    // Gets location of all the pieces in own pieces
    vector<int> mapLoc;
    for (auto i = ownPieces->begin(); i != ownPieces->end(); ++i)
        mapLoc.emplace_back(i->first);
    shuffler(&mapLoc);

    vector<vector<int>> movesList;

    // Goes through shuffled list of own pieces one by one
    for (auto pieceLoc = mapLoc.begin(); pieceLoc != mapLoc.end(); ++pieceLoc) {
        char pieceType = (*ownPieces)[*pieceLoc];

        // Piece is a white pawn
        if (pieceType == 'P') {
            // Pawn moves forward 2 squares
            if (*pieceLoc / 10 == 1 &&
                ownPieces->count(*pieceLoc + 1) == 0 &&
                ownPieces->count(*pieceLoc + 2) == 0 &&
                oppPieces->count(*pieceLoc + 1) == 0 &&
                oppPieces->count(*pieceLoc + 2) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 2, col,
                                            ownPieces, oppPieces, true) &&
                (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 2, 1,
                        ownPieces, oppPieces, true) || 
                (oppPieces->count(*pieceLoc + 12) == 0 &&
                 oppPieces->count(*pieceLoc - 8) == 0 &&
                 avoidsCapture(*pieceLoc, *pieceLoc + 2))))
                movesList.emplace_back(vector<int>{*pieceLoc, *pieceLoc + 2});

            // Pawn moves forward 1 square
            if (ownPieces->count(*pieceLoc + 1) == 0 &&
                oppPieces->count(*pieceLoc + 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1, col,
                                            ownPieces, oppPieces, true)) {

                // Promotion occurs
                if (*pieceLoc / 10 == 6) {
                    vector<int> promotion{'Q', 'B', 'N', 'R'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i) 
                    {
                        (*ownPieces)[*pieceLoc] = *i;
                        if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 
                        1, 1, ownPieces, oppPieces, true) ||
                        avoidsCapture(*pieceLoc, *pieceLoc + 1))
                            movesList.emplace_back(
                                vector<int>{*pieceLoc, *pieceLoc + 1, *i});
                    }
                    (*ownPieces)[*pieceLoc] = 'P';
                }
                else if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1, 
                1, ownPieces, oppPieces, true) || 
                avoidsCapture(*pieceLoc, *pieceLoc + 1))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 1});
            }

            // Pawn moves 1 square diagonally right without en passant
            if (oppPieces->count(*pieceLoc + 11) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                                            col, ownPieces, oppPieces, true)) {

                // Promotion occurs
                if (*pieceLoc / 10 == 6) {
                    vector<int> promotion{'Q', 'B', 'N', 'R'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                        movesList.emplace_back(
                            vector<int>{*pieceLoc, *pieceLoc + 11, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 11});
            }

            // Pawn moves 1 square diagonally left without en passant
            if (oppPieces->count(*pieceLoc - 9) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9, col,
                                            ownPieces, oppPieces, true)) {
                // Promotion occurs
                if (*pieceLoc / 10 == 6) {
                    vector<int> promotion{'Q', 'B', 'N', 'R'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                        movesList.emplace_back(
                            vector<int>{*pieceLoc, *pieceLoc - 9, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc - 9});
            }

            // Pawn performs en passant on the right
            if (*pieceLoc / 10 == 4 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc + 10) {
                
                oppPieces->erase(*pieceLoc + 10);
                bool whiteInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc + 11, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc + 10] = 'p';
                if(!whiteInCheck) movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 11});
            }

            // Pawn performs en passant on the left
            if (*pieceLoc / 10 == 4 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc - 10) {
                
                oppPieces->erase(*pieceLoc - 10);
                bool whiteInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc - 9, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc - 10] = 'p';
                if(!whiteInCheck) movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc - 9});
            }
        }

        // Piece is a black pawn
        else if (pieceType == 'p') {
            // Pawn moves forward 2 squares
            if (*pieceLoc / 10 == 6 &&
                ownPieces->count(*pieceLoc - 1) == 0 &&
                ownPieces->count(*pieceLoc - 2) == 0 &&
                oppPieces->count(*pieceLoc - 1) == 0 &&
                oppPieces->count(*pieceLoc - 2) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 2, col,
                                            ownPieces, oppPieces, true) &&
                (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 2, 0,
                                           ownPieces, oppPieces, true) ||
                (oppPieces->count(*pieceLoc - 12) == 0 &&
                 oppPieces->count(*pieceLoc + 8) == 0 &&
                 avoidsCapture(*pieceLoc, *pieceLoc - 2))))
                movesList.emplace_back(vector<int>{*pieceLoc, *pieceLoc - 2});

            // Pawn moves forward 1 square
            if (ownPieces->count(*pieceLoc - 1) == 0 &&
                oppPieces->count(*pieceLoc - 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1, col,
                                            ownPieces, oppPieces, true)) {

                if (*pieceLoc / 10 == 1) {
                    vector<int> promotion{'q', 'b', 'n', 'r'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i) 
                    {
                        (*ownPieces)[*pieceLoc] = *i;
                        if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 
                        1, 0, ownPieces, oppPieces, true) ||
                        avoidsCapture(*pieceLoc, *pieceLoc - 1))
                            movesList.emplace_back(
                                vector<int>{*pieceLoc, *pieceLoc - 1, *i});
                    }
                    (*ownPieces)[*pieceLoc] = 'p';
                }
                else if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1, 
                0, ownPieces, oppPieces, true) ||
                avoidsCapture(*pieceLoc, *pieceLoc - 1))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 1});
            }

            // Pawn moves 1 square diagonally left without en passant
            if (oppPieces->count(*pieceLoc - 11) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                                            col, ownPieces, oppPieces, true)) {

                if (*pieceLoc / 10 == 1) {
                    vector<int> promotion{'q', 'b', 'n', 'r'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                            movesList.emplace_back(
                                vector<int>{*pieceLoc, *pieceLoc - 11, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc - 11});
            }

            // Pawn moves 1 square diagonally right without en passant
            if (oppPieces->count(*pieceLoc + 9) == 1 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9, col,
                                            ownPieces, oppPieces, true)) {

                if (*pieceLoc / 10 == 1) {
                    vector<int> promotion{'q', 'b', 'n', 'r'};
                    for (auto i = promotion.begin(); i != promotion.end(); ++i)
                        movesList.emplace_back(
                            vector<int>{*pieceLoc, *pieceLoc + 9, *i});
                } else movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 9});
            }

            // Pawn performs en passant on the right
            if (*pieceLoc / 10 == 3 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc + 10) {

                oppPieces->erase(*pieceLoc + 10);
                bool blackInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc + 9, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc + 10] = 'P';
                if (!blackInCheck) movesList.emplace_back(
                    vector<int>{*pieceLoc, *pieceLoc + 9});
            }

            // Pawn performs en passant on the left
            if (*pieceLoc / 10 == 3 &&
                board->enPassantPawns.size() == 1 &&
                board->enPassantPawns.at(0) == *pieceLoc - 10) {

                oppPieces->erase(*pieceLoc - 10);
                bool blackInCheck = board->movePutsKingInCheck(*pieceLoc,
                                                               *pieceLoc - 11, 
                                                               col, ownPieces, 
                                                               oppPieces, true);
                (*oppPieces)[*pieceLoc - 10] = 'P';
                if (!blackInCheck) movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 11});
            }
        }

        // Piece is a rook
        else if (pieceType == 'r' || pieceType == 'R') {
            // Rook moves up
            for (int i = *pieceLoc + 1; i % 10 != 8 &&
                                        ownPieces->count(i) == 0; ++i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Rook moves right
            for (int i = *pieceLoc + 10; i < 78 &&
                                         ownPieces->count(i) == 0; i += 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Rook moves down
            for (int i = *pieceLoc - 1; i >= *pieceLoc / 10 * 10 &&
                                        ownPieces->count(i) == 0; --i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Rook moves left
            for (int i = *pieceLoc - 10; i >= 0 &&
                                         ownPieces->count(i) == 0; i -= 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
        }

        // Piece is a bishop
        else if (pieceType == 'b' || pieceType == 'B') {
            // Bishop moves right and up
            for (int i = *pieceLoc + 11; i < 78 && i % 10 != 8 &&
                                         ownPieces->count(i) == 0; i += 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Bishop moves right and down
            for (int i = *pieceLoc + 9; i < 78 && i % 10 != 9 &&
                                        ownPieces->count(i) == 0; i += 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Bishop moves left and down
            for (int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 &&
                                         ownPieces->count(i) == 0; i -= 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Bishop moves left and up
            for (int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 &&
                                        ownPieces->count(i) == 0; i -= 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
        }

        // Piece is a queen
        else if (pieceType == 'q' || pieceType == 'Q') {
            // Queen moves up
            for (int i = *pieceLoc + 1; i % 10 != 8 &&
                                        ownPieces->count(i) == 0; ++i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves right
            for (int i = *pieceLoc + 10; i < 78 &&
                                         ownPieces->count(i) == 0; i += 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves down
            for (int i = *pieceLoc - 1; i >= *pieceLoc / 10 * 10 &&
                                        ownPieces->count(i) == 0; --i) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves left
            for (int i = *pieceLoc - 10; i >= 0 &&
                                         ownPieces->count(i) == 0; i -= 10) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves right and up
            for (int i = *pieceLoc + 11; i < 78 && i % 10 != 8 &&
                                         ownPieces->count(i) == 0; i += 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves right and down
            for (int i = *pieceLoc + 9; i < 78 && i % 10 != 9 &&
                                        ownPieces->count(i) == 0; i += 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                        avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves left and down
            for (int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 &&
                                         ownPieces->count(i) == 0; i -= 11) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }

            // Queen moves left and up
            for (int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 &&
                                        ownPieces->count(i) == 0; i -= 9) {

                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true) &&
                    oppPieces->count(i) == 1) break;
                if (board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces,
                                               oppPieces, true)) continue;
                
                if (oppPieces->count(i) == 1) {
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
                    break;
                }
                if (board->movePutsKingInCheck(*pieceLoc, i, 
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    avoidsCapture(*pieceLoc, i))
                    movesList.emplace_back(vector<int>{*pieceLoc, i});
            }
        }

        // Piece is a knight
        else if (pieceType == 'n' || pieceType == 'N') {
            // Knight moves 2 left, 1 down
            if (*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc - 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 21,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 21,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 21) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc - 21))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 21});
            }

            // Knight moves 2 left, 1 up
            if (*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc - 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 19,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 19,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 19) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc - 19))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 19});
            }

            // Knight moves 1 left, 2 down
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 >= 2 &&
                ownPieces->count(*pieceLoc - 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 12,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 12,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 12) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc - 12))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 12});

            }

            // Knight moves 1 left, 2 up
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 <= 5 &&
                ownPieces->count(*pieceLoc - 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 8,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 8,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 8) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc - 8))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 8});
            }

            // Knight moves 2 right, 1 up
            if (*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc + 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 21,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 21,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 21) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc + 21))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 21});
            }

            // Knight moves 2 right, 1 down
            if (*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc + 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 19,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 19,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 19) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc + 19))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 19});
            }

            // Knight moves 1 right, 2 up
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 <= 5 &&
                ownPieces->count(*pieceLoc + 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 12,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 12,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 12) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc + 12))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 12});
            }

            // Knight moves 1 right, 2 down
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 >= 2 &&
                ownPieces->count(*pieceLoc + 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 8,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 8,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 8) == 1 ||
                    avoidsCapture(*pieceLoc, *pieceLoc + 8))
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 8});
            }
        }

        // Piece is a king
        else if (pieceType == 'k' || pieceType == 'K') {
            // King moves 1 up
            if (*pieceLoc % 10 != 7 && ownPieces->count(*pieceLoc + 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 1) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 1});
            }

            // King moves 1 down
            if (*pieceLoc % 10 != 0 && ownPieces->count(*pieceLoc - 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 1) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 1});
            }

            // King moves 1 right
            if (*pieceLoc / 10 != 7 && ownPieces->count(*pieceLoc + 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 10,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 10,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 10) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 10});
            }

            // King moves 1 left
            if (*pieceLoc / 10 != 0 && ownPieces->count(*pieceLoc - 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 10,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 10,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 10) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 10});
            }

            // King moves 1 right, 1 up
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc + 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 11) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 11});
            }

            // King moves 1 left, 1 down
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc - 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 11) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 11});
            }

            // King moves 1 right, 1 down
            if (*pieceLoc / 10 != 7 && *pieceLoc % 10 != 0 &&
                ownPieces->count(*pieceLoc + 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc + 9) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc + 9});
            }

            // King moves 1 left, 1 up
            if (*pieceLoc / 10 != 0 && *pieceLoc % 10 != 7 &&
                ownPieces->count(*pieceLoc - 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9,
                                            col, ownPieces, oppPieces, true)) {

                if (board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9,
                        col == 0 ? 1 : 0, ownPieces, oppPieces, true) ||
                    oppPieces->count(*pieceLoc - 9) == 1)
                    movesList.emplace_back(
                        vector<int>{*pieceLoc, *pieceLoc - 9});
            }

            // White king castles left
            if(col == 0 && !board->whiteKingMoved && !board->whiteLRookMoved) {
                int i = *pieceLoc - 10;
                for( ; i >= 10 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && (i == 10 || 
                       !board->movePutsKingInCheck(40, i, 0, 
                       ownPieces, oppPieces, true)); i -= 10);
                if(i == 0) {
                    ownPieces->erase(0);
                    (*ownPieces)[30] = 'R';
                    bool inCheck = board->movePutsKingInCheck(40, 20, 1, ownPieces, oppPieces, true);
                    ownPieces->erase(30);
                    (*ownPieces)[0] = 'R';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 20});
                }
            }

            // White king castles right
            if(col == 0 && !board->whiteKingMoved && !board->whiteRRookMoved) {
                int i = *pieceLoc + 10;
                for( ; i <= 60 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && !board->movePutsKingInCheck(
                       40, i, 0, ownPieces, oppPieces, true); i += 10);
                if(i == 70) {
                    ownPieces->erase(70);
                    (*ownPieces)[50] = 'R';
                    bool inCheck = board->movePutsKingInCheck(40, 60, 1, ownPieces, oppPieces, true);
                    ownPieces->erase(50);
                    (*ownPieces)[70] = 'R';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 60});
                }
            }

            // Black king castles left
            if(col == 1 && !board->blackKingMoved && !board->blackLRookMoved) {
                int i = *pieceLoc - 10;
                for( ; i >= 17 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && (i == 17 || !board->movePutsKingInCheck(47, i, 1, 
                       ownPieces, oppPieces, true)); i -= 10);
                if(i == 7) {
                    ownPieces->erase(7);
                    (*ownPieces)[37] = 'r';
                    bool inCheck = board->movePutsKingInCheck(47, 27, 0, ownPieces, oppPieces, true);
                    ownPieces->erase(37);
                    (*ownPieces)[7] = 'r';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 27});
                }
            }

            // Black king castles right
            if(col == 1 && !board->blackKingMoved && !board->blackRRookMoved) {
                int i = *pieceLoc + 10;
                for( ; i <= 67 && ownPieces->count(i) == 0 &&
                       oppPieces->count(i) == 0 && !board->movePutsKingInCheck(
                       47, i, 1, ownPieces, oppPieces, true); i += 10);
                if(i == 77) {
                    ownPieces->erase(77);
                    (*ownPieces)[57] = 'r';
                    bool inCheck = board->movePutsKingInCheck(47, 67, 0, ownPieces, oppPieces, true);
                    ownPieces->erase(57);
                    (*ownPieces)[77] = 'r';
                    if(inCheck) movesList.emplace_back(
                        vector<int> {*pieceLoc, 67});
                }
            }
        }
    }
    std::cout << "FORLOOPENDS\n";
    if (movesList.size() != 0) {
        shuffler(&movesList);
        std::cout << movesList.at(0).at(0) << " " << movesList.at(0).at(1) << "\n";
        return movesList.at(0);
    } else {
        std::cout << "BOT!\n";
        return getMove(start, end);
    }
}
