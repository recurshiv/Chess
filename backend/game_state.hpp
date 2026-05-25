#pragma once
#include <vector>

int check_game_state() {
    // 1. Fifty-Move Rule (50 full moves = 100 half-moves without capture/pawn push)
    if (fifty_check >= 100) return 4;

    // 2. Insufficient Material Check
    int white_knights = 0, white_bishops = 0;
    int black_knights = 0, black_bishops = 0;
    int major_pieces_or_pawns = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char p = board[r][c];
            if (p == 0 || p == 'K' || p == 'k') continue; // Ignore empty squares and Kings
            
            if (p == 'N') white_knights++;
            else if (p == 'n') black_knights++;
            else if (p == 'B') white_bishops++;
            else if (p == 'b') black_bishops++;
            else major_pieces_or_pawns++; // Counts Pawns, Rooks, and Queens
        }
    }

    // If there are no Queens, Rooks, or Pawns, we might have a draw
    if (major_pieces_or_pawns == 0) {
        int white_minor = white_knights + white_bishops;
        int black_minor = black_knights + black_bishops;
        
        // King vs King
        if (white_minor == 0 && black_minor == 0) return 4;
        // King + Minor Piece vs King
        if (white_minor == 1 && black_minor == 0) return 4;
        if (black_minor == 1 && white_minor == 0) return 4;
    }

    // 3. Checkmate vs Stalemate Check
    bool is_white_turn = (moves_till_now % 2 == 0);
    bool has_legal_moves = false;

    // Scan the board to see if the current player has ANY valid moves left
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char p = board[r][c];
            if (p == 0) continue;

            bool is_white_piece = (p >= 'A' && p <= 'Z');
            
            if (is_white_piece == is_white_turn) {
                std::vector<std::vector<bool>> moves = get_legal_moves(r, c);
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if (moves[i][j]) {
                            has_legal_moves = true;
                            break;
                        }
                    }
                    if (has_legal_moves) break;
                }
            }
            if (has_legal_moves) break;
        }
        if (has_legal_moves) break;
    }

    // If the player has moves, the game continues normally
    if (has_legal_moves) return 0;

    // IF NO MOVES LEFT: We must determine if it is Checkmate or Stalemate
    // We check if the global `under_check` flag was triggered during the move calculation
    if (under_check) {
        // If it's White's turn and they are in check with no moves -> Black Wins (2)
        // If it's Black's turn and they are in check with no moves -> White Wins (1)
        return is_white_turn ? 2 : 1; 
    } else {
        // If they have no moves but are NOT in check -> Stalemate (3)
        return 3; 
    }
}