/*
 * Created by 李晨曦 on 2019-07-14.
 */

#include "structures.hpp"

int main() {
    B b(0);
    auto & ref_data = b.a().const_data();
    ref_data = 1;
    return 0;
}