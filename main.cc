#include <iostream>
#include <memory>
#include <string>
#include "Player.h"
#include "Board.h"
using namespace std;

int main() {
    int whiteWins = 0;
    int blackWins = 0;
    bool gameRunning = false;
    bool setupFlag = false;
    bool whiteMove = true;

    cout << "Welcome to Chess! Please enter a command: " << endl;

    while (true) {
        Board game{0};
        vector<unique_ptr<Player>> players;
        gameRunning = false;
        setupFlag = false;
        whiteMove = true;
        string command;
        while (true) {
            if (!(cin >> command)) {
                if (cin.eof()) {
                    cout << "Final Score:" << endl;
                    cout << "White: " << whiteWins << endl;
                    cout << "Black: " << blackWins << endl;
                    return 0;
                } else {
                    cin.clear();
                    cin.ignore();
                    cout << "Invalid command." << endl;
                    continue;
                }
            }
            if (command == "game") {
                if (gameRunning) {
                    gameRunning = false;
                    setupFlag = false;
                    break;
                }
                string whitePlayerType, blackPlayerType;
                cin >> whitePlayerType >> blackPlayerType;
                players.clear();
                if (whitePlayerType == "human") {
                    players.emplace_back(make_unique<Human>());
                }
                if (blackPlayerType == "human") {
                    players.emplace_back(make_unique<Human>());
                } 
                gameRunning = true;
                if (!setupFlag) {
                    game.init(true);
                }
                game.display();
            } else if (command == "resign") {
                if (gameRunning) {
                    gameRunning = false;
                    if (whiteMove) {
                        blackWins++;
                    } else {
                        whiteWins++;
                    }
                    break;
                }
            } else if (command == "move") {
                if (gameRunning) {
                    vector<int> move;
                    if (whiteMove) {
                        move = players.at(0)->getMove();
                    } else {
                        move = players.at(1)->getMove();
                    }
                    int col = whiteMove ? 0 : 1;
                    bool success = game.makeMove(move, col);
                    if (success) {
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
            } else if (command == "quit") {
                cout << "Final Score:" << endl;
                cout << "White: " << whiteWins << endl;
                cout << "Black: " << blackWins << endl;
                return 0;
            } else {
                cout << "Invalid command." << endl;
            }
        }
    }
    return 0;
}
