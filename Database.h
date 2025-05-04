#pragma once

#include <vector>
#include <string>
#include <iostream>

class Database {
private:
    bool connected_ = false;
    long long next_id_ = 1;

public:
    void connect() {
        std::cout << "[DB] Connecting to DB" << std::endl;
    }

    void disconnect() {
        std::cout << "[DB] Disconnecting from DB" << std::endl;
    }

    long long executeInsert(const std::string& query) {
        std::cout << "[DB] Executing Insert: " << query << std::endl;
        long long current_id = next_id_++;
        return current_id;
    }

    int executeUpdateOrDelete(const std::string& query) {
        std::cout << "[DB] Executing Update/Delete: " << query << std::endl;
        return 0;
    }

    std::vector<std::vector<std::string>> executeSelect(const std::string& query) {
        std::cout << "[DB] Executing Select: " << query << std::endl;
        return {};
    }


};
