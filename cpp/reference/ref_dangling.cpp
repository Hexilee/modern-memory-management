/*
 * Created by 李晨曦 on 2019-07-14.
 */

#include "structures.hpp"

decltype(auto) get_data() {
    return C(0).id();
}

int main() {
    decltype(auto) ref_data = get_data();
    std::cout << "id=" << ref_data << std::endl;
}