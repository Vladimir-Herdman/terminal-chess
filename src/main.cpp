// TODO
    // Configuration file
        // Should be able to change all colors and symbols used throughout
        // Turn on / off a lot of settings, like highlight move, abc123 row/cols
    // Break apart some sections so more readable
    // Seperate code into other header and source files for logic based main.cpp
    // Learn cmake for better make file setup for others and deviceas
    // Command line arguments
        // Start from black side
        // Show both sides at once
#include <stdbool.h>
#include <iostream>
#include <string>

#include "config/ConfigReader.h"
#include "config/ConfigData.h"

//ESC[38;2;{r};{g};{b}m - \x1B[38;2;255;255;255m(white) - Set foreground color as RGB
//ESC[48;2;{r};{g};{b}m - \1xB[48;2;255;255;255m(white) - Set background color as RGB
//ESC[0m - \x1B[0m - reset all modes styles and colors
//(165, 42, 42

auto& PIECES = CONFIG.PIECES;
auto& COLORS = CONFIG.COLORS;
auto& OPTIONS = CONFIG.OPTIONS;

std::string PIECE = "   ";
enum class enum_pieces { PAWN = 1, KNIGHT, BISHOP, ROOK, QUEEN, KING };

//ANSI rgb colors: b_=background, f_=foreground
// Keep all actual colors same width for later indexing ease

void setPiece(int row, int column) {
    switch (row) {
        case 2: PIECE = PIECES.b_pawn; break;
        case 7: PIECE = PIECES.w_pawn; break;
        case 1: 
            switch (column) {
                case 2: PIECE = PIECES.b_knight; break;
                case 6: PIECE = PIECES.b_knight; break;
                case 1: PIECE = PIECES.b_rook; break;
                case 7: PIECE = PIECES.b_rook; break;
                case 3: PIECE = PIECES.b_bishop; break;
                case 5: PIECE = PIECES.b_bishop; break;
                case 4: PIECE = PIECES.b_queen; break;
                default: PIECE = PIECES.b_king; break;
            }; break;
        case 8:
            switch (column) {
                case 2: PIECE = PIECES.w_knight; break;
                case 6: PIECE = PIECES.w_knight; break;
                case 1: PIECE = PIECES.w_rook; break;
                case 7: PIECE = PIECES.w_rook; break;
                case 3: PIECE = PIECES.w_bishop; break;
                case 5: PIECE = PIECES.w_bishop; break;
                case 4: PIECE = PIECES.w_queen; break;
                default: PIECE = PIECES.w_king; break;
            }; break;
    }
}

void initializeWhiteBoard() {
    int i, j;
    std::string edge_h = COLORS.edge + "   " + COLORS.reset;
    std::string edge_v = COLORS.edge + "  " + COLORS.reset;
    std::string background;
    std::string foreground = COLORS.b_fg;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {

            if (j == 0) { // Edges
                if (i > 0 && i < 9) { edge_v[19] = '9'-i; }
                std::cout << edge_v;
                continue;
            }
            if (j == 9) { edge_v[19] = ' '; std::cout << edge_v; continue; }

            if (i == 0) { edge_h[20] = ' '; std::cout << edge_h; continue; } 
            if (i == 9) {
                if (j > 0 && j < 9) { edge_h[20] = '`'+j; }
                std::cout << edge_h; 
                continue;
            }

            background = ((j+i)%2 == 0) ? COLORS.w_bg : COLORS.b_bg;

            setPiece(i, j);

            // piece[0] = map(j);

            //printf("%s%s%s\x1B[0m", background, foreground, PIECE);
            std::cout << background << foreground << PIECE;
            PIECE = "   ";
        }
        std::cout << "\n";
    }
}

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
    initializeWhiteBoard();
}
