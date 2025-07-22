#include "game/ui/ui.hpp"

#include <cstdint>
#include <string>

#include "config/ConfigData.hpp"

using u64 = std::uint64_t;

auto& COLORS = CONFIG::COLORS;
auto& PIECES = CONFIG::PIECES;

std::string PIECE = "   ";

// Check options and configure ui based off such configurations
    // For example, check for the no letter or numbers option to turn off
    // those pieces from board
UI::UI() {
    m_assign_board();
}


//void setPiece(int row, int column) {
//    PIECE = ui_lookup[static_cast<int>(board[row-1][column-1])];
//
//    // Old logic (just prints a board, no connection to bitboards)
//    //switch (row) {
//    //    case 2: PIECE = PIECES.b_pawn; break;
//    //    case 7: PIECE = PIECES.w_pawn; break;
//    //    case 1: 
//    //        switch (column) {
//    //            case 2: PIECE = PIECES.b_knight; break;
//    //            case 7: PIECE = PIECES.b_knight; break;
//    //            case 1: PIECE = PIECES.b_rook; break;
//    //            case 8: PIECE = PIECES.b_rook; break;
//    //            case 3: PIECE = PIECES.b_bishop; break;
//    //            case 6: PIECE = PIECES.b_bishop; break;
//    //            case 4: PIECE = PIECES.b_queen; break;
//    //            default: PIECE = PIECES.b_king; break;
//    //        }; break;
//    //    case 8:
//    //        switch (column) {
//    //            case 2: PIECE = PIECES.w_knight; break;
//    //            case 7: PIECE = PIECES.w_knight; break;
//    //            case 1: PIECE = PIECES.w_rook; break;
//    //            case 8: PIECE = PIECES.w_rook; break;
//    //            case 3: PIECE = PIECES.w_bishop; break;
//    //            case 6: PIECE = PIECES.w_bishop; break;
//    //            case 4: PIECE = PIECES.w_queen; break;
//    //            default: PIECE = PIECES.w_king; break;
//    //        }; break;
//    //}
//}
//
//void initializeWhiteBoard() {
//    int i, j;
//    std::string edge_h = COLORS.edge + "   " + COLORS.reset;
//    std::string edge_v = COLORS.edge + "  " + COLORS.reset;
//    std::string background = COLORS.b_bg;
//    std::string foreground = COLORS.b_fg;
//
//    for (i = 0; i < 10; i++) {
//        for (j = 0; j < 10; j++) {
//
//            if (j == 0) { // Edges
//                if (i > 0 && i < 9) { edge_v[19] = '9'-i; }
//                std::cout << edge_v;
//                continue;
//            }
//            if (j == 9) { edge_v[19] = ' '; std::cout << edge_v; continue; }
//
//            if (i == 0) { edge_h[20] = ' '; std::cout << edge_h; continue; } 
//            if (i == 9) {
//                if (j > 0 && j < 9) { edge_h[20] = '`'+j; }
//                std::cout << edge_h; 
//                continue;
//            }
//
//            background = ((j+i)%2 == 0) ? COLORS.w_bg : COLORS.b_bg;
//
//            setPiece(i, j);
//
//            // piece[0] = map(j);
//
//            //printf("%s%s%s\x1B[0m", background, foreground, PIECE);
//            std::cout << background << foreground << PIECE;
//            PIECE = "   ";
//        }
//        std::cout << "\n";
//        foreground = (i < 4) ? COLORS.b_fg : COLORS.w_fg;
//    }
//}
