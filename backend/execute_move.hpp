#pragma once
#include <cctype>
#include <cmath>
#include <iostream>

void execute_move(int from_x, int from_y, int to_x, int to_y) {
    char original_piece = board[from_x][from_y]; // Capture the piece BEFORE it promotes
    char to_move = original_piece;
    char target_square_piece = board[to_x][to_y];
    
    // 1. En Passant Capture
    if ((to_move == 'P' || to_move == 'p') && from_y != to_y && target_square_piece == 0) {
        board[from_x][to_y] = 0; // Destroy the captured pawn
    }

    // 2. Set En Passant Target Squares for the NEXT turn
    if (to_move == 'P' && from_x == 6 && to_x == 4) { ep_x = 5; ep_y = from_y; }
    else if (to_move == 'p' && from_x == 1 && to_x == 3) { ep_x = 2; ep_y = from_y; }
    else { ep_x = -1; ep_y = -1; }

    // 3. CASTLING MOVEMENT (The restored Rook logic!)
    if ((to_move == 'K' || to_move == 'k') && std::abs(from_y - to_y) == 2) {
        if (to_y > from_y) { 
            // Kingside Castle
            board[from_x][5] = board[from_x][7]; // Move Rook to f-file
            board[from_x][7] = 0;                // Clear original Rook square
        } else { 
            // Queenside Castle
            board[from_x][3] = board[from_x][0]; // Move Rook to d-file
            board[from_x][0] = 0;                // Clear original Rook square
        }
    }

    // 4. Update Castling Rights Flags
    if (to_move == 'K') white_king_moved = true;
    if (to_move == 'k') black_king_moved = true;
    if (original_piece == 'R' && from_x == 7 && from_y == 0) white_rooka_moved = true;
    if (original_piece == 'R' && from_x == 7 && from_y == 7) white_rookh_moved = true;
    if (original_piece == 'r' && from_x == 0 && from_y == 0) black_rooka_moved = true;
    if (original_piece == 'r' && from_x == 0 && from_y == 7) black_rookh_moved = true;

    // 5. PAWN PROMOTION
    if (to_move == 'P' && to_x == 0) {
        if (promotion_choice != 0) to_move = std::toupper(promotion_choice);
        else to_move = 'Q'; 
    }
    else if (to_move == 'p' && to_x == 7) {
        if (promotion_choice != 0) to_move = std::tolower(promotion_choice);
        else to_move = 'q';
    }

    // 6. Finalize Board State
    board[to_x][to_y] = to_move;
    board[from_x][from_y] = 0; 
    
    // 7. Safe 50-Move Rule Check
    // Resets if a pawn was moved (using the original_piece tracker) or a capture occurred
    if(target_square_piece != 0 || original_piece == 'p' || original_piece == 'P') {
        fifty_check = 0;
    } else {
        fifty_check++;
    }
    
    moves_till_now++;
}