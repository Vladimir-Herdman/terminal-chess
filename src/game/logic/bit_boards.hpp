#ifndef TERMINALCHESS_GAME_LOGIC_BIT_BOARDS_H
#define TERMINALCHESS_GAME_LOGIC_BIT_BOARDS_H

#include <array>
#include <cstdint>

#include "types/moves.hpp"
#include "types/classes.hpp"

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

        const std::array<U64MemberVarPtr<Side>, 17> lookup_piece_boards = []() {
            std::array<U64MemberVarPtr<Side>, 17> arr = {};
            arr.fill(nullptr);
            arr[('B'-'B')] = &Side::bishops;
            arr[('K'-'B')] = &Side::king;
            arr[('N'-'B')] = &Side::knights;
            arr[('Q'-'B')] = &Side::queens;
            arr[('R'-'B')] = &Side::rooks;
            return arr;
        }();
        const std::array<U64MemberFuncIntPtrConst<BITBOARDS::Side>, 17> lookup_move_function = []() {
            std::array<U64MemberFuncIntPtrConst<BITBOARDS::Side>, 17> arr = {};
            arr.fill(nullptr);
            arr[('B'-'B')] = &Side::getBishopMove;
            //arr[('K'-'B')] = &Side::getAllKingMoves;
            //arr[('N'-'B')] = &Side::getAllKnightMoves;
            arr[('Q'-'B')] = &Side::getQueenMove;
            arr[('R'-'B')] = &Side::getRookMove;
            return arr;
        }();
        const std::array<U64MemberFuncPtrConst<BITBOARDS::Side>, 17> lookup_all_move_functions = []() {
            std::array<U64MemberFuncPtrConst<BITBOARDS::Side>, 17> arr = {};
            arr.fill(nullptr);
            arr[('B'-'B')] = &Side::getAllBishopMoves;
            //arr[('K'-'B')] = &Side::getAllKingMoves;
            //arr[('N'-'B')] = &Side::getAllKnightMoves;
            arr[('Q'-'B')] = &Side::getAllQueenMoves;
            arr[('R'-'B')] = &Side::getAllRookMoves;
            return arr;
        }();

        uint64_t getAllPieces() const;

        uint64_t getSlidingMoves(const int piece_index, const uint64_t mask) const;

        uint64_t getQueenMove(const int piece_index) const;
        uint64_t getRookMove(const int piece_index) const;
        uint64_t getBishopMove(const int piece_index) const;

        uint64_t getAllPawnMoves() const;
        uint64_t getAllBishopMoves() const;
        uint64_t getAllRookMoves() const;
        uint64_t getAllQueenMoves() const;

        MoveResult makePawnMove(const int r, const int c);

        MoveResult makePieceAttack(const int piece, const int r, const int c);
    };

    //TODO:
        // Maybe the lookup tables in here for reference later? As they're needed?

    extern Side white;
    extern Side black;
}

#ifdef DEV_HELPERS_LATER_REMOVE //REMOVE
void bitboardDevFunc();
#endif

#endif //TERMINALCHESS_GAME_LOGIC_BIT_BOARDS_H
