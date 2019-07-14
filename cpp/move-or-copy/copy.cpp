/*
 * Created by 李晨曦 on 2019-07-15.
 */

#include "structures.hpp"

auto set_a(A a) {}

auto get_a() {
    A a;
    return static_cast<A&>(a);
}

int main() {
    A a;
    set_a(a);
    get_a();
    auto copy_a = a;
    a = copy_a;
    return 0;
}