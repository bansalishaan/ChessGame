#include "Player.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

int charToInt(char num) {
    string n;
    n += num;
    istringstream numStr {n};
    int numInt;
    numStr >> numInt;
    return numInt;
}

Human::Human(): Player{} {}
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


vector<int> Human::getMove(string start, string end) {
    if(start.length() != 2 || end.length() != 2 || start[0] < 'a' || start[0] > 'h' || start[1] < '1' || start[1] > '8' || end[0] < 'a' || end[0] > 'h' || end[1] < '1' || end[1] > '8') {
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

Bot::Bot(Board *b, int col): board{b}, col{col} {}

Bot1::Bot1(Board *b, int col): Bot{b, col} {}

template <typename T>
void Bot1::randNumGenerator(T *list) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng{seed};

    std::shuffle(list->begin(), list->end(), rng);
}

vector<int> Bot1::getMove() {
    map<int, char> *ownPieces = (col == 0 ? &board->whitePieces : 
                                            &board->blackPieces);
    map<int, char> *oppPieces = (col == 0 ? &board->blackPieces :
                                            &board->whitePieces);
    vector<int> mapLoc;
    for(auto i = ownPieces->begin(); i != ownPieces->end(); ++i) 
        mapLoc.emplace_back(i->first);
    randNumGenerator(&mapLoc);

    for(auto pieceLoc = mapLoc.begin(); pieceLoc != mapLoc.end(); ++pieceLoc) {
        std::cout << *pieceLoc << " DONE\n";
        char pieceType = (*ownPieces)[*pieceLoc];
        vector<int> movesList;

        if(pieceType == 'P') {
            if(*pieceLoc / 10 == 1 && 
               ownPieces->count(*pieceLoc + 1) == 0 && 
               ownPieces->count(*pieceLoc + 2) == 0 && 
               oppPieces->count(*pieceLoc + 1) == 0 && 
               oppPieces->count(*pieceLoc + 2) == 0 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 2, col, 
               ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc + 2);
            
            if(ownPieces->count(*pieceLoc + 1) == 0 && 
               oppPieces->count(*pieceLoc + 1) == 0 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1, col, 
               ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc + 1);

            if(oppPieces->count(*pieceLoc + 11) == 1 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11, 
               col, ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc + 11);
            
            if(oppPieces->count(*pieceLoc - 9) == 1 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9, col, 
               ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc - 9);

            if(*pieceLoc / 10 == 4 &&
               board->enPassantPawns.size() == 1 && 
               board->enPassantPawns.at(0) == *pieceLoc + 10) {
                oppPieces->erase(*pieceLoc + 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc, 
                    *pieceLoc + 11, col, ownPieces, oppPieces, true);
                (*oppPieces)[*pieceLoc + 10] = 'p';
                if(!inCheck) movesList.emplace_back(*pieceLoc + 11);
            }
            
            if(*pieceLoc / 10 == 4 &&
               board->enPassantPawns.size() == 1 && 
               board->enPassantPawns.at(0) == *pieceLoc - 10) {
                oppPieces->erase(*pieceLoc - 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc, 
                    *pieceLoc - 9, col, ownPieces, oppPieces, true);
                (*oppPieces)[*pieceLoc - 10] = 'p';
                if(!inCheck) movesList.emplace_back(*pieceLoc - 9);
            }
        }

        else if(pieceType == 'p') {
            if(*pieceLoc / 10 == 6 && 
               ownPieces->count(*pieceLoc - 1) == 0 && 
               ownPieces->count(*pieceLoc - 2) == 0 && 
               oppPieces->count(*pieceLoc - 1) == 0 && 
               oppPieces->count(*pieceLoc - 2) == 0 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 2, col, 
               ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc - 2);
            
            if(ownPieces->count(*pieceLoc - 1) == 0 && 
               oppPieces->count(*pieceLoc - 1) == 0 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1, col, 
               ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc - 1);

            if(oppPieces->count(*pieceLoc - 11) == 1 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11, 
               col, ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc - 11);
            
            if(oppPieces->count(*pieceLoc + 9) == 1 && 
               !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9, col, 
               ownPieces, oppPieces, true))
                movesList.emplace_back(*pieceLoc + 9);

            if(*pieceLoc / 10 == 3 &&
               board->enPassantPawns.size() == 1 && 
               board->enPassantPawns.at(0) == *pieceLoc + 10) {
                oppPieces->erase(*pieceLoc + 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc, 
                    *pieceLoc + 9, col, ownPieces, oppPieces, true);
                (*oppPieces)[*pieceLoc + 10] = 'P';
                if(!inCheck) movesList.emplace_back(*pieceLoc + 9);
            }
            
            if(*pieceLoc / 10 == 3 &&
               board->enPassantPawns.size() == 1 && 
               board->enPassantPawns.at(0) == *pieceLoc - 10) {
                oppPieces->erase(*pieceLoc - 10);
                bool inCheck = board->movePutsKingInCheck(*pieceLoc, 
                    *pieceLoc - 11, col, ownPieces, oppPieces, true);
                (*oppPieces)[*pieceLoc - 10] = 'P';
                if(!inCheck) movesList.emplace_back(*pieceLoc - 11);
            }
        }

        else if(pieceType == 'r' || pieceType == 'R') {
           for(int i = *pieceLoc + 1; i % 10 != 8 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }
           for(int i = *pieceLoc + 10; i < 78 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }
           for(int i = *pieceLoc - 1; i % 10 != 9 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }
           for(int i = *pieceLoc - 10; i >= 0 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }   
        }

        else if(pieceType == 'b' || pieceType == 'B') {
            for(int i = *pieceLoc + 11; i < 78 && i % 10 != 8 && 
                    ownPieces->count(i) == 0; i += 11) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
            for(int i = *pieceLoc + 9; i < 78 && i % 10 != 9 && 
                    ownPieces->count(i) == 0; i += 9) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
            for(int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 && 
                    ownPieces->count(i) == 0; i -= 11) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
            for(int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 && 
                    ownPieces->count(i) == 0; i -= 9) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
        }

        else if(pieceType == 'q' || pieceType == 'Q') {
            for(int i = *pieceLoc + 1; i % 10 != 8 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }
           for(int i = *pieceLoc + 10; i < 78 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }
           for(int i = *pieceLoc - 1; i % 10 != 9 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }
           for(int i = *pieceLoc - 10; i >= 0 && 
                    ownPieces->count(i) == 0; ++i) {

                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
           }
           for(int i = *pieceLoc + 11; i < 78 && i % 10 != 8 && 
                    ownPieces->count(i) == 0; i += 11) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
            for(int i = *pieceLoc + 9; i < 78 && i % 10 != 9 && 
                    ownPieces->count(i) == 0; i += 9) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
            for(int i = *pieceLoc - 11; i >= 0 && i % 10 != 9 && 
                    ownPieces->count(i) == 0; i -= 11) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
            for(int i = *pieceLoc - 9; i >= 0 && i % 10 != 8 && 
                    ownPieces->count(i) == 0; i -= 9) {
        
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true) && oppPieces->count(i) == 1) break;
                if(board->movePutsKingInCheck(*pieceLoc, i, col, ownPieces, 
                    oppPieces, true)) continue;
                movesList.emplace_back(i);
                if(oppPieces->count(i) == 1) break;
            }
        }

        else if(pieceType == 'n' || pieceType == 'N') {
            if(*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 0 && 
                ownPieces->count(*pieceLoc - 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 21,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 21);

            if(*pieceLoc / 10 >= 2 && *pieceLoc % 10 != 7 && 
                ownPieces->count(*pieceLoc - 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 19,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 19);

            if(*pieceLoc / 10 != 0 && *pieceLoc % 10 >= 2 && 
                ownPieces->count(*pieceLoc - 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 12,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 12);

            if(*pieceLoc / 10 != 0 && *pieceLoc % 10 <= 5 && 
                ownPieces->count(*pieceLoc - 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 8,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 8);

            if(*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 7 && 
                ownPieces->count(*pieceLoc + 21) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 21,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 21);

            if(*pieceLoc / 10 <= 5 && *pieceLoc % 10 != 0 && 
                ownPieces->count(*pieceLoc + 19) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 19,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 19);

            if(*pieceLoc / 10 != 7 && *pieceLoc % 10 <= 5 && 
                ownPieces->count(*pieceLoc + 12) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 12,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 12);

            if(*pieceLoc / 10 != 7 && *pieceLoc % 10 >= 2 && 
                ownPieces->count(*pieceLoc + 8) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 8,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 8);
        }

        else if(pieceType == 'k' || pieceType == 'K') {
            if(*pieceLoc % 10 != 7 && ownPieces->count(*pieceLoc + 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 1,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 1);

            if(*pieceLoc % 10 != 0 && ownPieces->count(*pieceLoc - 1) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 1,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 1);

            if(*pieceLoc / 10 != 7 && ownPieces->count(*pieceLoc + 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 10,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 10);

            if(*pieceLoc / 10 != 0 && ownPieces->count(*pieceLoc - 10) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 10,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 10);

            if(*pieceLoc / 10 != 7 && *pieceLoc % 10 != 7 && 
                ownPieces->count(*pieceLoc + 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 11,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 11);

            if(*pieceLoc / 10 != 0 && *pieceLoc % 10 != 0 && 
                ownPieces->count(*pieceLoc - 11) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 11,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 11);

            if(*pieceLoc / 10 != 7 && *pieceLoc % 10 != 0 && 
                ownPieces->count(*pieceLoc + 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc + 9,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc + 9);

            if(*pieceLoc / 10 != 0 && *pieceLoc % 10 != 7 && 
                ownPieces->count(*pieceLoc - 9) == 0 &&
                !board->movePutsKingInCheck(*pieceLoc, *pieceLoc - 9,    
                col, ownPieces, oppPieces, true)) 
                movesList.emplace_back(*pieceLoc - 9);
        }

        if(movesList.size() == 0) continue;
        randNumGenerator(&movesList);
        if(pieceType == 'P' && *pieceLoc / 10 == 6) {
            vector<int> promotion {'Q', 'R', 'N', 'B'};
            randNumGenerator(&promotion);
            return vector<int> {*pieceLoc, movesList.at(0), promotion.at(0)};
        }
        if(pieceType == 'p' && *pieceLoc / 10 == 1) {
            vector<int> promotion {'q', 'r', 'n', 'b'};
            randNumGenerator(&promotion);
            return vector<int> {*pieceLoc, movesList.at(0), promotion.at(0)};
        }
        return vector<int> {*pieceLoc, movesList.at(0)};
    }
}