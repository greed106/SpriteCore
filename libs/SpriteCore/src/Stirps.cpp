#include "SpriteCore/Stirps.hpp"
#include "SpriteCore/Factory.hpp"

namespace {
    static Register<Primeape> registerPrimeape;
    static Register<Charmander> registerCharmander;
    static Register<Bulbasaur> registerBulbasaur;
    static Register<Squirtle> registerSquirtle;
}

Primeape::Primeape(int level) : 
    Stirps(new PowerType(15, 6, 60, 5), "Primeape")
{
    while(level > 1)
    {
        type->levelUp();
        level--;
    }
}

Buff Primeape::generateBuff() const
{
    // 选择攻击/防御/速度Buff的权重
    std::vector<double> weights = {0.5, 0.3, 0.2};
    int index = RandomUtil::weightedRandomIndex(weights);
    switch(index)
    {
        case 0:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 2);
            int increase = std::round(RandomUtil::normalDistributedRandom(5, 0, 100));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, rate, 0, 0, increase);
        }
        case 1:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.5);
            int increase = std::round(RandomUtil::normalDistributedRandom(4, 0, 10));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, 0, rate, 0, 0, increase);
        }
        case 2:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.3);
            int increase = std::round(RandomUtil::normalDistributedRandom(2, 0, 5));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, 0, 0, rate, 0, 0, increase);
        }
    }


}

void Primeape::enhance()
{
    type->levelUp();
}

Stirps::Stirps(Type *type, std::string stirpsName) : type(type), stirpsName(stirpsName)
{
}

void Stirps::showInfo() const
{
    std::cout << "Stirps: " << stirpsName << std::endl;
    type->showInfo();
}

Type *Stirps::getType() const {
    return type->clone();
}

Stirps* Primeape::clone()
{
    return new Primeape(type->clone(), stirpsName);
}

Primeape::Primeape(Type *type, std::string stirpsName) : Stirps(type, stirpsName)
{
}

Buff Charmander::generateBuff() const {
    // 选择攻击/防御/速度Buff的权重
    std::vector<double> weights = {0.3, 0.3, 0.4};
    int index = RandomUtil::weightedRandomIndex(weights);
    switch(index)
    {
        case 0:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 2);
            int increase = std::round(RandomUtil::normalDistributedRandom(4, 0, 100));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, rate, 0, 0, increase);
        }
        case 1:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.5);
            int increase = std::round(RandomUtil::normalDistributedRandom(4, 0, 10));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, 0, rate, 0, 0, increase);
        }
        case 2:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.3);
            int increase = std::round(RandomUtil::normalDistributedRandom(3, 0, 6));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, 0, 0, rate, 0, 0, increase);
        }
    }
}

Buff Bulbasaur::generateBuff() const {
    // 选择攻击/防御/速度Buff的权重
    std::vector<double> weights = {0.3, 0.5, 0.2};
    int index = RandomUtil::weightedRandomIndex(weights);
    switch(index)
    {
        case 0:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 2);
            int increase = std::round(RandomUtil::normalDistributedRandom(4, 0, 100));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, rate, 0, 0, increase);
        }
        case 1:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.5);
            int increase = std::round(RandomUtil::normalDistributedRandom(6, 0, 20));
            int duration = std::round(RandomUtil::normalDistributedRandom(2, 0, 4));

            return Buff(duration, 0, rate, 0, 0, increase);
        }
        case 2:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.3);
            int increase = std::round(RandomUtil::normalDistributedRandom(2, 0, 5));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, 0, 0, rate, 0, 0, increase);
        }
    }
}

Buff Squirtle::generateBuff() const {
    // 选择攻击/防御/速度Buff的权重
    std::vector<double> weights = {0.4, 0.4, 0.2};
    int index = RandomUtil::weightedRandomIndex(weights);
    switch(index)
    {
        case 0:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 2);
            int increase = std::round(RandomUtil::normalDistributedRandom(5, 0, 100));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, rate, 0, 0, increase);
        }
        case 1:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.5);
            int increase = std::round(RandomUtil::normalDistributedRandom(4, 0, 10));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, 0, rate, 0, 0, increase);
        }
        case 2:
        {
            double rate = RandomUtil::normalDistributedRandom(0.1, 0, 0.3);
            int increase = std::round(RandomUtil::normalDistributedRandom(2, 0, 5));
            int duration = std::round(RandomUtil::normalDistributedRandom(1, 0, 3));

            return Buff(duration, 0, 0, rate, 0, 0, increase);
        }
    }
}
