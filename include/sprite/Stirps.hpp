#pragma once

#include "Type.hpp"
#include <memory>

class Stirps {
protected:
    std::unique_ptr<Type> type;
    std::string stirpsName;
public:
    Stirps(Type* type, std::string stirpsName);
    virtual ~Stirps() = default;
    void showInfo() const;
    bool hurt(int damage);
    virtual int doAttack() const = 0;
    virtual void enhance() = 0;
    virtual Stirps* clone() = 0;
};

// 火暴猴@力量型
class Primeape : public Stirps {
public:
    Primeape(int level = 1);
    Primeape(Type* type, std::string stirpsName);
    int doAttack() const override;
    void enhance() override;
    Stirps* clone() override;
};