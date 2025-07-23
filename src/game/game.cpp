#include "game.hpp"

#include <iostream>

// Publics
// Check options and configure game based off such configurations
Game::Game() {
}

void Game::begin() {
    m_print_board();
}

// Privates
void Game::m_print_board() {
    std::string line = "";
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            line += ui.get_square(r, c);
        }

        std::cout << line + CONFIG::COLORS.reset << '\n';
        line = "";
    }
}
