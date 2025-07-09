#ifndef TERMINALCHESS_BIT_BOARDS_H
#define TERMINALCHESS_BIT_BOARDS_H

#include <cstdint>

namespace BITBOARDS {
    constexpr int popLSB(const uint64_t bitboard); //REMOVE: maybe?

    //TODO: Fix this once SideCompiled is finished
    struct Side {
        uint64_t pawns;
        uint64_t rooks;
        uint64_t knights;
        uint64_t bishops;
        uint64_t queens;
        uint64_t king;
        Side& opposing;

        constexpr uint64_t getAllPieces();
        constexpr uint64_t getKnightMoves();
        constexpr uint64_t getKingMoves();
        //removes LSB from board, returns index of LSB
        constexpr uint64_t reverseBoard(uint64_t board);
        // Generate moves for ray mask based on sliding piece index
        constexpr uint64_t slidingMoves(const int piece_index, const uint64_t mask);
    };

    extern Side white;
    extern Side black;
}

#ifdef DEV_HELPERS_LATER_REMOVE
void bitboardDevFunc();
#endif

#endif //TERMINALCHESS_BIT_BOARDS_H
