#include "crossplatform/commandline.hpp"

#include <iostream>

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

#include "config/ConfigData.hpp"

namespace {
    void usage() {
        std::cout << "Usage: term-chess [OPTION]\n\n";
        std::cout << "  -h\tdisplay help text\n";
        std::cout << "  -d\tdry-run chess match with default configuration settings (no config file reading)\n";
        std::cout << "  -v\tverbose mode, print more output during startup\n";
        std::cout << "\nAll command line arguments are available as config file ";
        std::cout << "options under the 'options_*' style, examples in 'example_config/chess.conf'.";
        exit(0);
    }
    void unknown_option(const char opt) {
        std::cout << "Invalid command line argument passed: -" << opt << '\n';
        exit(1);
    }
}
// Parse command line arguments and change config as a result
void command_line_arguments(int& argc, char *argv[]) {
#if defined(__unix__) || defined(__APPLE__)
    int opt;
    while ((opt = getopt(argc, argv, ":hdv")) != -1) {
        switch (opt) {
            case 'h': //help
                usage(); break;
            case 'd': //dry-run
                CONFIG::OPTIONS.dry_run = true; break;
            case 'v': //verbose
                CONFIG::OPTIONS.verbose = true; break;
            case '?':
                unknown_option(optopt); break;
        }
    }
#else
    for (int i = 1; i < argc; i++) {
        const char arg = argv[i][1];
        switch (arg) {
            case 'h': usage(); break; 
            case 'd': CONFIG::OPTIONS.dry_run = true; break; //defaults, so no config reading
            case 'v': CONFIG::OPTIONS.verbose = true; break; //verbose
            default: unknown_option(arg);
        }
    }
#endif
}
