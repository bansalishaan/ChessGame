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
