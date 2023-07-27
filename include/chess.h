//
// Created by Jonas Langner on 17.07.23.
//

#ifndef CHESS_CHESS_H
#define CHESS_CHESS_H

#include <iostream>
#include <map>

namespace chess {
    class Move;

    enum ChessColor {
        LIGHT, DARK
    };

    [[nodiscard]] ChessColor other_chess_color(ChessColor);

    namespace piece {
        struct CoordVec {
            int x;
            int y;
            bool relative;
            bool jump{false};

            [[nodiscard]] CoordVec operator+(CoordVec const &other) const;
            [[nodiscard]] CoordVec operator-(CoordVec const &other) const;
            [[nodiscard]] CoordVec operator*(int fac) const;
            [[nodiscard]] bool same_position(CoordVec const &other) const noexcept;
            bool operator==(CoordVec const &other) const = delete;
            bool operator!=(CoordVec const &other) const = delete;
        };

        class Piece {
        private:
            bool m_moved{false};
            ChessColor const m_color;

        public:
            Piece();
            Piece(ChessColor color);

            [[nodiscard]] virtual ChessColor color() const;
            void move();
            [[nodiscard]] bool moved() const;
            [[nodiscard]] virtual std::vector<CoordVec> move_coords() const;
            [[nodiscard]] virtual std::vector<CoordVec> attack_coords() const;
            [[nodiscard]] virtual std::string display() const;
            [[nodiscard]] virtual bool empty() const;
        };

        class ChessPiece: public Piece {
        protected:
            [[nodiscard]] std::string calc_display(std::string const &, std::string const &) const;
        public:
            ChessPiece(ChessColor color);
            [[nodiscard]] bool empty() const override;
            [[nodiscard]] virtual std::vector<CoordVec> attack_coords() const override;
        };

        class Pawn: public ChessPiece {
        public:
            Pawn(ChessColor color);
            [[nodiscard]] std::vector<CoordVec> move_coords() const override;
            [[nodiscard]] std::vector<CoordVec> attack_coords() const override;
            [[nodiscard]] std::string display() const override;
        };

        class Rook: public ChessPiece {
        public:
            Rook(ChessColor color);
            [[nodiscard]] std::vector<CoordVec> move_coords() const override;
            [[nodiscard]] std::string display() const override;
        };

        class Bishop: public ChessPiece {
        public:
            Bishop(ChessColor color);
            [[nodiscard]] std::vector<CoordVec> move_coords() const override;
            [[nodiscard]] std::string display() const override;
        };

        class Knight: public ChessPiece {
        public:
            Knight(ChessColor color);
            [[nodiscard]] std::vector<CoordVec> move_coords() const override;
            [[nodiscard]] std::string display() const override;
        };

        class King: public ChessPiece {
        public:
            King(ChessColor color);
            [[nodiscard]] std::vector<CoordVec> move_coords() const override;
            [[nodiscard]] std::string display() const override;
        };

        class Queen: public ChessPiece {
        public:
            Queen(ChessColor color);
            [[nodiscard]] std::vector<CoordVec> move_coords() const override;
            [[nodiscard]] std::string display() const override;
        };
    }

    class Board{
    private:
        std::map<const int, std::unique_ptr<chess::piece::Piece>> m_piece;

        [[nodiscard]] static int calculate_index(int x, int y);
        [[nodiscard]] static int calculate_index(char x, int y);
        [[nodiscard]] static int calculate_index(std::string const &name);
        [[nodiscard]] static piece::CoordVec calculate_position(int index);

        friend void setup_board(Board &);
        friend void print(Board &);
        friend bool move_is_legal(chess::Move const &, Board*);
        friend bool in_check(Board const &, ChessColor);
        friend Move build_move_from_input(std::string const &, std::string const &, ChessColor, Board const &);
        friend std::vector<int> calculate_possible_end_indices(piece::CoordVec, std::vector<piece::CoordVec> const &, ChessColor, Board const &);
    public:
        Board() = default;
        Board(Board const &);
        Board(Board &&) noexcept ;
        Board& operator=(Board const &);
        Board& operator=(Board &&) noexcept;
        ~Board() = default;

        [[nodiscard]] piece::Piece* piece_ptr(int index) const;
        [[nodiscard]] piece::Piece* piece_ptr(int x, int y) const;
        [[nodiscard]] piece::Piece* piece_ptr(char x, int y) const;
        [[nodiscard]] piece::Piece* piece_ptr(std::string const &name) const;
        [[nodiscard]] static std::size_t size();

        bool operator==(Board const &other) const;
        bool operator!=(Board const &other) const;
    };

    struct AtomicMove {
        int const start_index;
        int const end_index;
        ChessColor const by;
        Board const &board;
    };

    class Move {
    private:
        std::vector<AtomicMove> const m_atomics;
    public:
        Move(AtomicMove atomic);
        explicit Move(std::vector<AtomicMove> const &atomics);

        [[nodiscard]] std::vector<AtomicMove>::const_iterator begin() const;
        [[nodiscard]] std::vector<AtomicMove>::const_iterator end() const;
        [[nodiscard]] AtomicMove operator[](int) const;
        [[nodiscard]] bool empty() const;
    };

    [[nodiscard]] bool move_is_legal(Move const &, Board* = nullptr);
    [[nodiscard]] bool in_check(Board const &, ChessColor);
    [[nodiscard]] std::vector<int> calculate_possible_end_indices(piece::CoordVec, std::vector<piece::CoordVec> const &, ChessColor, Board const &);
    [[nodiscard]] bool has_legal_moves(piece::Piece*, int, Board const &);
    [[nodiscard]] Move build_move_from_input(std::string const &, std::string const &, ChessColor, Board const &);
    void setup_board(Board &);
    void print(Board &);

}








#endif //CHESS_CHESS_H
