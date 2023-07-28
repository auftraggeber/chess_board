//
// Created by Jonas Langner on 28.07.23.
//
#include "../include/ui.h"
#include "../include/chess.h"

#include <iostream>

#define ENTER_MOVE "Please enter your move (from,to)"


chess::Move chess::ui::ask_for_move(chess::ChessColor const by, Board const &board) {
    constexpr auto char_is_valid_for_field_name = [](char c) {
        switch (c) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
                return true;
        }

        return false;
    };

    std::cout << ENTER_MOVE << std::endl;

    std::string input;
    std::getline(std::cin, input);

    std::array<std::string, 2> field_names;
    int current_field_name_index{0};

    for (int i = 0; i < input.size(); ++i) {
        char const current_char = input[i];
        char c{static_cast<char>(tolower(current_char))};

        if (!char_is_valid_for_field_name(c)) {
            continue;
        }

        std::string current{field_names[current_field_name_index]};
        current += c;
        field_names[current_field_name_index] = current;

        if (current.size() == 2) {
            ++current_field_name_index;
        }

        if (current_field_name_index >= 2) {
            break;
        }
    }

    std::for_each(field_names.begin(), field_names.end(), [](std::string const &field_name){
        if (field_name.size() != 2) {
            throw std::invalid_argument("All field names must exist!");
        }
    });

    return chess::build_move_from_input(field_names[0], field_names[1], by, board);
}