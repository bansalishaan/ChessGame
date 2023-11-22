#include <vector>
#include "Board.h"
using namespace std;

class Player {
    protected:
        virtual vector<vector<int>> getMove() = 0;
    public:
        virtual ~Player();
};

class Human: public Player {
    public:
        Human();
        vector<vector<int>> getMove() override;
};

// class Bot: public