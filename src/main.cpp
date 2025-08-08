// TODO
    // Configuration file
        // Should be able to change all colors and symbols used throughout
        // Turn on / off a lot of settings, like highlight move, abc123 row/cols
        // Interactive Mode
            // VIM bindings?
    // Break apart some sections so more readable
    // Seperate code into other header and source files for logic based main.cpp
    // Command line arguments
        // Start from black side
        // Show both sides at once
#include <ios>

#include "config/ConfigData.hpp"
#include "game/game.hpp"
#include "crossplatform/commandline.hpp"

#ifdef TERMINALCHESS_INCLUDE_CONFIGREADER
    #include "config/ConfigReader.hpp"
#endif

auto& OPTIONS = CONFIG::OPTIONS;

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);

    // Before config file, direct chess command line options are applied
    commandLineArguments(argc, argv);

    // Read in config file, will overwrite any previous command line arguments
    #ifdef TERMINALCHESS_INCLUDE_CONFIGREADER
        if (!OPTIONS.dry_run) {ConfigReader();}
    #endif

    // Game will check options and configure, then begin will start everything to stdout
    // Game cleans up itself, begin runs loop and afterwards returns here to end
    Game game;
    game.begin();
    game.end();

    //#ifdef DEV_HELPERS_LATER_REMOVE
    //bitboardDevFunc();
    //#endif

    return 0;
}
