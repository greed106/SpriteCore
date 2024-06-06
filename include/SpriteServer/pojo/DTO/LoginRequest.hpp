#pragma once

#include <nlohmann/json.hpp>
#include <utility>

class LoginRequest {
public:
    std::string username;
    std::string password;

    LoginRequest() = default;
    LoginRequest(std::string  username, std::string  password)
        : username(std::move(username)), password(std::move(password)) {}
    std::string getUsername() const {
        return username;
    }
    void setUsername(const std::string& username) {
        this->username = username;
    }
    std::string getPassword() const {
        return password;
    }
    void setPassword(const std::string& password) {
        this->password = password;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoginRequest, username, password)
};