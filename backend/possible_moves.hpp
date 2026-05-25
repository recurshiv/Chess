#pragma once

bool is_square_valid(int x, int y){
    if(x <= 7 && x >= 0 && y <= 7 && y >= 0) return true;
    return false;
}

bool is_enemy(char to_move, char target)
{
    if(target == 0) return true;
    if(isupper(to_move) != isupper(target)) return true;
    return false;
}

int moves_possible(int x, int y, vector<vector<bool>> &state_arr) {
    char to_move = board[x][y];
    if((to_move == 'B' && moves_till_now%2 == 0)|| (to_move == 'b' && moves_till_now%2 == 1) || (to_move == 'Q' && moves_till_now%2 == 0)|| (to_move == 'q' && moves_till_now%2 == 1)) {
        for(int i = 1; x - i >= 0 && y - i >= 0; i++) {
            if(board[x - i][y - i] == 0) state_arr[x - i][y - i] = true;
            else if(is_enemy(to_move, board[x - i][y - i])) {state_arr[x - i][y - i] = true; break;}
            else break;
        }
        for(int i = 1; x + i <= 7 && y + i >= 0; i++) {
            if(board[x + i][y + i] == 0) state_arr[x + i][y + i] = true;
            else if(is_enemy(to_move, board[x + i][y + i])) {state_arr[x + i][y + i] = true; break;}
            else break;
        }
        for(int i = 1; x - i >= 0 && y + i <= 7; i++) {
            if(board[x - i][y + i] == 0) state_arr[x - i][y + i] = true;
            else if(is_enemy(to_move, board[x - i][y + i])) {state_arr[x - i][y + i] = true; break;}
            else break;
        }
        for(int i = 1; x + i <= 7 && y - i >= 0; i++) {
            if(board[x + i][y - i] == 0) state_arr[x + i][y - i] = true;
            else if(is_enemy(to_move, board[x + i][y - i])) {state_arr[x + i][y - i] = true; break;}
            else break;
        }
        if((to_move == 'B' && moves_till_now%2 == 0)|| (to_move == 'b' && moves_till_now%2 == 1)) return 0;
        if(to_move == 'B' || to_move == 'b') return 1;
    }
    if((to_move == 'R' && moves_till_now%2 == 0) || (to_move == 'r' && moves_till_now%2 == 1) || (to_move == 'Q' && moves_till_now%2 == 0) || (to_move == 'q' && moves_till_now%2 == 1)) {
        // CASTLING LOGIC
        // if(to_move == 'R' && (!white_rookh_moved || !white_rooka_moved))
        // {
        //     if(x == 7)
        //     {
        //         if(y == 0 && !white_rooka_moved) white_rooka_moved = true;
        //         else if(y == 0 && !white_rookh_moved) white_rookh_moved = true; 
        //     }
        // }
        // if(to_move == 'r' && (!white_rookh_moved || !white_rooka_moved))
        // {
        //     if(x == 0)
        //     {
        //         if(y == 0 && !black_rooka_moved) black_rooka_moved = true;
        //         else if(y == 0 && !white_rookh_moved) white_rookh_moved = true; 
        //     }
        // }
        for(int i = x + 1; i < 8; i++){
            if(board[i][y] == 0) state_arr[i][y] = true;
            else if(is_enemy(to_move, board[i][y])) {state_arr[i][y] = true; break;}
            else break;
        }
        for(int i = x - 1; i >= 0; i--){
            if(board[i][y] == 0) state_arr[i][y] = true;
            else if(is_enemy(to_move, board[i][y])) {state_arr[i][y] = true; break;}
            else break;
        }
        for(int i = y - 1; i >= 0; i--){
            if(board[x][i] == 0) state_arr[x][i] = true;
            else if(is_enemy(to_move, board[x][i])) {state_arr[x][i] = true; break;}
            else break;
        }
        for(int i = y + 1; i < 8; i++){
            if(board[x][i] == 0) state_arr[x][i] = true;
            else if(is_enemy(to_move, board[x][i])) {state_arr[x][i] = true; break;}
            else break;
        }
        return 0;
    }
    else if((to_move == 'N' && moves_till_now%2 == 0) || (to_move == 'n' && moves_till_now%2 == 1))
    {
        if(is_square_valid(x + 2, y + 1) && (board[x + 2][y + 1] == 0 || is_enemy(to_move, board[x + 2][y + 1]))) state_arr[x + 2][y + 1] = true;
        if(is_square_valid(x + 2, y - 1) && (board[x + 2][y - 1] == 0 || is_enemy(to_move, board[x + 2][y - 1]))) state_arr[x + 2][y - 1] = true;
        if(is_square_valid(x - 2, y + 1) && (board[x - 2][y + 1] == 0 || is_enemy(to_move, board[x - 2][y + 1]))) state_arr[x - 2][y + 1] = true;
        if(is_square_valid(x - 2, y - 1) && (board[x - 2][y - 1] == 0 || is_enemy(to_move, board[x - 2][y - 1]))) state_arr[x - 2][y - 1] = true;
        if(is_square_valid(x + 1, y + 2) && (board[x + 1][y + 2] == 0 || is_enemy(to_move, board[x + 1][y + 2]))) state_arr[x + 1][y + 2] = true;
        if(is_square_valid(x + 1, y - 2) && (board[x + 1][y - 2] == 0 || is_enemy(to_move, board[x + 1][y - 2]))) state_arr[x + 1][y - 2] = true;
        if(is_square_valid(x - 1, y + 2) && (board[x - 1][y + 2] == 0 || is_enemy(to_move, board[x - 1][y + 2]))) state_arr[x - 1][y + 2] = true;
        if(is_square_valid(x - 1, y - 2) && (board[x - 1][y - 2] == 0 || is_enemy(to_move, board[x - 1][y - 2]))) state_arr[x - 1][y - 2] = true;
        return 0;
    }
    else if((to_move == 'P' && moves_till_now%2 == 0)){
        if(x == 6) {
            if(is_square_valid(x - 1, y) && board[x - 1][y] == 0)
            {
                state_arr[x - 1][y] = true;
                if(board[x - 2][y] == 0) state_arr[x - 2][y] = true;
            }
        }
        else if(is_square_valid(x - 1, y) && board[x - 1][y] == 0) state_arr[x - 1][y] = true;
        // else if(x == 0){
        //     // PAWN PROMOTION LOGIC
        // }
        if(is_square_valid(x - 1, y - 1) && islower(board[x - 1][y - 1]))
        {
           state_arr[x - 1][y - 1] = true;                
        }
        if(is_square_valid(x - 1, y + 1) && islower(board[x - 1][y + 1]))
        {
           state_arr[x - 1][y + 1] = true;                
        }
        if(is_square_valid(x - 1, y - 1) && (x - 1) == ep_x && (y - 1) == ep_y) state_arr[x - 1][y - 1] = true;
        if(is_square_valid(x - 1, y + 1) && (x - 1) == ep_x && (y + 1) == ep_y) state_arr[x - 1][y + 1] = true;
        return 0;
    }
    else if((to_move == 'p' && moves_till_now%2 == 1)){
        if(x == 1)
        {
            if(board[x + 1][y] == 0)
            {
                state_arr[x + 1][y] = true;
                if(board[x + 2][y] == 0) state_arr[x + 2][y] = true;
            }
        }
        // else if(x == 7){
        //     // PAWN PROMOTION LOGIC
        // }
        else if(is_square_valid(x + 1, y) && board[x + 1][y] == 0) state_arr[x + 1][y] = true;
        if(is_square_valid(x + 1, y - 1) && isupper(board[x + 1][y - 1]))
        {
            state_arr[x + 1][y - 1] = true;                
        }
        if(is_square_valid(x + 1, y + 1) && isupper(board[x + 1][y + 1]))
        {
            state_arr[x + 1][y + 1] = true;                
        }
        if(is_square_valid(x + 1, y - 1) && (x + 1) == ep_x && (y - 1) == ep_y) state_arr[x + 1][y - 1] = true;
        if(is_square_valid(x + 1, y + 1) && (x + 1) == ep_x && (y + 1) == ep_y) state_arr[x + 1][y + 1] = true;
        return 0;
    }
    else if((to_move == 'K' && moves_till_now%2 == 0) || (to_move == 'k' && moves_till_now%2 == 1)) { 
        // 1. Standard 1-square moves
        if(is_square_valid(x , y + 1) && is_enemy(to_move, board[x][y + 1])) state_arr[x][y + 1] = true;
        if(is_square_valid(x , y - 1) && is_enemy(to_move, board[x][y - 1])) state_arr[x][y - 1] = true;
        if(is_square_valid(x + 1 , y + 1) && is_enemy(to_move, board[x + 1][y + 1])) state_arr[x + 1][y + 1] = true;
        if(is_square_valid(x + 1 , y) && is_enemy(to_move, board[x + 1][y])) state_arr[x + 1][y] = true;
        if(is_square_valid(x + 1 , y - 1) && is_enemy(to_move, board[x + 1][y - 1])) state_arr[x + 1][y - 1] = true;
        if(is_square_valid(x - 1, y + 1) && is_enemy(to_move, board[x - 1][y + 1])) state_arr[x - 1][y + 1] = true;
        if(is_square_valid(x - 1, y) && is_enemy(to_move, board[x - 1][y])) state_arr[x - 1][y] = true;
        if(is_square_valid(x - 1, y - 1) && is_enemy(to_move, board[x - 1][y - 1])) state_arr[x - 1][y - 1] = true;

        // 2. THE CLEAN CASTLING FIX: Just check if pieces are physically empty!
        if (to_move == 'K' && !white_king_moved) {
            // White Kingside (f1 and g1 must be empty)
            if (!white_rookh_moved && board[7][5] == 0 && board[7][6] == 0) {
                state_arr[7][6] = true;
            }
            // White Queenside (b1, c1, and d1 must be empty)
            if (!white_rooka_moved && board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0) {
                state_arr[7][2] = true;
            }
        }
        else if (to_move == 'k' && !black_king_moved) {
            // Black Kingside (f8 and g8 must be empty)
            if (!black_rookh_moved && board[0][5] == 0 && board[0][6] == 0) {
                state_arr[0][6] = true;
            }
            // Black Queenside (b8, c8, and d8 must be empty)
            if (!black_rooka_moved && board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0) {
                state_arr[0][2] = true;
            }
        }
        return 0;
    }
    else return 1;
}

