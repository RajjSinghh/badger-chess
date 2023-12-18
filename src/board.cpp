#include "board.hpp"

using std::cout; 

Board::Board() {
    from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    show();

    init_board_masks();
    init_pawn_bitboards();
    init_knight_bitboards();
    init_king_bitboards();
}

Board::Board(std::string fen) {
    from_fen(fen);
    show();

    init_board_masks();
    init_pawn_bitboards();
    init_knight_bitboards();
    init_king_bitboards();
}

void Board::show() {
    cout << '\n';
    int square;
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            square = 8*rank + file; 

            if (pieces[P] & set_bit(square)) cout << " P";
            else if (pieces[N] & set_bit(square)) cout << " N";
            else if (pieces[B] & set_bit(square)) cout << " B";
            else if (pieces[R] & set_bit(square)) cout << " R";
            else if (pieces[Q] & set_bit(square)) cout << " Q";
            else if (pieces[K] & set_bit(square)) cout << " K";

            else if (pieces[p] & set_bit(square)) cout << " p";
            else if (pieces[n] & set_bit(square)) cout << " n";
            else if (pieces[b] & set_bit(square)) cout << " b";
            else if (pieces[r] & set_bit(square)) cout << " r";
            else if (pieces[q] & set_bit(square)) cout << " q";
            else if (pieces[k] & set_bit(square)) cout << " k";

            else cout << " .";
        }
        cout << '\n';
    }
    cout << '\n';
    cout << ((to_move == 0) ? "White's move" : "Black's move") << '\n';

    cout << "Castling: ";
    if (castling & 1)
        cout << 'K';
    else 
        cout << '-';
    if (castling & 2)
        cout << 'Q';
    else 
        cout << '-';
    if (castling & 4)
        cout << 'k';
    else 
        cout << '-';
    if (castling & 8)
        cout << 'q';
    else 
        cout << '-';
    cout << '\n';

    cout << "En passant: " << en_passant;
    cout << '\n';
}

void Board::from_fen(std::string fen) {
    // Empty current state of board
    for (int i = P; i < all; i++) 
        pieces[i] = 0ULL;

    int mode = 0;
    int board_idx = 56;

    for (char c : fen) {
        if (c == ' ')
            mode++;

        if (mode == 0) {
            // Board Layout
            switch (c) {
                case 'P':
                    pieces[P] ^= set_bit(board_idx);
                    pieces[ALL] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'N':
                    pieces[N] ^= set_bit(board_idx);
                    pieces[ALL] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'B':
                    pieces[B] ^= set_bit(board_idx);
                    pieces[ALL] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'R':
                    pieces[R] ^= set_bit(board_idx);
                    pieces[ALL] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'Q':
                    pieces[Q] ^= set_bit(board_idx);
                    pieces[ALL] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'K':
                    pieces[K] ^= set_bit(board_idx);
                    pieces[ALL] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'p':
                    pieces[p] ^= set_bit(board_idx);
                    pieces[all] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'n':
                    pieces[n] ^= set_bit(board_idx);
                    pieces[all] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'b':
                    pieces[b] ^= set_bit(board_idx);
                    pieces[all] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'r':
                    pieces[r] ^= set_bit(board_idx);
                    pieces[all] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'q':
                    pieces[q] ^= set_bit(board_idx);
                    pieces[all] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case 'k':
                    pieces[k] ^= set_bit(board_idx);
                    pieces[all] ^= set_bit(board_idx);
                    board_idx++;
                    break;
                case '/':
                    board_idx -= 16;
                    break;
            }

            if (isdigit(c))
                board_idx += c - '0';
        }

        else if (mode == 1) {
            // Player to move
            if (c == 'w')
                to_move = 0;
            else if (c == 'b')
                to_move = 1;
        }

        else if (mode == 2) {
            // Castling rights
            switch (c) {
                case 'K':
                    castling ^= 1;
                    break;
                case 'Q':
                    castling ^= 2;
                    break;
                case 'k':
                    castling ^= 4;
                    break;
                case 'q':
                    castling ^= 8;
                    break;
            }
        }

        else if (mode == 3) {
            if (c == '-')
                en_passant = 0;
            else if (isalpha(c)) {
                cout << "file: " << c - 'a' << ' ';
                en_passant += c - 'a';
            }
            else if (isdigit(c)) {
                cout << "rank: " << c - '0' - 1;
                en_passant += 8*(c - '0' - 1);
            }
        }
        
        // TODO: implement half and full move counters if necessary

    }
}

void show_bitboard(bitboard bb) {
    // Utility function for displaying bitboards in a more human readable way
    cout << '\n';
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            if (bb & set_bit((8*rank + file))) 
                cout << " 1";
            else 
                cout << " 0";
        }
        cout << '\n';
    }
    cout << '\n';
}

void Board::init_board_masks() {
    ranks[0] = 0x00000000000000FF;
    for (int i = 1; i < 8; i++) 
        ranks[i] = ranks[i-1] << 8;
    

    files[0] = 0x0101010101010101;
    for (int i = 1; i < 8; i++) 
        files[i] = files[i-1] << 1;

}

void Board::init_pawn_bitboards() {
    bitboard pawn;
    bitboard targets;
    for (int i = 0; i < 64; i++) {
        targets = 0ULL;
        pawn = set_bit(i);

        targets ^= pawn << 8;
        targets ^= (pawn & ranks[1]) << 16;
        targets ^= (pawn & ~files[7]) << 9;
        targets ^= (pawn & ~files[0]) << 7;
        white_pawn_moves[i] = targets;

        targets = 0ULL;
        targets ^= pawn >> 8;
        targets ^= (pawn & ranks[6]) >> 16;
        targets ^= (pawn & ~files[0]) >> 9;
        targets ^= (pawn & ~files[7]) >> 7;
        black_pawn_moves[i] = targets;
    }
}

void Board::init_knight_bitboards() {
    bitboard knight;
    bitboard targets;
    for (int i = 0; i < 64; i++) {
        targets = 0ULL;
        knight = set_bit(i);
        
        targets ^= (knight & ~(files[0] | files[1] | ranks[7])) << 6;
        targets ^= (knight & ~(files[6] | files[7] | ranks[7])) << 10;
        targets ^= (knight & ~(files[0] | ranks[6] | ranks[7])) << 15;
        targets ^= (knight & ~(files[7] | ranks[6] | ranks[7])) << 17;

        targets ^= (knight & ~(files[6] | files[7] | ranks[0])) >> 6;
        targets ^= (knight & ~(files[0] | files[1] | ranks[0])) >> 10;
        targets ^= (knight & ~(files[7] | ranks[0] | ranks[1])) >> 15;
        targets ^= (knight & ~(files[0] | ranks[0] | ranks[1])) >> 17;

        knight_moves[i] = targets;
    }
}

void Board::init_king_bitboards() {
    bitboard king;
    bitboard targets;

    for (int i = 0; i < 64; i++) {
        targets = 0ULL;
        king = set_bit(i);

        targets ^= (king & ~files[7]) << 1;
        targets ^= (king & ~files[0]) >> 1;
        targets ^= (king & ~ranks[7]) << 8;
        targets ^= (king & ~ranks[0]) >> 8;

        targets ^= (king & ~(ranks[7] | files[7])) << 9;
        targets ^= (king & ~(ranks[7] | files[0])) << 7;
        targets ^= (king & ~(ranks[0] | files[0])) >> 9;
        targets ^= (king & ~(ranks[0] | files[7])) >> 7;

        king_moves[i] = targets;
    }
}

int main() {
    Board b("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    return 0;
}
