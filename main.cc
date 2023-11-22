#include <iostream>
#include "Player.h"

int main() {
    Human white;

    auto n = white.getMove();
    
    cout << n[0][0] << n[0][1] << n[1][0] << n[1][1] << endl;
}