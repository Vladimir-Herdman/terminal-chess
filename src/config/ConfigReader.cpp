#include <cstdlib>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "ConfigReader.hpp"
#include "ConfigMap.hpp"

ConfigReader::ConfigReader() {
    std::string line;
    std::ifstream config_file(this->m_getConfigFile());

    if (!config_file.is_open()) { 
        std::cerr << "config file not found" << '\n';
        return; 
    }

    // Get each line and put assertions to variable map for color configurations
    while (getline (config_file, line)) {
        if (line.length() < 9) {continue;} // NOTE: As of now, expected minimum assertion size is 9 as in 'a=#bcdefg'
                                                // but will probably be a bit larger, as no way config var one char size
        int equalsign_idx = this->m_cleanLine(line);
        if (equalsign_idx == -1 || line.at(0) == '#' || line.at(0) == '\n' || line.at(0) == '\r') {continue;}

        std::string key = line.substr(0, equalsign_idx);
        std::string value = line.substr(equalsign_idx+1);

        // TODO:
            // Now place key and value into map that connects key to function
              // that will clean value for that key, turn into color code, or
              // whatever configuration, and then set global value for reference
              // in main -> maybe singleton class? or pass an interface to method?
        if (config_map.find(key) != config_map.end()) {
            config_map.at(key)(value);
        } else {std::cerr << "Config file variable " << key << " does not exist" << '\n';}
    }
    
    config_file.close();
}

int ConfigReader::m_cleanLine(std::string& line) {
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    if (line.length() == 0) {return -1;}

    size_t equal_spot = line.find('=', 0);
    if (equal_spot != std::string::npos) {
        size_t hash_spot = line.find('#', equal_spot+2);
        if (hash_spot != std::string::npos) {
            line = line.substr(0, hash_spot);
        }
        return equal_spot;
    }
    return -1;
}

std::string ConfigReader::m_getConfigFile() {
    // different paths to check on OS
    #if defined(_WIN32)
        std::string appdata = "";

        if (std::getenv("APPDATA") != nullptr) {appdata = std::string(std::getenv("APPDATA")) + "\\chess.conf";}
        std::string config_locations[] = {
            appdata,
            std::filesystem::current_path().string() + "\\example_config\\chess.conf",
        };
    #else
        std::string home = "";
        std::string xdg_home = "";

        if (std::getenv("HOME") != nullptr) {home = std::string(std::getenv("HOME")) + "/.config/chess.conf";}
        if (std::getenv("XDG_CONFIG_HOME") != nullptr) {xdg_home = std::string(std::getenv("XDG_CONFIG_HOME")) + "/.config/chess.conf";}

        std::string config_locations[] = {
            home,
            xdg_home,
            std::filesystem::current_path().string() + "/example_config/chess.conf",
        };
    #endif

    for (const std::string& path : config_locations) {
        std::ifstream config_file(path);
        if (config_file.is_open()) {return path;}
    }
    return "";
}
