// TODO
    // Configuration file
        // Should be able to change all colors and symbols used throughout
        // Turn on / off a lot of settings, like highlight move, abc123 row/cols
    // Break apart some sections so more readable
    // Seperate code into other header and source files for logic based main.cpp
    // Command line arguments
        // Start from black side
        // Show both sides at once
#include "config/ConfigReader.hpp"
#include "config/ConfigData.hpp"
#include "game/ui.hpp"

auto& OPTIONS = CONFIG::OPTIONS;

void commandLineArguments(int& argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        switch (arg[1]) {
            case 'v': OPTIONS.verbose = true; break; //verbose
            case 'd': OPTIONS.dry_run = true; break; //defaults, so no config reading
        }
    }
}

int main(int argc, char *argv[]) {
    // Before config file, direct chess command line options are applied
    commandLineArguments(argc, argv);

    // Read in config file, will overwrite any previous command line arguments
    if (!OPTIONS.dry_run) {ConfigReader();}

    // Show boards + start game
    //initializeWhiteBoard();
    initializeWhiteBoard();
}
