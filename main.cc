#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "Board.h"
using namespace std;

int main()
{
    int whiteWins = 0;
    int blackWins = 0;
    int whiteDraws = 0;
    int blackDraws = 0;
    int whiteScore = 0;
    int blackScore = 0;
    int gameCount = 0;
    int whiteMoveCount = 0;
    string whiteScoreString = "";
    string blackScoreString = "";
    bool gameRunning = false;
    bool setupFlag = false;
    bool whiteMove = true;

    cout << "Welcome to Chess! Please enter a command: " << endl;

    while (true)
    {
        Board game{0};
        vector<unique_ptr<Player>> players;
        gameRunning = false;
        setupFlag = false;
        whiteMove = true;
        string commandLine;
        string filepath = "game_log.txt";
        ofstream out(filepath, std::ios::app);
        while (true)
        {
            getline(cin, commandLine);

            if (cin.eof())
            {
                if ((blackDraws % 2) == 0)
                {
                    blackScore = blackWins + (blackDraws / 2);
                    blackScoreString = to_string(blackScore);
                }
                else if ((blackDraws % 2) == 1)
                {
                    blackScore = blackWins + (blackDraws / 2);
                    blackScoreString = to_string(blackScore) + ".5";
                }

                if ((whiteDraws % 2) == 0)
                {
                    whiteScore = whiteWins + (whiteDraws / 2);
                    whiteScoreString = to_string(whiteScore);
                }
                else if ((whiteDraws % 2) == 1)
                {
                    whiteScore = whiteWins + (whiteDraws / 2);
                    whiteScoreString = to_string(whiteScore) + ".5";
                }
                cout << "Final Score:" << endl;
                out << "Final Score:" << endl;
                cout << "White: " << whiteScoreString << endl;
                out << "White: " << whiteScoreString << endl;
                cout << "Black: " << blackScoreString << endl;
                out << "Black: " << blackScoreString << endl;
                return 0;
            }

            if (game.inCheck(0) && game.noMoves(0) && setupFlag)
            {
                cout << "Checkmate for White. Black Wins!" << endl;
                out << "Checkmate for White. Black Wins!" << endl;
                gameRunning = false;
                blackWins++;
                break;
            }
            else if (game.inCheck(1) && game.noMoves(1) && setupFlag)
            {
                cout << "Checkmate for Black. White Wins!" << endl;
                out << "Checkmate for Black. White Wins!" << endl;
                gameRunning = false;
                whiteWins++;
                break;
            }
            else if ((game.noMoves(0) || game.noMoves(1)) && setupFlag)
            {
                cout << "Draw!" << endl;
                out << "Draw!" << endl;
                gameRunning = false;
                whiteDraws++;
                blackDraws++;
                break;
            }

            if (game.inCheck(0))
            {
                cout << "White is in Check!" << endl;
                out << "White is in Check!" << endl;
            }
            else if (game.inCheck(1))
            {
                cout << "Black is in Check!" << endl;
                out << "Black is in Check!" << endl;
            }

            string command;
            istringstream iss(commandLine);
            iss >> command;

            if (command == "game")
            {
                if (gameRunning)
                {
                    gameRunning = false;
                    setupFlag = false;
                    break;
                }
                gameCount++;
                whiteMoveCount = 0;
                out << "Game " << gameCount << ":" << endl;
                string whitePlayerType, blackPlayerType;
                iss >> whitePlayerType >> blackPlayerType;
                players.clear();
                if (whitePlayerType == "human")
                {
                    players.emplace_back(make_unique<Human>());
                }
                else if (whitePlayerType == "computer1")
                {
                    players.emplace_back(make_unique<Bot1>(&game, 0));
                }
                else if (whitePlayerType == "computer2")
                {
                    players.emplace_back(make_unique<Bot2>(&game, 0));
                }
                else if (whitePlayerType == "computer3")
                {
                    players.emplace_back(make_unique<Bot3>(&game, 0));
                }
                else if (whitePlayerType == "computer4")
                {
                    players.emplace_back(make_unique<Bot4>(&game, 0));
                }
                if (blackPlayerType == "human")
                {
                    players.emplace_back(make_unique<Human>());
                }
                else if (blackPlayerType == "computer1")
                {
                    players.emplace_back(make_unique<Bot1>(&game, 1));
                }
                else if (blackPlayerType == "computer2")
                {
                    players.emplace_back(make_unique<Bot2>(&game, 1));
                }
                else if (blackPlayerType == "computer3")
                {
                    players.emplace_back(make_unique<Bot3>(&game, 1));
                }
                else if (blackPlayerType == "computer4")
                {
                    players.emplace_back(make_unique<Bot4>(&game, 1));
                }
                gameRunning = true;
                if (!setupFlag)
                {
                    game.init(true);
                    setupFlag = true;
                }
                game.display();
            }
            else if (command == "resign")
            {
                if (gameRunning)
                {
                    gameRunning = false;
                    if (whiteMove)
                    {
                        blackWins++;
                        out << "White Resigns." << endl;
                    }
                    else
                    {
                        whiteWins++;
                        out << "Black Resigns." << endl;
                    }
                    break;
                }
            }
            else if (command == "move")
            {
                if (gameRunning)
                {
                    vector<int> move;
                    string start, end;
                    if (iss >> start >> end)
                    {
                        string promotion;
                        if (iss >> promotion)
                        {
                            // Handle pawn promotion
                            if (whiteMove)
                            {
                                move = players.at(0)->getMove(start, end);
                                move.emplace_back(promotion[0]);
                                out << "White moves from " << move_start << " to " << move_end << << " and promotes to " << promotion << "." << endl;
                            }
                            else
                            {
                                move = players.at(1)->getMove(start, end);
                                move.emplace_back(promotion[0]);
                                out << "Black moves from " << move_start << " to " << move_end << << " and promotes to " << promotion << "." << endl;
                            }
                        } 
                        else
                        {
                        if (whiteMove)
                        {
                            move = players.at(0)->getMove(start, end);
                            out << "White moves from " << start << " to " << end << "." << endl;
                            whiteMoveCount++;
                        }
                        else
                        {
                            move = players.at(1)->getMove(start, end);
                            out << "Black moves from " << start << " to " << end << "." << endl;
                        }
                        if (whiteMoveCount > 2)
                        {
                            cout << "You have made more than 2 moves and have failed the puzzle." << endl;
                            out << "PUZZLE FAILED." << endl;
                            gameRunning = false;
                            break;
                        }
                        }
                    }
                    else
                    {
                        if (whiteMove)
                        {
                            move = players.at(0)->getMove("", "");
                            int second = (move[0] % 10) + 1;
                            char first = (move[0] / 10) + 'a';
                            string move_start = first + to_string(second);
                            int second1 = (move[1] % 10) + 1;
                            char first1 = (move[1] / 10) + 'a';
                            string move_end = first1 + to_string(second1);
                            out << "White moves from " << move_start << " to " << move_end << "." << endl;
                        }
                        else
                        {
                            move = players.at(1)->getMove("", "");
                            int second = (move[0] % 10) + 1;
                            char first = (move[0] / 10) + 'a';
                            string move_start = first + to_string(second);
                            int second1 = (move[1] % 10) + 1;
                            char first1 = (move[1] / 10) + 'a';
                            string move_end = first1 + to_string(second1);
                            out << "Black moves from " << move_start << " to " << move_end << "." << endl;
                        }
                    }
                    int col = whiteMove ? 0 : 1;
                    bool success = game.makeMove(move, col);
                    if (success)
                    {
                        whiteMove = !whiteMove;
                    }
                    game.display();
                }
            }
            else if (command == "setup")
            {
                if (!gameRunning)
                {
                    setupFlag = true;
                    string setupCommand;
                    while (cin >> setupCommand && setupCommand != "done")
                    {
                        if (setupCommand == "+" || setupCommand == "-")
                        {
                            string piece, spot;
                            bool validPiece = true;
                            if (setupCommand == "+")
                            {
                                cin >> piece;
                                if (piece.find_first_not_of("RBNQKPrbnqkp") != string::npos)
                                {
                                    cout << "Enter a valid piece" << endl;
                                    validPiece = false;
                                    continue;
                                }
                            }
                            if (validPiece)
                            {
                                cin >> spot;
                                if (spot.length() != 2 || spot[0] < 'a' || spot[0] > 'h' || spot[1] < '1' || spot[1] > '8')
                                {
                                    cout << "Enter a valid location." << endl;
                                    continue;
                                }
                                int location = (spot[0] - 'a') * 10 + (spot[1] - '1');
                                if (setupCommand == "+")
                                {
                                    game.init(false, piece[0], location);
                                }
                                else
                                {
                                    game.init(false, '\0', location, true);
                                }
                                game.display();
                            }
                        }
                        else if (setupCommand == "=")
                        {
                            string colour;
                            cin >> colour;
                            whiteMove = (colour == "white");
                        }
                        else
                        {
                            cout << "Enter a valid setup command." << endl;
                        }
                    }
                }
            }
            else if (command == "quit")
            {
                if ((blackDraws % 2) == 0)
                {
                    blackScore = blackWins + (blackDraws / 2);
                    blackScoreString = to_string(blackScore);
                }
                else if ((blackDraws % 2) == 1)
                {
                    blackScore = blackWins + (blackDraws / 2);
                    blackScoreString = to_string(blackScore) + ".5";
                }
                if ((whiteDraws % 2) == 0)
                {
                    whiteScore = whiteWins + (whiteDraws / 2);
                    whiteScoreString = to_string(whiteScore);
                }
                else if ((whiteDraws % 2) == 1)
                {
                    whiteScore = whiteWins + (whiteDraws / 2);
                    whiteScoreString = to_string(whiteScore) + ".5";
                }
                cout << "Final Score:" << endl;
                out << "Final Score:" << endl;
                cout << "White: " << whiteScoreString << endl;
                out << "White: " << whiteScoreString << endl;
                cout << "Black: " << blackScoreString << endl;
                out << "Black: " << blackScoreString << endl;
                return 0;
            }
            else if (command == "puzzle1")
            {
                if (gameRunning)
                {
                    gameRunning = false;
                    setupFlag = false;
                    break;
                }
                setupFlag = true;
                cout << "This is a simple chess puzzle, which can have White Mate in 2 moves." << endl;
                out << "Puzzle 1:" << endl;
                game.init(false, 'b', 21);
                game.init(false, 'r', 31);
                game.init(false, 'R', 51);
                game.init(false, 'B', 02);
                game.init(false, 'P', 62);
                game.init(false, 'K', 72);
                game.init(false, 'r', 33);
                game.init(false, 'N', 63);
                game.init(false, 'P', 73);
                game.init(false, 'k', 74);
                game.init(false, 'p', 65);
                players.emplace_back(make_unique<Human>());
                players.emplace_back(make_unique<Bot3>(&game, 1));
                game.display();
                whiteMove = true;
                gameRunning = true;
                whiteMoveCount = 0;
                cout << "You are white, make your move: " << endl;
            }
            else if (command == "puzzle2")
            {
                if (gameRunning)
                {
                    gameRunning = false;
                    setupFlag = false;
                    break;
                }
                setupFlag = true;
                cout << "This is a simple chess puzzle, which can have White Mate in 2 moves." << endl;
                out << "Puzzle 2:" << endl;
                game.init(false, 'R', 40);
                game.init(false, 'K', 60);
                game.init(false, 'P', 51);
                game.init(false, 'P', 61);
                game.init(false, 'Q', 62);
                game.init(false, 'P', 72);
                game.init(false, 'P', 55);
                game.init(false, 'k', 77);
                game.init(false, 'q', 67);
                game.init(false, 'p', 75);
                players.emplace_back(make_unique<Human>());
                players.emplace_back(make_unique<Bot3>(&game, 1));
                game.display();
                whiteMove = true;
                gameRunning = true;
                whiteMoveCount = 0;
                cout << "You are white, make your move: " << endl;
            }
            else
            {
                if (command != "")
                    cout << "Invalid command." << endl;
            }
        }
    }
    return 0;
}
