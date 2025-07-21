#include <string>

//#include "config/ConfigData.hpp"
#include "game.hpp"

namespace {
    //auto& PIECES = CONFIG::PIECES;
    //auto& COLORS = CONFIG::COLORS;
}

// Publics
Game::Game() {
    
}

void Game::beginMatch() {
    std::string board_presentation = this->m_generateBoard();
}

// Private helpers
std::string Game::m_generateBoard() {
    std::string board_to_print;
    
    return board_to_print;
}
