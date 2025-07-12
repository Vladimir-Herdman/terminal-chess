//REMOVE
    // OVERALL PLAN:
    // creante constexpr SideCompileds white_comp and black_comp for compile time
    // optimizations, then use SideCompiled white and black for runtime usage. The
    // goal is to compile time as much as possible for faster runtime usage,
    // so it may get a little dirtier here at first.
// NOTE: use static_assert() to check compile time values are actually 'compile-time'
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

    constexpr U64 DIAGONAL = 0x0102040810204080ull;
    constexpr U64 DIAGONAL_MASK_EAST = 0x0102040810204080ull; //REMOVE?
    constexpr U64 DIAGONAL_MASK_WEST = 0x0102040810204080ull;

    constexpr U64 ANTIDIAGONAL = 0x8040201008040201ull;
    constexpr U64 ANTIDIAGONAL_MASK_EAST = 0ull;
    constexpr U64 ANTIDIAGONAL_MASK_WEST = 0ull;

    // Compiled Side struct
    struct SideCompiled {
        const U64 pawns;
        const U64 rooks;
        const U64 knights;
        const U64 bishops;
        const U64 queens;
        const U64 king;

        // Generate moves for ray mask based on sliding piece index
        constexpr U64 m_slidingMoves(const int piece_index, const U64 mask, SideCompiled opposing) const;

        constexpr U64 getAllPieces() const;
        //sliding piece move getters return all pieces possible moves
        //TODO: Later if needed, individual moves
        constexpr U64 getAllKnightMoves() const;
        constexpr U64 getAllRookMoves(SideCompiled opposing) const;
        constexpr U64 getAllBishopMoves(SideCompiled opposing) const;
        constexpr U64 getAllQueenMoves(SideCompiled opposing) const;
        constexpr U64 getAllKingMoves() const;
    };

    constexpr SideCompiled white_compiled {
        .pawns   = 0x000000000000FF00ull,
        .rooks   = 0x0000000000000081ull,
        .knights = 0x0000000000000042ull,
        .bishops = 0x0000000000000024ull,
        .queens  = 0x0000000000000010ull,
        .king    = 0x0000000000000008ull,
    };
    constexpr SideCompiled black_compiled {
        .pawns   = 0x00FF000000000000ull,
        .rooks   = 0x8100000000000000ull,
        .knights = 0x4200000000000000ull,
        .bishops = 0x2400000000000000ull,
        .queens  = 0x1000000000000000ull,
        .king    = 0x0800000000000000ull,
    };

    // Function definitions
    // Helpers
    // TODO: Lookup table for file, rank, diagonal, and antidiagonal
        // Although already pretty computationally irrelevant as this is at compile time,
        // think about doing this lookup table later on if such fucntions are needed at runtime
        // so we don't have to generate each time?
    constexpr U64 getFile(const int piece_index) {return (FILE_H << (piece_index % 8));}
    constexpr U64 getRank(const int piece_index) {return (RANK_1 << ((piece_index / 8)*8));}   
    constexpr U64 getDiagonal(const int piece_index) {
        const int piece_x = piece_index % 8;
        const int piece_y = piece_index / 8;

        U64 moves = DIAGONAL >> piece_x*8;
        moves = moves << piece_y*8;

        return moves;
    }
    constexpr U64 getAntidiagonal(const int piece_index) {return 0ull;} //TODO
    constexpr U64 reverseBoard(U64 board) {
        board = (board >> 1 & 0x5555555555555555ull) | ((board & 0x5555555555555555ull) << 1);
        board = (board >> 2 & 0x3333333333333333ull) | ((board & 0x3333333333333333ull) << 2);
        board = (board >> 4 & 0x0F0F0F0F0F0F0F0Full) | ((board & 0x0F0F0F0F0F0F0F0Full) << 4);
        board = (board >> 8 & 0x00FF00FF00FF00FFull) | ((board & 0x00FF00FF00FF00FFull) << 8);
        board = (board >> 16 & 0x0000FFFF0000FFFFull) | ((board & 0x0000FFFF0000FFFFull) << 16);
        board = (board >> 32) | (board << 32);
        return board;
    }

    //Compiled Struct functions
    constexpr U64 SideCompiled::getAllPieces() const {
        return pawns | bishops | knights | king | queens | rooks;
    }
    constexpr U64 SideCompiled::getAllKnightMoves() const {
        return  ((((knights >> 6)  | (knights << 10)) & ~FILE_GH) |
                (((knights >> 10) | (knights << 6))  & ~FILE_AB) |
                (((knights >> 15) | (knights << 17)) & ~FILE_H)  |
                (((knights >> 17) | (knights << 15)) & ~FILE_A)) & (~getAllPieces());
    }
    constexpr U64 SideCompiled::getAllRookMoves(SideCompiled opposing) const {
        U64 moves = 0ull;
        for (int i = 0; i < 64; i++) {
            // if true, found bit for 1
            if (rooks & (1ull << i)) {
                moves |= this->m_slidingMoves(i, getFile(i), opposing);
                moves |= this->m_slidingMoves(i, getRank(i), opposing);
            }
        }
        return moves;
    }
    constexpr U64 SideCompiled::getAllBishopMoves(SideCompiled opposing) const { //TODO: Finish
        U64 moves = 0ull;
        for (int i = 0; i < 64; i++) {
            // if true, found bit for 1
            if (bishops & (1ull << i)) {
                moves |= this->m_slidingMoves(i, getDiagonal(i), opposing);
                moves |= this->m_slidingMoves(i, getAntidiagonal(i), opposing);
            }
        }
        return moves;
    }
    constexpr U64 SideCompiled::getAllQueenMoves(SideCompiled opposing) const { //TODO: Finish
        U64 moves = 0ull;
        for (int i = 0; i < 64; i++) {
            // if true, found bit for 1
            if (queens & (1ull << i)) {
                moves |= this->m_slidingMoves(i, getFile(i), opposing);
                moves |= this->m_slidingMoves(i, getRank(i), opposing);
                moves |= this->m_slidingMoves(i, getDiagonal(i), opposing);
                moves |= this->m_slidingMoves(i, getAntidiagonal(i), opposing);
            }
        }
        return moves;
    }
    constexpr U64 SideCompiled::getAllKingMoves() const {
        return  ((king << 1) & (~FILE_H) | (king >> 1) & (~FILE_A)  |
                (king << 8)             | (king >> 8)              |
                (king << 7) & (~FILE_A) | (king >> 7) & (~FILE_H)  |
                (king << 9) & (~FILE_H) | (king >> 9) & (~FILE_A)) & (~getAllPieces());
    }
    constexpr U64 SideCompiled::m_slidingMoves(const int piece_index, const U64 mask, SideCompiled opposing) const {
        U64 piece_board = 1ull << piece_index;
        U64 moves_board = 0ull;
        U64 negative = 0ull;
        U64 friendly = this->getAllPieces();

        U64 occupied = friendly | opposing.getAllPieces();
        U64 occMask = occupied & mask;

        moves_board = occMask^(occMask-(piece_board<<1));
        moves_board &= mask;

        negative = occMask^reverseBoard(reverseBoard(occMask)-(reverseBoard(piece_board)<<1));
        negative &= mask;
        negative &= (1ull<<piece_index)-1;
        moves_board |= negative;

        moves_board &= ~friendly;
        return moves_board;
    }
}

//TODO: Fix with Side once SideCompiled is finished
namespace BITBOARDS {
    constexpr int popLSB(U64 bitboard) {
        for (int i = 0; i < 64; i++) {
            if (bitboard & (1ull << i)) {
                bitboard ^= (1ull << i);
                return i;
            }
        } return -1;
    }

    // Individual sides

    // SideCompiled member functions
}

#ifdef DEV_HELPERS_LATER_REMOVE

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


//REMOVE - may be deprecated, may be usefull later for runtime Side
// piece is a bitboard of all 0's and a single 1 to represent the piece
    //U64 getFile(const U64 piece) {
    //    U64 file = FILE_A;
    //    for (int i = 0; i < 8; i++) {
    //        if (file & piece) {break;}
    //        file >>= 1;
    //    }
    //    return file;
    //}
    //
    ////REMOVE - may be deprecated
    //U64 getRank(const U64 piece) {
    //    U64 rank = RANK_1;
    //    for (int i = 0; i < 8; i++) {
    //        if (rank & piece) {break;}
    //        rank <<= 8;
    //    }
    //    return rank;
    //}

void bitboardDevFunc() {
    constexpr U64 rook_ex = 0x0000000800000000ull;
    constexpr U64 square = white_compiled.getAllKnightMoves();
    //static assert for compile time checking of code
    //static_assert((square > 0), "square does not exist");
    U64 file = getFile(square);
    U64 rank = getRank(square);
    //U64 moves = sliding_moves(square, file);
    //moves |= sliding_moves(square, rank);

    //printBitBoard(reverseBoard(0x0000080000000000ull));
    //constexpr int pos = 30;
    //constexpr U64 examp = white_compiled.m_slidingMoves(pos, getRank(pos), black_compiled) | white_compiled.m_slidingMoves(pos, getFile(pos), black_compiled);
    //static_assert(examp > 0, "examp not compile time");
    //printBitBoard(examp);
    //constexpr U64 rookAttacks = black_compiled.getAllRookMoves(white_compiled);
    //static_assert(rookAttacks > 0, "rookAttacks not compile time based");
    printBitBoard(getDiagonal(8));
}
#endif
