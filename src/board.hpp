#include <array>
#include <string>

#define set_bit(x) (1ULL << x)

typedef unsigned long long bitboard;

// Pieces enumeration refers to normal FEN convention.
// White pieces are upper case, black pieces lower case
enum pieces {
    P, N, B, R, Q, K, ALL,
    p, n, b, r, q, k, all
};

class Board {
    public:
        void show();
        void from_fen(std::string fen);

    private:
        std::array<bitboard, 14> pieces;
        short to_move; // 0 if white, 1 if black
        short castling = 0; // bit 0 is K, bit 1 is Q, bit 2 is k, bit 3 is q
        // square of en_passant, or 0 if unavailable. This doesn't affect 
        // square indexing because en passant can never happen on the a1 square
        // and allows en_passant to be used like a boolean
        int en_passant = 0; 
};
