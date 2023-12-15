#include <array>
#include <cctype>
#include <cassert>
#include <iostream>
#include <string>

#define set_bit(x) (1ULL << x)

typedef unsigned long long bitboard;

// Pieces enumeration refers to normal FEN convention.
// White pieces are upper case, black pieces lower case
enum pieces {
    P, N, B, R, Q, K, ALL,
    p, n, b, r, q, k, all
};

void show_bitboard(bitboard bb);

class Board {
    public:
        void show();
        void from_fen(std::string fen);
        void init_board_masks();

        void init_pawn_bitboards();
        void init_knight_bitboards();
        void init_king_bitboards();

    private:
        std::array<bitboard, 14> pieces;
        short to_move; // 0 if white, 1 if black
        short castling = 0; // bit 0 is K, bit 1 is Q, bit 2 is k, bit 3 is q
        // square of en_passant, or 0 if unavailable. This doesn't affect 
        // square indexing because en passant can never happen on the a1 square
        // and allows en_passant to be used like a boolean
        int en_passant = 0; 

        std::array<bitboard, 64> white_pawn_moves;
        std::array<bitboard, 64> black_pawn_moves;
        std::array<bitboard, 64> knight_moves;
        std::array<bitboard, 64> king_moves;

        std::array<bitboard, 8> files;
        std::array<bitboard, 8> ranks;



};
