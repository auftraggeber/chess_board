//
// Created by Jonas Langner on 17.07.23.
//

#ifndef CHESS_UI_H
#define CHESS_UI_H

#include "chess.h"

namespace chess::ui {
    chess::Move ask_for_move(ChessColor, Board const &);
}

#endif //CHESS_UI_H
