#include <iostream>
#include <vector>
#include <string>
#include "httplib.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#include "board_setup.hpp"
#include "possible_moves.hpp"
#include "is_square_attacked.hpp"
#include "get_legal_moves.hpp"
#include "execute_move.hpp"
#include "game_state.hpp"

int main() {
    cout << "========================================" << endl;
    cout << "INITIALIZING CHESS BACKEND..." << endl;
    
    httplib::Server svr;

    auto set_cors = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };

    svr.Options(".*", [&](const auto& req, auto& res) { 
        set_cors(res); 
        res.status = 204; 
    });

    // 1. Endpoint to get the board
// 1. Endpoint to get the current board
    svr.Get("/board", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        json response;
        response["board"] = board; 
        response["turn"] = (moves_till_now % 2 == 0) ? "white" : "black";
        
        // LINK THE ENGINE BRAIN: Call your actual game state function!
        response["state"] = check_game_state(); 
        
        res.set_content(response.dump(), "application/json");
    });

    // 2. Endpoint to get legal moves (THIS WAS MISSING!)
    svr.Get(R"(/moves/(\d+)/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        int x = stoi(req.matches[1]);
        int y = stoi(req.matches[2]);
        
        vector<vector<bool>> moves = get_legal_moves(x, y);
        json response;
        response["moves"] = moves;
        res.set_content(response.dump(), "application/json");
    });

    // 3. Endpoint to execute a move
    svr.Post("/move", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        try {
            auto body = json::parse(req.body);
            int from_x = body["from_x"];
            int from_y = body["from_y"];
            int to_x = body["to_x"];
            int to_y = body["to_y"];
            
            // THE FIX: Ultra-safe JSON type checking
            if (body.contains("promotion") && body["promotion"].is_string()) {
                string promo = body["promotion"].get<string>();
                promotion_choice = promo[0];
                cout << "--> RECEIVED PROMOTION CHOICE: " << promotion_choice << endl;
            } else {
                promotion_choice = 0;
            }
            
            execute_move(from_x, from_y, to_x, to_y);
            
            json response = {{"status", "success"}};
            res.set_content(response.dump(), "application/json");
        } catch (const exception& e) {
            cout << "--> JSON CRASH AVOIDED: " << e.what() << endl;
        }
    });
    // 4. Endpoint to restart the game
    svr.Post("/reset", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        reset_board(); // Resets the C++ engine memory
        
        json response = {{"status", "success"}};
        res.set_content(response.dump(), "application/json");
    });

    cout << "STARTING SERVER ON PORT 9090..." << endl;
    cout << "========================================" << endl;
    
    if (!svr.listen("0.0.0.0", 9090)) {
        cout << "CRITICAL ERROR: Port 9090 is blocked!" << endl;
    }
}