#include "game.hpp"

#include <iostream>

// Publics
// Check options and configure game based off such configurations
Game::Game() {
}

void Game::begin() {
    print_board();
}

// Privates
void Game::print_board() {
    for (int row = 0; row < 10; row++) {
        for (int column = 0; column < 10; column++) {
            std::cout << ui.pieces_lookup[static_cast<int>(ui.board[row][column])];
        }
    }
    std::cout << '\n';
}
