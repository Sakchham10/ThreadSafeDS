//
// Created by Sakchham Sangroula on 6/30/26.
//

#include "utils.hpp"
#include <random>

int utils::getRandom(int start, int end) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution distrib(start, end);
        int random_value = distrib(gen);
        return random_value;
}