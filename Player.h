#include <vector>
#include "Board.h"
using namespace std;

class Player {

    public:
        vector<vector<int>> getMove();
        virtual ~Player() = 0;
};

class Human: public Player {
    public:
        Human();
        vector<vector<int>> getMove();
};