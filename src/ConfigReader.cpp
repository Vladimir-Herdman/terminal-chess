//TODO
    // Better config file locations depending on OS
    // OS independent pathing
#include <cstdlib> //REMOVE
#include <algorithm>
#include <fstream>
#include <iostream> //REMOVE
#include <string>
#include "ConfigReader.h"

ConfigReader::ConfigReader() {
    std::string line;
    std::ifstream config_file("/home/vova/Code/terminal-chess/example_config/chess.conf");

    if (!config_file.is_open()) { 
        std::cerr << "config file not found" << '\n';
        return; 
    }

    while (getline (config_file, line)) {
        this->cleanLine(line);
        if (line.at(0) == '#' || line.at(0) == '\n' || line.at(0) == '\r') {continue;}
        std::cout << line << '\n';
    }
    
    config_file.close();

    exit(0); //REMOVE
}

void ConfigReader::cleanLine(std::string& line) {
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

    size_t equal_spot = line.find('=', 0);
    if (equal_spot != std::string::npos) {
        size_t hash_spot = line.substr(equal_spot).find('#', 2);
        if (hash_spot != std::string::npos) {
            line = line.substr(0, hash_spot);
        }
    }
}
