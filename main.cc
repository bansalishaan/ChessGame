#include <iostream>
#include <memory>
#include <string>
#include "Player.h"
#include "Board.h"
using namespace std;

int main() {
    Board game{0};
    vector<unique_ptr<Player>> players;
    string command;
    bool gameRunning = false;
    bool setupFlag = false;
    bool whiteMove = true;
    int whiteWins = 0;
    int blackWins = 0;

    cout << "Welcome to Chess! Please Enter a command: " << endl;

    while (true) {
        
        cin >> command;

        if (command == "game") {
            string whitePlayerType, blackPlayerType;
            cin >> whitePlayerType >> blackPlayerType;

            players.clear();

            if (whitePlayerType == "human") {
                players.emplace_back(make_unique<Human>());
            } else if (whitePlayerType == "computer1") {
                // TBD
            } else if (whitePlayerType == "computer2") {
                // TBD
            } else if (whitePlayerType == "computer3") {
                // TBD
            }else if (whitePlayerType == "computer4") {
                // TBD
            }

            if (blackPlayerType == "human") {
                players.emplace_back(make_unique<Human>());
            } else if (blackPlayerType == "computer1") {
                // TBD
            } else if (blackPlayerType == "computer2") {
                // TBD
            } else if (blackPlayerType == "computer3") {
                // TBD
            }else if (blackPlayerType == "computer4") {
                // TBD
            }

            gameRunning = true;
            if(!setupFlag) game.init(true);
            game.display();

        } else if (command == "resign") {
            if (gameRunning) {
                // Handle resignation
                gameRunning = false;
                if (whiteMove) {
                    blackWins++;
                } else {
                    whiteWins++;
                }
            }
        } else if (command == "move") {
            if (gameRunning) {
                vector<int> move;
                if (whiteMove) {
                    move = players.at(0)->getMove();
                } else {
                    move = players.at(1)->getMove();
                }

                // The column is determined by who is making the move
                int col = whiteMove ? 0 : 1;

                // Perform the move and capture the success of the move
                bool moveSuccess = game.makeMove(move, col);
                if (moveSuccess) {
                    whiteMove = !whiteMove;
                }
                game.display();
            }
        } else if (command == "setup") {
                if (!gameRunning) {
                    setupFlag = true;
                    string setupCommand;
                    while (cin >> setupCommand && setupCommand != "done") {
                        if (setupCommand == "+" || setupCommand == "-") {
                            string piece, spot;
                            bool validPiece = true;
                            if (setupCommand == "+") {
                                cin >> piece;
                                if (piece.find_first_not_of("RBNQKPrbnqkp") != string::npos) {
                                    cout << "Enter a valid piece" << endl;
                                    validPiece = false;
                                    continue;
                                }
                            }
                            if (validPiece) {
                                cin >> spot;
                                if (spot.length() != 2 || spot[0] < 'a' || spot[0] > 'h' || spot[1] < '1' || spot[1] > '8') {
                                    cout << "Enter a valid location." << endl;
                                    continue;
                                }
                                int location = (spot[0] - 'a') * 10 + (spot[1] - '1');
                                if (setupCommand == "+") {
                                    game.init(false, piece[0], location);
                                } else {
                                    game.init(false, '\0', location, true);
                                }
                                game.display();
                            }
                        }
                        else if (setupCommand == "=") {
                            string colour;
                            cin >> colour;
                            whiteMove = (colour == "white");
                        }
                        else {
                            cout << "Enter a valid setup command." << endl;
                        }
                    }
                }
        } else if (command == "quit" || cin.eof()) {
            cout << "Final Score:" << endl;
            cout << "White: " << whiteWins << endl;
            cout << "Black: " << blackWins << endl;
            break;
        }
        else {
            cout << "Invalid command." << endl;
        }
    }

    return 0;

}

/*
string start, end;
                cin >> start >> end;
                if(start.length() != 2 || end.length() != 2 || start[0] < 'a' || start[0] > 'h' || start[1] < '1' || start[1] > '8' || end[0] < 'a' || end[0] > 'h' || end[1] < '1' || end[1] > '8') {
                    cout << "Enter a valid location." << endl;
                    continue;
                }
                int first_start = start[0] - 'a';
                int second_start = start[1] - '1';
                int first_end = end[0] - 'a';
                int second_end = end[1] - '1';
                int start_location_int = first_start * 10 + second_start;
                int end_location_int = first_end * 10 + second_end;
                int col = whiteMove ? 0 : 1;
                vector<int> move = {start_location_int, end_location_int};
                bool moveSuccess = game.makeMove(move, col);
                if (moveSuccess) {
                    whiteMove = !whiteMove;
                }
                game.display();
*/




/*
int main() {
    vector<unique_ptr<Player>> players;
    players.emplace_back(make_unique<Human>());
    Board game{0};

    // auto n = players.at(0)->getMove();
    game.init(true);
    if(!game.validInit()) {
        game.init(false, '\0', 0, true);
    }
    /*vector<int> move;
    move.push_back(11); // Starting square
    move.push_back(12); // Ending square
    game.makeMove(move, 0);
    vector<int> move2;
    move2.push_back(20); // Starting square
    move2.push_back(02); // Ending square
    game.makeMove(move2, 0);
    game.display();
} */

// command line
// extra features
// bot
// graphics display

/*
else if (command == "setup") {
            if (!gameRunning) {
                string setupCommand;
                while (cin >> setupCommand && setupCommand != "done") {
                    if (setupCommand == "+") {
                        string piece, spot;
                        cin >> piece >> spot;
                        if (piece != "R" || piece != "B" || piece != "N" || piece != "Q" || piece != "K" || piece != "P" || piece != "r"
                            || piece != "b" || piece != "n" || piece != "q" || piece != "k" || piece != "p") {
                                cout << "Enter a valid piece" << endl;
                                continue;
                            }
                        if(spot.length() > 2 || spot[0] != "a" || spot[0] != "b" || spot[0] != "c" || spot[0] != "d" || spot[0] != "e" 
                            || spot[0] != "f" || spot[0] != "g" || spot[0] != "h" || stoi(spot[1]) < 1 || stoi(spot[1]) > 8) {
                                cout << "Enter a valid location." << endl;
                                continue;
                        }
                        int first = spot[0] - 'a';
                        int second = spot[1] - '1';
                        string location = to_string(first) + to_string(second);
                        game.init(false, piece, stoi(location));
                    }
                    else if (setupCommand == "-") {
                        string spot;
                        cin >> spot;
                        if(spot.length() > 2 || spot[0] != "a" || spot[0] != "b" || spot[0] != "c" || spot[0] != "d" || spot[0] != "e" 
                            || spot[0] != "f" || spot[0] != "g" || spot[0] != "h" || stoi(spot[1]) < 1 || stoi(spot[1]) > 8) {
                                cout << "Enter a valid location." << endl;
                                continue;
                        }
                        int first = spot[0] - 'a';
                        int second = spot[1] - '1';
                        string location = to_string(first) + to_string(second);
                        game.init(false, piece, stoi(location), true);
                    }
                    else if (setupCommand == "=") {
                        string colour;
                        cin >> colour;
                        if (colour == "white"){
                            whiteMove = true;
                        } else if (colour == "black") {
                            whiteMove = false;
                        }
                    } else if (setupCommand == "done"){
                        break;
                    } else {
                        cout << "Enter a valid setup command."
                    }
                }
            }
        } */