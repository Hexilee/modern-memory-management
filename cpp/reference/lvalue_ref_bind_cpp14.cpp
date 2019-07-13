/*
 * Created by 李晨曦 on 2019-07-14.
 */

#include "structures.hpp"
#include <iostream>

int main() {
    A a(0);
    decltype(auto) data_ref = a.data();
    data_ref = 1;
    std::cout << "a._data = " << a.data() << std::endl;
    return 0;
}