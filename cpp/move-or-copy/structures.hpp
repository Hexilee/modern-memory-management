/*
 * Created by 李晨曦 on 2019-07-14.
 */

#ifndef CPP_STRUCTURES_HPP
#define CPP_STRUCTURES_HPP
#include <iostream>

class A {
  public:
    A() = default;
    
    A(const A &) {
        std::cout << "call copy constructor" << std::endl;
    }
    
    A(A &&) noexcept {
        std::cout << "call move constructor" << std::endl;
    }
    
    auto operator=(const A &) -> A & {
        std::cout << "call copy operator=" << std::endl;
        return *this;
    }
    
    auto operator=(A &&) noexcept -> A & {
        std::cout << "call move operator=" << std::endl;
        return *this;
    }
};

#endif //CPP_STRUCTURES_HPP
