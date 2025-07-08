#include <cstdint>
#include <iostream> //REMOVE

#include "game/logic/bit_boards.hpp"

namespace {
    //Usings
    using U64 = std::uint64_t;

    //Variables
    constexpr U64 FILE_A = 0x8080808080808080ull;
    constexpr U64 FILE_AB = (FILE_A | 0x4040404040404040ull);
    constexpr U64 FILE_H = 0x0101010101010101ull;
    constexpr U64 FILE_GH = (0x0202020202020202ull | FILE_H);

    constexpr U64 RANK_1 = 0x00000000000000FFull;
    constexpr U64 RANK_8 = 0xFF00000000000000ull;
}

namespace BITBOARDS {
    // Individual sides
    Side white {
        .pawns   = 0x000000000000FF00ull,
        .rooks   = 0x0000000000000081ull,
        .knights = 0x0000000000000042ull,
        .bishops = 0x0000000000000024ull,
        .queens  = 0x0000000000000010ull,
        .king    = 0x0000000000000008ull,
        .opposing = black,
    };
    Side black {
        .pawns   = 0x00FF000000000000ull,
        .rooks   = 0x8100000000000000ull,
        .knights = 0x4200000000000000ull,
        .bishops = 0x2400000000000000ull,
        .queens  = 0x1000000000000000ull,
        .king    = 0x0800000000000000ull,
        .opposing = white,
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
    constexpr U64 Side::reverseBoard(U64 board) {
        board = (board >> 1 & 0x5555555555555555ull) | ((board & 0x5555555555555555ull) << 1);
        board = (board >> 2 & 0x3333333333333333ull) | ((board & 0x3333333333333333ull) << 2);
        board = (board >> 4 & 0x0F0F0F0F0F0F0F0Full) | ((board & 0x0F0F0F0F0F0F0F0Full) << 4);
        board = (board >> 8 & 0x00FF00FF00FF00FFull) | ((board & 0x00FF00FF00FF00FFull) << 8);
        board = (board >> 16 & 0x0000FFFF0000FFFFull) | ((board & 0x0000FFFF0000FFFFull) << 16);
        board = (board >> 32) | (board << 32);
        return board;
    }
    constexpr U64 Side::slidingMoves(const int piece_index, const U64 mask) {
        U64 pieceBB = 1ull << piece_index;
        U64 movesBB = 0ull;
        U64 negative = 0ull;
        U64 friendly = this->getAllPieces();

        U64 occupied = friendly | opposing.getAllPieces();
        U64 occMask = occupied & mask;

        movesBB = occMask^(occMask-(pieceBB<<1));
        movesBB &= mask;

        negative = occMask^reverseBoard(reverseBoard(occMask)-(reverseBoard(pieceBB)<<1));
        negative &= mask;
        negative &= (1ull<<piece_index)-1;
        movesBB |= negative;

        movesBB &= ~friendly;
        return movesBB;
    }
}

#ifdef DEV_HELPERS_LATER_REMOVE

int popLSB(U64& bitboard) {
    for (int i = 0; i < 64; i++) {
        if (bitboard & (1ull << i)) {
            bitboard ^= (1ull << i);
            return i;
        }
    }
    return -1;
}

void printBitBoard(U64 num) {
    for (int i = 0; i < 64; i++) {
        // Check first digit of num and print out, move on to next
        if ((num & 0x8000000000000000) == 0x8000000000000000) {
            std::cout << "1 ";
        } else {std::cout << "0 ";}
        num <<= 1;
        if ((i+1) % 8 == 0) {
            std::cout << '\n';
        }
    }
}


//REMOVE - may be deprecated
// piece is a bitboard of all 0's and a single 1 to represent the piece
U64 getFile(const U64 piece) {
    U64 file = FILE_A;
    for (int i = 0; i < 8; i++) {
        if (file & piece) {break;}
        file >>= 1;
    }
    return file;
}
U64 getFile(const int piece_index) {
    return (FILE_H << (piece_index % 8));
}

//REMOVE - may be deprecated
U64 getRank(const U64 piece) {
    U64 rank = RANK_1;
    for (int i = 0; i < 8; i++) {
        if (rank & piece) {break;}
        rank <<= 8;
    }
    return rank;
}
U64 getRank(const int piece_index) {
    return (RANK_1 << ((piece_index / 8)*8));
}   

void bitboardDevFunc() {
    //printBitBoard(whiteSide);
    //constexpr U64 king = 0x0000000008000000ull;
    //printBitBoard(kingMoves(king));
    //printBitBoard(BITBOARDS::black.getKnightMoves());
    U64 rook_ex = 0x0000000800000000ull;
    int square = popLSB(rook_ex);
    U64 file = getFile(square);
    U64 rank = getRank(square);
    //U64 moves = sliding_moves(square, file);
    //moves |= sliding_moves(square, rank);

    printBitBoard((0x0000080000000000ull));
    std::cout << '\n';
    printBitBoard(BITBOARDS::white.reverseBoard(0x0000080000000000ull));
}
#endif
