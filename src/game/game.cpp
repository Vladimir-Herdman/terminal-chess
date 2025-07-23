#include "game.hpp"

// Publics
// Check options and configure game based off such configurations
Game::Game() {
}

void Game::begin() {
    m_print_board();
}

// Privates
void Game::m_print_board() {
    ui.print_board();
}
