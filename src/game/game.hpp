#ifndef TERMINALCHESS_GAME_GAME_H
#define TERMINALCHESS_GAME_GAME_H

#include "game/ui/ui.hpp"

#include <atomic>
#include <thread>

#include "game/logic/bit_boards.hpp"

class Game {
public:
    Game();
    ~Game();
    void begin();

private:
    void m_printBoard();
    void m_refreshScreen();

    int m_input();
    int m_inputTyped() const;
    int m_inputInteractive() const;
    enum class m_InputStatus {
        DONT_REFRESH = 0, FULL_REFRESH = 1
    };

    UI ui;


    void m_configDaemonFunction();
    bool m_hasConfigFileChanged() const;
    std::thread m_config_daemon;
    std::atomic<bool> m_config_daemon_running; //loaded later from defaults
    std::atomic<bool> m_refreshing_screen = false;
    std::atomic<int> m_config_daemon_sleep_time;

    bool m_game_running = true;
    const BITBOARDS::Side& white = BITBOARDS::white;
    const BITBOARDS::Side& black = BITBOARDS::black;
};

#endif //TERMINALCHESS_GAME_GAME_H
