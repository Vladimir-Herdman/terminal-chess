#include "game.hpp"

#include <cctype>
#include <chrono>
#include <filesystem>
#include <mutex>
#include <thread>

#include "config/ConfigData.hpp"
#include "config/ConfigReader.hpp"
#include "game/ui/ui.hpp"

#define SCI(arg) static_cast<int>(arg)

namespace {
    using namespace CONFIG;
}

// Publics
// Check options and configure game based off such configurations
Game::Game(): m_config_daemon(&Game::m_configDaemonFunction, this) {}

Game::~Game() {
    if (m_config_daemon.joinable()) {
        m_config_daemon.join();
    }
}

void Game::begin() {
    m_printBoard();
    while (m_game_running) {
        using uis = UI::InputStatus;

        switch (m_input()) {
            case SCI(uis::FULL_REFRESH):
                m_refreshScreen(); break;

            case SCI(uis::DONT_REFRESH):
                break;

            case SCI(uis::IMPROPER_INPUT):
                ui.inputImproper(); break;

            case SCI(uis::END_GAME):
                m_game_running = false; break;
        }
    }
}
void Game::end() {
    // stop daemon
    m_config_daemon_running.store(false);
    m_config_daemon_end.notify_all();

    //TODO
    // print out results
}

// Privates
void Game::m_printBoard() {
    m_refreshing_screen.store(true);
    ui.printBoard();
    m_refreshing_screen.store(false);
}
void Game::m_refreshScreen() {
    m_refreshing_screen.store(true);
    ui.refreshBoard();
    m_refreshing_screen.store(false);
}

int Game::m_input() {
    if (OPTIONS.input_interactive) {return m_inputInteractive();}
    return m_inputTyped();
}
int Game::m_inputTyped() {
    std::string input;
    std::getline(std::cin, input);

    for (char& ch : input) {
        if (std::isalpha(ch)) {continue;}
        ch = std::tolower(ch);
    }

    if (input.length() == 3 && input[0] == 'h') { //highlight square
        return ui.highlight(input);
    }

    // commands seperate from chess instructions
    if (m_map_input.find(input) != m_map_input.end()) {
        return m_map_input.at(input);
    }

    return SCI(UI::InputStatus::IMPROPER_INPUT);
}
int Game::m_inputInteractive() const {
    return SCI(UI::InputStatus::DONT_REFRESH);
}

void Game::m_configDaemonFunction() {
    std::unique_lock<std::mutex> lock(m_config_daemon_mutex);
    m_config_daemon_running.store(DAEMON.run_daemon);
    m_config_daemon_sleep_time.store(DAEMON.daemon_sleep_milliseconds);
    bool game_ended_while_sleeping;

    while (m_config_daemon_running.load()) {
        //check for early finish with game end
        //little gross, but activated in game loop at end condition so thread doesn't keep game open after finish
        game_ended_while_sleeping = m_config_daemon_end.wait_for(
            lock,
            std::chrono::milliseconds(m_config_daemon_sleep_time.load()),
            [this]{return (m_config_daemon_running.load());}
        );
        if (game_ended_while_sleeping) {break;}

        if (m_hasConfigFileChanged()) {
            ConfigReader();
            m_config_daemon_sleep_time.store(DAEMON.daemon_sleep_milliseconds);
            m_refreshScreen();
        }
    }
}
bool Game::m_hasConfigFileChanged() const {
    if (! std::filesystem::exists(DAEMON.config_path)) {return false;}

    const auto file_now_last_write = std::filesystem::last_write_time(DAEMON.config_path);
    if (file_now_last_write > DAEMON.last_write) {
        DAEMON.last_write = file_now_last_write;
        return true;
    }
    return false;
}

#undef SCI
