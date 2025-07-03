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

    //Functions
    constexpr U64 leftShift(const U64 b) {return b << 1;}
    U64 getSidePieces(BITBOARDS::Side side) {
        return side.pawns | side.bishops | side.knights | side.king | side.queens | side.rooks;
    }
    
    constexpr U64 kingMoves(U64 piece) {
        return  ((piece << 1) & (~FILE_H)    | (piece >> 1) & (~FILE_A)  |
                 (piece << 8)                | (piece >> 8)              |
                 (piece << 7) & (~FILE_A)    | (piece >> 7) & (~FILE_H)  |
                 (piece << 9) & (~FILE_H)    | (piece >> 9) & (~FILE_A)) & (~getSidePieces(BITBOARDS::white));
    }
    constexpr U64 knightMoves(U64 piece) {
        return  ((((piece >> 6)  | (piece << 10)) & ~FILE_GH) |
                 (((piece >> 10) | (piece << 6))  & ~FILE_AB) |
                 (((piece >> 15) | (piece << 17)) & ~FILE_H)  |
                 (((piece >> 17) | (piece << 15)) & ~FILE_A)) & (~getSidePieces(BITBOARDS::white));
    }
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
    U64 whiteSide = 
        BITBOARDS::white.pawns |
        BITBOARDS::white.rooks |
        BITBOARDS::white.knights |
        BITBOARDS::white.bishops |
        BITBOARDS::white.queens |
        BITBOARDS::white.king;
    //printBitBoard(whiteSide);
    //constexpr U64 king = 0x0000000008000000ULL;
    //printBitBoard(kingMoves(king));
    U64 piece = 0x0000000002000000ULL;
    printBitBoard(knightMoves(piece));
}
#endif
