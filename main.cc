#include <iostream>
#include <memory>
#include <string>
#include "Player.h"
#include "Board.h"
using namespace std;

int main() {
    int whiteWins = 0;
    int blackWins = 0;
    int whiteDraws = 0;
    int blackDraws = 0;
    int whiteScore = 0;
    int blackScore = 0;
    string whiteScoreString = "";
    string blackScoreString = "";
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
                    if ((blackDraws % 2) == 0) {
                        blackScore = blackWins + (blackDraws / 2);
                        blackScoreString = to_string(blackScore);
                    } else if ((blackDraws % 2) == 1) {
                        blackScore = blackWins + (blackDraws / 2);
                        blackScoreString = to_string(blackScore) + ".5";
                    } 
                    
                    if ((whiteDraws % 2) == 0) {
                        whiteScore = whiteWins + (whiteDraws / 2);
                        whiteScoreString = to_string(whiteScore);
                    } else if ((whiteDraws % 2) == 1) {
                        whiteScore = whiteWins + (whiteDraws / 2);
                        whiteScoreString = to_string(whiteScore) + ".5";
                    }
                    cout << "Final Score:" << endl;
                    cout << "White: " << whiteScoreString << endl;
                    cout << "Black: " << blackScoreString << endl;
                    return 0;
                } else {
                    cin.clear();
                    cin.ignore();
                    cout << "Invalid command." << endl;
                    continue;
                }
            }
            if (game.inCheck(0) && game.noMoves(0) && setupFlag){
                cout << "Here 1" << endl;
                gameRunning = false;
                blackWins++;
                break;
            } else if (game.inCheck(1) && game.noMoves(1) && setupFlag) {
                cout << "Here 2" << endl;
                gameRunning = false;
                whiteWins++;
                break;
            } else if ((game.noMoves(0) || game.noMoves(1)) && setupFlag) {

                gameRunning = false;
                whiteDraws++;
                blackDraws++;
                break;
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
                    setupFlag = true;
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
                if ((blackDraws % 2) == 0) {
                        blackScore = blackWins + (blackDraws / 2);
                        blackScoreString = to_string(blackScore);
                    } else if ((blackDraws % 2) == 1) {
                        blackScore = blackWins + (blackDraws / 2);
                        blackScoreString = to_string(blackScore) + ".5";
                    } 
                    if ((whiteDraws % 2) == 0) {
                        whiteScore = whiteWins + (whiteDraws / 2);
                        whiteScoreString = to_string(whiteScore);
                    } else if ((whiteDraws % 2) == 1) {
                        whiteScore = whiteWins + (whiteDraws / 2);
                        whiteScoreString = to_string(whiteScore) + ".5";
                    }
                    cout << "Final Score:" << endl;
                    cout << "White: " << whiteScoreString << endl;
                    cout << "Black: " << blackScoreString << endl;
                    return 0;
            } else {
                cout << "Invalid command." << endl;
            }
        }
    }
    return 0;
}
