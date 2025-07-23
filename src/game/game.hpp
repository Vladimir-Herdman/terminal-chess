#ifndef TERMINALCHESS_GAME_GAME_H
#define TERMINALCHESS_GAME_GAME_H

#include "game/logic/bit_boards.hpp"
#include "game/ui/ui.hpp"

class Game {
public:
    Game();
    void begin();

private:
    void m_print_board();
    UI ui;
    const BITBOARDS::Side& white = BITBOARDS::white;
    const BITBOARDS::Side& black = BITBOARDS::black;
};

#endif //TERMINALCHESS_GAME_GAME_H
