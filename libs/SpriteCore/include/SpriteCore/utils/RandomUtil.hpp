#pragma once

#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

class RandomUtil {
public:
    /**
     * 根据指定的权重生成相应权重下的随机数
     *
     * @param weights 权重数组，每个元素代表相应类别的权重
     * @return 根据权重随机生成的索引
     */
    static int weightedRandomIndex(const std::vector<double>& weights) {
        // 计算累计分布函数 (CDF)
        std::vector<double> cdf(weights.size());
        std::partial_sum(weights.begin(), weights.end(), cdf.begin());

        // 生成 [0, 1) 范围内的随机数
        std::random_device rd; // 随机设备，用于生成随机数种子
        std::mt19937 gen(rd()); // 标准梅森旋转算法的随机数生成器
        std::uniform_real_distribution<> dis(0, 1); // 均匀分布 [0, 1)

        double randomValue = dis(gen); // 生成一个随机数

        // 根据随机值确定对应的权重索引
        auto it = std::lower_bound(cdf.begin(), cdf.end(), randomValue);
        return std::distance(cdf.begin(), it);
    }

    /**
     * 生成一个在指定范围内的符合标准正态分布的随机数
     *
     * @param mean 正态分布的均值
     * @param min 随机数的最小值
     * @param max 随机数的最大值
     * @return 符合正态分布且在指定范围内的随机数
     */
    static double normalDistributedRandom(double mean, double min, double max) {
        std::random_device rd; // 随机设备，用于生成随机数种子
        std::mt19937 gen(rd()); // 标准梅森旋转算法的随机数生成器

        std::normal_distribution<> d(mean, mean); // 标准正态分布生成器，均值为mean，标准差为mean

        double value;
        do {
            value = d(gen); // 生成一个正态分布的随机数
        } while (value < min || value > max); // 确保生成的随机数在指定范围内

        return value;
    }

    // 获取0-1之间的随机数
    static double random() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);
        return dis(gen);
    }

};
