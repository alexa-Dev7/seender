#include "uwebsockets/App.h"
#include "encrypt.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

using json = nlohmann::json;
using namespace std;

const string USERS_FILE = "users.json";
const string MESSAGES_FILE = "messages.json";
const string SESSIONS_FILE = "sessions.json";

// Load JSON from file
json load_json(const string& filename) {
    ifstream file(filename);
    json data;
    file >> data;
    return data;
}

// Save JSON to file
void save_json(const string& filename, const json& data) {
    ofstream file(filename);
    file << data.dump(4);
}

// Authenticate user
bool authenticate_user(const string& username, const string& password) {
    json users = load_json(USERS_FILE);
    return users.contains(username) && bcrypt::validatePassword(password, users[username]["password"]);
}

// Create new user
bool register_user(const string& username, const string& password) {
    json users = load_json(USERS_FILE);

    if (users.contains(username)) return false;

    users[username] = {{"password", bcrypt::generateHash(password)}};
    save_json(USERS_FILE, users);

    return true;
}

// Save message to JSON
void save_message(const string& sender, const string& recipient, const string& encrypted_msg) {
    json messages = load_json(MESSAGES_FILE);
    messages.push_back({{"sender", sender}, {"recipient", recipient}, {"message", encrypted_msg}});
    save_json(MESSAGES_FILE, messages);
}

// Track active user sessions
void track_session(const string& username, const string& session_id) {
    json sessions = load_json(SESSIONS_FILE);
    sessions[session_id] = username;
    save_json(SESSIONS_FILE, sessions);
}

// Short polling function
void start_server() {
    uWS::App().get("/messages", [](auto *res, auto *req) {
        string session_id = req->getQuery("session");

        json sessions = load_json(SESSIONS_FILE);
        if (!sessions.contains(session_id)) {
            res->writeHeader("Content-Type", "application/json")
                ->end(R"({"error": "Invalid session"})");
            return;
        }

        string username = sessions[session_id];
        json messages = load_json(MESSAGES_FILE);

        json user_messages = json::array();
        for (const auto& msg : messages) {
            if (msg["recipient"] == username || msg["sender"] == username) {
                user_messages.push_back(msg);
            }
        }

        res->writeHeader("Content-Type", "application/json")
            ->end(user_messages.dump());
    })
    .post("/send", [](auto *res, auto *req) {
        string body;
        req->onData([res, &body](std::string_view data, bool last) {
            body.append(data.data(), data.size());
            if (last) {
                json payload = json::parse(body);

                string sender = payload["sender"];
                string recipient = payload["recipient"];
                string message = payload["message"];

                string encrypted_msg = encrypt_message(message);

                save_message(sender, recipient, encrypted_msg);

                res->writeHeader("Content-Type", "application/json")
                    ->end(R"({"status": "Message sent"})");
            }
        });
    })
    .listen(8080, [](auto *token) {
        if (token) {
            cout << "Server running on port 8080..." << endl;
        } else {
            cout << "Failed to start server!" << endl;
        }
    }).run();
}

int main() {
    cout << "Starting chat server..." << endl;
    start_server();
    return 0;
}
