#ifndef TERMINALCHESS_GAME_H
#define TERMINALCHESS_GAME_H

#include <string>

class Game {
private:
    std::string m_generateBoard();

public:
    // Initialize all the settings and setups needed to play
    Game();
    // Actually start the match and begin printing everything
    void beginMatch();
};

#endif //TERMINALCHESS_GAME_H
