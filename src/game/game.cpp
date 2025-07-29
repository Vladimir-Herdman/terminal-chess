#include "game.hpp"

#include <chrono>
#include <filesystem>
#include <thread>

#include "config/ConfigData.hpp"
#include "config/ConfigReader.hpp"

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
        //std::cout << test << std::flush;
        std::string test;
        std::cin >> test;
        m_refresh_screen();
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
void Game::m_config_daemeon_function() {
    m_config_daemon_running.store(CONFIG::run_daemon);
    m_config_daemon_sleep_time.store(CONFIG::daemon_sleep_milliseconds);

    while (m_config_daemon_running.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_config_daemon_sleep_time.load()));
        if (m_has_config_file_changed()) {
            m_config_daemon_sleep_time.store(CONFIG::daemon_sleep_milliseconds);

            ConfigReader();
            m_refresh_screen();
        }
    }
}
bool Game::m_has_config_file_changed() const {
    if (! std::filesystem::exists(CONFIG::config_path)) {return false;}

    const auto file_now_last_write = std::filesystem::last_write_time(CONFIG::config_path);
    if (file_now_last_write > CONFIG::last_write) {
        CONFIG::last_write = file_now_last_write;
        return true;
    }
    return false;
}
