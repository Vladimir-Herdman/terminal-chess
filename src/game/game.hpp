#ifndef TERMINALCHESS_GAME_GAME_H
#define TERMINALCHESS_GAME_GAME_H

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include "game/logic/bit_boards.hpp"
#include "game/ui/ui.hpp"

class Game {
public:
    Game();
    ~Game();
    void begin();
    void end();

private:
    void m_printBoard();
    void m_refreshScreen();

    int m_input();
    int m_inputTyped();
    int m_inputInteractive() const;

    void m_configDaemonFunction();
    bool m_hasConfigFileChanged() const;
    std::thread m_config_daemon;
    std::mutex m_config_daemon_mutex;
    std::condition_variable m_config_daemon_end;
    std::atomic<bool> m_config_daemon_running; //loaded later from defaults
    std::atomic<bool> m_refreshing_screen = false;
    std::atomic<int> m_config_daemon_sleep_time;

    UI ui;

    bool m_game_running = true;
    const BITBOARDS::Side& m_white = BITBOARDS::white;
    const BITBOARDS::Side& m_black = BITBOARDS::black;

    const std::map<std::string, int> m_map_input = {
        {"refresh", static_cast<int>(UI::InputStatus::FULL_REFRESH)},
        {"r", static_cast<int>(UI::InputStatus::FULL_REFRESH)},
        {"exit", static_cast<int>(UI::InputStatus::END_GAME)},
        {"e", static_cast<int>(UI::InputStatus::END_GAME)},
    };
};

#endif //TERMINALCHESS_GAME_GAME_H
