#pragma once

vector<vector<bool>> get_legal_moves(int from_x, int from_y) 
{
    vector<vector<bool>> legal_moves(8, vector<bool>(8, false));
    vector<vector<bool>> possible_moves(8, vector<bool>(8, false));

    if (moves_possible(from_x, from_y, possible_moves) != 0) return legal_moves;

    char to_move = board[from_x][from_y];
    bool is_white_moving = isupper(to_move);
    char king = is_white_moving ? 'K' : 'k';

    for (int to_x = 0; to_x < 8; to_x++)
    {
        for (int to_y = 0; to_y < 8; to_y++)
        {
            if (possible_moves[to_x][to_y] == true)
            {
                if ((to_move == 'K' || to_move == 'k') && abs(from_y - to_y) == 2)
                {
                    int pass_y = (from_y + to_y) / 2;
                    if (is_square_attacked(from_x, from_y, !is_white_moving) || is_square_attacked(from_x, pass_y, !is_white_moving)) continue;
                }

                char captured_piece = board[to_x][to_y]; 
                board[to_x][to_y] = to_move;
                board[from_x][from_y] = 0;
                
                int king_x = -1, king_y = -1;
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (board[i][j] == king)
                        {
                            king_x = i;
                            king_y = j;
                            break;
                        }
                    }
                    if (king_x != -1) break;
                }
                
                if (!is_square_attacked(king_x, king_y, !is_white_moving))
                {
                    legal_moves[to_x][to_y] = true;
                }
                
                board[from_x][from_y] = to_move;
                board[to_x][to_y] = captured_piece;
            }
        }
    }
    
    return legal_moves;
}