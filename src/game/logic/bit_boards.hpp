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
    };

    extern Side white;
    extern Side BlackSide;
}

//REMOVE
void bitboardDevFunc();

#endif //TERMINALCHESS_BIT_BOARDS_H
