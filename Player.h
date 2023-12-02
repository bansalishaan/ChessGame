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
    public:
        Bot(Board *b, int col);
        virtual ~Bot() = default;
};

class Bot1: public Bot {
    template<typename T>
        void randNumGenerator(T *list);
    public:
        Bot1(Board *b, int col);
        vector<int> getMove() override;
};

// class Bot2: public Bot {
//     public:
//         Bot2();
//         vector<int> getMove() override;
// };

// class Bot3: public Bot {
//     public:
//         Bot3();
//         vector<int> getMove() override;
// };

// class Bot4: public Bot {
//     public:
//         Bot4();
//         vector<int> getMove() override;
// };

#endif
