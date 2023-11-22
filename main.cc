#include <iostream>
#include <memory>
#include "Player.h"
#include "Board.h"

int main() {
    vector<unique_ptr<Player>> players;
    players.emplace_back(make_unique<Human>());
    Board game{0};

    // auto n = players.at(0)->getMove();
    game.init(false, 'P', 17);
    game.init(false, 'Q', 34);
    game.init(false, 'k', 26);
    game.init(false, 'B', 57);
    game.init(false, 'n', 20);
    game.init(false, 'p', 0);
    if(!game.validInit()) {
        game.init(false, '\0', 0, true);
    }
    game.display();
}