#ifndef TERMINALCHESS_TYPES_MOVES_H
#define TERMINALCHESS_TYPES_MOVES_H

struct MoveResult {
    const int legal;
    const int r; //row and col of piece to move
    const int c;
};
struct MoveConversion {
    int r;
    int c;
};
enum class Move {
    LEGAL = true, ILLEGAL = false, 
    NOT_SPECIFIC = 2, NO_ENEMY,
    NONE = -1,
};

#endif //TERMINALCHESS_TYPES_MOVES_H
