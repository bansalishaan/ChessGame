#ifndef PLAYER
#define PLAYER

#include <vector>
#include "Board.h"
using namespace std;

class Player {
    public:
        virtual vector<int> getMove(string start, string end) = 0;
        virtual ~Player() = default;
};

class Human: public Player {
    public:
        Human();
        vector<int> getMove(string start, string end) override;
};

class Bot: public Player {
    protected:
        Board *board;
        int col;
        template<typename T>
            void shuffler(T *list);
    public:
        Bot(Board *b, int col);
        virtual ~Bot() = default;
};

class Bot1: public Bot {
    public:
        Bot1(Board *b, int col);
        vector<int> getMove(string start, string end) override;
};

class Bot2: public Bot1 {
    public:
        Bot2(Board *b, int col);
        vector<int> getMove(string start, string end) override;
};

class Bot3: public Bot1 {
    protected:
        bool avoidsCapture(int loc, int move);
    public:
        Bot3(Board *b, int col);
        vector<int> getMove(string start, string end) override;
};

class Bot4: public Bot3 {
    public:
        Bot4(Board *b, int col);
        vector<int> getMove(string start, string end) override;
};

#endif
