#ifndef TERMINALCHESS_TYPES_MOVES_H
#define TERMINALCHESS_TYPES_MOVES_H

struct MoveResult {
    const bool legal;
    const int r; //row and col of piece to move
    const int c;
};
enum class Move {
    LEGAL = true, ILLEGAL = false, 
    NONE = -1,
};

#endif //TERMINALCHESS_TYPES_MOVES_H
