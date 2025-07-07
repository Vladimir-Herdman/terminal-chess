#ifndef TERMINALCHESS_BIT_BOARDS_H
#define TERMINALCHESS_BIT_BOARDS_H

#include <cstdint>

namespace BITBOARDS {
    struct Side {
        uint64_t pawns;
        uint64_t rooks;
        uint64_t knights;
        uint64_t bishops;
        uint64_t queens;
        uint64_t king;

        uint64_t getAllPieces();
        constexpr uint64_t getKnightMoves();
        constexpr uint64_t getKingMoves();
    };

    extern Side white;
    extern Side BlackSide;
}

#ifdef DEV_HELPERS_LATER_REMOVE
void bitboardDevFunc();
#endif

#endif //TERMINALCHESS_BIT_BOARDS_H
