/*
 * Created by 李晨曦 on 2019-07-14.
 */

#include "structures.hpp"
#include <iostream>

int main() {
    A a(0);
    a.data() = 1; // equal to `a._data = 1;`
    std::cout << "a._data = " << a.data() << std::endl;
    return 0;
}