/*
 * Created by 李晨曦 on 2019-07-15.
 */

#include "structures.hpp"

auto set_a(A a) {}
auto copy_ref(A&& a) {
    A _copy(a);
}
auto move_ref(A&& a) {
    A _move(std::forward<A>(a));
}
int main() {
    A a;
    set_a(std::move(a));
    set_a(static_cast<A&&>(A()));
    copy_ref(A());
    move_ref(A());
    return 0;
}