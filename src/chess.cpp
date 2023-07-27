//
// Created by Jonas Langner on 17.07.23.
//
#include "../include/chess.h"

#include <utility>
#include "../include/log.h"
#include "../include/math_utils.h"
#include "../include/common.h"

chess::ChessColor chess::other_chess_color(chess::ChessColor const color) {
    if (color == LIGHT){
        return DARK;
    }
    return LIGHT;
}

chess::piece::CoordVec chess::piece::CoordVec::operator+(const chess::piece::CoordVec &other) const {
    return {x + other.x, y + other.y, relative, jump};
}
chess::piece::CoordVec chess::piece::CoordVec::operator-(const chess::piece::CoordVec &other) const {
    return {x - other.x, y - other.y, relative, jump};
}
chess::piece::CoordVec chess::piece::CoordVec::operator*(const int fac) const {
    return {x * fac, y * fac, relative, jump};
}
bool chess::piece::CoordVec::same_position(const chess::piece::CoordVec &other) const noexcept {
    return x == other.x && y == other.y;
}

chess::piece::Piece::Piece(): Piece(DARK) {}
chess::piece::Piece::Piece(chess::ChessColor color): m_color{color} {}
chess::ChessColor chess::piece::Piece::color() const { return m_color; }
void chess::piece::Piece::move() { m_moved = !empty(); }
bool chess::piece::Piece::moved() const { return m_moved; }
std::vector<chess::piece::CoordVec> chess::piece::Piece::move_coords() const { throw std::exception{}; }
std::vector<chess::piece::CoordVec> chess::piece::Piece::attack_coords() const { throw std::exception{}; }
std::string chess::piece::Piece::display() const { return {}; }
bool chess::piece::Piece::empty() const { return true; }

chess::piece::ChessPiece::ChessPiece(chess::ChessColor const color): Piece{color} {}
std::string chess::piece::ChessPiece::calc_display(const std::string &dark, const std::string &light) const {
    return color() == DARK ? dark : light;
}
bool chess::piece::ChessPiece::empty() const { return false; }
std::vector<chess::piece::CoordVec> chess::piece::ChessPiece::attack_coords() const { return move_coords(); }

chess::piece::Pawn::Pawn(chess::ChessColor const color): ChessPiece{color} {}
std::vector<chess::piece::CoordVec> chess::piece::Pawn::move_coords() const {
    std::vector<chess::piece::CoordVec> coords = {{0,1, false}};

    if (!moved()) {
        coords.push_back({0,2, false});
    }

    return coords;
}
std::vector<chess::piece::CoordVec> chess::piece::Pawn::attack_coords() const { return {{1,1, false}, {-1,1, false}}; }
std::string chess::piece::Pawn::display() const {
    return calc_display("♙", "♟");
}

chess::piece::Rook::Rook(chess::ChessColor const color): ChessPiece{color} {}
std::vector<chess::piece::CoordVec> chess::piece::Rook::move_coords() const {
    return {{1,0, true}, {-1,0, true}, {0, 1, true}, {0, -1, true}};
}
std::string chess::piece::Rook::display() const {
    return calc_display("♖", "♜");
}

chess::piece::Bishop::Bishop(chess::ChessColor const color): ChessPiece{color} {}
std::vector<chess::piece::CoordVec> chess::piece::Bishop::move_coords() const {
    return {{1,1, true}, {-1, 1, true}, {1, -1, true}, {-1, -1, true}};
}
std::string chess::piece::Bishop::display() const {
    return calc_display("♗", "♝");
}

chess::piece::Knight::Knight(chess::ChessColor const color): ChessPiece{color} {}
std::vector<chess::piece::CoordVec> chess::piece::Knight::move_coords() const {
    return {{-1, 2, false}, {1,2, false}, {-1, -2, false}, {1, -2, false},
            {-2, 1, false}, {2, 1, false}, {-2, -1, false}, {2, -1, false}};
}
std::string chess::piece::Knight::display() const {
    return calc_display("♘", "♞");
}

chess::piece::King::King(chess::ChessColor const color): ChessPiece{color} {}
std::vector<chess::piece::CoordVec> chess::piece::King::move_coords() const {
    return {{-1,0, false},{-1, 1, false},{0, 1, false},
            {1, 1, false},{1, 0, false},{1, -1, false},
            {0, -1, false},{-1, -1, false}};
}
std::string chess::piece::King::display() const {
    return calc_display("♔", "♚");
}

chess::piece::Queen::Queen(chess::ChessColor const color): ChessPiece{color} {}
std::vector<chess::piece::CoordVec> chess::piece::Queen::move_coords() const {
    return {{-1,0, true},{-1, 1, true},{0, 1, true},
            {1, 1, true},{1, 0, true},{1, -1, true},
            {0, -1, true},{-1, -1, true}};
}
std::string chess::piece::Queen::display() const {
    return calc_display("♕", "♛");
}

chess::Board::Board(Board const &other) {
    std::cout << "Board was copied!" << std::endl;
    std::for_each(other.m_piece.begin(), other.m_piece.end(), [this](auto const &pair) {
        int const index{pair.first};
        chess::piece::Piece* piece_ptr{pair.second.get()};

        void* new_piece_ptr = malloc(sizeof(*piece_ptr));
        std::memcpy(new_piece_ptr, (void*)piece_ptr, sizeof(*piece_ptr));

        std::unique_ptr<chess::piece::Piece> unique_ptr{static_cast<chess::piece::Piece*>(new_piece_ptr)};

        m_piece.insert({index, std::move(unique_ptr)});
    });
}
chess::Board::Board(Board &&other) noexcept:m_piece{std::move(other.m_piece)} {
    std::cout << "Board was moved!" << std::endl;
    other.m_piece.clear();
}
chess::Board& chess::Board::operator=(const chess::Board &other) {
    std::cout << "Board was copied!" << std::endl;

    if (&other == this) {
        return *this;
    }

    m_piece.clear();

    std::for_each(other.m_piece.begin(), other.m_piece.end(), [this](auto const &pair) {
        int const index{pair.first};
        chess::piece::Piece* piece_ptr{pair.second.get()};

        void* new_piece_ptr = malloc(sizeof(*piece_ptr));
        std::memcpy(new_piece_ptr, (void*)piece_ptr, sizeof(*piece_ptr));

        std::unique_ptr<chess::piece::Piece> unique_ptr{static_cast<chess::piece::Piece*>(new_piece_ptr)};

        m_piece.insert({index, std::move(unique_ptr)});
    });

    return *this;
}
chess::Board& chess::Board::operator=(chess::Board &&other) noexcept {
    std::cout << "Board was moved!" << std::endl;

    if (this != &other) {
        m_piece.clear();

        m_piece = std::move(other.m_piece);
    }
    return *this;
}
int chess::Board::calculate_index(int x, int y) {
    int const index = (y-1) * 8 + (x-1);
    return index;
}
int chess::Board::calculate_index(char x, int y) {
    auto const i_x = [&x] {
        switch (tolower(x)) {
            case 'a': return 1;
            case 'b': return 2;
            case 'c': return 3;
            case 'd': return 4;
            case 'e': return 5;
            case 'f': return 6;
            case 'g': return 7;
            case 'h': return 8;
            default:
                throw std::invalid_argument{"Illegal char."};
        }
    };
    return chess::Board::calculate_index(i_x(), y);
}
int chess::Board::calculate_index(std::string const &name) {
    if (name.length() == 2) {
        char const *c = name.c_str();

        char x = *c;
        char y_c = *(c+1);

        int y = std::stoi(std::string{y_c});

        return chess::Board::calculate_index(x, y);
    }

    throw std::invalid_argument{"Name is no field"};
}
bool chess::Board::operator==(const chess::Board &other) const {
    return m_piece == other.m_piece;
}
bool chess::Board::operator!=(const chess::Board &other) const {
    return !operator==(other);
}
chess::piece::CoordVec chess::Board::calculate_position(int index) {
    int const x = index % 8 + 1;
    int const y = index / 8 + 1;

    return {x,y,false};
}
chess::piece::Piece* chess::Board::piece_ptr(int index) const {
    return m_piece.contains(index) ? m_piece.at(index).get() : nullptr;
}
chess::piece::Piece* chess::Board::piece_ptr(int x, int y) const {
    auto index = calculate_index(x,y);
    return piece_ptr(index);
}
chess::piece::Piece* chess::Board::piece_ptr(char x, int y) const {
    auto index = calculate_index(x,y);
    return piece_ptr(index);
}
chess::piece::Piece* chess::Board::piece_ptr(std::string const &name) const {
    auto index = calculate_index(name);
    return piece_ptr(index);
}

std::size_t chess::Board::size() { return 64; }


chess::Move::Move(chess::AtomicMove atomic): chess::Move::Move{std::vector{atomic}} {}
chess::Move::Move(std::vector<AtomicMove> const &atomics): m_atomics{atomics} {}
std::vector<chess::AtomicMove>::const_iterator chess::Move::begin() const {
    return m_atomics.begin();
}
std::vector<chess::AtomicMove>::const_iterator chess::Move::end() const {
    return m_atomics.end();
}
chess::AtomicMove chess::Move::operator[](int const index) const {
    return m_atomics[index];
}
bool chess::Move::empty() const {
    return m_atomics.empty();
}



bool chess::move_is_legal(const chess::Move &move, Board* override_board) {
    using log::debug;
    if (move.empty()) {
        debug("Move is empty");
        return false;
    }

    bool consistent_atomics{true};
    std::for_each(move.begin(), move.end(), [&consistent_atomics](chess::AtomicMove const &atomic_move) {
        static chess::ChessColor const by = atomic_move.by;
        static chess::Board const &board = atomic_move.board;

        if (atomic_move.by != by || atomic_move.board != board) {
            consistent_atomics = false;

            debug("Inconsistent color or board.");
            return;
        }

        auto const piece_ptr{board.piece_ptr(atomic_move.start_index)};

        if (piece_ptr == nullptr || piece_ptr->color() != by) {
            consistent_atomics = false;

            debug("No piece at start position found!");
            return;
        }



        auto const end_piece_ptr{board.piece_ptr(atomic_move.end_index)};
        auto const &moves{(end_piece_ptr != nullptr && end_piece_ptr->color() != by) ? piece_ptr->attack_coords() : piece_ptr->move_coords()};

        auto const possible_end_indices{calculate_possible_end_indices(board.calculate_position(atomic_move.start_index), moves, atomic_move.by, board)};
        bool const can_move_to_field{common::contains(possible_end_indices, atomic_move.end_index)};

        if (!can_move_to_field) {
            consistent_atomics = false;

            debug("Piece cannot move to end index.");
        }
    });

    if (!consistent_atomics) {
        return false;
    }

    auto const by{move[0].by};
    auto const &board{move[0].board};

    Board copy{move[0].board};

    std::for_each(move.begin(), move.end(), [&copy](chess::AtomicMove const &atomic) {
        auto piece_ptr = std::move(copy.m_piece[atomic.start_index]);
        copy.m_piece.erase(atomic.start_index);
        piece_ptr->move();
        copy.m_piece.insert({atomic.end_index, std::move(piece_ptr)});
    });

    if (!in_check(copy, by)) {
        if (override_board != nullptr) {
            *override_board = std::move(copy);
        }

        return true;
    }

    return false;
}
bool chess::in_check(const chess::Board &board, chess::ChessColor color) {
    bool in_check{false};
    ChessColor other_color{other_chess_color(color)};

    std::for_each(board.m_piece.begin(), board.m_piece.end(), [&other_color, &in_check, &board](auto const &pair){
        chess::piece::Piece* piece_ptr{pair.second.get()};

        if (piece_ptr->color() == other_color) {
            std::vector<int> attacks{chess::calculate_possible_end_indices(Board::calculate_position(pair.first),
                                                                           piece_ptr->attack_coords(), other_color, board)};

            if (!attacks.empty()) {
                std::for_each(attacks.begin(), attacks.end(), [&board, &in_check, &pair](int const index) {
                    auto start_v{Board::calculate_position(pair.first)};

                    auto* piece{board.piece_ptr(index)};

                    if (piece != nullptr) {
                        auto* king{dynamic_cast<piece::King*>(piece)};

                        if (king != nullptr) {
                            in_check = true;
                            log::debug("King would be in check! by: " + std::to_string(start_v.x) + "," + std::to_string(start_v.y));
                            return;
                        }
                    }
                });
            }
        }
    });

    return in_check;
}

std::vector<int> chess::calculate_possible_end_indices(piece::CoordVec start, const std::vector<piece::CoordVec> &moves, ChessColor color, Board const &board) {
    std::vector<int> end_indices;

    if (moves.empty()) {
        return end_indices;
    }

    std::for_each(moves.begin(), moves.end(), [&end_indices, &start, &color, &board](chess::piece::CoordVec vect) {
        if (color == DARK) {
            vect = vect * -1;
        }

        if (vect.relative) {
            int i{1};

            while (true) {
                auto const end_pos{start + (vect * i)};

                if (end_pos.x > 8 || end_pos.y > 8 || end_pos.x < 1 || end_pos.y < 1) {
                    break;
                }

                auto end_i{Board::calculate_index(end_pos.x, end_pos.y)};
                auto* piece_ptr{board.piece_ptr(end_i)};

                if (piece_ptr != nullptr) {
                    if (piece_ptr->color() != color) {
                        end_indices.push_back(end_i);
                    }

                    if (!vect.jump) {
                        break;
                    }
                }

                end_indices.push_back(end_i);
                ++i;
            }
        }
        else {
            auto const end_pos{start + vect};

            if (end_pos.x >= 1 && end_pos.x <= 8 && end_pos.y >= 1 && end_pos.y <= 8) {

                auto step_x{end_pos.x - start.x};
                auto step_y{end_pos.y - start.y};

                auto const gcd{math_utils::gcd(step_x, step_y)};
                step_x = step_x / gcd;
                step_y = step_y / gcd;

                int step{1};
                bool legal{true};

                while (true) {
                    chess::piece::CoordVec const current_pos{start.x + (step_x * step), start.y + (step_y * step)};

                    if (current_pos.x < 1 || current_pos.x > 8 || current_pos.y < 1 || current_pos.y > 8) {
                        break;
                    }

                    if (!vect.jump || end_pos.same_position(current_pos)) {
                        int const index{board.calculate_index(current_pos.x, current_pos.y)};
                        auto* piece_ptr{board.piece_ptr(index)};

                        if (piece_ptr != nullptr && piece_ptr->color() == color) {
                            legal = false;
                            break;
                        }
                    }

                    if (current_pos.same_position(end_pos)) {
                        break;
                    }

                    ++step;
                }

                if (legal) {
                    end_indices.push_back(Board::calculate_index(end_pos.x, end_pos.y));
                }
            }
        }
    });

    return end_indices;
}

chess::Move chess::build_move_from_input(std::string const &from, std::string const &to, ChessColor by, Board const &board) {
    std::vector<chess::AtomicMove> atomics;

    if (false) { // todo: castle

    }
    else {
        int const start_index{board.calculate_index(from)};
        int const end_index(board.calculate_index(to));

        atomics.push_back({start_index, end_index, by, board});
    }

    return chess::Move{atomics};
}

void chess::setup_board(chess::Board &board) {
    for (int y = 1; y <= 8; ++y) {
        for (int x = 1; x <= 8; ++x) {
            auto const index = Board::calculate_index(x, y);

            std::unique_ptr<chess::piece::Piece> piece_ptr{nullptr};
            auto const color = (y <= 2) ? LIGHT : DARK;
            if (y == 2 || y == 7) {
                piece_ptr = std::make_unique<chess::piece::Pawn>(color);
            }
            else if (y == 1 || y == 8) {
                if (x == 1 || x == 8) {
                    piece_ptr = std::make_unique<chess::piece::Rook>(color);
                }
                else if (x == 2 || x == 7) {
                    piece_ptr = std::make_unique<chess::piece::Knight>(color);
                }
                else if (x == 3 || x == 6) {
                    piece_ptr = std::make_unique<chess::piece::Bishop>(color);
                }
                else if (x == 4) {
                    piece_ptr = std::make_unique<chess::piece::Queen>(color);
                }
                else {
                    piece_ptr = std::make_unique<chess::piece::King>(color);
                }
            }

            if (piece_ptr != nullptr) {
                board.m_piece.insert({index, std::move(piece_ptr)});
                board.m_piece.size();
            }
        }
    }
}

void chess::print(chess::Board &board) {

    std::cout << "Address of board: " << &board << std::endl;

    int last_y{0};

    for (int index = 0; index < board.size(); ++index) {
        chess::piece::CoordVec vector{board.calculate_position(index)};
        if (vector.y != last_y) {
            last_y = vector.y;
            std::cout << std::endl << vector.y << "\t";
        }

        auto piece = board.piece_ptr(vector.x, vector.y);
        std::string name = (piece == nullptr || piece->empty()) ? "·" : piece->display();
        std::cout << name << ' ';
    }

    std::cout << std::endl;

    for (int i = 0; i < 8; ++i) {
        std::cout << "----";
    }
    std::cout << std::endl;
}

