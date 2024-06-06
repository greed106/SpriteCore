#pragma once

#include <Poco/Data/Session.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>
#include <memory>
#include <string>
#include <stdexcept>

class Mapper {
protected:
    static std::shared_ptr<Poco::Data::SessionPool> pool;

    Mapper() {
        if (!pool) {
            throw std::runtime_error("Connection pool not initialized");
        }
    }

    virtual ~Mapper() = default;

public:
    static void initializePool(const std::string& connectionString,
                               size_t minPoolSize = 1, size_t maxPoolSize = 10,
                               size_t idlePoolSize = 5)
    {
        Poco::Data::MySQL::Connector::registerConnector();
        pool = std::make_shared<Poco::Data::SessionPool>(
            Poco::Data::MySQL::Connector::KEY,
            connectionString, minPoolSize,
            maxPoolSize, idlePoolSize);
    }

    static std::shared_ptr<Poco::Data::SessionPool> getPool() {
        return pool;
    }

    // 禁止复制构造和赋值操作
    Mapper(const Mapper&) = delete;
    Mapper& operator=(const Mapper&) = delete;
};

std::shared_ptr<Poco::Data::SessionPool> Mapper::pool = nullptr;
