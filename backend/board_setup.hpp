#pragma once
#include <vector>

using namespace std;

// Initialize the board directly!
vector<vector<char>> board = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

vector<vector<bool>> possible_moves(8, vector<bool>(8, false));
int moves_till_now = 0;
bool under_check = false;
char promotion_choice = 0;

int wkx = 7, wky = 4;
int bkx = 0, bky = 4;

bool white_king_moved = false;
bool black_king_moved = false;
int fifty_check = 0;

bool white_rooka_moved = false;
bool white_rookh_moved = false;
bool black_rooka_moved = false;
bool black_rookh_moved = false;

int ep_x = -1;
int ep_y = -1;

// ADD THIS TO THE BOTTOM OF board.hpp
inline void reset_board() {
    board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    moves_till_now = 0;
    under_check = false;
    promotion_choice = 0;
    wkx = 7; wky = 4;
    bkx = 0; bky = 4;
    white_king_moved = false;
    black_king_moved = false;
    fifty_check = 0;
    white_rooka_moved = false;
    white_rookh_moved = false;
    black_rooka_moved = false;
    black_rookh_moved = false;
    ep_x = -1;
    ep_y = -1;
}