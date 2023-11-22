#include <iostream>
#include <memory>
#include "Player.h"
#include "Board.h"

int main() {
    vector<unique_ptr<Player>> players;
    players.emplace_back(make_unique<Human>());
    Board game{0};

    // auto n = players.at(0)->getMove();
    game.init(false, 'p', 14);
    game.init(false, 'Q', 34);
    game.init(false, 'k', 26);
    game.init(false, 'B', 57);
    game.init(false, 'N', 20);
    game.init(false, 'j', 14);
    game.display();
}