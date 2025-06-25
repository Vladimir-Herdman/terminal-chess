//TODO
    // Better config file locations depending on OS
    // OS independent pathing
#include <cstdlib> //REMOVE
#include <algorithm>
#include <fstream>
#include <iostream> //REMOVE
#include <string>
#include "ConfigReader.h"

// Unnamed namespace in place of static so it can apply to type declaration as well
// Define defaults here in case of no config file
ConfigData CONFIG = {
    .COLORS {
        .b_black = "\x1B[48;2;000;000;000m",
        .b_brown = "\x1B[48;2;082;031;000m",
        .b_white = "\x1B[48;2;234;233;230m",

        .f_black = "\x1B[38;2;000;000;000m",

        .reset = "\x1B[0m",
    },
    .PIECES {
        .pawn = " \u2659 ",
        .rook = " \u2656 ",
        .knight = " \u2658 ",
        .bishop = " \u2657 ",
        .queen = " \u2655 ",
        .king = " \u2654 ",
    },
};

namespace {
    using ptr_configFunction = void (*)(std::string&);

    static void background_color(std::string &value) { std::cout << "Test worked" << '\n'; }
    static void foreground_color(std::string& value) { std::cout << "Test worked" << '\n'; }

    std::map<std::string, ptr_configFunction> config_map = {
        {"test", background_color},
        {"test2", foreground_color},
    };
}

ConfigReader::ConfigReader() {
    std::string line;
    std::ifstream config_file("/home/vova/Code/terminal-chess/example_config/chess.conf");

    if (!config_file.is_open()) { 
        std::cerr << "config file not found" << '\n';
        return; 
    }

    // Get each line and put assertions to variable map for color configurations
    while (getline (config_file, line)) {
        if (line.length() < 9) {continue;} // NOTE: As of now, expected minimum assertion size is 9 as in 'a=#bcdefg'
                                                // but will probably be a bit larger, as no way config var one char size
        int equalsign_idx = this->cleanLine(line);
        if (line.at(0) == '#' || line.at(0) == '\n' || line.at(0) == '\r') {continue;}

        std::string key = line.substr(0, equalsign_idx);
        std::string value = line.substr(equalsign_idx+1);

        // TODO:
            // Now place key and value into map that connects key to function
              // that will clean value for that key, turn into color code, or
              // whatever configuration, and then set global value for reference
              // in main -> maybe singleton class? or pass an interface to method?
        if (config_map.find(key) != config_map.end()) {
            std::cout << "Key " << key << " does exists, so calling map on it" << '\n';
            config_map[key](value);
        } else {std::cerr << "Config file variable " << key << " does not exist" << '\n';}
    }
    
    config_file.close();

    exit(0); //REMOVE
}

int ConfigReader::cleanLine(std::string& line) {
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

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
