#include <iostream>
#include <memory>
#include "Player.h"

int main() {
    vector<unique_ptr<Player>> players;
    players.emplace_back(make_unique<Human>());

    auto n = players.at(0)->getMove();
    
    cout << n[0][0] << n[0][1] << n[1][0] << n[1][1] << endl;
}