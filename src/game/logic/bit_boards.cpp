#include <cstdint>
#include <iostream> //REMOVE

#include "game/logic/bit_boards.hpp"

namespace {
    //Usings
    using U64 = std::uint64_t;

    //Variables
    constexpr U64 FILE_A = 0x8080808080808080ULL;
    constexpr U64 FILE_AB = (FILE_A | 0x4040404040404040ULL);
    constexpr U64 FILE_H = 0x0101010101010101ULL;
    constexpr U64 FILE_GH = (0x0202020202020202ULL | FILE_H);

    constexpr U64 RANK_1 = 0x00000000000000FFULL;
    constexpr U64 RANK_8 = 0xFF00000000000000ULL;
}

namespace BITBOARDS {
    // Individual sides
    Side white {
        .pawns   = 0x000000000000FF00ULL,
        .rooks   = 0x0000000000000081ULL,
        .knights = 0x0000000000000042ULL,
        .bishops = 0x0000000000000024ULL,
        .queens  = 0x0000000000000010ULL,
        .king    = 0x0000000000000008ULL,
    };
    Side black {
        .pawns   = 0x00FF000000000000ULL,
        .rooks   = 0x8100000000000000ULL,
        .knights = 0x4200000000000000ULL,
        .bishops = 0x2400000000000000ULL,
        .queens  = 0x1000000000000000ULL,
        .king    = 0x0800000000000000ULL,
    };

    // Side member functions
    U64 Side::getAllPieces() {
        return pawns | bishops | knights | king | queens | rooks;
    }
    constexpr U64 Side::getKnightMoves() {
        return  ((((knights >> 6)  | (knights << 10)) & ~FILE_GH) |
                 (((knights >> 10) | (knights << 6))  & ~FILE_AB) |
                 (((knights >> 15) | (knights << 17)) & ~FILE_H)  |
                 (((knights >> 17) | (knights << 15)) & ~FILE_A)) & (~getAllPieces());
    }
    constexpr U64 Side::getKingMoves() {
        return  ((king << 1) & (~FILE_H) | (king >> 1) & (~FILE_A)  |
                 (king << 8)             | (king >> 8)              |
                 (king << 7) & (~FILE_A) | (king >> 7) & (~FILE_H)  |
                 (king << 9) & (~FILE_H) | (king >> 9) & (~FILE_A)) & (~getAllPieces());
    }
}

#ifdef DEV_HELPERS_LATER_REMOVE
constexpr U64 leftShift(const U64 b) {return b << 1;}

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

void bitboardDevFunc() {
    //printBitBoard(whiteSide);
    //constexpr U64 king = 0x0000000008000000ULL;
    //printBitBoard(kingMoves(king));
    //printBitBoard(BITBOARDS::black.getKnightMoves());
    printBitBoard(BITBOARDS::white.rooks);
}
#endif
