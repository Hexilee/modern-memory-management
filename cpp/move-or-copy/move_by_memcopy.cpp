/*
 * Created by 李晨曦 on 2019-07-15.
 */

#include "structures.hpp"

auto get_b() {
    auto b = B(1);
    return std::move(b);
}

int main() {
    auto b = get_b();
    return 0;
}