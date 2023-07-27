#include <iostream>

#include "../include/chess.h"

int main() {
    using namespace chess;

    Board board1;
    setup_board(board1);

    Move move{build_move_from_input("b1", "a3", LIGHT, board1)};

    print(board1);

    if (move_is_legal(move, &board1)) {
        std::cout << "Move is legal" << std::endl;
    }
    else {
        std::cout << "Move is not legal" << std::endl;
    }

    print(board1);

    Move move2{build_move_from_input("a2","a4", LIGHT, board1)};
    move_is_legal(move2, &board1);

    print(board1);
    return 0;
}
