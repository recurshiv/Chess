#pragma once
bool is_square_attacked(int x, int y, bool attacked_by_white) 
{
    char en = attacked_by_white ? 'N' : 'n';
    char er = attacked_by_white ? 'R' : 'r';
    char eb = attacked_by_white ? 'B' : 'b';
    char eq = attacked_by_white ? 'Q' : 'q';
    char ep = attacked_by_white ? 'P' : 'p';
    char ek = attacked_by_white ? 'K' : 'k';

    if(is_square_valid(x + 2, y + 1) && board[x + 2][y + 1] == en) return true;
    if(is_square_valid(x + 2, y - 1) && board[x + 2][y - 1] == en) return true;
    if(is_square_valid(x - 2, y + 1) && board[x - 2][y + 1] == en) return true;
    if(is_square_valid(x - 2, y - 1) && board[x - 2][y - 1] == en) return true;
    if(is_square_valid(x + 1, y + 2) && board[x + 1][y + 2] == en) return true;
    if(is_square_valid(x + 1, y - 2) && board[x + 1][y - 2] == en) return true;
    if(is_square_valid(x - 1, y + 2) && board[x - 1][y + 2] == en) return true;
    if(is_square_valid(x - 1, y - 2) && board[x - 1][y - 2] == en) return true;

    
    if(is_square_valid(x, y + 1) && board[x][y + 1] == ek) return true;
    if(is_square_valid(x, y - 1) && board[x][y - 1] == ek) return true;
    if(is_square_valid(x + 1, y + 1) && board[x + 1][y + 1] == ek) return true;
    if(is_square_valid(x + 1, y) && board[x + 1][y] == ek) return true;
    if(is_square_valid(x + 1, y - 1) && board[x + 1][y - 1] == ek) return true;
    if(is_square_valid(x - 1, y + 1) && board[x - 1][y + 1] == ek) return true;
    if(is_square_valid(x - 1, y) && board[x - 1][y] == ek) return true;
    if(is_square_valid(x - 1, y - 1) && board[x - 1][y - 1] == ek) return true;

    if(attacked_by_white) {
        if(is_square_valid(x + 1, y - 1) && board[x + 1][y - 1] == ep) return true;
        if(is_square_valid(x + 1, y + 1) && board[x + 1][y + 1] == ep) return true;
    }
    else
    {
        if(is_square_valid(x - 1, y - 1) && board[x - 1][y - 1] == ep) return true;
        if(is_square_valid(x - 1, y + 1) && board[x - 1][y + 1] == ep) return true;
    }
    for(int i = x + 1; i < 8; i++) {
        if(board[i][y] == er || board[i][y] == eq) return true;
        if(board[i][y] != 0) break; 
    }
    for(int i = x - 1; i >= 0; i--) {
        if(board[i][y] == er || board[i][y] == eq) return true;
        if(board[i][y] != 0) break;
    }
    for(int i = y - 1; i >= 0; i--) {
        if(board[x][i] == er || board[x][i] == eq) return true;
        if(board[x][i] != 0) break;
    }
    for(int i = y + 1; i < 8; i++) {
        if(board[x][i] == er || board[x][i] == eq) return true;
        if(board[x][i] != 0) break;
    }

    for(int i = 1; x - i >= 0 && y - i >= 0; i++) {
        if(board[x - i][y - i] == eb || board[x - i][y - i] == eq) return true;
        if(board[x - i][y - i] != 0) break;
    }
    for(int i = 1; x + i <= 7 && y + i >= 0; i++) {
        if(board[x + i][y + i] == eb || board[x + i][y + i] == eq) return true;
        if(board[x + i][y + i] != 0) break;
    }
    for(int i = 1; x - i >= 0 && y + i <= 7; i++) {
        if(board[x - i][y + i] == eb || board[x - i][y + i] == eq) return true;
        if(board[x - i][y + i] != 0) break;
    }
    for(int i = 1; x + i <= 7 && y - i >= 0; i++) {
        if(board[x + i][y - i] == eb || board[x + i][y - i] == eq) return true;
        if(board[x + i][y - i] != 0) break;
    }
    return false;
}