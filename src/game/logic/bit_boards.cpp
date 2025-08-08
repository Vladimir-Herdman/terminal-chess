//REMOVE
    // OVERALL PLAN:
    // creante constexpr SideCompileds white_comp and black_comp for compile time
    // optimizations, then use SideCompiled white and black for runtime usage. The
    // goal is to compile time as much as possible for faster runtime usage,
    // so it may get a little dirtier here at first.
// NOTE: use static_assert() to check compile time values are actually 'compile-time'
// NOTE: magic bitboards papaer
#include "game/logic/bit_boards.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <iostream> //REMOVE

#include "types/moves.hpp"

#define SCB_M(arg) static_cast<bool>(Move::arg)

namespace {
    //Usings
    using u64 = std::uint64_t;

    //Variables
    constexpr u64 FILE_A = 0x8080808080808080ull;
    constexpr u64 FILE_AB = (FILE_A | 0x4040404040404040ull);
    constexpr u64 FILE_H = 0x0101010101010101ull;
    constexpr u64 FILE_GH = (0x0202020202020202ull | FILE_H);

    constexpr u64 RANK_1 = 0x00000000000000FFull;
    constexpr u64 RANK_2 = 0x000000000000FF00ull;

    constexpr u64 DIAGONAL = 0x0102040810204080ull;

    constexpr u64 ANTIDIAGONAL = 0x8040201008040201ull;
    //this converts ui values to side compiled values with index's when needed
    //so, row 7 in ui should be row 0 here with how we u64 interact
    constexpr int lookup_ui_index[8] = {7, 6, 5, 4, 3 ,2 ,1, 0};

    // Compiled Side struct
    struct SideCompiled {
        const u64 pawns;
        const u64 rooks;
        const u64 knights;
        const u64 bishops;
        const u64 queens;
        const u64 king;

        // Generate moves for ray mask based on sliding piece index
        constexpr u64 m_slidingMoves(const int piece_index, const u64 mask, SideCompiled opposing) const;

        constexpr u64 getAllPieces() const;
        //sliding piece move getters return all pieces possible moves
        //TODO: Later if needed, individual moves
        constexpr u64 getAllPawnMoves(SideCompiled opposing) const;
        constexpr u64 getAllRookMoves(SideCompiled opposing) const;
        constexpr u64 getAllBishopMoves(SideCompiled opposing) const;
        constexpr u64 getAllQueenMoves(SideCompiled opposing) const;

        static constexpr u64 getKnightMoves(const u64 piece);
        static constexpr u64 getKingMoves(const u64 piece);
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
    constexpr u64 getFile(const int piece_index) {return (FILE_H << (piece_index % 8));}
    constexpr u64 getRank(const int piece_index) {return (RANK_1 << ((piece_index / 8)*8));}
    // r and c using ui board numbering, so r=0 means here, but should be left for there
    constexpr int getPieceIndex(const int r, const int c) {return (lookup_ui_index[r]*8 + lookup_ui_index[c]);}
    constexpr u64 getDiagonal(const int piece_index) {
        //const int piece_x = piece_index % 8; // For future reference if debugging or needed
        //const int piece_y = piece_index / 8;
        const int shift = ((7-(piece_index % 8))-(piece_index / 8));

        if (shift > 0) {return DIAGONAL >> shift*8;}
        return DIAGONAL << (-shift)*8;
    }
    constexpr u64 getAntidiagonal(const int piece_index) {
        const int shift = ((piece_index % 8)-(piece_index / 8));

        if (shift > 0) {return ANTIDIAGONAL >> shift*8;}
        return ANTIDIAGONAL << (-shift)*8;
    }
    constexpr u64 reverseBoard(u64 board) {
        board = (board >> 1 & 0x5555555555555555ull) | ((board & 0x5555555555555555ull) << 1);
        board = (board >> 2 & 0x3333333333333333ull) | ((board & 0x3333333333333333ull) << 2);
        board = (board >> 4 & 0x0F0F0F0F0F0F0F0Full) | ((board & 0x0F0F0F0F0F0F0F0Full) << 4);
        board = (board >> 8 & 0x00FF00FF00FF00FFull) | ((board & 0x00FF00FF00FF00FFull) << 8);
        board = (board >> 16 & 0x0000FFFF0000FFFFull) | ((board & 0x0000FFFF0000FFFFull) << 16);
        board = (board >> 32) | (board << 32);
        return board;
    }

    //Compiled Struct functions
    constexpr u64 SideCompiled::getAllPieces() const {
        return pawns | bishops | knights | king | queens | rooks;
    }
    constexpr u64 SideCompiled::getAllPawnMoves(const SideCompiled opposing) const {
        const u64 opp_piecesMask = (~opposing.getAllPieces());
        const u64 blockers_mask = (opp_piecesMask & ~getAllPieces());

        const u64 moves_single = (pawns << 8) & opp_piecesMask;
        const u64 moves_double = (((pawns & RANK_2) << 8) & opp_piecesMask) << 8;

        //u64 pawns_temp = pawns;
        //int i = 0;
        //TODO: pawn attacks in moves
        //while (pawns_temp) {
        //    i += std::countr_zero(pawns_temp);
        //    pawns_temp
        //}

        // NOTE: When at runtime, use a board check for if to search en passant
            // if (board.white.en_passant_potential) { ...CheckBoardForAvailableWhiteMoves... }

        return (moves_single | moves_double) & blockers_mask;
    }
    constexpr u64 SideCompiled::getAllRookMoves(const SideCompiled opposing) const {
        u64 moves = 0ull;
        u64 rooks_temp = rooks;
        while (rooks_temp) {
            int i = std::countr_zero(rooks_temp);
            rooks_temp &= rooks_temp - 1;
            moves |= this->m_slidingMoves(i, getFile(i), opposing);
            moves |= this->m_slidingMoves(i, getRank(i), opposing);
        }
        return moves;
    }
    constexpr u64 SideCompiled::getAllBishopMoves(const SideCompiled opposing) const { //TODO: Finish
        u64 moves = 0ull;
        u64 bishops_temp = bishops;
        while (bishops_temp) {
            int i = std::countr_zero(bishops_temp);
            bishops_temp &= bishops_temp - 1;
            moves |= this->m_slidingMoves(i, getDiagonal(i), opposing);
            moves |= this->m_slidingMoves(i, getAntidiagonal(i), opposing);
        }
        return moves;
    }
    constexpr u64 SideCompiled::getAllQueenMoves(const SideCompiled opposing) const { //TODO: Finish
        u64 moves = 0ull;
        u64 queens_temp = queens;
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
    constexpr u64 SideCompiled::getKnightMoves(const u64 piece) {
        return  ((((piece >> 6) | (piece << 10)) & ~FILE_GH) |
                (((piece >> 10) | (piece << 6))  & ~FILE_AB) |
                (((piece >> 15) | (piece << 17)) & ~FILE_H)  |
                (((piece >> 17) | (piece << 15)) & ~FILE_A));
    }
    constexpr u64 SideCompiled::getKingMoves(const u64 piece) {
        return  ((piece << 1) & (~FILE_H) | (piece >> 1) & (~FILE_A) |
                (piece << 8)              | (piece >> 8)             |
                (piece << 7) & (~FILE_A)  | (piece >> 7) & (~FILE_H) |
                (piece << 9) & (~FILE_H)  | (piece >> 9) & (~FILE_A));
    }
    constexpr u64 SideCompiled::m_slidingMoves(const int piece_index, const u64 mask, SideCompiled opposing) const {
        u64 piece_board = 1ull << piece_index;
        u64 moves_board = 0ull;
        u64 negative = 0ull;
        u64 friendly = this->getAllPieces();

        u64 occupied = friendly | opposing.getAllPieces();
        u64 occMask = occupied & mask;

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
    constexpr std::array<u64, 64> getKingMovesLookupTable() {
        std::array<u64, 64> table{};
        for (int i = 0; i < 64; i++) {
            table[i] = SideCompiled::getKingMoves(1ull << i);
        }
        return table;
    }
    constexpr std::array<u64, 64> getKnightMovesLookupTable() {
        std::array<u64, 64> table{};
        for (int i = 0; i < 64; i++) {
            table[i] = SideCompiled::getKnightMoves(1ull << i);
        }
        return table;
    }
    constexpr std::array<u64, 64> getPawnAttackLookupTable() {
        std::array<u64, 64> table{};
        for (int i = 0; i < 64; i++) {
            u64 moves = (2ull << i) & ~FILE_H;
            if (i > 0) {moves |= ((1ull << (i-1)) & ~FILE_A);}
            table[i] = moves;
        }
        return table;
    }
    void printBB(u64 num) { //REMOVE
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
} //end unnamed namespace

//TODO: Fix with Side once SideCompiled is finished
namespace BITBOARDS {
    namespace PRECOMPILED {
        constexpr std::array<u64, 64> lookup_king_moves = getKingMovesLookupTable();
        constexpr std::array<u64, 64> lookup_knight_moves = getKnightMovesLookupTable();
        constexpr std::array<u64, 64> lookup_pawn_attacks = getPawnAttackLookupTable();  //TODO: Use some white/black finagling to figure out attack index at runtime
    }

    u64 Side::getAllPieces() const {return pawns | bishops | knights | king | queens | rooks;}
    u64 Side::getAllPawnMoves() const {
        const u64 opp_piecesMask = (~opposing.getAllPieces());
        const u64 blockers_mask = (opp_piecesMask & ~getAllPieces());

        const u64 moves_single = (pawns << 8) & opp_piecesMask;
        const u64 moves_double = (((pawns & RANK_2) << 8) & opp_piecesMask) << 8;

        return (moves_single | moves_double) & blockers_mask;
    }

    MoveResult Side::makePawnMove(const int r, const int c) {
        //TODO: need to account for en peassant
        if (side == 'w') {
            const int move_index = getPieceIndex(r, c);
            const u64 move = (1ull<<move_index);
            const u64 allPawnMoves = getAllPawnMoves();

            if (move & allPawnMoves) {
                const u64 potential_one = pawns & (move >> 8);
                const u64 potential_two = pawns & (move >> 16);
                //TODO: check for king in check for legality of move
                ////if (isKingInCheck(new_board)) {return ILLEGAL}
                if (potential_one) {
                    pawns = ((pawns & ~potential_one) | move);
                    return {SCB_M(LEGAL), r+2, c+1}; //MoveResult is {legal,row,column}
                } else if (r == 4 && potential_two) {
                    pawns = ((pawns & ~potential_two) | move);
                    return {SCB_M(LEGAL), r+3, c+1};
                }
            }
        }
        //TODO: blackside pawn movement implementation
        return {SCB_M(ILLEGAL), SCB_M(NONE), SCB_M(NONE)};
    }

    Side white = {
        .pawns    = 0x000000000000FF00ull,
        .rooks    = 0x0000000000000081ull,
        .knights  = 0x0000000000000042ull,
        .bishops  = 0x0000000000000024ull,
        .queens   = 0x0000000000000010ull,
        .king     = 0x0000000000000008ull,
        .side     = 'w',
        .opposing = black,
    };
    Side black = {
        .pawns    = 0x00FF000000000000ull,
        .rooks    = 0x8100000000000000ull,
        .knights  = 0x4200000000000000ull,
        .bishops  = 0x2400000000000000ull,
        .queens   = 0x1000000000000000ull,
        .king     = 0x0800000000000000ull,
        .side     = 'b',
        .opposing = white,
    };
    //constexpr int popLSB(u64 bitboard) {
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

void printBitBoard(u64 num) {
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
    //u64 getFile(const u64 piece) {
    //    u64 file = FILE_A;
    //    for (int i = 0; i < 8; i++) {
    //        if (file & piece) {break;}
    //        file >>= 1;
    //    }
    //    return file;
    //}
    //
    ////REMOVE - may be deprecated
    //u64 getRank(const u64 piece) {
    //    u64 rank = RANK_1;
    //    for (int i = 0; i < 8; i++) {
    //        if (rank & piece) {break;}
    //        rank <<= 8;
    //    }
    //    return rank;
    //}

void bitboardDevFunc() {
    //constexpr u64 rook_ex = 0x0000000800000000ull;
    //constexpr u64 square = white_compiled.getKnightMoves(0x0000000000000002ull);
    //static assert for compile time checking of code
    //static_assert((square > 0), "square does not exist");
    //u64 file = getFile(square);
    //u64 rank = getRank(square);
    //u64 moves = sliding_moves(square, file);
    //moves |= sliding_moves(square, rank);

    //printBitBoard(reverseBoard(0x0000080000000000ull));
    //constexpr int pos = 30;
    //constexpr u64 examp = white_compiled.m_slidingMoves(pos, getRank(pos), black_compiled) | white_compiled.m_slidingMoves(pos, getFile(pos), black_compiled);
    //static_assert(examp > 0, "examp not compile time");
    //printBitBoard(examp);
    //constexpr u64 rookAttacks = black_compiled.getAllRookMoves(white_compiled);
    //static_assert(rookAttacks > 0, "rookAttacks not compile time based");
    //printBitBoard(getAntidiagonal(57));
    //static_assert(getAntidiagonal(0) > 0, "This is not at compile time");
    printBitBoard(white_compiled.getAllQueenMoves(black_compiled));
    static_assert(getDiagonal(0) >= 0, "test here");
    static_assert(white_compiled.getAllQueenMoves(black_compiled) >= 0, "Not compile-time");

    std::cout << '\n';
    printBitBoard(BITBOARDS::PRECOMPILED::lookup_king_moves[26]);
    static_assert(BITBOARDS::PRECOMPILED::lookup_king_moves[26] >= 0, "Not compile-time");

    std::cout << '\n';
    printBitBoard(BITBOARDS::PRECOMPILED::lookup_knight_moves[63]);
    static_assert(BITBOARDS::PRECOMPILED::lookup_knight_moves[63] >= 0, "Not compile-time");

    //std::cout << '\n';
    //printBitBoard(white_compiled.getAllPawnMoves(black_compiled));
    //static_assert(white_compiled.getAllPawnMoves(black_compiled) >= 0, "Not compile-time");

    std::cout << '\n';
    printBitBoard(BITBOARDS::PRECOMPILED::lookup_pawn_attacks[0]);
    static_assert(BITBOARDS::PRECOMPILED::lookup_pawn_attacks[0] > 0, "not compile-time");
}
#endif

#undef SCB_M
