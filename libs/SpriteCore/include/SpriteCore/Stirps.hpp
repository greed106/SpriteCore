#pragma once

#include "Type.hpp"
#include <memory>
#include "Buff.hpp"
#include "utils/RandomUtil.hpp"

class Stirps {
protected:
    std::unique_ptr<Type> type;
    std::string stirpsName;
public:
    Stirps(Type* type, std::string stirpsName);
    virtual ~Stirps() = default;
    void showInfo() const;
    virtual Buff generateBuff() const = 0;
    virtual void enhance() = 0;
    virtual Stirps* clone() = 0;
    Type* getType() const;
};

// 火暴猴@力量型@15,6,60,5
class Primeape : public Stirps {
public:
    Primeape(int level = 1);
    Primeape(Type* type, std::string stirpsName);
    Buff generateBuff() const override;
    void enhance() override;
    Stirps* clone() override;
};
// 小火龙@速度型@10,6,50,8
class Charmander : public Stirps {
public:
    Charmander(int level = 1) : Stirps(new SpeedType(10, 6, 50, 8), "Charmander"){
        while(level > 1)
        {
            type->levelUp();
            level--;
        }
    }
    Charmander(Type* type, std::string stirpsName) : Stirps(type, stirpsName){}
    Buff generateBuff() const override;
    void enhance() override{type->levelUp();}
    Stirps* clone() override{return new Charmander(type->clone(), stirpsName);}
};
// 妙蛙种子@防御型@8,10,60,4
class Bulbasaur : public Stirps {
public:
    Bulbasaur(int level = 1) : Stirps(new DefenseType(8, 10, 60, 4), "Bulbasaur"){
        while(level > 1)
        {
            type->levelUp();
            level--;
        }
    }

    Bulbasaur(Type* type, std::string stirpsName) : Stirps(type, stirpsName){}
    Buff generateBuff() const override;
    void enhance() override {type->levelUp();}
    Stirps* clone() override {return new Bulbasaur(type->clone(), stirpsName);}
};
// 杰尼龟@肉盾型@9,12,70,3
class Squirtle : public Stirps {
public:
    Squirtle(int level = 1) : Stirps(new TankType(9, 12, 70, 3), "Squirtle"){
        while(level > 1)
        {
            type->levelUp();
            level--;
        }
    }
    Squirtle(Type* type, std::string stirpsName) : Stirps(type, stirpsName){}
    Buff generateBuff() const override;
    void enhance() override {type->levelUp();}
    Stirps* clone() override {return new Squirtle(type->clone(), stirpsName);}
};