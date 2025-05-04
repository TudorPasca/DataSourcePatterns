#pragma once

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include "Database.h"

class User {
private:
    inline static Database *db = nullptr;

    std::optional<long long> id;
    std::string username;
    std::string email;

    bool insert() {
        std::string query = "INSERT INTO users (username, email) VALUES ('" + username + "', '" + email + "');";
        long long new_id = db->executeInsert(query);
        if (new_id > 0) {
            this->id = new_id;
            return true;
        }
        return false;
    }

    bool update() {
        if (!id.has_value()) {
            std::cerr << "[User] Error: Cannot update user without an ID." << std::endl;
            return false;
        }
        std::string query = "UPDATE users SET username = '" + username
                            + "', email = '" + email
                            + "' WHERE id = " + std::to_string(id.value()) + ";";

        int affected_rows = db->executeUpdateOrDelete(query);
        return affected_rows > 0;
    }

    User(long long id, std::string username, std::string email)
            : id(id), username(std::move(username)), email(std::move(email)) {}

public:
    static void connectDatabase(Database *db_) {
        db = db_;
        std::cout << "[User] Database driver assigned." << std::endl;
    }

    static void disconnectDatabase() {
        db = nullptr;
        std::cout << "[User] Database driver disconnected/unassigned." << std::endl;
    }

    User(std::string username, std::string email)
            : id(std::nullopt), username(std::move(username)), email(std::move(email)) {}

    std::optional<long long> getId() const { return id; }

    const std::string &getUsername() const { return username; }

    const std::string &getEmail() const { return email; }

    bool isPersisted() const { return id.has_value(); }

    void setUsername(std::string username) { this->username = std::move(username); }

    void setEmail(std::string email) { this->email = std::move(email); }

    bool save() {
        if (!isPersisted()) {
            return insert();
        } else {
            return update();
        }
    }

    bool remove() {
        if (!isPersisted()) {
            std::cerr << "[User] Error: Cannot remove non-persisted User." << std::endl;
            return false;
        }
        std::string query = "DELETE FROM users WHERE id = " + std::to_string(id.value()) + ";";
        int affected_rows = db->executeUpdateOrDelete(query);
        return affected_rows > 0;
    }

    static std::optional<User> findById(long long id) {
        std::string query = "SELECT id, username, email FROM users WHERE id = " + std::to_string(id) + ";";
        auto results = db->executeSelect(query);
        if (!results.empty()) {
            const auto &row = results[0];
            long long found_id = std::stoll(row[0]);
            return User(found_id, row[1], row[2]);
        }
        return std::nullopt;
    }
};
