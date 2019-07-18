/*
 * Created by 李晨曦 on 2019-07-18.
 */

#include "structures.hpp"

auto product(int data) {
    auto a = shared_ptr(new A(data));
    return static_cast<shared_ptr<A>&>(a);
}

auto consume(shared_ptr<A> a) {

}

int main() {
    auto a = product(1);
    auto a2 = a;
    consume(a2);
    a->say();
    return 0;
}