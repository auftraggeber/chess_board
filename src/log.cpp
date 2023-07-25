//
// Created by Jonas Langner on 22.07.23.
//

#include "../include/log.h"

void log::debug(std::string const &string) {
    if constexpr (DEBUG) {
        std::cout << "[DEBUG] " << string << std::endl;
    }
}