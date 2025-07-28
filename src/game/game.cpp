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
    while (m_game_running) {
        m_print_board();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

// Privates
void Game::m_print_board() const {
    ui.print_board();
}
void Game::m_config_daemeon_function() {
    //m_config_daemon_running.store(CONFIG::run_daemon);
    m_config_daemon_running.store(true);
    //m_config_daemon_sleep_time.store(CONFIG::daemon_sleep_milliseconds);
    m_config_daemon_sleep_time.store(500);
    while (m_config_daemon_running.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_config_daemon_sleep_time.load()));
        if (m_has_config_file_changed()) {
            ConfigReader();
        }
    }
}
bool Game::m_has_config_file_changed() const {
    const auto file_now_last_write = std::filesystem::last_write_time(CONFIG::config_path);
    if (file_now_last_write > CONFIG::last_write) {
        CONFIG::last_write = file_now_last_write;
        return true;
    }
    return false;
}
