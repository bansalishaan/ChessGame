#include <vector>
using namespace std;

class Player {
    public:
        virtual vector<int> getMove() = 0;
        virtual ~Player() = default;
};

class Human: public Player {
    public:
        Human();
        vector<int> getMove() override;
};

// class Bot: public