/*
 * Created by 李晨曦 on 2019-07-15.
 */

#include "structures.hpp"

int main() {
    auto v = Vector<int>(10);
    auto& ref_v = v;
    std::cout << ref_v.first() << std::endl;
    auto moved = std::move(v);
    std::cout << ref_v.first() << std::endl;
}