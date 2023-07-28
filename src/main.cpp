#include <iostream>

#include "../include/chess.h"
#include "../include/ui.h"

int main() {
    using namespace chess;
    using namespace chess::ui;

    Board board;
    setup_board(board);
    ChessColor current{LIGHT};

    while (true) {
        print(board);
        Move move{ask_for_move(current, board)};
        if (move_is_legal(move, &board)) {
            current = other_chess_color(current);
        }
    }

    return 0;
}
