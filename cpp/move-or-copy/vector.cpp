/*
 * Created by 李晨曦 on 2019-07-15.
 */
#include "structures.hpp"

auto product_vector() {
    auto v = Vector<int>(10);
    return std::move(v);
}

auto consume_vector(Vector<int> _v) {

}

int main() {
    auto v = product_vector();
    consume_vector(std::move(v));
    return 0;
}