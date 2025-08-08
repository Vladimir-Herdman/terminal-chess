#ifndef TERMINALCHESS_GAME_LOGIC_BIT_BOARDS_H
#define TERMINALCHESS_GAME_LOGIC_BIT_BOARDS_H

#include <cstdint>

#include "types/moves.hpp"

namespace BITBOARDS {
    //TODO: Fix this once SideCompiled is finished
    struct Side {
        uint64_t pawns;
        uint64_t rooks;
        uint64_t knights;
        uint64_t bishops;
        uint64_t queens;
        uint64_t king;
        const char side;
        const Side& opposing;

        uint64_t getAllPieces() const;
        uint64_t getAllPawnMoves() const;

        MoveResult makePawnMove(const int r, const int c);
    };

    //TODO:
        // Maybe the lookup tables in here for reference later? As they're needed?

    extern Side white;
    extern Side black;
}

#ifdef DEV_HELPERS_LATER_REMOVE
void bitboardDevFunc();
#endif

#endif //TERMINALCHESS_GAME_LOGIC_BIT_BOARDS_H
