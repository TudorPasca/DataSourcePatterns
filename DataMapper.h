#pragma once

#include <optional>
#include <string>
#include <vector>
#include "Database.h"

class User {
private:
    friend class UserMapper;

    std::optional<long long> id;
    std::string username;
    std::string email;

    void setId(long long id) {
        this->id = id;
    }

public:
    User(std::string username, std::string email)
            : id(std::nullopt),
              username(std::move(username)),
              email(std::move(email)) {}

    User(long long id, std::string username, std::string email)
            : id(id),
              username(std::move(username)),
              email(std::move(email)) {}

    std::optional<long long> getId() const { return id; }
    const std::string& getUsername() const { return username; }
    const std::string& getEmail() const { return email; }
    void setUsername(std::string username) { this->username = std::move(username); }
    void setEmail(std::string email) { this->email = std::move(email); }
};

class UserMapper {
private:
    Database* db;
public:
    explicit UserMapper(Database* db) : db(db) {
        std::cout << "[UserMapper] Initialized." << std::endl;
    }

    std::optional<User> findById(long long id) {
        std::string query = "SELECT id, username, email FROM users WHERE id = " + std::to_string(id) + ";";
        auto results = db->executeSelect(query);
        if (!results.empty()) {
            const auto &row = results[0];
            long long found_id = std::stoll(row[0]);
            return User(found_id, row[1], row[2]);
        }
        return std::nullopt;
    }

    bool insert(User& user) {
        if (user.getId().has_value()) {
            std::cerr << "[UserMapper] Error: Cannot insert a user that already has an ID." << std::endl;
            return false;
        }
        std::string query = "INSERT INTO users (username, email) VALUES ('" + user.getUsername() + "', '" + user.getEmail() + "');";
        long long new_id = db->executeInsert(query);
        if (new_id > 0) {
            user.setId(new_id);
            return true;
        }
        return false;
    }

    bool update(const User& user) {
        if (!user.getId().has_value()) {
            std::cerr << "[UserMapper] Error: Cannot update a user without an ID." << std::endl;
            return false;
        }
        std::string query = "UPDATE users SET username = '" + user.getUsername()
                            + "', email = '" + user.getEmail()
                            + "' WHERE id = " + std::to_string(user.getId().value()) + ";";
        int affected_rows = db->executeUpdateOrDelete(query);
        return affected_rows > 0;
    }

    bool remove(const User& user) {
        if (!user.getId().has_value()) {
            std::cerr << "[UserMapper] Error: Cannot remove a user without an ID." << std::endl;
            return false;
        }
        std::string query = "DELETE FROM users WHERE id = " + std::to_string(user.getId().value()) + ";";
        int affected_rows = db->executeUpdateOrDelete(query);
        return affected_rows > 0;
    }
};