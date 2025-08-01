#include "game.hpp"

#include <chrono>
#include <filesystem>
#include <thread>

#include "config/ConfigData.hpp"
#include "config/ConfigReader.hpp"

#define SCI(arg) static_cast<int>(arg)

namespace {
    using namespace CONFIG;
}

// Publics
// Check options and configure game based off such configurations
Game::Game(): m_config_daemon(&Game::m_config_daemeon_function, this) {}

Game::~Game() {
    if (m_config_daemon.joinable()) {
        m_config_daemon.join();
    }
}

void Game::begin() {
    m_print_board();
    while (m_game_running) {
        if(m_input()) {
            m_refresh_screen();
        }
        //std::string input;
        //std::cin >> test;
        //if (test == "test") {std::cout<<"\nYou got it right!\033[01A\r";}
    }
}

// Privates
void Game::m_print_board() {
    m_refreshing_screen.store(true);
    ui.print_board();
    m_refreshing_screen.store(false);
}
void Game::m_refresh_screen() {
    m_refreshing_screen.store(true);
    ui.refresh_board();
    m_refreshing_screen.store(false);
}

int Game::m_input() {
    if (OPTIONS.input_interactive) {return m_input_interactive();}
    return m_input_typed();
}
int Game::m_input_typed() const {
    //if (typed == highlight choice) {return DONT_REFRESH}
    return SCI(m_input_status::REFRESH);
}
int Game::m_input_interactive() const {
    return SCI(m_input_status::DONT_REFRESH);
}

void Game::m_config_daemeon_function() {
    m_config_daemon_running.store(DAEMON.run_daemon);
    m_config_daemon_sleep_time.store(DAEMON.daemon_sleep_milliseconds);

    while (m_config_daemon_running.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_config_daemon_sleep_time.load()));
        if (m_has_config_file_changed()) {
            ConfigReader();
            m_config_daemon_sleep_time.store(DAEMON.daemon_sleep_milliseconds);
            m_refresh_screen();
        }
    }
}
bool Game::m_has_config_file_changed() const {
    if (! std::filesystem::exists(DAEMON.config_path)) {return false;}

    const auto file_now_last_write = std::filesystem::last_write_time(DAEMON.config_path);
    if (file_now_last_write > DAEMON.last_write) {
        DAEMON.last_write = file_now_last_write;
        return true;
    }
    return false;
}

#undef SCI
