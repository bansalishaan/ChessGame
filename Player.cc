#include "Player.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
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
vector<vector<int>> Human::getMove() {
    vector<vector<int>> move;
    move.emplace_back();
    move.emplace_back();

    string commandLine;
    getline(cin, commandLine);
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
    move.at(0).emplace_back(piece[0] - 97);
    move.at(0).emplace_back(charToInt(piece[1]));
    move.at(1).emplace_back(moveLoc[0] - 97);
    move.at(1).emplace_back(charToInt(moveLoc[1]));
    
    if(!(promotion == "")) {    // ADD PROMOTION TO MOVE
        move.emplace_back();
    }

    return move;
}

Player::~Player() {}
