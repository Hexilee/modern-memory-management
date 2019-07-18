/*
 * Created by 李晨曦 on 2019-07-18.
 */

#include "structures.hpp"

auto get_a(int data) {
    auto a = unique_ptr(new A(data));
    return std::move(a);
}

int main() {
    auto a = get_a(1);
    auto a2 = std::move(a);
    a2->say();
    return 0;
}