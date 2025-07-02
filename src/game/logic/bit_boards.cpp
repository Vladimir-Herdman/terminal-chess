#include <cstdint>
#include <iostream> //REMOVE

#include "game/logic/bit_boards.hpp"

namespace {
    //usings
    using U64 = std::uint64_t;

    //variables
    constexpr U64 FILE_A = 0x8080808080808080ULL;
    constexpr U64 FILE_H = 0x0101010101010101ULL;

    constexpr U64 RANK_1 = 0x00000000000000FFULL;
    constexpr U64 RANK_8 = 0xFF00000000000000ULL;

    //Functions
    constexpr U64 leftShift(const U64 b) {return b << 1;}
    
    constexpr U64 kingMoves(U64 king) {
        return (king << 1) & (~FILE_H) |
            (king >> 1) & (~FILE_A)    |
            (king << 8)                |
            (king >> 8)                |
            (king << 7) & (~FILE_A)    |
            (king >> 7) & (~FILE_H)    |
            (king << 9) & (~FILE_H)    |
            (king >> 9) & (~FILE_A);
    }

    #ifdef DEV_HELPERS_LATER_REMOVE
    void printBitBoard(U64 num) {
        for (int i = 0; i < 64; i++) {
            // Check first digit of num and print out, move on to next
            if ((num & 0x8000000000000000) == 0x8000000000000000) {
                std::cout << "1 ";
            } else {std::cout << "0 ";}
            num = leftShift(num);
            if ((i+1) % 8 == 0) {
                std::cout << '\n';
            }
        }
    }
    #endif
}

namespace BITBOARDS {
    Side white {
        .pawns   = 0x000000000000FF00ULL,
        .rooks   = 0x0000000000000081ULL,
        .knights = 0x0000000000000042ULL,
        .bishops = 0x0000000000000024ULL,
        .queens  = 0x0000000000000010ULL,
        .king    = 0x0000000000000008ULL,
    };
    Side black {

    };
}

#ifdef DEV_HELPERS_LATER_REMOVE
void bitboardDevFunc() {
    U64 whiteSide = 
        BITBOARDS::white.pawns |
        BITBOARDS::white.rooks |
        BITBOARDS::white.knights |
        BITBOARDS::white.bishops |
        BITBOARDS::white.queens |
        BITBOARDS::white.king;
    //printBitBoard(whiteSide);
    constexpr U64 king = 0x0000000008000000ULL;
    printBitBoard(kingMoves(king));
}
#endif
