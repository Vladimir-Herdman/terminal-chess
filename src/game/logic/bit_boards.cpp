//REMOVE
    // OVERALL PLAN:
    // creante constexpr SideCompileds white_comp and black_comp for compile time
    // optimizations, then use SideCompiled white and black for runtime usage. The
    // goal is to compile time as much as possible for faster runtime usage,
    // so it may get a little dirtier here at first.
// NOTE: use static_assert() to check compile time values are actually 'compile-time'
#include <array>
#include <bit>
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
    constexpr U64 RANK_2 = 0x000000000000FF00ull;
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
        constexpr U64 getAllPawnMoves(SideCompiled opposing) const;
        constexpr U64 getAllRookMoves(SideCompiled opposing) const;
        constexpr U64 getAllBishopMoves(SideCompiled opposing) const;
        constexpr U64 getAllQueenMoves(SideCompiled opposing) const;

        static constexpr U64 getKnightMoves(const U64 piece);
        static constexpr U64 getKingMoves(const U64 piece);
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
        //const int piece_x = piece_index % 8; // For future reference if debugging or needed
        //const int piece_y = piece_index / 8;
        const int shift = ((7-(piece_index % 8))-(piece_index / 8));

        if (shift > 0) {return DIAGONAL >> shift*8;}
        return DIAGONAL << (-shift)*8;
    }
    constexpr U64 getAntidiagonal(const int piece_index) {
        const int shift = ((piece_index % 8)-(piece_index / 8));

        if (shift > 0) {return ANTIDIAGONAL >> shift*8;}
        return ANTIDIAGONAL << (-shift)*8;
    }
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
    constexpr U64 SideCompiled::getAllPawnMoves(const SideCompiled opposing) const {
        const U64 opp_piecesMask = (~opposing.getAllPieces());
        const U64 blockers_mask = (opp_piecesMask & ~getAllPieces());

        const U64 moves_single = (pawns << 8) & opp_piecesMask;
        const U64 moves_double = (((pawns & RANK_2) << 8) & opp_piecesMask) << 8;

        U64 pawns_temp = pawns;
        int i = 0;
        //TODO: pawn attacks in moves
        //while (pawns_temp) {
        //    i += std::countr_zero(pawns_temp);
        //    pawns_temp
        //}

        // NOTE: When at runtime, use a board check for if to search en passant
            // if (board.white.en_passant_potential) { ...CheckBoardForAvailableWhiteMoves... }

        return (moves_single | moves_double) & blockers_mask;
    }
    constexpr U64 SideCompiled::getAllRookMoves(const SideCompiled opposing) const {
        U64 moves = 0ull;
        U64 rooks_temp = rooks;
        while (rooks_temp) {
            int i = std::countr_zero(rooks_temp);
            rooks_temp &= rooks_temp - 1;
            moves |= this->m_slidingMoves(i, getFile(i), opposing);
            moves |= this->m_slidingMoves(i, getRank(i), opposing);
        }
        return moves;
    }
    constexpr U64 SideCompiled::getAllBishopMoves(const SideCompiled opposing) const { //TODO: Finish
        U64 moves = 0ull;
        U64 bishops_temp = bishops;
        while (bishops_temp) {
            int i = std::countr_zero(bishops_temp);
            bishops_temp &= bishops_temp - 1;
            moves |= this->m_slidingMoves(i, getDiagonal(i), opposing);
            moves |= this->m_slidingMoves(i, getAntidiagonal(i), opposing);
        }
        return moves;
    }
    constexpr U64 SideCompiled::getAllQueenMoves(const SideCompiled opposing) const { //TODO: Finish
        U64 moves = 0ull;
        U64 queens_temp = queens;
        while (queens_temp) {
            int i = std::countr_zero(queens_temp);
            queens_temp &= queens_temp - 1;
            moves |= this->m_slidingMoves(i, getFile(i), opposing);
            moves |= this->m_slidingMoves(i, getRank(i), opposing);
            moves |= this->m_slidingMoves(i, getDiagonal(i), opposing);
            moves |= this->m_slidingMoves(i, getAntidiagonal(i), opposing);
        }
        return moves;
    }
    constexpr U64 SideCompiled::getKnightMoves(const U64 piece) {
        return  ((((piece >> 6) | (piece << 10)) & ~FILE_GH) |
                (((piece >> 10) | (piece << 6))  & ~FILE_AB) |
                (((piece >> 15) | (piece << 17)) & ~FILE_H)  |
                (((piece >> 17) | (piece << 15)) & ~FILE_A));
    }
    constexpr U64 SideCompiled::getKingMoves(const U64 piece) {
        return  ((piece << 1) & (~FILE_H) | (piece >> 1) & (~FILE_A) |
                (piece << 8)              | (piece >> 8)             |
                (piece << 7) & (~FILE_A)  | (piece >> 7) & (~FILE_H) |
                (piece << 9) & (~FILE_H)  | (piece >> 9) & (~FILE_A));
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

    // Seperate functions
    constexpr std::array<U64, 64> getKingMovesLookupTable() {
        std::array<U64, 64> table{};
        for (int i = 0; i < 64; i++) {
            table[i] = SideCompiled::getKingMoves(1ull << i);
        }
        return table;
    }
    constexpr std::array<U64, 64> getKnightMovesLookupTable() {
        std::array<U64, 64> table{};
        for (int i = 0; i < 64; i++) {
            table[i] = SideCompiled::getKnightMoves(1ull << i);
        }
        return table;
    }
    constexpr std::array<U64, 64> getPawnAttackLookupTable() {
        std::array<U64, 64> table{};
        for (int i = 0; i < 64; i++) {
            U64 moves = (2ull << i) & ~FILE_H;
            if (i > 0) {moves |= ((1ull << (i-1)) & ~FILE_A);}
            table[i] = moves;
        }
        return table;
    }
}

//TODO: Fix with Side once SideCompiled is finished
namespace BITBOARDS {
    namespace PRECOMPILED {
        constexpr std::array<U64, 64> kingMovesLookup = getKingMovesLookupTable();
        constexpr std::array<U64, 64> knightMovesLookup = getKnightMovesLookupTable();
        constexpr std::array<U64, 64> pawnAttackLookup = getPawnAttackLookupTable();  //TODO: Use some white/black finagling to figure out attack index at runtime
    }
    //constexpr int popLSB(U64 bitboard) {
    //    for (int i = 0; i < 64; i++) {
    //        if (bitboard & (1ull << i)) {
    //            bitboard ^= (1ull << i);
    //            return i;
    //        }
    //    } return -1;
    //}

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
    constexpr U64 square = white_compiled.getKnightMoves(0x0000000000000002ull);
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
    //printBitBoard(getAntidiagonal(57));
    //static_assert(getAntidiagonal(0) > 0, "This is not at compile time");
    printBitBoard(white_compiled.getAllQueenMoves(black_compiled));
    static_assert(getDiagonal(0) >= 0, "test here");
    static_assert(white_compiled.getAllQueenMoves(black_compiled) >= 0, "Not compile-time");

    std::cout << '\n';
    printBitBoard(BITBOARDS::PRECOMPILED::kingMovesLookup[26]);
    static_assert(BITBOARDS::PRECOMPILED::kingMovesLookup[26] >= 0, "Not compile-time");

    std::cout << '\n';
    printBitBoard(BITBOARDS::PRECOMPILED::knightMovesLookup[63]);
    static_assert(BITBOARDS::PRECOMPILED::knightMovesLookup[63] >= 0, "Not compile-time");

    //std::cout << '\n';
    //printBitBoard(white_compiled.getAllPawnMoves(black_compiled));
    //static_assert(white_compiled.getAllPawnMoves(black_compiled) >= 0, "Not compile-time");

    printBitBoard(BITBOARDS::PRECOMPILED::pawnAttackLookup[0]);
    static_assert(BITBOARDS::PRECOMPILED::pawnAttackLookup[0] > 0, "not compile-time");
}
#endif
