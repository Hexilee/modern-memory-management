/*
 * Created by 李晨曦 on 2019-07-14.
 */

#include "structures.hpp"

int main() {
    B b(0);
    auto & ref_a = b.a();
    ref_a = A(1);
    auto & ref_data = ref_a.data();
    return 0;
}